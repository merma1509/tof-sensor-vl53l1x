/**
 * @file platform_gd32.c
 * @brief GD32 platform implementation
 */

#include "../platform.h"
#include "../../drivers/common/string_utils.h"

/* GD32 Register definitions */
#define RCU_CTL         (*((volatile uint32_t*)0x40021000))
#define RCU_AHBEN       (*((volatile uint32_t*)0x40021014))
#define RCU_APB2EN      (*((volatile uint32_t*)0x4002101C))

#define GPIOA_CTL0      (*((volatile uint32_t*)0x48000000))
#define GPIOA_CTL1      (*((volatile uint32_t*)0x48000004))

#define USART0_CTL0     (*((volatile uint32_t*)0x40011000))
#define USART0_CTL1     (*((volatile uint32_t*)0x40011004))
#define USART0_CTL2     (*((volatile uint32_t*)0x40011008))
#define USART0_STAT     (*((volatile uint32_t*)0x4001100C))
#define USART0_DATA     (*((volatile uint32_t*)0x40011010))
#define USART0_BAUD     (*((volatile uint32_t*)0x40011014))

#define I2C0_CTL0       (*((volatile uint32_t*)0x40005400))
#define I2C0_CTL1       (*((volatile uint32_t*)0x40005404))
#define I2C0_CTL2       (*((volatile uint32_t*)0x40005408))
#define I2C0_STAT       (*((volatile uint32_t*)0x4000540C))
#define I2C0_DATA       (*((volatile uint32_t*)0x40005410))
#define I2C0_STAT1      (*((volatile uint32_t*)0x40005414))
#define I2C0_CKCFG      (*((volatile uint32_t*)0x40005418))
#define I2C0_RT         (*((volatile uint32_t*)0x4000541C))
#define I2C0_SAMTIME    (*((volatile uint32_t*)0x40005420))

/* Bit definitions */
#define USART_CTL0_UEN      (1 << 0)
#define USART_CTL0_TEN      (1 << 3)
#define USART_CTL0_REN     (1 << 2)
#define USART_STAT_TC      (1 << 6)
#define USART_STAT_TBE     (1 << 7)
#define USART_STAT_RBNE    (1 << 5)

#define I2C_CTL0_I2CEN     (1 << 0)
#define I2C_CTL0_START     (1 << 8)
#define I2C_CTL0_STOP      (1 << 9)
#define I2C_CTL0_ACKEN     (1 << 10)
#define I2C_STAT1_ADDSEND  (1 << 1)
#define I2C_STAT1_BTC      (1 << 2)
#define I2C_STAT1_RXNE     (1 << 6)
#define I2C_STAT1_TXE      (1 << 7)

/* System clock initialization */
static void gd32_system_clock_init(void) {
    // Enable IRC48M internal RC oscillator
    RCU_CTL |= (1 << 2); // IRC48MEN
    while (!(RCU_CTL & (1 << 3))); // IRC48MSTB
    
    // Configure PLL for 48MHz
    RCU_CTL &= ~(0x1F << 13); // Clear PLLMF
    RCU_CTL |= (9 << 13);     // PLLMF = 9 (48MHz)
    RCU_CTL |= (1 << 13);     // PLLMFEN
    
    // Enable PLL
    RCU_CTL |= (1 << 24); // PLLEN
    while (!(RCU_CTL & (1 << 25))); // PLLSTB
    
    // Select PLL as system clock
    RCU_CTL &= ~(0x3 << 0); // Clear SCS
    RCU_CTL |= (0x2 << 0);  // SCS = PLL
    while ((RCU_CTL & 0x3) != 0x2);
}

/* UART implementation */
static platform_status_t gd32_uart_init(uint32_t baudrate) {
    // Enable GPIOA clock
    RCU_AHBEN |= (1 << 17); // GPIOA enable
    
    // Enable USART0 clock
    RCU_APB2EN |= (1 << 14); // USART0 enable
    
    // Configure PA9 (TX) and PA10 (RX) as alternate function
    GPIOA_CTL0 &= ~(0xF << 4); // Clear PA9
    GPIOA_CTL0 |= (0x1 << 4);  // AF1 for PA9
    GPIOA_CTL0 &= ~(0xF << 8); // Clear PA10
    GPIOA_CTL0 |= (0x1 << 8);  // AF1 for PA10
    
    // Reset USART0
    USART0_CTL0 = 0;
    USART0_CTL1 = 0;
    USART0_CTL2 = 0;
    
    // Set baud rate: 115200 at 48MHz
    // Baud = Fck / (16 * USARTDIV)
    // USARTDIV = 48000000 / (16 * 115200) = 26.0417
    // DIV_MANTISSA = 26, DIV_FRAC = 0.0417 * 16 = 0.667 ≈ 1
    USART0_BAUD = (26 << 4) | 1;
    
    // Configure USART0: 8N1, TX enable, RX enable, USART enable
    USART0_CTL0 = USART_CTL0_UEN | USART_CTL0_TEN | USART_CTL0_REN;
    
    return PLATFORM_OK;
}

