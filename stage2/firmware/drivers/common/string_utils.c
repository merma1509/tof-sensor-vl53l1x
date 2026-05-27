/**
 * @file string_utils.c
 * @brief Safe string utility functions implementation
 * @author VL53L1 TOF Project
 * @date 2026
 */

#include "string_utils.h"

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
