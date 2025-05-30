#include "sysCallDispatcher.h"
#include "videoDriver.h"

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
            vd_show_registers(STDOUT);
            break;

        case SYS_GET_TIME: {
            char* time_str = getTime();
            vd_put_string(time_str, STDOUT);
            ret = 0;
            break;
        }
        case SYS_SET_CURSOR: {
            int x = (int)va_arg(args, uint64_t);
            int y = (int)va_arg(args, uint64_t);
            vd_set_cursor(x, y);
            ret = 0;
            break;
        }
        case SYS_CLEAR_SCREEN: {
            vd_clear_screen();
            ret = 0;
            break;
        }

        case SYS_SLEEP: {
            int seconds = (int)va_arg(args, uint64_t);
            sleep(seconds * 18); // Assuming 18 ticks per second
            ret = 0;
            break;
        }

        case SYS_SET_ZOOM: {
            int zoom_level = (int)va_arg(args, uint64_t);
            if (zoom_level >= ZOOM_MIN && zoom_level <= ZOOM_MAX) {
                vd_set_zoom(zoom_level);
                ret = 0;
            } else {
                vd_put_string("\nInvalid zoom level. Usage: zoom <number (Between 1 and 10)>", STDOUT);
                return SYS_ERR;
            }
            break;
        }

        case SYS_EXIT:
            
            outw(0x604, 0x2000);  // QEMU ACPI shutdown
        

        case SYS_PUT_PIXEL: {
            putPixel()
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
    vd_put_string(buf, fd);

    return;
}

uint64_t sys_read(FileDescriptor fd, char *buf, size_t count) {
    if (fd == STDIN) {
        int i = 0;
        while (i < count) {
            while (!kbd_has_char()) {
                _sti(); // Enable interrupts to allow keyboard input
                _hlt(); // Wait for an interrupt
            }
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



