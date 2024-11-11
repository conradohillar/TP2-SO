#include "../include/pipes.h"

extern processManagerADT my_pm;
extern semaphoreManagerADT my_sm;
extern schedulerADT my_scheduler;

typedef struct pipeManagerCDT {
  pipe_t *pipes[MAX_PIPES_COUNT];
  uint64_t count;
} pipeManagerCDT;

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

      // pipe id = 0 is for STDIN, the rest are for the processes (we skip 1 and
      // 2 for STDOUT and STDERR)
      pipe->id = (i ? i + 2 : 0);

      // We want to create semaphores outside the "user semaphore space"
      pipe->mutex = sem_init(my_sm, MAX_USER_SEM_ID + 3 * i, 1);
      if (pipe->mutex == NULL) {
        mm_free(pipe);
        return NULL;
      }
      pipe->read_sem = sem_init(my_sm, MAX_USER_SEM_ID + 3 * i + 1, 0);
      if (pipe->read_sem == NULL) {
        sem_destroy(my_sm, pipe->mutex);
        mm_free(pipe);
        return NULL;
      }
      pipe->write_sem = sem_init(my_sm, MAX_USER_SEM_ID + 3 * i + 2, 1);
      if (pipe->write_sem == NULL) {
        sem_destroy(my_sm, pipe->mutex);
        sem_destroy(my_sm, pipe->read_sem);
        mm_free(pipe);
        return NULL;
      }
      pipe->last_write_pos = 0;
      pipe->last_read_pos = 0;
      pipe->to_read_count = 0;
      pipe->write_waiting = 0;
      pipe->read_waiting = 1;
      pipe_manager->pipes[i] = pipe;
      pipe_manager->count++;
      return pipe;
    }
  }
  return NULL;
}

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

  // We create the STDIN pipe
  create_pipe(pipe_manager);

  return pipe_manager;
}

void destroy_pipe(pipeManagerADT pipe_manager, pipe_t *pipe) {
  if (pipe == NULL) {
    return;
  }

  uint16_t pipe_id = pipe->id ? pipe->id - 2 : 0;

  sem_destroy(my_sm, pipe->mutex);
  sem_destroy(my_sm, pipe->read_sem);
  sem_destroy(my_sm, pipe->write_sem);
  pipe_manager->pipes[pipe_id] = NULL;
  mm_free(pipe);
  pipe_manager->count--;
}

uint64_t write_pipe(pipeManagerADT pipe_manager, pipe_t *pipe, uint8_t *buffer,
                    uint64_t bytes) {
  if (pipe == NULL || bytes == 0) {
    return 0;
  }
  process_control_block *running_pcb = get_running(my_scheduler);

  sem_wait(my_sm, running_pcb, pipe->mutex);
  pipe->write_waiting = 1;
  sem_post(my_sm, pipe->mutex);
  sem_wait(my_sm, running_pcb, pipe->write_sem);
  sem_wait(my_sm, running_pcb, pipe->mutex);
  pipe->write_waiting = 0;

  uint64_t i = 0;
  while (i < bytes) {
    pipe->buffer[pipe->last_write_pos] = buffer[i++];
    pipe->last_write_pos = (pipe->last_write_pos + 1) % PIPE_BUFFER_SIZE;
    pipe->to_read_count++;

    if (pipe->read_waiting) {
      sem_post(my_sm, pipe->read_sem);
      pipe->read_waiting = 0;
    }

    if (pipe->to_read_count == PIPE_BUFFER_SIZE && i < bytes) {
      pipe->write_waiting = 1;
      sem_post(my_sm, pipe->mutex);
      sem_wait(my_sm, running_pcb, pipe->write_sem);
      sem_wait(my_sm, running_pcb, pipe->mutex);
    }
  }

  if (pipe->to_read_count < PIPE_BUFFER_SIZE) {
    sem_post(my_sm, pipe->write_sem);
  }

  sem_post(my_sm, pipe->mutex);
  return i;
}

uint64_t read_pipe(pipeManagerADT pipe_manager, pipe_t *pipe, uint8_t *buffer,
                   uint64_t bytes) {
  if (pipe == NULL || bytes == 0) {
    return 0;
  }
  process_control_block *running_pcb = get_running(my_scheduler);

  sem_wait(my_sm, running_pcb, pipe->mutex);
  pipe->read_waiting = 1;
  sem_post(my_sm, pipe->mutex);
  sem_wait(my_sm, running_pcb, pipe->read_sem);
  sem_wait(my_sm, running_pcb, pipe->mutex);
  pipe->read_waiting = 0;

  uint64_t i = 0;
  while (i < bytes) {
    buffer[i++] = pipe->buffer[pipe->last_read_pos];
    pipe->last_read_pos = (pipe->last_read_pos + 1) % PIPE_BUFFER_SIZE;
    pipe->to_read_count--;

    if (pipe->write_waiting) {
      sem_post(my_sm, pipe->write_sem);
      pipe->write_waiting = 0;
    }

    // If the buffer is empty and we haven't reached bytes amount of chars read,
    // we block the process until something is written
    if (pipe->to_read_count == 0 && i < bytes) {
      pipe->read_waiting = 1;
      sem_post(my_sm, pipe->mutex);
      sem_wait(my_sm, running_pcb, pipe->read_sem);
      sem_wait(my_sm, running_pcb, pipe->mutex);
    }
  }

  // If there are still bytes to read, we allow the other read_pipes to read, if
  // not, write_pipes will allow reading once it writes something in the buffer
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

uint8_t check_pipe_id(pipeManagerADT pipe_manager, uint16_t pipe_id) {
  int16_t real_id = pipe_id ? pipe_id - 2 : 0;
  return (real_id >= 0 && real_id < MAX_PIPES_COUNT &&
          pipe_manager->pipes[real_id] != NULL);
}

pipe_t *get_pipe(pipeManagerADT pipe_manager, uint16_t pipe_id) {
  int16_t real_id = pipe_id ? pipe_id - 2 : 0;
  if (real_id < 0 || real_id >= MAX_PIPES_COUNT) {
    return NULL;
  }
  return pipe_manager->pipes[real_id];
}