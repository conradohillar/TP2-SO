#include "../include/tests.h"

typedef enum { RUNNING, BLOCKED, KILLED } status;

typedef struct P_rq {
  int64_t pid;
  status state;
} p_rq;

uint64_t test_processes_fn(uint64_t argc, uint8_t *argv[]) {
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
    p_rqs[rq].pid = sys_create_process_asm((uint64_t)endless_loop, 0, argvAux,
                                           "endless_loop", 0);

    if (p_rqs[rq].pid == -1) {
      printcolor("test_processes: ERROR creating process\n", 0xFF0000, 0x000);
      return -1;
    } else {
      printcolor((uint8_t *)"Created process number: ", 0x00FF00, 0x000000);
      uint8_t num[20] = {0};
      itoa(rq, num);
      printcolor((uint8_t *)num, 0x00FF00, 0x000000);
      printcolor((uint8_t *)"\n", 0x00FF00, 0x000000);
      p_rqs[rq].state = RUNNING;
      alive++;
    }
  }

  // // Randomly kills, blocks or unblocks processes until every one has been
  // //   killed
  // while (alive > 0) {

  //   uint64_t killed_processes = 0;
  //   uint64_t blocked_processes = 0;
  //   uint64_t unblocked_processes = 0;

  //   for (rq = 0; rq < max_processes; rq++) {
  //     action = GetUniform(100) % 2;

  //     switch (action) {
  //     case 0:
  //       if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
  //         if (kill(pm, p_rqs[rq].pid, scheduler) == -1) {
  //           printcolor("test_processes:  ERROR killing process\n",
  //           0xFF0000,
  //                         0x000000);
  //           return -1;
  //         }

  //         killed_processes++;

  //         p_rqs[rq].state = KILLED;
  //         printcolor((uint8_t *)"Killed process number: ", ORANGE,
  //         0x000000); uint8_t num[20] = {0}; itoa(rq, num);
  //         printcolor((uint8_t *)num, ORANGE, 0x000000);
  //         printcolor((uint8_t *)"\n", ORANGE, 0x000000);
  //         alive--;
  //         sys_sleep(0, 500);
  //       }
  //       break;

  //     case 1:
  //       if (p_rqs[rq].state == RUNNING) {
  //         if (block(pm, scheduler, p_rqs[rq].pid) == -1) {
  //           printcolor("test_processes:  ERROR blocking process\n",
  //           0xFF0000,
  //                         0x000000);
  //           // printf("test_processes: ERROR blocking process\n");
  //           return -1;
  //         }

  //         blocked_processes++;

  //         p_rqs[rq].state = BLOCKED;
  //         printcolor((uint8_t *)"Blocked process number: ", 0x00FF00,
  //                       0x000000);
  //         uint8_t num[20] = {0};
  //         itoa(rq, num);
  //         printcolor((uint8_t *)num, 0x00FF00, 0x000000);
  //         printcolor((uint8_t *)"\n", 0x00FF00, 0x000000);
  //         sys_sleep(0, 500);
  //       }
  //       break;
  //     }
  //   }

  //   // Randomly unblocks processes
  //   for (rq = 0; rq < max_processes; rq++)
  //     if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
  //       if (unblock(pm, scheduler, p_rqs[rq].pid) == -1) {
  //         printcolor("test_processes:  ERROR unblocking process\n",
  //         0xFF0000,
  //                       0x000000);
  //         // printf("test_processes: ERROR unblocking process\n");
  //         return -1;
  //       }

  //       unblocked_processes++;

  //       p_rqs[rq].state = RUNNING;
  //       printcolor((uint8_t *)"Unblocked process number: ", BLUE,
  //       0x000000); uint8_t num[20] = {0}; itoa(rq, num);
  //       printcolor((uint8_t *)num, BLUE, 0x000000);
  //       printcolor((uint8_t *)"\n", BLUE, 0x000000);
  //       sys_sleep(0, 500);
  //     }
  //   printcolor("Blocked processes: ", GREEN, BLACK);
  //   uint8_t num[20] = {0};
  //   itoa(blocked_processes, num);
  //   printcolor((uint8_t *)num, BLUE, 0x000000);
  //   printcolor((uint8_t *)"\n", BLUE, 0x000000);

  //   printcolor("Killed processes: ", GREEN, BLACK);
  //   itoa(killed_processes, num);
  //   printcolor((uint8_t *)num, BLUE, 0x000000);
  //   printcolor((uint8_t *)"\n", BLUE, 0x000000);

  //   printcolor("Unblocked processes: ", GREEN, BLACK);
  //   itoa(unblocked_processes, num);
  //   printcolor((uint8_t *)num, BLUE, 0x000000);
  //   printcolor((uint8_t *)"\n", BLUE, 0x000000);

  //   printcolor("Remaining alive processes: ", GREEN, BLACK);
  //   itoa(alive, num);
  //   printcolor((uint8_t *)num, BLUE, 0x000000);
  //   printcolor((uint8_t *)"\n", BLUE, 0x000000);

  //   sys_sleep(10, 0);
  // }
  // printcolor("Test: Success\n", GREEN, BLACK);
  return 0;
}