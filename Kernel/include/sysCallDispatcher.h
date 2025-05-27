#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h> 
#include <stdarg.h>
#include "time_display.h"
#include "time.h"
#include "keyboardDriver.h"
#include "interrupts.h"

typedef unsigned long size_t;

typedef enum {
    STDIN = 0,
    STDOUT,
    STDERR,
} FileDescriptor;

#define SYS_READ            0
#define SYS_WRITE           1
#define SYS_REGISTERS       2
#define SYS_GET_TIME        4
#define SYS_SET_CURSOR      5
#define SYS_CLEAR_SCREEN    6
#define SYS_SET_ZOOM        7

#define SYS_EXIT            8
#define SYS_GET_TICKS       14
#define SYS_SLEEP           35
#define SYS_ERR   ((uint64_t)-1)

uint64_t sysCallDispatcher(uint64_t rax, ...);

void sys_write(FileDescriptor fd, const char *buf, size_t count);

uint64_t sys_read(FileDescriptor fd, char *buf, size_t count);

void     sys_clearScreen(void);

void     sys_setCursor(int col, int row);


#endif