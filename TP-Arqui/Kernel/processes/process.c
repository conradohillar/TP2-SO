// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "./process.h"
#include "../memory/memoryManager.h"
#include "./scheduler.h"

extern schedulerADT my_scheduler;
extern processManagerADT my_pm;

static void init_pcb(process_control_block *pcb, main_function code,
                     uint64_t pid, uint64_t ppid, char **argv, uint64_t argc,
                     uint8_t *name, uint8_t priority, uint8_t killable,
                     uint8_t in_fg);

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

void process_wrapper(main_function code, uint64_t argc, uint8_t **argv) {
  code(argc, argv);
  kill(my_pm, getpid(), my_scheduler);
  exit();
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

  init_pcb(new_pcb, &process_wrapper, process_manager->next_pid, ppid, argv,
           argc, name, priority, killable, in_fg);

  process_manager->process_table[process_manager->next_pid].pcb = new_pcb;
  process_manager->next_pid =
      process_manager->process_table[process_manager->next_pid].next_index;
  process_manager->process_count++;
  add_to_scheduler(scheduler, new_pcb);
}

static void init_pcb(process_control_block *pcb, main_function code,
                     uint64_t pid, uint64_t ppid, char **argv, uint64_t argc,
                     uint8_t *name, uint8_t priority, uint8_t killable,
                     uint8_t in_fg) {

  pcb->stack_base_pointer = mm_malloc(STACK_SIZE_BYTES) + STACK_SIZE_BYTES;
  pcb->stack_pointer = pcb->stack_base_pointer - sizeof(stack_template);
  stack_template *sp = (stack_template *)pcb->stack_pointer;

  sp->ss = 0;
  sp->cs = 0x8;
  sp->rflags = 0x202;
  sp->rip = code;
  sp->rsp = pcb->stack_pointer;
  sp->rbp = pcb->stack_base_pointer;
  sp->rax = 0;
  sp->rbx = 0;
  sp->rcx = 0;
  sp->rdx = 0;
  sp->rdi = 0;
  sp->rsi = 0;
  sp->r8 = 0;
  sp->r9 = 0;
  sp->r10 = 0;
  sp->r11 = 0;
  sp->r12 = 0;
  sp->r13 = 0;
  sp->r14 = 0;
  sp->r15 = 0;

  pcb->pid = pid;
  pcb->parent_pid = ppid;

  pcb->name = name;
  pcb->argv = argv;
  pcb->argc = argc;

  pcb->killable = killable;
  pcb->priority = priority;
  pcb->status = READY;

  pcb->waited = 0;
  pcb->waiting = 0;

  pcb->in_fg = in_fg;
}

uint8_t kill(processManagerADT pm, uint64_t pid, schedulerADT scheduler) {
  if (pid >= MAX_PROCESS_COUNT || pid < 0 ||
      pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->killable == 0) {
    return -1;
  }

  if (pm->process_table[pid].pcb->status == READY) {
    remove_from_scheduler(scheduler, pm->process_table[pid].pcb);
  }
  pm->process_table[pid].pcb->status = KILLED;

  for (int i = 0; i < pm->process_count; i++) {
    if (pm->process_table[i].pcb->parent_pid == pid) {
      pm->process_table[i].pcb->parent_pid = INIT_PID;
      // OR KILL
    }
  }

  uint32_t ppid = pm->process_table[pid].pcb->parent_pid;
  // tengo que ver si el padre lo estaba esperando
  if (pm->process_table[ppid].pcb->waiting) {
    pm->process_table[ppid].pcb->waiting = 0;
    pm->process_table[ppid].pcb->status =
        READY; // HAY QUE VER SI ES LO QUE QUIEREN
    add_to_scheduler(scheduler, pm->process_table[ppid].pcb);
  }

  // Desalojo de la tabla
  if (pm->process_table[pid].pcb->waited ||
      pm->process_table[pid].pcb->parent_pid == INIT_PID) {
    // IMPLEMENTAR UN free_process
    mm_free(pm->process_table[pid].pcb);
    pm->process_table[pid].pcb = NULL;
    // Chequear por las dudas
    uint32_t aux_pid = pm->next_pid;
    pm->next_pid = pid;
    pm->process_table[pid].next_index = aux_pid;
    return 0;
  }
}

// cuando hago wait hago kill y eso lo mata definitivo

void wait(processManagerADT pm, uint64_t pid, schedulerADT scheduler) {
  if (pid >= MAX_PROCESS_COUNT || pid < 0 ||
      pm->process_table[pid].pcb == NULL) {
    return;
  }

  // Si habia un hijo que habia terminado esperando un wait, entonces sigo
  for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
    if (pm->process_table[i].pcb != NULL &&
        pm->process_table[i].pcb->parent_pid == pid &&
        pm->process_table[i].pcb->status == ZOMBIE) {
      kill(pm, i, scheduler);
      return;
    }
  }
  // Si no, lo bloqueo

  pm->process_table[pid].pcb->status = BLOCKED;
  pm->process_table[pid].pcb->waiting = 1; // DESPUES HACEMOS ENUM O DEFINE
  remove_from_scheduler(scheduler, pm->process_table[pid].pcb);
  return;
}

uint16_t block(processManagerADT pm, schedulerADT scheduler, uint64_t pid) {
  if (pid >= MAX_PROCESS_COUNT || pid < 0 ||
      pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->status == BLOCKED ||
      pm->process_table[pid].pcb->status == KILLED) {
    return -1;
  }

  pm->process_table[pid].pcb->status == BLOCKED;
  remove_from_scheduler(scheduler, pm->process_table[pid].pcb);
  return 0;
}

uint16_t unblock(processManagerADT pm, schedulerADT scheduler, uint64_t pid) {
  if (pid >= MAX_PROCESS_COUNT || pid < 0 ||
      pm->process_table[pid].pcb == NULL ||
      pm->process_table[pid].pcb->status == READY ||
      pm->process_table[pid].pcb->status == KILLED) {
    return -1;
  }

  pm->process_table[pid].pcb->status == READY;
  add_to_scheduler(scheduler, pm->process_table[pid].pcb);
  return 0;
}

uint16_t setpriority(processManagerADT pm, uint64_t pid, uint8_t priority) {
  if (pid < 0 || pid >= MAX_PROCESS_COUNT ||
      pm->process_table[pid].pcb == NULL) {
    return -1;
  }

  pm->process_table[pid].pcb->priority = priority;
  return 0;
}
