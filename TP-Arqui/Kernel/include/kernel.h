#ifndef _KERNEL_H
#define _KERNEL_H

typedef int (*EntryPoint)();

#include <idtLoader.h>
#include <lib.h>
#include <memoryManager.h>
#include <moduleLoader.h>
#include <pipes.h>
#include <process.h>
#include <processManager.h>
#include <scheduler.h>
#include <sempahores.h>
#include <stdint.h>

#endif