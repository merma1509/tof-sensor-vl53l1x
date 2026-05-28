/**
 * @file platform_mik32_direct.c
 * @brief MIK32 platform implementation using direct register access
 */

#include "../platform.h"

// MIK32 Register definitions from memory map
#define UART_0_BASE              0x00081400
#define I2C_0_BASE               0x00083800
#define GPIO_0_BASE              0x00084000
#define GPIO_1_BASE              0x00084400
#define PM_BASE                  0x000050000

// UART Register offsets
#define UART_CONTROL_OFFSET     0x00
#define UART_STATUS_OFFSET      0x04
#define UART_DATA_OFFSET         0x08
#define UART_DIVIDER_OFFSET      0x0C

// I2C Register offsets
#define I2C_CR1_OFFSET          0x00
#define I2C_CR2_OFFSET          0x04
#define I2C_OAR1_OFFSET         0x08
#define I2C_TIMINGR_OFFSET      0x10
#define I2C_ISR_OFFSET          0x18
#define I2C_ICR_OFFSET          0x1C
#define I2C_RXDR_OFFSET         0x24
#define I2C_TXDR_OFFSET         0x28

// GPIO Register offsets
#define GPIO_MODE_OFFSET        0x00
#define GPIO_DATA_OFFSET        0x10
#define GPIO_PULL_OFFSET        0x18

// Power Management Register offsets
#define PM_CLK_AHB_SET_OFFSET   0x00
#define PM_CLK_APB_M_SET_OFFSET 0x04
#define PM_CLK_APB_P_SET_OFFSET 0x08

// Register access macros
#define UART_0               (*((volatile uint32_t*)UART_0_BASE))
#define UART_0_CONTROL       (*((volatile uint32_t*)(UART_0_BASE + UART_CONTROL_OFFSET)))
#define UART_0_STATUS        (*((volatile uint32_t*)(UART_0_BASE + UART_STATUS_OFFSET)))
#define UART_0_DATA          (*((volatile uint32_t*)(UART_0_BASE + UART_DATA_OFFSET)))
#define UART_0_DIVIDER       (*((volatile uint32_t*)(UART_0_BASE + UART_DIVIDER_OFFSET)))

#define I2C_0                (*((volatile uint32_t*)I2C_0_BASE))
#define I2C_0_CR1            (*((volatile uint32_t*)(I2C_0_BASE + I2C_CR1_OFFSET)))
#define I2C_0_CR2            (*((volatile uint32_t*)(I2C_0_BASE + I2C_CR2_OFFSET)))
#define I2C_0_OAR1           (*((volatile uint32_t*)(I2C_0_BASE + I2C_OAR1_OFFSET)))
#define I2C_0_TIMINGR        (*((volatile uint32_t*)(I2C_0_BASE + I2C_TIMINGR_OFFSET)))
#define I2C_0_ISR            (*((volatile uint32_t*)(I2C_0_BASE + I2C_ISR_OFFSET)))
#define I2C_0_ICR            (*((volatile uint32_t*)(I2C_0_BASE + I2C_ICR_OFFSET)))
#define I2C_0_RXDR           (*((volatile uint32_t*)(I2C_0_BASE + I2C_RXDR_OFFSET)))
#define I2C_0_TXDR           (*((volatile uint32_t*)(I2C_0_BASE + I2C_TXDR_OFFSET)))

#define GPIO_0               (*((volatile uint32_t*)GPIO_0_BASE))
#define GPIO_0_MODE          (*((volatile uint32_t*)(GPIO_0_BASE + GPIO_MODE_OFFSET)))
#define GPIO_0_DATA          (*((volatile uint32_t*)(GPIO_0_BASE + GPIO_DATA_OFFSET)))
#define GPIO_0_PULL          (*((volatile uint32_t*)(GPIO_0_BASE + GPIO_PULL_OFFSET)))

#define GPIO_1               (*((volatile uint32_t*)GPIO_1_BASE))
#define GPIO_1_MODE          (*((volatile uint32_t*)(GPIO_1_BASE + GPIO_MODE_OFFSET)))
#define GPIO_1_DATA          (*((volatile uint32_t*)(GPIO_1_BASE + GPIO_DATA_OFFSET)))
#define GPIO_1_PULL          (*((volatile uint32_t*)(GPIO_1_BASE + GPIO_PULL_OFFSET)))

