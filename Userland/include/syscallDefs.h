#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

// standard file descriptors
typedef enum {
    STDIN  = 0,
    STDOUT = 1,
    STDERR = 2
} FDS;

#define SYS_READ            0
#define SYS_WRITE           1
#define SYS_REGISTERS       2
#define SYS_GET_TIME        4
#define SYS_SET_CURSOR      5
#define SYS_CLEAR_SCREEN    6
#define SYS_GET_TICKS       14
#define SYS_SLEEP           35

#endif // _SYSCALLS_H_
