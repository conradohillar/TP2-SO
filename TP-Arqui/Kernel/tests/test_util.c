// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java:
// https://pvs-studio.com

#include "test_util.h"
#include "../include/textMode.h"
#include "syscall.h"
#include <stdint.h>

// Random
static uint32_t m_z = 362436069;
static uint32_t m_w = 521288629;

uint32_t GetUint() {
  m_z = 36969 * (m_z & 65535) + (m_z >> 16);
  m_w = 18000 * (m_w & 65535) + (m_w >> 16);
  return (m_z << 16) + m_w;
}

uint32_t GetUniform(uint32_t max) {
  uint32_t u = GetUint();
  return (u + 1.0) * 2.328306435454494e-10 * max;
}

// Memory
uint8_t memcheck(void *start, uint8_t value, uint32_t size) {
  uint8_t *p = (uint8_t *)start;
  uint32_t i;

  for (i = 0; i < size; i++, p++)
    if (*p != value)
      return 0;

  return 1;
}

// Parameters
int64_t satoi(uint8_t *str) {
  uint64_t i = 0;
  int64_t res = 0;
  int8_t sign = 1;

  if (!str)
    return 0;

  if (str[i] == '-') {
    i++;
    sign = -1;
  }

  for (; str[i] != '\0'; ++i) {
    if (str[i] < '0' || str[i] > '9')
      return 0;
    res = res * 10 + str[i] - '0';
  }

  return res * sign;
}

// Dummies
void bussy_wait(uint64_t n) {
  uint64_t i;
  for (i = 0; i < n; i++)
    ;
}

void endless_loop() {
  while (1)
    ;
}

void endless_loop_print(uint64_t wait) {

  while (1) {

    put_string_nt((uint8_t *)"HOLA", 0x00FF00, 0x000000);
    bussy_wait(wait);
  }
}

uint32_t itoa(uint32_t num, uint8_t *str) {
  uint32_t digits = 1;
  for (uint32_t n = num / 10; n != 0; digits++, n /= 10)
    ;

  if (digits == 1) {
    str[0] = num + '0';
    str[1] = '\0';
    return digits;
  }

  str[digits] = '\0';
  for (int32_t i = digits - 1; i >= 0; i--) {
    str[i] = (num % 10) + '0';
    num /= 10;
  }

  return digits;
}
