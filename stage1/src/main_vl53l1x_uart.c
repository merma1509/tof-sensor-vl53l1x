/**
 * @file  main_vl53l1x_uart.c
 * @brief Main application with UART command interface for VL53L1X
 */

#include "../include/uart_commands.h"
#include "../include/i2c_driver.h"
#include "../include/filter.h"

// Forward declarations
extern void uart_send_string(const char *str);

/* Filter state */
static filter_state_t distance_filter;

/* System clock initialization */
void SystemInit(void) {
    // Enable IRC48M internal oscillator
    volatile uint32_t *RCU_CTL = (uint32_t*)0x40021000;
    *RCU_CTL |= (1 << 7);  // IRC48MEN
    
    // Wait for IRC48M to be stable
    while (!(*RCU_CTL & (1 << 10))); // IRC48MSTB
    
    // Configure PLL: IRC48M / 2 * 2 = 48MHz
    volatile uint32_t *RCU_CFG0 = (uint32_t*)0x40021004;
    *RCU_CFG0 &= ~(0xF << 18); // Clear PLLMF
    *RCU_CFG0 |= (1 << 18);     // PLLMF = 2 (multiply by 2)
    
    // Select IRC48M/2 as PLL source
    *RCU_CFG0 |= (1 << 27); // PLLSEL = IRC48M
    
    // Enable PLL
    *RCU_CTL |= (1 << 24); // PLLEN
    
    // Wait for PLL to be stable
    while (!(*RCU_CTL & (1 << 25))); // PLLSTB
    
    // Select PLL as system clock
    *RCU_CFG0 &= ~(0x3 << 0); // Clear SCS
    *RCU_CFG0 |= (0x2 << 0);  // SCS = PLL
    
    // Wait for switch to complete
    while ((*RCU_CFG0 & 0x3) != 0x2);
}

/* Delay function */
void delay_ms(uint32_t ms) {
    volatile uint32_t count = ms * 48000; // Approximate for 48MHz
    while (count--) {
        __asm("nop");
    }
}

/* Test function for VL53L1X */
void test_vl53l1x_functionality(void) {
    uart_send_string("\r\n=== VL53L1X Functionality Test ===\r\n");
    
    // Test 1: Calibration
    uart_send_string("Test 1: Calibration...\r\n");
    uart_process_command("CAL");
    delay_ms(1000);
    
    // Test 2: Short range mode
    uart_send_string("Test 2: Short range mode...\r\n");
    uart_process_command("SHORT");
    delay_ms(1000);
    
    // Test 3: Start ranging
    uart_send_string("Test 3: Start ranging...\r\n");
    uart_process_command("START");
    delay_ms(2000);
    
    // Test 4: Check status
    uart_send_string("Test 4: Check status...\r\n");
    uart_process_command("STATUS");
    delay_ms(1000);
    
    // Test 5: Medium range mode
    uart_send_string("Test 5: Medium range mode...\r\n");
    uart_process_command("MED");
    delay_ms(2000);
    
    // Test 6: Check status
    uart_send_string("Test 6: Check status...\r\n");
    uart_process_command("STATUS");
    delay_ms(1000);
    
    // Test 7: Long range mode
    uart_send_string("Test 7: Long range mode...\r\n");
    uart_process_command("LONG");
    delay_ms(2000);
    
    // Test 8: Check status
    uart_send_string("Test 8: Check status...\r\n");
    uart_process_command("STATUS");
    delay_ms(1000);
    
    // Test 9: Stop ranging
    uart_send_string("Test 9: Stop ranging...\r\n");
    uart_process_command("STOP");
    delay_ms(1000);
    
    // Test 10: Final status
    uart_send_string("Test 10: Final status...\r\n");
    uart_process_command("STATUS");
    
    uart_send_string("\r\n=== Test Complete ===\r\n");
    uart_send_string("Ready for manual testing via UART\r\n");
    uart_send_string("Available commands: START, STOP, CAL, SHORT, MED, LONG, STATUS, HELP\r\n");
    uart_send_string(">");
}
/* Main application */
int main(void) {
    // Initialize system
    SystemInit();
    
    // Initialize I2C
    i2c_init();
    
    // Initialize UART
    uart_init();
    
    // Initialize filter
    filter_init(&distance_filter);
    
    // Show welcome message
    uart_send_string("\r\nVL53L1X GD32E230 UART Interface\r\n");
    uart_send_string("Version 1.1 - With Combined Filter\r\n");
    
    // Run automated test sequence
    test_vl53l1x_functionality();
    
    // Main loop for Hardware Reading
    while (1) {
        // Check if measurement is ready using I2C
        uint8_t status = i2c_read_register(0x013); // VL53L1X_GPIO_HV_MUX_ACTIVE
        uint8_t data_ready = (status & 0x01) ? 1 : 0;   // Check data ready bit
        if (data_ready) {
            // Read 16-bit distance from VL53L1X registers
            uint8_t distance_high = i2c_read_register(0x096); // VL53L1X_RESULT_DISTANCE_HIGH
            uint8_t distance_low = i2c_read_register(0x097); // VL53L1X_RESULT_DISTANCE_LOW
            uint16_t distance = (distance_high << 8) | distance_low;
            
            // Apply filter if enabled (compile-time and runtime)
            uint16_t output_distance;
            #if FILTER_ENABLED
                if (filter_get_enabled()) {
                    output_distance = filter_process(&distance_filter, distance);
                } else {
                    output_distance = distance;  // Filter disabled at runtime
                }
            #else
                output_distance = distance;  // Filter disabled at compile-time
            #endif
            
            uart_send_distance(output_distance); // Send filtered distance to PC via UART
            // Clear interrupt using I2C
            i2c_write_register(0x011, 0x01); // VL53L1X_SYSTEM_INTERRUPT_CLEAR
        }
        
        // Process UART commands
        uart_command_loop();
        
        // Small delay
        delay_ms(10);
    }
    
    return 0;
}
