#ifndef PROCESS_H
#define PROCESS_H

#include "../include/interrupts.h"
#include "../include/lib.h"
#include "./processManager.h"

#define USERLAND_ADDRESS 0x400000
#define MAX_PROCESS_COUNT 64
#define STACK_SIZE_BYTES 4096
#define INIT_PID 0

typedef struct process_node {
  process_control_block *pcb;
  uint16_t next_index;
} process_node;

typedef void (*main_fn)(uint64_t argc, uint8_t **argv);
typedef void (*wrapper_fn)(main_fn code, uint64_t argc, uint8_t **argv);

/**
 * Creates a new process table.
 */
processManagerADT create_process_manager(schedulerADT scheduler);

/**
 * Creates a new process.
 * @param process_manager The process manager.
 * @param code The code to execute.
 * @param argv The arguments.
 * @param argc The number of arguments.
 * @param name The name of the process.
 * @param in_fg Whether the process is in the foreground.
 * @return The PID of the new process. -1 if the process could not be created.
 */
uint64_t create_process(processManagerADT pm, main_fn code, uint64_t argc,
                        uint8_t **argv, uint8_t *name, uint8_t in_fg);

void init_process(int argc, char **argv);

/**
 * Exits the current process.
 */
uint8_t exit(processManagerADT pm, uint64_t pid);

/**
 * Kills a process.
 * @param pid The PID of the process to kill.
 */
uint8_t kill(processManagerADT pm, uint64_t pid);

/**
 * Waits for all child processes to finish.
 */
void wait(processManagerADT pm, uint64_t pid);

/**
 * Blocks a process.
 * @param pid The PID of the process to block.
 */
uint8_t block(processManagerADT pm, uint64_t pid);

/**
 * Unblocks a process.
 * @param pid The PID of the process to unblock.
 */
uint8_t unblock(processManagerADT pm, uint64_t pid);

/**
 * Returns the PID of the current process.
 */
uint64_t getpid();

/**
 * Returns the PID of the parent process.
 */
uint64_t getppid();

/**
 * Sets the priority of a process.
 * @param pid The PID of the process.
 * @param priority The new priority.
 */
uint16_t set_priority(processManagerADT pm, uint64_t pid, uint8_t priority);

/**
 * Yields the CPU to the next process.
 */
void yield();

/**
 * Destroys the process table.
 */
void destroy_process_table(processManagerADT pm);

#endif