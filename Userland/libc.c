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

char    *strcpy(char *dest, const char *src) {
    strncpy(dest, src, strlen(src) + 1); // +1 to include null terminator
    return dest;
}

char   *strncpy(char *dest, const char *src, uint64_t n) {
    uint64_t i;

    // Copy up to n chars or until '\0'
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    // If we hit end of src before n, pad with '\0'
    for (; i < n; i++) {
        dest[i] = '\0';
    }
    return dest;
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


// Case-insensitive compare 
int strcasecmp(const char *s1, const char *s2) {
    unsigned char a, b;
    do {
        a = (unsigned char)*s1++;
        b = (unsigned char)*s2++;

        // Convert to uppercase if lowercase
        a = toupper(a);
        b = toupper(b);
    } while (a == b && a);
    return a - b;
}


// Returns non-zero if c is A–Z or a–z
int isalpha(int c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// Convert lowercase ASCII to uppercase; leave others unchanged
int toupper(int c) {
    if (c >= 'a' && c <= 'z')
        return c - ('a' - 'A');
    return c;
}

// Convert uppercase ASCII to lowercase; leave others unchanged
int tolower(int c) {
    if (c >= 'A' && c <= 'Z')
        return c + ('a' - 'A');
    return c;
}


//void    *memset(void *s, int c, size_t n);
//void    *memcpy(void *dest, const void *src, size_t n);

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

uint64_t get_ticks(){
    return sys_call(SYS_GET_TICKS, 0, 0, 0, 0, 0);
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
        for (volatile int j = 0; j < 20000*duration[i]; j++);
    }
}

void system_start_sound() {
    unsigned int melody[] = { 523, 659, 784, 1046};
    unsigned int duration[] = { 300, 300, 300, 500};

    for (int i = 0; i < 4; i++) {
        sys_call(SYS_BEEP, melody[i], duration[i], 0, 0, 0);
        for (volatile int j = 0; j < 15000*duration[i]; j++);
    }
}


//! LA VAMOS A USAR DESP SI IMPLEMENTAMOS DOS BUFFERS
// int  blit(const void *user_buffer, uint64_t size); /* copy `size` bytes from user‐buffer into VRAM */


