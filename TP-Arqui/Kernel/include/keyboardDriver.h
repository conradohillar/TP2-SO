#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include <libasm.h>
#include <registers.h>
#include <stdint.h>

#define KILL_PROCESS_SHORTCUT 'c'
#define EOF_SHORTCUT 'd'

uint32_t get_key();

uint8_t get_ascii();

void keyboard_handler();

#endif