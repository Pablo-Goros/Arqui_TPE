#include "shell.h"

#define MAX_CMD_LENGTH 256 

void shell(void) {
    clear_screen();
    set_cursor(0, 0);   
    putString("Welcome to the!\n");
    putString("Type 'help' for a list of commands.\n");
    while (1) {
        char command[MAX_CMD_LENGTH];
        int len = 0;

        // Prompt for input
        putString("\\> ");

        // Read command
        while (len < MAX_CMD_LENGTH - 1) {
            char c = getChar();
            if (c == '\n' || c == '\r') {
                break; // End of command
            } else if ((c == 0x7F || c == 0x08) && len > 0) { // Handle backspace
                putChar('\b');
                putChar(' ');  // Clear the character
                putChar('\b');
                len--;
            } else if (len < MAX_CMD_LENGTH-1 && c >= ' ') {
                command[len++] = c;
                
                putChar(c);
            } else if (len >= MAX_CMD_LENGTH - 1) {
                putString("\nCommand too long, try again.\n");
                len = 0; // Reset length for new command
                break; // Exit the input loop
            }
        }
        command[len] = '\0';   
             
        if      (strcmp(command, "help")==0)            cmd_help();
        else if (strcmp(command, "time")==0)            cmd_time();
        else if (strcmp(command, "regs")==0)            cmd_registers();
        else if (strcmp(command, "pong")==0)            cmd_pong();
        else if (strcmp(command, "div0")==0)            cmd_div0();
        else if (strcmp(command, "ud2")==0)             cmd_invalid_opcode();
        else                                            putString("Unknown command\n");

    }
    return;
}