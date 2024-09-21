#include "test_util.h"
#include "memoryManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "syscall.h"

#define MAX_BLOCKS 128

uint64_t test_mm(uint64_t argc, char *argv[]);