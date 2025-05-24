//
// Created by UD on 24/5/2025.
//

#ifndef ARQUI_TPE_VIDEODRIVER_H
#define ARQUI_TPE_VIDEODRIVER_H

#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H
#include <stdint.h>
// Function declarations and macros for the video driver

#define DIM_X 1024
#define DIM_Y 768

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

typedef struct {
    int x;
    int y;
}Point;
/**
 * @brief Draws a pixel on the screen at the specified coordinates with the given color.
 *
 * @param hexColor The color of the pixel in hexadecimal format.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 */
/**
 * @brief Puts a pixel on the screen at the specified coordinates with the given color.
 *
 * @param hexColor The color of the pixel in hexadecimal format.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 */
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

/**
 * @brief Pinta un una cantidad mult de pixeles en la pantalla, sirve para escalar un pixel a un cuadrado mult x mult
 * @param hexColor Color en formato hexadecimal
 * @param x Coordenada X
 * @param y Coordenada Y
 * @param mult Multiplicador de tamaño
 */
static void putMultPixel(uint32_t hexColor, uint64_t x, uint64_t y, int mult);


/**
 * @brief Draws a character on the screen with specified foreground and background colors.
 *
 * @param c The character to be drawn.
 * @param x The x-coordinate of the character's position.
 * @param y The y-coordinate of the character's position.
 * @param fgcolor The foreground color of the character in hexadecimal format.
 * @param bgcolor The background color of the character in hexadecimal format.
 * @param mult The size multiplier for the character.
 */
void drawChar(unsigned char c, int x, int y, int fgcolor, int bgcolor, int mult);

/**
 * @brief Puts a character on the screen at the specified coordinates.
 *
 * @param c The character to be put on the screen.
 * @param x The x-coordinate of the character's position.
 * @param y The y-coordinate of the character's position.
 */
void putchar(char c, int x, int y);


#endif // VIDEODRIVER_H

#endif //ARQUI_TPE_VIDEODRIVER_H
