/**
 * @file debug_utils.c
 * @brief Debug utility functions implementation
 * @author VL53L1 TOF Project
 * @date 2026
 */

#include "debug_utils.h"
#include "string_utils.h"
#include "math_utils.h"
#include "../../platform/platform.h"

static bool debug_enabled = true;

void debug_hex_dump(const uint8_t* data, size_t length, uint32_t base_address) {
    if (!data || !debug_enabled) return;
    
    const platform_interface_t* platform = platform_get_interface();
    if (!platform || !platform->uart_send_string) return;
    
    char hex_line[80];
    char temp[8];
    
    for (size_t i = 0; i < length; i += 16) {
        // Address
        int_to_string((int)(base_address + i), temp, sizeof(temp));
        safe_strcpy(hex_line, temp, sizeof(hex_line));
        safe_strcat(hex_line, ": ", sizeof(hex_line));
        
        // Hex bytes
        for (size_t j = 0; j < 16 && (i + j) < length; j++) {
            uint8_t byte = data[i + j];
            int_to_string(byte, temp, sizeof(temp));
            safe_strcat(hex_line, temp, sizeof(hex_line));
            safe_strcat(hex_line, " ", sizeof(hex_line));
        }
        
        platform->uart_send_string(hex_line);
    }
}

void debug_print(const char* format, ...) {
    // Simple implementation - just print the format string
    if (!debug_enabled) return;
    
    const platform_interface_t* platform = platform_get_interface();
    if (platform && platform->uart_send_string) {
        platform->uart_send_string(format);
    }
}

void debug_assert(bool condition, const char* file, int line) {
    if (!condition) {
        const platform_interface_t* platform = platform_get_interface();
        if (platform && platform->uart_send_string) {
            char msg[64];
            safe_strcpy(msg, "ASSERT: ", sizeof(msg));
            safe_strcat(msg, file, sizeof(msg));
            safe_strcat(msg, ":", sizeof(msg));
            
            char line_str[8];
            int_to_string(line, line_str, sizeof(line_str));
            safe_strcat(msg, line_str, sizeof(msg));
            
            platform->uart_send_string(msg);
        }
        
        // Hang the system
        while (1) {
            __asm__("nop");
        }
    }
}

void debug_print_memory_usage(void) {
    if (!debug_enabled) return;
    
    const platform_interface_t* platform = platform_get_interface();
    if (platform && platform->uart_send_string) {
        platform->uart_send_string("Memory usage info not implemented\r\n");
    }
}

void debug_print_stack_trace(void) {
    if (!debug_enabled) return;
    
    const platform_interface_t* platform = platform_get_interface();
    if (platform && platform->uart_send_string) {
        platform->uart_send_string("Stack trace not implemented\r\n");
    }
}

void debug_set_enabled(bool enabled) {
    debug_enabled = enabled;
}

bool debug_is_enabled(void) {
    return debug_enabled;
}
