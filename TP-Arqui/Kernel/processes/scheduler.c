// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "scheduler.h"
#include "../include/textMode.h"
#include "../memory/memoryManager.h"
#include "process.h"

typedef struct schedulerCDT {
  listADT list;
  process_control_block *running;
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
  if (!add_to_list(scheduler->list, pcb)) {
    put_string_nt((uint8_t *)"Error adding to scheduler\n", 0xFF0000, 0x000000);
  }
}

void remove_from_scheduler(schedulerADT scheduler, process_control_block *pcb) {
  if (!remove_from_list(scheduler->list, pcb)) {
    put_string_nt((uint8_t *)"Error removing from scheduler\n", 0xFF0000,
                  0x000000);
  }
}

process_control_block *schedule(schedulerADT scheduler) {
  if (is_empty(scheduler->list)) {
    return NULL;
  }

  if (scheduler->running == NULL) {
    scheduler->running = list_next(scheduler->list);
    scheduler->running->status = RUNNING;
    scheduler->running->remaining_quantum--;
    return scheduler->running;
  }

  if (scheduler->running->remaining_quantum == 0) {
    if (scheduler->running->status == RUNNING) {
      scheduler->running->status = READY;
    }
    scheduler->running->remaining_quantum = scheduler->running->priority;
    scheduler->running = list_next(scheduler->list);
    scheduler->running->status = RUNNING;
  }

  scheduler->running->remaining_quantum--;
  return scheduler->running;
}

uint64_t context_switch(schedulerADT scheduler, uint64_t stack_pointer) {
  if (scheduler->running != NULL) {
    scheduler->running->stack_pointer = (void *)stack_pointer;
  }

  process_control_block *next_process = schedule(scheduler);

  return (uint64_t)next_process->stack_pointer;
}

void running_ended(schedulerADT scheduler) {
  if (scheduler->running == NULL) {
    return;
  }
  scheduler->running = NULL;
}

process_control_block *get_running(schedulerADT scheduler) {
  return scheduler->running;
}

void destroy_scheduler(schedulerADT scheduler) {
  free_list(scheduler->list);
  mm_free(scheduler);
}