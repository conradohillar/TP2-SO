// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "./process.h"
#include "../memory/memoryManager.h"
#include "./scheduler.h"

static void pcb_init(processManagerADT pm, schedulerADT scheduler,
                     process_control_block *pcb, wrapper_fn wrapper,
                     main_fn code, uint64_t argc, uint8_t **argv, uint8_t *name,
                     uint8_t in_fg);

static void process_wrapper(processManagerADT pm, schedulerADT scheduler,
                            uint64_t code, uint64_t argc, uint8_t **argv);

static uint8_t terminate_process(processManagerADT pm, schedulerADT scheduler,
                                 uint64_t pid, process_status status);

struct processManagerCDT {
  process_node process_table[MAX_PROCESS_COUNT];
  uint64_t next_pid;
  uint64_t current_pid;
  uint64_t process_count;
} processManagerCDT;

processManagerADT create_process_manager(schedulerADT scheduler) {

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

  // INIT process (unkillable)
  create_process(pm, scheduler, init_process, NULL, 0, "init", 0);

  // SHELL
  create_process(pm, scheduler, (void *)USERLAND_ADDRESS, NULL, 0, "shell", 1);
  set_priority(pm, 1, 5);

  return pm;
}

uint64_t create_process(processManagerADT process_manager,
                        schedulerADT scheduler, main_fn code, uint64_t argc,
                        uint8_t **argv, uint8_t *name, uint8_t in_fg) {

  if (process_manager->process_count == MAX_PROCESS_COUNT) {
    // MANEJAR ERRORES DESPUES
    return -1;
  }
  process_control_block *new_pcb = mm_malloc(sizeof(process_control_block));
  if (new_pcb == NULL) {
    // MANEJAR ERRORES DESPUES
    return -1;
  }

  uint8_t *argv_copy[argc];
  for (int i = 0; i < argc; i++) {
    argv_copy[i] = argv[i];
  }

  pcb_init(process_manager, scheduler, new_pcb, &process_wrapper, code, argc,
           argv_copy, name, in_fg);

  process_manager->process_table[process_manager->next_pid].pcb = new_pcb;

  process_manager->next_pid =
      process_manager->process_table[process_manager->next_pid].next_index;

  process_manager->current_pid = new_pcb->pid;

  process_manager->process_count++;

  add_to_scheduler(scheduler, new_pcb);

  return new_pcb->pid;
}

static void pcb_init(processManagerADT pm, schedulerADT scheduler,
                     process_control_block *pcb, wrapper_fn wrapper,
                     main_fn code, uint64_t argc, uint8_t **argv, uint8_t *name,
                     uint8_t in_fg) {

  pcb->stack_base_pointer = mm_malloc(STACK_SIZE_BYTES) + STACK_SIZE_BYTES;
  pcb->stack_pointer = pcb->stack_base_pointer - sizeof(stack_template);
  stack_template *sp = (stack_template *)pcb->stack_pointer;

  sp->ss = 0;
  sp->cs = 0x8;
  sp->rflags = 0x202;
  sp->rip = (uint64_t)wrapper;
  sp->rsp = (uint64_t)pcb->stack_pointer;
  sp->rbp = (uint64_t)pcb->stack_base_pointer;
  sp->rax = 0;
  sp->rbx = 0;
  sp->rcx = (uint64_t)argc;
  sp->rdx = (uint64_t)code;
  sp->rdi = (uint64_t)pm;
  sp->rsi = (uint64_t)scheduler;
  sp->r8 = (uint64_t)argv;
  sp->r9 = 0;
  sp->r10 = 0;
  sp->r11 = 0;
  sp->r12 = 0;
  sp->r13 = 0;
  sp->r14 = 0;
  sp->r15 = 0;

  pcb->pid = pm->next_pid;
  pcb->parent_pid = pm->current_pid;

  pcb->name = name;
  pcb->argv = argv;
  pcb->argc = argc;

  pcb->killable = (pcb->pid == INIT_PID ? 0 : 1);
  pcb->priority = 1;
  pcb->status = READY;
  pcb->waiting = 0;
  pcb->in_fg = in_fg;
}

static void process_wrapper(processManagerADT pm, schedulerADT scheduler,
                            uint64_t code, uint64_t argc, uint8_t **argv) {
  ((main_fn)code)(argc, argv);
  exit(pm, scheduler, getpid(pm));
}

