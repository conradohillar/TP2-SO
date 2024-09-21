#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <stdint.h>
#include <keyboardDriver.h>
#include <textMode.h>
#include <videoDriver.h>
#include <mytime.h>
#include <registers.h>
#include <rtcDriver.h>
#include <soundDriver.h>


#define STDOUT 1
#define STDIN 0


uint64_t sys_read(uint8_t fd, uint8_t * buffer, uint64_t size);

uint64_t sys_write(uint64_t fd, uint8_t * buffer, uint64_t size, uint32_t fore_color, uint32_t back_color);

void sys_write_at(uint8_t * buffer, uint64_t size, uint64_t color, uint64_t pos, uint64_t scale);

void sys_clear_screen();

void sys_set_text_size(uint64_t size);

void sys_get_time(uint8_t * buffer);

void sys_get_registers();

void sys_put_rectangle(uint64_t TopLeftX, uint64_t TopLeftY, uint64_t BottomRightX, uint64_t BottomRightY, uint32_t color);

uint64_t sys_get_ticks();

void sys_check_keyboard(uint8_t* buffer);

void sys_make_sound(uint64_t millis, uint32_t freq);

void sys_sleep(uint64_t secs, uint64_t millis);


#endif