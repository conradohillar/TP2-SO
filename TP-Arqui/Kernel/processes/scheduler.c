// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "scheduler.h"
#include "../include/textMode.h"
#include "../memory/memoryManager.h"
#include "process.h"

typedef struct schedulerCDT {
  listADT list;
  scheduler_data *running;
} schedulerCDT;

schedulerADT create_scheduler() {

  schedulerADT scheduler = mm_malloc(sizeof(schedulerCDT));
  if (scheduler == NULL) {
    return NULL;
  }

  scheduler->list = create_list();
  if (scheduler->list == NULL) {
    mm_free(scheduler);
    return NULL;
  }

  return scheduler;
}

void add_to_scheduler(schedulerADT scheduler, process_control_block *pcb) {

  scheduler_data *new_node = mm_malloc(sizeof(scheduler_data));
  if (new_node == NULL) {
    put_string_nt("Error adding to scheduler\n", 0xFF0000, 0x000000);
    return;
  }

  new_node->pcb = pcb;
  new_node->remaining_quantum = pcb->priority;

  if (!add_to_list(scheduler->list, new_node)) {
    put_string_nt("Error adding to scheduler\n", 0xFF0000, 0x000000);
    mm_free(new_node);
  }
}

void remove_from_scheduler(schedulerADT scheduler, process_control_block *pcb) {
  remove_from_list(scheduler->list, pcb);
  if (scheduler->running->pcb == pcb) {
    scheduler->running = list_next(scheduler->list);
    scheduler->running->pcb->status = RUNNING;
  }
}

scheduler_data *schedule(schedulerADT scheduler) {
  if (is_empty(scheduler->list)) {
    return NULL;
  }

  if (scheduler->running == NULL) {
    scheduler->running = list_next(scheduler->list);
    scheduler->running->pcb->status = RUNNING;
    scheduler->running->remaining_quantum--;
    return scheduler->running;
  }

  if (scheduler->running->remaining_quantum == 0) {
    scheduler->running->remaining_quantum = scheduler->running->pcb->priority;
    scheduler->running = list_next(scheduler->list);
    scheduler->running->pcb->status = RUNNING;
  }

  scheduler->running->remaining_quantum--;
  return scheduler->running;
}

uint64_t context_switch(schedulerADT scheduler, uint64_t stack_pointer) {
  if (scheduler->running != NULL) {
    scheduler->running->pcb->stack_pointer = (void *)stack_pointer;
  }

  scheduler_data *next_process = schedule(scheduler);

  return (uint64_t)next_process->pcb->stack_pointer;
}

void destroy_scheduler(schedulerADT scheduler) {
  free_list(scheduler->list);
  mm_free(scheduler);
}