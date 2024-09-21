#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <registers.h>
#include <syscalls.h>
#include <textMode.h>
#include <interrupts.h>
#include <colors.h>


#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6


void exception_dispatcher(int exception);

void invalid_opcode();

void zero_division();

#endif