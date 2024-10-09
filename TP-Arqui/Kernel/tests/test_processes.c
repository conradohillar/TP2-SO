#include "../include/syscalls.h"
#include "../include/textMode.h"
#include "../processes/process.h"
#include "../processes/processManager.h"
#include "../processes/scheduler.h"
#include "syscall.h"
#include "test_util.h"

// enum State { RUNNING, BLOCKED, KILLED };

typedef struct P_rq {
  int32_t pid;
  process_status state;
} p_rq;

int64_t test_processes(uint64_t argc, char *argv[], processManagerADT pm,
                       schedulerADT scheduler) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {0};

  if (argc != 1)
    return -1;

  if ((max_processes = satoi(argv[0])) <= 0)
    return -1;

  p_rq p_rqs[max_processes];

  // while (1) {

  // Create max_processes processes
  for (rq = 0; rq < max_processes; rq++) {
    p_rqs[rq].pid = create_process(scheduler, pm, &endless_loop, argvAux, 0,
                                   "endless_loop", 0, 1, 1, 1);

    if (p_rqs[rq].pid == -1) {
      put_string_nt("test_processes: ERROR creating process\n", 0xFF0000,
                    0x000);
      return -1;
    } else {
      put_string_nt((uint8_t *)"Created process number: ", 0x00FF00, 0x000000);
      uint8_t num[20] = {0};
      itoa(rq, num);
      put_string_nt((uint8_t *)num, 0x00FF00, 0x000000);
      put_string_nt((uint8_t *)"\n", 0x00FF00, 0x000000);
      p_rqs[rq].state = RUNNING;
      alive++;
    }
  }

  // Randomly kills, blocks or unblocks processes until every one has been
  //   killed
  while (alive > 0) {

    uint64_t killed_processes = 0;
    uint64_t blocked_processes = 0;
    uint64_t unblocked_processes = 0;

    for (rq = 0; rq < max_processes; rq++) {
      action = GetUniform(100) % 2;

      switch (action) {
      case 0:
        if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
          if (kill(pm, p_rqs[rq].pid, scheduler) == -1) {
            put_string_nt("test_processes:  ERROR killing process\n", 0xFF0000,
                          0x000000);
            return -1;
          }

          killed_processes++;

          p_rqs[rq].state = KILLED;
          put_string_nt((uint8_t *)"Killed process number: ", ORANGE, 0x000000);
          uint8_t num[20] = {0};
          itoa(rq, num);
          put_string_nt((uint8_t *)num, ORANGE, 0x000000);
          put_string_nt((uint8_t *)"\n", ORANGE, 0x000000);
          alive--;
          sys_sleep(0, 500);
        }
        break;

      case 1:
        if (p_rqs[rq].state == RUNNING) {
          if (block(pm, scheduler, p_rqs[rq].pid) == -1) {
            put_string_nt("test_processes:  ERROR blocking process\n", 0xFF0000,
                          0x000000);
            // printf("test_processes: ERROR blocking process\n");
            return -1;
          }

          blocked_processes++;

          p_rqs[rq].state = BLOCKED;
          put_string_nt((uint8_t *)"Blocked process number: ", 0x00FF00,
                        0x000000);
          uint8_t num[20] = {0};
          itoa(rq, num);
          put_string_nt((uint8_t *)num, 0x00FF00, 0x000000);
          put_string_nt((uint8_t *)"\n", 0x00FF00, 0x000000);
          sys_sleep(0, 500);
        }
        break;
      }
    }

    // Randomly unblocks processes
    for (rq = 0; rq < max_processes; rq++)
      if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
        if (unblock(pm, scheduler, p_rqs[rq].pid) == -1) {
          put_string_nt("test_processes:  ERROR unblocking process\n", 0xFF0000,
                        0x000000);
          // printf("test_processes: ERROR unblocking process\n");
          return -1;
        }

        unblocked_processes++;

        p_rqs[rq].state = RUNNING;
        put_string_nt((uint8_t *)"Unblocked process number: ", BLUE, 0x000000);
        uint8_t num[20] = {0};
        itoa(rq, num);
        put_string_nt((uint8_t *)num, BLUE, 0x000000);
        put_string_nt((uint8_t *)"\n", BLUE, 0x000000);
        sys_sleep(0, 500);
      }
    put_string_nt("Blocked processes: ", GREEN, BLACK);
    uint8_t num[20] = {0};
    itoa(blocked_processes, num);
    put_string_nt((uint8_t *)num, BLUE, 0x000000);
    put_string_nt((uint8_t *)"\n", BLUE, 0x000000);

    put_string_nt("Killed processes: ", GREEN, BLACK);
    itoa(killed_processes, num);
    put_string_nt((uint8_t *)num, BLUE, 0x000000);
    put_string_nt((uint8_t *)"\n", BLUE, 0x000000);

    put_string_nt("Unblocked processes: ", GREEN, BLACK);
    itoa(unblocked_processes, num);
    put_string_nt((uint8_t *)num, BLUE, 0x000000);
    put_string_nt((uint8_t *)"\n", BLUE, 0x000000);

    put_string_nt("Remaining alive processes: ", GREEN, BLACK);
    itoa(alive, num);
    put_string_nt((uint8_t *)num, BLUE, 0x000000);
    put_string_nt((uint8_t *)"\n", BLUE, 0x000000);

    sys_sleep(10, 0);
  }
  put_string_nt("Test: Success\n", GREEN, BLACK);
}