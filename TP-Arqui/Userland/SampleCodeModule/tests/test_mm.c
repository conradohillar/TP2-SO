// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include <libc.h>
#include <tests.h>

#define MAX_BLOCKS 128

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

int64_t test_mm_fn(uint64_t argc, uint8_t *argv[]) {

  uint64_t total_allocated_blocks = 0;

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  if (argc == 2) {
    sys_set_fd_asm(STDOUT, satoi(argv[1]));
  }

  if ((max_memory = satoi((uint8_t *)argv[0])) <= 0) {
    printerr(
        (uint8_t
             *)"ERROR in test_mm: max_memory must be a value greater than 0\n");
    return -1;
  }

  while (1) {

    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      if (total_allocated_blocks % 100000 == 0) {
        printcolor((uint8_t *)"Allocated blocks: ", 0x00FF00, 0x000000);
        uint8_t num[20] = {0};
        itoa(total_allocated_blocks, num);
        printcolor((uint8_t *)num, 0x00FF00, 0x000000);
        printcolor((uint8_t *)"\n", 0x00FF00, 0x000000);
        itoa((uint64_t)mm_rqs[rq].address, num);
        printcolor((uint8_t *)"Last address: ", 0x00FF00, 0x000000);
        printcolor((uint8_t *)num, 0x00FF00, 0x000000);
        printcolor((uint8_t *)"\n\n", 0x00FF00, 0x000000);
      }
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = sys_mm_malloc_asm(mm_rqs[rq].size);

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
          printerr((uint8_t *)"ERROR in test_mm: memcheck failed\n");
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        sys_mm_free_asm(mm_rqs[i].address);
  }

  if (argc == 2) {
    print((uint8_t *)"\0");
    sys_set_fd_asm(STDOUT, STDOUT);
  }

  return 0;
}