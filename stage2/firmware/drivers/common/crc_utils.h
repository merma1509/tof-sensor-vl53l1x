/**
 * @file crc_utils.h
 * @brief CRC checksum utility functions
 * @author VL53L1 TOF Project
 * @date 2026
 */

#ifndef CRC_UTILS_H
#define CRC_UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculate CRC-8 checksum
 * @param data Input data
 * @param length Data length
 * @return CRC-8 value
 */
uint8_t crc8(const uint8_t* data, size_t length);

/**
 * @brief Calculate CRC-8 with custom polynomial
 * @param data Input data
 * @param length Data length
 * @param polynomial Custom polynomial
 * @param initial Initial value
 * @return CRC-8 value
 */
uint8_t crc8_custom(const uint8_t* data, size_t length, uint8_t polynomial, uint8_t initial);

/**
 * @brief Calculate CRC-16 checksum
 * @param data Input data
 * @param length Data length
 * @return CRC-16 value
 */
uint16_t crc16(const uint8_t* data, size_t length);

/**
 * @brief Calculate CRC-16 with custom polynomial
 * @param data Input data
 * @param length Data length
 * @param polynomial Custom polynomial
 * @param initial Initial value
 * @return CRC-16 value
 */
uint16_t crc16_custom(const uint8_t* data, size_t length, uint16_t polynomial, uint16_t initial);

/**
 * @brief Calculate CRC-32 checksum
 * @param data Input data
 * @param length Data length
 * @return CRC-32 value
 */
uint32_t crc32(const uint8_t* data, size_t length);

/**
 * @brief Verify CRC-8 checksum
 * @param data Input data
 * @param length Data length
 * @param expected_crc Expected CRC value
 * @return True if CRC matches, false otherwise
 */
bool verify_crc8(const uint8_t* data, size_t length, uint8_t expected_crc);

/**
 * @brief Verify CRC-16 checksum
 * @param data Input data
 * @param length Data length
 * @param expected_crc Expected CRC value
 * @return True if CRC matches, false otherwise
 */
bool verify_crc16(const uint8_t* data, size_t length, uint16_t expected_crc);

// Convenience aliases for test compatibility
#define crc8_calculate(data, length) crc8(data, length)
#define crc16_calculate(data, length) crc16(data, length)
#define crc32_calculate(data, length) crc32(data, length)

#ifdef __cplusplus
}
#endif

#endif // CRC_UTILS_H
