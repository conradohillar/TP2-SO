// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "memoryManager.h"
#include <pipes.h>
#include <scheduler.h>

extern schedulerADT my_scheduler;
extern pipeManagerADT my_pipe_manager;
extern memoryManagerADT mem_manager;

#define TOTAL_BLOCKS 750
#define BLOCK_SIZE 16384

typedef struct memoryManagerCDT {
  void *memory_start;           // Puntero al bloque de memoria inicial
  int free_array[TOTAL_BLOCKS]; // Array que representa los bloques libres
                                // (usamos índices)
  int free_index;               // Índice del primer bloque libre
} memoryManagerCDT;

memoryManagerADT mem_manager;

memoryManagerADT
create_memory_manager(void *memory_start_address,
                      memoryManagerADT memory_manager_address) {

  if (memory_manager_address == NULL) {
    return NULL;
  }

  mem_manager = (memoryManagerADT)memory_manager_address;
  mem_manager->memory_start =
      memory_start_address; // Apuntamos a la memoria contigua

  for (int i = 0; i < TOTAL_BLOCKS - 1; i++) {
    mem_manager->free_array[i] = i + 1; // Cada bloque libre apunta al siguiente
  }
  mem_manager->free_array[TOTAL_BLOCKS - 1] =
      -1;                      // El último bloque no tiene siguiente
  mem_manager->free_index = 0; // Inicialmente, el primer bloque es el índice 0

  return mem_manager;
}

// Asignar un bloque de memoria
void *mm_malloc(uint64_t size) {

  // Verifica que el tamaño sea válido y que haya bloques libres
  if (size > BLOCK_SIZE ||
      mem_manager->free_array[mem_manager->free_index] == -1) {
    return NULL;
  }
  int block_idx =
      mem_manager->free_index; // Toma el índice del primer bloque libre
  mem_manager->free_index =
      mem_manager->free_array[block_idx]; // Actualiza el índice del próximo
                                          // bloque libre

  return (void *)((char *)mem_manager->memory_start +
                  block_idx * BLOCK_SIZE); // Calcula y retorna la dirección del
                                           // bloque asignado
}

void mm_free(void *block) {
  int block_idx = (int)(((char *)block - (char *)mem_manager->memory_start) /
                        BLOCK_SIZE); // Calcula el índice del bloque
  mem_manager->free_array[block_idx] =
      mem_manager->free_index; // Apunta al anterior primer bloque libre
  mem_manager->free_index =
      block_idx; // El bloque liberado es ahora el primero libre
}

uint64_t mem_status() {
  mem_info *info = (mem_info *)mm_malloc(sizeof(mem_info));
  if (info == NULL) {
    return 0;
  }
  info->mem_start_address = (uint64_t)mem_manager->memory_start;
  info->total_mem = TOTAL_BLOCKS * BLOCK_SIZE;
  int64_t free_blocks = 0;

  int i = mem_manager->free_index;
  while (i != -1) {
    free_blocks++;
    i = mem_manager->free_array[i];
  }

  info->free_mem = free_blocks * BLOCK_SIZE;
  info->used_mem = (TOTAL_BLOCKS - free_blocks) * BLOCK_SIZE;
  info->memory_manager = (uint8_t *)"FreeArray";
  return (uint64_t)info;
}