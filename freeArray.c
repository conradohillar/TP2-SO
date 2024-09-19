#include "memoryManager.h"

typedef struct MemoryManagerCDT {
    void** memory_start;   // Cambiado a puntero a puntero

    // Array que representa la free list (almacena los indices de los bloques libres)
    int free_array[TOTAL_BLOCKS];  // Solo necesitamos almacenar índices, así que puede ser un int.
    
    // Índice del primer bloque libre en el array
    int free_index;

} MemoryManagerCDT;

MemoryManagerADT mem_manager;

MemoryManagerADT freeArrayConstructor(void* memory_start_address, MemoryManagerADT memory_manager_address) {
    mem_manager = (MemoryManagerADT) memory_manager_address;

    // Apuntamos el inicio de la memoria a una dirección definida por el parámetro memory_start_address
    mem_manager->memory_start = (void**)memory_start_address; 
    
    for (int i = 0; i < TOTAL_BLOCKS - 1; i++) {
        mem_manager->free_array[i] = i + 1;  // Cada entrada apunta al siguiente bloque libre
    }
    mem_manager->free_array[TOTAL_BLOCKS - 1] = -1;  // El último bloque no tiene siguiente
    
    mem_manager->free_index = 0;

    return mem_manager;
}

// Asignar un bloque de memoria
void* malloc(size_t size) {
    if (mem_manager->free_index == -1) {
        perror("No hay bloques libres disponibles.");
        return NULL;
    }

    int block_idx = mem_manager->free_index;  // Tomamos el índice del primer bloque libre
    mem_manager->free_index = mem_manager->free_array[block_idx];  // Actualizamos el índice del próximo bloque libre

    return (void*)((char*)mem_manager->memory_start + block_idx * BLOCK_SIZE);  // Retornamos la dirección del bloque asignado
}

// Liberar un bloque de memoria
void free(void* block) {
    int block_idx = (int)(((char*)block - (char*)mem_manager->memory_start) / BLOCK_SIZE);  // Calculamos el índice del bloque
    mem_manager->free_array[block_idx] = mem_manager->free_index;  // El bloque liberado apunta al antiguo primer libre
    mem_manager->free_index = block_idx;  // El bloque liberado es ahora el primero en la lista de libres
}

int main(){
    return 0;
}

/*#include "memoryManager.h"

typedef struct MemoryManagerCDT {
    void* memory_start[TOTAL_BLOCKS][BLOCK_SIZE];

    // Array que representa la free list (almacena los indices de los bloques libres)
    void* free_array[TOTAL_BLOCKS];
    
    // Índice del primer bloque libre en el array
    int free_index;

    //Information about the memory manager
    // uint64_t freeBlocks;
    // uint64_t usedBlocks;
    // uint64_t blockCount;
    // uint64_t blockSize;
    // uint64_t freeMemory;
    // uint64_t usedMemory;
    // uint64_t totalMemory;

} MemoryManagerCDT;

MemoryManagerADT mem_manager;

MemoryManagerADT freeArrayConstructor(void* memory_start_address, MemoryManagerADT memory_manager_address) {
    mem_manager = (MemoryManagerADT) memory_manager_address;

    *(mem_manager).memory_start = memory_start_address;   // Apuntamos el incio de la memoria a una direccion definida por el parametro memstart.
    
    for (int i = 0; i < TOTAL_BLOCKS - 1; i++) {
        mem_manager->free_array[i] = i + 1;     // Cada entrada apunta al siguiente bloque libre
    }
    mem_manager->free_array[TOTAL_BLOCKS - 1] = -1;  // El último bloque no tiene siguiente
    
    mem_manager->free_index = 0;
}

// Asignar un bloque de memoria
void* malloc(size_t size) {
    if (mem_manager->free_index == -1) {
        perror("No hay bloques libres disponibles.");
        return NULL;
    }

    int block_idx = mem_manager->free_index;     // Tomamos el índice del primer bloque libre
    mem_manager->free_index = mem_manager->free_array[block_idx];  // Actualizamos el índice del próximo bloque libre

    return (*(mem_manager->memory_start))[block_idx];  // Retornamos la dirección del bloque asignado
}

// Liberar un bloque de memoria
void free(void* block) {
    int block_idx = (int)(((char*)block - (char*)*(mem_manager->memory_start)) / BLOCK_SIZE); // Calculamos el índice del bloque
    mem_manager->free_array[block_idx] = mem_manager->free_index;  // El bloque liberado apunta al antiguo primer libre
    mem_manager->free_index = block_idx;             // El bloque liberado es ahora el primero en la lista de libres
}
*/