#ifndef _VIDEODRIVER_H
#define _VIDEODRIVER_H
#include <stdint.h>

#define STD_W_CHAR 8
#define STD_Y_CHAR 16


void put_pixel(uint32_t hexColor, uint64_t x, uint64_t y);

void put_char_scale(uint8_t c, uint32_t foreColor, uint32_t backColor, uint64_t * x, uint64_t * y, uint8_t scale);

void put_string_scale(uint8_t * s, uint64_t size, uint32_t foreColor, uint32_t backColor, uint64_t * x, uint64_t * y, uint8_t scale);

void put_rectangle(uint64_t topLeftX, uint64_t topLeftY, uint64_t bottomRightX, uint64_t bottomRightY, uint32_t color);

uint8_t get_w_char();

uint8_t get_y_char();

uint16_t get_width();

uint16_t get_height();

#endif