static platform_status_t gd32_uart_send(const uint8_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        // Wait for transmit buffer empty
        while (!(USART0_STAT & USART_STAT_TBE));
        USART0_DATA = data[i];
    }
    // Wait for transmission complete
    while (!(USART0_STAT & USART_STAT_TC));
    return PLATFORM_OK;
}

static platform_status_t gd32_uart_receive(uint8_t *data, size_t len, uint32_t timeout_ms) {
    for (size_t i = 0; i < len; i++) {
        uint32_t start_time = PLATFORM_GET_TICK_MS();
        while (!(USART0_STAT & USART_STAT_RBNE)) {
            if ((PLATFORM_GET_TICK_MS() - start_time) > timeout_ms) {
                return PLATFORM_TIMEOUT;
            }
        }
        data[i] = (uint8_t)USART0_DATA;
    }
    return PLATFORM_OK;
}

static platform_status_t gd32_uart_send_string(const char *str) {
    return gd32_uart_send((const uint8_t*)str, safe_strlen(str));
}

/* I2C implementation */
static platform_status_t gd32_i2c_init(uint32_t frequency) {
    // Enable GPIOA clock
    RCU_AHBEN |= (1 << 17); // GPIOA enable
    
    // Enable I2C0 clock
    RCU_APB2EN |= (1 << 16); // I2C0 enable
    
    // Configure PA6 (SCL) and PA7 (SDA) as open-drain output
    GPIOA_CTL0 &= ~(0xF << 24); // Clear PA6
    GPIOA_CTL0 |= (0xD << 24);  // Open-drain output, 50MHz
    GPIOA_CTL0 &= ~(0xF << 28); // Clear PA7
    GPIOA_CTL0 |= (0xD << 28);  // Open-drain output, 50MHz
    
    // Reset I2C0
    I2C0_CTL0 = 0;
    I2C0_CTL1 = 0;
    I2C0_CTL2 = 0;
    
    // Configure I2C clock for 100kHz at 48MHz APB1
    // I2CCLK = 48MHz, CCR = I2CCLK / (2 * FREQ) = 48000000 / (2 * 100000) = 240
    I2C0_CTL1 = 240;
    I2C0_RT = 40; // Rise time
    I2C0_CTL0 = I2C_CTL0_I2CEN;
    
    return PLATFORM_OK;
}

static platform_status_t gd32_i2c_wait_for_flag(uint32_t flag, uint32_t timeout_ms) {
    uint32_t start_time = PLATFORM_GET_TICK_MS();
    while (!(I2C0_STAT1 & flag)) {
        if ((PLATFORM_GET_TICK_MS() - start_time) > timeout_ms) {
            return PLATFORM_TIMEOUT;
        }
    }
    return PLATFORM_OK;
}

static platform_status_t gd32_i2c_write(uint8_t addr, const uint8_t *data, size_t len) {
    // Generate START condition
    I2C0_CTL0 |= I2C_CTL0_START;
    if (gd32_i2c_wait_for_flag(I2C_STAT1_ADDSEND, 100) != PLATFORM_OK) {
        return PLATFORM_ERROR;
    }
    
    // Send slave address with write bit
    I2C0_DATA = (addr << 1);
    if (gd32_i2c_wait_for_flag(I2C_STAT1_ADDSEND, 100) != PLATFORM_OK) {
        return PLATFORM_ERROR;
    }
    
    // Send data
    for (size_t i = 0; i < len; i++) {
        I2C0_DATA = data[i];
        if (gd32_i2c_wait_for_flag(I2C_STAT1_TXE, 100) != PLATFORM_OK) {
            return PLATFORM_ERROR;
        }
    }
    
    // Generate STOP condition
    I2C0_CTL0 |= I2C_CTL0_STOP;
    
    return PLATFORM_OK;
}

