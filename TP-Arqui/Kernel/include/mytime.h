#ifndef _MYTIME_H_
#define _MYTIME_H_

#include <processManager.h>
#include <stdint.h>

uint64_t timer_handler(schedulerADT scheduler, uint64_t stack_pointer);

uint64_t ticks_elapsed();

uint64_t seconds_elapsed();

uint64_t millis_elapsed();
void sleep(uint64_t secs);

void nanosleep(uint64_t millis);

#endif
