#include "../include/syscalls.h"
#include "../include/textMode.h"
#include "../processes/process.h"
#include "../processes/processManager.h"
#include "../processes/scheduler.h"
#include "syscall.h"
#include "test_util.h"

#define MINOR_WAIT                                                             \
  1000000 // TODO: Change this value to prevent a process from flooding the
          // screen
#define WAIT                                                                   \
  10000000 // TODO: Change this value to make the wait long enough to see theese
           // processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 1  // TODO: Change as required
#define MEDIUM 2  // TODO: Change as required
#define HIGHEST 3 // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};
void holaquetal(uint64_t argc, uint8_t **argv);

void test_prio(uint64_t argc, uint8_t *argv[], processManagerADT pm,
               schedulerADT scheduler) {
  int64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = create_process(scheduler, pm, &holaquetal, argv, 10,
                             "endless_loop_print", 1);

  bussy_wait(WAIT);
  // printf("\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    set_priority(pm, pids[i], prio[i]);

  bussy_wait(WAIT);
  // printf("\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    block(pm, scheduler, pids[i]);

  // printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    set_priority(pm, pids[i], MEDIUM);

  // printf("UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    unblock(pm, scheduler, pids[i]);

  bussy_wait(WAIT);
  // printf("\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    kill(pm, pids[i], scheduler);
}

void holaquetal(uint64_t argc, uint8_t **argv) {
  while (1)
    put_string_nt("Holaquetal jaja\n", BLUE, BLACK);
  return 0;
}