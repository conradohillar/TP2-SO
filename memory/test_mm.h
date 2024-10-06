#include "memoryManager.h"
#include "syscall.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BLOCKS 128

uint64_t test_mm(uint64_t argc, char *argv[]);