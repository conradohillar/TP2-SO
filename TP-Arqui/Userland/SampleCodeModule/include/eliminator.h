#ifndef ELIMINATOR_H
#define ELIMINATOR_H

#include <inttypes.h>

void put_rectangle(uint64_t TopLeftX, uint64_t TopLeftY, uint64_t BottomRightX, uint64_t BottomRightY, uint32_t color);

uint64_t play_eliminator();

uint64_t play_eliminator_singleplayer();

void eliminator_menu();

#endif