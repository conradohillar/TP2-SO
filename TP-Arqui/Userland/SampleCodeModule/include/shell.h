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

void play_song(uint8_t idx);

uint64_t get_command(uint8_t *str);

void run_shell();

#endif