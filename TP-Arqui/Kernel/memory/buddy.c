// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "memoryManager.h"
#include <pipes.h>
#include <scheduler.h>

extern schedulerADT my_scheduler;
extern pipeManagerADT my_pipe_manager;

#define MAX_LEVEL                                                              \
  25 // Este seria un nivel maximo hardcodeado (podria ser limitacion del nivel
     // maximo del nivel q se obtiene con el size provisto)
#define MEMORY_SIZE 4194304 //
#define MIN_LEVEL 5

typedef struct Block {
  struct Block *next;
  uint8_t level;
  uint8_t is_free;
} Block;

// Por cada nivel tengo un array de bloques, cuando quiero preguntar por un
// bloque de un tamano x, primero verifico free_list[nivel del bloque]

typedef struct memoryManagerCDT {
  uint8_t max_level;
  void *mem_start;
  Block *free_blocks_per_level[MAX_LEVEL];
  uint64_t memory_size;
} memoryManagerCDT;

memoryManagerADT mem_manager;

static Block *create_block(void *block_pointer, uint8_t level);
static void rem_block(Block *block, uint8_t level);
static void split(uint8_t index);
static Block *merge(Block *block, Block *buddy);
void print_memory_state();

memoryManagerADT
create_memory_manager(void *memory_start_address,
                      memoryManagerADT memory_manager_address) {
  mem_manager = (memoryManagerADT)memory_manager_address;
  mem_manager->mem_start = memory_start_address;
  mem_manager->memory_size = MEMORY_SIZE;

  int current_size = 2, level = 1;
  while (current_size < MEMORY_SIZE) {
    level++;
    current_size *= 2;
  }

  mem_manager->max_level = level;

  for (int i = 0; i < MAX_LEVEL; i++) {
    mem_manager->free_blocks_per_level[i] = NULL;
  }

  create_block(memory_start_address, level);
  return mem_manager;
}

void *mm_malloc(uint64_t size) {
  // Validations
  if (size <= 0 || size > MEMORY_SIZE) {
    // printf("Invalid size: %lu\n", size);
    return NULL;
  }

  // Calculate level (exponent)
  uint64_t current_size = 2, level = 1;
  while (current_size < size + sizeof(Block)) {
    level++;
    current_size *= 2;
  }

  level = (MIN_LEVEL > level) ? MIN_LEVEL : level;

  if (!mem_manager
           ->free_blocks_per_level[level]) { // Si no hay definidos bloques
                                             // libres de ese nivel
    uint8_t closest_index = 0;
    uint8_t found = 0;
    // Asigno a closest_index el nivel mayor mas cercano
    for (uint8_t i = level + 1; i <= mem_manager->max_level; i++) {
      if (mem_manager->free_blocks_per_level[i] != NULL) {
        closest_index = i;
        found = 1;
        break;
      }
    }

    if (closest_index == 0 || !found) {
      // printf("Error: No available blocks to allocate at or above level %d\n",
      // level);
      return NULL; // Salir si no hay bloques disponibles
    }

    while (closest_index > level) {

      split(closest_index);
      closest_index--;
    }
  }

  Block *new_block = mem_manager->free_blocks_per_level[level];

  rem_block(new_block, level);
  //   printf("size: %lu, block size: %d, level: %d, rel: %d\n", size, 1 <<
  //   level,
  //          level, (void *)new_block - mem_manager->mem_start);
  //   mem_status();
  return (void *)((char *)new_block + sizeof(Block));
}

void mm_free(void *ptr) {
  Block *block = (Block *)(ptr - sizeof(Block));
  //   printf("Freeing: %d, size: %d\n",
  //          (void *)ptr - mem_manager->mem_start - sizeof(Block),
  //          1 << block->level);
  if (block->is_free == 1)
    return;
  block->is_free = 1;

  uint64_t rel = (uint64_t)((void *)block - mem_manager->mem_start);
  //   printf("relative: %d\n", rel + sizeof(Block));
  // printf("%lu\n", (((uint64_t)rel) ^ (2L << block->level)));
  Block *buddy;

  // AGREGAR UN IF, QUE NO SE HAGA CUANDO EL LEVEL ES MAYOR AL MAXIMO
  buddy = (Block *)((uint64_t)mem_manager->mem_start +
                    (((uint64_t)rel) ^ ((1L << block->level))));

  //   printf("In free: buddy at: %d, level: %lu, current block level: %ld,
  //   buddy "
  //          "is_free: %d\n",
  //          (void *)buddy - mem_manager->mem_start, buddy->level,
  //          block->level, buddy->is_free);
  while (block->level < mem_manager->max_level && buddy->is_free &&
         buddy->level == block->level) {
    block = merge(block, buddy);
    block->is_free = 1;
    rel = (uint64_t)((void *)block - mem_manager->mem_start);
    // printf("relative: %d\n", rel + sizeof(Block));
    buddy = (Block *)((uint64_t)mem_manager->mem_start +
                      (((uint64_t)rel) ^ (1L << block->level)));
    // printf("\nIn while (free): buddy at: %p, level: %lu, current block level:
    // "
    //        "%ld, buddy is_free: %d\n",
    //        buddy, buddy->level, block->level, buddy->is_free);
  }
  create_block((void *)block, block->level);
  //   mem_status();
}

static Block *merge(Block *block, Block *buddy) {
  //   printf("Merging blocks at level: %ld, at: %p and %p\n", block->level,
  //   block,
  //          buddy);
  Block *leftBlock = block < buddy ? block : buddy;
  // Block *rightBlock = block < buddy ? buddy : block;

  rem_block(buddy, buddy->level);

  leftBlock->level++;
  leftBlock->is_free = 1;

  //   printf("Returned: %p, level: %ld, is_free: %d\n", leftBlock,
  //   leftBlock->level,
  //          leftBlock->is_free);
  //   printf("\n");
  //   print_memory_state();
  return leftBlock;
}

