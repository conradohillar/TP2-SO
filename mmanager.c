#include <mmanager.h>

#define TOTAL_BLOCKS 300   // Total de bloques de memoria
#define BLOCK_SIZE 20     // Tamaño de cada bloque (en bytes)

// Arreglo para simular los bloques de memoria
int memory_pool[TOTAL_BLOCKS][BLOCK_SIZE];

// Array que representa la free list (almacena los índices de los bloques libres)
int free_list[TOTAL_BLOCKS];

// Índice del primer bloque libre en el array
int free_index = 0;

// Inicializar la free list con los índices de los bloques libres
void initialize_free_list() {
    for (int i = 0; i < TOTAL_BLOCKS - 1; i++) {
        free_list[i] = i + 1;  // Cada entrada apunta al siguiente bloque libre
    }
    free_list[TOTAL_BLOCKS - 1] = -1;  // El último bloque no tiene siguiente
}

// Asignar un bloque de memoria
void* allocate_block() {
    if (free_index == -1) {
        perror("No hay bloques libres disponibles.");
        return NULL;
    }

    int block_idx = free_index;     // Tomamos el índice del primer bloque libre
    free_index = free_list[block_idx];  // Actualizamos el índice del próximo bloque libre

    return memory_pool[block_idx];  // Retornamos la dirección del bloque asignado
}

// Liberar un bloque de memoria
void free_block(void* block) {
    int block_idx = (int)(((char*)block - (char*)memory_pool) / BLOCK_SIZE); // Calculamos el índice del bloque
    free_list[block_idx] = free_index;  // El bloque liberado apunta al antiguo primer libre
    free_index = block_idx;             // El bloque liberado es ahora el primero en la lista de libres
}

int main() {

    initialize_free_list();
   
}
