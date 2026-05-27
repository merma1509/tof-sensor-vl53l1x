/**
 * @file vl53l1_platform.h
 * @brief VL53L1 platform abstraction interface
 */

#ifndef VL53L1_PLATFORM_H
#define VL53L1_PLATFORM_H

#include <stdint.h>
#include "../../platform/platform.h"

/* VL53L1 I2C device address */
#define VL53L1_I2C_ADDRESS    0x29

/* VL53L1 platform interface - portable across MCUs */
typedef struct {
    /* I2C communication */
    int (*WriteMulti)(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count);
    int (*ReadMulti)(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count);
    int (*WriteByte)(uint16_t dev, uint16_t index, uint8_t data);
    int (*ReadByte)(uint16_t dev, uint16_t index, uint8_t *pdata);
    int (*WriteWord)(uint16_t dev, uint16_t index, uint16_t data);
    int (*ReadWord)(uint16_t dev, uint16_t index, uint16_t *pdata);
    int (*WriteDWord)(uint16_t dev, uint16_t index, uint32_t data);
    int (*ReadDWord)(uint16_t dev, uint16_t index, uint32_t *pdata);
    
    /* Timing functions */
    void (*WaitMs)(uint32_t wait_ms);
    void (*WaitUs)(uint32_t wait_us);
    
    /* GPIO functions (for interrupt handling) */
    int (*SetGPIO)(uint8_t level, uint8_t gpio);
    int (*GetGPIO)(uint8_t gpio, uint8_t *plevel);
} VL53L1_Dev_t;

/* Standard VL53L1 platform implementations */
int VL53L1_WriteMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count);
int VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count);
int VL53L1_WriteByte(uint16_t dev, uint16_t index, uint8_t data);
int VL53L1_ReadByte(uint16_t dev, uint16_t index, uint8_t *pdata);
int VL53L1_WriteWord(uint16_t dev, uint16_t index, uint16_t data);
int VL53L1_ReadWord(uint16_t dev, uint16_t index, uint16_t *pdata);
int VL53L1_WriteDWord(uint16_t dev, uint16_t index, uint32_t data);
int VL53L1_ReadDWord(uint16_t dev, uint16_t index, uint32_t *pdata);
void VL53L1_WaitMs(uint32_t wait_ms);
void VL53L1_WaitUs(uint32_t wait_us);

/* GPIO functions */
int VL53L1_SetGPIO(uint8_t level, uint8_t gpio);
int VL53L1_GetGPIO(uint8_t gpio, uint8_t *plevel);

/* Platform initialization for VL53L1 */
int VL53L1_PlatformInit(void);
void VL53L1_PlatformDeInit(void);

#endif /* VL53L1_PLATFORM_H */