#define PM                   (*((volatile uint32_t*)PM_BASE))
#define PM_CLK_AHB_SET       (*((volatile uint32_t*)(PM_BASE + PM_CLK_AHB_SET_OFFSET)))
#define PM_CLK_APB_M_SET     (*((volatile uint32_t*)(PM_BASE + PM_CLK_APB_M_SET_OFFSET)))
#define PM_CLK_APB_P_SET     (*((volatile uint32_t*)(PM_BASE + PM_CLK_APB_P_SET_OFFSET)))

// Bit definitions
#define UART_CR1_ENABLE      (1 << 0)
#define UART_CR1_TX_ENABLE   (1 << 1)
#define UART_CR1_RX_ENABLE   (1 << 2)
#define UART_STATUS_TX_EMPTY (1 << 0)
#define UART_STATUS_RX_READY (1 << 1)

#define I2C_CR1_ENABLE       (1 << 0)
#define I2C_CR2_AUTOEND      (1 << 25)
#define I2C_ISR_TXIS         (1 << 1)
#define I2C_ISR_RXNE         (1 << 2)
#define I2C_ISR_STOPF        (1 << 5)
#define I2C_ISR_NACKF        (1 << 4)

#define GPIO_MODE_INPUT      (0b100 << 0)
#define GPIO_MODE_OUTPUT     (0b000 << 0)
#define GPIO_MODE_SERIAL     (0b01 << 0)

#define PM_CLOCK_UART_0      (1 << 2)
#define PM_CLOCK_I2C_0       (1 << 5)
#define PM_CLOCK_GPIO_0      (1 << 0)
#define PM_CLOCK_GPIO_1      (1 << 1)

// Forward declarations
static void mik32_delay_us(uint32_t us);

/* System clock initialization */
static void mik32_system_clock_init(void) {
    // MIK32 uses 32MHz internal oscillator by default
    // No additional configuration needed for basic operation
}

/* UART implementation */
static platform_status_t mik32_uart_init(uint32_t baudrate) {
    (void)baudrate; // Mark as intentionally unused
    // Enable UART clock
    PM_CLK_APB_P_SET = PM_CLOCK_UART_0;
    
    // Configure GPIO pins P1.10 (TX) and P1.11 (RX) for UART
    // Based on MIK32 pin mapping, these correspond to GPIO_1 pins
    GPIO_1_MODE &= ~(0xF << (10 * 2));  // Clear P1.10 mode
    GPIO_1_MODE |= (GPIO_MODE_SERIAL << (10 * 2));  // Set P1.10 to UART TX
    GPIO_1_MODE &= ~(0xF << (11 * 2));  // Clear P1.11 mode
    GPIO_1_MODE |= (GPIO_MODE_SERIAL << (11 * 2));  // Set P1.11 to UART RX
    
    // Configure UART for 115200 baud at 32MHz
    // Baud = Fclk / (16 * DIVIDER)
    // DIVIDER = 32000000 / (16 * 115200) = 17.36 ≈ 17
    UART_0_DIVIDER = 17;
    
    // Enable UART, TX, and RX
    UART_0_CONTROL = UART_CR1_ENABLE | UART_CR1_TX_ENABLE | UART_CR1_RX_ENABLE;
    
    return PLATFORM_OK;
}

static platform_status_t mik32_uart_send(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        // Wait for TX buffer to be empty
        while (!(UART_0_STATUS & UART_STATUS_TX_EMPTY));
        
        // Send data
        UART_0_DATA = data[i];
    }
    return PLATFORM_OK;
}

static platform_status_t mik32_uart_receive(uint8_t *data, size_t len, uint32_t timeout_ms) {
    for (size_t i = 0; i < len; i++) {
        uint32_t start_time = PLATFORM_GET_TICK_MS();
        
        // Wait for RX data to be ready with precise timeout
        while (!(UART_0_STATUS & UART_STATUS_RX_READY)) {
            if ((PLATFORM_GET_TICK_MS() - start_time) > timeout_ms) {
                return PLATFORM_TIMEOUT;
            }
            mik32_delay_us(1); // Small delay to prevent busy-wait
        }
        
        // Read data
        data[i] = UART_0_DATA;
    }
    return PLATFORM_OK;
}

static platform_status_t mik32_uart_send_string(const char *str) {
    size_t len = 0;
    while (str[len]) len++;
    return mik32_uart_send((const uint8_t*)str, len);
}

