#include "keyboardDriver.h"
#include "interrupts.h"        


#define BUF_SIZE      128

static char           buf[BUF_SIZE];
static volatile int   head = 0;
static volatile int   tail = 0;
static uint8_t        shift = 0;
static uint8_t        caps = 0;



/* Partial scancode→ASCII tables (fill out the rest as needed) */
static const char tbl_no_shift[128] = {
    [0x01] = 27,    // ESC
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
    [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8',
    [0x0A] = '9', [0x0B] = '0',
    [0x0C] = '-', [0x0D] = '=',
    [0x0E] = '\b', // Backspace
    [0x0F] = '\t', // Tab
    [0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r',
    [0x14] = 't', [0x15] = 'y', [0x16] = 'u', [0x17] = 'i',
    [0x18] = 'o', [0x19] = 'p',
    [0x1A] = '[', [0x1B] = ']',
    [0x1C] = '\n',// Enter
    /* 0x1D = Left Ctrl */
    [0x1E] = 'a', [0x1F] = 's', [0x20] = 'd', [0x21] = 'f',
    [0x22] = 'g', [0x23] = 'h', [0x24] = 'j', [0x25] = 'k',
    [0x26] = 'l',
    [0x27] = ';', [0x28] = '\'', [0x29] = '`',
    /* 0x2A = Left Shift */
    [0x2B] = '\\',
    [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v',
    [0x30] = 'b', [0x31] = 'n', [0x32] = 'm',
    [0x33] = ',', [0x34] = '.', [0x35] = '/',
    /* 0x36 = Right Shift */
    [0x37] = '*',
    /* 0x38 = Left Alt */
    [0x39] = ' ', // Space
    /* 0x3A = Caps Lock, function keys, etc. left as 0 */
};

static const char tbl_shift[128] = {
    [0x01] = 27,    // ESC
    [0x02] = '!', [0x03] = '@', [0x04] = '#', [0x05] = '$',
    [0x06] = '%', [0x07] = '^', [0x08] = '&', [0x09] = '*',
    [0x0A] = '(', [0x0B] = ')',
    [0x0C] = '_', [0x0D] = '+',
    [0x0E] = '\b', // Backspace
    [0x0F] = '\t', // Tab
    [0x10] = 'Q', [0x11] = 'W', [0x12] = 'E', [0x13] = 'R',
    [0x14] = 'T', [0x15] = 'Y', [0x16] = 'U', [0x17] = 'I',
    [0x18] = 'O', [0x19] = 'P',
    [0x1A] = '{', [0x1B] = '}',
    [0x1C] = '\n',// Enter
    /* 0x1D = Left Ctrl */
    [0x1E] = 'A', [0x1F] = 'S', [0x20] = 'D', [0x21] = 'F',
    [0x22] = 'G', [0x23] = 'H', [0x24] = 'J', [0x25] = 'K',
    [0x26] = 'L',
    [0x27] = ':', [0x28] = '"', [0x29] = '~',
    /* 0x2A = Left Shift */
    [0x2B] = '|',
    [0x2C] = 'Z', [0x2D] = 'X', [0x2E] = 'C', [0x2F] = 'V',
    [0x30] = 'B', [0x31] = 'N', [0x32] = 'M',
    [0x33] = '<', [0x34] = '>', [0x35] = '?',
    /* 0x36 = Right Shift */
    [0x37] = '*',
    /* 0x38 = Left Alt */
    [0x39] = ' ', // Space
    /* others left as 0 */
};

void kbd_get_key(void) {
    
    uint8_t sc = get_key_asm();

    uint8_t make_flag = !(sc & 0x80);
    uint8_t code = sc & 0x7F;

    // Shift keys 
    if (code == 0x2A || code == 0x36) {
        shift = make_flag;
    }
    // Caps Lock 
    else if (code == 0x3A && make_flag) {
        caps = !caps;
    }
    // Key press → convert & buffer 
    else if (make_flag) {
        char ch = shift ? tbl_shift[code] : tbl_no_shift[code];
        // apply CapsLock for letters 
        if (caps && ch >= 'a' && ch <= 'z') {
            ch = ch - 'a' + 'A';
        }
        if (ch) {
            buf[head] = ch;
            head = (head + 1) % BUF_SIZE;
            if (head == tail)   // overrun → drop oldest
                tail = (tail + 1) % BUF_SIZE;
        }
    }
}


uint8_t kbd_has_char(void) {
    return head != tail;
}

char kbd_get_char(void) {
    if (head == tail) {
        return 0;
    } 
    char c = buf[tail];
    tail = (tail + 1) % BUF_SIZE;
    return c;
}