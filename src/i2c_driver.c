/**
 * @file  i2c_driver.c
 * @brief I2C driver for GD32E230 with VL53L1X support
 */

#include "../include/i2c_driver.h"

/* I2C implementation for GD32E230 */
void i2c_init(void) {
    // Configure I2C pins PB6(SDA), PB7(SCL)
    volatile uint32_t *RCU_AHBEN = (uint32_t*)0x40021014;
    *RCU_AHBEN |= (1 << 17) | (1 << 18); // GPIOA, GPIOB enable
    
    // Configure PB6 and PB7 as alternate function open-drain
    volatile uint32_t *GPIO_CTL0 = (uint32_t*)0x48000008;
    *GPIO_CTL0 &= ~((0xF << 24) | (0xF << 28)); // Clear PB6, PB7
    *GPIO_CTL0 |= (0xC << 24) | (0xC << 28); // AF output, open-drain, 50MHz
    
    // Enable pull-up for PB6, PB7
    volatile uint32_t *GPIO_PUD = (uint32_t*)0x4800000C;
    *GPIO_PUD &= ~((0x3 << 12) | (0x3 << 14)); // Clear PB6, PB7
    *GPIO_PUD |= (0x1 << 12) | (0x1 << 14); // Pull-up
    
    // Enable I2C0 clock
    volatile uint32_t *RCU_APB1EN = (uint32_t*)0x40021018;
    *RCU_APB1EN |= (1 << 21); // I2C0 clock enable
    
    // Reset I2C0
    volatile uint32_t *I2C0_CTL0 = (uint32_t*)0x40005400;
    *I2C0_CTL0 &= ~(1 << 0); // Disable I2C
    *I2C0_CTL0 |= (1 << 0);  // Enable I2C
    
    // Configure I2C timing for 100kHz at 48MHz
    volatile uint32_t *I2C0_CTL1 = (uint32_t*)0x40005404;
    *I2C0_CTL1 &= ~0x0F; // Clear clock bits
    *I2C0_CTL1 |= 0x0B;  // 100kHz timing
    
    // Set slave address
    volatile uint32_t *I2C0_SADDR0 = (uint32_t*)0x4000541C;
    *I2C0_SADDR0 = (VL53L1X_DEVICE_ADDR << 1);
}

uint8_t i2c_write_register(uint16_t reg, uint8_t data) {
    volatile uint32_t *I2C0_CTL0 = (uint32_t*)0x40005400;
    volatile uint32_t *I2C0_STAT0 = (uint32_t*)0x40005410;
    volatile uint32_t *I2C0_DATA = (uint32_t*)0x40005418;
    
    // Wait for I2C to be ready
    while (*I2C0_STAT0 & (1 << 1)); // Wait for BUSY=0
    
    // Send start condition
    *I2C0_CTL0 |= (1 << 8); // START=1
    
    // Wait for start condition generated
    while (!(*I2C0_STAT0 & (1 << 5))); // Wait for SBSEND=1
    
    // Send device address (write)
    *I2C0_DATA = (VL53L1X_DEVICE_ADDR << 1);
    while (!(*I2C0_STAT0 & (1 << 1))); // Wait for ADDSEND=1
    
    // Clear ADDSEND flag
    *I2C0_STAT0 = (1 << 3);
    
    // Send register address (16-bit - high byte first)
    *I2C0_DATA = (reg >> 8) & 0xFF;
    while (!(*I2C0_STAT0 & (1 << 7))); // Wait for BTC=1
    
    *I2C0_DATA = reg & 0xFF;
    while (!(*I2C0_STAT0 & (1 << 7))); // Wait for BTC=1
    
    // Send data
    *I2C0_DATA = data;
    while (!(*I2C0_STAT0 & (1 << 7))); // Wait for BTC=1
    
    // Send stop condition
    *I2C0_CTL0 |= (1 << 9); // STOP=1
    
    return 0; // Success
}

uint8_t i2c_read_register(uint16_t reg) {
    volatile uint32_t *I2C0_CTL0 = (uint32_t*)0x40005400;
    volatile uint32_t *I2C0_STAT0 = (uint32_t*)0x40005410;
    volatile uint32_t *I2C0_DATA = (uint32_t*)0x40005418;
    uint8_t data;
    
    // Wait for I2C to be ready
    while (*I2C0_STAT0 & (1 << 1)); // Wait for BUSY=0
    
    // Send start condition for write
    *I2C0_CTL0 |= (1 << 8); // START=1
    while (!(*I2C0_STAT0 & (1 << 5))); // Wait for SBSEND=1
    
    // Send device address (write)
    *I2C0_DATA = (VL53L1X_DEVICE_ADDR << 1);
    while (!(*I2C0_STAT0 & (1 << 1))); // Wait for ADDSEND=1
    *I2C0_STAT0 = (1 << 3); // Clear ADDSEND
    
    // Send register address (16-bit - high byte first)
    *I2C0_DATA = (reg >> 8) & 0xFF;
    while (!(*I2C0_STAT0 & (1 << 7))); // Wait for BTC=1
    
    *I2C0_DATA = reg & 0xFF;
    while (!(*I2C0_STAT0 & (1 << 7))); // Wait for BTC=1
    
    // Send restart condition for read
    *I2C0_CTL0 |= (1 << 8); // START=1
    while (!(*I2C0_STAT0 & (1 << 5))); // Wait for SBSEND=1
    
    // Send device address (read)
    *I2C0_DATA = (VL53L1X_DEVICE_ADDR << 1) | 1;
    while (!(*I2C0_STAT0 & (1 << 1))); // Wait for ADDSEND=1
    *I2C0_STAT0 = (1 << 3); // Clear ADDSEND
    
    // Disable acknowledge for last byte
    *I2C0_CTL0 &= ~(1 << 10); // ACKEN=0
    
    // Wait for data received
    while (!(*I2C0_STAT0 & (1 << 6))); // Wait for RBNE=1
    data = *I2C0_DATA;
    
    // Send stop condition
    *I2C0_CTL0 |= (1 << 9); // STOP=1
    
    // Re-enable acknowledge
    *I2C0_CTL0 |= (1 << 10); // ACKEN=1
    
    return data;
}
