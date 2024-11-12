// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/syscaller.h"
#include "../include/tests.h"

#define SEM_ID 1
#define TOTAL_PAIR_PROCESSES 4
#define NULL (void *)0

int64_t global; // shared memory
void ps();

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  sys_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

int64_t my_process_inc(uint64_t argc, uint8_t *argv[]) {
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;

  if (use_sem)
    if (sys_sem_open_asm(SEM_ID) == -1) {
      print((uint8_t *)"test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      sys_sem_wait_asm(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      sys_sem_post_asm(SEM_ID);
  }

  // if (use_sem) {
  //    sys_sem_close_asm(SEM_ID);
  // }

  return 0;
}

int64_t test_sem_synchro_fn(uint64_t argc, uint8_t *argv[]) { //{n, use_sem}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];
  // ps();
  if (argc != 2)
    return -1;

  uint8_t *argvDec[] = {argv[0], (uint8_t *)"-1", argv[1], NULL};
  uint8_t *argvInc[] = {argv[0], (uint8_t *)"1", argv[1], NULL};

  global = 0;
  sys_sem_open_asm(SEM_ID);
  uint64_t i = 0;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = sys_create_process_asm(my_process_inc, 3, argvDec,
                                     (uint8_t *)"my_process_inc", 0);
    pids[i + TOTAL_PAIR_PROCESSES] = sys_create_process_asm(
        my_process_inc, 3, argvInc, (uint8_t *)"my_process_inc", 0);
  }
  //   Uncomment the following line to see that all the processes are being
  //   correctly created
  // ps();
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    sys_waitpid_asm(pids[i]);
    sys_waitpid_asm(pids[i + TOTAL_PAIR_PROCESSES]);
  }
  // ps();
  sys_sem_destroy_asm(SEM_ID);
  return global;
}