/**
 * @file vl53l1_driver.h
 * @brief VL53L1 sensor driver interface
 */

#ifndef VL53L1_DRIVER_H
#define VL53L1_DRIVER_H

#include <stdint.h>

/* VL53L1 sensor definitions */
#define VL53L1_DEVICE_ID          0x0104
#define VL53L1_DEVICE_ID_VALUE    0xEACC

/* VL53L1 result structure */
typedef struct {
    uint16_t distance_mm;
    uint16_t ambient_count;
    uint8_t signal_rate;
    uint8_t status;
} vl53l1_result_t;

/* VL53L1 driver functions */
int vl53l1_init(void);
int vl53l1_deinit(void);
int vl53l1_start_measurement(void);
int vl53l1_stop_measurement(void);
int vl53l1_get_measurement(vl53l1_result_t *result);
int vl53l1_check_device_id(void);

#endif // VL53L1_DRIVER_H
