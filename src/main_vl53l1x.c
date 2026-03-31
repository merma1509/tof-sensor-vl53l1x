/**
 * @file  main_vl53l1x.c
 * @brief VL53L1X implementation for GD32E230
 */

#include <stdint.h>

/* VL53L1X register addresses */
#define VL53L1X_SOFTWARE_RESET          0x0000
#define VL53L1X_RESULT_INTERRUPT_STATUS   0x0013
#define VL53L1X_RESULT_FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0 0x0096
#define VL53L1X_RESULT_FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD1 0x0097
#define VL53L1X_SYSTEM_INTERRUPT_CLEAR   0x0086
#define VL53L1X_SYSTEM_MODE_START       0x0087
#define VL53L1X_SYSTEM_INTERRUPT_ENABLE_GPIO 0x000A
#define VL53L1X_SYSTEM_RANGE_CONFIG      0x0011
#define VL53L1X_DEVICE_ID               0x010F
#define VL53L1X_DEVICE_ID_LOW           0x0110

/* System initialization */
void SystemInit(void) {
    // Enable GPIOA and GPIOB clocks
    volatile uint32_t *RCU_AHBEN = (uint32_t*)0x40021014;
    *RCU_AHBEN |= (1 << 17) | (1 << 18); // GPIOA, GPIOB
    
    // Configure UART pins
    volatile uint32_t *GPIO_CTL0 = (uint32_t*)0x48000008;
    *GPIO_CTL0 |= (1 << 4) | (1 << 5); // PA9 (TX), PA10 (RX)
    
    // Configure I2C pins
    *GPIO_CTL0 |= (1 << 6) | (1 << 7); // PB6 (SDA), PB7 (SCL)
}

/* Delay function */
void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 1000; i++);
}

/* I2C functions */
uint8_t i2c_read_register(uint16_t reg) {
    // Real I2C implementation for GD32E230
    volatile uint32_t *I2C_CTL0 = (uint32_t*)0x40005400;
    volatile uint32_t *I2C_DATA = (uint32_t*)0x40005418;
    volatile uint32_t *I2C_STAT = (uint32_t*)0x40005410;
    
    // Start I2C communication
    *I2C_CTL0 |= (1 << 0); // Enable I2C
    *I2C_CTL0 |= (1 << 8); // Start condition
    
    // Send device address (0x52 write)
    while (!(*I2C_STAT & (1 << 13))); // Wait for address sent
    *I2C_DATA = 0xA4; // 0x52 << 1 | write
    
    // Send register address (16-bit)
    while (!(*I2C_STAT & (1 << 13))); // Wait for address sent
    *I2C_DATA = (reg >> 8) & 0xFF; // High byte
    
    while (!(*I2C_STAT & (1 << 13))); // Wait for address sent
    *I2C_DATA = reg & 0xFF; // Low byte
    
    // Restart condition for read
    *I2C_CTL0 |= (1 << 8); // Start condition
    *I2C_DATA = 0xA5; // 0x52 << 1 | read
    
    // Read data
    while (!(*I2C_STAT & (1 << 6))); // Wait for data ready
    uint8_t data = *I2C_DATA;
    
    // Stop condition
    *I2C_CTL0 |= (1 << 9); // Stop condition
    
    return data;
}

uint8_t i2c_write_register(uint16_t reg, uint8_t data) {
    // Real I2C implementation for GD32E230
    volatile uint32_t *I2C_CTL0 = (uint32_t*)0x40005400;
    volatile uint32_t *I2C_DATA = (uint32_t*)0x40005418;
    volatile uint32_t *I2C_STAT = (uint32_t*)0x40005410;
    
    // Start I2C communication
    *I2C_CTL0 |= (1 << 0); // Enable I2C
    *I2C_CTL0 |= (1 << 8); // Start condition
    
    // Send device address (0x52 write)
    while (!(*I2C_STAT & (1 << 13))); // Wait for address sent
    *I2C_DATA = 0xA4; // 0x52 << 1 | write
    
    // Send register address (16-bit)
    while (!(*I2C_STAT & (1 << 13))); // Wait for address sent
    *I2C_DATA = (reg >> 8) & 0xFF; // High byte
    
    while (!(*I2C_STAT & (1 << 13))); // Wait for address sent
    *I2C_DATA = reg & 0xFF; // Low byte
    
    // Send data
    while (!(*I2C_STAT & (1 << 13))); // Wait for address sent
    *I2C_DATA = data;
    
    // Stop condition
    *I2C_CTL0 |= (1 << 9); // Stop condition
    
    return 0x00; // Success
}

