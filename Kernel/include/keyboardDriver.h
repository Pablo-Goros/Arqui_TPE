#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <stdint.h>

#define ARROW_UP    0x80
#define ARROW_DOWN  0x81
#define ARROW_LEFT  0x82
#define ARROW_RIGHT 0x83

#define BUF_SIZE        128
#define KEY_STATE_SIZE  256

extern unsigned int get_key_asm(void);

void kbd_get_key(void);

/** Non-blocking: true if a character is waiting */
uint8_t kbd_has_char(void);

/** Pop one ASCII char, or 0 if none */
char kbd_get_char(void);

void kbd_reset_buff();

uint64_t kbd_is_key_down(char key);

#endif // KEYBOARD_DRIVER_H