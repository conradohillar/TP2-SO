#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "./processManager.h"

#define NULL '\0'

/**
 * Creates a new scheduler.
 */
schedulerADT create_scheduler();

uint64_t schedule(uint64_t prev_sp);

void add_to_scheduler(schedulerADT scheduler, process_control_block *pcb);

void remove_from_scheduler(uint64_t pid);

#endif