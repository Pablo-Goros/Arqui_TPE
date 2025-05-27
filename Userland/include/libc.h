#ifndef _LIBC_H_
#define _LIBC_H_

#include <stdint.h>
#include "libasm.h"
#include "syscallDefs.h"
#include <stdint.h>

typedef uint64_t size_t;

/* Time structure */
typedef struct {
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} Time;

static const int ZOOM_MAX = 10; // Maximum zoom level
static const int ZOOM_MIN = 1; // Minimum zoom level
/* ------------------------------------------------------------------------- */
/* Basic character I/O (built on sys_read/write)                             */
/* ------------------------------------------------------------------------- */
void     putChar(char c);
char     getChar(void);
void     putString(const char *s);
// int      getLine(char *buf, size_t maxlen);   /* read up to maxlen–1 chars + '\0' */ 

/* ------------------------------------------------------------------------- */
/* String and memory routines                                                */
/* ------------------------------------------------------------------------- */
size_t   strlen(const char *s);
char    *strcpy(char *dest, const char *src);
char    *strncpy(char *dest, const char *src, size_t n);
int      strcmp(const char *s1, const char *s2);
int      strcasecmp(const char *s1, const char *s2);
int      isalpha(int c);
int      toupper(int c);
int      tolower(int c);

void    *memset(void *s, int c, size_t n);
void    *memcpy(void *dest, const void *src, size_t n);

/* ------------------------------------------------------------------------- */
/* Numeric conversions                                                       */
/* ------------------------------------------------------------------------- */
int      itoa(int64_t value, char *buf, int base);
int      atoi(const char *s);

/* ------------------------------------------------------------------------- */
/* Timing and delays                                                         */
/* ------------------------------------------------------------------------- */
Time*     getTime(void);            /* fetch system time */ 
uint64_t getTicks(void);           /* ticks since boot */ 
void     sleep(uint64_t seconds);

/* ------------------------------------------------------------------------- */
/* Cursor control & screen clearing                                          */
/* ------------------------------------------------------------------------- */
void     set_cursor(uint32_t x, uint32_t y);
void     clear_screen(void);

/* ------------------------------------------------------------------------- */
/* Exception / halt helper                                                    */
/* ------------------------------------------------------------------------- */
void     hltUntilQ(void);

/* ------------------------------------------------------------------------- */
/* CPU register inspection                                                   */
/* ------------------------------------------------------------------------- */
uint64_t *getRegisters(void);
void      showRegisters(void);    /* print regs to stdout */ 


#endif /* _LIBC_H_ */
