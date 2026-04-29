/**
 * @file  sdk_integration.h
 * @brief SDK integration header for VL53L1 and GD32E230
 */

#ifndef SDK_INTEGRATION_H
#define SDK_INTEGRATION_H

#include <stdint.h>

// Include GD32E230 SDK
#ifdef __clang__
/* Use compatibility header for Clang IDE lint - provides all needed definitions */
#include "clang_compat.h"
#else
/* GCC/ARM compiler - use real SDK headers */
#include "../GD32E23Firmware/CMSIS/GD/GD32E23x/Include/gd32e23x.h"
#include "../GD32E23Firmware/GD32E23x_standard_peripheral/Include/gd32e23x_i2c.h"
#include "../GD32E23Firmware/GD32E23x_standard_peripheral/Include/gd32e23x_usart.h"
#include "../GD32E23Firmware/GD32E23x_standard_peripheral/Include/gd32e23x_gpio.h"
#include "../GD32E23Firmware/GD32E23x_standard_peripheral/Include/gd32e23x_rcu.h"
#endif

// Include VL53L1X API (compatible with VL53L1 for basic functions)
#include "../API/core/VL53L1X_api.h"
#include "../API/core/VL53L1X_calibration.h"

// Compatibility defines for VL53L1
#define VL53L1_I2C_ADDRESS      0x52
#define VL53L1_ERROR_NONE       0
#define VL53L1_ERROR           int8_t

// Map VL53L1 API calls to VL53L1X API (they are compatible)
#define VL53L1_SensorInit       VL53L1X_SensorInit
#define VL53L1_GetDistance      VL53L1X_GetDistance
#define VL53L1_SetDistanceMode  VL53L1X_SetDistanceMode
#define VL53L1_SetTimingBudgetInMs VL53L1X_SetTimingBudgetInMs
#define VL53L1_StartTemperatureUpdate VL53L1X_StartTemperatureUpdate

// SDK integration constants
#define SDK_SYSTEM_CLOCK_48MHZ     48000000
#define SDK_I2C_CLOCK_100KHZ       100000
#define SDK_UART_BAUD_RATE          115200

// SDK integration structures
typedef struct {
    uint8_t vl53l1_initialized;
    uint8_t gd32_initialized;
    uint16_t current_distance;
    uint8_t current_mode;
    uint32_t timing_budget;
} sdk_integration_t;

// SDK function prototypes
uint8_t sdk_system_init(void);
uint8_t sdk_i2c_init(void);
uint8_t sdk_uart_init(void);
uint8_t sdk_vl53l1_init(void);
uint8_t sdk_vl53l1_get_distance(uint16_t *distance);
uint8_t sdk_vl53l1_set_mode(uint8_t mode);
uint8_t sdk_vl53l1_set_timing(uint32_t timing);
uint8_t sdk_vl53l1_calibrate(void);

// Additional SDK functions
uint8_t sdk_full_init(void);
sdk_integration_t* sdk_get_status(void);

// GD32E230 SDK wrapper functions
void sdk_gpio_init(void);
void sdk_system_clock_config(void);

#endif // SDK_INTEGRATION_H
