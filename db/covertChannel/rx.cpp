/// g++ -std=c++17 -O2 -Wall rx.cpp -lpthread -o rx && ./rx

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
#include <chrono>
#include <iomanip>

// ANSI color codes
#define RX_COLOR "\033[1;34m"
#define ERROR_COLOR "\033[1;31m"
#define RESET_COLOR "\033[0m"

static constexpr auto OversamplingFactor = 3;
static constexpr auto SampleDuration = side_channel::params::ChipPeriod / OversamplingFactor;
static constexpr auto PHYAveragingFactor = 8;

// Get timestamp like [2023-10-01 14:30:00]
std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time), "[%Y-%m-%d %H:%M:%S]");
    return ss.str();
}

// Hex dump helper
void hexDump(const std::vector<uint8_t>& data) {
    std::cout << RX_COLOR << "  Hex Dump:" << RESET_COLOR << "\n";
    for (size_t i = 0; i < data.size(); ++i) {
        if (i % 16 == 0) {
            std::cout << RX_COLOR << "  " << std::hex << std::setw(4) 
                      << std::setfill('0') << i << ": " << RESET_COLOR;
        }
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(data[i]) << " ";
        if ((i + 1) % 16 == 0 || i == data.size() - 1) {
            std::cout << "\n";
        }
    }
    std::cout << std::dec;
}

template <typename S>
inline std::pair<S, S> computeMeanStdev(const std::vector<S>& cvec) {
    const auto mean = std::accumulate(std::begin(cvec), std::end(cvec), 0.0F) / cvec.size();
    auto variance = S{};
    for (auto e : cvec) {
        variance += std::pow(e - mean, 2) / cvec.size();
    }
    return {mean, std::sqrt(variance)};
}

static bool readPHY() {
    static auto deadline = std::chrono::steady_clock::now();
    deadline += SampleDuration;
    const auto started_at = std::chrono::steady_clock::now();

    std::vector<std::int64_t> counters;
    const auto loop = [&counters](std::uint32_t index) {
        auto& cnt = counters.at(index);
        while (std::chrono::steady_clock::now() < deadline) {
            cnt++;
        }
    };

    static const auto thread_count = std::max<unsigned>(1, std::min<unsigned>(MAX_CONCURRENCY,
                                                                              std::thread::hardware_concurrency()));
    if (thread_count > 1U) {
        counters.resize(thread_count, 0);
        std::vector<std::thread> pool;
        for (auto i = 0U; i < thread_count; i++) {
            pool.emplace_back(loop, i);
        }
        for (auto& t : pool) {
            t.join();
        }
    } else {
        counters.push_back(0);
        loop(0);
    }

    const double elapsed_ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - started_at).count();
    const double rate = double(std::accumulate(std::begin(counters), std::end(counters), 0)) / elapsed_ns;

    static double rate_average = rate;
    rate_average += (rate - rate_average) / PHYAveragingFactor;

    return rate < rate_average;
}

class CorrelationChannel {
public:
    struct Result {
        float correlation = 0.0F;
        bool data;
        bool clock;
    };

    CorrelationChannel(std::vector<bool> spread_code, const std::uint32_t offset) :
        spread_code_(spread_code),
        position_(offset)
    { }

    Result feed(const bool sample) {
        std::optional<bool> received;
        if (position_ >= spread_code_.size()) {
            updateCorrelation();
            state_ = match_hi_ > match_lo_;
            position_ = 0;
            match_hi_ = 0;
            match_lo_ = 0;
        }
        if (sample == spread_code_.at(position_)) {
            match_hi_++;
        } else {
            match_lo_++;
        }
        position_++;
        return {
            correlation_,
            state_,
            position_ > spread_code_.size() / 2
        };
    }

    float getCorrelation() const { return correlation_; }

private:
    void updateCorrelation() {
        const bool hi_top = match_hi_ > match_lo_;
        const auto top = hi_top ? match_hi_ : match_lo_;
        const auto bot = hi_top ? match_lo_ : match_hi_;
        assert(top >= bot);
        assert(position_ > 0);
        correlation_ = static_cast<float>(top - bot) / static_cast<float>(position_);
    }

    std::vector<bool> spread_code_;
    std::uint32_t position_;
    std::uint32_t match_hi_ = 0;
    std::uint32_t match_lo_ = 0;
    float correlation_ = 0.0F;
    bool state_ = false;
};

class Correlator {
    static constexpr auto SequenceLength = side_channel::params::CDMACodeLength * OversamplingFactor;

public:
    Correlator() {
        using side_channel::params::CDMACode;
        std::vector<bool> seq;
        for (auto i = 0U; i < CDMACode.size(); i++) {
            for (auto j = 0U; j < OversamplingFactor; j++) {
                seq.push_back(CDMACode[i]);
            }
        }
        for (std::uint32_t i = 0; i < SequenceLength; i++) {
            channels_.emplace_back(seq, i);
        }
    }

    struct Result {
        float data  = 0.0F;
        float clock = 0.0F;
    };

    Result feed(const bool sample) {
        float data = 0.0F;
        float clock = 0.0F;
        for (auto& a : channels_) {
            const auto res = a.feed(sample);
            const float weight = std::pow(res.correlation, 4.0F);
            data  += res.data  ? weight : -weight;
            clock += res.clock ? weight : -weight;
        }
        return {data, clock};
    }

    std::vector<float> getCorrelationVector() const {
        std::vector<float> out;
        std::transform(std::begin(channels_),
                       std::end(channels_),
                       std::back_insert_iterator(out),
                       [](const CorrelationChannel& x) { return x.getCorrelation(); });
        return out;
    }

