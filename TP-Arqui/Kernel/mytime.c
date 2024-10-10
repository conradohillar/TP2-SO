#include <mytime.h>

static uint64_t ticks = 0;

uint64_t timer_handler(schedulerADT scheduler, uint64_t stack_pointer) {
  ticks++;

  return context_switch(scheduler, stack_pointer);
}

uint64_t ticks_elapsed() { return ticks; }

uint64_t seconds_elapsed() { return ticks / 18; }

uint64_t millis_elapsed() { return (ticks * 1000) / 18; }

void sleep(uint64_t secs) {
  uint64_t start = seconds_elapsed();
  uint64_t timeElapsed = start;
  while (timeElapsed < start + secs) {
    timeElapsed = seconds_elapsed();
  }
  return;
}

void nanosleep(uint64_t millis) {
  uint64_t start = millis_elapsed();
  uint64_t timeElapsed = start;
  while (timeElapsed < start + millis) {
    timeElapsed = millis_elapsed();
  }
  return;
}
