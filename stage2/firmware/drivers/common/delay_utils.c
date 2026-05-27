/**
 * @file delay_utils.c
 * @brief Delay utility functions implementation
 * @author VL53L1 TOF Project
 * @date 2026
 */

#include "delay_utils.h"
#include "../../platform/platform.h"
#include <stdbool.h>

void delay_us(uint32_t microseconds) {
    // Use platform-specific delay
    const platform_interface_t* platform = platform_get_interface();
    if (platform && platform->delay_us) {
        platform->delay_us(microseconds);
    } else {
        // Fallback: simple loop (very approximate)
        volatile uint32_t count = microseconds * 8; // Rough approximation
        while (count--) {
            __asm__("nop");
        }
    }
}

void delay_ms(uint32_t milliseconds) {
    delay_us(milliseconds * 1000);
}

uint32_t get_tick_ms(void) {
    const platform_interface_t* platform = platform_get_interface();
    if (platform && platform->get_tick_ms) {
        return platform->get_tick_ms();
    }
    return 0; // Fallback
}

uint32_t get_tick_us(void) {
    const platform_interface_t* platform = platform_get_interface();
    if (platform && platform->get_tick_us) {
        return platform->get_tick_us();
    }
    return 0; // Fallback
}

bool delay_until_ms(uint32_t target_time) {
    uint32_t current_time = get_tick_ms();
    
    // Handle wrap-around
    if (target_time > current_time) {
        while (get_tick_ms() < target_time) {
            // Wait
        }
        return true;
    } else {
        // Target time has passed or wrapped around
        return false;
    }
}

bool delay_with_timeout_ms(uint32_t duration_ms, uint32_t timeout_ms) {
    uint32_t start_time = get_tick_ms();
    uint32_t target_time = start_time + duration_ms;
    uint32_t timeout_time = start_time + timeout_ms;
    
    while (get_tick_ms() < target_time) {
        if (get_tick_ms() >= timeout_time) {
            return false; // Timeout
        }
        // Small delay to prevent busy-wait
        delay_us(100);
    }
    
    return true; // Duration completed
}
