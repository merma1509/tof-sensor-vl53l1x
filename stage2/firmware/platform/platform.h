/**
 * @file platform.h
 * @brief Platform abstraction interface
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdint.h>
#include <stddef.h>
#include "../config/platform_config.h"
#include "../drivers/common/debug_utils.h"

/* Platform return codes */
typedef enum {
    PLATFORM_OK = 0,
    PLATFORM_ERROR = -1,
    PLATFORM_BUSY = -2,
    PLATFORM_TIMEOUT = -3,
    PLATFORM_NOT_SUPPORTED = -4
} platform_status_t;

/* Platform interface structure */
typedef struct {
    /* UART interface */
    platform_status_t (*uart_init)(uint32_t baudrate);
    platform_status_t (*uart_send)(const uint8_t *data, size_t len);
    platform_status_t (*uart_receive)(uint8_t *data, size_t len, uint32_t timeout_ms);
    platform_status_t (*uart_send_string)(const char *str);
    
    /* I2C interface */
    platform_status_t (*i2c_init)(uint32_t frequency);
    platform_status_t (*i2c_write)(uint8_t addr, const uint8_t *data, size_t len);
    platform_status_t (*i2c_read)(uint8_t addr, uint8_t *data, size_t len);
    platform_status_t (*i2c_write_reg)(uint8_t addr, uint16_t reg, const uint8_t *data, size_t len);
    platform_status_t (*i2c_read_reg)(uint8_t addr, uint16_t reg, uint8_t *data, size_t len);
    
    /* GPIO interface */
    platform_status_t (*gpio_init)(uint32_t pin, uint32_t mode);
    platform_status_t (*gpio_set)(uint32_t pin, uint32_t state);
    platform_status_t (*gpio_get)(uint32_t pin, uint32_t *state);
    
    /* Timer/Delay interface */
    void (*delay_ms)(uint32_t ms);
    void (*delay_us)(uint32_t us);
    uint32_t (*get_tick_ms)(void);
    uint32_t (*get_tick_us)(void);
    
    /* System interface */
    platform_status_t (*system_init)(void);
    void (*system_reset)(void);
    uint32_t (*get_system_clock)(void);
} platform_interface_t;

/* Platform initialization */
platform_status_t platform_init(void);
platform_status_t platform_deinit(void);

/* Get platform interface */
const platform_interface_t* platform_get_interface(void);

/* Platform-specific implementations */
#if TARGET_PLATFORM == PLATFORM_GD32
extern const platform_interface_t gd32_platform_interface;
#elif TARGET_PLATFORM == PLATFORM_MIK32
extern const platform_interface_t mik32_platform_interface;
#endif

/* Convenience macros */
#define PLATFORM_UART_INIT(baud)          platform_get_interface()->uart_init(baud)
#define PLATFORM_UART_SEND(data, len)      platform_get_interface()->uart_send(data, len)
#define PLATFORM_UART_RECV(data, len, to)  platform_get_interface()->uart_receive(data, len, to)
#define PLATFORM_UART_SEND_STR(str)       platform_get_interface()->uart_send_string(str)

#define PLATFORM_I2C_INIT(freq)            platform_get_interface()->i2c_init(freq)
#define PLATFORM_I2C_WRITE(addr, data, len) platform_get_interface()->i2c_write(addr, data, len)
#define PLATFORM_I2C_READ(addr, data, len)  platform_get_interface()->i2c_read(addr, data, len)
#define PLATFORM_I2C_WRITE_REG(addr, reg, data, len) platform_get_interface()->i2c_write_reg(addr, reg, data, len)
#define PLATFORM_I2C_READ_REG(addr, reg, data, len) platform_get_interface()->i2c_read_reg(addr, reg, data, len)

#define PLATFORM_GPIO_INIT(pin, mode)      platform_get_interface()->gpio_init(pin, mode)
#define PLATFORM_GPIO_SET(pin, state)      platform_get_interface()->gpio_set(pin, state)
#define PLATFORM_GPIO_GET(pin, state)      platform_get_interface()->gpio_get(pin, state)

#define PLATFORM_DELAY_MS(ms)              platform_get_interface()->delay_ms(ms)
#define PLATFORM_DELAY_US(us)              platform_get_interface()->delay_us(us)
#define PLATFORM_GET_TICK_MS()             platform_get_interface()->get_tick_ms()
#define PLATFORM_GET_TICK_US()             platform_get_interface()->get_tick_us()

/* Debug macros - using common utilities */
#if DEBUG_ENABLED
#define DEBUG_PRINT(...) COMMON_DEBUG_PRINT(__VA_ARGS__)
#define DEBUG_PRINT_HEX(data, len) debug_hex_dump(data, len, 0)
#else
#define DEBUG_PRINT(...)
#define DEBUG_PRINT_HEX(data, len)
#endif

#endif /* PLATFORM_H */
