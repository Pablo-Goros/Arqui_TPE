#include "libc.h"

/* ------------------------------------------------------------------------- */
/* Basic character I/O (built on sys_read/write)                             */
/* ------------------------------------------------------------------------- */

void putChar(char c) {
    sys_call(SYS_WRITE, STDOUT, (uint64_t)&c, 1, 0, 0);
}

char getChar(void) {
    char c;
    sys_call((uint64_t) SYS_READ, (uint64_t) STDIN,(uint64_t) &c,(uint64_t) 1, 0, 0);
    return c;
}

uint8_t isCharReady(void) {
    return sys_call(SYS_CHAR_READY, 0, 0, 0, 0, 0);
}

void putString(const char *str) {
    sys_call(SYS_WRITE, STDOUT, (uint64_t)str, strlen(str), 0, 0);
}

/* ------------------------------------------------------------------------- */
/* String and memory routines                                                */
/* ------------------------------------------------------------------------- */
uint64_t strlen(const char *s) {
    uint64_t len = 0;
    while (s[len]) len++;
    return len;
}

// Compare two strings byte-wise
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

int strncmp(const char *s1, const char *s2, uint64_t n) {
    while (n-- && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    if (n == (uint64_t)-1) return 0; // If n was 0, they are equal
    return (unsigned char)*s1 - (unsigned char)*s2;
}


/* ------------------------------------------------------------------------- */
/* Numeric conversions                                                       */
/* ------------------------------------------------------------------------- */

// Convert a non‐negative value to a string in buf using the given base (2–36).q2   1
int itoa(int64_t value, char *buf, int base) {
    if (base < 2 || base > 36) {
        *buf = '\0';
        return 0;
    }

    char *p = buf;
    uint64_t u = (uint64_t)value;  // assume value >= 0

    // Generate digits in reverse order
    do {
        int digit = u % base;
        *p++ = (digit < 10 ? '0' + digit : 'a' + (digit - 10));
        u /= base;
    } while (u);

    int len = p - buf;
    *p = '\0';

    // Reverse the buffer in-place
    char *start = buf, *end = p - 1;
    while (start < end) {
        char tmp = *start;
        *start++ = *end;
        *end-- = tmp;
    }

    return len;
}

// Convert a string to int (base 10)
int atoi(const char *s) {
    int result = 0;
    while (*s >= '0' && *s <= '9') {
        result = result * 10 + (*s - '0');
        s++;
    }
    return result;
}


/* ------------------------------------------------------------------------- */
/* Timing and delays                                                         */
/* ------------------------------------------------------------------------- */

void getTime() {
	sys_call(SYS_GET_TIME, 0, 0, 0, 0, 0);
}

void wait_next_tick(void) {
    _hlt(); // Halt the CPU until the next tick
}

/* ------------------------------------------------------------------------- */
/* Cursor control & screen clearing                                          */
/* ------------------------------------------------------------------------- */

void clear_screen(void) {
    sys_call(SYS_CLEAR_SCREEN, 0, 0, 0, 0, 0);
}

void set_cursor(uint32_t col, uint32_t row) {
    sys_call(SYS_SET_CURSOR, (uint64_t)col, (uint64_t)row, 0, 0, 0);
}

void set_zoom(int level) {
    sys_call(SYS_SET_ZOOM, (uint64_t)level, 0, 0, 0, 0);
}
int get_zoom(void) {
    return sys_call(SYS_GET_ZOOM, 0, 0, 0, 0, 0);
}
/* ------------------------------------------------------------------------- */
/* Exception / halt helper                                                    */
/* ------------------------------------------------------------------------- */

void hltUntil_c() {
    while (getChar() != 'c') {
        _hlt();
    }
    clear_screen();
}

/* ------------------------------------------------------------------------- */
/* CPU register inspection                                                   */
/* ------------------------------------------------------------------------- */

void showRegisters(void) {
    sys_call(SYS_REGISTERS, 0, 0, 0, 0, 0);
}

/* ------------------------------------------------------------------------- */
/* Game (Pongis)                                                             */
/* ------------------------------------------------------------------------- */


uint64_t get_mode_info(ModeInfo *mode) {
    return sys_call(SYS_GET_MODE_INFO, (uint64_t)mode, 0, 0, 0, 0);
}


void exception_sound() {
    uint64_t melody[] = { 523, 415};
    uint64_t duration[] = { 150, 300};

    for (int i = 0; i < 2; i++) {
        sys_call(SYS_BEEP, melody[i], duration[i], 0, 0, 0);
    }
}

void system_start_sound() {
    unsigned int melody[] = { 330, 392, 494, 659};
    unsigned int duration[] = { 200, 200, 200, 400};

    for (int i = 0; i < 4; i++) {
        sys_call(SYS_BEEP, melody[i], duration[i], 0, 0, 0);
    }
}




void draw_smile(void){

    int width = 1024; // Width of the screen
    int height = 768; // Height of the screen
    // Draw a simple smiley face using sys_call
    int centerX = width/2; // Center of the face
    int centerY = height/2; // Center of the face
    //FACE
    sys_call(SYS_DRAW_CIRCLE, centerX, centerY, 200, 0xFF8700, 0); // Orange circle
    sys_call(SYS_DRAW_CIRCLE,centerX, centerY, 175, 0xFFFF00, 0); // Yellow circle
    //EYES
    sys_call(SYS_DRAW_RECT, centerX-60, centerY-90, 20, 70, 0x000000); // Left eye
    sys_call(SYS_DRAW_RECT, centerX+40, centerY-90, 20, 70, 0x00000); // Right eye
    // MOUTH
    sys_call(SYS_DRAW_RECT, centerX-90, centerY+55, 180,10, 0x000000); // Black mouth
    sys_call(SYS_DRAW_RECT, centerX-110,centerY+35,10,10, 0x000000); // Left side of mouth
    sys_call(SYS_DRAW_RECT, centerX-100,centerY+45,10,10, 0x000000); // left side of mouth
    sys_call(SYS_DRAW_RECT, centerX+100,centerY+35,10,10, 0x000000); // Right side of mouth
    sys_call(SYS_DRAW_RECT, centerX+90,centerY+45,10,10, 0x000000); // Right side of mouth
}

void draw_welcome(){
    sys_call(SYS_DRAW_RECT, 0, 0,1024, 50,0x9300ff) ; // TOP BORDER in purple
    sys_call(SYS_DRAW_RECT, 0, 718, 1024, 50, 0x9300ff); // BOTTOM BORDER in purple
    //costados violetas
    sys_call(SYS_DRAW_RECT, 0, 50, 10, 668, 0x9300ff); // LEFT BORDER in purple
    sys_call(SYS_DRAW_RECT, 1014, 50, 10, 668, 0x9300ff); // RIGHT BORDER in purple
    //bordes azul
    sys_call(SYS_DRAW_RECT, 10, 50, 10, 668, 0x0020ff); // LEFT BORDER in blue
    sys_call(SYS_DRAW_RECT, 1004, 50, 10, 668, 0x0020ff); // RIGHT BORDER in blue
    sys_call(SYS_DRAW_RECT, 10, 40, 1004, 10, 0x0020ff); // top border in blue
    sys_call(SYS_DRAW_RECT, 10, 718, 1004, 10, 0x0020ff); // bottom border in blue

    for(int i = 10; i < 1004; i+=10){
        if(i%20 == 0) {
            sys_call(SYS_DRAW_RECT, i, 40, 10, 10, 0x0a00c7);
            sys_call(SYS_DRAW_RECT, i, 718, 10, 10, 0x0a00c7);
        }
    }
    for(int i = 0; i < 1024; i+=64){
        if(i%128 == 0) {
            sys_call(SYS_DRAW_RECT, i, 0, 64, 40, 0xff00ff);
            sys_call(SYS_DRAW_RECT, i,728 , 64, 40, 0xff00ff);
        }
    }

    draw_smile(); // Draw smiley face in the center
    //message
    set_zoom (3);
    set_cursor(50, 50);
    putString("ARQUITECTURA DE COMPUTADORAS - ITBA");
    set_cursor(100, 100);
    putString("Trabajo Practico Especial");

    set_zoom(2);
    set_cursor(40, 630);
    putString("Creador por: Pablo Gorostiaga, Tiago Heras y Amador Vallejo\n");
    set_cursor(40, 680);
    putString("Presione 'c' para continuar...");
}
