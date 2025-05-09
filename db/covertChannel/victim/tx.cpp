// g++ -std=c++17 -O3 -Wall -Wextra -lpthread tx.cpp -o tx && ./tx

#include "side_channel_params.hpp"
#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

namespace fs = std::filesystem;
using namespace std::chrono_literals;

// Configuration constants
constexpr size_t DELIMITER_LENGTH = 20;
constexpr size_t SPREAD_CODE_SIZE = side_channel::params::CDMACode.size();
constexpr auto CHIP_DURATION = side_channel::params::ChipPeriod;

class PHYDriver {
public:
    PHYDriver() {
        // Precompute spread sequences for 0 and 1
        precompute_spread_sequences();
        precompute_delimiter();
    }

    void transmit(const std::vector<uint8_t>& data) {
        std::cout << "Transmitting " << data.size() << " bytes\n";
        transmit_sequence(delimiter_sequence_);
        
        uint16_t crc = side_channel::CRCInitial;
        size_t progress = 0;
        const size_t total = data.size() + 2;  // Include CRC bytes
        
        for (uint8_t byte : data) {
            transmit_byte(byte);
            crc = side_channel::crcAdd(crc, byte);
            show_progress(++progress, total);
        }
        
        transmit_byte(static_cast<uint8_t>(crc >> 8));
        transmit_byte(static_cast<uint8_t>(crc));
        show_progress(progress += 2, total);
        
        transmit_sequence(delimiter_sequence_);
        std::cout << "\nTransmission complete.\n";
    }

private:
    std::vector<bool> spread_0_;
    std::vector<bool> spread_1_;
    std::vector<bool> delimiter_sequence_;

    void precompute_spread_sequences() {
        spread_0_.reserve(SPREAD_CODE_SIZE);
        spread_1_.reserve(SPREAD_CODE_SIZE);
        
        for (size_t i = 0; i < SPREAD_CODE_SIZE; ++i) {
            bool code_bit = side_channel::params::CDMACode[i];
            spread_1_.push_back(code_bit);
            spread_0_.push_back(!code_bit);
        }
    }

    void precompute_delimiter() {
        delimiter_sequence_.reserve(DELIMITER_LENGTH * SPREAD_CODE_SIZE);
        for (size_t i = 0; i < DELIMITER_LENGTH; ++i) {
            delimiter_sequence_.insert(delimiter_sequence_.end(), 
                                     spread_0_.begin(), spread_0_.end());
        }
    }

    void transmit_sequence(const std::vector<bool>& sequence) {
        for (bool bit : sequence) {
            drive_phy(bit, CHIP_DURATION);
        }
    }

    void transmit_byte(uint8_t byte) {
        drive_phy(true, CHIP_DURATION);  // Start bit
        for (int i = 7; i >= 0; --i) {
            bool bit = (byte >> i) & 1;
            transmit_sequence(bit ? spread_1_ : spread_0_);
        }
    }

    static void drive_phy(bool level, std::chrono::nanoseconds duration) {
        const auto deadline = std::chrono::steady_clock::now() + duration;
        if (level) {
            // Optimized CPU-bound busy wait
            while (std::chrono::steady_clock::now() < deadline) {
                asm volatile("pause" ::: "memory");  // Reduce power consumption
            }
        } else {
            std::this_thread::sleep_until(deadline);
        }
    }

    static void show_progress(size_t current, size_t total) {
        constexpr size_t BAR_WIDTH = 50;
        const float progress = static_cast<float>(current) / total;
        const size_t pos = BAR_WIDTH * progress;

        std::cout << "[";
        for (size_t i = 0; i < BAR_WIDTH; ++i) {
            std::cout << (i < pos ? '=' : (i == pos ? '>' : ' '));
        }
        std::cout << "] " << int(progress * 100.0) << " %\r";
        std::cout.flush();
    }
};

std::vector<uint8_t> read_file(const fs::path& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) throw std::runtime_error("Cannot open file: " + path.string());

    const size_t file_size = file.tellg();
    file.seekg(0);

    std::vector<uint8_t> buffer(file_size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), file_size)) {
        throw std::runtime_error("Error reading file: " + path.string());
    }
    return buffer;
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <file>\n";
            return EXIT_FAILURE;
        }

        side_channel::initThread();
        PHYDriver transmitter;
        
        const auto start_time = std::chrono::steady_clock::now();
        transmitter.transmit(read_file(argv[1]));
        
        const auto duration = std::chrono::steady_clock::now() - start_time;
        std::cout << "Transmission time: " 
                << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
                << " ms\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}