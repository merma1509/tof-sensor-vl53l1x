/**
 * @file  uart_commands.c
 * @brief UART command interface implementation for VL53L1X control
 */

#include "../include/uart_commands.h"
#include "../include/i2c_driver.h"
#include <string.h>
#include <stdio.h>

/* Delay function */
static void delay_ms(uint32_t ms) {
    volatile uint32_t count = ms * 48000; // Approximate for 48MHz
    while (count--) {
        __asm("nop");
    }
}

/* Global variables */
static uint8_t measuring = 0;
static uint16_t current_distance = 0;

/* UART send string function */
void uart_send_string(const char *str) {
    while (*str) {
        // Wait for transmit buffer empty
        while (!(*(volatile uint32_t*)0x40011004 & (1 << 7))); // USART_STAT_TBE
        *(volatile uint32_t*)0x40011008 = *str; // USART_DATA
        str++;
    }
}

/* UART send number function */
static void uart_send_number(uint16_t num) {
    char buffer[6];
    int i = 4;
    buffer[5] = '\0';
    
    do {
        buffer[i] = '0' + (num % 10);
        num /= 10;
        i--;
    } while (num > 0);
    
    while (i >= 0) {
        buffer[i] = ' ';
        i--;
    }
    
    uart_send_string(buffer);
}

/* Initialize UART command interface */
void uart_commands_init(void) {
    measuring = 0;
    current_distance = 0;
    uart_send_string("\r\nVL53L1X Ready>");
}

