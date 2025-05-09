#include <bitset>
#include <chrono>
#include <vector>
#include <thread>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <variant>
#include <optional>
#include <cstdint>

#define MAX_CONCURRENCY 2

// Stub for side_channel_params.hpp
namespace side_channel {
    namespace params {
        using ChipPeriod = std::chrono::nanoseconds;
        static constexpr size_t CDMACodeLength = 1023;
        static constexpr std::bitset<CDMACodeLength> CDMACode{ /* ... */ };
        static constexpr uint16_t CRCInitial = 0xFFFF;
    }
    inline uint16_t crcAdd(uint16_t crc, uint8_t v) { return crc; }
}

static constexpr auto OversamplingFactor    = 3;
static constexpr auto SampleDuration        = side_channel::params::ChipPeriod{} / OversamplingFactor;
static constexpr auto PHYAveragingFactor    = 8;

// Thread-local buffers for readPHY
static thread_local std::vector<int64_t> phyCounters;
static thread_local std::vector<std::thread> phyPool;

// One-pass Welford mean/stddev
template <typename S>
inline std::pair<S, S> computeMeanStdev(const std::vector<S>& v) {
    S mean = 0, m2 = 0;
    size_t n = 0;
    for (auto x : v) {
        ++n;
        S delta = x - mean;
        mean += delta / n;
        m2   += delta * (x - mean);
    }
    return { mean, std::sqrt(m2 / n) };
}

// readPHY with reuse of buffers
static bool readPHY() {
    static auto deadline = std::chrono::steady_clock::now();
    deadline += SampleDuration;
    auto start = std::chrono::steady_clock::now();

    unsigned tc = std::max<unsigned>(1,
        std::min<unsigned>(MAX_CONCURRENCY, std::thread::hardware_concurrency()));

    if (phyCounters.size() < tc) {
        phyCounters.assign(tc, 0);
        phyPool.reserve(tc);
    }
    phyPool.clear();

    auto loop = [&](unsigned i){
        auto& cnt = phyCounters[i];
        while (std::chrono::steady_clock::now() < deadline) ++cnt;
    };

    if (tc > 1) {
        for (unsigned i = 0; i < tc; ++i) phyPool.emplace_back(loop, i);
        for (auto& t : phyPool) t.join();
    } else {
        phyCounters[0] = 0;
        loop(0);
    }

    double dt   = std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::steady_clock::now() - start).count();
    double rate = std::accumulate(phyCounters.begin(), phyCounters.begin()+tc, int64_t(0)) / dt;

    static double avg = rate;
    avg += (rate - avg) / PHYAveragingFactor;
    return rate < avg;
}

// Single correlation channel
class CorrelationChannel {
public:
    struct Result { float correlation; bool data, clock; };
    CorrelationChannel(const std::vector<bool>& code, uint32_t offset): code_(code), pos_(offset) {}

    Result feed(bool sample) {
        if (pos_ >= code_.size()) {
            update(); state_ = hi_ > lo_; pos_ = hi_ = lo_ = 0;
        }
        if (sample == code_[pos_]) ++hi_; else ++lo_;
        ++pos_;
        return { corr_, state_, pos_ > code_.size()/2 };
    }
private:
    void update() {
        auto top = std::max(hi_, lo_);
        auto bot = std::min(hi_, lo_);
        corr_ = float(top - bot) / float(pos_);
    }
    const std::vector<bool> code_;
    uint32_t pos_, hi_ = 0, lo_ = 0;
    float corr_ = 0;
    bool state_ = false;
};

// Multi-channel correlator
class Correlator {
    static constexpr size_t SeqLen = side_channel::params::CDMACodeLength * OversamplingFactor;
    std::vector<CorrelationChannel> channels_;
public:
    struct Result { float data, clock; };
    Correlator() {
        using namespace side_channel::params;
        std::vector<bool> seq;
        seq.reserve(CDMACodeLength * OversamplingFactor);
        for (size_t i = 0; i < CDMACodeLength; ++i) {
            bool bit = CDMACode[i];
            for (int j = 0; j < OversamplingFactor; ++j) seq.push_back(bit);
        }
        channels_.reserve(SeqLen);
        for (uint32_t off = 0; off < SeqLen; ++off) channels_.emplace_back(seq, off);
    }
    Result feed(bool sample) {
        float data = 0, clock = 0;
        for (auto& ch : channels_) {
            auto r = ch.feed(sample);
            float w = std::pow(r.correlation, 4.0F);
            data  += r.data  ?  w : -w;
            clock += r.clock ?  w : -w;
        }
        return { data, clock };
    }
    bool isCodePhaseLocked(float thr = 5.0F) const {
        auto vec = [&]{ std::vector<float> v; v.reserve(channels_.size());
            for (auto& ch : channels_) v.push_back(ch.feed(readPHY()).correlation);
            return v; }();
        auto [m, s] = computeMeanStdev(vec);
        return (*std::max_element(vec.begin(), vec.end()) - m) > thr * s;
    }
};

// Reads bits
class BitReader {
    Correlator corr_;
    bool latch_ = false;
public:
    bool next() {
        while (true) {
            bool phy = readPHY();
            auto r = corr_.feed(phy);
            if (!latch_ && r.clock > 0) { latch_ = true; return r.data > 0; }
            if (latch_ && r.clock < 0) latch_ = false;
        }
    }
};

// Reads symbols
class SymbolReader {
    BitReader br_;
    uint64_t zeros_ = 0;
    int8_t rem_ = -1;
    uint8_t buf_ = 0;
public:
    struct Delimiter {};
    using Symbol = std::variant<Delimiter, uint8_t>;
    Symbol next() {
        while (true) {
            bool bit = br_.next();
            if (rem_ >= 0) {
                buf_ = (buf_ << 1) | bit; if (--rem_ < 0) return buf_;
            } else if (bit) { zeros_ = 0; rem_ = 7; buf_ = 0; }
            else if (++zeros_ > 8) return Delimiter{};
        }
    }
};

// Packet assembler
class PacketReader {
    SymbolReader sr_;
    struct Assembler {
        std::vector<uint8_t> buf;
        std::optional<std::vector<uint8_t>> operator()(SymbolReader::Delimiter) {
            if (buf.size() >= 2) {
                uint16_t crc = side_channel::params::CRCInitial;
                for (auto b : buf) crc = side_channel::crcAdd(crc, b);
                if (crc == 0) {
                    buf.pop_back(); buf.pop_back();
                    return buf;
                }
            }
            buf.clear(); return {};
        }
        std::optional<std::vector<uint8_t>> operator()(uint8_t d) {
            buf.push_back(d); return {};
        }
    } asm_;
public:
    std::vector<uint8_t> next() {
        while (true) {
            auto sym = sr_.next();
            if (auto opt = std::visit(asm_, sym)) return *opt;
        }
    }
};

int main() {
    std::ios::sync_with_stdio(false); std::cin.tie(nullptr);
    std::cout << "CDMA Receiver Starting...\n";
    PacketReader pr;
    while (true) {
        auto packet = pr.next();
        std::cout << "Received packet (" << packet.size() << " bytes)\n";
    }
    return 0;
}
