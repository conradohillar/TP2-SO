#include <../../../processUtils.h>
#include <stdint.h>

#define STDIN 0
#define STDOUT 1

typedef int64_t (*fn)(uint64_t argc, uint8_t *argv[]);

uint64_t sys_read_asm(uint64_t fd, uint8_t *buffer, uint64_t size);

uint64_t sys_write_asm(uint64_t fd, uint8_t *buffer, uint64_t size,
                       uint32_t fore_color, uint32_t back_color);

void sys_write_at_asm(uint8_t *buffer, uint64_t size, uint64_t color,
                      uint64_t pos, uint64_t scale);

void sys_clear_screen_asm();

void sys_get_time_asm(uint8_t *buff);

void sys_get_registers_asm();

void sys_set_text_size_asm(uint64_t size);

void sys_beep_asm(uint64_t frequency, uint64_t duration);

void sys_put_rectangle_asm(uint64_t TopLeftX, uint64_t TopLeftY,
                           uint64_t BottomRightX, uint64_t BottomRightY,
                           uint32_t color);

void sys_make_sound_asm(uint64_t millis, uint32_t freq);

uint64_t sys_get_ticks_asm();

void sys_check_keyboard_asm(uint8_t *buffer);

void sys_sleep_asm(uint64_t secs, uint64_t millis);

int64_t sys_create_process_asm(fn code, uint64_t argc, uint8_t **argv,
                               uint8_t *name, uint8_t in_fg);

uint64_t sys_kill_asm(uint64_t pid);

void sys_wait_asm();

int64_t sys_waitpid_asm(uint64_t pid);

uint64_t sys_block_asm(uint64_t pid);

uint64_t sys_unblock_asm(uint64_t pid);

uint64_t sys_getpid_asm();

void sys_set_priority_asm(uint64_t pid, uint8_t new_priority);

uint64_t sys_ps_asm();

void sys_free_ps_asm(ps_struct *ps);

int8_t sys_sem_init_asm(uint8_t id, uint8_t count);

void sys_sem_wait_asm(semaphore sem);

void sys_sem_post_asm(semaphore sem);

void sys_sem_destroy_asm(semaphore sem);

int8_t sys_sem_open_asm(semaphore sem);

void sys_yield();

int8_t sys_set_fd_asm(uint16_t fd, uint16_t pipe_id);

int16_t sys_create_pipe_asm();

void sys_destroy_pipe_asm(uint16_t pipe_id);