#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);

uint64_t strlen(const uint8_t *str);
void strcpy(uint8_t *dest, const uint8_t *src);

#endif