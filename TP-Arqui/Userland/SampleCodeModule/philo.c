#include "../include/libc.h"
#include "../include/syscaller.h"

#define MAX_PHILOS 10
#define MIN_PHILOS 1
#define START_PHILOS 4
#define MUTEX_ID MAX_PHILOS + 1
typedef uint8_t philo;

philo diners[MAX_PHILOS];
sem_t forks[MAX_PHILOS];

uint8_t philo_count = START_PHILOS;
sem_t mutex;

int64_t philosopher(uint64_t argc, uint8_t *argv[]);

void init_philos() {
  for (int i = 0; i < START_PHILOS; i++) {
    forks[i] = sys_sem_init_asm(i, 1);
  }
  for (int i = 0; i < START_PHILOS; i++) {
    uint8_t *pos;
    itoa(i, pos);
    uint8_t *argv[] = {pos};
    diners[i] = sys_create_process_asm(philosopher, 1, argv, "philosopher", 0);
  }
  return;
}

int64_t philosopher(uint64_t argc, uint8_t *argv[]) {
  if (argc != 1) {
    print("Error: philosopher needs 1 argument\n");
    return;
  }
  uint8_t var = (uint8_t)argv[0][0] - '0';
  sem_t left_fork = sys_sem_open_asm(var);
  sem_t right_fork = sys_sem_open_asm((var + 1) % (philo_count - 1));
  uint8_t *to_write = "Philosopher   is eating\n";
  to_write[12] = argv[0][0];

  while (1) {
    // sys_sem_wait_asm(mutex);
    sys_sem_wait_asm(left_fork);
    sys_sem_wait_asm(right_fork);
    // sys_sem_post_asm(mutex);

    print("Philosopher ");
    print(argv[0]);
    print(" is eating\n");
    sys_sleep_asm(1, 500);
    sys_sem_post_asm(left_fork);
    sys_sem_post_asm(right_fork);
    sys_sleep_asm(6, 0);
  }
  return;
}

int64_t philos(uint64_t argc, uint8_t *argv[]) {
  uint8_t philo_count = START_PHILOS;
  sem_t mutex = sys_sem_init_asm(MUTEX_ID, 1);
  uint8_t aux[10];
  init_philos();

  while (1) {
    uint8_t c = getchar();
    if (c == 'q') {
    //   sys_sem_wait_asm(mutex);
      for (int i = 0; i < philo_count; i++) {
        sys_kill_asm(diners[i]);
        sys_sem_destroy_asm(forks[i]);
      }
      break;
    } else if (c == 'a') {
      if (philo_count < MAX_PHILOS) {
        // sys_sem_wait_asm(mutex);
        uint8_t *pos;
        itoa(philo_count, pos);
        uint8_t *argv[] = {pos};
        sys_sem_wait_asm(forks[philo_count - 1]);
        sys_sem_wait_asm(forks[0]);
        diners[philo_count] =
            sys_create_process_asm(philosopher, 1, argv, "philosopher", 0);
        forks[philo_count] = sys_sem_init_asm(philo_count, 1);
        print("Added philosopher number: ");
        itoa(philo_count, aux);
        print(aux);
        print("\n");
        philo_count++;
        sys_sem_post_asm(forks[philo_count - 2]);
        sys_sem_post_asm(forks[0]);
        // sys_sem_post_asm(mutex);
      }
    } else if (c == 'd') {
      if (philo_count - 1 > MIN_PHILOS) {
        // sys_sem_wait_asm(mutex);
        sys_sem_wait_asm(forks[philo_count - 1]);
        sys_sem_wait_asm(forks[philo_count - 2]);
        sys_sem_wait_asm(forks[0]);
        philo_count--;
        print("Deleted philosopher number: ");
        itoa(philo_count, aux);
        print(aux);
        print("\n");
        sys_kill_asm(diners[philo_count]);
        sys_sem_destroy_asm(forks[philo_count]);
        sys_sem_post_asm(forks[philo_count - 1]);
        sys_sem_post_asm(forks[0]);
        // sys_sem_post_asm(mutex);
      }
    }
  }
  sys_sem_destroy_asm(mutex);
  return 0;
}
