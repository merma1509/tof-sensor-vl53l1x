/**
 * @file string_utils.c
 * @brief Safe string utility functions implementation
 * @author VL53L1 TOF Project
 * @date 2026
 */

#include "string_utils.h"
#include <stdarg.h>

char* safe_strcpy(char* dest, const char* src, size_t dest_size) {
    if (!dest || !src || dest_size == 0) return dest;
    
    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    
    return dest;
}

char* safe_strcat(char* dest, const char* src, size_t dest_size) {
    if (!dest || !src || dest_size == 0) return dest;
    
    size_t dest_len = safe_strlen(dest);
    size_t src_len = safe_strlen(src);
    
    if (dest_len + src_len >= dest_size) {
        src_len = dest_size - dest_len - 1;
    }
    
    for (size_t i = 0; i < src_len; i++) {
        dest[dest_len + i] = src[i];
    }
    dest[dest_len + src_len] = '\0';
    
    return dest;
}

int safe_strncmp(const char* s1, const char* s2, size_t n) {
    if (!s1 || !s2) return -1;
    
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i]) {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
        if (s1[i] == '\0') break;
    }
    return 0;
}

size_t safe_strlen(const char* str) {
    if (!str) return 0;
    
    size_t len = 0;
    while (str[len] != '\0') len++;
    return len;
}

int safe_strcmp(const char* s1, const char* s2) {
    if (!s1 || !s2) return -1;
    
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void* safe_memcpy(void* dest, const void* src, size_t n) {
    if (!dest || !src) return dest;
    
    uint8_t *d = (uint8_t*)dest;
    const uint8_t *s = (const uint8_t*)src;
    
    while (n--) {
        *d++ = *s++;
    }
    
    return dest;
}

int safe_snprintf(char* str, size_t size, const char* format, ...) {
    if (!str || size == 0 || !format) return 0;
    
    // Very minimal implementation - only supports %s, %d, %u, %X
    // For production use, this should be expanded or use a proper library
    va_list args;
    va_start(args, format);
    
    size_t written = 0;
    const char* p = format;
    
    while (*p && written < size - 1) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 's': {
                    char* s = va_arg(args, char*);
                    if (s) {
                        while (*s && written < size - 1) {
                            str[written++] = *s++;
                        }
                    }
                    break;
                }
                case 'd': {
                    int val = va_arg(args, int);
                    if (val < 0) {
                        str[written++] = '-';
                        val = -val;
                    }
                    char buf[16];
                    int i = 0;
                    if (val == 0) {
                        buf[i++] = '0';
                    } else {
                        while (val > 0 && i < 15) {
                            buf[i++] = '0' + (val % 10);
                            val /= 10;
                        }
                    }
                    while (i > 0 && written < size - 1) {
                        str[written++] = buf[--i];
                    }
                    break;
                }
                case 'u': {
                    unsigned int val = va_arg(args, unsigned int);
                    char buf[16];
                    int i = 0;
                    if (val == 0) {
                        buf[i++] = '0';
                    } else {
                        while (val > 0 && i < 15) {
                            buf[i++] = '0' + (val % 10);
                            val /= 10;
                        }
                    }
                    while (i > 0 && written < size - 1) {
                        str[written++] = buf[--i];
                    }
                    break;
                }
                case 'X': {
                    unsigned int val = va_arg(args, unsigned int);
                    char buf[16];
                    int i = 0;
                    if (val == 0) {
                        buf[i++] = '0';
                    } else {
                        while (val > 0 && i < 15) {
                            int digit = val % 16;
                            buf[i++] = digit < 10 ? '0' + digit : 'A' + digit - 10;
                            val /= 16;
                        }
                    }
                    while (i > 0 && written < size - 1) {
                        str[written++] = buf[--i];
                    }
                    break;
                }
                default:
                    if (written < size - 1) {
                        str[written++] = *p;
                    }
                    break;
            }
        } else {
            str[written++] = *p;
        }
        p++;
    }
    
    str[written] = '\0';
    va_end(args);
    
    return (int)written;
}

int safe_rand(void) {
    static unsigned int seed = 12345;
    seed = seed * 1103515245 + 12345;
    return (seed >> 16) & 0x7FFF;
}
