#include "./sempahores.h"

extern processManagerADT my_pm;
extern schedulerADT my_scheduler;

typedef struct semaphoreManagerCDT {
  sem_t *semaphores[MAX_SEM_COUNT];
  uint8_t count;
} semaphoreManagerCDT;

sem_t *sem_init(semaphoreManagerADT sm, uint8_t id, uint8_t count) {
  if (sm == NULL || sm->semaphores[id] != NULL) {
    // TODO: MANEJAR ERRORES DE CREACION DE SEMAFOROS
    return NULL;
  }

  sem_t *sem = (sem_t *)mm_malloc(sizeof(sem_t));
  if (sem == NULL) {
    // TODO: MANEJAR ERROR DE MALLOC
    return NULL;
  }
  sem->id = id;
  sem->lock = 0;
  sem->count = count;
  sem->blocked_processes = NULL;
  sm->semaphores[id] = sem;
  return sem;
}

void sem_wait(semaphoreManagerADT sm, process_control_block *running_pcb,
              sem_t *sem) {
  acquire(&sem->lock);
  if (sem->count > 0) {
    sem->count--;
    release(&sem->lock);
  } else {
    release(&sem->lock);
    add_to_list(sem->blocked_processes, running_pcb);
    block(my_pm, running_pcb);
  }
}

void sem_post(semaphoreManagerADT sm, sem_t *sem) {
  acquire(&sem->lock);
  if (sem->count == 0 && !is_empty(sem->blocked_processes)) {
    release(&sem->lock);
    process_control_block *pcb =
        (process_control_block *)remove_first(sem->blocked_processes);
    unblock(my_pm, pcb);
  } else {
    sem->count++;
    release(&sem->lock);
  }
}

void sem_destroy(semaphoreManagerADT sm, sem_t *sem) {
  if (sm == NULL || sem == NULL) {
    return;
  }
  sm->semaphores[sem->id] = NULL;
  mm_free(sem);
}
