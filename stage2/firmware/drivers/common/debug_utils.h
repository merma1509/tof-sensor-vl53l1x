/**
 * @file debug_utils.h
 * @brief Debug utility functions
 * @author VL53L1 TOF Project
 * @date 2026
 */

#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Print hex dump of memory buffer
 * @param data Pointer to data
 * @param length Data length
 * @param base_address Base address for display
 */
void debug_hex_dump(const uint8_t* data, size_t length, uint32_t base_address);

/**
 * @brief Print formatted debug message
 * @param format Format string
 * @param ... Variable arguments
 */
void debug_print(const char* format, ...);

/**
 * @brief Assert function for debugging
 * @param condition Condition to check
 * @param file Source file name
 * @param line Line number
 */
void debug_assert(bool condition, const char* file, int line);

/**
 * @brief Print memory usage information
 */
void debug_print_memory_usage(void);

/**
 * @brief Print stack trace (if available)
 */
void debug_print_stack_trace(void);

/**
 * @brief Enable/disable debug output
 * @param enabled True to enable, false to disable
 */
void debug_set_enabled(bool enabled);

/**
 * @brief Check if debug output is enabled
 * @return True if enabled, false otherwise
 */
bool debug_is_enabled(void);

// Debug assert macro
#define DEBUG_ASSERT(condition) debug_assert(condition, __FILE__, __LINE__)

// Debug print macro (only if debug is enabled)
#define COMMON_DEBUG_PRINT(...) do { if (debug_is_enabled()) debug_print(__VA_ARGS__); } while(0)

#ifdef __cplusplus
}
#endif

#endif // DEBUG_UTILS_H
