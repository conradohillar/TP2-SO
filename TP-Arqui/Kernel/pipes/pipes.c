#include "../include/pipes.h"

extern processManagerADT my_pm;
extern semaphoreManagerADT my_sm;
extern schedulerADT my_scheduler;

typedef struct pipeManagerCDT {
  pipe_t *pipes[MAX_PIPES_COUNT];
  uint64_t count;
} pipeManagerCDT;

pipeManagerADT create_pipe_manager() {
  pipeManagerADT pipe_manager =
      (pipeManagerADT)mm_malloc(sizeof(pipeManagerCDT));
  if (pipe_manager == NULL) {
    return NULL;
  }
  pipe_manager->count = 0;
  for (int i = 0; i < MAX_PIPES_COUNT; i++) {
    pipe_manager->pipes[i] = NULL;
  }
  return pipe_manager;
}

pipe_t *create_pipe(pipeManagerADT pipe_manager) {
  if (pipe_manager->count == MAX_PIPES_COUNT) {
    return NULL;
  }
  pipe_t *pipe = (pipe_t *)mm_malloc(sizeof(pipe_t));
  if (pipe == NULL) {
    return NULL;
  }
  for (int i = 0; i < MAX_PIPES_COUNT; i++) {
    if (pipe_manager->pipes[i] == NULL) {
      pipe->id = i;
      // We want to create semaphores outside the "user semaphore space"
      pipe->mutex = sem_init(my_sm, MAX_USER_SEM_ID + 2 * i, 1);
      if (pipe->mutex == NULL) {
        mm_free(pipe);
        return NULL;
      }
      pipe->read_sem = sem_init(my_sm, MAX_USER_SEM_ID + 2 * i + 1, 0);
      if (pipe->read_sem == NULL) {
        sem_destroy(my_sm, pipe->mutex);
        mm_free(pipe);
        return NULL;
      }
      pipe->last_write_pos = 0;
      pipe->last_read_pos = 0;
      pipe->to_read_count = 0;
      pipe_manager->pipes[i] = pipe;
      pipe_manager->count++;
      return pipe;
    }
  }
  return NULL;
}

void destroy_pipe(pipeManagerADT pipe_manager, pipe_t *pipe) {
  if (pipe == NULL) {
    return;
  }
  sem_destroy(my_sm, pipe->mutex);
  pipe_manager->pipes[pipe->id] = NULL;
  mm_free(pipe);
  pipe_manager->count--;
}

uint64_t write_pipe(pipeManagerADT pipe_manager, pipe_t *pipe, uint8_t *buffer,
                    uint64_t bytes) {
  if (pipe == NULL) {
    return 0;
  }
  process_control_block *running_pcb = get_running(my_scheduler);
  sem_wait(my_sm, running_pcb, pipe->mutex);

  // We need to update the semaphore if the buffer was empty before writing and
  // now it has data
  uint8_t update_sem = pipe->to_read_count == 0;

  uint64_t i = 0;
  for (; i < bytes; i++) {
    pipe->buffer[pipe->last_write_pos] = buffer[i];
    pipe->last_write_pos = (pipe->last_write_pos + 1) % PIPE_BUFFER_SIZE;
    pipe->to_read_count++;
  }

  if (update_sem && pipe->to_read_count > 0) {
    sem_post(my_sm, pipe->read_sem);
  }

  sem_post(my_sm, pipe->mutex);
  return i;
}

uint64_t read_pipe(pipeManagerADT pipe_manager, pipe_t *pipe, uint8_t *buffer,
                   uint64_t bytes) {
  if (pipe == NULL) {
    return 0;
  }
  process_control_block *running_pcb = get_running(my_scheduler);

  sem_wait(my_sm, running_pcb, pipe->read_sem);
  sem_wait(my_sm, running_pcb, pipe->mutex);

  uint64_t i = 0;
  for (; i < bytes && pipe->to_read_count != 0; i++) {
    buffer[i] = pipe->buffer[pipe->last_read_pos];
    pipe->last_read_pos = (pipe->last_read_pos + 1) % PIPE_BUFFER_SIZE;
    pipe->to_read_count--;
  }

  // We need to leave the semaphore as "ready to read" if the buffer isn't empty
  if (pipe->to_read_count > 0) {
    sem_post(my_sm, pipe->read_sem);
  }

  sem_post(my_sm, pipe->mutex);
  return i;
}

void destroy_pipe_manager(pipeManagerADT pipe_manager) {
  for (int i = 0; i < MAX_PIPES_COUNT; i++) {
    if (pipe_manager->pipes[i] != NULL) {
      destroy_pipe(pipe_manager, pipe_manager->pipes[i]);
    }
  }
  mm_free(pipe_manager);
}