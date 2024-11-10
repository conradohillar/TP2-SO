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

int64_t check_div_by_zero(uint64_t argc, uint8_t *argv[]);

int64_t check_invalid_opcode(uint64_t argc, uint8_t *argv[]);

int64_t get_registers(uint64_t argc, uint8_t *argv[]);

int64_t run_eliminator(uint64_t argc, uint8_t *argv[]);

int64_t increase_text_size(uint64_t argc, uint8_t *argv[]);

int64_t decrease_text_size(uint64_t argc, uint8_t *argv[]);

void set_text_size(uint64_t new_size);

int64_t get_time(uint64_t argc, uint8_t *argv[]);

int64_t clear(uint64_t argc, uint8_t *argv[]);

int64_t test_processes(uint64_t argc, uint8_t *argv[]);

int64_t test_priority(uint64_t argc, uint8_t *argv[]);

int64_t ps(uint64_t argc, uint8_t *argv[]);

int64_t mem_status(uint64_t argc, uint8_t *argv[]);

int64_t test_semaphores(uint64_t argc, uint8_t *argv[]);

int64_t test_ipc(uint64_t argc, uint8_t *argv[]);

int64_t loop(uint64_t argc, uint8_t *argv[]);

int64_t cat(uint64_t argc, uint8_t *argv[]);

int64_t wc(uint64_t argc, uint8_t *argv[]);

int64_t filter(uint64_t argc, uint8_t *argv[]);

void kill(uint8_t pid);

void kill_by_name(uint8_t *name);

void block(uint8_t pid);

void unblock(uint8_t pid);

void nice(uint8_t pid, uint8_t new_priority);

void pipe_functions(fn fn1, fn fn2);

void play_song(uint8_t id);

uint64_t get_command(uint8_t *str);

void run_shell();

#endif