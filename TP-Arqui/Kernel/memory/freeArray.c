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

#define TOTAL_BLOCKS 750
#define BLOCK_SIZE 7000

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
  uint16_t fd = get_running(my_scheduler)->fds[STDOUT];
  pipe_t *pipe = get_pipe(my_pipe_manager, fd);

  uint64_t counter = 0;
  for (int64_t i = mem_manager->free_index; i != -1;) {
    counter++;
    i = mem_manager->free_array[i];
  }

  uint8_t aux[10];
  itoa(counter, aux);
  write_pipe(my_pipe_manager, pipe, (uint8_t *)"There are ", 10);
  write_pipe(my_pipe_manager, pipe, aux, strlen(aux));
  write_pipe(my_pipe_manager, pipe, (uint8_t *)" of ", 4);
  itoa(TOTAL_BLOCKS, aux);
  write_pipe(my_pipe_manager, pipe, aux, strlen(aux));
  write_pipe(my_pipe_manager, pipe,
             (uint8_t *)" free blocks available with size ", 34);
  itoa(BLOCK_SIZE, aux);
  write_pipe(my_pipe_manager, pipe, aux, strlen(aux));
  write_pipe(my_pipe_manager, pipe, (uint8_t *)" each\n", 6);
}