    bool isCodePhaseSynchronized(const float stdev_multiple_threshold = 5.0F) const {
        const auto cvec = getCorrelationVector();
        const auto [mean, stdev] = computeMeanStdev(cvec);
        const auto max = *std::max_element(std::begin(cvec), std::end(cvec));
        return (max - mean) > (stdev * stdev_multiple_threshold);
    }

private:
    std::vector<CorrelationChannel> channels_;
};

class BitReader {
public:
    bool next() {
        for (;;) {
            const bool phy_state = readPHY();
            const auto result = correlator_.feed(phy_state);

            if (!clock_latch_ && result.clock > 0.0F) {
                clock_latch_ = true;
                return result.data > 0.0F;
            }

            if (clock_latch_ && result.clock < 0.0F) {
                clock_latch_ = false;
            }
        }
    }

    void printDiagnostics() {
        const auto cvec = correlator_.getCorrelationVector();
        const auto [mean, stdev] = computeMeanStdev(cvec);
        std::cout << RX_COLOR << "[RX]" << RESET_COLOR << " " << getTimestamp()
                  << " Corr: mean=" << std::fixed << std::setprecision(2) << mean
                  << " max=" << *std::max_element(std::begin(cvec), std::end(cvec))
                  << " stdev=" << stdev
                  << " lock=" << correlator_.isCodePhaseSynchronized()
                  << " | ";
        for (auto c : cvec) {
            if (c > 0.2F) {
                std::cout << RX_COLOR << std::hex << int(c * 16.0F) << RESET_COLOR;
            } else {
                std::cout << ".";
            }
        }
        std::cout << "\n";
    }

private:
    Correlator correlator_;
    bool clock_latch_ = false;
};

class SymbolReader {
public:
    struct Delimiter {};
    using Symbol = std::variant<Delimiter, std::uint8_t>;

    Symbol next() {
        while (true) {
            const bool bit = bit_reader_.next();
            bit_reader_.printDiagnostics();
            if (remaining_bits_ >= 0) {
                buffer_ = (buffer_ << 1U) | bit;
                remaining_bits_--;
                if (remaining_bits_ < 0) {
                    return Symbol{buffer_};
                }
            } else if (bit) {
                consecutive_zeros_ = 0;
                remaining_bits_ = 7;
                buffer_ = 0;
            } else {
                consecutive_zeros_++;
                if (consecutive_zeros_ > 8) {
                    remaining_bits_ = -1;
                    return Symbol{Delimiter{}};
                }
            }
        }
    }

private:
    BitReader bit_reader_;
    std::uint64_t consecutive_zeros_ = 0;
    std::uint8_t  buffer_ = 0;
    std::int8_t   remaining_bits_ = -1;
};

class PacketReader {
    class FrameAssembler {
    public:
        std::optional<std::vector<std::uint8_t>> operator()(const SymbolReader::Delimiter&) {
            std::optional<std::vector<std::uint8_t>> result;
            if (buffer_.size() >= 2) {
                std::uint16_t crc = side_channel::CRCInitial;
                for (std::uint8_t v : buffer_) {
                    crc = side_channel::crcAdd(crc, v);
                }
                if (0 == crc) {
                    buffer_.pop_back();
                    buffer_.pop_back();
                    result.emplace(buffer_);
                } else {
                    std::cerr << ERROR_COLOR << "[ERROR]" << RESET_COLOR << " " << getTimestamp()
                              << " CRC error! Calculated: 0x" << std::hex << std::setw(4) 
                              << std::setfill('0') << crc << "\n";
                }
            }
            buffer_.clear();
            return result;
        }

        std::optional<std::vector<std::uint8_t>> operator()(const std::uint8_t data) {
            buffer_.push_back(data);
            return {};
        }

    private:
        std::vector<std::uint8_t> buffer_;
    };

public:
    std::vector<std::uint8_t> next() {
        while (true) {
            const auto sym = symbol_reader_.next();
            if (const auto ret = std::visit(assembler_, sym)) {
                return *ret;
            }
        }
    }

private:
    SymbolReader symbol_reader_;
    FrameAssembler assembler_;
};

int main() {
    std::cout << RX_COLOR << "[RX]" << RESET_COLOR << " " << getTimestamp()
              << " Initializing covert channel receiver\n"
              << "  SPREAD CODE LENGTH: " << side_channel::params::CDMACodeLength << " bits\n"
              << "  CHIP DURATION: " << side_channel::params::ChipPeriod.count() * 1e-6 << " ms\n";

    side_channel::initThread();
    PacketReader reader;
    
    while (true) {
        const auto packet = reader.next();

        std::ostringstream file_name;
        file_name << std::chrono::system_clock::now().time_since_epoch().count() << ".bin";
        
        if (std::ofstream out_file(file_name.str(), std::ios::binary | std::ios::out); out_file) {
            out_file.write(reinterpret_cast<const char*>(packet.data()), packet.size());
            out_file.close();
            
            std::cout << RX_COLOR << "[RX]" << RESET_COLOR << " " << getTimestamp()
                      << " Received packet (" << packet.size() << " bytes)\n"
                      << "  Saved to: " << file_name.str() << "\n";
            hexDump(packet);
        } else {
            std::cerr << ERROR_COLOR << "[ERROR]" << RESET_COLOR << " " << getTimestamp()
                      << " Failed to save packet to: " << file_name.str() << "\n";
            return 1;
        }
    }
    return 0;
}