/* game.h */

#ifndef PONGIS_H
#define PONGIS_H

#include <stdint.h>
#include "libc.h"

/*————————————————————————————————————————————————————————————————————
  Mode info (call once at startup to size buffers, etc.)
  ------------------------------------------------------------------------------*/
typedef struct ModeInfo {
    uint32_t width;
    uint32_t height;
    uint32_t bpp;
} ModeInfo;

/*————————————————————————————————————————————————————————————————————
  New syscalls (implement in your syscall dispatcher)
  ------------------------------------------------------------------------------*/
/* fill out current video mode info */
int    sys_get_mode_info(ModeInfo *info);
/* block until next timer IRQ (60 fps target) */
void   sys_wait_next_tick(void);
/* copy `size` bytes from user‐buffer into VRAM */
int    sys_blit(const void *user_buffer, uint64_t size);
/* non‐blocking key check */
int    sys_key_ready(void);   /* returns 0 = no key, 1 = key waiting */
/* read one ASCII key (blocking only if you call without ready-check) */
char   sys_read_key(void);
/* draw pixel or simple primitives in VRAM via syscall */
int    sys_put_pixel(int x, int y, uint32_t color);
int    sys_draw_rect(int x, int y, int w, int h, uint32_t color);
int    sys_draw_circle(int cx, int cy, int r, uint32_t color);
int    sys_draw_bitmap(int x, int y, int w, int h, const uint32_t *pixels);
#endif /* GAME_H */
