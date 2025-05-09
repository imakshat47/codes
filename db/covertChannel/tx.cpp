/// g++ -std=c++17 -O2 -Wall tx.cpp -lpthread -o tx && ./tx

#include "side_channel_params.hpp"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <iterator>
#include <thread>
#include <vector>
#include <atomic>
#include <cassert>
#include <stdexcept>
#include <chrono>
#include <iomanip>

// ANSI color codes
#define TX_COLOR "\033[1;32m"
#define ERROR_COLOR "\033[1;31m"
#define RESET_COLOR "\033[0m"

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
    std::cout << TX_COLOR << "  Hex Dump:" << RESET_COLOR << "\n";
    for (size_t i = 0; i < data.size(); ++i) {
        if (i % 16 == 0) {
            std::cout << TX_COLOR << "  " << std::hex << std::setw(4) 
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

static void drivePHY(const bool level, const std::chrono::nanoseconds duration) {
    static auto deadline = std::chrono::steady_clock::now();
    deadline += duration;
    if (level) {
        std::atomic<bool> finish = false;
        const auto loop = [&finish]() {
            while (!finish) {
                volatile std::uint16_t i = 1;
                while (i != 0) {
                    i = i + 1U;
                }
            }
        };
        static const auto thread_count = std::max<unsigned>(1, std::min<unsigned>(MAX_CONCURRENCY,
                                                                                  std::thread::hardware_concurrency()));
        std::vector<std::thread> pool;
        assert(thread_count > 0);
        for (auto i = 0U; i < (thread_count - 1); i++) {
            pool.emplace_back(loop);
        }
        while (std::chrono::steady_clock::now() < deadline) {
            volatile std::uint16_t i = 1;
            while (i != 0) {
                i = i + 1U;
            }
        }
        finish = true;
        for (auto& t : pool) {
            t.join();
        }
    } else {
        std::this_thread::sleep_for(deadline - std::chrono::steady_clock::now());
    }
}

static void emitBit(const bool value) {
    using side_channel::params::ChipPeriod;
    using side_channel::params::CDMACode;

    for (auto i = 0U; i < CDMACode.size(); i++) {
        const bool code_position = CDMACode[i];
        const bool bit = value ? code_position : !code_position;
        drivePHY(bit, ChipPeriod);
    }
}

static void emitByte(const std::uint8_t data) {
    auto i = sizeof(data) * 8U;
    std::cout << TX_COLOR << "[TX]" << RESET_COLOR << " " << getTimestamp()
              << " Transmitting byte: 0x" << std::hex << std::setw(2) 
              << std::setfill('0') << static_cast<int>(data) << "\n";
    emitBit(1); // START BIT
    while (i --> 0) {
        const bool bit = (static_cast<std::uintmax_t>(data) & (1ULL << i)) != 0U;
        emitBit(bit);
    }
}

static void emitFrameDelimiter() {
    std::cout << TX_COLOR << "[TX]" << RESET_COLOR << " " << getTimestamp()
              << " Sending frame delimiter (20 zero bits)\n";
    for (auto i = 0U; i < 20; i++) {
        emitBit(0);
    }
}

static void emitPacket(const std::vector<std::uint8_t>& data) {
    std::cout << TX_COLOR << "[TX]" << RESET_COLOR << " " << getTimestamp()
              << " Starting packet transmission (" << data.size() << " bytes)\n";
    emitFrameDelimiter();
    
    std::uint16_t crc = side_channel::CRCInitial;
    for (std::uint8_t v : data) {
        emitByte(v);
        crc = side_channel::crcAdd(crc, v);
    }
    
    // Emit CRC
    emitByte(static_cast<std::uint8_t>(crc >> 8U));
    emitByte(static_cast<std::uint8_t>(crc >> 0U));
    
    emitFrameDelimiter();
    std::cout << TX_COLOR << "[TX]" << RESET_COLOR << " " << getTimestamp()
              << " Packet transmission complete\n";
}

static std::vector<std::uint8_t> readFile(const std::string& path) {
    std::ifstream ifs(path, std::ios::binary);
    if (ifs) {
        ifs.unsetf(std::ios::skipws);
        ifs.seekg(0, std::ios::end);
        std::vector<std::uint8_t> buf;
        buf.reserve(ifs.tellg());
        ifs.seekg(0, std::ios::beg);
        buf.insert(buf.begin(), std::istream_iterator<std::uint8_t>(ifs), 
                  std::istream_iterator<std::uint8_t>());
        return buf;
    }
    throw std::runtime_error("File access error: " + path);
}

int main(const int argc, const char* const argv[]) {
    std::cout << TX_COLOR << "[TX]" << RESET_COLOR << " " << getTimestamp()
              << " Initializing covert channel transmitter\n"
              << "  SPREAD CODE LENGTH: " << side_channel::params::CDMACodeLength << " bits\n"
              << "  CHIP DURATION: " << side_channel::params::ChipPeriod.count() * 1e-6 << " ms\n"
              << "  MAX CONCURRENCY: " << MAX_CONCURRENCY << " threads\n";

    if (argc < 2) {
        std::cerr << ERROR_COLOR << "[ERROR]" << RESET_COLOR << " " << getTimestamp()
                  << " Missing file argument\n"
                  << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    side_channel::initThread();
    
    try {
        const auto data = readFile(argv[1]);
        std::cout << TX_COLOR << "[TX]" << RESET_COLOR << " " << getTimestamp()
                  << " Loaded " << data.size() << " bytes from: " << argv[1] << "\n";
        hexDump(data);
        
        emitPacket(data);
    } catch (const std::exception& e) {
        std::cerr << ERROR_COLOR << "[ERROR]" << RESET_COLOR << " " << getTimestamp()
                  << " " << e.what() << "\n";
        return 1;
    }

    std::cout << TX_COLOR << "[TX]" << RESET_COLOR << " " << getTimestamp()
              << " Transmission process completed successfully\n";
    return 0;
}