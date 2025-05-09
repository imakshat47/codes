/// g++ -std=c++17 -O3 -Wall -Wextra -fopenmp rx.cpp -lpthread -o rx && ./rx

#include "side_channel_params.hpp"
#include <cstdio>
#include <sstream>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <optional>
#include <cassert>
#include <tuple>
#include <variant>
#include <cmath>
#include <atomic>
#include <chrono>
#include <omp.h>

static constexpr auto OversamplingFactor = 3;
static constexpr auto SampleDuration = side_channel::params::ChipPeriod / OversamplingFactor;
static constexpr auto PHYAveragingFactor = 8;
static constexpr auto DiagnosticsInterval = 1000;  // Print diagnostics every N bits

/// Single-pass mean and standard deviation calculation
template <typename S>
inline std::pair<S, S> computeMeanStdev(const std::vector<S>& cvec) noexcept {
    if (cvec.empty()) return {S(0), S(0)};
    
    S sum = 0;
    S sum_sq = 0;
    for (const auto e : cvec) {
        sum += e;
        sum_sq += e * e;
    }
    const S mean = sum / static_cast<S>(cvec.size());
    const S variance = (sum_sq / static_cast<S>(cvec.size())) - (mean * mean);
    return {mean, std::sqrt(std::max(variance, S(0)))};
}

/// Optimized PHY reading with single-threaded timing loop
static bool readPHY() noexcept {
    static auto deadline = std::chrono::steady_clock::now();
    deadline += SampleDuration;
    const auto started_at = std::chrono::steady_clock::now();

    volatile uint64_t counter = 0;  // Prevent loop optimization
    while (std::chrono::steady_clock::now() < deadline) {
        ++counter;
    }

    const double elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now() - started_at
    ).count();
    const double rate = static_cast<double>(counter) / elapsed_ns;

    static std::atomic<double> rate_average{rate};
    rate_average.store(rate_average.load() + (rate - rate_average.load()) / PHYAveragingFactor);

    return rate < rate_average.load();
}

/// Optimized correlation channel with reference to shared spread code
class CorrelationChannel {
public:
    CorrelationChannel(const std::vector<char>& spread_code, const uint32_t offset) noexcept
        : spread_code_(spread_code), position_(offset) {}

    struct Result {
        float correlation = 0.0f;
        bool data;
        bool clock;
    };

    Result feed(const bool sample) noexcept {
        if (position_ >= spread_code_.size()) {
            updateCorrelation();
            state_ = match_hi_ > match_lo_;
            position_ = 0;
            match_hi_ = 0;
            match_lo_ = 0;
        }

        if (sample == spread_code_[position_]) {
            ++match_hi_;
        } else {
            ++match_lo_;
        }
        ++position_;

        return {correlation_, state_, position_ > spread_code_.size() / 2};
    }

    float getCorrelation() const noexcept { return correlation_; }

private:
    void updateCorrelation() noexcept {
        const bool hi_top = match_hi_ > match_lo_;
        const auto top = hi_top ? match_hi_ : match_lo_;
        const auto bot = hi_top ? match_lo_ : match_hi_;
        correlation_ = static_cast<float>(top - bot) / position_;
    }

    const std::vector<char>& spread_code_;
    uint32_t position_;
    uint32_t match_hi_ = 0;
    uint32_t match_lo_ = 0;
    float correlation_ = 0.0f;
    bool state_ = false;
};

class Correlator {
    static constexpr auto SequenceLength = side_channel::params::CDMACodeLength * OversamplingFactor;
    static std::vector<char> spread_code_seq_;  // Shared spread code

public:
    struct Result {
        float data = 0.0f;
        float clock = 0.0f;
    };

   // In Correlator class constructor:
Correlator() {
    using side_channel::params::CDMACode;
    if (spread_code_seq_.empty()) {
        spread_code_seq_.reserve(CDMACode.size() * OversamplingFactor);
        // Iterate through bitset using index-based access
        for (size_t i = 0; i < CDMACode.size(); ++i) {
            const bool code_bit = CDMACode[i];  // Access bitset by index
            for (auto j = 0U; j < OversamplingFactor; j++) {
                spread_code_seq_.push_back(static_cast<char>(code_bit));
            }
        }
    }
        channels_.reserve(SequenceLength);
        for (uint32_t i = 0; i < SequenceLength; ++i) {
            channels_.emplace_back(spread_code_seq_, i);
        }
    }

    Result feed(const bool sample) noexcept {
        float data = 0.0f;
        float clock = 0.0f;

        #pragma omp parallel for reduction(+:data, clock)
        for (size_t i = 0; i < channels_.size(); ++i) {
            const auto res = channels_[i].feed(sample);
            const float weight = res.correlation * res.correlation * res.correlation * res.correlation;
            data += res.data ? weight : -weight;
            clock += res.clock ? weight : -weight;
        }

        return {data, clock};
    }

