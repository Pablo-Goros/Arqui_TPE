
#include <videoDriver.h>

struct vbe_mode_info_structure {
    uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t window_a;			// deprecated
    uint8_t window_b;			// deprecated
    uint16_t granularity;		// deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;			// number of bytes per horizontal line
    uint16_t width;			// width in pixels
    uint16_t height;		// height in pixels
    uint8_t w_char;			// unused...
    uint8_t y_char;			// ...
    uint8_t planes;
    uint8_t bpp;			// bits per pixel in this mode
    uint8_t banks;			// deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

static int cursorX, cursorY;     // (Pixels)
static uint32_t text_color = COLOR_WHITE;
static uint32_t background_color = COLOR_BLACK;
static int zoom = 1;   

static void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF;
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

static void putMultPixel(uint32_t hexColor, uint64_t x, uint64_t y, int mult) {
    for (int i = 0; i < mult; i++) {
        for (int j = 0; j < mult; j++) {
            putPixel(hexColor, x+i, y+j);
        }
    }
}

void vd_put_char(unsigned char c, FileDescriptor fd) {
    text_color = (fd == STDOUT) ? COLOR_WHITE : COLOR_RED;

    if (c == '\n') { // new line
        cursorX = 0;
        cursorY += CHAR_HEIGHT * zoom;

    } else if (c == '\b') { // delete 
        if (cursorX >= CHAR_WIDTH * zoom) {
            cursorX -= CHAR_WIDTH * zoom;
            drawChar(' ');
        } else if (cursorX == 0 && cursorY >= CHAR_HEIGHT * zoom) {
            cursorY -= CHAR_HEIGHT * zoom;
            cursorX = (VBE_mode_info->width / (CHAR_WIDTH * zoom)) * CHAR_WIDTH * zoom; // move to the end of the previous line
            drawChar(' ');
        } 

    } else {
        drawChar(c);
        cursorX += CHAR_WIDTH * zoom;
    }

    // check if cursorX exceeds screen width
    if (cursorX + CHAR_WIDTH * zoom > VBE_mode_info->width) {
        cursorX = 0;
        cursorY += CHAR_HEIGHT * zoom;
    }

    // check if cursorY exceeds screen height
    if (cursorY + CHAR_HEIGHT * zoom > VBE_mode_info->height) {
        scroll_screen();
        cursorY -= CHAR_HEIGHT * zoom;
    }
}

static void draw_char(char c) {
    int mask[8]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}; //mascara para cada bit
    unsigned char * glyph=font_bitmap+(int)c*16; // se posiciona en la letra correspondiente

    for (int cy = 0; cy < CHAR_HEIGHT*zoom; cy+=zoom) {
        for (int cx = 0; cx < CHAR_WIDTH*zoom; cx+=zoom) {
            putMultPixel(glyph[cy/zoom] & mask[cx/zoom] ? text_color : background_color, (cursorX + cx), (cursorY + cy), zoom);
        }
    }
}

void vd_init(void) {
    cursorX = cursorY = 0;
    vd_clear_screen();
}

void vd_set_cursor(int col, int row) {
    cursorX = col * CHAR_WIDTH * zoom;
    cursorY = row * CHAR_HEIGHT * zoom;
}