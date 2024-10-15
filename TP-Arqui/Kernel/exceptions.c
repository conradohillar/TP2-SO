// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <exceptions.h>

void exception_dispatcher(int exception) {
  switch (exception) {
  case ZERO_EXCEPTION_ID:
    zero_division();
    break;

  case INVALID_OPCODE_ID:
    invalid_opcode();
    break;

  default:
    break;
  }
}

void invalid_opcode() {
  track_registers();
  put_string_nt((uint8_t *)"Exception: invalid opcode detected", ORANGE, BLACK);
  put_char('\n', BLACK, BLACK);
  peek_registers();
  put_char('\n', BLACK, BLACK);
  put_string_nt((uint8_t *)"Press ENTER to restart the shell", ORANGE, BLACK);
  _sti();
  uint8_t buffer[] = {0};
  while (buffer[0] != '\n') {
    sys_read(0, buffer, 1);
  }
  clear_screen_and_buffer();
}

void zero_division() {
  track_registers();
  put_string_nt((uint8_t *)"Exception: invalid operation: divided by zero",
                ORANGE, BLACK);
  put_char('\n', BLACK, BLACK);
  peek_registers();
  put_char('\n', BLACK, BLACK);
  put_string_nt((uint8_t *)"Press ENTER to return to shell", ORANGE, BLACK);
  _sti();
  uint8_t buffer[1];
  while (buffer[0] != '\n') {
    sys_read(0, buffer, 1);
  }
  clear_screen_and_buffer();
}
