/**
 * @file  uart_init.c
 * @brief UART initialization for GD32E230
 */

#include "../include/uart_commands.h"

// Forward declaration
static void uart_send_string(const char *str);

/* UART send string function */
static void uart_send_string(const char *str) {
    while (*str) {
        // Wait for transmit buffer empty
        while (!(*(volatile uint32_t*)0x40011008 & (1 << 7))); // USART_STAT_TBE
        *(volatile uint32_t*)0x4001100C = *str; // USART_DATA
        str++;
    }
}

/* UART initialization function */
void uart_init(void) {
    // Enable GPIOA clock for PA9(TX) and PA10(RX)
    volatile uint32_t *RCU_AHBEN = (uint32_t*)0x40021014;
    *RCU_AHBEN |= (1 << 17); // GPIOA enable
    
    // Enable USART0 clock
    volatile uint32_t *RCU_APB2EN = (uint32_t*)0x4002101C;
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
    volatile uint32_t *USART0_CTL0 = (uint32_t*)0x40011000;
    volatile uint32_t *USART0_CTL1 = (uint32_t*)0x40011004;
    volatile uint32_t *USART0_CTL2 = (uint32_t*)0x40011008;
    volatile uint32_t *USART0_BAUD = (uint32_t*)0x4001100C;
    
    // Reset USART0
    *USART0_CTL0 = 0;
    *USART0_CTL1 = 0;
    *USART0_CTL2 = 0;
    
    // Set baud rate: 115200 at 48MHz
    // Baud = Fck / (16 * USARTDIV)
    // USARTDIV = 48000000 / (16 * 115200) = 26.0417
    // DIV_MANTISSA = 26, DIV_FRAC = 0.0417 * 16 = 0.667 ≈ 1
    *USART0_BAUD = (26 << 4) | 1; // DIV_MANTISSA=26, DIV_FRAC=1
    
    // Configure USART0: 8N1, TX enable, RX enable, USART enable
    *USART0_CTL0 = (1 << 2) | (1 << 3); // TXEN, RXEN
    *USART0_CTL1 = (1 << 0);           // USART enable
    
    // Wait for USART to be ready
    while (!(*USART0_CTL1 & (1 << 6))); // Wait for TC flag
}

/* UART receive character function */
char uart_receive_char(void) {
    volatile uint32_t *USART0_STAT = (uint32_t*)0x40011008;
    volatile uint32_t *USART0_DATA = (uint32_t*)0x4001100C;
    
    // Wait for data received
    while (!(*USART0_STAT & (1 << 5))); // Wait for RBNE flag
    
    return (char)(*USART0_DATA);
}

/* UART command processing loop */
void uart_command_loop(void) {
    char buffer[64];
    int index = 0;
    
    while (1) {
        char c = uart_receive_char();
        
        if (c == '\r' || c == '\n') {
            if (index > 0) {
                buffer[index] = '\0';
                uart_send_string("\r\n");
                uart_process_command(buffer);
                index = 0;
            }
        } else if (c >= 32 && c < 127) { // Printable characters
            if (index < 63) {
                buffer[index++] = c;
                // Echo character
                volatile uint32_t *USART0_DATA = (uint32_t*)0x4001100C;
                *USART0_DATA = c;
            }
        } else if (c == 8 || c == 127) { // Backspace
            if (index > 0) {
                index--;
                // Send backspace
                volatile uint32_t *USART0_DATA = (uint32_t*)0x4001100C;
                *USART0_DATA = 8;
                *USART0_DATA = ' ';
                *USART0_DATA = 8;
            }
        }
    }
}
