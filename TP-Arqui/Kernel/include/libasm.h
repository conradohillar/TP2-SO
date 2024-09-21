#ifndef _LIBASM_H
#define _LIBASM_H

#include <stdint.h>


char * cpu_vendor(char *result);

uint32_t read_keyboard();

uint8_t get_seconds();

uint8_t get_minutes();

uint8_t get_hours();

void get_registers(uint64_t * registers, uint8_t jump);

void beep_asm(uint64_t millis, uint32_t freq);



#endif