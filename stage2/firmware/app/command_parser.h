/**
 * @file command_parser.h
 * @brief UART command parser interface
 */

#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdint.h>

/* Command parser return codes */
typedef enum {
    CMD_PARSER_OK = 0,
    CMD_PARSER_ERROR = -1,
    CMD_PARSER_UNKNOWN = -2,
    CMD_PARSER_INVALID_ARGS = -3
} cmd_parser_status_t;

/* Command parser functions */
int command_parser_init(void);
int command_parser_deinit(void);
int command_parser_process(const char *command);
int command_parser_help(void);
int command_parser_status(void);

#endif /* COMMAND_PARSER_H */
