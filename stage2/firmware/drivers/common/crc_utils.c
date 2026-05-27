/**
 * @file crc_utils.c
 * @brief CRC checksum utility functions implementation
 * @author VL53L1 TOF Project
 * @date 2026
 */

#include "crc_utils.h"
#include <stdbool.h>

uint8_t crc8(const uint8_t* data, size_t length) {
    return crc8_custom(data, length, 0x07, 0x00); // CRC-8 polynomial
}

uint8_t crc8_custom(const uint8_t* data, size_t length, uint8_t polynomial, uint8_t initial) {
    if (!data) return 0;
    
    uint8_t crc = initial;
    
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int bit = 0; bit < 8; bit++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ polynomial;
            } else {
                crc <<= 1;
            }
        }
    }
    
    return crc;
}

uint16_t crc16(const uint8_t* data, size_t length) {
    return crc16_custom(data, length, 0x1021, 0xFFFF); // CRC-16-CCITT polynomial
}

uint16_t crc16_custom(const uint8_t* data, size_t length, uint16_t polynomial, uint16_t initial) {
    if (!data) return 0;
    
    uint16_t crc = initial;
    
    for (size_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (int bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ polynomial;
            } else {
                crc <<= 1;
            }
        }
    }
    
    return crc;
}

uint32_t crc32(const uint8_t* data, size_t length) {
    if (!data) return 0;
    
    uint32_t crc = 0xFFFFFFFF;
    const uint32_t polynomial = 0x04C11DB7; // CRC-32 polynomial
    
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int bit = 0; bit < 8; bit++) {
            if (crc & 0x80000000) {
                crc = (crc << 1) ^ polynomial;
            } else {
                crc <<= 1;
            }
        }
    }
    
    return crc ^ 0xFFFFFFFF; // Final XOR
}

bool verify_crc8(const uint8_t* data, size_t length, uint8_t expected_crc) {
    return crc8(data, length) == expected_crc;
}

bool verify_crc16(const uint8_t* data, size_t length, uint16_t expected_crc) {
    return crc16(data, length) == expected_crc;
}