/* Process UART command */
void uart_process_command(char *cmd) {
    // Convert to uppercase
    for (int i = 0; cmd[i]; i++) {
        if (cmd[i] >= 'a' && cmd[i] <= 'z') {
            cmd[i] -= 32;
        }
    }
    
    if (strcmp(cmd, CMD_START) == 0) {
        if (!measuring) {
            // Start VL53L1X ranging using I2C
            // Write to start register
            uint8_t start_result = i2c_write_register(0x010, 0x01); // VL53L1X_SYSRANGE_START
            if (start_result == 0) {
                measuring = 1;
                uart_send_response(RESP_OK);
                uart_send_string(" - Ranging started\r\n");
            } else {
                uart_send_response(RESP_ERROR);
                uart_send_string(" - Failed to start\r\n");
            }
        } else {
            uart_send_response(RESP_BUSY);
            uart_send_string(" - Already measuring\r\n");
        }
    }
    else if (strcmp(cmd, CMD_STOP) == 0) {
        if (measuring) {
            // Stop VL53L1X ranging
            uint8_t stop_result = i2c_write_register(0x010, 0x00); // VL53L1X_SYSRANGE_STOP
            if (stop_result == 0) {
                measuring = 0;
                uart_send_response(RESP_OK);
                uart_send_string(" - Ranging stopped\r\n");
            } else {
                uart_send_response(RESP_ERROR);
                uart_send_string(" - Failed to stop\r\n");
            }
        } else {
            uart_send_response(RESP_READY);
            uart_send_string(" - Not measuring\r\n");
        }
    }
    else if (strcmp(cmd, CMD_CALIBRATE) == 0) {
        // Perform comprehensive calibration using I2C
        uart_send_string("Calibrating...");
        
        // Step 1: Soft reset
        uint8_t reset_result = i2c_write_register(0x0000, 0x00); // VL53L1X_SOFT_RESET
        delay_ms(10);
        
        // Step 2: Clear interrupt
        uint8_t clear_result = i2c_write_register(0x0086, 0x01); // VL53L1X_SYSTEM_INTERRUPT_CLEAR
        delay_ms(5);
        
        // Step 3: Set measurement timing budget to 100ms for calibration
        uint8_t timing_result = i2c_write_register(0x0096, 0x00); // Low byte
        uint8_t timing_high = i2c_write_register(0x0097, 0x27); // High byte (10000us = 100ms)
        
        // Step 4: Set distance mode to long for calibration
        uint8_t mode_result = i2c_write_register(0x0011, 0x03); // VL53L1X_DISTANCE_MODE_LONG
        
        // Step 5: Start ranging for calibration
        uint8_t start_result = i2c_write_register(0x0087, 0x01); // VL53L1X_SYSTEM_MODE_START
        
        // Wait for calibration to complete
        delay_ms(100);
        
        // Step 6: Stop ranging
        uint8_t stop_result = i2c_write_register(0x0087, 0x00); // VL53L1X_SYSTEM_MODE_STOP
        
        // Check calibration results
        if (reset_result == 0 && clear_result == 0 && timing_result == 0 && 
            timing_high == 0 && mode_result == 0 && start_result == 0 && stop_result == 0) {
            uart_send_response(RESP_OK);
            uart_send_string(" - Calibration complete\r\n");
        } else {
            uart_send_response(RESP_ERROR);
            uart_send_string(" - Calibration failed\r\n");
        }
    }
    else if (strcmp(cmd, CMD_MODE_SHORT) == 0) {
        // Set short range mode using I2C
        uart_send_string("Setting short range mode...");
        
        // Step 1: Stop current ranging
        i2c_write_register(0x0087, 0x00); // VL53L1X_SYSTEM_MODE_STOP
        delay_ms(10);
        
        // Step 2: Set distance mode to short
        uint8_t mode_result = i2c_write_register(0x0011, 0x01); // VL53L1X_DISTANCE_MODE_SHORT
        
        // Step 3: Set timing budget to 15ms
        uint8_t timing_low = i2c_write_register(0x0096, 0xB0); // Low byte (15000us)
        uint8_t timing_high = i2c_write_register(0x0097, 0x03); // High byte
        
        // Step 4: Configure phase cal period for short range
        uint8_t phase_cal = i2c_write_register(0x0030, 0x04); // Short range phase cal
        
        if (mode_result == 0 && timing_low == 0 && timing_high == 0 && phase_cal == 0) {
            uart_send_response(RESP_OK);
            uart_send_string(" - Short range mode (15ms)\r\n");
        } else {
            uart_send_response(RESP_ERROR);
            uart_send_string(" - Failed to set short mode\r\n");
        }
    }
    else if (strcmp(cmd, CMD_MODE_MEDIUM) == 0) {
        // Set medium range mode using I2C
        uart_send_string("Setting medium range mode...");
        
        // Step 1: Stop current ranging
        i2c_write_register(0x0087, 0x00); // VL53L1X_SYSTEM_MODE_STOP
        delay_ms(10);
        
        // Step 2: Set distance mode to medium
        uint8_t mode_result = i2c_write_register(0x0011, 0x02); // VL53L1X_DISTANCE_MODE_MEDIUM
        
        // Step 3: Set timing budget to 50ms
        uint8_t timing_low = i2c_write_register(0x0096, 0x88); // Low byte (50000us)
        uint8_t timing_high = i2c_write_register(0x0097, 0x0C); // High byte
        
        // Step 4: Configure phase cal period for medium range
        uint8_t phase_cal = i2c_write_register(0x0030, 0x06); // Medium range phase cal
        
        if (mode_result == 0 && timing_low == 0 && timing_high == 0 && phase_cal == 0) {
            uart_send_response(RESP_OK);
            uart_send_string(" - Medium range mode (50ms)\r\n");
        } else {
            uart_send_response(RESP_ERROR);
            uart_send_string(" - Failed to set medium mode\r\n");
        }
    }
    else if (strcmp(cmd, CMD_MODE_LONG) == 0) {
        // Set long range mode using I2C
        uart_send_string("Setting long range mode...");
        
        // Step 1: Stop current ranging
        i2c_write_register(0x0087, 0x00); // VL53L1X_SYSTEM_MODE_STOP
        delay_ms(10);
        
        // Step 2: Set distance mode to long
        uint8_t mode_result = i2c_write_register(0x0011, 0x03); // VL53L1X_DISTANCE_MODE_LONG
        
        // Step 3: Set timing budget to 100ms
        uint8_t timing_low = i2c_write_register(0x0096, 0x10); // Low byte (100000us)
        uint8_t timing_high = i2c_write_register(0x0097, 0x86); // High byte
        
        // Step 4: Configure phase cal period for long range
        uint8_t phase_cal = i2c_write_register(0x0030, 0x08); // Long range phase cal
        
        if (mode_result == 0 && timing_low == 0 && timing_high == 0 && phase_cal == 0) {
            uart_send_response(RESP_OK);
            uart_send_string(" - Long range mode (100ms)\r\n");
        } else {
            uart_send_response(RESP_ERROR);
            uart_send_string(" - Failed to set long mode\r\n");
        }
    }
    else if (strcmp(cmd, CMD_STATUS) == 0) {
        // Show current status
        uart_send_string("Status: ");
        if (measuring) {
            uart_send_string("Measuring");
            
            // Try to get current distance - read result register
            uint8_t distance_high = i2c_read_register(0x0096); // VL53L1X_RESULT_DISTANCE_HIGH
            uint8_t distance_low = i2c_read_register(0x0097); // VL53L1X_RESULT_DISTANCE_LOW
            uint16_t distance = (distance_high << 8) | distance_low;
            
            uart_send_string(", Distance: ");
            uart_send_number(distance);
            uart_send_string(" mm");
            
            // Read current distance mode
            uint8_t mode = i2c_read_register(0x0011); // VL53L1X_DISTANCE_MODE
            uart_send_string(", Mode: ");
            if (mode == 0x01) {
                uart_send_string("Short");
            } else if (mode == 0x02) {
                uart_send_string("Medium");
            } else if (mode == 0x03) {
                uart_send_string("Long");
            } else {
                uart_send_string("Unknown");
            }
            
            // Read current timing budget
            uint8_t timing_low = i2c_read_register(0x0096);
            uint8_t timing_high = i2c_read_register(0x0097);
            uint16_t timing = (timing_high << 8) | timing_low;
            uart_send_string(", Timing: ");
            uart_send_number(timing / 1000);
            uart_send_string("ms");
            
            current_distance = distance;
            uart_send_string("\r\n");
        } else {
            uart_send_string("Idle");
            
            // Read current distance mode
            uint8_t mode = i2c_read_register(0x0011); // VL53L1X_DISTANCE_MODE
            uart_send_string(", Mode: ");
            if (mode == 0x01) {
                uart_send_string("Short");
            } else if (mode == 0x02) {
                uart_send_string("Medium");
            } else if (mode == 0x03) {
                uart_send_string("Long");
            } else {
                uart_send_string("Unknown");
            }
            uart_send_string("\r\n");
        }
    }
    else if (strcmp(cmd, CMD_HELP) == 0) {
        uart_show_help();
    }
    else if (strcmp(cmd, "START") == 0) {
        measuring = 1;
        uart_send_string("Measuring started...");
    } else if (strcmp(cmd, "STOP") == 0) {
        measuring = 0;
        uart_send_string("Measuring stopped");
    } else if (strcmp(cmd, "GET") == 0) {
        uint8_t distance_high = i2c_read_register(0x0096);
        uint8_t distance_low = i2c_read_register(0x0097);
        uint16_t distance = (distance_high << 8) | distance_low;
        
        if (distance != 0) {
            current_distance = distance;
            char buffer[50];
            sprintf(buffer, "Distance: %d mm", distance);
            uart_send_string(buffer);
        } else {
            uart_send_string("Error reading distance");
        }
    } else {
        uart_send_string("Unknown command");
    }
}

