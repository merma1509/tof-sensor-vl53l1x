/**
 * @file  vl53l1_platform.c
 * @brief Platform abstraction layer implementation for VL53L1X API
 */

#include "vl53l1_platform.h"

// Simple delay function
void delay_ms(uint32_t ms) {
    volatile uint32_t count = ms * 48000; // Approximate for 48MHz
    while (count--) {
        __asm("nop");
    }
}

// Platform I2C write function
int32_t VL53L1X_WriteMulti(uint16_t index, uint8_t *pdata, uint32_t count) {
    if (count == 0) return 0;
    
    // Write register address first
    uint8_t reg_high = (index >> 8) & 0xFF;
    uint8_t reg_low = index & 0xFF;
    
    // Start condition
    volatile uint32_t *I2C0_CTL0 = (uint32_t*)0x40005400;
    volatile uint32_t *I2C0_STAT0 = (uint32_t*)0x40005408;
    volatile uint32_t *I2C0_DATA = (uint32_t*)0x4000540C;
    
    *I2C0_CTL0 |= (1 << 8); // START
    while (!(*I2C0_STAT0 & (1 << 5))); // Wait for SBSEND=1
    
    // Send device address (write)
    *I2C0_DATA = (VL53L1X_I2C_ADDRESS << 1);
    while (!(*I2C0_STAT0 & (1 << 1))); // Wait for ADDSEND=1
    *I2C0_STAT0 = (1 << 3); // Clear ADDSEND
    
    // Send register address
    *I2C0_DATA = reg_high;
    while (!(*I2C0_STAT0 & (1 << 7))); // Wait for BTC=1
    *I2C0_DATA = reg_low;
    while (!(*I2C0_STAT0 & (1 << 7))); // Wait for BTC=1
    
    // Send data
    for (uint32_t i = 0; i < count; i++) {
        *I2C0_DATA = pdata[i];
        while (!(*I2C0_STAT0 & (1 << 7))); // Wait for BTC=1
    }
    
    // Stop condition
    *I2C0_CTL0 |= (1 << 9); // STOP
    while (!(*I2C0_CTL0 & (1 << 8))); // Wait for START=0
    
    return 0;
}

// Platform I2C read function
int32_t VL53L1X_ReadMulti(uint16_t index, uint8_t *pdata, uint32_t count) {
    if (count == 0) return 0;
    
    // Write register address first (using existing function)
    uint8_t reg_high = (index >> 8) & 0xFF;
    uint8_t reg_low = index & 0xFF;
    
    volatile uint32_t *I2C0_CTL0 = (uint32_t*)0x40005400;
    volatile uint32_t *I2C0_STAT0 = (uint32_t*)0x40005408;
    volatile uint32_t *I2C0_DATA = (uint32_t*)0x4000540C;
    
    // Start condition
    *I2C0_CTL0 |= (1 << 8); // START
    while (!(*I2C0_STAT0 & (1 << 5))); // Wait for SBSEND=1
    
    // Send device address (write)
    *I2C0_DATA = (VL53L1X_I2C_ADDRESS << 1);
    while (!(*I2C0_STAT0 & (1 << 1))); // Wait for ADDSEND=1
    *I2C0_STAT0 = (1 << 3); // Clear ADDSEND
    
    // Send register address
    *I2C0_DATA = reg_high;
    while (!(*I2C0_STAT0 & (1 << 7))); // Wait for BTC=1
    *I2C0_DATA = reg_low;
    while (!(*I2C0_STAT0 & (1 << 7))); // Wait for BTC=1
    
    // Restart condition
    *I2C0_CTL0 |= (1 << 8); // START
    while (!(*I2C0_STAT0 & (1 << 5))); // Wait for SBSEND=1
    
    // Send device address (read)
    *I2C0_DATA = (VL53L1X_I2C_ADDRESS << 1) | 0x01;
    while (!(*I2C0_STAT0 & (1 << 1))); // Wait for ADDSEND=1
    *I2C0_STAT0 = (1 << 3); // Clear ADDSEND
    
    // Read data
    for (uint32_t i = 0; i < count; i++) {
        // Enable ACK for all but last byte
        if (i < count - 1) {
            *I2C0_CTL0 |= (1 << 10); // ACKEN
        } else {
            *I2C0_CTL0 &= ~(1 << 10); // No ACK for last byte
        }
        
        // Wait for data
        while (!(*I2C0_STAT0 & (1 << 6))); // Wait for RBNE=1
        pdata[i] = *I2C0_DATA;
    }
    
    // Stop condition
    *I2C0_CTL0 |= (1 << 9); // STOP
    while (!(*I2C0_CTL0 & (1 << 8))); // Wait for START=0
    
    return 0;
}

// Single byte write
int32_t VL53L1X_WrByte(uint16_t index, uint8_t data) {
    return VL53L1X_WriteMulti(index, &data, 1);
}

// Single byte read
int32_t VL53L1X_RdByte(uint16_t index, uint8_t *data) {
    return VL53L1X_ReadMulti(index, data, 1);
}

// Platform delay
void VL53L1X_PollingDelay(uint32_t n) {
    delay_ms(n);
}

// Additional platform functions needed by VL53L1X API
int32_t VL53L1X_WrWord(uint16_t index, uint16_t data) {
    uint8_t buffer[2] = {(data >> 8) & 0xFF, data & 0xFF};
    return VL53L1X_WriteMulti(index, buffer, 2);
}

int32_t VL53L1X_RdWord(uint16_t index, uint16_t *data) {
    uint8_t buffer[2];
    int32_t status = VL53L1X_ReadMulti(index, buffer, 2);
    if (status == 0) {
        *data = (buffer[0] << 8) | buffer[1];
    }
    return status;
}

int32_t VL53L1X_WrDWord(uint16_t index, uint32_t data) {
    uint8_t buffer[4] = {(data >> 24) & 0xFF, (data >> 16) & 0xFF, (data >> 8) & 0xFF, data & 0xFF};
    return VL53L1X_WriteMulti(index, buffer, 4);
}

int32_t VL53L1X_RdDWord(uint16_t index, uint32_t *data) {
    uint8_t buffer[4];
    int32_t status = VL53L1X_ReadMulti(index, buffer, 4);
    if (status == 0) {
        *data = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
    }
    return status;
}
