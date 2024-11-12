// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../../processUtils.h"
#include <libc.h>
#include <stddef.h>
#include <syscaller.h>
#include <tests.h>

#define MAX_PHILOS 10
#define MIN_PHILOS 2
#define START_PHILOS 4
typedef uint8_t philo;

philo diners[MAX_PHILOS];
semaphore forks[MAX_PHILOS];
uint8_t *states[MAX_PHILOS];

uint8_t philo_count = START_PHILOS;
semaphore mutex = MAX_PHILOS, print_flag = MAX_PHILOS + 1;

int64_t philosopher(uint64_t argc, uint8_t *argv[]);

void init_philos() {
  for (int i = 0; i < START_PHILOS; i++) {
    sys_sem_destroy_asm(i);
    forks[i] = sys_sem_init_asm(i, 1);
    states[i] = (uint8_t *)".";
  }
  for (int i = 0; i < START_PHILOS; i++) {
    uint8_t pos[3] = {0};
    itoa(i, pos);
    uint8_t *argv[] = {pos};
    diners[i] = sys_create_process_asm(philosopher, 1, argv,
                                       (uint8_t *)"philosopher", 0);
  }
  sys_sem_destroy_asm(print_flag);
  sys_sem_destroy_asm(mutex);
  print_flag = sys_sem_init_asm(print_flag, 0);
  mutex = sys_sem_init_asm(mutex, 1);
}

void change_right_fork(uint8_t var) {
  sys_sem_wait_asm(mutex);

  sys_sem_post_asm(mutex);
}

int64_t philosopher(uint64_t argc, uint8_t *argv[]) {
  if (argc != 1) {
    print((uint8_t *)"Error: philosopher needs 1 argument\n");
    return 0;
  }
  uint8_t var = (uint8_t)satoi(argv[0]);
  semaphore left_fork = sys_sem_open_asm(forks[var]);
  semaphore right_fork;

  while (1) {
    right_fork = sys_sem_open_asm(forks[(var + 1) % philo_count]);
    sys_sem_wait_asm(left_fork);
    sys_sem_wait_asm(right_fork);

    states[var] = (uint8_t *)"E";
    sys_sem_post_asm(print_flag);
    sleep(0, 500);
    sys_sem_post_asm(left_fork);
    sys_sem_post_asm(right_fork);
    states[var] = (uint8_t *)".";
    sys_sem_post_asm(print_flag);
    sleep(0, 1500);
  }
  return 1;
}

int64_t print_philos_fn(uint64_t argc, uint8_t *argv[]) {
  while (1) {
    sys_sem_wait_asm(print_flag);
    for (int i = 0; i < philo_count; i++) {
      print(states[i]);
      print((uint8_t *)" ");
    }
    print((uint8_t *)"\n");
  }
  return 0;
}

int64_t philos_fn(uint64_t argc, uint8_t *argv[]) {
  uint8_t aux[10];
  init_philos();
  int64_t print_philos = sys_create_process_asm(print_philos_fn, 0, NULL,
                                                (uint8_t *)"print_philos", 1);

  while (1) {
    uint8_t c = getchar();
    if (c == 'q') {
      sys_kill_asm(print_philos);
      sys_sem_destroy_asm(print_flag);
      for (int i = 0; i < philo_count; i++) {
        sys_kill_asm(diners[i]);
        sys_sem_destroy_asm(forks[i]);
      }
      sys_sem_destroy_asm(mutex);
      break;
    } else if (c == 'a') {
      if (philo_count < MAX_PHILOS) {
        uint8_t pos[3] = {0};
        itoa(philo_count, pos);
        uint8_t *argv[] = {pos};
        sys_sem_wait_asm(forks[philo_count - 1]);
        states[philo_count] = (uint8_t *)".";
        diners[philo_count] = sys_create_process_asm(
            philosopher, 1, argv, (uint8_t *)"philosopher", 0);
        forks[philo_count] = sys_sem_init_asm(philo_count, 1);
        philo_count++;
        sys_sem_post_asm(forks[philo_count - 2]);
        print((uint8_t *)"Added philosopher number: ");
        itoa(philo_count, aux);
        print(aux);
        print((uint8_t *)"\n");
      }
    } else if (c == 'd') {
      if (philo_count > MIN_PHILOS) {
        sys_sem_wait_asm(forks[philo_count - 1]);
        print((uint8_t *)"Deleted philosopher number: ");
        itoa(philo_count, aux);
        print(aux);
        print((uint8_t *)"\n");
        philo_count--;
        sys_kill_asm(diners[philo_count]);
        sys_sem_post_asm(forks[philo_count]);
        sleep(0, 50);

        sys_sem_destroy_asm(forks[philo_count]);
      }
    }
  }
  return 0;
}