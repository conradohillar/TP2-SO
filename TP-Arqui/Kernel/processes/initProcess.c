#include "./initProcess.h"

extern processManagerADT my_pm;

void init_process(uint64_t argc, uint8_t **argv) {
  while (1) {
    for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
      if (get_status(my_pm, i) == ZOMBIE) {
        waitpid(my_pm, i);
      }
    }
    _hlt();
  }
}