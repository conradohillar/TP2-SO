// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <initProcess.h>

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