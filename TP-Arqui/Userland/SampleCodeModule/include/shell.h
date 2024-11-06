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

void check_div_by_zero();

void check_invalid_opcode();

void get_registers();

void run_eliminator();

void increase_text_size();

void decrease_text_size();

void set_text_size(uint64_t new_size);

void get_time();

void clear();

void test_processes();

void test_priority();

void ps();

void mem_status();

void test_semaphores();

void loop();

void kill(uint8_t id, uint8_t aux);

void block(uint8_t id, uint8_t aux);

void unblock(uint8_t id, uint8_t aux);

void nice(uint8_t id, uint8_t aux);

void test_ipc();

void play_song(uint8_t id, uint8_t aux);

uint64_t get_command(uint8_t *str);

void run_shell();

#endif