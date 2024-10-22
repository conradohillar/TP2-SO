#include "./sempahores.h"

sem_t *sem_init(uint8_t id, uint8_t count) {
  sem_t *sem = (sem_t *)mm_malloc(sizeof(sem_t));
  sem->id = id;
  sem->lock = 0;
  sem->count = count;
  sem->blocked_processes = NULL;
  return sem;
}

void sem_wait(sem_t *sem) {}

void sem_post(sem_t *sem) {
  acquire(&sem->lock);
  sem->count++;

  if (sem->count <= 0) {
    // wake up a process
  }
  release(&sem->lock);
}

void sem_destroy(sem_t *sem) { mm_free(sem); }
