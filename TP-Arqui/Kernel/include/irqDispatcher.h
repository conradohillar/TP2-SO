#ifndef _IRQDISPATHCER_H
#define _IRQDISPATHCER_H

#include <keyboardDriver.h>
#include <mytime.h>
#include <stdint.h>
#include <videoDriver.h>

uint64_t int_20(uint64_t stack_pointer);
uint64_t int_21();

uint64_t irq_dispatcher(uint64_t irq, uint64_t stack_pointer);

#endif