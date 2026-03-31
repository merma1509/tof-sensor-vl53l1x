/**
 * @file  uart_commands.h
 * @brief UART command interface for VL53L1X control
 */

#ifndef UART_COMMANDS_H
#define UART_COMMANDS_H

#include <stdint.h>

/* Command definitions */
#define CMD_START        "START"
#define CMD_STOP         "STOP"
#define CMD_CALIBRATE    "CAL"
#define CMD_MODE_SHORT   "SHORT"
#define CMD_MODE_MEDIUM  "MEDIUM"
#define CMD_MODE_LONG    "LONG"
#define CMD_STATUS       "STATUS"
#define CMD_HELP         "HELP"

/* Command response codes */
#define RESP_OK          "OK"
#define RESP_ERROR       "ERROR"
#define RESP_BUSY        "BUSY"
#define RESP_READY       "READY"

/* Function declarations */
void uart_commands_init(void);
void uart_init(void);
void uart_command_loop(void);
void uart_process_command(char *cmd);
void uart_send_response(const char *response);
void uart_send_distance(uint16_t distance);
void uart_show_help(void);
char uart_receive_char(void);

#endif /* UART_COMMANDS_H */
