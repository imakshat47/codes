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
    // Compute lock status without modifying channel state
    bool isCodePhaseLocked(float thr = 5.0F) const {
        auto vec = getCorrVec();
        auto [m, s] = computeMeanStdev(vec);
        float mx = *std::max_element(vec.begin(), vec.end());
        return (mx - m) > thr * s;
    }
    // Retrieve per-channel correlation metrics
    std::vector<float> getCorrVec() const {
        std::vector<float> v;
        v.reserve(channels_.size());
        for (const auto& ch : channels_) {
            v.push_back(ch.getCorrelation());
        }
        return v;
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
