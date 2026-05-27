/**
 * @file  vl53l1_platform.h
 * @brief Platform abstraction layer for VL53L1X API
 */

#ifndef VL53L1_PLATFORM_H
#define VL53L1_PLATFORM_H

#include <stdint.h>

// VL53L1X platform definitions
#define VL53L1X_I2C_ADDRESS    0x52

// Platform function prototypes only - no error definitions
int32_t VL53L1X_WriteMulti(uint16_t index, uint8_t *pdata, uint32_t count);
int32_t VL53L1X_ReadMulti(uint16_t index, uint8_t *pdata, uint32_t count);
int32_t VL53L1X_WrByte(uint16_t index, uint8_t data);
int32_t VL53L1X_RdByte(uint16_t index, uint8_t *data);
int32_t VL53L1X_WrWord(uint16_t index, uint8_t *pdata, uint32_t count);
int32_t VL53L1X_RdWord(uint16_t index, uint8_t *pdata, uint32_t count);
int32_t VL53L1X_WrDWord(uint16_t index, uint8_t *pdata, uint32_t count);
int32_t VL53L1X_RdDWord(uint16_t index, uint8_t *pdata, uint32_t count);
void VL53L1X_PollingDelay(uint32_t n);

// I2C handle structure
typedef struct {
    uint32_t i2c_periph;
    uint8_t device_address;
} VL53L1X_DEV;

#endif // VL53L1_PLATFORM_H
