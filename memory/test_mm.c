// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "test_mm.h"

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

  while(1) {
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    printf("Creando...\n");

    printf("Creando...\n");

while (rq < MAX_BLOCKS && total < max_memory) {
    mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
    printf("Requesting block of size: %d\n", mm_rqs[rq].size);
    
    mm_rqs[rq].address = mm_malloc(mm_rqs[rq].size);
    printf("Allocated block at address: %p\n", mm_rqs[rq].address);
    
    if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
    } else {
        printf("Failed to allocate block of size: %d\n", mm_rqs[rq].size);
    }
}

    // Set
    printf("Seteando...\n");

    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    printf("Checkeando...\n");

    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          printf("test_mm ERROR\n");
          return -1;
        }

    // Free
    printf("Liberando...\n");

    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        mm_free(mm_rqs[i].address);
  }
}