#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "./list.h"
#include "./processManager.h"

#define NULL ((void *)0)

typedef struct scheduler_data {
  process_control_block *pcb;
  uint64_t remaining_quantum;
} scheduler_data;

/**
 * Creates a new scheduler.
 */
schedulerADT create_scheduler();

scheduler_data *schedule(schedulerADT scheduler);

void add_to_scheduler(schedulerADT scheduler, process_control_block *pcb);

void remove_from_scheduler(schedulerADT scheduler, process_control_block *pcb);

void destroy_scheduler(schedulerADT scheduler);

#endif