static platform_status_t gd32_i2c_read(uint8_t addr, uint8_t *data, size_t len) {
    // Generate START condition
    I2C0_CTL0 |= I2C_CTL0_START;
    if (gd32_i2c_wait_for_flag(I2C_STAT1_ADDSEND, 100) != PLATFORM_OK) {
        return PLATFORM_ERROR;
    }
    
    // Send slave address with read bit
    I2C0_DATA = (addr << 1) | 1;
    if (gd32_i2c_wait_for_flag(I2C_STAT1_ADDSEND, 100) != PLATFORM_OK) {
        return PLATFORM_ERROR;
    }
    
    // Enable ACK for all bytes except last
    I2C0_CTL0 |= I2C_CTL0_ACKEN;
    
    // Read data
    for (size_t i = 0; i < len; i++) {
        if (i == len - 1) {
            // Last byte - disable ACK
            I2C0_CTL0 &= ~I2C_CTL0_ACKEN;
        }
        
        if (gd32_i2c_wait_for_flag(I2C_STAT1_RXNE, 100) != PLATFORM_OK) {
            return PLATFORM_ERROR;
        }
        
        data[i] = (uint8_t)I2C0_DATA;
    }
    
    // Generate STOP condition
    I2C0_CTL0 |= I2C_CTL0_STOP;
    
    return PLATFORM_OK;
}

static platform_status_t gd32_i2c_write_reg(uint8_t addr, uint16_t reg, const uint8_t *data, size_t len) {
    uint8_t buffer[2 + len];
    buffer[0] = (reg >> 8) & 0xFF;
    buffer[1] = reg & 0xFF;
    safe_memcpy(&buffer[2], data, len);
    return gd32_i2c_write(addr, buffer, 2 + len);
}

static platform_status_t gd32_i2c_read_reg(uint8_t addr, uint16_t reg, uint8_t *data, size_t len) {
    uint8_t reg_buffer[2];
    reg_buffer[0] = (reg >> 8) & 0xFF;
    reg_buffer[1] = reg & 0xFF;
    
    // Write register address first
    if (gd32_i2c_write(addr, reg_buffer, 2) != PLATFORM_OK) {
        return PLATFORM_ERROR;
    }
    
    // Then read data
    return gd32_i2c_read(addr, data, len);
}

/* GPIO implementation */
static platform_status_t gd32_gpio_init(uint32_t pin, uint32_t mode) {
    // Simple GPIO implementation - would need expansion for real use
    return PLATFORM_OK;
}

static platform_status_t gd32_gpio_set(uint32_t pin, uint32_t state) {
    return PLATFORM_OK;
}

static platform_status_t gd32_gpio_get(uint32_t pin, uint32_t *state) {
    return PLATFORM_OK;
}

/* Timer/Delay implementation */
static void gd32_delay_ms(uint32_t ms) {
    volatile uint32_t count = ms * 48000; // Approximate for 48MHz
    while (count--) {
        __asm("nop");
    }
}

static void gd32_delay_us(uint32_t us) {
    volatile uint32_t count = us * 48; // Approximate for 48MHz
    while (count--) {
        __asm("nop");
    }
}

static uint32_t gd32_get_tick_ms(void) {
    static uint32_t tick_count = 0;
    return tick_count++; // Simple implementation - would need SysTick for real use
}

static uint32_t gd32_get_tick_us(void) {
    return gd32_get_tick_ms() * 1000;
}

/* System implementation */
static platform_status_t gd32_system_init(void) {
    gd32_system_clock_init();
    return PLATFORM_OK;
}

static void gd32_system_reset(void) {
    // System reset implementation
    while(1);
}

static uint32_t gd32_get_system_clock(void) {
    return SYSTEM_CLOCK_HZ;
}

/* Platform interface definition */
const platform_interface_t gd32_platform_interface = {
    .uart_init = gd32_uart_init,
    .uart_send = gd32_uart_send,
    .uart_receive = gd32_uart_receive,
    .uart_send_string = gd32_uart_send_string,
    
    .i2c_init = gd32_i2c_init,
    .i2c_write = gd32_i2c_write,
    .i2c_read = gd32_i2c_read,
    .i2c_write_reg = gd32_i2c_write_reg,
    .i2c_read_reg = gd32_i2c_read_reg,
    
    .gpio_init = gd32_gpio_init,
    .gpio_set = gd32_gpio_set,
    .gpio_get = gd32_gpio_get,
    
    .delay_ms = gd32_delay_ms,
    .delay_us = gd32_delay_us,
    .get_tick_ms = gd32_get_tick_ms,
    .get_tick_us = gd32_get_tick_us,
    
    .system_init = gd32_system_init,
    .system_reset = gd32_system_reset,
    .get_system_clock = gd32_get_system_clock
};
