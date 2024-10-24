#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "test_mm.h"

#define TOTAL_BLOCKS 1000 // Total de bloques de memoria
#define BLOCK_SIZE 1024   // Tamaño de cada bloque (en bytes)
#define MEM_START_ADDRESS 0x200000
#define MEM_MANAGER_ADDRESS                                                    \
  0x050000 // Dirección de memoria para el Memory Manager (segun Pure64 alli
           // esta libre)
#define MEMORY_SIZE 4194304 //
typedef struct MemoryManagerCDT *MemoryManagerADT;

MemoryManagerADT *mm_init(void *mem_start_address, void *mem_manager_address,
                          uint64_t size);

/*

*/
void *mm_malloc(uint64_t size);

/*

*/
void mm_free(void *ptr);

#endif