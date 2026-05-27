/**
 * @file endian_utils.h
 * @brief Endian conversion utility functions
 * @author VL53L1 TOF Project
 * @date 2026
 */

#ifndef ENDIAN_UTILS_H
#define ENDIAN_UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Convert 16-bit value from host to little-endian
 * @param value Host value
 * @return Little-endian value
 */
uint16_t host_to_le16(uint16_t value);

/**
 * @brief Convert 16-bit value from little-endian to host
 * @param value Little-endian value
 * @return Host value
 */
uint16_t le16_to_host(uint16_t value);

/**
 * @brief Convert 32-bit value from host to little-endian
 * @param value Host value
 * @return Little-endian value
 */
uint32_t host_to_le32(uint32_t value);

/**
 * @brief Convert 32-bit value from little-endian to host
 * @param value Little-endian value
 * @return Host value
 */
uint32_t le32_to_host(uint32_t value);

/**
 * @brief Convert 16-bit value from host to big-endian
 * @param value Host value
 * @return Big-endian value
 */
uint16_t host_to_be16(uint16_t value);

/**
 * @brief Convert 16-bit value from big-endian to host
 * @param value Big-endian value
 * @return Host value
 */
uint16_t be16_to_host(uint16_t value);

/**
 * @brief Convert 32-bit value from host to big-endian
 * @param value Host value
 * @return Big-endian value
 */
uint32_t host_to_be32(uint32_t value);

/**
 * @brief Convert 32-bit value from big-endian to host
 * @param value Big-endian value
 * @return Host value
 */
uint32_t be32_to_host(uint32_t value);

/**
 * @brief Swap bytes in 16-bit value
 * @param value Input value
 * @return Byte-swapped value
 */
uint16_t swap_bytes16(uint16_t value);

/**
 * @brief Swap bytes in 32-bit value
 * @param value Input value
 * @return Byte-swapped value
 */
uint32_t swap_bytes32(uint32_t value);

/**
 * @brief Read 16-bit little-endian value from buffer
 * @param buffer Input buffer
 * @return 16-bit value
 */
uint16_t read_le16(const uint8_t* buffer);

/**
 * @brief Read 32-bit little-endian value from buffer
 * @param buffer Input buffer
 * @return 32-bit value
 */
uint32_t read_le32(const uint8_t* buffer);

/**
 * @brief Write 16-bit little-endian value to buffer
 * @param buffer Output buffer
 * @param value Value to write
 */
void write_le16(uint8_t* buffer, uint16_t value);

/**
 * @brief Write 32-bit little-endian value to buffer
 * @param buffer Output buffer
 * @param value Value to write
 */
void write_le32(uint8_t* buffer, uint32_t value);

// Convenience aliases for test compatibility
#define swap_uint16(value) swap_bytes16(value)
#define swap_uint32(value) swap_bytes32(value)
#define host_to_network_uint16(value) host_to_be16(value)
#define host_to_network_uint32(value) host_to_be32(value)
#define network_to_host_uint16(value) be16_to_host(value)
#define network_to_host_uint32(value) be32_to_host(value)

#ifdef __cplusplus
}
#endif

#endif // ENDIAN_UTILS_H
