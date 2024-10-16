// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "./process.h"
#include "../memory/memoryManager.h"
#include "./scheduler.h"

static void pcb_init(processManagerADT pm, process_control_block *pcb,
                     wrapper_fn wrapper, main_fn code, uint64_t argc,
                     uint8_t **argv, uint8_t *name, uint8_t in_fg);

static void process_wrapper(processManagerADT pm, uint64_t code, uint64_t argc,
                            uint8_t **argv);

static uint8_t terminate_process(processManagerADT pm, uint64_t pid,
                                 process_status status);

struct processManagerCDT {
  process_node process_table[MAX_PROCESS_COUNT];
  uint64_t process_count;
  uint64_t next_pid;
  schedulerADT scheduler;
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
  pm->process_count = 0;

  pm->scheduler = scheduler;

  // INIT process (unkillable)
  create_process(pm, (main_fn)init_process, 0, NULL, (uint8_t *)"init", 0);

  // SHELL
  create_process(pm, (main_fn)USERLAND_ADDRESS, 0, NULL, (uint8_t *)"shell", 1);
  set_priority(pm, 1, 5);

  return pm;
}

uint64_t create_process(processManagerADT pm, main_fn code, uint64_t argc,
                        uint8_t **argv, uint8_t *name, uint8_t in_fg) {

  if (pm->process_count == MAX_PROCESS_COUNT) {
    // MANEJAR ERRORES DESPUES
    return -1;
  }
  process_control_block *new_pcb = mm_malloc(sizeof(process_control_block));
  if (new_pcb == NULL) {
    // MANEJAR ERRORES DESPUES
    return -1;
  }

  uint8_t **argv_copy = (uint8_t **)mm_malloc(argc * sizeof(uint8_t *));
  for (int i = 0; i < argc; i++) {
    argv_copy[i] = mm_malloc(strlen(argv[i]) + 1);
    strcpy(argv_copy[i], argv[i]);
  }

  pcb_init(pm, new_pcb, (wrapper_fn)process_wrapper, code, argc, argv_copy,
           name, in_fg);

  pm->process_table[pm->next_pid].pcb = new_pcb;

  pm->next_pid = pm->process_table[pm->next_pid].next_index;

  pm->process_count++;

  add_to_scheduler(pm->scheduler, new_pcb);

  return new_pcb->pid;
}

static void pcb_init(processManagerADT pm, process_control_block *pcb,
                     wrapper_fn wrapper, main_fn code, uint64_t argc,
                     uint8_t **argv, uint8_t *name, uint8_t in_fg) {

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
  sp->rcx = (uint64_t)argv;
  sp->rdx = (uint64_t)argc;
  sp->rdi = (uint64_t)pm;
  sp->rsi = (uint64_t)code;
  sp->r8 = 0;
  sp->r9 = 0;
  sp->r10 = 0;
  sp->r11 = 0;
  sp->r12 = 0;
  sp->r13 = 0;
  sp->r14 = 0;
  sp->r15 = 0;

  pcb->pid = pm->next_pid;
  pcb->parent_pid = getpid(pm);

  pcb->name = name;
  pcb->argv = argv;
  pcb->argc = argc;

  pcb->killable = (pcb->pid == INIT_PID ? 0 : 1);
  pcb->priority = 1;
  pcb->remaining_quantum = 1;
  pcb->status = READY;
  pcb->waiting = 0;
  pcb->waiting_pid = -1;
  pcb->in_fg = in_fg;
}

static void process_wrapper(processManagerADT pm, uint64_t code, uint64_t argc,
                            uint8_t **argv) {
  ((main_fn)code)(argc, argv);
  exit(pm, getpid(pm));
  running_ended(pm->scheduler);
  yield();
}

uint8_t exit(processManagerADT pm, uint64_t pid) {
  return terminate_process(pm, pid, ZOMBIE);
}

uint8_t kill(processManagerADT pm, uint64_t pid) {
  return terminate_process(pm, pid, KILLED);
}

static uint8_t terminate_process(processManagerADT pm, uint64_t pid,
                                 process_status status) {
  if (pid >= MAX_PROCESS_COUNT || pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->killable == 0) {
    return -1;
  }

  if (pm->process_table[pid].pcb->status == KILLED) {
    return 0;

  } else if (pm->process_table[pid].pcb->status != ZOMBIE) {

    // Remove from scheduler
    if (pm->process_table[pid].pcb->status != BLOCKED) {
      remove_from_scheduler(pm->scheduler, pm->process_table[pid].pcb);
    }

    pm->process_table[pid].pcb->status = status;

    // Make orphan children INIT children
    for (int i = 0; i < pm->process_count; i++) {
      if (pm->process_table[i].pcb->parent_pid == pid) {
        pm->process_table[i].pcb->parent_pid = INIT_PID;
      }
    }

    // Check if parent was waiting
    uint32_t ppid = pm->process_table[pid].pcb->parent_pid;
    if (pm->process_table[ppid].pcb->waiting &&
        (pm->process_table[ppid].pcb->waiting_pid == pid ||
         pm->process_table[ppid].pcb->waiting_pid == -1)) {
      pm->process_table[ppid].pcb->waiting = 0;
      pm->process_table[ppid].pcb->waiting_pid = -1;
      unblock(pm, ppid);
      pm->process_table[pid].pcb->status = KILLED;
    }
  }

  // If it's INIT's child, remove from process_table and free
  if (pm->process_table[pid].pcb->status == KILLED ||
      pm->process_table[pid].pcb->parent_pid == INIT_PID) {
    // IMPLEMENTAR UN free_process
    free_process(pm->process_table[pid].pcb);
    pm->process_table[pid].pcb = NULL;
    pm->process_count--;

    uint64_t aux_pid = pm->next_pid;
    pm->next_pid = pid;
    pm->process_table[pid].next_index = aux_pid;
  }

  return 0;
}

