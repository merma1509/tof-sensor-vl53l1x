/**
 * @file  clang_compat.h
 * @brief Compatibility header for Clang IDE lint
 * 
 * This file provides stub definitions for Clang IDE lint
 * when it cannot find GD32 SDK headers. The actual
 * compilation uses real GD32 SDK headers.
 */

#ifndef CLANG_COMPAT_H
#define CLANG_COMPAT_H

#include <stdint.h>

/* Standard types for Clang when stdint.h isn't available */
#ifndef __UINT32_TYPE__
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
#endif

/* GD32 Register access macro */
#ifndef REG32
#define REG32(addr) (*(volatile uint32_t *)(uint32_t)(addr))
#endif

/* BIT macro */
#ifndef BIT
#define BIT(x) ((uint32_t)((uint32_t)0x01U << (x)))
#endif

/* BITS macro */
#ifndef BITS
#define BITS(start, end) ((0xFFFFFFFFU << (start)) & (0xFFFFFFFFU >> (31U - (uint32_t)(end))))
#endif

/* I2C Base address */
#ifndef I2C_BASE
#define I2C_BASE ((uint32_t)0x40005400U)
#endif

#ifndef I2C0_BASE
#define I2C0_BASE I2C_BASE
#endif

#ifndef I2C1_BASE
#define I2C1_BASE (I2C_BASE + 0x00000400U)
#endif

/* GPIO Base addresses */
#ifndef GPIO_BASE
#define GPIO_BASE ((uint32_t)0x48000000U)
#endif

#ifndef GPIOA_BASE
#define GPIOA_BASE (GPIO_BASE + 0x00000000U)
#endif

#ifndef GPIOB_BASE
#define GPIOB_BASE (GPIO_BASE + 0x00000400U)
#endif

/* USART Base address */
#ifndef USART_BASE
#define USART_BASE ((uint32_t)0x40013800U)
#endif

#ifndef USART0_BASE
#define USART0_BASE (USART_BASE + 0x00000000U)
#endif

/* RCU Base address */
#ifndef RCU_BASE
#define RCU_BASE ((uint32_t)0x40021000U)
#endif

/* Key I2C defines that are commonly used */
/* Note: I2C0 and I2C1 are defined later with I2C0_BASE and I2C1_BASE */

/* __IO qualifier */
#ifndef __IO
#define __IO volatile
#endif

/* GD32 error status */
typedef enum {
    ERROR = 0,
    SUCCESS = !ERROR
} ErrorStatus;

/* Function prototypes that Clang needs to see */
void rcu_periph_clock_enable(uint32_t periph);
void gpio_mode_set(uint32_t gpio_periph, uint32_t mode, uint32_t pull_up_down, uint32_t pin);
void gpio_output_options_set(uint32_t gpio_periph, uint8_t otype, uint32_t speed, uint32_t pin);
void gpio_af_set(uint32_t gpio_periph, uint32_t alt_func_num, uint32_t pin);
void i2c_clock_config(uint32_t i2c_periph, uint32_t clkspeed, uint32_t dutycycle);
void i2c_mode_addr_config(uint32_t i2c_periph, uint32_t mode, uint32_t addformat, uint32_t addr);
void i2c_enable(uint32_t i2c_periph);
void i2c_deinit(uint32_t i2c_periph);
void usart_baudrate_set(uint32_t usart_periph, uint32_t baudval);
void usart_word_length_set(uint32_t usart_periph, uint32_t wlen);
void usart_stop_bit_set(uint32_t usart_periph, uint32_t stblen);
void usart_parity_config(uint32_t usart_periph, uint32_t paritycfg);
void usart_hardware_flow_rts_config(uint32_t usart_periph, uint32_t rtsconfig);
void usart_hardware_flow_cts_config(uint32_t usart_periph, uint32_t ctsconfig);
void usart_transmit_config(uint32_t usart_periph, uint32_t txconfig);
void usart_receive_config(uint32_t usart_periph, uint32_t rxconfig);
void usart_enable(uint32_t usart_periph);
void usart_deinit(uint32_t usart_periph);

/* Key constants */
#define RCU_GPIOA ((uint32_t)0x00000002U)
#define RCU_GPIOB ((uint32_t)0x00000004U)
#define RCU_I2C0  ((uint32_t)0x00000020U)
#define RCU_USART0 ((uint32_t)0x00004000U)

#define GPIO_MODE_OUTPUT ((uint8_t)0x01U)
#define GPIO_MODE_INPUT  ((uint8_t)0x00U)
#define GPIO_PUPD_PULLUP ((uint8_t)0x01U)
#define GPIO_OTYPE_PP    ((uint8_t)0x00U)
#define GPIO_OSPEED_50MHZ ((uint8_t)0x03U)
#define GPIO_AF_1        ((uint8_t)0x01U)

#define GPIO_PIN_6  ((uint16_t)0x0040U)
#define GPIO_PIN_7  ((uint16_t)0x0080U)
#define GPIO_PIN_9  ((uint16_t)0x0200U)
#define GPIO_PIN_10 ((uint16_t)0x0400U)

#define I2C_DTCY_2    ((uint32_t)0x00000000U)
#define I2C_I2CMODE_ENABLE ((uint32_t)0x00000000U)
#define I2C_ADDFORMAT_7BITS ((uint32_t)0x00000000U)

#define USART_WL_8BIT ((uint32_t)0x00000000U)
#define USART_STB_1BIT ((uint32_t)0x00000000U)
#define USART_PM_NONE ((uint32_t)0x00000000U)
#define USART_RTS_DISABLE ((uint32_t)0x00000000U)
#define USART_CTS_DISABLE ((uint32_t)0x00000000U)
#define USART_TXPIN_ENABLE ((uint32_t)0x00000008U)
#define USART_RXPIN_ENABLE ((uint32_t)0x00000004U)

/* I2C register bit definitions */
#define I2C_CTL0_I2CEN  BIT(0)
#define I2C_CTL0_SMBEN  BIT(1)
#define I2C_CTL0_START  BIT(8)
#define I2C_CTL0_STOP   BIT(9)
#define I2C_CTL0_ACKEN  BIT(10)
#define I2C_STAT0_SBSEND BIT(0)
#define I2C_STAT0_ADDSEND BIT(1)
#define I2C_STAT0_BTC   BIT(2)
#define I2C_STAT0_RBNE  BIT(6)
#define I2C_STAT0_TBE   BIT(7)
#define I2C_STAT0_BERR  BIT(8)
#define I2C_STAT0_LOSTARB BIT(9)
#define I2C_STAT0_AERR  BIT(10)
#define I2C_STAT0_NACKF BIT(14)
#define I2C_STAT1_MASTER BIT(0)
#define I2C_STAT1_I2CBSY BIT(1)

/* Base addresses for peripherals */
#ifndef I2C0
#define I2C0 I2C0_BASE
#endif
#ifndef I2C1
#define I2C1 I2C1_BASE
#endif
#ifndef USART0
#define USART0 USART0_BASE
#endif
#ifndef GPIOA
#define GPIOA GPIOA_BASE
#endif
#ifndef GPIOB
#define GPIOB GPIOB_BASE
#endif

/* System init */
void SystemInit(void);

#endif /* CLANG_COMPAT_H */
