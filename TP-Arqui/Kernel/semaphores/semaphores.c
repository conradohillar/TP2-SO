#include "./sempahores.h"

extern processManagerADT my_pm;
extern schedulerADT my_scheduler;

typedef struct semaphoreManagerCDT {
  sem_t *semaphores[MAX_SEM_COUNT];
  uint8_t count;
} semaphoreManagerCDT;

semaphoreManagerADT create_semaphore_manager() {
  semaphoreManagerADT sm =
      (semaphoreManagerADT)mm_malloc(sizeof(semaphoreManagerCDT));
  if (sm == NULL) {
    // TODO: MANEJAR ERROR DE MALLOC
    return NULL;
  }
  sm->count = 0;
  for (int i = 0; i < MAX_SEM_COUNT; i++) {
    sm->semaphores[i] = NULL;
  }
  return sm;
}

sem_t *sem_init(semaphoreManagerADT sm, uint16_t id, uint8_t count) {
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
  sem->lock = 1;
  sem->count = count;
  sem->blocked_processes = create_list();
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
    add_to_list(sem->blocked_processes, (void *)running_pcb);
    block(my_pm, running_pcb->pid);
  }
}

void sem_post(semaphoreManagerADT sm, sem_t *sem) {
  acquire(&sem->lock);
  if (sem->count == 0 && !is_empty(sem->blocked_processes)) {
    release(&sem->lock);
    process_control_block *pcb =
        (process_control_block *)remove_first(sem->blocked_processes);
    unblock(my_pm, pcb->pid);
  } else {
    sem->count++;
    release(&sem->lock);
  }
}

sem_t *get_sem(semaphoreManagerADT sm, uint16_t id) {
  if (sm == NULL || id >= MAX_SEM_COUNT) {
    return NULL;
  }
  return sm->semaphores[id];
}

void sem_destroy(semaphoreManagerADT sm, sem_t *sem) {
  if (sm == NULL || sem == NULL) {
    return;
  }
  free_list(sem->blocked_processes);
  sm->semaphores[sem->id] = NULL;
  mm_free(sem);
}

void destroy_semaphore_manager(semaphoreManagerADT sm) {
  if (sm == NULL) {
    return;
  }
  for (int i = 0; i < MAX_SEM_COUNT; i++) {
    if (sm->semaphores[i] != NULL) {
      sem_destroy(sm, sm->semaphores[i]);
    }
  }
  mm_free(sm);
}
