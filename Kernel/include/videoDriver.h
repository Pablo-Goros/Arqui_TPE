#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>
#include "sysCallDispatcher.h"  // for FileDescriptor Enum
#include "libc.h"

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

#define COLOR_WHITE 0xFFFFFF
#define COLOR_BLACK 0x000000
#define COLOR_RED 0xFF0000

#define ZOOM_MAX 10  // Maximum zoom level
#define ZOOM_MIN 1   // Minimum zoom level

/** Character cell dimensions (in pixels) */ 
#define CHAR_WIDTH   8
#define CHAR_HEIGHT 16

#define NUMBER_OF_REGISTERS 18 // Number of CPU registers we want to inspect

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
 * @brief Draw a string at the current cursor, advance, wrap & scroll.
 * @args c:  String to draw
 *       fd: FileDescriptor (STDOUT or STDERR) to select text color
 */
void vd_put_string(const char *str, FileDescriptor fd);

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
/**
 * @brief Show the CPU registers in a human‐readable format.
 *        This function will print the register names and their values to the console.
 */
void vd_show_registers(FileDescriptor fd);
/**
 * @brief Set the zoom level for the text.
 *        The zoom level affects the size of characters drawn on the screen.
 * @args new_zoom: New zoom level (1 to 10)
 */
void vd_set_zoom(int new_zoom);

#endif 