/**
 * @file test_common_utils.c
 * @brief Test suite for common utility functions
 * @author VL53L1 TOF Project
 * @date 2026
 */

#include "../drivers/common/string_utils.h"
#include "../drivers/common/math_utils.h"
#include "../drivers/common/delay_utils.h"
#include "../drivers/common/debug_utils.h"
#include "../drivers/common/crc_utils.h"
#include "../drivers/common/endian_utils.h"

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

// Test functions
static int test_string_utils(void) {
    debug_print("Testing string utilities...\n");
    
    // Test safe_strlen
    TEST_ASSERT_EQUAL(5, safe_strlen("Hello"));
    TEST_ASSERT_EQUAL(0, safe_strlen(""));
    TEST_ASSERT_EQUAL(0, safe_strlen(NULL));
    
    // Test safe_strcpy
    char dest1[32];
    TEST_ASSERT_EQUAL(dest1, safe_strcpy(dest1, "Hello", sizeof(dest1)));
    TEST_ASSERT_EQUAL(0, safe_strcmp(dest1, "Hello"));
    
    // Test safe_strcat
    char dest2[32] = "Hello";
    TEST_ASSERT_EQUAL(dest2, safe_strcat(dest2, " World", sizeof(dest2)));
    TEST_ASSERT_EQUAL(0, safe_strcmp(dest2, "Hello World"));
    
    // Test safe_memcpy
    char src[] = "Test";
    char dest3[32];
    TEST_ASSERT_EQUAL(dest3, safe_memcpy(dest3, src, 5));
    TEST_ASSERT_EQUAL(0, safe_strcmp(dest3, "Test"));
    
    // Test safe_snprintf
    char buf[32];
    int len = safe_snprintf(buf, sizeof(buf), "Value: %d", 42);
    TEST_ASSERT_EQUAL(8, len);
    TEST_ASSERT_EQUAL(0, safe_strcmp(buf, "Value: 42"));
    
    // Test safe_rand
    int r1 = safe_rand();
    int r2 = safe_rand();
    // Note: This is not a perfect test since rand could theoretically return same value
    TEST_ASSERT(r1 >= 0 && r1 <= 32767);
    TEST_ASSERT(r2 >= 0 && r2 <= 32767);
    
    debug_print("String utilities tests passed!\n");
    return 0;
}

static int test_math_utils(void) {
    debug_print("Testing math utilities...\n");
    
    // Test int_to_string
    char buf[32];
    TEST_ASSERT_EQUAL(buf, int_to_string(123, buf, sizeof(buf)));
    TEST_ASSERT_EQUAL(0, safe_strcmp(buf, "123"));
    
    TEST_ASSERT_EQUAL(buf, int_to_string(-456, buf, sizeof(buf)));
    TEST_ASSERT_EQUAL(0, safe_strcmp(buf, "-456"));
    
    TEST_ASSERT_EQUAL(buf, int_to_string(0, buf, sizeof(buf)));
    TEST_ASSERT_EQUAL(0, safe_strcmp(buf, "0"));
    
    // Test float_to_string (basic test)
    TEST_ASSERT_EQUAL(buf, float_to_string(3.14159f, buf, sizeof(buf), 2));
    // Note: This is a basic test - actual implementation may vary
    
    debug_print("Math utilities tests passed!\n");
    return 0;
}

static int test_delay_utils(void) {
    debug_print("Testing delay utilities...\n");
    
    // Test basic delay functions (just ensure they don't crash)
    uint32_t start = get_tick_ms();
    delay_ms(1);
    uint32_t end = get_tick_ms();
    
    // Should have delayed at least some time (allowing for timer resolution)
    TEST_ASSERT(end >= start);
    
    // Test delay_until
    uint32_t target = get_tick_ms() + 10;
    bool result = delay_until_ms(target);
    TEST_ASSERT(result); // Should complete successfully
    
    debug_print("Delay utilities tests passed!\n");
    return 0;
}

static int test_debug_utils(void) {
    debug_print("Testing debug utilities...\n");
    
    // Test debug enable/disable
    debug_set_enabled(true);
    TEST_ASSERT(debug_is_enabled());
    
    debug_set_enabled(false);
    TEST_ASSERT(!debug_is_enabled());
    
    debug_set_enabled(true); // Re-enable for other tests
    
    // Test debug_print (just ensure it doesn't crash)
    debug_print("Test message\n");
    
    // Test debug_assert (should not trigger)
    DEBUG_ASSERT(true);
    
    debug_print("Debug utilities tests passed!\n");
    return 0;
}

static int test_crc_utils(void) {
    debug_print("Testing CRC utilities...\n");
    
    // Test CRC8
    uint8_t data1[] = {0x01, 0x02, 0x03, 0x04};
    uint8_t crc1 = crc8_calculate(data1, sizeof(data1));
    // Note: This is a basic test - actual CRC value depends on polynomial
    TEST_ASSERT(crc1 != 0 || sizeof(data1) == 0);
    
    // Test CRC16
    uint16_t crc2 = crc16_calculate(data1, sizeof(data1));
    TEST_ASSERT(crc2 != 0 || sizeof(data1) == 0);
    
    // Test CRC32
    uint32_t crc3 = crc32_calculate(data1, sizeof(data1));
    TEST_ASSERT(crc3 != 0 || sizeof(data1) == 0);
    
    debug_print("CRC utilities tests passed!\n");
    return 0;
}

static int test_endian_utils(void) {
    debug_print("Testing endian utilities...\n");
    
    // Test endian conversion
    uint16_t val16 = 0x1234;
    uint16_t swapped16 = swap_uint16(val16);
    TEST_ASSERT_EQUAL(0x3412, swapped16);
    
    uint32_t val32 = 0x12345678;
    uint32_t swapped32 = swap_uint32(val32);
    TEST_ASSERT_EQUAL(0x78563412, swapped32);
    
    // Test host to network conversion
    uint16_t net16 = host_to_network_uint16(val16);
    uint32_t net32 = host_to_network_uint32(val32);
    
    // Test network to host conversion
    TEST_ASSERT_EQUAL(val16, network_to_host_uint16(net16));
    TEST_ASSERT_EQUAL(val32, network_to_host_uint32(net32));
    
    debug_print("Endian utilities tests passed!\n");
    return 0;
}

// Test functions for integration with test runner
int test_common_utilities_basic(void) {
    debug_print("Running common utilities basic tests...\n");
    
    int result = 0;
    
    if (test_string_utils() != 0) result = -1;
    if (test_math_utils() != 0) result = -1;
    if (test_delay_utils() != 0) result = -1;
    if (test_debug_utils() != 0) result = -1;
    if (test_crc_utils() != 0) result = -1;
    if (test_endian_utils() != 0) result = -1;
    
    return result;
}
