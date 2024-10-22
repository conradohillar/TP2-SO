#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include "../memory/memoryManager.h"
#include "../processes/process.h"
#include <stdint.h>

typedef struct sem_t {
  uint8_t id;
  uint8_t lock;                             // 0 = free, 1 = taken
  uint8_t count;                            // sem value (shared resource)
  process_control_block *blocked_processes; // list
} sem_t;

void acquire(uint8_t *lock);
void release(uint8_t *lock);

sem_t *sem_init(uint8_t id, uint8_t count);
void sem_wait(sem_t *sem);
void sem_post(sem_t *sem);
void sem_destroy(sem_t *sem);

#endif