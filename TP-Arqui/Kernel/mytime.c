// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <mytime.h>
#include <pipes.h>

extern pipeManagerADT my_pipe_manager;

static uint64_t ticks = 0;

uint64_t timer_handler(schedulerADT scheduler, uint64_t stack_pointer) {
  ticks++;

  // pipe_t *stdout_pipe = get_pipe(my_pipe_manager, STDOUT);

  // uint16_t to_read_count = stdout_pipe->to_read_count;

  // // Empty STDOUT and print to screen
  // if (to_read_count > 0) {
  //   uint8_t buffer[MAX_CHARS_IN_SCREEN + 1];
  //   uint64_t count =
  //       read_pipe(my_pipe_manager, stdout_pipe, buffer, to_read_count);
  //   buffer[count] = '\0';
  //   put_string_nt(buffer, WHITE, BLACK);
  // }

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
