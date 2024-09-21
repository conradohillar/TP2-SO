#ifndef _RTCDRIVER_H_
#define _RTCDRIVER_H_

#include <textMode.h>
#include <libasm.h>

void display_rtc();

void time_to_string(uint8_t * buffer);

void num_to_time(uint8_t num, uint8_t vec[], uint32_t pos, uint8_t c);

uint8_t bcd_sub_hours24(uint8_t bcdHours, uint8_t hoursToSub);


#endif

