#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include <stdint.h>
#include <registers.h>
#include <libasm.h>

#define SAVE_REGS_SHORTCUT 'r'


uint32_t get_key();

uint8_t get_ascii();

void keyboard_handler();

#endif