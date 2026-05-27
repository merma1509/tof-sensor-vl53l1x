/**
 * @file endian_utils.c
 * @brief Endian conversion utility functions implementation
 * @author VL53L1 TOF Project
 * @date 2026
 */

#include "endian_utils.h"

// Assume little-endian host (common for ARM and RISC-V)
uint16_t host_to_le16(uint16_t value) {
    return value;
}

uint16_t le16_to_host(uint16_t value) {
    return value;
}

uint32_t host_to_le32(uint32_t value) {
    return value;
}

uint32_t le32_to_host(uint32_t value) {
    return value;
}

// Big-endian conversions require byte swapping
uint16_t host_to_be16(uint16_t value) {
    return swap_bytes16(value);
}

uint16_t be16_to_host(uint16_t value) {
    return swap_bytes16(value);
}

uint32_t host_to_be32(uint32_t value) {
    return swap_bytes32(value);
}

uint32_t be32_to_host(uint32_t value) {
    return swap_bytes32(value);
}

uint16_t swap_bytes16(uint16_t value) {
    return ((value & 0xFF) << 8) | ((value >> 8) & 0xFF);
}

uint32_t swap_bytes32(uint32_t value) {
    return ((value & 0xFF) << 24) |
           ((value & 0xFF00) << 8) |
           ((value >> 8) & 0xFF00) |
           ((value >> 24) & 0xFF);
}

uint16_t read_le16(const uint8_t* buffer) {
    return (uint16_t)buffer[0] | ((uint16_t)buffer[1] << 8);
}

uint32_t read_le32(const uint8_t* buffer) {
    return (uint32_t)buffer[0] |
           ((uint32_t)buffer[1] << 8) |
           ((uint32_t)buffer[2] << 16) |
           ((uint32_t)buffer[3] << 24);
}

void write_le16(uint8_t* buffer, uint16_t value) {
    buffer[0] = value & 0xFF;
    buffer[1] = (value >> 8) & 0xFF;
}

void write_le32(uint8_t* buffer, uint32_t value) {
    buffer[0] = value & 0xFF;
    buffer[1] = (value >> 8) & 0xFF;
    buffer[2] = (value >> 16) & 0xFF;
    buffer[3] = (value >> 24) & 0xFF;
}
