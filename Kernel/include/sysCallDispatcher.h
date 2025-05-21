#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h> 
#include <stdlib.h>

typedef enum{
    STDIN = 0,
    STDOUT,
    STDERR,
} FileDescriptor;

uint64_t sysCallDispatcher(uint64_t rax, ...);

void sys_write(FileDescriptor fd, const char *buf, size_t count);
size_t sys_read(FileDescriptor fd, char *buf, size_t count);



#endif