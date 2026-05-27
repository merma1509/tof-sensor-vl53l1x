/**
 * @file  filter.h
 * @brief Combined Median + Moving Average Filter for VL53L1X
 *
 * Pipeline: Median(5) -> Moving Average(4)
 * Removes outliers then smooths the result
 */

#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

/* Compile-time filter enable/disable
 * Set to 0 to disable filter completely (for baseline testing)
 * Set to 1 to enable filter (default)
 */
#ifndef FILTER_ENABLED
#define FILTER_ENABLED        1   /* 1 = enabled, 0 = disabled */
#endif

/* Filter configuration */
#define FILTER_MEDIAN_SIZE    5   /* Median filter window */
#define FILTER_AVERAGE_SIZE   4   /* Moving average window */
#define FILTER_TOTAL_SIZE     (FILTER_MEDIAN_SIZE + FILTER_AVERAGE_SIZE - 1)

/* Filter state structure */
typedef struct {
    uint16_t median_buffer[FILTER_MEDIAN_SIZE];    /* Raw samples for median */
    uint16_t avg_buffer[FILTER_AVERAGE_SIZE];       /* Median outputs for averaging */
    uint8_t  median_index;                          /* Current position in median buffer */
    uint8_t  avg_index;                             /* Current position in avg buffer */
    uint8_t  sample_count;                          /* Total samples collected */
} filter_state_t;

/**
 * @brief Initialize filter state
 * @param state Pointer to filter state structure
 */
void filter_init(filter_state_t *state);

/**
 * @brief Process new sample through filter pipeline
 * @param state Pointer to filter state
 * @param new_sample Raw distance measurement
 * @return Filtered distance value
 */
uint16_t filter_process(filter_state_t *state, uint16_t new_sample);

/**
 * @brief Check if filter has enough samples for valid output
 * @param state Pointer to filter state
 * @return 1 if ready, 0 if still filling buffers
 */
uint8_t filter_is_ready(const filter_state_t *state);

/**
 * @brief Reset filter to initial state
 * @param state Pointer to filter state
 */
void filter_reset(filter_state_t *state);

#if FILTER_ENABLED
/**
 * @brief Enable/disable filter at runtime
 * @param enable 1 to enable, 0 to disable
 */
void filter_set_enabled(uint8_t enable);

/**
 * @brief Check if filter is currently enabled
 * @return 1 if enabled, 0 if disabled
 */
uint8_t filter_get_enabled(void);

/**
 * @brief Get filter status string
 * @return "ON" or "OFF"
 */
const char* filter_get_status_string(void);
#endif

#endif /* FILTER_H */
