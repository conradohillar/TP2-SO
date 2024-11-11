// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pipes.h>
#include <syscalls.h>

extern pipeManagerADT my_pipe_manager;
extern processManagerADT my_pm;
extern schedulerADT my_scheduler;
extern semaphoreManagerADT my_sm;
extern memoryManagerADT my_mm;

uint64_t sys_read(uint16_t fd, uint8_t *buffer, uint64_t size) {
  if (fd == STDOUT || fd == STDERR) {
    return 0;
  }
  pipe_t *pipe = get_pipe(my_pipe_manager, fd);
  if (pipe == NULL) {
    return 0;
  }
  return read_pipe(my_pipe_manager, pipe, buffer, size);
}

uint64_t sys_write(int16_t fd, uint8_t *buffer, uint64_t size,
                   uint32_t fore_color, uint32_t back_color) {
  switch (fd) {
  case -1:
    return 0;
  case STDIN:
    return 0;
  case STDOUT:
    put_string(buffer, size, fore_color, back_color);
    return size;
  case STDERR:
    put_string(buffer, size, RED, BLACK);
    return size;

  default:
    pipe_t *pipe = get_pipe(my_pipe_manager, fd);
    if (pipe == NULL) {
      return 0;
    }
    return write_pipe(my_pipe_manager, pipe, buffer, size);
  }
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
  return res;
}

int64_t sys_kill(uint64_t pid) { return kill(my_pm, pid); }

void sys_kill_by_name(uint8_t *name) { kill_by_name(my_pm, name); }

void sys_wait() { wait(my_pm); }

int64_t sys_waitpid(uint64_t pid) { return waitpid(my_pm, pid); }

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

int8_t sys_sem_init(uint8_t id, uint8_t count) {
  if (id >= MAX_USER_SEM_ID) {
    return -1;
  }
  sem_t *sem = sem_init(my_sm, id, count);
  if (sem == NULL) {
    return -1;
  }
  return (int8_t)id;
}

void sys_sem_wait(uint8_t id) {
  if (id >= MAX_USER_SEM_ID) {
    return;
  }
  sem_wait(my_sm, get_running(my_scheduler), get_sem(my_sm, id));
}

void sys_sem_post(uint8_t id) {
  if (id >= MAX_USER_SEM_ID) {
    return;
  }
  sem_post(my_sm, get_sem(my_sm, id));
}

void sys_sem_destroy(uint8_t id) {
  if (id >= MAX_USER_SEM_ID) {
    return;
  }
  sem_destroy(my_sm, get_sem(my_sm, id));
}

int8_t sys_sem_open(uint8_t id) {
  if (my_sm == NULL || id >= MAX_USER_SEM_ID) {
    return -1;
  }
  if (get_sem(my_sm, id) == NULL) {
    sem_init(my_sm, id, 1);
  }
  if (get_sem(my_sm, id) == NULL) {
    return -1;
  }
  return (int8_t)id;
}

uint64_t sys_mem_status() { return mem_status(my_mm); }

int8_t sys_set_fd(uint16_t fd, uint16_t pipe_id) {
  return set_fd(my_pm, fd, pipe_id);
}

int16_t sys_get_fd(uint16_t fd) { return get_fd(my_pm, fd); }

int16_t sys_create_pipe() {
  pipe_t *pipe = create_pipe(my_pipe_manager);
  if (pipe == NULL) {
    return -1;
  }
  return pipe->id;
}

void sys_destroy_pipe(uint16_t pipe_id) {
  pipe_t *pipe = get_pipe(my_pipe_manager, pipe_id);
  if (pipe == NULL) {
    return;
  }
  destroy_pipe(my_pipe_manager, pipe);
}

void *sys_mm_malloc(uint64_t size) { return mm_malloc(size); }

void sys_mm_free(void *ptr) { mm_free(ptr); }