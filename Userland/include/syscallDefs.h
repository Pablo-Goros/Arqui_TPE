#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

// standard file descriptors
typedef enum {
    STDIN  = 0,
    STDOUT = 1,
    STDERR = 2
} FDS;

// syscall numbers 
enum {
    SYS_READ                       = 0,
    SYS_WRITE                      = 1,
    SYS_REGISTERS                  = 2,
    SYS_GET_TIME                   = 4,
    SYS_SET_CURSOR                 = 5,
    SYS_CLEAR_SCREEN               = 6,
    SYS_GET_TICKS                  = 14,
    SYS_SLEEP                      = 35
};

#endif // _SYSCALLS_H_
