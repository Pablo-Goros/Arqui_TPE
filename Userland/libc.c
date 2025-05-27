#include "libc.h"

static int scan(char * buf, uint32_t count) {
    return sys_call((uint64_t) SYS_READ, (uint64_t) STDIN,(uint64_t) buf,(uint64_t) count, 0);
}


/* ------------------------------------------------------------------------- */
/* Basic character I/O (built on sys_read/write)                             */
/* ------------------------------------------------------------------------- */

void putChar(char c) {
    sys_call(SYS_WRITE, STDOUT, (uint64_t)&c, 1, 0);
}


char getChar(void) {
    char c;
    scan(&c, 1);
    return c;
}

void putString(const char *str) {
    sys_call(SYS_WRITE, STDOUT, (uint64_t)str, strlen(str), 0);
}

//? la vamos a usar? 
/*
int getLine(char *buf, size_t maxlen) {
    scan(buf, maxlen);
    return buf;   
}
*/
/* ------------------------------------------------------------------------- */
/* String and memory routines                                                */
/* ------------------------------------------------------------------------- */
size_t strlen(const char *s) {
    size_t len = 0;
    while (s[len]) len++;
    return len;
}

char    *strcpy(char *dest, const char *src) {
    strncpy(dest, src, strlen(src) + 1); // +1 to include null terminator
    return dest;
}

char    *strncpy(char *dest, const char *src, size_t n) {
    size_t i;

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


void    *memset(void *s, int c, size_t n);
void    *memcpy(void *dest, const void *src, size_t n);

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

Time* getTime() {
	return (Time*) sys_call(SYS_GET_TIME, 0, 0, 0, 0);
}

uint64_t getTicks(){
    return sys_call(SYS_GET_TICKS, 0, 0, 0, 0);
}

void sleep(uint64_t seconds) {
    sys_call(SYS_SLEEP, seconds, 0, 0, 0);
}



/* ------------------------------------------------------------------------- */
/* Cursor control & screen clearing                                          */
/* ------------------------------------------------------------------------- */

void clear_screen(void) {
    // invoke SYS_CLEAR_SCREEN (no args)
    sys_call(SYS_CLEAR_SCREEN, 0, 0, 0, 0);
}

void set_cursor(uint32_t col, uint32_t row) {
    // invoke SYS_SET_CURSOR with (col, row)
    sys_call(SYS_SET_CURSOR, (uint64_t)col, (uint64_t)row, 0, 0);
}

/* ------------------------------------------------------------------------- */
/* Exception / halt helper                                                    */
/* ------------------------------------------------------------------------- */

void hltUntilQ() {
    while (getChar() != 'q') {
        _hlt();
    }
}

/* ------------------------------------------------------------------------- */
/* CPU register inspection                                                   */
/* ------------------------------------------------------------------------- */

uint64_t *getRegisters(void) {
    return (uint64_t *) sys_call(SYS_REGISTERS, 0, 0, 0, 0);
}

void showRegisters(void) {
    static const char *names[16] = {
        "RAX","RBX","RCX","RDX",
        "RSI","RDI","RBP","RSP",
        "R8", "R9", "R10","R11",
        "R12","R13","R14","R15"
    };

    uint64_t *regs = getRegisters();
    if (!regs) {
        putString("Error: could not fetch registers\n");
        return;
    }

    char buf[32];
    for (int i = 0; i < 16; i++) {
        putString(names[i]);
        putString(": 0x");
        itoa(regs[i], buf, 16);
        putString(buf);
        putChar('\n');
    }
}
