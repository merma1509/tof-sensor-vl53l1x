/**
 * @file test_tof_service.c
 * @brief Test suite for TOF service functionality
 * @author VL53L1 TOF Project
 * @date 2026
 */

#include "../app/tof_service.h"
#include "../drivers/common/string_utils.h"
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

static int test_tof_service_basic_operations(void) {
    debug_print("Testing TOF service basic operations...\n");
    
    // Test initialization
    TEST_ASSERT_EQUAL(0, tof_service_init());
    
    // Test state after initialization
    TEST_ASSERT_EQUAL(TOF_STATE_READY, tof_service_get_state());
    
    // Test mode setting
    TEST_ASSERT_EQUAL(0, tof_service_set_mode(TOF_MODE_SHORT));
    TEST_ASSERT_EQUAL(TOF_MODE_SHORT, tof_service_get_mode());
    
    TEST_ASSERT_EQUAL(0, tof_service_set_mode(TOF_MODE_MEDIUM));
    TEST_ASSERT_EQUAL(TOF_MODE_MEDIUM, tof_service_get_mode());
    
    TEST_ASSERT_EQUAL(0, tof_service_set_mode(TOF_MODE_LONG));
    TEST_ASSERT_EQUAL(TOF_MODE_LONG, tof_service_get_mode());
    
    // Test invalid mode
    TEST_ASSERT_EQUAL(-1, tof_service_set_mode((tof_mode_t)99));
    
    debug_print("TOF service basic operations tests passed!\n");
    return 0;
}

static int test_tof_service_ranging(void) {
    debug_print("Testing TOF service ranging...\n");
    
    // Test start ranging
    TEST_ASSERT_EQUAL(0, tof_service_start_ranging());
    TEST_ASSERT_EQUAL(TOF_STATE_RANGING, tof_service_get_state());
    
    // Test distance measurement (may fail if no sensor connected)
    uint16_t distance;
    int result = tof_service_get_distance(&distance);
    debug_print("Distance measurement result: %d (distance: %d mm)\n", result, distance);
    
    // Test stop ranging
    TEST_ASSERT_EQUAL(0, tof_service_stop_ranging());
    TEST_ASSERT_EQUAL(TOF_STATE_READY, tof_service_get_state());
    
    debug_print("TOF service ranging tests passed!\n");
    return 0;
}

static int test_tof_service_filter(void) {
    debug_print("Testing TOF service filter...\n");
    
#if FILTER_ENABLED
    // Test filter enable/disable
    TEST_ASSERT_EQUAL(0, tof_service_filter_enable());
    
    uint8_t enabled;
    TEST_ASSERT_EQUAL(0, tof_service_filter_get_enabled(&enabled));
    TEST_ASSERT_EQUAL(1, enabled);
    
    TEST_ASSERT_EQUAL(0, tof_service_filter_disable());
    TEST_ASSERT_EQUAL(0, tof_service_filter_get_enabled(&enabled));
    TEST_ASSERT_EQUAL(0, enabled);
#else
    debug_print("Filter not enabled - skipping filter tests\n");
#endif
    
    debug_print("TOF service filter tests passed!\n");
    return 0;
}

static int test_tof_service_status(void) {
    debug_print("Testing TOF service status...\n");
    
    char status_buffer[256];
    TEST_ASSERT_EQUAL(0, tof_service_get_status(status_buffer, sizeof(status_buffer)));
    
    // Check that status buffer contains expected information
    debug_print("Status: %s\n", status_buffer);
    TEST_ASSERT(safe_strlen(status_buffer) > 0);
    
    // Test with small buffer
    char small_buffer[10];
    TEST_ASSERT_EQUAL(0, tof_service_get_status(small_buffer, sizeof(small_buffer)));
    debug_print("Small buffer status: %s\n", small_buffer);
    
    debug_print("TOF service status tests passed!\n");
    return 0;
}

static int test_tof_service_measurement(void) {
    debug_print("Testing TOF service measurement...\n");
    
    // Start ranging first
    TEST_ASSERT_EQUAL(0, tof_service_start_ranging());
    
    tof_measurement_t measurement;
    int result = tof_service_get_measurement(&measurement);
    
    debug_print("Measurement result: %d\n", result);
    if (result == 0) {
        debug_print("Distance: %d mm\n", measurement.distance_mm);
        debug_print("Status: %d\n", measurement.status);
        debug_print("Timestamp: %u\n", measurement.timestamp);
    }
    
    // Stop ranging
    TEST_ASSERT_EQUAL(0, tof_service_stop_ranging());
    
    debug_print("TOF service measurement tests passed!\n");
    return 0;
}

// Main test runner
int main(void) {
    debug_print("Starting TOF service test suite...\n");
    debug_print("=====================================\n");
    
    int result = 0;
    
    if (test_tof_service_basic_operations() != 0) result = -1;
    if (test_tof_service_ranging() != 0) result = -1;
    if (test_tof_service_filter() != 0) result = -1;
    if (test_tof_service_status() != 0) result = -1;
    if (test_tof_service_measurement() != 0) result = -1;
    
    // Cleanup
    tof_service_deinit();
    
    debug_print("=====================================\n");
    if (result == 0) {
        debug_print("All TOF service tests PASSED!\n");
    } else {
        debug_print("Some TOF service tests FAILED!\n");
    }
    
    return result;
}
