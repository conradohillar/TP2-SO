#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include "../../processUtils.h"
#include "../processes/process.h"
#include "../processes/processManager.h"
#include "../semaphores/sempahores.h"
#include <keyboardDriver.h>
#include <mytime.h>
#include <registers.h>
#include <rtcDriver.h>
#include <soundDriver.h>
#include <stdint.h>
#include <textMode.h>
#include <videoDriver.h>

typedef void (*main_fn)(uint64_t argc, uint8_t **argv);

#define STDOUT 1
#define STDIN 0

uint64_t sys_read(uint8_t fd, uint8_t *buffer, uint64_t size);

uint64_t sys_write(uint64_t fd, uint8_t *buffer, uint64_t size,
                   uint32_t fore_color, uint32_t back_color);

void sys_write_at(uint8_t *buffer, uint64_t size, uint64_t color, uint64_t pos,
                  uint64_t scale);

void sys_clear_screen();

void sys_set_text_size(uint64_t size);

void sys_get_time(uint8_t *buffer);

void sys_get_registers();

void sys_put_rectangle(uint64_t TopLeftX, uint64_t TopLeftY,
                       uint64_t BottomRightX, uint64_t BottomRightY,
                       uint32_t color);

uint64_t sys_get_ticks();

void sys_check_keyboard(uint8_t *buffer);

void sys_make_sound(uint64_t millis, uint32_t freq);

void sys_sleep(uint64_t secs, uint64_t millis);

int32_t sys_create_process(main_fn code, uint64_t argc, uint8_t **argv,
                           uint8_t *name, uint8_t in_fg);

int64_t sys_kill(uint64_t pid);

void sys_wait();

void sys_waitpid(uint64_t pid);

int64_t sys_block(uint64_t pid);

int64_t sys_unblock(uint64_t pid);

uint64_t sys_getpid();

uint64_t sys_ps();

void sys_free_ps(ps_struct *ps);

void sys_set_priority(uint64_t pid, uint8_t new_priority);

int8_t sys_sem_init(uint8_t id, uint8_t count);

void sys_sem_wait(uint8_t id);

void sys_sem_post(uint8_t id);

void sys_sem_destroy(uint8_t id);

#endif