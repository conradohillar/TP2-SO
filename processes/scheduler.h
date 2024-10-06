#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

#define NULL '\0'
#define SCHEDULER_ADDRESS 0x060000

typedef SchedulerCDT *SchedulerADT;
typedef enum { READY, RUNNING, BLOCKED } process_status;

/**
 * Creates a new scheduler.
 */
SchedulerADT createScheduler();

/**
 * Returns the scheduler.
 */
SchedulerADT getScheduler();

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
void setpriority(uint64_t pid, uint8_t priority);

/**
 * Blocks a process.
 * @param pid The PID of the process to block.
 */
void block(uint64_t pid);

/**
 * Unblocks a process.
 * @param pid The PID of the process to unblock.
 */
void unblock(uint64_t pid);

/**
 * Creates a new process.
 * @param code The function to execute.
 * @param args The arguments to pass to the function.
 * @param argc The number of arguments.
 * @param name The name of the process.
 * @param priority The priority of the process.
 *@param unkillable If the process is unkillable.
 */
uint16_t createProcess(main_function code, char **args, uint64_t argc,
                       char *name, uint8_t priority, uint8_t unkillable);

#endif