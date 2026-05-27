/**
 * @file platform_config.h
 * @brief Platform configuration definitions
 */

#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

#include <stdint.h>

/* Platform selection */
#define PLATFORM_GD32    1
#define PLATFORM_MIK32   2

/* Current platform - modify this for different builds */
#ifndef TARGET_PLATFORM
#define TARGET_PLATFORM PLATFORM_GD32
#endif

/* Common configurations */
#define UART_BAUDRATE           115200
#define I2C_FREQUENCY           100000  // 100kHz
#define VL53L1_I2C_ADDRESS      0x29    // Default VL53L1 address

/* Debug configuration */
#define DEBUG_ENABLED           1
#define DEBUG_UART_BAUDRATE     115200

/* Application configuration */
#define FILTER_ENABLED          1
#define FILTER_MEDIAN_SIZE      5
#define FILTER_AVERAGE_SIZE    4

/* Timing configurations */
#define SYSTEM_CLOCK_HZ         48000000
#define SYSTICK_FREQUENCY_HZ    1000

/* Pin configurations - platform specific */
#if TARGET_PLATFORM == PLATFORM_GD32
    #define UART_TX_PIN      9   // PA9
    #define UART_RX_PIN      10  // PA10
    #define I2C_SCL_PIN      6   // PA6
    #define I2C_SDA_PIN      7   // PA7
#elif TARGET_PLATFORM == PLATFORM_MIK32
    #define UART_TX_PIN      10  // P1.10
    #define UART_RX_PIN      11  // P1.11
    #define I2C_SCL_PIN      10  // P0.10
    #define I2C_SDA_PIN      9   // P0.9
#endif

#endif /* PLATFORM_CONFIG_H */
