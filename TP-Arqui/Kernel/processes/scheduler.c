// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "scheduler.h"
#include "../include/textMode.h"
#include "../memory/memoryManager.h"
#include "process.h"

typedef struct schedulerCDT {
  scheduler_node *first;
  scheduler_node *last;
  scheduler_node *running;
  uint64_t size;
} schedulerCDT;

schedulerADT create_scheduler() {

  schedulerADT scheduler = mm_malloc(sizeof(schedulerCDT));
  if (scheduler == NULL) {
    return NULL;
  }

  scheduler->first = NULL;
  scheduler->last = NULL;
  scheduler->running = NULL;
  scheduler->size = 0;

  return scheduler;
}

void add_to_scheduler(schedulerADT scheduler, process_control_block *pcb) {

  scheduler_node *new_node = mm_malloc(sizeof(scheduler_node));
  if (new_node == NULL) {
    return;
  }

  new_node->pcb = pcb;
  new_node->remaining_quantum = pcb->priority;
  new_node->next = scheduler->first;
  scheduler->last = new_node;
  scheduler->size++;

  if (scheduler->size == 0) {
    scheduler->first = new_node;
  } else if (scheduler->size == 1) {
    scheduler->first->next = new_node;
  }
}

void remove_from_scheduler(schedulerADT scheduler, process_control_block *pcb) {
  scheduler_node *current = scheduler->first;
  scheduler_node *prev = NULL;

  while (prev != scheduler->last) {
    if (current->pcb == pcb) {
      if (prev == NULL) {
        scheduler->first = current->next;
      } else {
        prev->next = current->next;
      }
      scheduler->size--;
      return;
    }
    prev = current;
    current = current->next;
  }
}

scheduler_node *schedule(schedulerADT scheduler) {
  if (scheduler->size == 0) {
    return NULL;
  }

  if (scheduler->size == 1) {
    scheduler->running = scheduler->first;
  }

  if (scheduler->running->remaining_quantum == 0) {
    scheduler->running->remaining_quantum = scheduler->running->pcb->priority;
    scheduler->running = scheduler->running->next;
  }

  scheduler->running->remaining_quantum--;
  return scheduler->running;
}

uint64_t context_switch(schedulerADT scheduler, uint64_t stack_pointer) {
  if (scheduler->size != 0) {
    scheduler->running->pcb->stack_pointer = stack_pointer;
  }

  scheduler_node *next_process = schedule(scheduler);
  if (next_process == NULL) {
    return stack_pointer;
  }

  scheduler->running = next_process;

  return next_process->pcb->stack_pointer;
}
