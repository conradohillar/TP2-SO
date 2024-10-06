// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "memoryManager.h"

typedef struct MemoryManagerCDT {
  void *memory_start;           // Pointer to the initial memory block
  int free_array[TOTAL_BLOCKS]; // Array representing the free blocks (we use
                                // indices)
  int free_index;               // Index of the first free block
} MemoryManagerCDT;

MemoryManagerADT mem_manager;

MemoryManagerADT
free_array_constructor(void *memory_start_address,
                       MemoryManagerADT memory_manager_address) {
  printf("Creating...\n");

  if (memory_manager_address == NULL) {
    perror("Error: memory_manager_address is NULL");
    return NULL;
  }

  mem_manager = (MemoryManagerADT)memory_manager_address;
  mem_manager->memory_start =
      memory_start_address; // Point to the contiguous memory

  for (int i = 0; i < TOTAL_BLOCKS - 1; i++) {
    mem_manager->free_array[i] =
        i + 1; // Each free block points to the next one
  }
  mem_manager->free_array[TOTAL_BLOCKS - 1] =
      -1;                      // The last block has no next block
  mem_manager->free_index = 0; // Initially, the first block is index 0
  // printf("Created!\n");

  return mem_manager;
}

// Allocate a memory block
void *mm_malloc(size_t size) {
  // printf("Requesting size: %zu\n", size);
  // printf("Free index: %d\n", mem_manager->free_index);

  // Check if the size is valid and if there are free blocks
  if (size > BLOCK_SIZE || mem_manager->free_index == -1) {
    perror("No available free blocks or size is too large.");
    fprintf(stderr, "Free index: %d\n", mem_manager->free_index);
    return NULL;
  }

  int block_idx =
      mem_manager->free_index; // Take the index of the first free block
  mem_manager->free_index =
      mem_manager
          ->free_array[block_idx]; // Update the index of the next free block

  // printf("Allocating block index: %d\n", block_idx);
  // printf("New free index: %d\n", mem_manager->free_index);

  return (void *)((char *)mem_manager->memory_start +
                  block_idx * BLOCK_SIZE); // Calculate and return the address
                                           // of the allocated block
}

void mm_free(void *block) {
  int block_idx = (int)(((char *)block - (char *)mem_manager->memory_start) /
                        BLOCK_SIZE); // Calculate the index of the block
  // printf("Freeing block index: %d\n", block_idx);
  mem_manager->free_array[block_idx] =
      mem_manager->free_index; // Point to the previous first free block
  mem_manager->free_index =
      block_idx; // The freed block is now the first free block
  // printf("New free index after free: %d\n", mem_manager->free_index);
}

int main(int argc, char *argv[]) {
  // printf("BLOCK_SIZE: %d\n", BLOCK_SIZE);
  // printf("TOTAL_BLOCKS: %d\n", TOTAL_BLOCKS);

  uint8_t array[BLOCK_SIZE * TOTAL_BLOCKS];

  // Reserve memory for the Memory Manager
  MemoryManagerADT my_mm = malloc(sizeof(MemoryManagerCDT));
  if (my_mm == NULL) {
    perror("Error allocating memory for the Memory Manager.");
    return -1;
  }

  // Initialize the Memory Manager
  MemoryManagerADT mem_manager = free_array_constructor(&array, my_mm);
  if (mem_manager == NULL) {
    perror("Error initializing the Memory Manager.");
    return -1;
  }

  printf("Starting test:\n");
  test_mm(argc - 1, &argv[1]);

  // Cleanup
  free(my_mm);
  return 0;
}
