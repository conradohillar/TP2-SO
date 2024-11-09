// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/tests.h"

typedef struct P_rq {
  int64_t pid;
  process_status state;
} p_rq;

int64_t test_processes_fn(uint64_t argc, uint8_t *argv[]) {
  if (argc == 2) {
    sys_set_fd_asm(STDOUT, satoi(argv[1]));
  }

  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  uint8_t *argvAux[] = {0};

  if ((max_processes = satoi(argv[0])) <= 0)
    return -1;

  p_rq p_rqs[max_processes];

  // while (1) {
  // Create max_processes processes
  for (rq = 0; rq < max_processes; rq++) {

    p_rqs[rq].pid = sys_create_process_asm((fn)endless_loop, 0, argvAux,
                                           (uint8_t *)"endless_loop", 0);

    if (p_rqs[rq].pid == -1) {
      printcolor((uint8_t *)"test_processes: ERROR creating process\n",
                 0xFF0000, 0x000);
      return -1;
    } else {
      printcolor((uint8_t *)"Created process pid: ", ORANGE, 0x000000);
      uint8_t num[20] = {0};
      itoa(p_rqs[rq].pid, num);
      printcolor((uint8_t *)num, ORANGE, 0x000000);
      printcolor((uint8_t *)"\n", ORANGE, 0x000000);
      p_rqs[rq].state = RUNNING;
      alive++;
      sleep(0, 100);
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
          if (sys_kill_asm(p_rqs[rq].pid) == -1) {
            printcolor((uint8_t *)"test_processes:  ERROR killing process\n",
                       0xFF0000, 0x000000);
            return -1;
          }

          killed_processes++;

          p_rqs[rq].state = KILLED;
          printcolor((uint8_t *)"Killed process pid: ", RED, 0x000000);
          uint8_t num[20] = {0};
          itoa(p_rqs[rq].pid, num);
          printcolor((uint8_t *)num, RED, 0x000000);
          printcolor((uint8_t *)"\n", RED, 0x000000);
          alive--;
          sleep(0, 100);
        }
        break;

      case 1:
        if (p_rqs[rq].state == RUNNING) {
          if (sys_block_asm(p_rqs[rq].pid) == -1) {
            printcolor((uint8_t *)"test_processes:  ERROR blocking process\n",
                       0xFF0000, 0x000000);
            // printf((uint8_t *)"test_processes: ERROR blocking process\n");
            return -1;
          }

          blocked_processes++;

          p_rqs[rq].state = BLOCKED;
          printcolor((uint8_t *)"Blocked process pid: ", ORANGE, 0x000000);
          uint8_t num[20] = {0};
          itoa(p_rqs[rq].pid, num);
          printcolor((uint8_t *)num, ORANGE, 0x000000);
          printcolor((uint8_t *)"\n", ORANGE, 0x000000);
          sleep(0, 100);
        }
        break;
      }
    }

    // Randomly unblocks processes
    for (rq = 0; rq < max_processes; rq++)
      if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
        if (sys_unblock_asm(p_rqs[rq].pid) == -1) {
          printcolor((uint8_t *)"test_processes:  ERROR unblocking process\n",
                     0xFF0000, 0x000000);
          return -1;
        }

        unblocked_processes++;

        p_rqs[rq].state = RUNNING;
        printcolor((uint8_t *)"Unblocked process pid: ", GREEN, 0x000000);
        uint8_t num[20] = {0};
        itoa(p_rqs[rq].pid, num);
        printcolor((uint8_t *)num, GREEN, 0x000000);
        printcolor((uint8_t *)"\n", GREEN, 0x000000);
        sleep(0, 100);
      }
    printcolor((uint8_t *)"Blocked processes count: ", ORANGE, BLACK);
    uint8_t num[20] = {0};
    itoa(blocked_processes, num);
    printcolor((uint8_t *)num, ORANGE, 0x000000);
    printcolor((uint8_t *)"\n", BLUE, 0x000000);

    printcolor((uint8_t *)"Killed processes count: ", RED, BLACK);
    itoa(killed_processes, num);
    printcolor((uint8_t *)num, RED, 0x000000);
    printcolor((uint8_t *)"\n", BLUE, 0x000000);

    printcolor((uint8_t *)"Unblocked processes count: ", GREEN, BLACK);
    itoa(unblocked_processes, num);
    printcolor((uint8_t *)num, GREEN, 0x000000);
    printcolor((uint8_t *)"\n", BLUE, 0x000000);

    printcolor((uint8_t *)"Remaining alive processes: ", BLUE, BLACK);
    itoa(alive, num);
    printcolor((uint8_t *)num, BLUE, 0x000000);
    printcolor((uint8_t *)"\n", BLUE, 0x000000);

    sleep(0, 500);
  }
  printcolor((uint8_t *)"Test: Success\n", GREEN, BLACK);

  if (argc == 2) {
    print((uint8_t *)"\0");
    sys_set_fd_asm(STDOUT, STDOUT);
  }
  return 0;
}