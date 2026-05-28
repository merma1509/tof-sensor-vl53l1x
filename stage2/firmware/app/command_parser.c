/**
 * @file command_parser.c
 * @brief UART command parser implementation
 */

#include "command_parser.h"
#include "tof_service.h"
#include "../platform/platform.h"
#include "../drivers/common/string_utils.h"

// Standard definitions for bare-metal environment
#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef SIZE_MAX
#define SIZE_MAX ((size_t)-1)
#endif

// Using common utilities for library functions

// Minimal string functions for bare-metal environment
static int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}


// Using safe_strlen from common utilities

/* Command processing functions */
static int cmd_start(const char *args);
static int cmd_stop(const char *args);
static int cmd_calibrate(const char *args);
static int cmd_mode_short(const char *args);
static int cmd_mode_medium(const char *args);
static int cmd_mode_long(const char *args);
static int cmd_status(const char *args);
static int cmd_filter(const char *args);
static int cmd_help(const char *args);

/* Command table */
typedef struct {
    const char *name;
    int (*handler)(const char *args);
    const char *description;
} command_t;

static const command_t commands[] = {
    {"START", cmd_start, "Start distance measurements"},
    {"STOP", cmd_stop, "Stop distance measurements"},
    {"CAL", cmd_calibrate, "Calibrate sensor"},
    {"SHORT", cmd_mode_short, "Set short range mode (15ms)"},
    {"MED", cmd_mode_medium, "Set medium range mode (50ms)"},
    {"MEDIUM", cmd_mode_medium, "Set medium range mode (50ms)"},
    {"LONG", cmd_mode_long, "Set long range mode (100ms)"},
    {"STATUS", cmd_status, "Show system status"},
    {"FILTER", cmd_filter, "Filter control (ON/OFF/STATUS)"},
    {"HELP", cmd_help, "Show this help"},
    {NULL, NULL, NULL} // End marker
};

/* Command parser implementation */
int command_parser_init(void) {
    DEBUG_PRINT("Command parser initialized\r\n");
    return CMD_PARSER_OK;
}

int command_parser_deinit(void) {
    return CMD_PARSER_OK;
}

int command_parser_process(const char *command) {
    if (!command || safe_strlen(command) == 0) {
        return CMD_PARSER_ERROR;
    }
    
    // Simple command parsing - find first space
    const char *args = NULL;
    char cmd_name[16];
    size_t i = 0;
    
    // Extract command name
    while (i < sizeof(cmd_name) - 1 && command[i] && command[i] != ' ') {
        cmd_name[i] = command[i];
        i++;
    }
    cmd_name[i] = '\0';
    
    // Skip to arguments
    if (command[i] == ' ') {
        args = &command[i + 1];
    }
    
    // Find and execute command
    for (i = 0; commands[i].name != NULL; i++) {
        if (strcmp(cmd_name, commands[i].name) == 0) {
            int result = commands[i].handler(args);
            if (result == CMD_PARSER_OK) {
                DEBUG_PRINT("OK\r\n");
            } else if (result == CMD_PARSER_UNKNOWN) {
                DEBUG_PRINT("Unknown command: ");
                DEBUG_PRINT(cmd_name);
                DEBUG_PRINT("\r\n");
            } else {
                DEBUG_PRINT("ERROR\r\n");
            }
            return result;
        }
    }
    
    DEBUG_PRINT("Unknown command: ");
    DEBUG_PRINT(cmd_name);
    DEBUG_PRINT("\r\n");
    return CMD_PARSER_UNKNOWN;
}

int command_parser_help(void) {
    DEBUG_PRINT("Available commands:\r\n");
    for (size_t i = 0; commands[i].name != NULL; i++) {
        DEBUG_PRINT("  ");
        DEBUG_PRINT(commands[i].name);
        DEBUG_PRINT(" - ");
        DEBUG_PRINT(commands[i].description);
        DEBUG_PRINT("\r\n");
    }
    return CMD_PARSER_OK;
}

