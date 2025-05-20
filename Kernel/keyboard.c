/* /*
#include <keyboard.h>
#include "video.h"
#include <lib.h>

extern uint8_t getKeyPressed(void);

/* scancode de Shift 
#define SC_LSHIFT_MAKE   0x2A
#define SC_LSHIFT_BREAK  0xAA
#define SC_RSHIFT_MAKE   0x36
#define SC_RSHIFT_BREAK  0xB6
#define SC_EXTENDED      0xE0

/* Tabla scancode→ASCII: [no-shift, shift] 
static const char sc_to_ascii[58][2] = {
    {0,0}, {27,27}, {'1','!'}, {'2','@'}, {'3','#'}, {'4','$'},
    {'5','%'}, {'6','^'}, {'7','&'}, {'8','*'}, {'9','('}, {'0',')'},
    {'-','_'}, {'=','+'}, {8,8}, {9,9},
    {'q','Q'},{'w','W'},{'e','E'},{'r','R'},{'t','T'},{'y','Y'},
    {'u','U'},{'i','I'},{'o','O'},{'p','P'},{'[','{'}, {']','}'},
    {13,13}, {0,0},
    {'a','A'},{'s','S'},{'d','D'},{'f','F'},{'g','G'},{'h','H'},
    {'j','J'},{'k','K'},{'l','L'},{';',':'},{'\'','"'},{'`','~'},
    {0,0},
    {'\\','|'},{'z','Z'},{'x','X'},{'c','C'},{'v','V'},{'b','B'},
    {'n','N'},{'m','M'},{',','<'},{'.','>'},{'/','?'},{0,0},{0,0},
    {0,0},{' ',' '}
};

/**
 * Lee un scancode y devuelve:
 *  - SC_LSHIFT_MAKE / SC_LSHIFT_BREAK para Shift
 *  - 0 para break de otras teclas o códigos fuera de rango
 *  - el ASCII traducido (según shift) para un make válido
 
char getPressed(char shift) {
    uint8_t sc = getKeyPressed();

    // 1) Shift make/break de ambos lados
    if (sc == SC_LSHIFT_MAKE || sc == SC_RSHIFT_MAKE) 
        return SC_LSHIFT_MAKE;     // unificamos ambos como “make”
    if (sc == SC_LSHIFT_BREAK || sc == SC_RSHIFT_BREAK) 
        return SC_LSHIFT_BREAK;    // idem “break”

    // 2) Ignorar prefijo de scancode extendido
    if (sc == SC_EXTENDED)  
        return 0;

    // Scancode fuera de la tabla
    if (sc >= 58)  
        return 0;

    // Traducir a ASCII usando shift
    return sc_to_ascii[sc][ shift ? 1 : 0 ];
}

void pressKey() {
    char shiftPressed = 0;
    while (1) {
        char c = getPressed(shiftPressed);
        if (!c) 
            continue;     // nada que procesar

        if (c == 27)      // ESC → salir
            break;

        // Actualizar estado de Shift
        if (c == SC_LSHIFT_MAKE) {
            shiftPressed = 1;
            continue;
        }
        if (c == SC_LSHIFT_BREAK) {
            shiftPressed = 0;
            continue;
        }

        // Ya es un carácter imprimible o especial
        switch (c) {
            case 8:   vga_delete();   break;   // Backspace
            case 13:  vga_new_line();  break;   // Enter
            default:  vga_putc(c);         // Letra, número, símbolo
        }
    }
}
*/