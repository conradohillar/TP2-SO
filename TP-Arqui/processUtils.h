#ifndef PROCESSUTILS_H
#define PROCESSUTILS_H

#include <stdint.h>

typedef enum { READY, RUNNING, BLOCKED, KILLED, ZOMBIE } process_status;

typedef struct ps_info {
  uint64_t pid;
  uint8_t *name;
  uint64_t sp;
  uint64_t bp;
  process_status state;
  uint64_t priority;
  uint64_t in_fg;
} ps_info;

typedef struct ps_struct {
  ps_info *info;
  uint64_t count;
} ps_struct;

typedef struct mem_info {
  uint64_t mem_start_address;
  uint64_t total_mem;
  uint64_t used_mem;
  uint64_t free_mem;
} mem_info;

typedef uint8_t semaphore;

#endif