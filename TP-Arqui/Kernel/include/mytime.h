#ifndef _MYTIME_H_
#define _MYTIME_H_

#include <stdint.h>


void timer_handler();

uint64_t ticks_elapsed();

uint64_t seconds_elapsed();

uint64_t millis_elapsed();
void sleep(uint64_t secs);


void nanosleep(uint64_t millis);

#endif
