#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_BLOCKS 300   // Total de bloques de memoria
#define BLOCK_SIZE 20     // Tamaño de cada bloque (en bytes)
#define MEM_START_ADDRESS 0x200000
#define MEM_MANAGER_ADDRESS 0x050000

typedef struct MemoryManagerCDT * MemoryManagerADT;

MemoryManagerADT freeArrayConstructor(void* memory_start_address, MemoryManagerADT memory_manager_address);

/*

*/
void * malloc(uint64_t size);

/*

*/
void free(void* ptr);
