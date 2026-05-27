/**
 * @file mik32_uart_hello.c
 * @brief MIK32 UART Hello World test
 */

#include "../platform/platform.h"
#include "../app/tof_service.h"

int main(void) {
    // Initialize platform
    platform_status_t status = platform_init();
    if (status != PLATFORM_OK) {
        // If platform init fails, hang
        while(1);
    }
    
    // Initialize UART
    status = platform_get_interface()->uart_init(115200);
    if (status != PLATFORM_OK) {
        while(1);
    }
    
    // Send Hello World message
    const char* hello_msg = "MIK32 VL53L1 TOF Sensor - Hello World!\r\n";
    platform_get_interface()->uart_send_string(hello_msg);
    
    // Send system information
    platform_get_interface()->uart_send_string("System Clock: ");
    uint32_t clock = platform_get_interface()->get_system_clock();
    char clock_str[16];
    // Simple integer to string conversion
    char* ptr = clock_str + sizeof(clock_str) - 1;
    *ptr = '\0';
    if (clock == 0) {
        *--ptr = '0';
    } else {
        while (clock > 0) {
            *--ptr = '0' + (clock % 10);
            clock /= 10;
        }
    }
    platform_get_interface()->uart_send_string(ptr);
    platform_get_interface()->uart_send_string(" Hz\r\n");
    
    // Main loop - blink LED indicator by toggling GPIO
    uint32_t led_pin = 0; // Use P0.0 as LED
    platform_get_interface()->gpio_init(led_pin, 1); // Output mode
    
    while(1) {
        // Toggle LED
        static uint32_t led_state = 0;
        led_state = !led_state;
        platform_get_interface()->gpio_set(led_pin, led_state);
        
        // Send status message every 2 seconds
        static uint32_t counter = 0;
        counter++;
        if (counter >= 2000) {
            platform_get_interface()->uart_send_string("MIK32 System Running\r\n");
            counter = 0;
        }
        
        // Delay
        platform_get_interface()->delay_ms(1);
    }
    
    return 0;
}