// Voy partir el bloque para aprovechar el resto del espacio
static void split(uint8_t level) {

  if (level <= 0 || level > mem_manager->max_level) {
    // printf("Error at split: level: %ld out of bounds\n", level);
    return;
  }
  if (mem_manager->free_blocks_per_level[level] == NULL) {
    // printf("Error at split: Not free blocks for level: %ld\n", level);
    return;
  }

  Block *current_block = mem_manager->free_blocks_per_level[level];
  rem_block(current_block, level);

  Block *buddy =
      (Block *)((void *)current_block +
                (1L << (level - 1))); // Calcula el puntero al bloque hermano
  create_block((void *)buddy, level - 1);
  create_block((void *)current_block,
               level - 1); // Crea y añade los bloques a la lista

  return;
}

// Elimina de la lista de bloques libres el bloque que se le pasa
void rem_block(Block *block, uint8_t level) {
  if (block == NULL) {
    // printf("Error: trying to remove a NULL block.\n");
    return;
  }

  Block *current_block = mem_manager->free_blocks_per_level[level];
  if (current_block == NULL) {
    // printf("Error in rem_block: level: %ld does not have free blocks\n",
    // level);
    return;
  }

  if (current_block == block) {
    mem_manager->free_blocks_per_level[level]->is_free = 0;
    mem_manager->free_blocks_per_level[level] =
        mem_manager->free_blocks_per_level[level]->next;
    return;
  }
  // Si llegue hasta aca: hay primero y no es el bloque

  while (current_block->next != NULL && current_block->next != block) {
    current_block = current_block->next;
  }
  if (current_block->next == NULL) {
    // printf("Error: culd not find the block in the specified level\n");
    return;
  }

  current_block->next = current_block->next->next;

  current_block->is_free = 0;
  return;
}

static Block *create_block(void *block_pointer, uint8_t level) {
  if (block_pointer == NULL) {
    // printf("Error trying to create a pointer NULL\n");
  }
  //   printf("created block at %p\n", block_pointer -
  //   mem_manager->mem_start);
  Block *new_block = (Block *)block_pointer;
  new_block->is_free = 1;
  new_block->level = level;
  new_block->next = mem_manager->free_blocks_per_level[level];
  mem_manager->free_blocks_per_level[level] = new_block;

  return new_block;
}

void mem_status(memoryManagerADT mm) {
  uint16_t fd = get_running(my_scheduler)->fds[STDOUT];
  pipe_t *pipe = get_pipe(my_pipe_manager, fd);

  // write_pipe(
  //     my_pipe_manager, pipe,
  //     "El aprendizaje continuo es clave para adaptarse a un mundo en
  //     constante " "cambio. Cada dia surgen nuevas tecnologias y herramientas
  //     que " "transforman nuestra manera de trabajar, comunicarnos y aprender.
  //     La " "curiosidad y la disposicion a adquirir conocimientos nos
  //     permiten" "mantenernos actualizados y competitivos. Al enfrentar
  //     desafios con una " "mentalidad abierta y dispuesta a aprender,
  //     incrementamos nuestras " "habilidades y ampliamos nuestras
  //     perspectivas.\n", 453);

  write_pipe(my_pipe_manager, pipe, (uint8_t *)"\nMEMORY STATE:\n", 15);
  void *null = NULL;
  for (int level = MIN_LEVEL; level <= mem_manager->max_level; level++) {
    Block *current = mem_manager->free_blocks_per_level[level];
    uint8_t aux[10];
    if (current != null) {
      itoa(level, aux);
      write_pipe(my_pipe_manager, pipe, (uint8_t *)"Level ", 7);
      write_pipe(my_pipe_manager, pipe, aux, strlen(aux));
      write_pipe(my_pipe_manager, pipe, (uint8_t *)":\n", 1);
      // Imprimir cada bloque de la lista
      while (current != null) {
        itoa((void *)current - (void *)mem_manager->mem_start, aux);
        write_pipe(my_pipe_manager, pipe,
                   (uint8_t *)"  Block at address: ", 20);
        write_pipe(my_pipe_manager, pipe, aux, strlen(aux));
        itoa((1UL << current->level), aux);
        write_pipe(my_pipe_manager, pipe, (uint8_t *)", size: ", 8);
        write_pipe(my_pipe_manager, pipe, aux, strlen(aux));
        itoa((void *)current->next - mem_manager->mem_start, aux);
        write_pipe(my_pipe_manager, pipe, (uint8_t *)", next: ", 8);
        write_pipe(my_pipe_manager, pipe, aux, strlen(aux));
        itoa(current->is_free, aux);
        write_pipe(my_pipe_manager, pipe, (uint8_t *)", is_free: ", 11);
        write_pipe(my_pipe_manager, pipe, aux, strlen(aux));
        write_pipe(my_pipe_manager, pipe, (uint8_t *)"\n", 1);

        current = current->next; // Mover al siguiente bloque
      }
    } else {
      itoa(level, aux);
      write_pipe(my_pipe_manager, pipe, (uint8_t *)"Level ", 6);
      write_pipe(my_pipe_manager, pipe, aux, strlen(aux));
      write_pipe(my_pipe_manager, pipe, (uint8_t *)":  No blocks available\n",
                 24);
    }
  }
  write_pipe(my_pipe_manager, pipe, (uint8_t *)"\n", 1);
}
