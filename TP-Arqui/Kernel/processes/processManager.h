#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <stdint.h>

typedef enum { READY, RUNNING, BLOCKED, KILLED } process_status;

typedef uint32_t pid_t;

typedef struct schedulerCDT *schedulerADT;

typedef struct processManagerCDT *processManagerADT;

typedef struct process_control_block {
  void *stack_base_pointer;
  void *stack_pointer;
  pid_t pid;
  uint64_t parent_pid;

  uint8_t *name;
  uint8_t **argv;
  uint64_t argc;

  uint8_t in_fg;
  uint8_t killable;
  uint8_t priority;
  process_status status;

} process_control_block;

typedef struct stack_template {
  uint64_t r15;
  uint64_t r14;
  uint64_t r13;
  uint64_t r12;
  uint64_t r11;
  uint64_t r10;
  uint64_t r9;
  uint64_t r8;
  uint64_t rsi;
  uint64_t rdi;
  uint64_t rbp;
  uint64_t rdx;
  uint64_t rcx;
  uint64_t rbx;
  uint64_t rax;
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
} stack_template;

#endif