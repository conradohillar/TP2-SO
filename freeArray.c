#include "memoryManager.h"
#include <stdio.h>
#include <stdlib.h>

/* TEMPORAL
typedef struct MemoryManagerCDT {
    void* memory_start;   // Puntero al bloque de memoria inicial
    int free_array[TOTAL_BLOCKS];  // Array que representa los bloques libres (usamos índices)
    int free_index;  // Índice del primer bloque libre
} MemoryManagerCDT;
*/

MemoryManagerADT mem_manager;

MemoryManagerADT freeArrayConstructor(void* memory_start_address, MemoryManagerADT memory_manager_address) {
    printf("Creating...\n");
    mem_manager = (MemoryManagerADT) memory_manager_address;
    mem_manager->memory_start = memory_start_address;  // Apuntamos a la memoria contigua
    
    for (int i = 0; i < TOTAL_BLOCKS - 1; i++) {
        mem_manager->free_array[i] = i + 1;  // Cada bloque libre apunta al siguiente
    }
    mem_manager->free_array[TOTAL_BLOCKS - 1] = -1;  // El último bloque no tiene siguiente
    mem_manager->free_index = 0;  // Inicialmente, el primer bloque es el índice 0
    //printf("Created!\n");

    return mem_manager;
}

// Asignar un bloque de memoria
void* mm_malloc(size_t size) {
    //printf("Requesting size: %zu\n", size);
    //printf("Free index: %d\n", mem_manager->free_index);
    
    // Verifica que el tamaño sea válido y que haya bloques libres
    if (size > BLOCK_SIZE || mem_manager->free_index == -1) {
        perror("No hay bloques libres disponibles o el tamaño es demasiado grande.");
        return NULL;
    }

    int block_idx = mem_manager->free_index;  // Toma el índice del primer bloque libre
    mem_manager->free_index = mem_manager->free_array[block_idx];  // Actualiza el índice del próximo bloque libre

    //printf("Allocating block index: %d\n", block_idx);
    //printf("New free index: %d\n", mem_manager->free_index);

    return (void*)((char*)mem_manager->memory_start + block_idx * BLOCK_SIZE);  // Calcula y retorna la dirección del bloque asignado
}

void mm_free(void* block) {
    int block_idx = (int)(((char*)block - (char*)mem_manager->memory_start) / BLOCK_SIZE);  // Calcula el índice del bloque
    //printf("Freeing block index: %d\n", block_idx);
    mem_manager->free_array[block_idx] = mem_manager->free_index;  // Apunta al anterior primer bloque libre
    mem_manager->free_index = block_idx;  // El bloque liberado es ahora el primero libre
    //printf("New free index after free: %d\n", mem_manager->free_index);
}
