#include "libc.h"



void putChar(char c) {

}

void putString(const char *str) {
    while (*str) {
        putChar(*str++);
    }
}

char getChar(void) {
    
}