/* UART functions */
void UART_SendString(const char *str) {
    volatile uint32_t *UART0_STAT = (uint32_t*)0x40011020;
    volatile uint32_t *UART0_DATA = (uint32_t*)0x40011014;
    
    while (*str) {
        // Wait for transmit buffer empty
        while (!(*UART0_STAT & (1 << 7)));
        *UART0_DATA = *str;
        str++;
    }
}

void UART_SendDistance(uint16_t distance) {
    // Distance formatting
    char buffer[16];
    int i = 0;
    
    // Convert distance to string
    if (distance == 0) {
        buffer[i++] = '0';
    } else {
        while (distance > 0) {
            buffer[i++] = '0' + (distance % 10);
            distance /= 10;
        }
    }
    
    buffer[i++] = ' ';
    buffer[i++] = 'm';
    buffer[i++] = 'm';
    buffer[i++] = '\r';
    buffer[i++] = '\n';
    buffer[i] = '\0';
    
    UART_SendString(buffer);
}

/* VL53L1X functions */
uint8_t VL53L1X_Initialize(void) {
    uint16_t device_id_high = i2c_read_register(VL53L1X_DEVICE_ID);
    uint16_t device_id_low = i2c_read_register(VL53L1X_DEVICE_ID_LOW);
    uint16_t device_id = (device_id_high << 8) | device_id_low;
    
    // Check if device ID matches VL53L1X (0xEACC)
    if (device_id != 0xEACC) {
        return 0; // Error: device not found
    }
    
    // Software reset
    i2c_write_register(VL53L1X_SOFTWARE_RESET, 0x00);
    delay_ms(10);
    i2c_write_register(VL53L1X_SOFTWARE_RESET, 0x01);
    delay_ms(10);
    
    // Configure for short range mode
    i2c_write_register(VL53L1X_SYSTEM_RANGE_CONFIG, 0x02);
    
    // Enable interrupt
    i2c_write_register(VL53L1X_SYSTEM_INTERRUPT_ENABLE_GPIO, 0x01);
    
    // Start ranging
    i2c_write_register(VL53L1X_SYSTEM_MODE_START, 0x01);
    
    return 1; // Success
}

uint16_t VL53L1X_GetDistance(void) {
    // Check if data is ready
    uint8_t status = i2c_read_register(VL53L1X_RESULT_INTERRUPT_STATUS);
    if (!(status & 0x07)) {
        return 0; // No data ready
    }
    
    // Read distance (16-bit value)
    uint8_t distance_high = i2c_read_register(VL53L1X_RESULT_FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD0);
    uint8_t distance_low = i2c_read_register(VL53L1X_RESULT_FINAL_CROSSTALK_CORRECTED_RANGE_MM_SD1);
    uint16_t distance = (distance_high << 8) | distance_low;
    
    // Clear interrupt
    i2c_write_register(VL53L1X_SYSTEM_INTERRUPT_CLEAR, 0x01);
    
    return distance;
}

/* Main application */
int main(void) {
    uint16_t distance = 0;
    uint8_t init_result = 0;
    
    // Initialize system
    SystemInit();
    
    // Initialize UART
    UART_SendString("VL53L1X GD32E230 Implementation Ready\r\n");
    
    // Initialize VL53L1X
    UART_SendString("Initializing VL53L1X sensor...\r\n");
    init_result = VL53L1X_Initialize();
    
    if (init_result) {
        UART_SendString("VL53L1X initialized successfully!\r\n");
        UART_SendString("Starting distance measurements...\r\n");
    } else {
        UART_SendString("VL53L1X initialization failed!\r\n");
        UART_SendString("Using dummy distance values\r\n");
    }
    
    // Main loop
    while (1) {
        if (init_result) {
            // Real VL53L1X measurement
            distance = VL53L1X_GetDistance();
            
            if (distance > 0) {
                UART_SendString("Distance: ");
                UART_SendDistance(distance);
            } else {
                UART_SendString("No data ready\r\n");
            }
        } else {
            // Dummy distance for testing
            distance = 1234; // Dummy distance value
            UART_SendString("Distance (dummy): ");
            UART_SendDistance(distance);
        }
        
        delay_ms(1000); // 1 second delay
    }
    
    return 0;
}
