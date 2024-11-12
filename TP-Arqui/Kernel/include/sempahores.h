#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <list.h>
#include <memoryManager.h>
#include <process.h>
#include <scheduler.h>
#include <stdint.h>

#define MAX_SEM_COUNT 1024

#define MAX_USER_SEM_ID 256

typedef struct sem_t {
  uint16_t id;
  uint8_t lock;              // 1 = free, 0 = taken
  uint8_t count;             // sem value (shared resource)
  listADT blocked_processes; // list
} sem_t;

typedef struct semaphoreManagerCDT *semaphoreManagerADT;

semaphoreManagerADT create_semaphore_manager();
void destroy_semaphore_manager(semaphoreManagerADT sm);

void acquire(uint8_t *lock);
void release(uint8_t *lock);

/**
 *@param sm the semaphore manager structure
 *@param id the semaphore's identifier, must be between 0 and
 *MAX_SEM_COUNT - 1
 *@param count the initial value assigned to the semaphore
 */
sem_t *sem_init(semaphoreManagerADT sm, uint16_t id, uint8_t count);
void sem_wait(semaphoreManagerADT sm, process_control_block *running_pcb,
              sem_t *sem);
void sem_post(semaphoreManagerADT sm, sem_t *sem);
void sem_destroy(semaphoreManagerADT sm, sem_t *sem);

sem_t *get_sem(semaphoreManagerADT sm, uint16_t id);

#endif