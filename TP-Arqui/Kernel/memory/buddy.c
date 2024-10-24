// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "memoryManager.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEVEL                                                              \
  25 // Este seria un nivel maximo hardcodeado (podria ser limitacion del nivel
     // maximo del nivel q se obtiene con el size provisto)

#define MIN_LEVEL 5

typedef struct Block {
  struct Block *next;
  uint8_t level;
  uint8_t is_free;
} Block;

// Por cada nivel tengo un array de bloques, cuando quiero preguntar por un
// bloque de un tamano x, primero verifico free_list[nivel del bloque]

typedef struct MemoryManagerCDT {
  uint8_t max_level;
  void *mem_start;
  Block *free_blocks_per_level[MAX_LEVEL];
  uint64_t memory_size;
} MemoryManagerCDT;

MemoryManagerADT memory_manager;

static Block *create_block(void *block_pointer, uint8_t level);
static void rem_block(Block *block, uint8_t level);
static void split(uint8_t index);
static Block *merge(Block *block, Block *buddy);

MemoryManagerADT *mm_init(void *mem_start_address, void *mem_manager_address,
             uint64_t size) {
  memory_manager = (MemoryManagerADT)mem_manager_address;
  memory_manager->mem_start = mem_start_address;
  memory_manager->memory_size = size;

  uint64_t current_size = 2, level = 1;
  while (current_size < size) {
    level++;
    current_size *= 2;
  }

  memory_manager->max_level = level;

  for (uint8_t i = 0; i < MAX_LEVEL; i++) {
    memory_manager->free_blocks_per_level[i] = NULL;
  }

  create_block(mem_start_address, level);
  return memory_manager;
}

void *mm_malloc(size_t size) {
  // Validations
  if (size <= 0 || size > MEMORY_SIZE) {
    return NULL;
  }

  // Calculate level (exponent)
  uint64_t current_size = 2, level = 1;
  while (current_size < size + sizeof(Block)) {
    level++;
    current_size *= 2;
  }

  level = (MIN_LEVEL > level) ? MIN_LEVEL : level;

  if (!memory_manager
           ->free_blocks_per_level[level]) { // Si no hay definidos bloques
                                             // libres de ese nivel
    uint8_t closest_index = 0;
    uint8_t found = 0;
    // Asigno a closest_index el nivel mayor mas cercano
    for (uint8_t i = level + 1; i <= memory_manager->max_level; i++) {
      if (memory_manager->free_blocks_per_level[i] != NULL) {
        closest_index = i;
        found = 1;
        break;
      }
    }

    if (closest_index == 0 || !found) {
      return NULL; // Salir si no hay bloques disponibles
    }

    while (closest_index > level) {

      split(closest_index);
      closest_index--;
    }
  }

  Block *new_block = memory_manager->free_blocks_per_level[level];

  rem_block(new_block, level);
  return (void *)((char *)new_block + sizeof(Block));
}

void mm_free(void *ptr) {
  Block *block = (Block *)(ptr - sizeof(Block));
  if (block->is_free == 1)
    return;
  block->is_free = 1;

  uint64_t rel = (uint64_t)((void *)block - memory_manager->mem_start);
  Block *buddy;

  buddy = (Block *)((uint64_t)memory_manager->mem_start +
                    (((uint64_t)rel) ^ ((1L << block->level))));
  while (block->level < memory_manager->max_level && buddy->is_free &&
         buddy->level == block->level) {
    block = merge(block, buddy);
    block->is_free = 1;
    rel = (uint64_t)((void *)block - memory_manager->mem_start);
    buddy = (Block *)((uint64_t)memory_manager->mem_start +
                      (((uint64_t)rel) ^ (1L << block->level)));
  }
  create_block((void *)block, block->level);
}

static Block *merge(Block *block, Block *buddy) {
  Block *leftBlock = block < buddy ? block : buddy;
  Block *rightBlock = block < buddy ? buddy : block;

  rem_block(buddy, buddy->level);

  leftBlock->level++;
  leftBlock->is_free = 1;
  return leftBlock;
}

static void split(uint8_t level) {

  if (level <= 0 || level > memory_manager->max_level) {
    return;
  }
  if (memory_manager->free_blocks_per_level[level] == NULL) {
    return;
  }

  Block *current_block = memory_manager->free_blocks_per_level[level];
  rem_block(current_block, level);

  Block *buddy = (Block *)((void *)current_block + (1L << (level - 1)));
  create_block((void *)buddy, level - 1);
  create_block((void *)current_block, level - 1);

  return;
}

void rem_block(Block *block, uint8_t level) {
  if (block == NULL) {
    return;
  }

  Block *current_block = memory_manager->free_blocks_per_level[level];
  if (current_block == NULL) {
    return;
  }

  if (current_block == block) {
    memory_manager->free_blocks_per_level[level]->is_free = 0;
    memory_manager->free_blocks_per_level[level] =
        memory_manager->free_blocks_per_level[level]->next;
    return;
  }

  while (current_block->next != NULL && current_block->next != block) {
    current_block = current_block->next;
  }
  if (current_block->next == NULL) {
    return;
  }

  current_block->next = current_block->next->next;

  current_block->is_free = 0;
  return;
}

static Block *create_block(void *block_pointer, uint8_t level) {
  if (block_pointer == NULL) {
    return NULL;
  }
  Block *new_block = (Block *)block_pointer;
  new_block->is_free = 1;
  new_block->level = level;
  new_block->next = memory_manager->free_blocks_per_level[level];
  memory_manager->free_blocks_per_level[level] = new_block;

  return new_block;
}
