#include "sysCallDispatcher.h"
#include "time.h"
#include "videoDriver.h"
#include "keyboardDriver.h"



uint64_t sysCallDispatcher(uint64_t rax, ...) {
    va_list args;
    va_start(args, rax);

    uint64_t ret = 0;

    switch (rax) {
        case SYS_READ: {
            uint64_t fd    = va_arg(args, uint64_t);
            char * buf   = va_arg(args,  char *);
            uint64_t count = va_arg(args, uint64_t);
            ret = sys_read(fd, buf, count);
            break;
        }
        case SYS_WRITE: {
            FileDescriptor fd = va_arg(args, FileDescriptor);
            const char *buf   = va_arg(args, const char *);
            uint64_t     count = va_arg(args, uint64_t);
            sys_write(fd, buf, count);
            ret = 0;
            break;
        }
        case SYS_REGISTERS:
            //ret = getRegisters();
            break;

        case SYS_GET_TIME:
            //ret = (uint64_t)getTime();
            break;

        case SYS_SET_CURSOR: {
            int x = (int)va_arg(args, uint64_t);
            int y = (int)va_arg(args, uint64_t);
            vd_set_cursor(x, y);
            ret = 0;
            break;
        }
        case SYS_CLEAR_SCREEN:
            vd_clear_screen();
            ret = 0;
            break;
        /*
        case SYS_SET_FONT_COLOR: {
            uint32_t hexColor = va_arg(args, uint32_t);
            setFontColor(hexColor);
            ret = 0;
            break;
        } */

        case SYS_GET_TICKS:
            ret = getTicks();
            break;

        case SYS_SLEEP: {
            int seconds = (int)va_arg(args, uint64_t);
            sleep(seconds * 18); // Assuming 18 ticks per second
            ret = 0;
            break;
        }

        default:
            // syscall not recognized
            ret = SYS_ERR;
            break;
    }

    va_end(args);
    return ret;
}

void sys_write(FileDescriptor fd, const char *buf, size_t count){  
    if (fd != STDOUT && fd != STDERR) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        vd_put_char(buf[i], fd);
    }      
    return;
}

uint64_t sys_read(FileDescriptor fd, char *buf, size_t count) {
    if (fd == STDIN) {
        int i = 0;
        while (i < count && kbd_has_char()) {
            char c = kbd_get_char();
            buf[i++] = c;
            if (buf[i - 1] == '\n') {
                break;
            }
        }
        return i;
    }   

    return 0;
}


