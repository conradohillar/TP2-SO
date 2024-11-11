#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);

uint64_t strlen(const uint8_t *str);
void strcpy(uint8_t *dest, const uint8_t *src);
int8_t strcmp(const uint8_t *str1, const uint8_t *str2);
uint32_t itoa(uint32_t num, uint8_t *str);

#endif