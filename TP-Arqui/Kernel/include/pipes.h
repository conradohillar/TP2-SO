#ifndef PIPES_H
#define PIPES_H

#include "../processes/process.h"
#include <stdint.h>

#define MAX_PIPES_COUNT                                                        \
  256 // As there are 64 MAX_PROCESSES, we can have 4 pipes per process when
      // using all processes
#define PIPE_BUFFER_SIZE 256

typedef struct pipeManagerCDT *pipeManagerADT;

typedef struct pipe_t {
  uint16_t id;
  uint8_t buffer[PIPE_BUFFER_SIZE];
  sem_t *mutex;
  sem_t *read_sem;
  uint16_t last_write_pos;
  uint16_t last_read_pos;
  uint16_t to_read_count;
} pipe_t;

/**
 * Creates a pipe manager.
 */
pipeManagerADT create_pipe_manager();

pipe_t *create_pipe(pipeManagerADT pipe_manager);

void destroy_pipe(pipeManagerADT pipe_manager, pipe_t *pipe);

uint64_t write_pipe(pipeManagerADT pipe_manager, pipe_t *pipe, uint8_t *buffer,
                    uint64_t bytes);

uint64_t read_pipe(pipeManagerADT pipe_manager, pipe_t *pipe, uint8_t *buffer,
                   uint64_t bytes);

void destroy_pipe_manager(pipeManagerADT pipe_manager);

uint8_t check_pipe_id(pipeManagerADT pipe_manager, uint16_t pipe_id);

#endif