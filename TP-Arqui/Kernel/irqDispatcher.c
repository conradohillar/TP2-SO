#include <irqDispatcher.h>

extern schedulerADT my_scheduler;

uint64_t irq_dispatcher(uint64_t irq, uint64_t stack_pointer) {
  switch (irq) {
  case 0:
    return int_20(stack_pointer);
  case 1:
    return int_21();

  default:
    break;
  }
  return;
}

uint64_t int_20(uint64_t stack_pointer) {
  return timer_handler(my_scheduler, stack_pointer);
}

uint64_t int_21() { keyboard_handler(); }
