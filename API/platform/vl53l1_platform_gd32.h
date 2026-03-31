/**
 * @file  vl53l1_platform_gd32.h
 * @brief GD32E230 platform header for VL53L1X ULD API
 */

#ifndef VL53L1_PLATFORM_GD32_H
#define VL53L1_PLATFORM_GD32_H

#include <stdint.h>

/* VL53L1X platform function declarations */
int8_t VL53L1X_WriteMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count);
int8_t VL53L1X_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count);
int8_t VL53L1X_WriteReg(uint16_t dev, uint16_t index, uint8_t data);
uint8_t VL53L1X_ReadReg(uint16_t dev, uint16_t index);
void VL53L1X_PollingDelay(void);
void VL53L1X_SetGpio(uint8_t level);
void VL53L1X_GetClocks(void);
void VL53L1X_SpiEnable(void);
void VL53L1X_SpiDisable(void);

/* I2C function declarations for compatibility */
uint8_t i2c_write_register(uint16_t reg, uint8_t data);
uint8_t i2c_read_register(uint16_t reg);
void i2c_init(void);

#endif /* VL53L1_PLATFORM_GD32_H */
