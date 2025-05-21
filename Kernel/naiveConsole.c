#include <naiveConsole.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

void ncScroll();

static char buffer[64] = {'0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;

static const uint32_t width = 80;
static const uint32_t height = 25 ;
static uint8_t textColor = 0x07;

void ncInit() {
    ncClear();
    textColor = 0x07; // Reinicia el color por defecto
}

void ncPrint(const char * string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintChar(char character) {
    if (character == '\n') {
        ncNewline();
        return;
    }
    *currentVideo = character;
    *(currentVideo + 1) = textColor;
    currentVideo += 2;
}

void ncNewline() {
    uint64_t offset = (uint64_t)(currentVideo - video);
    uint64_t lineStart = offset - (offset % (width * 2));
    currentVideo = (uint8_t *)(lineStart + width * 2);

    // Verifica si el cursor está más allá de la última línea
    if ((uint64_t)(currentVideo - video) >= width * height * 2) {
        ncScroll();
        currentVideo = video + (height - 1) * width * 2; // Ajusta el cursor a la última línea
    }
}

void ncScroll() {
// Mover cada línea una posición hacia arriba
    for (int i = 0; i < (height - 1) * width * 2; i++) {
        video[i] = video[i + width * 2];
    }
    // Limpiar la última línea
    for (int i = (height - 1) * width * 2; i < height * width * 2; i += 2) {
        video[i] = ' ';
        video[i + 1] = textColor;
    }
}


void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear() {
    for (int i = 0; i < height * width; i++) {
        video[i * 2] = ' '; // Espacio en blanco
        video[i * 2 + 1] = textColor; // Color del texto
    }
    currentVideo = video; // Reinicia el cursor al inicio
}

void ncBackspace() {
    // Verifica si estamos al principio del búfer de video
    if (currentVideo > video) {
        currentVideo -= 2;
        *currentVideo = 0;
        *(currentVideo + 1) = textColor;
    }

}
void ncSetTextColor(uint8_t color) {
    textColor = (textColor & 0xF0) | (color & 0x0F);
}
void ncSetBackgroundColor(uint8_t color) {
    textColor = (textColor & 0x0F) | ((color & 0x0F) << 4);
}
void ncSetCursorPosition(uint32_t x, uint32_t y) {
    if (x < width && y < height) {
        currentVideo = video + (y * width + x) * 2;
    }
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
