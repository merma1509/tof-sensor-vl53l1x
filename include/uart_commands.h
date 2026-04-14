/**
 * @file  uart_commands.h
 * @brief UART command interface for VL53L1X control
 */

#ifndef UART_COMMANDS_H
#define UART_COMMANDS_H

#include <stdint.h>

/* Command definitions */
#define CMD_START         "START"		    // Start measurements
#define CMD_STOP          "STOP"		        // Stop measurements
#define CMD_CALIBRATE     "CAL"		        // Calibrate sensor
#define CMD_MODE_SHORT    "SHORT"		    // Short range mode
#define CMD_MODE_MEDIUM   "MEDIUM"		    // Medium range mode
#define CMD_MODE_LONG     "LONG"		        // Long range mode
#define CMD_STATUS        "STATUS"		    // Show status
#define CMD_HELP          "HELP"		        // Show help
#define CMD_FILTER_ON     "FILTER ON"	    // Enable filter
#define CMD_FILTER_OFF    "FILTER OFF"	    // Disable filter
#define CMD_FILTER_STATUS "FILTER STATUS"	// Show filter status

/* Command response codes */
#define RESP_OK          "OK"		        // Command executed successfully
#define RESP_ERROR       "ERROR"		    // Command failed
#define RESP_BUSY        "BUSY"		        // System is busy
#define RESP_READY       "READY"		    // System is ready

/* Function declarations */
void uart_commands_init(void);
void uart_init(void);
void uart_command_loop(void);
void uart_process_command(char *cmd);
void uart_send_response(const char *response);
void uart_send_distance(uint16_t distance);
void uart_show_help(void);
char uart_receive_char(void);
void uart_send_string(const char *str);

#endif /* UART_COMMANDS_H */
