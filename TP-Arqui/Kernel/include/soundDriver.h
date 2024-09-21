#ifndef SOUNDDRIVER_H_
#define SOUNDDRIVER_H_

#include <stdint.h>
#include <time.h>
#include <libasm.h>

#define DO  262  
#define RE  294  
#define MI  330  
#define FA  349  
#define SOL 392 
#define LA  440  
#define SI  494  

void beep(uint64_t millis, uint32_t freq);

#endif