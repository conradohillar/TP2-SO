#include <../../../processUtils.h>
#include <stdint.h>

#define STDIN 0
#define STDOUT 1

typedef uint64_t (*fn)(uint64_t argc, uint8_t *argv[]);

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

uint64_t sys_block_asm(uint64_t pid);

uint64_t sys_unblock_asm(uint64_t pid);

uint64_t sys_ps_asm();

void sys_free_ps_asm(ps_struct *ps);
