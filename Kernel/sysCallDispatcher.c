#include "sysCallDispatcher.h"
#include "time.h"


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
