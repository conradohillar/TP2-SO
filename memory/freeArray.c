// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "memoryManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#define MAX_LEVEL 16
#define MEMORY_SIZE 65536 // 
#define MIN_LEVEL 2


typedef struct Block {
    struct Block *next;
    uint64_t level;
    uint8_t is_free;

} Block;

// Por cada nivel tengo un array de bloques, cuando quiero preguntar por un bloque de un tamano x, primero verifico
// free_list[nivel del bloque]

typedef struct MemoryManagerCDT {
    uint64_t max_level;
    void* mem_start;
    Block* free_blocks_per_level[MAX_LEVEL];
} MemoryManagerCDT;

MemoryManagerADT memory_manager;

static Block *create_block(void *block_pointer, uint64_t level, Block *next);
static void rem_block(Block *block, uint64_t level);
static void split(uint64_t index);
static Block* merge(Block* block, Block* buddy);

void mm_init(void* mem_start_address, void* mem_manager_address) {
    memory_manager = (MemoryManagerADT) mem_manager_address;
    memory_manager->mem_start = mem_manager_address;
    memory_manager->max_level = MAX_LEVEL;

    for (int i = 0; i < MAX_LEVEL; i++) {
        memory_manager->free_blocks_per_level[i] = NULL;
    }

    memory_manager->free_blocks_per_level[MAX_LEVEL] = create_block(mem_start_address, MAX_LEVEL, NULL);
    return;
}

void *mm_malloc(size_t size) {
    // Validations
    if(size <= 2 || size > MEMORY_SIZE){
        printf("Invalid size: %d\n", size);
        return NULL;
    }

    // Calculate level (exponent)
    int current_size = 2, level = 1;
    while (current_size < size) {
        level++;
        current_size *= 2;
    }
    printf("size: %d, block size: %d, level: %d\n", size, current_size, level);

    // Getting the block
    if (!memory_manager->free_blocks_per_level[level]) {    // Si no hay definidos bloques libres de ese nivel
        uint64_t closest_index = 0;
        uint8_t found = 0;
        //printf("No hay bloques libres en ese nivel\n");
        // Asigno a closest_index el nivel mayor mas cercano
        for (uint64_t i = level + 1; i <= MAX_LEVEL; i++) {
            if (memory_manager->free_blocks_per_level[i] != NULL) {
                closest_index = i;
                found = 1;
                break;
            }
        }

        //printf("Voy a tomar el nivel: %d\n", closest_index);

        if (closest_index == 0) {
            printf("Error: No available blocks to allocate at or above level %lu\n", level);
            return NULL;  // Salir si no hay bloques disponibles
        }
        // Ahora dividimos los bloques hasta llegar al nivel adecuado
        while (closest_index > level) {

            //printf("Splitting at index: %d, goal: %d\n", closest_index, level);
            split(closest_index);
            closest_index--;
        }
    }

    Block *new_block = memory_manager->free_blocks_per_level[level];
    new_block->is_free = 0;
    //printf("Borrando de la lista el bloque tomado: %p\n", new_block);
    rem_block(new_block, level);
    return (void *)((char *)new_block + sizeof(Block));
}




void mm_free(void *ptr) {

}

static Block* merge(Block* block, Block* buddy){

}


// Voy partir el bloque para aprovechar el resto del espacio
static void split(uint64_t level) {
    //printf("Splitting...\n");
    if(level <= 0 || level > MAX_LEVEL){
        printf("Error at split: level: %d out of bounds\n", level);
        return;
    }
    if(memory_manager->free_blocks_per_level[level] == NULL){
        printf("Error at split: Not free blocks for level: %d\n", level);
        return;
    }

    //printf("About to open the block at %d\n", memory_manager->free_blocks_per_level[level]);
    Block* current_block = memory_manager->free_blocks_per_level[level];
    rem_block(current_block, level);
    //printf("Opened the block in the level");
    //fflush(stdout);
    Block *buddy = (Block *) ((void *) current_block + (1L << (level))); // Calcula el puntero al bloque hermano
    create_block((void *) current_block, level - 1, buddy);  // Crea y añade los bloques a la lista
    create_block((void *) buddy, level - 1, memory_manager->free_blocks_per_level[level - 1]);
    //printf("Splitting block at index %lu, block address: %p, buddy address: %p\n", level, current_block, buddy);
    //print_memory_state();
    return;
}




// Elimina de la lista de bloques libres el bloque que se le pasa
void rem_block(Block *block, uint64_t level) {
    if(block == NULL){
        printf("Error: trying to remove a NULL block.\n");
        return;
    }
    
    Block* current_block = memory_manager->free_blocks_per_level[level];
    if(current_block == NULL){
        printf("Error in rem_block: level: %d does not have free blocks\n", level);
        return;
    }
    
    if(current_block == block){
        //printf("tendria que entrar aca\n");
        memory_manager->free_blocks_per_level[level]->is_free = 0;
        memory_manager->free_blocks_per_level[level] = memory_manager->free_blocks_per_level[level]->next;
        return;
    }
    // Si llegue hasta aca: hay primero y no es el bloque

    while(current_block->next != NULL && current_block->next != block){
        current_block = current_block->next;
    }
    if(current_block->next == NULL){
        printf("Error: culd not find the block in the specified level\n");
        return;
    }

    current_block->next = current_block->next->next;

    current_block->is_free = 0;
    return;
}





static Block *create_block(void *block_pointer, uint64_t level, Block *next) {
    Block* current_block = memory_manager->free_blocks_per_level[level];
    if(current_block != NULL){
        while(current_block->next = NULL){
            current_block = current_block->next;
        }
    }

    Block* new_block = (Block*) block_pointer;
    new_block->is_free = 1;
    new_block->next = next;
    new_block->level = level;
    memory_manager->free_blocks_per_level[level] = new_block;

    return new_block;
}

void print_memory_state() {
    printf("\nMemory State:\n");
    for (int level = 0; level <= MAX_LEVEL; level++) {
        Block *current = memory_manager->free_blocks_per_level[level];
        if (current != NULL) {
            printf("Level %d:\n", level);
            // Imprimir cada bloque de la lista
            while (current != NULL) {
                printf("  Block at address: %p, size: %lu, next: %p\n",
                       (void *)current, 
                       (1UL << current->level),
                       (void *)current->next);
                current = current->next; // Mover al siguiente bloque
            }
        } else {
            printf("Level %d: No blocks available\n", level);
        }
    }
}

int main(int argc, char *argv[]) {
    
    MemoryManagerADT my_mm = malloc(sizeof(MemoryManagerCDT));
    Block* blocks = malloc(MEMORY_SIZE * 2);

    mm_init(blocks, my_mm);


    printf("Created (%p - %p)\n", blocks, blocks + MEMORY_SIZE);
    print_memory_state();

    // Prueba de asignación y liberación
    void *p1 = mm_malloc(120);
    printf("Passed malloc 1 at: %p\n", p1);
    print_memory_state();
    fflush(stdout);

    void *p2 = mm_malloc(569);
    printf("Passed malloc 2 at: %p\n", p2);
    print_memory_state();
    void *p3 = mm_malloc(1010);
    printf("Passed malloc 3\n");
    print_memory_state();
    

    //mm_free(p1);
    //mm_free(p2);
    //mm_free(p3);
    
    printf("Success\n");
    return 0;

    //mm_init();
    printf("Starting test:\n");
    test_mm(argc - 1, &argv[1]);

    return 0;
}