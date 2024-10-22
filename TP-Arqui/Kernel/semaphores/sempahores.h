#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include "../memory/memoryManager.h"
#include "../processes/list.h"
#include "../processes/process.h"
#include "../processes/scheduler.h"
#include <stdint.h>

#define MAX_SEM_COUNT 32

typedef struct sem_t {
  uint8_t id;
  uint8_t lock;              // 0 = free, 1 = taken
  uint8_t count;             // sem value (shared resource)
  listADT blocked_processes; // list

} sem_t;

typedef struct semaphoreManagerCDT *semaphoreManagerADT;

void acquire(uint8_t *lock);
void release(uint8_t *lock);

/**
 *@param sm the semaphore manager structure
 *@param id the semaphore's identifier, must be between 0 and
 *MAX_SEM_COUNT - 1
 *@param count the initial value assigned to the semaphore
 */
sem_t *sem_init(semaphoreManagerADT sm, uint8_t id, uint8_t count);
void sem_wait(semaphoreManagerADT sm, process_control_block *running_pcb,
              sem_t *sem);
void sem_post(semaphoreManagerADT sm, sem_t *sem);
void sem_destroy(semaphoreManagerADT sm, sem_t *sem);

#endif