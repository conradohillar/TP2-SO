#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "test_mm.h"
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_BLOCKS 1000   // Total de bloques de memoria
#define BLOCK_SIZE 1024     // Tamaño de cada bloque (en bytes)
#define MEM_START_ADDRESS 0x200000
#define MEM_MANAGER_ADDRESS 0x050000

typedef struct MemoryManagerCDT * MemoryManagerADT;

/*

*/
void * mm_malloc(uint64_t size);

/*

*/
void mm_free(void* ptr);

#endif