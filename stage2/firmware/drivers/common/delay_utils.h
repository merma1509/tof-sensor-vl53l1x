/**
 * @file delay_utils.h
 * @brief Delay utility functions
 * @author VL53L1 TOF Project
 * @date 2026
 */

#ifndef DELAY_UTILS_H
#define DELAY_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Simple busy-wait delay
 * @param microseconds Delay in microseconds (approximate)
 */
void delay_us(uint32_t microseconds);

/**
 * @brief Simple busy-wait delay
 * @param milliseconds Delay in milliseconds (approximate)
 */
void delay_ms(uint32_t milliseconds);

/**
 * @brief Get system tick count in milliseconds
 * @return Current tick count in milliseconds
 */
uint32_t get_tick_ms(void);

/**
 * @brief Get system tick count in microseconds
 * @return Current tick count in microseconds
 */
uint32_t get_tick_us(void);

/**
 * @brief Delay until specified time (blocking)
 * @param target_time Target tick count in milliseconds
 * @return True if delay completed, false if timeout
 */
bool delay_until_ms(uint32_t target_time);

/**
 * @brief Delay for specified duration with timeout
 * @param duration_ms Duration in milliseconds
 * @param timeout_ms Maximum time to wait
 * @return True if delay completed, false if timeout
 */
bool delay_with_timeout_ms(uint32_t duration_ms, uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif // DELAY_UTILS_H
