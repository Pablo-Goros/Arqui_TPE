#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <stdint.h>

extern unsigned int get_key_asm(void);

void kbd_get_key(void);

/** Non-blocking: true if a character is waiting */
uint8_t kbd_has_char(void);

/** Pop one ASCII char, or 0 if none */
char kbd_get_char(void);

void kbd_reset_buff();

uint64_t kbd_is_key_down(char key);

#endif // KEYBOARD_DRIVER_H