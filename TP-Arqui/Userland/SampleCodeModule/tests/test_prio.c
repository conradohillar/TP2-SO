// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/shell.h"
#include "../include/syscaller.h"
#include "../include/tests.h"
#include "syscall.h"
#include <stdint.h>

#define MINOR_WAIT_S (uint8_t *)"1000000"
#define MINOR_WAIT 1000000
// TODO: Change this value to prevent a process from flooding the
// screen
#define WAIT_S (uint8_t *)"10000000"
#define WAIT 10000000
// TODO: Change this value to make the wait long enough to see
// theese processes beeing run at least twice

#define TOTAL_PROCESSES 5

#define LOWEST 1
#define LOW 2
#define MEDIUM 3
#define HIGH 4
#define HIGHEST 5

int64_t prio[TOTAL_PROCESSES] = {LOWEST, LOW, MEDIUM, HIGH, HIGHEST};

int64_t test_prio(uint64_t argc, uint8_t *argv[]) {
  print((uint8_t *)"EXECUTING PRIORITY TEST\n");
  int64_t pids[TOTAL_PROCESSES];
  uint8_t *args[] = {WAIT_S};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++) {
    pids[i] = sys_create_process_asm(endless_loop_print, 1, args,
                                     (uint8_t *)"endless_loop_print", 0);
    if (pids[i] == -1) {
      print((uint8_t *)"ERROR CREATING PROCESS\n");
      return -1;
    }
    print((uint8_t *)"CREATED PROCESS pid: ");
    uint8_t num[20] = {0};
    itoa(pids[i], num);
    print(num);
    print((uint8_t *)"\n");
  }

  // ps();

  bussy_wait(WAIT);
  print((uint8_t *)"\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_set_priority_asm(pids[i], prio[i]);

  // ps();

  bussy_wait(WAIT);

  print((uint8_t *)"\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_block_asm(pids[i]);

  print((uint8_t *)"\nCHANGING PRIORITIES WHILE BLOCKED...\n");

  ps();

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_set_priority_asm(pids[i], MEDIUM);

  print((uint8_t *)"UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_unblock_asm(pids[i]);

  bussy_wait(WAIT);

  print((uint8_t *)"\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    sys_kill_asm(pids[i]);

  print((uint8_t *)"TEST SUCCESSFUL!\n");
  return 0;
}
