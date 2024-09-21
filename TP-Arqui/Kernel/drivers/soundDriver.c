#include <soundDriver.h>

void beep(uint64_t millis, uint32_t freq){
    beep_asm(millis,freq);
}