#ifndef _KERNEL_H
#define _KERNEL_H

typedef int (*EntryPoint)();

#include <../memory/memoryManager.h>
#include <../processes/process.h>
#include <../processes/processManager.h>
#include <../processes/scheduler.h>
#include <idtLoader.h>
#include <lib.h>
#include <moduleLoader.h>
#include <stdint.h>

#endif