/* I2C implementation */
static platform_status_t mik32_i2c_init(uint32_t frequency) {
    (void)frequency; // Mark as intentionally unused
    // Enable I2C clock
    PM_CLK_APB_P_SET = PM_CLOCK_I2C_0;
    
    // Configure GPIO pins P0.9 (SDA) and P0.10 (SCL) for I2C
    GPIO_0_MODE &= ~(0xF << (9 * 2));   // Clear P0.9 mode
    GPIO_0_MODE |= (GPIO_MODE_SERIAL << (9 * 2));   // Set P0.9 to I2C SDA
    GPIO_0_MODE &= ~(0xF << (10 * 2));  // Clear P0.10 mode
    GPIO_0_MODE |= (GPIO_MODE_SERIAL << (10 * 2));  // Set P0.10 to I2C SCL
    
    // Configure pull-up resistors for I2C
    GPIO_0_PULL |= (1 << 9) | (1 << 10);
    
    // Configure I2C timing for 100kHz
    // These values are approximate and may need tuning
    I2C_0_TIMINGR = 0x10420F13; // Standard 100kHz timing
    
    // Enable I2C
    I2C_0_CR1 = I2C_CR1_ENABLE;
    
    return PLATFORM_OK;
}

static platform_status_t mik32_i2c_write(uint8_t addr, const uint8_t *data, size_t len) {
    // Configure transfer
    I2C_0_CR2 = (addr << 1) | (len << 16) | I2C_CR2_AUTOEND;
    
    // Start transfer
    I2C_0_CR2 |= (1 << 13); // START bit
    
    // Send data
    for (size_t i = 0; i < len; i++) {
        // Wait for TXIS flag
        while (!(I2C_0_ISR & I2C_ISR_TXIS));
        
        // Send data
        I2C_0_TXDR = data[i];
    }
    
    // Wait for transfer complete
    while (!(I2C_0_ISR & I2C_ISR_STOPF));
    
    // Clear STOP flag
    I2C_0_ICR = I2C_ISR_STOPF;
    
    return PLATFORM_OK;
}

static platform_status_t mik32_i2c_read(uint8_t addr, uint8_t *data, size_t len) {
    // Configure transfer
    I2C_0_CR2 = ((addr << 1) | 1) | (len << 16) | I2C_CR2_AUTOEND;
    
    // Start transfer
    I2C_0_CR2 |= (1 << 13); // START bit
    
    // Receive data
    for (size_t i = 0; i < len; i++) {
        // Wait for RXNE flag
        while (!(I2C_0_ISR & I2C_ISR_RXNE));
        
        // Read data
        data[i] = I2C_0_RXDR;
    }
    
    // Wait for transfer complete
    while (!(I2C_0_ISR & I2C_ISR_STOPF));
    
    // Clear STOP flag
    I2C_0_ICR = I2C_ISR_STOPF;
    
    return PLATFORM_OK;
}

static platform_status_t mik32_i2c_write_reg(uint8_t addr, uint16_t reg, const uint8_t *data, size_t len) {
    // For VL53L1, we need to write register first, then data
    uint8_t buffer[len + 2];
    buffer[0] = (reg >> 8) & 0xFF;  // High byte
    buffer[1] = reg & 0xFF;         // Low byte
    for (size_t i = 0; i < len; i++) {
        buffer[i + 2] = data[i];
    }
    
    return mik32_i2c_write(addr, buffer, len + 2);
}

static platform_status_t mik32_i2c_read_reg(uint8_t addr, uint16_t reg, uint8_t *data, size_t len) {
    // Write register address first
    uint8_t reg_buffer[2];
    reg_buffer[0] = (reg >> 8) & 0xFF;
    reg_buffer[1] = reg & 0xFF;
    
    if (mik32_i2c_write(addr, reg_buffer, 2) != PLATFORM_OK) {
        return PLATFORM_ERROR;
    }
    
    // Small delay between write and read
    mik32_delay_us(10);
    
    // Then read data
    return mik32_i2c_read(addr, data, len);
}

