#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "./list.h"
#include "./processManager.h"

#define NULL ((void *)0)

/**
 * Creates a new scheduler.
 */
schedulerADT create_scheduler();

process_control_block *schedule(schedulerADT scheduler);

void add_to_scheduler(schedulerADT scheduler, process_control_block *pcb);

void remove_from_scheduler(schedulerADT scheduler, process_control_block *pcb);

process_control_block *get_running(schedulerADT scheduler);

void running_ended(schedulerADT scheduler);

void destroy_scheduler(schedulerADT scheduler);

#endif