uint8_t exit(processManagerADT pm, schedulerADT scheduler, uint64_t pid) {
  return terminate_process(pm, scheduler, pid, ZOMBIE);
}

uint8_t kill(processManagerADT pm, schedulerADT scheduler, uint64_t pid) {
  return terminate_process(pm, scheduler, pid, KILLED);
}

static uint8_t terminate_process(processManagerADT pm, schedulerADT scheduler,
                                 uint64_t pid, process_status status) {
  if (pid >= MAX_PROCESS_COUNT || pid < 0 ||
      pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->killable == 0) {
    return -1;
  }

  if (pm->process_table[pid].pcb->status == KILLED) {
    return 0;

  } else if (pm->process_table[pid].pcb->status != ZOMBIE) {

    pm->process_table[pid].pcb->status = status;

    // Remove from scheduler
    if (pm->process_table[pid].pcb->status != BLOCKED) {
      remove_from_scheduler(scheduler, pm->process_table[pid].pcb);
    }

    // Make orphan children INIT children
    for (int i = 0; i < pm->process_count; i++) {
      if (pm->process_table[i].pcb->parent_pid == pid) {
        pm->process_table[i].pcb->parent_pid = INIT_PID;
      }
    }

    // Check if parent was waiting
    uint32_t ppid = pm->process_table[pid].pcb->parent_pid;
    if (pm->process_table[ppid].pcb->waiting) {
      pm->process_table[ppid].pcb->waiting = 0;
      pm->process_table[ppid].pcb->status = READY;
      add_to_scheduler(scheduler, pm->process_table[ppid].pcb);
    }
  }

  // If it's INIT's child, remove from process_table and free
  if (pm->process_table[pid].pcb->parent_pid == INIT_PID) {
    // IMPLEMENTAR UN free_process
    mm_free(pm->process_table[pid].pcb);
    pm->process_table[pid].pcb = NULL;

    uint64_t aux_pid = pm->next_pid;
    pm->next_pid = pid;
    pm->process_table[pid].next_index = aux_pid;
  }

  return 0;
}

void wait(processManagerADT pm, schedulerADT scheduler, uint64_t pid) {
  if (pid >= MAX_PROCESS_COUNT || pid < 0 ||
      pm->process_table[pid].pcb == NULL) {
    return;
  }

  for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
    if (pm->process_table[i].pcb != NULL &&
        pm->process_table[i].pcb->parent_pid == pid &&
        pm->process_table[i].pcb->status == ZOMBIE) {
      unblock(pm, scheduler, pid);
      kill(pm, scheduler, i);
      return;
    }
  }

  // In case no children has finished, block parent
  pm->process_table[pid].pcb->waiting = 1;
  block(pm, scheduler, pid);
}

uint8_t block(processManagerADT pm, schedulerADT scheduler, uint64_t pid) {
  if (pid >= MAX_PROCESS_COUNT || pid < 0 ||
      pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->status == BLOCKED ||
      pm->process_table[pid].pcb->status == KILLED) {
    return -1;
  }

  pm->process_table[pid].pcb->status = BLOCKED;
  remove_from_scheduler(scheduler, pm->process_table[pid].pcb);
  return 0;
}

uint8_t unblock(processManagerADT pm, schedulerADT scheduler, uint64_t pid) {
  if (pid >= MAX_PROCESS_COUNT || pid < 0 ||
      pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->status != BLOCKED) {
    return -1;
  }

  pm->process_table[pid].pcb->status = READY;
  add_to_scheduler(scheduler, pm->process_table[pid].pcb);
  return 0;
}

uint64_t getpid(processManagerADT pm) { return pm->current_pid; }

uint64_t getppid(processManagerADT pm) {
  return pm->process_table[pm->current_pid].pcb->parent_pid;
}

uint16_t set_priority(processManagerADT pm, uint64_t pid, uint8_t priority) {
  if (pid < 0 || pid >= MAX_PROCESS_COUNT ||
      pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->status == KILLED) {
    return -1;
  }

  pm->process_table[pid].pcb->priority = priority;
  return 0;
}

void destroy_process_table(processManagerADT pm) {
  for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
    if (pm->process_table[i].pcb != NULL) {
      mm_free(pm->process_table[i].pcb);
    }
  }
  mm_free(pm);
}

void init_process(int argc, char **argv) {
  while (1) {
    _hlt();
  }
}

process_control_block *getPCB(processManagerADT pm, uint64_t pid) {
  return pm->process_table[pid].pcb;
}