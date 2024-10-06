#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "test_mm.h"
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_BLOCKS 1000 // Total amount of blocks
#define BLOCK_SIZE 4096   // Block size in bytes
#define MEM_START_ADDRESS 0x200000
#define MEM_MANAGER_ADDRESS 0x050000

typedef struct MemoryManagerCDT *MemoryManagerADT;

/**
 * Allocates a block of memory of the specified size.
 *
 * @param size The size of the memory block to allocate.
 * @return A pointer to the allocated memory block, or NULL if there are no
 * available blocks or the size is too large.
 */
void *mm_malloc(uint64_t size);

/**
 * Frees a previously allocated memory block.
 *
 * @param ptr A pointer to the memory block to free.
 */
void mm_free(void *ptr);

#endif