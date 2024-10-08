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
      put_string_nt("test_processes: ERROR creating process\n", 0xFFF, 0x000);
      return -1;
    } else {
      put_string_nt((uint8_t *)"Process number: ", 0x00FF00, 0x000000);
      uint8_t num[20] = {0};
      itoa(rq, num);
      put_string_nt((uint8_t *)num, 0x00FF00, 0x000000);
      put_string_nt((uint8_t *)"\n", 0x00FF00, 0x000000);
      p_rqs[rq].state = RUNNING;
      alive++;
    }
  }

  // Randomly kills, blocks or unblocks processes until every one has been
  // killed while (alive > 0) {

  //   for (rq = 0; rq < max_processes; rq++) {
  //     action = GetUniform(100) % 2;

  //     switch (action) {
  //       case 0:
  //         if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
  //           if (my_kill(p_rqs[rq].pid) == -1) {
  //             printf("test_processes: ERROR killing process\n");
  //             return -1;
  //           }
  //           p_rqs[rq].state = KILLED;
  //           alive--;
  //         }
  //         break;

  //       case 1:
  //         if (p_rqs[rq].state == RUNNING) {
  //           if (my_block(p_rqs[rq].pid) == -1) {
  //             printf("test_processes: ERROR blocking process\n");
  //             return -1;
  //           }
  //           p_rqs[rq].state = BLOCKED;
  //         }
  //         break;
  //     }
  //   }

  //   // Randomly unblocks processes
  //   for (rq = 0; rq < max_processes; rq++)
  //     if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
  //       if (my_unblock(p_rqs[rq].pid) == -1) {
  //         printf("test_processes: ERROR unblocking process\n");
  //         return -1;
  //       }
  //       p_rqs[rq].state = RUNNING;
  //     }
  // }
  // }
}