    std::vector<float> getCorrelationVector() const noexcept {
        std::vector<float> out;
        out.reserve(channels_.size());
        for (const auto& ch : channels_) {
            out.push_back(ch.getCorrelation());
        }
        return out;
    }

    bool isCodePhaseSynchronized(const float stdev_multiple_threshold = 5.0f) const noexcept {
        const auto cvec = getCorrelationVector();
        const auto [mean, stdev] = computeMeanStdev(cvec);
        const auto max = *std::max_element(cvec.begin(), cvec.end());
        return (max - mean) > (stdev * stdev_multiple_threshold);
    }

private:
    std::vector<CorrelationChannel> channels_;
};

std::vector<char> Correlator::spread_code_seq_{};

class BitReader {
public:
    bool next() noexcept {
        for (;;) {
            const bool phy_state = readPHY();
            const auto result = correlator_.feed(phy_state);

            if (!clock_latch_ && result.clock > 0.0f) {
                clock_latch_ = true;
                return result.data > 0.0f;
            }

            if (clock_latch_ && result.clock < 0.0f) {
                clock_latch_ = false;
            }
        }
    }

    void printDiagnostics() noexcept {
        static int counter = 0;
        if (++counter % DiagnosticsInterval != 0) return;

        const auto cvec = correlator_.getCorrelationVector();
        const auto [mean, stdev] = computeMeanStdev(cvec);
        std::printf("mean=%.2f max=%.2f stdev=%.2f lock=%d | ",
                    mean,
                    *std::max_element(cvec.begin(), cvec.end()),
                    stdev,
                    correlator_.isCodePhaseSynchronized());
        std::fflush(stdout);
    }

private:
    Correlator correlator_;
    bool clock_latch_ = false;
};

class SymbolReader {
public:
    struct Delimiter {};
    using Symbol = std::variant<Delimiter, uint8_t>;

    Symbol next() noexcept {
        while (true) {
            const bool bit = bit_reader_.next();
            bit_reader_.printDiagnostics();

            if (remaining_bits_ >= 0) {
                buffer_ = (buffer_ << 1U) | bit;
                if (--remaining_bits_ < 0) {
                    return Symbol{buffer_};
                }
            } else if (bit) {
                consecutive_zeros_ = 0;
                remaining_bits_ = 7;
                buffer_ = 0;
            } else {
                if (++consecutive_zeros_ > 8) {
                    remaining_bits_ = -1;
                    return Symbol{Delimiter{}};
                }
            }
        }
    }

private:
    BitReader bit_reader_;
    uint64_t consecutive_zeros_ = 0;
    uint8_t buffer_ = 0;
    int8_t remaining_bits_ = -1;
};

class PacketReader {
public:
    std::vector<uint8_t> next() noexcept {
        while (true) {
            const auto sym = symbol_reader_.next();
            if (const auto ret = std::visit(assembler_, sym)) {
                return *ret;
            }
        }
    }

private:
    class FrameAssembler {
    public:
        std::optional<std::vector<uint8_t>> operator()(const SymbolReader::Delimiter&) noexcept {
            std::optional<std::vector<uint8_t>> result;
            if (buffer_.size() >= 2) {
                uint16_t crc = side_channel::CRCInitial;
                for (uint8_t v : buffer_) crc = side_channel::crcAdd(crc, v);
                if (crc == 0) {
                    buffer_.resize(buffer_.size() - 2);
                    result = std::move(buffer_);
                }
            }
            buffer_.clear();
            return result;
        }

        std::optional<std::vector<uint8_t>> operator()(const uint8_t data) noexcept {
            buffer_.push_back(data);
            return {};
        }

    private:
        std::vector<uint8_t> buffer_;
    };

    SymbolReader symbol_reader_;
    FrameAssembler assembler_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cout << "SPREAD CODE LENGTH: " << side_channel::params::CDMACodeLength << " bit\n"
              << "SPREAD CHIP PERIOD: " << side_channel::params::ChipPeriod.count() * 1e-6 << " ms\n";
    side_channel::initThread();

    PacketReader reader;
    while (true) {
        const auto packet = reader.next();

        const auto file_name = std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + ".bin";
        if (std::ofstream out(file_name, std::ios::binary); out) {
            out.write(reinterpret_cast<const char*>(packet.data()), packet.size());
            std::printf("\033[91mReceived %zu bytes saved to %s\033[m\n", packet.size(), file_name.c_str());
        } else {
            std::cerr << "Error writing to " << file_name << '\n';
            return EXIT_FAILURE;
        }
    }
}