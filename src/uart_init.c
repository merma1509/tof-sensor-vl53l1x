/**
 * @file  uart_init.c
 * @brief UART initialization for GD32E230
 */

#include "../include/uart_commands.h"

/* UART initialization function */
void uart_init(void) {
    // Enable GPIOA clock for PA9(TX) and PA10(RX)
    volatile uint32_t *RCU_AHBEN = (uint32_t*)0x40021014;
    *RCU_AHBEN |= (1 << 0); // GPIOA enable
    
    // Enable USART0 clock
    volatile uint32_t *RCU_APB2EN = (uint32_t*)0x40021018;
    *RCU_APB2EN |= (1 << 14); // USART0 enable
    
    // Configure PA9 (TX) and PA10 (RX) as alternate function
    volatile uint32_t *GPIO_CTL0 = (uint32_t*)0x48000000;
    // PA9 - AF1 (USART0_TX)
    *GPIO_CTL0 &= ~(0xF << 4); // Clear PA9
    *GPIO_CTL0 |= (0x1 << 4);  // AF1
    // PA10 - AF1 (USART0_RX)
    *GPIO_CTL0 &= ~(0xF << 8); // Clear PA10
    *GPIO_CTL0 |= (0x1 << 8);  // AF1
    
    // Configure USART0 for 115200 baud at 48MHz
    volatile uint32_t *USART0_STAT = (uint32_t*)0x40013800;
    volatile uint32_t *USART0_DATA = (uint32_t*)0x40013804;
    volatile uint32_t *USART0_BAUD = (uint32_t*)0x40013808;
    volatile uint32_t *USART0_CTL0 = (uint32_t*)0x4001380C;
    volatile uint32_t *USART0_CTL1 = (uint32_t*)0x40013810;
    volatile uint32_t *USART0_CTL2 = (uint32_t*)0x40013814;
    
    // Reset USART0
    *USART0_CTL0 = 0;
    *USART0_CTL1 = 0;
    *USART0_CTL2 = 0;

    // Set baud rate: 115200 at 48MHz
    // Baud = Fck / (16 * USARTDIV)
    // USARTDIV = 48000000 / (16 * 115200) = 26.0417
    // DIV_MANTISSA = 26, DIV_FRAC = 0.667 * 16 = 0.667 ~ 1
    *USART0_BAUD = (26 << 4) | 1; // DIV_MANTISSA=26, DIV_FRAC=1

    // Configure USART0: 8N1, TX enable, RX enable, USART enable
    *USART0_CTL0 = (1 << 0) | (1 << 2) | (1 << 3); // UEN, TXEN, RXEN
}

/* UART receive character function */
char uart_receive_char(void) {
    volatile uint32_t *USART0_STAT = (uint32_t*)0x40013800;
    volatile uint32_t *USART0_DATA = (uint32_t*)0x40013804;

    // Wait for data received (RBNE flag - bit 5)
    while (!(*USART0_STAT & (1 << 5)));

    return (char)(*USART0_DATA);
}
