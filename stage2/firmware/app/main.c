/**
 * @file main.c
 * @brief Main application entry point
 */

#include "../config/platform_config.h"
#include "../platform/platform.h"
#include "tof_service.h"
#include "command_parser.h"


/* Application state */
static uint8_t app_running = 1;

/* Forward declarations */
static void app_system_init(void);
static void app_main_loop(void);
static void app_handle_commands(void);

/* Main application entry point */
int main(void) {
    // Initialize platform
    if (platform_init() != PLATFORM_OK) {
        // Platform initialization failed
        while(1) {
            // Blink LED or indicate error
        }
    }
    
    // Initialize application
    app_system_init();
    
    // Main application loop
    app_main_loop();
    
    // Should never reach here
    return 0;
}

/* System initialization */
static void app_system_init(void) {
    DEBUG_PRINT("=== VL53L1 TOF Sensor Application ===\r\n");
    DEBUG_PRINT("Platform: ");
#if TARGET_PLATFORM == PLATFORM_GD32
    DEBUG_PRINT("GD32E230\r\n");
#elif TARGET_PLATFORM == PLATFORM_MIK32
    DEBUG_PRINT("MIK32 Amur\r\n");
#else
    DEBUG_PRINT("Unknown\r\n");
#endif
    
    DEBUG_PRINT("System Clock: %d Hz\r\n", SYSTEM_CLOCK_HZ);
    DEBUG_PRINT("UART Baudrate: %d\r\n", UART_BAUDRATE);
    DEBUG_PRINT("I2C Frequency: %d Hz\r\n", I2C_FREQUENCY);
    
    // Initialize TOF service
    if (tof_service_init() != 0) {
        DEBUG_PRINT("ERROR: TOF service initialization failed\r\n");
        while(1) {
            // Indicate error
        }
    }
    
    // Initialize command parser
    if (command_parser_init() != 0) {
        DEBUG_PRINT("ERROR: Command parser initialization failed\r\n");
        while(1) {
            // Indicate error
        }
    }
    
    DEBUG_PRINT("System initialization complete\r\n");
    DEBUG_PRINT("Type 'HELP' for available commands\r\n");
    DEBUG_PRINT("Ready>\r\n");
}

/* Main application loop */
static void app_main_loop(void) {
    while (app_running) {
        // Handle incoming commands
        app_handle_commands();
        
        // Process TOF measurements if ranging is active
        if (tof_service_get_state() == TOF_STATE_RANGING) {
            uint16_t distance;
            if (tof_service_get_distance(&distance) == 0) {
                DEBUG_PRINT("Distance: %d mm (= %d.%02d m)\r\n", 
                           distance, distance / 1000, (distance % 1000) / 10);
            }
        }
        
        // Small delay to prevent busy loop
        PLATFORM_DELAY_MS(100);
    }
}

/* Command handling */
static void app_handle_commands(void) {
    // TODO: Implement UART command reception and parsing
    // For now, this is a placeholder
    
    // Check for incoming UART data
    uint8_t rx_buffer[64];
    if (PLATFORM_UART_RECV(rx_buffer, sizeof(rx_buffer), 0) == PLATFORM_OK) {
        // Process received command
        command_parser_process((char*)rx_buffer);
    }
}
