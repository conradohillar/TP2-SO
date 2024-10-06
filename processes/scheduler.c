// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "scheduler.h"
#include "process.h"

typedef struct SchedulerCDT {
  process_control_block *process_table[MAX_PROCESS_COUNT];
  uint64_t pcb_array[MAX_PROCESS_COUNT]; // Array to keep track of unused PCBs
  uint64_t next_pid; // Next PID to assign (corresponds to the index
                     // of the next available PCB in pcb_array)
  uint64_t current_pid;
  uint64_t process_count;
} SchedulerCDT;

SchedulerADT create_scheduler() {

  SchedulerADT scheduler = get_scheduler();

  for (int i = 0; i < MAX_PROCESS_COUNT - 1; i++) {
    scheduler->pcb_array[i] = i + 1;
  }
  scheduler->pcb_array[MAX_PROCESS_COUNT - 1] = -1;
  scheduler->next_pid = 0;
  scheduler->current_pid = -1;
  scheduler->process_count = 0;

  return scheduler;
}

SchedulerADT get_scheduler() { return (SchedulerADT)SCHEDULER_ADDRESS; }

uint64_t getpid() { return get_scheduler()->current_pid; }

uint64_t getppid() {
  return get_scheduler()->process_table[getpid()]->parent_pid;
}

void setpriority(uint64_t pid, uint8_t priority) {
  get_scheduler()->process_table[pid]->priority = priority;
}

static void set_status(uint64_t pid, process_status status) {
  get_scheduler()->process_table[pid]->status = status;
}

// habria que hacer chequeos de esto, si un proceso esta ready no puede ser
// bloqueado
void block(uint64_t pid) {
  if (get_scheduler()->process_table[pid]->status == READY) {
    return;
  }
  set_status(pid, BLOCKED);
}

void unblock(uint64_t pid) {
  if (get_scheduler()->process_table[pid]->status != BLOCKED) {
    return;
  }
  set_status(pid, READY);
}