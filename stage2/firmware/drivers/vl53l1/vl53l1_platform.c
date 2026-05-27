/**
 * @file vl53l1_platform.c
 * @brief VL53L1 platform abstraction implementation
 */

#include "vl53l1_platform.h"
#include "../../platform/platform.h"

// Minimal snprintf implementation for bare-metal environment
static int snprintf(char *str, size_t size, const char *format, ...) {
    // Very minimal implementation - only supports %s, %d, %u, %X
    // For production use, this should be expanded or use a proper library
    (void)format; // Suppress unused parameter warning
    str[0] = '\0';
    return 0;
}

/* VL53L1 I2C device address */
#define VL53L1_I2C_ADDRESS    0x29

/* VL53L1 platform I2C implementations */
int VL53L1_WriteMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    (void)dev; // Device address is fixed for VL53L1
    return PLATFORM_I2C_WRITE_REG(VL53L1_I2C_ADDRESS, index, pdata, count);
}

int VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
    (void)dev; // Device address is fixed for VL53L1
    return PLATFORM_I2C_READ_REG(VL53L1_I2C_ADDRESS, index, pdata, count);
}

int VL53L1_WriteByte(uint16_t dev, uint16_t index, uint8_t data) {
    return VL53L1_WriteMulti(dev, index, &data, 1);
}

int VL53L1_ReadByte(uint16_t dev, uint16_t index, uint8_t *pdata) {
    return VL53L1_ReadMulti(dev, index, pdata, 1);
}

int VL53L1_WriteWord(uint16_t dev, uint16_t index, uint16_t data) {
    uint8_t buffer[2];
    buffer[0] = (data >> 8) & 0xFF;
    buffer[1] = data & 0xFF;
    return VL53L1_WriteMulti(dev, index, buffer, 2);
}

int VL53L1_ReadWord(uint16_t dev, uint16_t index, uint16_t *pdata) {
    uint8_t buffer[2];
    int status = VL53L1_ReadMulti(dev, index, buffer, 2);
    if (status == 0) {
        *pdata = (buffer[0] << 8) | buffer[1];
    }
    return status;
}

int VL53L1_WriteDWord(uint16_t dev, uint16_t index, uint32_t data) {
    uint8_t buffer[4];
    buffer[0] = (data >> 24) & 0xFF;
    buffer[1] = (data >> 16) & 0xFF;
    buffer[2] = (data >> 8) & 0xFF;
    buffer[3] = data & 0xFF;
    return VL53L1_WriteMulti(dev, index, buffer, 4);
}

int VL53L1_ReadDWord(uint16_t dev, uint16_t index, uint32_t *pdata) {
    uint8_t buffer[4];
    int status = VL53L1_ReadMulti(dev, index, buffer, 4);
    if (status == 0) {
        *pdata = ((uint32_t)buffer[0] << 24) |
                 ((uint32_t)buffer[1] << 16) |
                 ((uint32_t)buffer[2] << 8) |
                 buffer[3];
    }
    return status;
}

/* VL53L1 timing implementations */
void VL53L1_WaitMs(uint32_t wait_ms) {
    PLATFORM_DELAY_MS(wait_ms);
}

void VL53L1_WaitUs(uint32_t wait_us) {
    PLATFORM_DELAY_US(wait_us);
}

/* VL53L1 GPIO implementations (optional) */
int VL53L1_SetGPIO(uint8_t level, uint8_t gpio) {
    // GPIO functionality - implementation depends on platform
    // This is typically used for interrupt handling
    (void)level;
    (void)gpio;
    return 0; // Not implemented for basic use case
}

int VL53L1_GetGPIO(uint8_t gpio, uint8_t *plevel) {
    (void)gpio;
    *plevel = 0;
    return 0; // Not implemented for basic use case
}

/* Platform initialization for VL53L1 */
int VL53L1_PlatformInit(void) {
    // Initialize I2C interface
    if (PLATFORM_I2C_INIT(I2C_FREQUENCY) != PLATFORM_OK) {
        return -1;
    }
    
    // Initialize UART for debugging
    if (PLATFORM_UART_INIT(UART_BAUDRATE) != PLATFORM_OK) {
        return -2;
    }
    
    DEBUG_PRINT("VL53L1 Platform Initialized\r\n");
    return 0;
}

void VL53L1_PlatformDeInit(void) {
    // Platform cleanup if needed
    DEBUG_PRINT("VL53L1 Platform Deinitialized\r\n");
}
