#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>
#include "sysCallDispatcher.h"  // for FileDescriptor Enum

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLACK 0x000000
#define COLOR_RED 0xFF0000


/** Character cell dimensions (in pixels) */ 
#define CHAR_WIDTH   8
#define CHAR_HEIGHT 16

/**
 * @brief Initialize the graphics‐mode console.
 *        Must be called once after switching into a VBE linear‐framebuffer mode.
 */
void vd_init(void);

/**
 * @brief Draw one character at the current cursor, advance, wrap & scroll.
 * @args c:  Character to draw
 *       fd: FileDescriptor (STDOUT or STDERR) to select text color
 */
void vd_put_char(unsigned char c, FileDescriptor fd);

/**
 * @brief Clear the entire screen and reset the cursor to the top‐left.
 */
void vd_clear_screen(void);

/**
 * @brief Move the console cursor to the given character cell.
 * @args col: Column index (0..)
 *       row: Row index (0..)
 */
void vd_set_cursor(int col, int row);

#endif 