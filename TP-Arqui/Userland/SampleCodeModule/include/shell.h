#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>
#include <shell.h>
#include <libc.h>
#include <eliminator.h>
#include <syscaller.h>
#include <song_book.h>
#include <exceptionsTester.h>

void check_div_by_zero();

void check_invalid_opcode();

void get_registers();

void run_eliminator();

void increase_text_size();

void decrease_text_size();

void set_text_size(uint64_t new_size);

void get_time();

void clear();

void play_song(uint8_t idx);

uint64_t get_command(uint8_t * str);

void run_shell();

#endif