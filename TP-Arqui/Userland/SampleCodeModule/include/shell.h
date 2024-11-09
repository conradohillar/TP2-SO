#ifndef SHELL_H
#define SHELL_H

#include <eliminator.h>
#include <exceptionsTester.h>
#include <libc.h>
#include <shell.h>
#include <song_book.h>
#include <stdint.h>
#include <syscaller.h>
#include <tests.h>

#define NULL (void *)0
#define MIN_SONG_ID 1
#define MAX_SONG_ID 8

void check_div_by_zero(uint8_t in_fg);

void check_invalid_opcode(uint8_t in_fg);

void get_registers(uint8_t in_fg);

void run_eliminator(uint8_t in_fg);

void increase_text_size(uint8_t in_fg);

void decrease_text_size(uint8_t in_fg);

void set_text_size(uint64_t new_size);

void get_time(uint8_t in_fg);

void clear(uint8_t in_fg);

void test_processes(uint8_t in_fg);

void test_priority(uint8_t in_fg);

void ps(uint8_t in_fg);

void mem_status(uint8_t in_fg);

void test_semaphores(uint8_t in_fg);

void loop(uint8_t in_fg);

void cat(uint8_t in_fg);

void wc(uint8_t in_fg);

void filter(uint8_t in_fg);

void kill(uint8_t id, uint8_t aux);

void block(uint8_t id, uint8_t aux);

void unblock(uint8_t id, uint8_t aux);

void nice(uint8_t id, uint8_t aux);

void test_ipc(uint8_t in_fg);

void pipe_functions(shell_fn fn1, shell_fn fn2);

void play_song(uint8_t id, uint8_t aux);

uint64_t get_command(uint8_t *str);

void run_shell();

#endif