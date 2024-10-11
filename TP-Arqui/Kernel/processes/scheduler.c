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
    put_string_nt("Error adding to scheduler\n", 0xFF0000, 0x000000);
    return;
  }

  if (scheduler->size == 0) {
    scheduler->first = new_node;
    scheduler->running = new_node;
  } else {
    scheduler->last->next = new_node;
  }

  new_node->pcb = pcb;
  new_node->remaining_quantum = pcb->priority;
  new_node->next = scheduler->first;

  scheduler->last = new_node;
  scheduler->size++;
}

void remove_from_scheduler(schedulerADT scheduler, process_control_block *pcb) {
  scheduler_node *current = scheduler->first;
  scheduler_node *prev = NULL;

  while (prev != scheduler->last) {
    if (current->pcb == pcb) {
      if (prev == NULL) {
        scheduler->first = current->next;
        scheduler->last->next = scheduler->first;
      } else {
        prev->next = current->next;
        if (current == scheduler->last) {
          scheduler->last = prev;
        }
      }
      mm_free(current);
      scheduler->size--;
      if (scheduler->size == 0) {
        scheduler->first = NULL;
        scheduler->last = NULL;
        scheduler->running = NULL;
      }
      return;
    }
    prev = current;
    current = prev->next;
  }
}

scheduler_node *schedule(schedulerADT scheduler) {
  if (scheduler->size == 0) {
    return NULL;
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
    scheduler->running->pcb->stack_pointer = (void *)stack_pointer;
  }

  scheduler_node *next_process = schedule(scheduler);
  if (next_process == NULL) {
    return stack_pointer;
  }

  scheduler->running = next_process;

  return (uint64_t)next_process->pcb->stack_pointer;
}

void print_scheduler(schedulerADT scheduler) {
  scheduler_node *current = scheduler->first;
  scheduler_node *prev = NULL;
  while (prev != scheduler->last) {
    put_string_nt((uint8_t *)"Process ", ORANGE, 0x000000);
    uint8_t num[20] = {0};
    itoa(current->pcb->pid, num);
    put_string_nt((uint8_t *)num, ORANGE, 0x000000);
    put_string_nt((uint8_t *)": ", ORANGE, 0x000000);
    put_string_nt(current->pcb->name, ORANGE, 0x000000);
    put_string_nt("\n", ORANGE, 0x000000);
    prev = current;
    current = prev->next;
  }
}

void print_schedule_info(scheduler_node *node) {
  put_string_nt((uint8_t *)"Process ", ORANGE, 0x000000);
  uint8_t num[20] = {0};
  itoa(node->pcb->pid, num);
  put_string_nt((uint8_t *)num, ORANGE, 0x000000);
  put_string_nt((uint8_t *)": ", ORANGE, 0x000000);
  put_string_nt(node->pcb->name, ORANGE, 0x000000);
  put_string_nt(" -> ", ORANGE, 0x000000);
  itoa(node->remaining_quantum, num);
  put_string_nt((uint8_t *)num, ORANGE, 0x000000);
  put_string_nt((uint8_t *)" quantum remaining\n", ORANGE, 0x000000);
}
