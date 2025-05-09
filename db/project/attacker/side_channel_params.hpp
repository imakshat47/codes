/// Distributed under the terms of the MIT license.

#pragma once

#include <chrono>
#include <bitset>
#include <cstdint>
#include <thread>

#if defined(__linux__) && (MAX_CONCURRENCY == 1)
#include <pthread.h>
#endif

namespace side_channel {

// Configuration constants
constexpr unsigned MAX_CONCURRENCY_LIMIT = 999;
constexpr std::uint16_t CRC_CCITT_POLY = 0x1021U;
constexpr std::uint16_t CRC_INITIAL = 0xFFFFU;
constexpr unsigned CDMA_CODE_LENGTH = 1023;

inline void init_thread_affinity() {
#if defined(__linux__) && (MAX_CONCURRENCY == 1)
    // Pin thread to CPU 0 for consistent timing
    cpu_set_t cpuset{};
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
#endif
}

/// Optimized CRC-16-CCITT implementation (same algorithm as original)
inline std::uint16_t crc_add(std::uint16_t crc, uint8_t byte) noexcept {
    crc ^= static_cast<std::uint16_t>(byte) << 8;
    
    for (int i = 0; i < 8; ++i) {
        const bool msb_set = (crc & 0x8000U) != 0;
        crc = static_cast<std::uint16_t>((crc << 1) ^ (msb_set ? CRC_CCITT_POLY : 0));
    }
    
    return crc;
}

namespace params {
    // Transmission timing parameters
    constexpr std::chrono::nanoseconds CHIP_PERIOD{16'000'000};  // 16ms
    
    // CDMA spread code definition
    constexpr std::bitset<CDMA_CODE_LENGTH> CDMA_CODE{
        "1100100000111001010010011110010100010011111010101101000100010101"
        "0101100100011110100111111011011100110111110010101010000100000000"
        "1110101001000100110111100000111101011100110011110110000000101111"
        "0011111010100110001011011100011011110101000101011000001000000001"
        "0000001100011101100000011100011011111111101001110100101101100001"
        "0101011000100111001011011101100011101110111100001101100001100100"
        "1001000001101101001011011110001011100000010100100111111000001010"
        "1011100111110101111100110011000111000110110101010110110001101110"
        "1110000000000010110011011001110110100000101010111010111010010100"
        "0111001110001001010001010010110100001010110110101101100011100111"
        "1011001000011111100101101000100001111101010111001100100100100101"
        "1111111110000111110111100011011100101100001110010101000010100101"
        "0111111000111101101001110110011111101111101000110001111100000001"
        "0010100010110100010001001101100000011101101000110100010010001110"
        "0010110011001001111001101111110011001010011010011010111100110110"
        "101001110111100011010100010000100010010011100001110010100010000"
    };
} // namespace params

// Maintain original API names for backward compatibility
inline void initThread() { init_thread_affinity(); }
static constexpr std::uint16_t CRCInitial = CRC_INITIAL;
inline std::uint16_t crcAdd(std::uint16_t crc, uint8_t byte) { return crc_add(crc, byte); }

} // namespace side_channel