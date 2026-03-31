/**
 * @file  vl53l1_platform_gd32.c
 * @brief GD32E230 platform implementation for VL53L1X ULD API
 */

#include "vl53l1_platform_gd32.h"
#include "../../include/i2c_driver.h"

/* GD32E230 I2C implementation for VL53L1X ULD API */

int8_t VL53L1X_WriteMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    (void)dev; // Suppress unused parameter warning
    
    for (uint32_t i = 0; i < count; i++) {
        if (VL53L1X_WriteReg(dev, index + i, pdata[i]) != 0) {
            return -1; // Error
        }
    }
    return 0; // Success
}

int8_t VL53L1X_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    (void)dev; // Suppress unused parameter warning
    
    for (uint32_t i = 0; i < count; i++) {
        pdata[i] = VL53L1X_ReadReg(dev, index + i);
    }
    return 0; // Success
}

int8_t VL53L1X_WriteReg(uint16_t dev, uint16_t index, uint8_t data) {
    return i2c_write_register(0x52, index, data);
}

uint8_t VL53L1X_ReadReg(uint16_t dev, uint16_t index) {
    return i2c_read_register(0x52, index);
}

void VL53L1X_PollingDelay(void) {
    // Simple delay for GD32E230
    for (volatile uint32_t i = 0; i < 1000; i++);
}

void VL53L1X_SetGpio(uint8_t level) {
    // GPIO control for VL53L1X XSHUT pin
    // Implementation depends on GD32E230 GPIO setup
    // This is optional for basic operation
}

void VL53L1X_GetClocks(void) {
    // Clock management for GD32E230
    // Implementation depends on system requirements
}

void VL53L1X_SpiEnable(void) {
    // SPI enable - not used in I2C mode
}

void VL53L1X_SpiDisable(void) {
    // SPI disable - not used in I2C mode
}
