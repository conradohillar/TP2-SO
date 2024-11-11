#ifndef USERLAND_TESTS_H
#define USERLAND_TESTS_H

#include "../../../processUtils.h"
#include "./syscaller.h"
#include "libc.h"

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
int64_t satoi(uint8_t *str);
void bussy_wait(uint64_t n);
void endless_loop();
int64_t endless_loop_print(uint64_t argc, uint8_t *argv[]);
void *memset(void *destination, int32_t c, uint64_t length);

int64_t test_processes_fn(uint64_t argc, uint8_t *argv[]);
int64_t test_prio_fn(uint64_t argc, uint8_t *argv[]);
int64_t test_sem_synchro_fn(uint64_t argc, uint8_t *argv[]);
int64_t test_ipc_fn(uint64_t argc, uint8_t *argv[]);
int64_t test_mm_fn(uint64_t argc, uint8_t *argv[]);

#endif