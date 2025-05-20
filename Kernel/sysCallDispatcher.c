#include "sysCallDispatcher.h"
#include "time.h"
#include <stdint.h>
#include <stdarg.h>


#define SYSCALL_READ                        0
#define SYSCALL_WRITE                       1
#define SYSCALL_REGISTERS                   2
#define SYSCALL_GET_TIME                    4
#define SYSCALL_SET_CURSOR                  5
#define SYSCALL_SET_FONT_COLOR              7
#define SYSCALL_SET_ZOOM                    8
#define SYSCALL_DRAW_RECTANGLE              9
#define SYSCALL_SET_BACKGROUND_FONT_COLOR  10
#define SYSCALL_DRAW_SPRAY                 11
#define SYSCALL_PLAY_SOUND                 12
#define SYSCALL_STOP_SOUND                 13
#define SYSCALL_GET_TICKS                  14
#define SYSCALL_SLEEP                      35

uint64_t sysCallDispatcher(uint64_t rax, ...) {
    va_list args;
    va_start(args, rax);

    uint64_t ret = 0;

    switch (rax) {
        case SYSCALL_READ: {
            uint64_t fd    = va_arg(args, uint64_t);
            uint64_t buf   = va_arg(args, uint64_t);
            uint64_t count = va_arg(args, uint64_t);
            ret = sys_read(fd, buf, count);
            break;
        }

        case SYSCALL_WRITE: {
            FDS fd = va_arg(args, FDS);
            const char *buf   = va_arg(args, const char *);
            uint64_t     count = va_arg(args, uint64_t);
            sys_write(fd, buf, count);
            ret = 0;
            break;
        }
/*
        case SYSCALL_REGISTERS:
            ret = getRegisters();
            break;

        case SYSCALL_GET_TIME:
            ret = (uint64_t)getTime();
            break;

        case SYSCALL_SET_CURSOR: {
            int x = (int)va_arg(args, uint64_t);
            int y = (int)va_arg(args, uint64_t);
            sys_setCursor(x, y);
            ret = 0;
            break;
        }

        case SYSCALL_SET_FONT_COLOR: {
            uint32_t hexColor = va_arg(args, uint32_t);
            setFontColor(hexColor);
            ret = 0;
            break;
        }

        case SYSCALL_SET_ZOOM: {
            uint64_t new_zoom = va_arg(args, uint64_t);
            sys_setZoom(new_zoom);
            ret = 0;
            break;
        }

        case SYSCALL_DRAW_RECTANGLE: {
            Point   *p1 = va_arg(args, Point *);
            Point   *p2 = va_arg(args, Point *);
            uint32_t  c  = va_arg(args, uint32_t);
            drawRectangle(p1, p2, c);
            ret = 0;
            break;
        }

        case SYSCALL_SET_BACKGROUND_FONT_COLOR: {
            uint32_t hexColor = va_arg(args, uint32_t);
            changeBackgroundColor(hexColor);
            ret = 0;
            break;
        }

        case SYSCALL_DRAW_SPRAY: {
            uint64_t spray  = va_arg(args, uint64_t);
            uint64_t size_x = va_arg(args, uint64_t);
            uint64_t size_y = va_arg(args, uint64_t);
            drawSpray(size_x, size_y, spray, cursorX, cursorY);
            ret = 0;
            break;
        }

        case SYSCALL_PLAY_SOUND: {
            uint64_t frequency = va_arg(args, uint64_t);
            playSound(frequency);
            ret = 0;
            break;
        }

        case SYSCALL_STOP_SOUND:
            stopSound();
            ret = 0;
            break;

        case SYSCALL_GET_TICKS:
            ret = getTicks();
            break;

        case SYSCALL_SLEEP: {
            int seconds = (int)va_arg(args, uint64_t);
            sys_sleep(seconds);
            ret = 0;
            break;
        }
*/
        default:
            // syscall no reconocido
            ret = (uint64_t)-1;
            break;
    }

    va_end(args);
    return ret;
}
