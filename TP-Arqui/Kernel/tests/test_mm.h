#include "../memory/memoryManager.h"
#include "syscall.h"
#include "test_util.h"

#define MAX_BLOCKS 128

uint64_t test_mm(uint64_t argc, uint8_t *argv[]);