void wait(processManagerADT pm) {
  uint64_t pid = getpid(pm);
  if (pid >= MAX_PROCESS_COUNT || pm->process_table[pid].pcb == NULL) {
    return;
  }

  for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
    if (pm->process_table[i].pcb != NULL &&
        pm->process_table[i].pcb->parent_pid == pid &&
        pm->process_table[i].pcb->status == ZOMBIE) {
      unblock(pm, pid);
      kill(pm, i);
      return;
    }
  }

  // In case no children has finished, block parent
  pm->process_table[pid].pcb->waiting = 1;
  block(pm, pid);
}

void waitpid(processManagerADT pm, uint64_t pid) {
  uint64_t ppid = getpid(pm);

  if (pid >= MAX_PROCESS_COUNT || pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->parent_pid != ppid) {
    return;
  }

  if (pm->process_table[pid].pcb->status == ZOMBIE) {
    unblock(pm, ppid);
    kill(pm, pid);
    return;
  }

  // In case child with PID pid hasn't finished, block parent
  pm->process_table[ppid].pcb->waiting = 1;
  pm->process_table[ppid].pcb->waiting_pid = pid;
  block(pm, ppid);
}

uint8_t block(processManagerADT pm, uint64_t pid) {
  if (pid >= MAX_PROCESS_COUNT || pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->status == BLOCKED ||
      pm->process_table[pid].pcb->status == KILLED) {
    return -1;
  }

  pm->process_table[pid].pcb->status = BLOCKED;
  remove_from_scheduler(pm->scheduler, pm->process_table[pid].pcb);
  pm->process_table[pid].pcb->remaining_quantum = 0;
  yield();
  return 0;
}

uint8_t unblock(processManagerADT pm, uint64_t pid) {
  if (pid >= MAX_PROCESS_COUNT || pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->status != BLOCKED) {
    return -1;
  }

  pm->process_table[pid].pcb->status = READY;
  add_to_scheduler(pm->scheduler, pm->process_table[pid].pcb);
  return 0;
}

uint64_t getpid(processManagerADT pm) {
  process_control_block *current = get_running(pm->scheduler);
  return current->pid;
}

uint64_t getppid(processManagerADT pm) {
  process_control_block *current = get_running(pm->scheduler);
  return current->parent_pid;
}

uint16_t set_priority(processManagerADT pm, uint64_t pid, uint8_t priority) {
  if (pid >= MAX_PROCESS_COUNT || pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->status == KILLED || priority > MAX_PRIORITY) {
    return -1;
  }

  pm->process_table[pid].pcb->priority = priority;
  return 0;
}

void free_process(process_control_block *pcb) {
  for (int i = 0; i < pcb->argc; i++) {
    mm_free(pcb->argv[i]);
  }
  mm_free(pcb->argv);
}

ps_struct *send_ps_info(processManagerADT pm) {
  ps_info *info = mm_malloc(pm->process_count * sizeof(ps_info));
  int j = 0;
  for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
    if (pm->process_table[i].pcb != NULL) {
      info[j].pid = pm->process_table[i].pcb->pid;
      info[j].name = pm->process_table[i].pcb->name;
      info[j].sp = (uint64_t)pm->process_table[i].pcb->stack_pointer;
      info[j].bp = (uint64_t)pm->process_table[i].pcb->stack_base_pointer;
      info[j].state = pm->process_table[i].pcb->status;
      info[j].priority = pm->process_table[i].pcb->priority;
      info[j].in_fg = pm->process_table[i].pcb->in_fg;
      j++;
    }
  }
  ps_struct *ps = mm_malloc(sizeof(ps_struct));
  ps->info = info;
  ps->count = pm->process_count;
  return ps;
}

void free_ps(ps_struct *ps) {
  mm_free(ps->info);
  mm_free(ps);
}

void destroy_process_table(processManagerADT pm) {
  for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
    if (pm->process_table[i].pcb != NULL) {
      free_process(pm->process_table[i].pcb);
    }
  }
  destroy_scheduler(pm->scheduler);
  mm_free(pm);
}

void init_process(uint64_t argc, uint8_t **argv) {
  while (1) {
    _hlt();
  }
}

void yield() { asm_yield(); }

process_control_block *getPCB(processManagerADT pm, uint64_t pid) {
  return pm->process_table[pid].pcb;
}