void uart_command_loop(void) {
    uart_send_string("VL53L1X>");
}

/* Send response */
void uart_send_response(const char *response) {
    uart_send_string(response);
}

/* Send distance measurement */
void uart_send_distance(uint16_t distance) {
    current_distance = distance;
    uart_send_string("DIST: ");
    uart_send_number(distance);
    uart_send_string(" mm\r\n");
}

/* Command list */
const char *commands[] = {
    "START",
    "STOP", 
    "GET",
    "STATUS",
    "CALIBRATE",
    "MODE_SHORT",
    "MODE_MEDIUM", 
    "MODE_LONG",
    "FILTER",
    "WINDOW",
    "ENABLE", 
    "RESET",
    "STATUS",
    "HELP"
};

/* Show help information */
void uart_show_help(void) {
    uart_send_string("\r\n=== VL53L1X Distance Sensor ===\r\n");
    uart_send_string("Commands:\r\n");
    uart_send_string("  START     - Start distance measurement\r\n");
    uart_send_string("  STOP      - Stop distance measurement\r\n");
    uart_send_string("  GET       - Get current distance\r\n");
    uart_send_string("  STATUS    - Show system status\r\n");
    uart_send_string("  CALIBRATE - Calibrate sensor\r\n");
    uart_send_string("  MODE_*   - Set distance mode:\r\n");
    uart_send_string("    SHORT   - Short range mode\r\n");
    uart_send_string("    MEDIUM  - Medium range mode\r\n");
    uart_send_string("    LONG    - Long range mode\r\n");
    uart_send_string("  FILTER    - Filter commands:\r\n");
    uart_send_string("    TYPE <none|moving|median|exponential|kalman>\r\n");
    uart_send_string("    WINDOW <2-32> - Set filter window size\r\n");
    uart_send_string("    ENABLE <on|off> - Enable/disable filter\r\n");
    uart_send_string("    RESET - Reset filter state\r\n");
    uart_send_string("    STATUS - Show filter status\r\n");
    uart_send_string("  HELP      - Show this help\r\n");
    uart_send_string("\r\nExamples:\r\n");
    uart_send_string("  FILTER TYPE moving WINDOW 8 ENABLE\r\n");
    uart_send_string("  START\r\n");
    uart_send_string("  FILTER STATUS\r\n");
    uart_send_string("\r\n");
}
