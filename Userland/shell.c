#include "shell.h"

#define MAX_CMD_LENGTH 128


static int 
void shell(void) {

    while (1) {
        char command[MAX_CMD_LENGTH];
        int len = 0;

        // Prompt for input
        putString("shell> ");

        // Read command
        while (len < MAX_CMD_LENGTH - 1) {
            char c = getChar();
            if (c == '\n' || c == '\r') {
                break; // End of command
            } else if ((c == 0x7F || c == 0x08) && len > 0) { // Handle backspace
                putChar(' ');  // Clear the character
            } else {
                command[len++] = c;
                putChar(c); // Echo the character
            }
        }
        command[len] = '\0';         
    }
    return;
}