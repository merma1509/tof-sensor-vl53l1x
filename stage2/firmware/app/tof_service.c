/**
 * @file tof_service.c
 * @brief Time-of-Flight sensor service implementation
 */

#include "tof_service.h"
#include "../drivers/vl53l1/VL53L1X_api.h"
#include "../platform/platform.h"
#include "../drivers/common/string_utils.h"

// Using common utilities for library functions

/* TOF service state */
static tof_state_t current_state = TOF_STATE_IDLE;
static tof_mode_t current_mode = TOF_MODE_SHORT;
static uint8_t ranging_enabled = 0;

/* Filter state */
#if FILTER_ENABLED
static uint8_t filter_enabled = 1;
static uint16_t filter_buffer[FILTER_MEDIAN_SIZE];
static uint8_t filter_index = 0;
static uint8_t filter_count = 0;
#endif

/* Internal functions */
static uint16_t tof_apply_filter(uint16_t distance);
static void tof_sort_array(uint16_t *array, size_t size);

/* TOF service implementation */
int tof_service_init(void) {
    current_state = TOF_STATE_INIT;
    
    DEBUG_PRINT("Initializing TOF service...\r\n");
    
    // Initialize VL53L1X sensor
    if (VL53L1X_SensorInit(0x52) != VL53L1X_ERROR_NONE) {
        DEBUG_PRINT("VL53L1X sensor init failed\r\n");
        current_state = TOF_STATE_ERROR;
        return -1;
    }
    
    // Start measurement
    if (VL53L1X_StartRanging(0x52) != VL53L1X_ERROR_NONE) {
        DEBUG_PRINT("VL53L1X start ranging failed\r\n");
        current_state = TOF_STATE_ERROR;
        return -2;
    }
    
    current_state = TOF_STATE_READY;
    DEBUG_PRINT("TOF service initialized successfully\r\n");
    return 0;
}

int tof_service_deinit(void) {
    tof_service_stop_ranging();
    VL53L1X_StopRanging(0x52);
    current_state = TOF_STATE_IDLE;
    return 0;
}

int tof_service_start_ranging(void) {
    if (current_state != TOF_STATE_READY) {
        return -1;
    }
    
    // Start continuous ranging
    if (VL53L1X_StartRanging(0x52) != VL53L1X_ERROR_NONE) {
        DEBUG_PRINT("Failed to start VL53L1X ranging\r\n");
        return -2;
    }
    ranging_enabled = 1;
    current_state = TOF_STATE_RANGING;
    
    DEBUG_PRINT("TOF ranging started\r\n");
    return 0;
}

int tof_service_stop_ranging(void) {
    if (current_state != TOF_STATE_RANGING) {
        return -1;
    }
    
    // Stop continuous ranging
    if (VL53L1X_StopRanging(0x52) != VL53L1X_ERROR_NONE) {
        DEBUG_PRINT("Failed to stop VL53L1X ranging\r\n");
        return -2;
    }
    ranging_enabled = 0;
    current_state = TOF_STATE_READY;
    
    DEBUG_PRINT("TOF ranging stopped\r\n");
    return 0;
}

int tof_service_set_mode(tof_mode_t mode) {
    if (mode > TOF_MODE_LONG) {
        return -1;
    }
    
    current_mode = mode;
    
    // Configure VL53L1 timing budget based on mode
    uint32_t timing_budget_ms;
    switch (mode) {
        case TOF_MODE_SHORT:
            timing_budget_ms = 15;
            break;
        case TOF_MODE_MEDIUM:
            timing_budget_ms = 50;
            break;
        case TOF_MODE_LONG:
            timing_budget_ms = 100;
            break;
    }
    
    // Set timing budget using VL53L1X API
    if (VL53L1X_SetTimingBudgetInMs(0x52, timing_budget_ms) != VL53L1X_ERROR_NONE) {
        DEBUG_PRINT("Failed to set VL53L1X timing budget\r\n");
        return -2;
    }
    
    DEBUG_PRINT("TOF mode set to %d (timing: %dms)\r\n", mode, timing_budget_ms);
    return 0;
}

tof_mode_t tof_service_get_mode(void) {
    return current_mode;
}

int tof_service_get_distance(uint16_t *distance_mm) {
    if (current_state != TOF_STATE_RANGING || !ranging_enabled) {
        return -1;
    }
    
    // Get VL53L1 distance measurement
    uint16_t raw_distance = 0;
    uint8_t data_ready = 0;
    
    // Check if data is ready
    if (VL53L1X_CheckForDataReady(0x52, &data_ready) != VL53L1X_ERROR_NONE) {
        DEBUG_PRINT("Failed to check VL53L1X data ready\r\n");
        return -2;
    }
    
    if (data_ready) {
        // Get the distance measurement
        if (VL53L1X_GetDistance(0x52, &raw_distance) != VL53L1X_ERROR_NONE) {
            DEBUG_PRINT("Failed to get VL53L1X distance\r\n");
            return -3;
        }
        
        // Clear interrupt for next measurement
        VL53L1X_ClearInterrupt(0x52);
    } else {
        // No data ready, return last known distance or error
        return -4;
    }
    
    // Apply filter if enabled
    uint16_t filtered_distance = tof_apply_filter(raw_distance);
    *distance_mm = filtered_distance;
    
    return 0;
}

