// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "test_mm.h"
#include "../include/textMode.h"

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

uint64_t total_allocated_blocks = 0;

uint64_t test_mm(uint64_t argc, char *argv[]) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc != 1) {
    put_string_nt((uint8_t *)"Failed 1\n", 0xFF0000, 0x000000);
    return -1;
  }

  if ((max_memory = satoi(argv[0])) <= 0) {
    put_string_nt((uint8_t *)"Failed 2\n", 0xFF0000, 0x000000);
    return -1;
  }

  while (1) {

    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      if (total_allocated_blocks % 500000 == 0) {
        put_string_nt((uint8_t *)"Allocated blocks: ", 0x00FF00, 0x000000);
        uint8_t num[20] = {0};
        itoa(total_allocated_blocks, num);
        put_string_nt((uint8_t *)num, 0x00FF00, 0x000000);
        put_string_nt((uint8_t *)"\n", 0x00FF00, 0x000000);
        itoa(mm_rqs[rq].address, num);
        put_string_nt("last address: ", 0x00FF00, 0x000000);
        put_string_nt((uint8_t *)num, 0x00FF00, 0x000000);
        put_string_nt((uint8_t *)"\n", 0x00FF00, 0x000000);
      }
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = mm_malloc(mm_rqs[rq].size);

      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
        total_allocated_blocks++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          put_string_nt((uint8_t *)"test_mm ERROR\n", 0xFF0000, 0x000000);
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        mm_free(mm_rqs[i].address);
  }
}