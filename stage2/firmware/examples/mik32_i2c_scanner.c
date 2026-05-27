/**
 * @file mik32_i2c_scanner.c
 * @brief MIK32 I2C Scanner for VL53L1 sensor detection
 */

#include "../platform/platform.h"

// VL53L1 I2C address
#define VL53L1_I2C_ADDR    0x29

int main(void) {
    // Initialize platform
    platform_status_t status = platform_init();
    if (status != PLATFORM_OK) {
        while(1);
    }
    
    // Initialize UART for output
    status = platform_get_interface()->uart_init(115200);
    if (status != PLATFORM_OK) {
        while(1);
    }
    
    // Initialize I2C
    status = platform_get_interface()->i2c_init(100000);
    if (status != PLATFORM_OK) {
        platform_get_interface()->uart_send_string("I2C Init Failed\r\n");
        while(1);
    }
    
    platform_get_interface()->uart_send_string("MIK32 I2C Scanner\r\n");
    platform_get_interface()->uart_send_string("==================\r\n");
    
    // Scan I2C addresses from 0x08 to 0x77
    uint8_t devices_found = 0;
    for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
        // Try to read from device
        uint8_t test_data = 0;
        status = platform_get_interface()->i2c_read(addr, &test_data, 1);
        
        if (status == PLATFORM_OK) {
            devices_found++;
            platform_get_interface()->uart_send_string("Device found at 0x");
            
            // Convert address to hex string
            char addr_str[4];
            addr_str[0] = "0123456789ABCDEF"[(addr >> 4) & 0xF];
            addr_str[1] = "0123456789ABCDEF"[addr & 0xF];
            addr_str[2] = '\r';
            addr_str[3] = '\n';
            platform_get_interface()->uart_send(addr_str, 4);
        }
        
        platform_get_interface()->delay_ms(10); // Small delay between scans
    }
    
    // Report scan results
    if (devices_found == 0) {
        platform_get_interface()->uart_send_string("No I2C devices found\r\n");
    } else {
        platform_get_interface()->uart_send_string("Scan complete. ");
        
        // Convert device count to string
        char count_str[16];
        char* ptr = count_str + sizeof(count_str) - 1;
        *ptr = '\0';
        uint8_t temp = devices_found;
        if (temp == 0) {
            *--ptr = '0';
        } else {
            while (temp > 0) {
                *--ptr = '0' + (temp % 10);
                temp /= 10;
            }
        }
        platform_get_interface()->uart_send_string(ptr);
        platform_get_interface()->uart_send_string(" devices found\r\n");
    }
    
    // Test VL53L1 specifically
    platform_get_interface()->uart_send_string("\r\nTesting VL53L1 at 0x29...\r\n");
    
    // Try to read VL53L1 device ID register (0x010F)
    uint8_t device_id[2];
    status = platform_get_interface()->i2c_read_reg(VL53L1_I2C_ADDR, 0x010F, device_id, 2);
    
    if (status == PLATFORM_OK) {
        platform_get_interface()->uart_send_string("VL53L1 Device ID: 0x");
        
        // Convert device ID to hex string
        char id_str[8];
        id_str[0] = "0123456789ABCDEF"[(device_id[0] >> 4) & 0xF];
        id_str[1] = "0123456789ABCDEF"[device_id[0] & 0xF];
        id_str[2] = "0123456789ABCDEF"[(device_id[1] >> 4) & 0xF];
        id_str[3] = "0123456789ABCDEF"[device_id[1] & 0xF];
        id_str[4] = '\r';
        id_str[5] = '\n';
        platform_get_interface()->uart_send(id_str, 6);
        
        // Check if this is a valid VL53L1 ID (should be 0xEACC)
        if (device_id[0] == 0xEA && device_id[1] == 0xCC) {
            platform_get_interface()->uart_send_string("VL53L1 detected successfully!\r\n");
        } else {
            platform_get_interface()->uart_send_string("Unexpected device ID\r\n");
        }
    } else {
        platform_get_interface()->uart_send_string("Failed to read VL53L1 device ID\r\n");
    }
    
    // Main loop
    while(1) {
        platform_get_interface()->delay_ms(1000);
    }
    
    return 0;
}
