#ifndef _LIBC_H_
#define _LIBC_H_

#include <stdint.h>
#include "libasm.h"
#include "syscallDefs.h"
#include <stdint.h>


/* Time structure */
typedef struct {
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} Time;

typedef struct ModeInfo {
    uint32_t width;
    uint32_t height;
    uint32_t bpp;
} ModeInfo;


// #define NUMBER_OF_REGISTERS 18 // Number of CPU registers we want to inspect

/* ------------------------------------------------------------------------- */
/* Basic character I/O (built on sys_read/write)                             */
/* ------------------------------------------------------------------------- */
void     putChar(char c);
char     getChar(void);
void     putString(const char *s);

/* ------------------------------------------------------------------------- */
/* String and memory routines                                                */
/* ------------------------------------------------------------------------- */
uint64_t   strlen(const char *s);
char    *strcpy(char *dest, const char *src);
char    *strncpy(char *dest, const char *src, uint64_t n);
int      strcmp(const char *s1, const char *s2);
int      strncmp(const char *s1, const char *s2, uint64_t n);
int      strcasecmp(const char *s1, const char *s2);
int      isalpha(int c);
int      toupper(int c);
int      tolower(int c);

void    *memset(void *s, int c, uint64_t n);
void    *memcpy(void *dest, const void *src, uint64_t n);

/* ------------------------------------------------------------------------- */
/* Numeric conversions                                                       */
/* ------------------------------------------------------------------------- */
int      itoa(int64_t value, char *buf, int base);
int      atoi(const char *s);

/* ------------------------------------------------------------------------- */
/* Timing and delays                                                         */
/* ------------------------------------------------------------------------- */
void     getTime(void);            /* fetch system time */ 
uint64_t getTicks(void);           /* ticks since boot */ 
void     sleep(uint64_t seconds);

/* ------------------------------------------------------------------------- */
/* Cursor control & screen clearing                                          */
/* ------------------------------------------------------------------------- */
void     set_cursor(uint32_t x, uint32_t y);
void     clear_screen(void);
void     set_zoom(int level);

/* ------------------------------------------------------------------------- */
/* Exception / halt helper                                                    */
/* ------------------------------------------------------------------------- */
void hltUntil_c();

/* ------------------------------------------------------------------------- */
/* CPU register inspection                                                   */
/* ------------------------------------------------------------------------- */
uint64_t *getRegisters(void);
void      showRegisters(void);    /* print regs to stdout */ 

/* ------------------------------------------------------------------------- */
/* Game (Pongis)                                                             */
/* ------------------------------------------------------------------------- */
uint64_t get_mode_info(ModeInfo *mode);

uint8_t is_key_ready(void);   /* returns 0 = no key, 1 = key waiting */

void draw_bitmap(int x, int y, int w, int h, const uint32_t *pixels);

void wait_next_tick(void); /* block until next timer IRQ (60 fps target) */

int  blit(const void *user_buffer, uint64_t size); /* copy `size` bytes from user‐buffer into VRAM */

#endif /* _LIBC_H_ */
