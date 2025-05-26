#ifndef COMMANDS_H
#define COMMANDS_H

#include "libc.h"  

/**
 * @file commands.h
 * @brief Declarations for all built-in shell commands.
 */

/**
 * @brief Show a list of available commands.
 */
void cmd_help(void);

/**
 * @brief Display the current system date and time.
 */
void cmd_time(void);

/**
 * @brief Dump all CPU registers to the console.
 */
void cmd_registers(void);

/**
 * @brief Launch the Pong-Golf mini-game.
 */
void cmd_pong(void);

/**
 * @brief Trigger a divide-by-zero exception for testing.
 */
void cmd_div0(void);

/**
 * @brief Trigger an invalid-opcode exception for testing.
 */
void cmd_invalid_opcode(void);

/**
 * @brief Clear the terminal screen.
 */
void cmd_clear(void);

/**
 * @brief Halt the system.
 */
void cmd_shutdown(void);

/**
 * @brief Adjust the text zoom level.
 */
void cmd_zoom(void);

#endif // COMMANDS_H
