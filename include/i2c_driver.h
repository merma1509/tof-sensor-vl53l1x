/**
 * @file  i2c_driver.h
 * @brief I2C driver for GD32E230 and VL53L1
 */

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>

/* VL53L1 I2C device address */
#define VL53L1_DEVICE_ADDR     0x52
#define VL53L1X_DEVICE_ADDR    0x52  /* Backward compatibility */

/* I2C register addresses for GD32E230 */
#define I2C0_BASE          0x40005400

/* I2C status bits */
#define I2C_STAT_BERR       (1 << 8)
#define I2C_STAT_LOSTARB    (1 << 9)
#define I2C_STAT_BEC        (1 << 10)
#define I2C_STAT_ADDSEND    (1 << 13)
#define I2C_STAT_NACKF      (1 << 14)
#define I2C_STAT_RXNE       (1 << 6)
#define I2C_STAT_TXE        (1 << 7)
#define I2C_STAT_RXBNE      (1 << 5)
#define I2C_STAT_TC         (1 << 3)
#define I2C_STAT_BUSY        (1 << 1)

/* I2C control bits */
#define I2C_CTL1_CLOCK      0x0F

/* Delay function declaration */
extern void delay_ms(uint32_t ms);

/* VL53L1 control functions */
void vl53l1_reset(void);

/* I2C function declarations */
void i2c_init(void);
uint8_t i2c_write_register(uint16_t reg, uint8_t data);
uint8_t i2c_read_register(uint16_t reg);

/* VL53L1 API compatible functions */
int8_t VL53L1_WriteReg(uint16_t dev, uint16_t index, uint8_t data);
int8_t VL53L1_ReadReg(uint16_t dev, uint16_t index);
int32_t VL53L1_WriteMulti(uint16_t index, uint8_t *pdata, uint32_t count);
int32_t VL53L1_ReadMulti(uint16_t index, uint8_t *pdata, uint32_t count);

/* Backward compatibility */
#define VL53L1X_WriteReg VL53L1_WriteReg
#define VL53L1X_ReadReg VL53L1_ReadReg
#define VL53L1X_WriteMulti VL53L1_WriteMulti
#define VL53L1X_ReadMulti VL53L1_ReadMulti

#endif /* I2C_DRIVER_H */
