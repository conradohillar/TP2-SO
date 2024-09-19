#include "test_util.h"
#include "memoryManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "syscall.h"

#define MAX_BLOCKS 128

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv[]) {
    
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 1){
    printf("Failed 1\n");
    return -1;
  }

  if ((max_memory = satoi(argv[0])) <= 0){
    printf("Failed 2\n");
    return -1;
  }

  while (1) {
    rq = 0;
    total = 0;

    //printf("Request as many blocks as we can\n");
    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = mm_malloc(mm_rqs[rq].size);

      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    //printf("Set\n");
    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    //printf("Check\n");
    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          printf("test_mm ERROR\n");
          return -1;
        }

    //printf("Free\n");
    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        mm_free(mm_rqs[i].address);
  }
}

int main(int argc, char* argv[]) {
    //printf("BLOCK_SIZE: %d\n", BLOCK_SIZE);
    //printf("TOTAL_BLOCKS: %d\n", TOTAL_BLOCKS);
    
    uint8_t array[BLOCK_SIZE * TOTAL_BLOCKS];
    
    // Reserva memoria para el Memory Manager
    MemoryManagerADT my_mm = malloc(sizeof(MemoryManagerCDT));
    if (my_mm == NULL) {
        perror("Error al asignar memoria para el Memory Manager.");
        return -1;
    }

    // Inicializa el Memory Manager
    MemoryManagerADT mem_manager = freeArrayConstructor(&array, my_mm);
    if (mem_manager == NULL) {
        perror("Error al inicializar el Memory Manager.");
        return -1;
    }
    
    printf("Starting test:\n");
    test_mm(argc - 1, &argv[1]);

    // Limpieza
    free(my_mm);
    return 0;
}