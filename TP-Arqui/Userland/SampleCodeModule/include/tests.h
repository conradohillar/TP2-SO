#ifndef USERLAND_TESTS_H
#define USERLAND_TESTS_H

#include "../../../processUtils.h"
#include "./syscaller.h"
#include "libc.h"

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
int64_t satoi(char *str);
void bussy_wait(uint64_t n);
void endless_loop();
void endless_loop_print(uint64_t wait);

void test_mm(uint64_t argc, uint8_t *argv[]);
uint64_t test_processes_fn(uint64_t argc, uint8_t *argv[]);
void test_prio(uint64_t argc, uint8_t *argv[]);

#endif