int tof_service_get_measurement(tof_measurement_t *measurement) {
    if (current_state != TOF_STATE_RANGING || !ranging_enabled) {
        return -1;
    }
    
    if (tof_service_get_distance(&measurement->distance_mm) != 0) {
        return -2;
    }
    
    measurement->timestamp = PLATFORM_GET_TICK_MS();
    measurement->status = 0; // OK
    measurement->range_status = 0; // Valid
    measurement->signal_rate = 50; // Simulated
    measurement->ambient_rate = 10; // Simulated
    
    return 0;
}

tof_state_t tof_service_get_state(void) {
    return current_state;
}

int tof_service_get_status(char *status_buffer, size_t buffer_size) {
    const char *state_str;
    switch (current_state) {
        case TOF_STATE_IDLE:
            state_str = "Idle";
            break;
        case TOF_STATE_INIT:
            state_str = "Initializing";
            break;
        case TOF_STATE_CALIBRATING:
            state_str = "Calibrating";
            break;
        case TOF_STATE_READY:
            state_str = "Ready";
            break;
        case TOF_STATE_RANGING:
            state_str = "Ranging";
            break;
        case TOF_STATE_ERROR:
            state_str = "Error";
            break;
        default:
            state_str = "Unknown";
            break;
    }
    
    const char *mode_str;
    switch (current_mode) {
        case TOF_MODE_SHORT:
            mode_str = "Short";
            break;
        case TOF_MODE_MEDIUM:
            mode_str = "Medium";
            break;
        case TOF_MODE_LONG:
            mode_str = "Long";
            break;
        default:
            mode_str = "Unknown";
            break;
    }
    
    // Using safe string functions from common utilities
    safe_strcpy(status_buffer, "State: ", buffer_size);
    safe_strcat(status_buffer, state_str, buffer_size);
    safe_strcat(status_buffer, ", Mode: ", buffer_size);
    safe_strcat(status_buffer, mode_str, buffer_size);
    
#if FILTER_ENABLED
    safe_strcat(status_buffer, ", Filter: ", buffer_size);
    safe_strcat(status_buffer, filter_enabled ? "ON" : "OFF", buffer_size);
#endif
    
    return 0;
}

#if FILTER_ENABLED
int tof_service_filter_enable(void) {
    filter_enabled = 1;
    filter_count = 0;
    filter_index = 0;
    DEBUG_PRINT("TOF filter enabled\r\n");
    return 0;
}

int tof_service_filter_disable(void) {
    filter_enabled = 0;
    DEBUG_PRINT("TOF filter disabled\r\n");
    return 0;
}

int tof_service_filter_get_enabled(uint8_t *enabled) {
    *enabled = filter_enabled;
    return 0;
}
#endif


static uint16_t tof_apply_filter(uint16_t distance) {
#if FILTER_ENABLED
    if (!filter_enabled) {
        return distance;
    }
    
    // Add new sample to buffer
    filter_buffer[filter_index] = distance;
    filter_index = (filter_index + 1) % FILTER_MEDIAN_SIZE;
    
    if (filter_count < FILTER_MEDIAN_SIZE) {
        filter_count++;
    }
    
    // Apply median filter
    uint16_t sorted_buffer[FILTER_MEDIAN_SIZE];
    for (uint8_t i = 0; i < filter_count; i++) {
        sorted_buffer[i] = filter_buffer[i];
    }
    
    tof_sort_array(sorted_buffer, filter_count);
    uint16_t median = sorted_buffer[filter_count / 2];
    
    // Apply moving average filter
    static uint16_t average_buffer[FILTER_AVERAGE_SIZE];
    static uint8_t average_index = 0;
    static uint8_t average_count = 0;
    static uint32_t average_sum = 0;
    
    // Update moving average
    if (average_count >= FILTER_AVERAGE_SIZE) {
        average_sum -= average_buffer[average_index];
    } else {
        average_count++;
    }
    
    average_buffer[average_index] = median;
    average_sum += median;
    average_index = (average_index + 1) % FILTER_AVERAGE_SIZE;
    
    return (uint16_t)(average_sum / average_count);
#else
    return distance;
#endif
}

static void tof_sort_array(uint16_t *array, size_t size) {
    // Simple bubble sort
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                uint16_t temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

