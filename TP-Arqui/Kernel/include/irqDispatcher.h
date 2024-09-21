#ifndef _IRQDISPATHCER_H
#define _IRQDISPATHCER_H

#include <mytime.h>
#include <keyboardDriver.h>
#include <stdint.h>
#include <videoDriver.h>

void int_20();
void int_21();


void irq_dispatcher(uint64_t irq);

#endif