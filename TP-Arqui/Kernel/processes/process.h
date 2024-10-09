#ifndef PROCESS_H
#define PROCESS_H

#include "./processManager.h"

#define MAX_PROCESS_COUNT 64
#define STACK_SIZE_BYTES 4096
#define INIT_PID 0

typedef process_control_block *process_table[MAX_PROCESS_COUNT];

typedef void (*main_function)(int argc, char **argv);

/**
 * Creates a new process table.
 */
processManagerADT create_process_manager();

/**
 * Creates a new process.
 * @param code The function to execute.
 * @param args The arguments to pass to the function.
 * @param argc The number of arguments.
 * @param name The name of the process.
 * @param priority The priority of the process.
 *@param unkillable If the process is unkillable.
 */
uint16_t create_process(schedulerADT scheduler,
                        processManagerADT process_manager, main_function code,
                        char **argv, uint64_t argc, char *name, uint64_t ppid,
                        uint8_t priority, uint8_t killable, uint8_t in_fg);

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
uint16_t setpriority(processManagerADT pm, uint64_t pid, uint8_t priority);

/**
 * Blocks a process.
 * @param pid The PID of the process to block.
 */
uint16_t block(processManagerADT pm, schedulerADT scheduler, uint64_t pid);

/**
 * Unblocks a process.
 * @param pid The PID of the process to unblock.
 */
uint16_t unblock(processManagerADT pm, schedulerADT scheduler, uint64_t pid);

/**
 * Kills a process.
 * @param pid The PID of the process to kill.
 */
uint8_t kill(processManagerADT pm, uint64_t pid, schedulerADT scheduler);

/**
 * Waits for all child processes to finish.
 */
void wait(processManagerADT pm, uint64_t pid, schedulerADT scheduler);

/**
 * Yields the CPU to the next process.
 */
void yield();

#endif