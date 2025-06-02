#include "commands.h"

void cmd_help() {
    clear_screen(); //funcion para limpiar pantalla

    set_zoom(2);
    set_cursor(0, 0);
    putString("Available commands:\n");
    putString("  help          - Show this help message\n");
    putString("  time          - Display current system date and time\n");
    putString("  regs          - Show CPU registers\n");
    putString("  pongis        - Launch the Pong-Golf mini-game\n");
    putString("  div0          - Trigger a divide-by-zero exception\n");
    putString("  ud2           - Trigger an invalid-opcode exception\n");
    putString("  zoom <NUMBER> - Set text zoom level (1-10)\n");
    putString("  exit          - Exit the shell\n");
    putChar('\n');
    putString("\t Press 'c' to exit window");
    hltUntil_c();
}

void cmd_time() {
    putString("\nCurrent system time: ");
    getTime(); // Assuming getTime() prints the time directly
    putChar('\n');
}

void cmd_registers() {
    clear_screen();
    showRegisters();
    putChar('\n');
    putString("\t Press 'c' to quit");
    hltUntil_c();
}

void cmd_pong() {

}

void cmd_div0() {
    _div0();
}

void cmd_invalid_opcode() {
    _ioc();
}

void cmd_clear(void) {
    clear_screen();
}

void cmd_zoom(int zoom_level) {
    set_zoom(zoom_level);
}

void cmd_exit(void) {
    sys_call(SYS_EXIT, 0, 0, 0, 0, 0);
}