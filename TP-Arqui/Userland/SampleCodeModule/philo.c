#include <libc.h>
#include <stddef.h>
#include <syscaller.h>
#include <tests.h>

#define MAX_PHILOS 10
#define MIN_PHILOS 1
#define START_PHILOS 5
#define EATING (uint8_t *)"E"
#define THINKING (uint8_t *)"T"
#define HUNGRY (uint8_t *)"H"
uint8_t philo_count = START_PHILOS;
#define LEFT ((i + philo_count - 1) % philo_count)
#define RIGHT ((i + 1) % philo_count)
typedef uint8_t philo;
typedef uint16_t sem_t;

philo diners[MAX_PHILOS];
sem_t philo_sem[MAX_PHILOS];
uint8_t *states[MAX_PHILOS];

sem_t mutex = MAX_PHILOS, print_flag = MAX_PHILOS + 1, c_flag = MAX_PHILOS + 2;

int64_t philosopher(uint64_t argc, uint8_t *argv[]);

void init_philos() {
  for (uint8_t i = 0; i < philo_count; i++) {
    philo_sem[i] = sys_sem_init_asm(i, 0);
    states[i] = THINKING;
    uint8_t aux[10];
    itoa(i, aux);
    diners[i] = sys_create_process_asm(philosopher, 1, (uint8_t *[]){aux},
                                       (uint8_t *)"philosopher", 0);
  }
  mutex = sys_sem_init_asm(mutex, 1);
  print_flag = sys_sem_init_asm(print_flag, 0);
  c_flag = sys_sem_init_asm(c_flag, 0);
  return;
}

void test(uint8_t i) {
  if (states[i] == HUNGRY &&
      states[((i + philo_count - 1) % philo_count)] != EATING &&
      states[((i + 1) % philo_count)] != EATING) {
    states[i] = EATING;
    sys_sem_post_asm(print_flag);
    sys_sem_post_asm(philo_sem[i]);
  }
}

void take_forks(uint8_t i) {
  sys_sem_wait_asm(mutex);
  states[i] = HUNGRY;
  sys_sem_post_asm(print_flag);
  sleep(0, 50);
  test(i);
  sys_sem_post_asm(mutex);
  sys_sem_wait_asm(philo_sem[i]);
}

void put_forks(uint8_t i) {
  sys_sem_wait_asm(mutex);
  states[i] = THINKING;
  sys_sem_post_asm(print_flag);
  sleep(0, 50);

  test(((i + philo_count - 1) % philo_count));
  test(((i + 1) % philo_count));
  sys_sem_post_asm(mutex);
}

void think() { sleep(0, 150); }

void eat() { sleep(0, 300); }

int64_t philosopher(uint64_t argc, uint8_t *argv[]) {
  if (argc != 1) {
    print((uint8_t *)"Error: philosopher needs 1 argument\n");
    return -1;
  }
  uint8_t i = (uint8_t)argv[0][0] - '0';
  while (1) {
    think();
    take_forks(i);
    eat();
    put_forks(i);
  }
}

int64_t print_philos(uint64_t argc, uint8_t *argv[]) {
  if (argc) {
    sys_set_fd_asm(STDOUT, satoi(argv[0]));
  }
  while (1) {
    sys_sem_wait_asm(print_flag);
    for (uint8_t i = 0; i < philo_count; i++) {
      print(states[i]);
      print((uint8_t *)" ");
    }
    print((uint8_t *)"\n");
  }
  return 0;
}

int64_t philos_fn(uint64_t argc, uint8_t *argv[]) {
  uint64_t print_argc = argc;
  uint8_t *print_argv[1] = {0};

  if (argc == 1) {
    print_argv[0] = argv[0];
  }
  init_philos();
  uint8_t print_philos_pid = sys_create_process_asm(
      print_philos, print_argc, print_argv, (uint8_t *)"print_philos", 1);

  uint8_t c;
  while (1) {
    c = getchar();
    switch (c) {
    case 'q':
      sys_sem_wait_asm(mutex);
      sys_kill_asm(print_philos_pid);
      for (uint8_t i = 0; i < philo_count; i++) {
        sys_kill_asm(diners[i]);
        sys_sem_destroy_asm(philo_sem[i]);
      }
      sys_sem_destroy_asm(mutex);
      sys_sem_destroy_asm(print_flag);
      sys_sem_destroy_asm(c_flag);
      return 0;
      break;

    case 'a':
      sys_sem_wait_asm(mutex);
      if (philo_count < MAX_PHILOS) {
        uint8_t aux[10];
        philo_sem[philo_count] = sys_sem_init_asm(philo_count, 0);
        states[philo_count] = THINKING;
        itoa(philo_count, aux);
        diners[philo_count] = sys_create_process_asm(
            philosopher, 1, (uint8_t *[]){aux}, (uint8_t *)"philosopher", 0);
        philo_count++;
      }
      sys_sem_post_asm(mutex);
      break;

    case 'd':
      sys_sem_wait_asm(mutex);
      if (philo_count > MIN_PHILOS) {
        states[philo_count - 1] = THINKING;
        states[philo_count - 2] = THINKING;
        philo_count--;
        sys_kill_asm(diners[philo_count]);
        sys_sem_destroy_asm(philo_sem[philo_count]);
      }
      sys_sem_post_asm(mutex);
      break;

    default:
      break;
    }
  }
}