/**
 * @file  i2c_driver.h
 * @brief I2C driver for GD32E230 and VL53L1X ULD API
 */

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>

/* I2C register addresses for GD32E230 */
#define I2C0_BASE          0x40005400
#define I2C0_CTL0         (*((volatile uint32_t *)(I2C0_BASE + 0x00))
#define I2C0_CTL1         (*((volatile uint32_t *)(I2C0_BASE + 0x04))
#define I2C0_STAT         (*((volatile uint32_t *)(I2C0_BASE + 0x10))
#define I2C0_DATA         (*((volatile uint32_t *)(I2C0_BASE + 0x18))
#define I2C0_SADDR0       (*((volatile uint32_t *)(I2C0_BASE + 0x1C))
#define I2C0_SADDR1       (*((volatile uint32_t *)(I2C0_BASE + 0x20))

/* I2C control bits */
#define I2C_CTL0_I2CEN      (1 << 0)
#define I2C_CTL0_SMBEN      (1 << 1)
#define I2C_CTL0_SSTEN      (1 << 8)
#define I2C_CTL0_I2CCLK     (1 << 9)
#define I2C_CTL0_ARPDIS     (1 << 10)

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

/* I2C function declarations */
void i2c_init(void);
uint8_t i2c_write_register(uint8_t reg, uint8_t data);
uint8_t i2c_read_register(uint8_t reg);

/* VL53L1X ULD API compatible functions */
int8_t VL53L1X_WriteReg(uint16_t dev, uint16_t index, uint8_t data);
int8_t VL53L1X_ReadReg(uint16_t dev, uint16_t index);
int8_t VL53L1X_WriteMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count);
int8_t VL53L1X_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count);

#endif /* I2C_DRIVER_H */
