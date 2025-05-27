#include "commands.h"
#include <libc.h>

void cmd_help() {
    clear_screen(); //funcion para limpiar pantalla
    help();//imprime los mensajes

    set_zoom(2);
    set_cursor(0, 0);
    putString("Available commands:\n");
    putString("  help          - Show this help message\n");
    putString("  time          - Display current system date and time\n");
    putString("  regs          - Show CPU registers\n");
    putString("  pong          - Launch the Pong-Golf mini-game\n");
    putString("  div0          - Trigger a divide-by-zero exception\n");
    putString("  ud2           - Trigger an invalid-opcode exception\n");
    putString("  zoom <NUMBER> - Set text zoom level (1-10)\n");
    putString("  zoom in       - Increase text zoom level\n");
    putString("  zoom out      - Decrease text zoom level\n");
    putString("  exit          - Exit the shell\n");
    putChar('\n');
    putString("\t Press 'q' to quit");
    hltUntilQ();

}

void cmd_time() {
    Time *t = getTime();
    char buf[9];          // "hh:mm:ss\0"

    buf[0] = '0' + t->hour / 10;
    buf[1] = '0' + t->hour % 10;
    buf[2] = ':';
    buf[3] = '0' + t->min  / 10;
    buf[4] = '0' + t->min  % 10;
    buf[5] = ':';
    buf[6] = '0' + t->sec  / 10;
    buf[7] = '0' + t->sec  % 10;
    buf[8] = '\0';

    putString("Current system time: ");
    putString(buf);
    putChar('\n');
}

void cmd_registers() {
    
}

void cmd_pong() {

}

void cmd_div0() {

}

void cmd_invalid_opcode() {

}

void cmd_clear(void) {
    clear_screen();
}

void cmd_zoom(int zoom_level) {
    set_zoom(zoom_level);
}

void cmd_exit(void) {
    syscall(SYS_EXIT, 0, 0, 0, 0);
}