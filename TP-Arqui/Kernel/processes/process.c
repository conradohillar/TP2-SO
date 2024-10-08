// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "./process.h"
#include "../memory/memoryManager.h"
#include "./scheduler.h"

static void init_pcb(process_control_block *pcb, uint64_t pid, uint64_t ppid,
                     char **argv, uint64_t argc, uint8_t *name,
                     uint8_t priority, uint8_t killable, uint8_t in_fg);

typedef struct process_node {
  process_control_block *pcb;
  uint16_t next_index;
} process_node;

struct processManagerCDT {
  process_node process_table[MAX_PROCESS_COUNT];
  uint64_t next_pid;
  uint64_t current_pid;
  uint64_t process_count;
} processManagerCDT;

processManagerADT create_process_manager() {

  processManagerADT pm = mm_malloc(sizeof(processManagerCDT));
  if (pm == NULL) {
    return NULL;
  }

  for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
    pm->process_table[i].pcb = NULL;
    pm->process_table[i].next_index = i + 1;
  }
  pm->process_table[MAX_PROCESS_COUNT - 1].next_index = -1;

  pm->next_pid = 0;
  pm->current_pid = -1;
  pm->process_count = 0;
  return pm;
}

uint16_t create_process(schedulerADT scheduler,
                        processManagerADT process_manager, main_function code,
                        char **argv, uint64_t argc, char *name, uint64_t ppid,
                        uint8_t priority, uint8_t killable, uint8_t in_fg) {

  if (process_manager->process_count == MAX_PROCESS_COUNT) {
    // MANEJAR ERRORES DESPUES
    return -1;
  }
  process_control_block *new_pcb = mm_malloc(sizeof(process_control_block));
  if (new_pcb == NULL) {
    // MANEJAR ERRORES DESPUES
    return -1;
  }

  init_pcb(new_pcb, process_manager->next_pid, ppid, argv, argc, name, priority,
           killable, in_fg);

  process_manager->process_table[process_manager->next_pid].pcb = new_pcb;
  process_manager->current_pid = process_manager->next_pid;
  process_manager->next_pid =
      process_manager->process_table[process_manager->current_pid].next_index;
  process_manager->process_count++;
  add_to_scheduler(scheduler, new_pcb);
}

static void init_pcb(process_control_block *pcb, uint64_t pid, uint64_t ppid,
                     char **argv, uint64_t argc, uint8_t *name,
                     uint8_t priority, uint8_t killable, uint8_t in_fg) {

  pcb->stack_base_pointer = mm_malloc(STACK_SIZE_BYTES) + STACK_SIZE_BYTES;
  pcb->stack_pointer =
      (stack_template *)(pcb->stack_base_pointer - sizeof(stack_template));

  pcb->pid = pid;
  pcb->parent_pid = ppid;

  pcb->name = name;
  pcb->argv = argv;
  pcb->argc = argc;

  pcb->killable = killable;
  pcb->priority = priority;
  pcb->status = READY;

  pcb->in_fg = in_fg;
}

uint8_t kill(processManagerADT pm, uint64_t pid) {
  if (pid >= MAX_PROCESS_COUNT || pid < 0 ||
      pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->status == KILLED ||
      pm->process_table[pid].pcb->killable == 0) {
    return -1;
  }

  pm->process_table[pid].pcb->status = KILLED;

  for (int i = 0; i < pm->process_count; i++) {
    if (pm->process_table[i].pcb->parent_pid == pid) {
      pm->process_table[i].pcb->parent_pid = INIT_PID;
      // OR KILL
    }
  }

  if (pm->process_table[pid].pcb->parent_pid == INIT_PID) {
    // sacarlo de la tabla
  }
}