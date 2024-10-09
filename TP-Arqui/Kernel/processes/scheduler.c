// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "scheduler.h"
#include "../include/textMode.h"
#include "../memory/memoryManager.h"
#include "process.h"

typedef struct scheduler_node {
  process_control_block *pcb;
  uint64_t tail_index;
  uint64_t remaining_quantum;
} scheduler_node;

typedef struct schedulerCDT {
  scheduler_node unblocked_processes[MAX_PROCESS_COUNT];
  uint64_t next_index; // Next PID to assign (corresponds to the index
                       // of the next available PCB in unblocked_processes)
  uint64_t current_index;
  uint64_t unblocked_process_count;
} schedulerCDT;

schedulerADT create_scheduler() {

  schedulerADT scheduler = mm_malloc(sizeof(schedulerCDT));
  if (scheduler == NULL) {
    return NULL;
  }

  for (int i = 0; i < MAX_PROCESS_COUNT - 1; i++) {
    scheduler->unblocked_processes[i].tail_index = i + 1;
    scheduler->unblocked_processes[i].remaining_quantum = 0;
    scheduler->unblocked_processes[i].pcb = NULL;
  }

  scheduler->unblocked_processes[MAX_PROCESS_COUNT - 1].tail_index = -1;
  scheduler->next_index = 0;
  scheduler->current_index = -1;
  scheduler->unblocked_process_count = 0;

  return scheduler;
}

void add_to_scheduler(schedulerADT scheduler, process_control_block *pcb) {
  scheduler->unblocked_processes[scheduler->next_index].pcb = pcb;

  scheduler->unblocked_processes[scheduler->next_index].remaining_quantum =
      pcb->priority;

  scheduler->current_index = scheduler->next_index;

  scheduler->unblocked_processes[scheduler->next_index].tail_index =
      scheduler->unblocked_processes[scheduler->current_index].tail_index;
}

void remove_from_scheduler(schedulerADT scheduler, process_control_block *pcb) {
  for (int i = 0; i < scheduler->unblocked_process_count; i++) {
    if (scheduler->unblocked_processes[i].pcb == pcb) {
      scheduler->next_index = scheduler->current_index;
      scheduler->current_index = i;
    }
  }
}

uint64_t schedule(uint64_t prev_sp) {}