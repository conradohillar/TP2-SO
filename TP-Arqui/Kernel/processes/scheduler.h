#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "./processManager.h"

#define NULL '\0'

typedef struct scheduler_node {
  process_control_block *pcb;
  struct scheduler_node *next;
  uint64_t remaining_quantum;
} scheduler_node;

/**
 * Creates a new scheduler.
 */
schedulerADT create_scheduler();

scheduler_node *schedule(schedulerADT scheduler);

void add_to_scheduler(schedulerADT scheduler, process_control_block *pcb);

void remove_from_scheduler(schedulerADT scheduler, process_control_block *pcb);

#endif