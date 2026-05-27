/**
 * @file tof_service.h
 * @brief Time-of-Flight sensor service abstraction
 */

#ifndef TOF_SERVICE_H
#define TOF_SERVICE_H

#include <stdint.h>
#include <stddef.h>
#include "../config/platform_config.h"

/* TOF sensor states */
typedef enum {
    TOF_STATE_IDLE = 0,
    TOF_STATE_INIT,
    TOF_STATE_CALIBRATING,
    TOF_STATE_READY,
    TOF_STATE_RANGING,
    TOF_STATE_ERROR
} tof_state_t;

/* TOF measurement modes */
typedef enum {
    TOF_MODE_SHORT = 0,    // 15ms timing budget
    TOF_MODE_MEDIUM = 1,   // 50ms timing budget
    TOF_MODE_LONG = 2      // 100ms timing budget
} tof_mode_t;

/* TOF measurement result */
typedef struct {
    uint16_t distance_mm;      // Distance in millimeters
    uint16_t status;           // Measurement status
    uint32_t timestamp;        // Measurement timestamp
    uint8_t range_status;       // Range status from sensor
    uint8_t signal_rate;       // Signal rate
    uint8_t ambient_rate;      // Ambient rate
} tof_measurement_t;

/* TOF service interface */
typedef struct {
    /* Initialization and control */
    int (*init)(void);
    int (*deinit)(void);
    int (*calibrate)(void);
    int (*start_ranging)(void);
    int (*stop_ranging)(void);
    
    /* Configuration */
    int (*set_mode)(tof_mode_t mode);
    int (*get_mode)(tof_mode_t *mode);
    
    /* Measurement */
    int (*get_distance)(uint16_t *distance_mm);
    int (*get_measurement)(tof_measurement_t *measurement);
    
    /* Status and diagnostics */
    tof_state_t (*get_state)(void);
    int (*is_ready)(void);
    int (*get_status)(char *status_buffer, size_t buffer_size);
} tof_service_interface_t;

/* High-level TOF service functions */
int tof_service_init(void);
int tof_service_deinit(void);
int tof_service_start_ranging(void);
int tof_service_stop_ranging(void);
int tof_service_set_mode(tof_mode_t mode);
int tof_service_get_distance(uint16_t *distance_mm);
int tof_service_get_measurement(tof_measurement_t *measurement);
tof_state_t tof_service_get_state(void);
int tof_service_get_status(char *status_buffer, size_t buffer_size);

/* Filter functions */
#if FILTER_ENABLED
int tof_service_filter_enable(void);
int tof_service_filter_disable(void);
int tof_service_filter_get_enabled(uint8_t *enabled);
#endif

#endif /* TOF_SERVICE_H */
