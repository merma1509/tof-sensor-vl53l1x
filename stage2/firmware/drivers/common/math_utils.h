/**
 * @file math_utils.h
 * @brief Mathematical utility functions
 * @author VL53L1 TOF Project
 * @date 2026
 */

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Convert integer to string
 * @param value Integer value
 * @param buffer Output buffer
 * @param buffer_size Buffer size
 * @return Pointer to buffer
 */
char* int_to_string(int value, char* buffer, size_t buffer_size);

/**
 * @brief Convert string to integer
 * @param str Input string
 * @param base Number base (10 for decimal, 16 for hex)
 * @return Converted integer value
 */
int string_to_int(const char* str, int base);

/**
 * @brief Calculate absolute value
 * @param value Input value
 * @return Absolute value
 */
int abs_int(int value);

/**
 * @brief Simple square root approximation
 * @param value Input value (non-negative)
 * @return Approximate square root
 */
uint32_t sqrt_approx(uint32_t value);

/**
 * @brief Calculate minimum of two values
 * @param a First value
 * @param b Second value
 * @return Minimum value
 */
int min_int(int a, int b);

/**
 * @brief Calculate maximum of two values
 * @param a First value
 * @param b Second value
 * @return Maximum value
 */
int max_int(int a, int b);

/**
 * @brief Clamp value between min and max
 * @param value Input value
 * @param min Minimum value
 * @param max Maximum value
 * @return Clamped value
 */
int clamp_int(int value, int min, int max);

#ifdef __cplusplus
}
#endif

#endif // MATH_UTILS_H
