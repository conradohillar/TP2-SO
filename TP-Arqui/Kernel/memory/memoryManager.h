#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "../../processUtils.h"
#include "../include/textMode.h"

#define NULL ((void *)0)

typedef struct memoryManagerCDT *memoryManagerADT;

/**
 * @brief Creates a memory manager
 * @param memory_start_address Start address of the memory
 * @param memory_manager_address Address of the memory manager
 */
memoryManagerADT create_memory_manager(void *memory_start_address,
                                       memoryManagerADT memory_manager_address);

/**
 * @brief Allocates memory
 * @param size Size of the memory to allocate
 */
void *mm_malloc(uint64_t size);

/**
 * @brief Frees allocated memory
 * @param ptr Pointer to the memory to free
 */
void mm_free(void *ptr);

/**
 * @brief Prints the current status of the memory
 */
uint64_t mem_status();

#endif