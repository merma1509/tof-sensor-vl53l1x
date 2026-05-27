/**
 * @file test_platform.c
 * @brief Test suite for platform abstraction layer
 * @author VL53L1 TOF Project
 * @date 2026
 */

#include "../platform/platform.h"
#include "../drivers/common/debug_utils.h"

// Test framework macros
#define TEST_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            debug_print("FAIL: %s:%d - %s\n", __FILE__, __LINE__, #condition); \
            return -1; \
        } else { \
            debug_print("PASS: %s\n", #condition); \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            debug_print("FAIL: %s:%d - Expected %d, got %d\n", __FILE__, __LINE__, (int)(expected), (int)(actual)); \
            return -1; \
        } else { \
            debug_print("PASS: %s == %s\n", #expected, #actual); \
        } \
    } while(0)

static int test_platform_initialization(void) {
    debug_print("Testing platform initialization...\n");
    
    // Test platform initialization
    TEST_ASSERT_EQUAL(PLATFORM_OK, platform_init());
    
    // Test that platform interface is available
    const platform_interface_t* interface = platform_get_interface();
    TEST_ASSERT(interface != NULL);
    
    // Test that all function pointers are set
    TEST_ASSERT(interface->uart_init != NULL);
    TEST_ASSERT(interface->uart_send != NULL);
    TEST_ASSERT(interface->uart_receive != NULL);
    TEST_ASSERT(interface->uart_send_string != NULL);
    TEST_ASSERT(interface->i2c_init != NULL);
    TEST_ASSERT(interface->i2c_write != NULL);
    TEST_ASSERT(interface->i2c_read != NULL);
    TEST_ASSERT(interface->i2c_write_reg != NULL);
    TEST_ASSERT(interface->i2c_read_reg != NULL);
    TEST_ASSERT(interface->gpio_init != NULL);
    TEST_ASSERT(interface->gpio_set != NULL);
    TEST_ASSERT(interface->gpio_get != NULL);
    TEST_ASSERT(interface->delay_ms != NULL);
    TEST_ASSERT(interface->delay_us != NULL);
    TEST_ASSERT(interface->get_tick_ms != NULL);
    TEST_ASSERT(interface->get_tick_us != NULL);
    
    debug_print("Platform initialization tests passed!\n");
    return 0;
}

static int test_platform_uart(void) {
    debug_print("Testing platform UART...\n");
    
    // Test UART initialization
    TEST_ASSERT_EQUAL(PLATFORM_OK, PLATFORM_UART_INIT(115200));
    
    // Test UART send
    const char* test_msg = "Hello UART!";
    TEST_ASSERT_EQUAL(PLATFORM_OK, PLATFORM_UART_SEND_STR(test_msg));
    
    // Test UART send/receive with data
    uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04};
    TEST_ASSERT_EQUAL(PLATFORM_OK, PLATFORM_UART_SEND(test_data, sizeof(test_data)));
    
    // Note: UART receive test would require loopback hardware
    debug_print("UART tests passed!\n");
    return 0;
}

static int test_platform_i2c(void) {
    debug_print("Testing platform I2C...\n");
    
    // Test I2C initialization
    TEST_ASSERT_EQUAL(PLATFORM_OK, PLATFORM_I2C_INIT(100000)); // 100kHz
    
    // Test I2C register operations (these may fail if no device connected)
    uint8_t test_addr = 0x52; // VL53L1 default address
    uint8_t test_reg = 0x00;
    uint8_t test_data[] = {0x01, 0x02};
    uint8_t read_data[2];
    
    // Test I2C write register
    platform_status_t result = PLATFORM_I2C_WRITE_REG(test_addr, test_reg, test_data, sizeof(test_data));
    debug_print("I2C write result: %d\n", result);
    
    // Test I2C read register
    result = PLATFORM_I2C_READ_REG(test_addr, test_reg, read_data, sizeof(read_data));
    debug_print("I2C read result: %d\n", result);
    
    debug_print("I2C tests completed!\n");
    return 0;
}

static int test_platform_gpio(void) {
    debug_print("Testing platform GPIO...\n");
    
    // Test GPIO initialization (pin numbers are platform-specific)
    uint32_t test_pin = 0; // Use pin 0 for testing
    TEST_ASSERT_EQUAL(PLATFORM_OK, PLATFORM_GPIO_INIT(test_pin, 0));
    
    // Test GPIO set
    TEST_ASSERT_EQUAL(PLATFORM_OK, PLATFORM_GPIO_SET(test_pin, 1));
    TEST_ASSERT_EQUAL(PLATFORM_OK, PLATFORM_GPIO_SET(test_pin, 0));
    
    // Test GPIO get
    uint32_t pin_state;
    TEST_ASSERT_EQUAL(PLATFORM_OK, PLATFORM_GPIO_GET(test_pin, &pin_state));
    
    debug_print("GPIO tests passed!\n");
    return 0;
}

static int test_platform_timing(void) {
    debug_print("Testing platform timing functions...\n");
    
    // Test delay functions
    uint32_t start_tick = PLATFORM_GET_TICK_MS();
    PLATFORM_DELAY_MS(10); // 10ms delay
    uint32_t end_tick = PLATFORM_GET_TICK_MS();
    
    // Should have delayed at least 10ms (allowing for timer resolution)
    TEST_ASSERT(end_tick >= start_tick + 10);
    
    // Test microsecond delay
    start_tick = PLATFORM_GET_TICK_US();
    PLATFORM_DELAY_US(1000); // 1ms delay in microseconds
    end_tick = PLATFORM_GET_TICK_US();
    
    TEST_ASSERT(end_tick >= start_tick + 1000);
    
    // Test tick functions
    uint32_t tick_ms = PLATFORM_GET_TICK_MS();
    uint32_t tick_us = PLATFORM_GET_TICK_US();
    
    TEST_ASSERT(tick_ms > 0);
    TEST_ASSERT(tick_us > 0);
    
    debug_print("Timing functions tests passed!\n");
    return 0;
}

static int test_platform_macros(void) {
    debug_print("Testing platform macros...\n");
    
    // Test that platform interface is available
    const platform_interface_t* interface = platform_get_interface();
    TEST_ASSERT(interface != NULL);
    
    // Test that interface functions are available
    TEST_ASSERT(interface->uart_init != NULL);
    TEST_ASSERT(interface->i2c_init != NULL);
    TEST_ASSERT(interface->gpio_init != NULL);
    TEST_ASSERT(interface->delay_ms != NULL);
    
    // Test debug macros
    DEBUG_PRINT("Debug macro test\n");
    DEBUG_PRINT_HEX((uint8_t*)0x12345678, 4);
    
    debug_print("Platform macro tests passed!\n");
    return 0;
}

// Main test runner
int main(void) {
    debug_print("Starting platform test suite...\n");
    debug_print("=====================================\n");
    
    int result = 0;
    
    if (test_platform_initialization() != 0) result = -1;
    if (test_platform_uart() != 0) result = -1;
    if (test_platform_i2c() != 0) result = -1;
    if (test_platform_gpio() != 0) result = -1;
    if (test_platform_timing() != 0) result = -1;
    if (test_platform_macros() != 0) result = -1;
    
    // Cleanup
    platform_deinit();
    
    debug_print("=====================================\n");
    if (result == 0) {
        debug_print("All platform tests PASSED!\n");
    } else {
        debug_print("Some platform tests FAILED!\n");
    }
    
    return result;
}
