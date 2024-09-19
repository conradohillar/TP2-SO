#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_BLOCKS 1000   // Total de bloques de memoria
#define BLOCK_SIZE 1024     // Tamaño de cada bloque (en bytes)
#define MEM_START_ADDRESS 0x200000
#define MEM_MANAGER_ADDRESS 0x050000

typedef struct MemoryManagerCDT * MemoryManagerADT;

MemoryManagerADT freeArrayConstructor(void* memory_start_address, MemoryManagerADT memory_manager_address);

/*

*/
void * mm_malloc(uint64_t size);

/*

*/
void mm_free(void* ptr);

//TEMPORAL
typedef struct MemoryManagerCDT {
    void* memory_start;   // Puntero al bloque de memoria inicial
    int free_array[TOTAL_BLOCKS];  // Array que representa los bloques libres (usamos índices)
    int free_index;  // Índice del primer bloque libre
} MemoryManagerCDT;

#endif