/**
 * @file string_utils.h
 * @brief Safe string utility functions
 * @author VL53L1 TOF Project
 * @date 2026
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Safe string copy with length limit
 * @param dest Destination buffer
 * @param src Source string
 * @param dest_size Destination buffer size
 * @return Pointer to destination
 */
char* safe_strcpy(char* dest, const char* src, size_t dest_size);

/**
 * @brief Safe string concatenation with length limit
 * @param dest Destination buffer
 * @param src Source string
 * @param dest_size Destination buffer size
 * @return Pointer to destination
 */
char* safe_strcat(char* dest, const char* src, size_t dest_size);

/**
 * @brief Compare strings with length limit
 * @param s1 First string
 * @param s2 Second string
 * @param n Maximum characters to compare
 * @return 0 if equal, <0 if s1 < s2, >0 if s1 > s2
 */
int safe_strncmp(const char* s1, const char* s2, size_t n);

/**
 * @brief Get string length
 * @param str Input string
 * @return String length
 */
size_t safe_strlen(const char* str);

/**
 * @brief Compare strings
 * @param s1 First string
 * @param s2 Second string
 * @return 0 if equal, <0 if s1 < s2, >0 if s1 > s2
 */
int safe_strcmp(const char* s1, const char* s2);

/**
 * @brief Memory copy function
 * @param dest Destination buffer
 * @param src Source buffer
 * @param n Number of bytes to copy
 * @return Pointer to destination
 */
void* safe_memcpy(void* dest, const void* src, size_t n);

/**
 * @brief Minimal snprintf implementation for bare-metal environment
 * @param str Destination buffer
 * @param size Buffer size
 * @param format Format string
 * @param ... Variable arguments
 * @return Number of characters written (not including null terminator)
 */
int safe_snprintf(char* str, size_t size, const char* format, ...);

/**
 * @brief Simple random number generator
 * @return Random number between 0 and 32767
 */
int safe_rand(void);

#ifdef __cplusplus
}
#endif

#endif // STRING_UTILS_H
