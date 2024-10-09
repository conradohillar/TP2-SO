#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "../tests/test_mm.h"

#define TOTAL_BLOCKS 1000 // Total de bloques de memoria
#define BLOCK_SIZE 2048   // Tamaño de cada bloque (en bytes)

typedef struct MemoryManagerCDT *MemoryManagerADT;

/*

*/
void *mm_malloc(uint64_t size);

/*

*/
void mm_free(void *ptr);

#endif