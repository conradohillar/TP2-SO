#ifndef _TEXTMODE_H
#define _TEXTMODE_H

#include <videoDriver.h>
#include <lib.h>
#include <stdint.h>
#include <videoDriver.h>
#include <colors.h>

#define STD_CHAR_AMOUNT_X 128
#define STD_CHAR_AMOUNT_Y 48

typedef struct data_char{
    uint8_t c;
    uint32_t fore_color;
    uint32_t back_color;
} data_char;

void put_char(uint8_t c, uint32_t fore_color, uint32_t back_color);

uint64_t put_string(uint8_t * string, uint64_t size, uint32_t fore_color, uint32_t back_color);

void put_string_nt(uint8_t * string, uint32_t fore_color, uint32_t back_color);

void new_line();

void print_state();

void set_scale_text_mode(uint64_t new_scale);

void clear_screen();

void clear_screen_and_buffer();

#endif