/* GPIO implementation */
static platform_status_t mik32_gpio_init(uint32_t pin, uint32_t mode) {
    volatile uint32_t* gpio_mode_reg;
    uint32_t gpio_mode_value;
    
    if (pin < 16) {
        gpio_mode_reg = &GPIO_0_MODE;
    } else if (pin < 32) {
        gpio_mode_reg = &GPIO_1_MODE;
        pin -= 16;
    } else {
        return PLATFORM_ERROR;
    }
    
    // Convert mode to MIK32 GPIO mode
    switch (mode) {
        case 0: // Input
            gpio_mode_value = GPIO_MODE_INPUT;
            break;
        case 1: // Output
            gpio_mode_value = GPIO_MODE_OUTPUT;
            break;
        default:
            gpio_mode_value = GPIO_MODE_INPUT;
            break;
    }
    
    // Configure pin mode
    uint32_t mode_reg_value = *gpio_mode_reg;
    mode_reg_value &= ~(0xF << (pin * 2));
    mode_reg_value |= (gpio_mode_value << (pin * 2));
    *gpio_mode_reg = mode_reg_value;
    
    return PLATFORM_OK;
}

static platform_status_t mik32_gpio_set(uint32_t pin, uint32_t state) {
    volatile uint32_t* gpio_data_reg;
    uint32_t actual_pin;
    
    if (pin < 16) {
        gpio_data_reg = &GPIO_0_DATA;
    } else if (pin < 32) {
        gpio_data_reg = &GPIO_1_DATA;
        pin -= 16;
    } else {
        return PLATFORM_ERROR;
    }
    
    actual_pin = pin;
    
    if (state) {
        *gpio_data_reg |= (1 << actual_pin);
    } else {
        *gpio_data_reg &= ~(1 << actual_pin);
    }
    
    return PLATFORM_OK;
}

static platform_status_t mik32_gpio_get(uint32_t pin, uint32_t *state) {
    volatile uint32_t* gpio_data_reg;
    uint32_t actual_pin;
    
    if (pin < 16) {
        gpio_data_reg = &GPIO_0_DATA;
    } else if (pin < 32) {
        gpio_data_reg = &GPIO_1_DATA;
        pin -= 16;
    } else {
        return PLATFORM_ERROR;
    }
    
    actual_pin = pin;
    *state = (*gpio_data_reg >> actual_pin) & 1;
    
    return PLATFORM_OK;
}

/* Timer/Delay implementation */
static void mik32_delay_ms(uint32_t ms) {
    // Simple busy-wait based on 32MHz clock
    volatile uint32_t count = ms * 32000;
    while (count--) {
        __asm("nop");
    }
}

static void mik32_delay_us(uint32_t us) {
    volatile uint32_t count = us * 32;
    while (count--) {
        __asm("nop");
    }
}

static uint32_t mik32_get_tick_ms(void) {
    static uint32_t tick_count = 0;
    return tick_count++;
}

static uint32_t mik32_get_tick_us(void) {
    return mik32_get_tick_ms() * 1000;
}

/* System implementation */
static platform_status_t mik32_system_init(void) {
    mik32_system_clock_init();
    
    // Enable GPIO clocks
    PM_CLK_AHB_SET = PM_CLOCK_GPIO_0 | PM_CLOCK_GPIO_1;
    
    return PLATFORM_OK;
}

static void mik32_system_reset(void) {
    // TODO: Implement MIK32 system reset
    while(1);
}

static uint32_t mik32_get_system_clock(void) {
    // MIK32 runs at 32MHz internal oscillator
    return 32000000;
}

/* Platform interface definition */
const platform_interface_t mik32_platform_interface = {
    .uart_init = mik32_uart_init,
    .uart_send = mik32_uart_send,
    .uart_receive = mik32_uart_receive,
    .uart_send_string = mik32_uart_send_string,
    
    .i2c_init = mik32_i2c_init,
    .i2c_write = mik32_i2c_write,
    .i2c_read = mik32_i2c_read,
    .i2c_write_reg = mik32_i2c_write_reg,
    .i2c_read_reg = mik32_i2c_read_reg,
    
    .gpio_init = mik32_gpio_init,
    .gpio_set = mik32_gpio_set,
    .gpio_get = mik32_gpio_get,
    
    .delay_ms = mik32_delay_ms,
    .delay_us = mik32_delay_us,
    .get_tick_ms = mik32_get_tick_ms,
    .get_tick_us = mik32_get_tick_us,
    
    .system_init = mik32_system_init,
    .system_reset = mik32_system_reset,
    .get_system_clock = mik32_get_system_clock
};
