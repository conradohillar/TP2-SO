#ifndef SONG_BOOK_H
#define SONG_BOOK_H

#include <notes.h>
#include <libc.h>

void song_dispatcher(uint8_t idx);

void play_startsound();         // idx 1

void play_happybirthday();      // idx 2

void play_lacucaracha();        // idx 3

void play_imperialmarch();      // idx 4

void play_daledalebo();         // idx 5

void play_gameover();           // idx 6

void play_terraria();           // idx 7

void play_elhimno();            // idx 8


#endif