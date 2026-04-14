/**
 * @file  filter.c
 * @brief Combined Median + Moving Average Filter Implementation
 */

#include "../include/filter.h"
#include <stddef.h>  /* For NULL */

/* Simple insertion sort for median calculation */
static void sort_uint16(uint16_t *arr, uint8_t size) {
    uint8_t i, j;
    uint16_t key;
    for (i = 1; i < size; i++) {
        key = arr[i];
        j = i - 1;
        while (j < size && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/* Calculate median of buffer */
static uint16_t calculate_median(uint16_t *buffer, uint8_t size) {
    uint16_t sorted[5];  /* Max size is 5 */
    uint8_t i;
    
    /* Copy to temporary buffer */
    for (i = 0; i < size; i++) {
        sorted[i] = buffer[i];
    }
    
    /* Sort the copy */
    sort_uint16(sorted, size);
    
    /* Return middle element */
    return sorted[size / 2];
}

/* Calculate average of buffer */
static uint16_t calculate_average(const uint16_t *buffer, uint8_t count) {
    uint32_t sum = 0;
    uint8_t i;
    
    for (i = 0; i < count; i++) {
        sum += buffer[i];
    }
    
    return (uint16_t)(sum / count);
}

void filter_init(filter_state_t *state) {
    uint8_t i;
    
    if (state == NULL) {
        return;
    }
    
    /* Clear all buffers */
    for (i = 0; i < FILTER_MEDIAN_SIZE; i++) {
        state->median_buffer[i] = 0;
    }
    for (i = 0; i < FILTER_AVERAGE_SIZE; i++) {
        state->avg_buffer[i] = 0;
    }
    
    state->median_index = 0;
    state->avg_index = 0;
    state->sample_count = 0;
}

uint16_t filter_process(filter_state_t *state, uint16_t new_sample) {
    uint16_t median_output;
    
    if (state == NULL) {
        return new_sample;
    }
    
    /* Add new sample to median buffer */
    state->median_buffer[state->median_index] = new_sample;
    state->median_index = (state->median_index + 1) % FILTER_MEDIAN_SIZE; // 5 samples for median
    state->sample_count++;
    
    /* Need at least 1 sample to proceed */
    if (state->sample_count == 0) {
        return new_sample;
    }
    
    /* Calculate median (removes spike outliers) */
    median_output = calculate_median(state->median_buffer, 
                                     (state->sample_count >= FILTER_MEDIAN_SIZE) ? 
                                      FILTER_MEDIAN_SIZE : state->sample_count);
    
    /* Add median output to averaging buffer */
    state->avg_buffer[state->avg_index] = median_output;
    state->avg_index = (state->avg_index + 1) % FILTER_AVERAGE_SIZE; // 4 samples for average
    
    /* Calculate final filtered output (average of medians) */
    return calculate_average(state->avg_buffer, FILTER_AVERAGE_SIZE);
}

uint8_t filter_is_ready(const filter_state_t *state) {
    if (state == NULL) {
        return 0;
    }
    
    /* Need at least FILTER_TOTAL_SIZE samples for full filter */
    return (state->sample_count >= FILTER_TOTAL_SIZE) ? 1 : 0;
}

void filter_reset(filter_state_t *state) {
    filter_init(state);
}

#if FILTER_ENABLED
/* Runtime filter enable/disable */
static uint8_t filter_enabled = 1;  /* Default: enabled */

void filter_set_enabled(uint8_t enable) {
    filter_enabled = enable;
}

uint8_t filter_get_enabled(void) {
    return filter_enabled;
}

const char* filter_get_status_string(void) {
    return filter_enabled ? "ON" : "OFF";
}
#endif
