#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MAX_PROCESS_COUNT 64

typedef struct process_control_block {
  void *stack_base_pointer;
  void *stack_pointer;
  uint64_t pid;
  uint64_t parent_pid;
  uint64_t waiting_pid;

  uint8_t name[32];
  uint8_t **argv;

  uint8_t killable;
  uint8_t priority;
  uint8_t status;

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

typedef void (*main_function)(int argc, char **argv);

void kill(uint64_t pid);
void block(uint64_t pid);
void unblock(uint64_t pid);

void wait();

#endif