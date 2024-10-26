#include "../include/syscaller.h"
#include "../include/tests.h"

#define SEM_ID 1
#define TOTAL_PAIR_PROCESSES 10
#define NULL 0

int64_t global; // shared memory
void ps();

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  sys_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[]) {
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
      print("test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      sys_sem_wait_asm(SEM_ID);
    // print("At inc/dec\n");
    slowInc(&global, inc);
    if (use_sem)
      sys_sem_post_asm(SEM_ID);
  }

  if (use_sem) {
    // sys_sem_close_asm(SEM_ID);
  }

  return 0;
}

int64_t test_sem_synchro_fn(uint64_t argc, char *argv[]) { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)
    return -1;

  char *argvDec[] = {argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {argv[0], "1", argv[1], NULL};

  global = 0;

  uint64_t i;
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = sys_create_process_asm(&my_process_inc, 3, argvDec,
                                     "my_process_inc", 0);
    pids[i + TOTAL_PAIR_PROCESSES] = sys_create_process_asm(
        &my_process_inc, 3, argvInc, "my_process_inc", 0);
    // print("Process created\n");
  }
  // ps();
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    // print("Process waiting\n");
    sys_waitpid_asm(pids[i]);
    sys_waitpid_asm(pids[i + TOTAL_PAIR_PROCESSES]);
    // print("Process waited\n");
  }
  //   ps();
  sys_sem_destroy_asm(sys_sem_open_asm(SEM_ID));
  return global;
}