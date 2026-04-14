/**
 * @file  sdk_integration.c
 * @brief SDK integration implementation for VL53L1X and GD32E230
 */

#include "../include/sdk_integration.h"

// Global SDK integration instance
static sdk_integration_t g_sdk = {0};

// GD32E230 SDK wrapper functions
void sdk_gpio_init(void) {
    // Enable GPIO clocks
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    
    // Configure I2C pins (PB6/PB7)
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_6 | GPIO_PIN_7);
    
    // Configure UART pins (PA9/PA10)
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9 | GPIO_PIN_10);
}

void sdk_system_clock_config(void) {
    // Use existing SystemInit from current code
    // This will be replaced with GD32 SDK version later
    SystemInit();
}

uint8_t sdk_system_init(void) {
    sdk_system_clock_config();
    sdk_gpio_init();
    g_sdk.gd32_initialized = 1;
    return 1;
}

uint8_t sdk_i2c_init(void) {
    // Enable I2C clock
    rcu_periph_clock_enable(RCU_I2C0);
    
    // Configure I2C using GD32 SDK functions
    i2c_deinit(I2C0);
    i2c_clock_config(I2C0, 100000, I2C_DTCY_2);
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x00);
    i2c_enable(I2C0);
    
    return 1;
}

uint8_t sdk_uart_init(void) {
    // Enable UART clock
    rcu_periph_clock_enable(RCU_USART0);
    
    // Configure UART using GD32 SDK functions
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_transmit_config(USART0, USART_TXPIN_ENABLE);
    usart_receive_config(USART0, USART_RXPIN_ENABLE);
    usart_enable(USART0);
    
    return 1;
}

uint8_t sdk_vl53l1x_init(void) {
    // Initialize VL53L1X using API
    VL53L1X_ERROR status = VL53L1X_SensorInit(VL53L1X_I2C_ADDRESS);
    
    if (status == VL53L1X_ERROR_NONE) {
        g_sdk.vl53l1x_initialized = 1;
        g_sdk.current_mode = 2; // Default long range
        g_sdk.timing_budget = 50; // Default 50ms
        return 1;
    }
    
    return 0;
}

uint8_t sdk_vl53l1x_get_distance(uint16_t *distance) {
    if (!g_sdk.vl53l1x_initialized || !distance) {
        return 0;
    }
    
    VL53L1X_ERROR status = VL53L1X_GetDistance(VL53L1X_I2C_ADDRESS, distance);
    
    if (status == VL53L1X_ERROR_NONE) {
        g_sdk.current_distance = *distance;
        return 1;
    }
    
    return 0;
}

uint8_t sdk_vl53l1x_set_mode(uint8_t mode) {
    if (!g_sdk.vl53l1x_initialized) {
        return 0;
    }
    
    VL53L1X_ERROR status = VL53L1X_SetDistanceMode(VL53L1X_I2C_ADDRESS, mode);
    
    if (status == VL53L1X_ERROR_NONE) {
        g_sdk.current_mode = mode;
        return 1;
    }
    
    return 0;
}

uint8_t sdk_vl53l1x_set_timing(uint32_t timing) {
    if (!g_sdk.vl53l1x_initialized) {
        return 0;
    }
    
    VL53L1X_ERROR status = VL53L1X_SetTimingBudgetInMs(VL53L1X_I2C_ADDRESS, timing);
    
    if (status == VL53L1X_ERROR_NONE) {
        g_sdk.timing_budget = timing;
        return 1;
    }
    
    return 0;
}

uint8_t sdk_vl53l1x_calibrate(void) {
    if (!g_sdk.vl53l1x_initialized) {
        return 0;
    }
    
    // Use VL53L1X calibration API
    VL53L1X_ERROR status = VL53L1X_StartTemperatureUpdate(VL53L1X_I2C_ADDRESS);
    
    return (status == VL53L1X_ERROR_NONE) ? 1 : 0;
}

// Initialize all SDK components
uint8_t sdk_full_init(void) {
    if (!sdk_system_init()) return 0;
    if (!sdk_i2c_init()) return 0;
    if (!sdk_uart_init()) return 0;
    if (!sdk_vl53l1x_init()) return 0;
    
    return 1;
}

// Get SDK status
sdk_integration_t* sdk_get_status(void) {
    return &g_sdk;
}