int command_parser_status(void) {
    char status_buffer[128];
    if (tof_service_get_status(status_buffer, sizeof(status_buffer)) == 0) {
        DEBUG_PRINT(status_buffer);
        DEBUG_PRINT("\r\n");
    }
    return CMD_PARSER_OK;
}

/* Command implementations */
static int cmd_start(const char *args) {
    (void)args; // Unused
    
    if (tof_service_get_state() != TOF_STATE_READY) {
        DEBUG_PRINT("ERROR: Sensor not ready\r\n");
        return CMD_PARSER_ERROR;
    }
    
    if (tof_service_start_ranging() == 0) {
        DEBUG_PRINT("Ranging started\r\n");
        return CMD_PARSER_OK;
    }
    
    return CMD_PARSER_ERROR;
}

static int cmd_stop(const char *args) {
    (void)args; // Unused
    
    if (tof_service_get_state() != TOF_STATE_RANGING) {
        DEBUG_PRINT("ERROR: Not currently ranging\r\n");
        return CMD_PARSER_ERROR;
    }
    
    if (tof_service_stop_ranging() == 0) {
        DEBUG_PRINT("Ranging stopped\r\n");
        return CMD_PARSER_OK;
    }
    
    return CMD_PARSER_ERROR;
}

static int cmd_calibrate(const char *args) {
    (void)args; // Unused
    
    DEBUG_PRINT("Calibrating sensor...\r\n");
    
    // TODO: Implement calibration
    PLATFORM_DELAY_MS(1000); // Simulate calibration time
    
    DEBUG_PRINT("Calibration complete\r\n");
    return CMD_PARSER_OK;
}

static int cmd_mode_short(const char *args) {
    (void)args; // Unused
    
    if (tof_service_set_mode(TOF_MODE_SHORT) == 0) {
        DEBUG_PRINT("Short range mode set (15ms)\r\n");
        return CMD_PARSER_OK;
    }
    
    return CMD_PARSER_ERROR;
}

static int cmd_mode_medium(const char *args) {
    (void)args; // Unused
    
    if (tof_service_set_mode(TOF_MODE_MEDIUM) == 0) {
        DEBUG_PRINT("Medium range mode set (50ms)\r\n");
        return CMD_PARSER_OK;
    }
    
    return CMD_PARSER_ERROR;
}

static int cmd_mode_long(const char *args) {
    (void)args; // Unused
    
    if (tof_service_set_mode(TOF_MODE_LONG) == 0) {
        DEBUG_PRINT("Long range mode set (100ms)\r\n");
        return CMD_PARSER_OK;
    }
    
    return CMD_PARSER_ERROR;
}

static int cmd_status(const char *args) {
    (void)args; // Unused
    return command_parser_status();
}

static int cmd_filter(const char *args) {
#if FILTER_ENABLED
    if (!args || safe_strlen(args) == 0) {
        DEBUG_PRINT("Usage: FILTER ON|OFF|STATUS\r\n");
        return CMD_PARSER_INVALID_ARGS;
    }
    
    if (strcmp(args, "ON") == 0) {
        return tof_service_filter_enable();
    } else if (strcmp(args, "OFF") == 0) {
        return tof_service_filter_disable();
    } else if (strcmp(args, "STATUS") == 0) {
        uint8_t enabled;
        if (tof_service_filter_get_enabled(&enabled) == 0) {
            DEBUG_PRINT("Filter: ");
            DEBUG_PRINT(enabled ? "ON" : "OFF");
            DEBUG_PRINT("\r\n");
            return CMD_PARSER_OK;
        }
    } else {
        DEBUG_PRINT("Usage: FILTER ON|OFF|STATUS\r\n");
        return CMD_PARSER_INVALID_ARGS;
    }
#else
    DEBUG_PRINT("Filter not enabled in this build\r\n");
#endif
    
    return CMD_PARSER_ERROR;
}

static int cmd_help(const char *args) {
    (void)args; // Unused
    return command_parser_help();
}
