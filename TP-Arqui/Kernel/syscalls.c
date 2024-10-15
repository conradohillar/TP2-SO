#include <syscalls.h>

extern processManagerADT my_pm;

uint64_t sys_read(uint8_t fd, uint8_t *buffer, uint64_t size) {
  if (fd != STDIN) {
    return 0;
  }
  int i;
  for (i = 0; i < size; i++) {
    uint8_t c = '\0';
    while (c == '\0') {
      c = get_ascii();
    }
    buffer[i] = c;
  }
  return i;
}

uint64_t sys_write(uint64_t fd, uint8_t *buffer, uint64_t size,
                   uint32_t fore_color, uint32_t back_color) {
  if (fd == STDOUT) {
    return put_string(buffer, size, fore_color, back_color);
  }
  return 0;
}

void sys_write_at(uint8_t *buffer, uint64_t size, uint64_t color, uint64_t pos,
                  uint64_t scale) {
  uint32_t back_color = (uint32_t)color;
  uint32_t fore_color = color >> 32;
  uint64_t x = (uint32_t)pos;
  uint64_t y = pos >> 32;
  put_string_scale(buffer, size, fore_color, back_color, &x, &y, scale);
}

void sys_clear_screen() { clear_screen_and_buffer(); }

void sys_set_text_size(uint64_t size) { set_scale_text_mode(size); }

void sys_get_time(uint8_t *buffer) { time_to_string(buffer); }

void sys_get_registers() { peek_registers(); }

void sys_put_rectangle(uint64_t TopLeftX, uint64_t TopLeftY,
                       uint64_t BottomRightX, uint64_t BottomRightY,
                       uint32_t color) {
  put_rectangle(TopLeftX, TopLeftY, BottomRightX, BottomRightY, color);
}

uint64_t sys_get_ticks() { return ticks_elapsed(); }

void sys_check_keyboard(uint8_t *buffer) { buffer[0] = get_ascii(); }

void sys_make_sound(uint64_t millis, uint32_t freq) { beep(millis, freq); }

void sys_sleep(uint64_t secs, uint64_t millis) {
  sleep(secs);
  nanosleep(millis);
}

int32_t sys_create_process(main_fn code, uint64_t argc, uint8_t **argv,
                           uint8_t *name, uint8_t in_fg) {
  int32_t res = create_process(my_pm, code, argc, argv, name, in_fg);
  if (in_fg) {
    waitpid(my_pm, res);
  }
  return res;
}

int64_t sys_kill(uint64_t pid) { return kill(my_pm, pid); }

void sys_wait() { wait(my_pm); }

void sys_waitpid(uint64_t pid) { waitpid(my_pm, pid); }

int64_t sys_block(uint64_t pid) { return block(my_pm, pid); }

int64_t sys_unblock(uint64_t pid) { return unblock(my_pm, pid); }

uint64_t sys_getpid() { return getpid(my_pm); }

uint64_t sys_ps() {
  ps_struct *ps = send_ps_info(my_pm);
  return (uint64_t)ps;
}

void sys_free_ps(ps_struct *ps) { free_ps(ps); }

void sys_set_priority(uint64_t pid, uint8_t new_priority) {
  set_priority(my_pm, pid, new_priority);
}