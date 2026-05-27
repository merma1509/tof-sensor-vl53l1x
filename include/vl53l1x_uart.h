#ifndef VL53L1X_UART_H
#define VL53L1X_UART_H

#include <stdint.h>

// UART0 register addresses
#define UART0_BASE          0x40011000
#define UART0_CTL0         (*((volatile uint32_t *)(UART0_BASE + 0x00))
#define UART0_CTL1         (*((volatile uint32_t *)(UART0_BASE + 0x04))
#define UART0_BAUD         (*((volatile uint32_t *)(UART0_BASE + 0x08))
#define UART0_GPCTL        (*((volatile uint32_t *)(UART0_BASE + 0x0C))
#define UART0_RXDATA       (*((volatile uint32_t *)(UART0_BASE + 0x10))
#define UART0_TXDATA       (*((volatile uint32_t *)(UART0_BASE + 0x14))
#define UART0_INTEN        (*((volatile uint32_t *)(UART0_BASE + 0x18))
#define UART0_INTF         (*((volatile uint32_t *)(UART0_BASE + 0x1C))
#define UART0_STAT         (*((volatile uint32_t *)(UART0_BASE + 0x20))

// UART control bits
#define UART0_CTL0_UEN        (1 << 0)
#define UART0_CTL0_TEN        (1 << 3)
#define UART0_CTL0_REN        (1 << 2)

// UART status bits
#define UART0_STAT_TBE        (1 << 7)
#define UART0_STAT_RBNE       (1 << 5)

// VL53L1X measurement results
typedef struct {
    uint16_t distance_mm;
    uint8_t status;
    uint16_t ambient;
    uint32_t timestamp;
} VL53L1X_Measurement_t;

// Function prototypes
void UART0_Init(void);
void UART0_SendChar(char c);
void UART0_SendString(const char* str);
void UART0_SendNumber(uint32_t num);
void VL53L1X_Init(void);
uint16_t VL53L1X_GetDistance(void);
void VL53L1X_SendMeasurement(uint16_t distance);
void VL53L1X_ProcessCommand(char cmd);

// Commands
#define CMD_START_MEAS   'S'
#define CMD_STOP_MEAS    'T'
#define CMD_CALIBRATE    'C'
#define CMD_CHANGE_MODE  'M'

#endif // VL53L1X_UART_H
