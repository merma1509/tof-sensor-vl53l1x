/**
 * @file math_utils.c
 * @brief Mathematical utility functions implementation
 * @author VL53L1 TOF Project
 * @date 2026
 */

#include "math_utils.h"
#include "string_utils.h"

// Using safe_strlen from common utilities

char* int_to_string(int value, char* buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) return buffer;
    
    if (buffer_size < 2) {
        buffer[0] = '\0';
        return buffer;
    }
    
    // Handle negative numbers
    if (value < 0) {
        buffer[0] = '-';
        value = -value;
        return int_to_string(value, buffer + 1, buffer_size - 1) - 1;
    }
    
    // Convert digits
    int i = 0;
    int temp = value;
    
    if (temp == 0) {
        buffer[i++] = '0';
    } else {
        while (temp > 0 && i < (int)buffer_size - 1) {
            buffer[i++] = '0' + (temp % 10);
            temp /= 10;
        }
    }
    
    buffer[i] = '\0';
    
    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char c = buffer[j];
        buffer[j] = buffer[i - 1 - j];
        buffer[i - 1 - j] = c;
    }
    
    return buffer;
}

char* float_to_string(float value, char* buffer, size_t buffer_size, int decimal_places) {
    if (!buffer || buffer_size == 0) return buffer;
    
    if (buffer_size < 2) {
        buffer[0] = '\0';
        return buffer;
    }
    
    // Handle negative numbers
    if (value < 0) {
        buffer[0] = '-';
        value = -value;
        buffer++;
        buffer_size--;
    }
    
    // Convert integer part
    int int_part = (int)value;
    char* ptr = int_to_string(int_part, buffer, buffer_size);
    
    // Add decimal point if needed
    if (decimal_places > 0 && ptr < buffer + buffer_size - 1) {
        *ptr++ = '.';
        
        // Convert fractional part
        float frac_part = value - int_part;
        for (int i = 0; i < decimal_places && ptr < buffer + buffer_size - 1; i++) {
            frac_part *= 10;
            int digit = (int)frac_part;
            *ptr++ = '0' + digit;
            frac_part -= digit;
        }
    }
    
    *ptr = '\0';
    return buffer;
}

int string_to_int(const char* str, int base) {
    if (!str) return 0;
    
    int result = 0;
    int sign = 1;
    
    // Handle sign
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }
    
    // Convert digits
    while (*str != '\0') {
        int digit;
        
        if (*str >= '0' && *str <= '9') {
            digit = *str - '0';
        } else if (*str >= 'A' && *str <= 'F' && base == 16) {
            digit = *str - 'A' + 10;
        } else if (*str >= 'a' && *str <= 'f' && base == 16) {
            digit = *str - 'a' + 10;
        } else {
            break;
        }
        
        if (digit >= base) break;
        
        result = result * base + digit;
        str++;
    }
    
    return sign * result;
}

int abs_int(int value) {
    return value < 0 ? -value : value;
}

uint32_t sqrt_approx(uint32_t value) {
    if (value == 0) return 0;
    
    uint32_t result = 1;
    uint32_t bit = 1U << 30; // The second-to-top bit
    
    // Find the highest bit set
    while (bit > value) bit >>= 2;
    
    while (bit != 0) {
        if (value >= result + bit) {
            value -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    
    return result;
}

int min_int(int a, int b) {
    return a < b ? a : b;
}

int max_int(int a, int b) {
    return a > b ? a : b;
}

int clamp_int(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
