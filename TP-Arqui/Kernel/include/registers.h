#ifndef _REGISTERS_H_
#define _REGISTERS_H_

#include <textMode.h>
#include <libasm.h>
#include <colors.h>

void track_registers();

void save_registers();

void peek_registers();

void print_hexa(uint64_t num);

uint32_t uint_to_base(uint64_t value, uint8_t * buffer, uint32_t base);

#endif