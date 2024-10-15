// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <song_book.h>

void song_dispatcher(uint8_t idx) {
    switch (idx)
    {
    case 1: 
        play_startsound();
        break;
    case 2: 
        play_happybirthday();
        break;
    case 3: 
        play_lacucaracha();
        break;
    case 4: 
        play_imperialmarch();
        break;
    case 5: 
        play_daledalebo();
        break;
    case 6:
		play_gameover();
		break;
	case 7:
		play_terraria();
		break;
	case 8:
		play_elhimno();
        break;
    default:
        break;
    }
}

void play_imperialmarch() {
	make_sound(500,MI);
	sleep(0,200);
	make_sound(500,MI);
	sleep(0,200);
	make_sound(500,MI);
	sleep(0,200);
	make_sound(350,DO);
	sleep(0,80);
	make_sound(100,SOL);
	sleep(0,80);
	make_sound(400,MI);
	sleep(0,200);
	make_sound(350,DO);
	sleep(0,80);
	make_sound(100,SOL);
	sleep(0,80);
	make_sound(400,MI);
	sleep(0,50);
}

void play_happybirthday() {
    make_sound(200,DO);
	sleep(0,50);
	make_sound(100,DO);
	sleep(0,50);
	make_sound(500,RE);
	sleep(0,50);
	make_sound(300,DO);
	sleep(0,100);
	make_sound(400,FA);
	sleep(0,50);
	make_sound(300,MI);
	sleep(0,50);

	sleep(0,500);

	make_sound(200,DO);
	sleep(0,50);
	make_sound(100,DO);
	sleep(0,50);
	make_sound(500,RE);
	sleep(0,50);
	make_sound(300,DO);
	sleep(0,100);
	make_sound(400,SOL);
	sleep(0,50);
	make_sound(300,FA);
	sleep(0,50);

	sleep(0,500);

	make_sound(200,DO);
	sleep(0,50);
	make_sound(100,DO);
	sleep(0,50);
	make_sound(500,SI+40);
	sleep(0,50);
	make_sound(400,LA);
	sleep(0,200);
	make_sound(200,FA);
	sleep(0,50);
	make_sound(100,FA);
	sleep(0,50);
	make_sound(300,MI);
	sleep(0,50);
	make_sound(400,RE);
	
	sleep(0,500);

	make_sound(100,LAS);
	sleep(0,50);
	make_sound(150,LAS);
	sleep(0,50);
	make_sound(500,LA);
	sleep(0,50);
	make_sound(400,FA);
	sleep(0,200);
	make_sound(300,SOL);
	sleep(0,50);
	make_sound(200,FA);
	sleep(0,50);
}

void play_lacucaracha() {
	make_sound(100,DO);
	sleep(0,50);
	make_sound(100,DO);
	sleep(0,50);
	make_sound(100,DO);
	sleep(0,50);
	make_sound(200,FA);
	sleep(0,50);
	make_sound(200,LA);
	sleep(0,50);

	make_sound(100,DO);
	sleep(0,50);
	make_sound(100,DO);
	sleep(0,50);
	make_sound(100,DO);
	sleep(0,50);
	make_sound(200,FA);
	sleep(0,50);
	make_sound(200,LA);
	sleep(0,50);
	
	make_sound(100,FA);
	sleep(0,50);
	make_sound(100,FA);
	sleep(0,50);
	make_sound(100,MI);
	sleep(0,50);
	make_sound(100,MI);
	sleep(0,50);
	make_sound(100,RE);
	sleep(0,50);
	make_sound(100,RE);
	sleep(0,50);
	make_sound(500,DO);
	sleep(0,50);
}

void play_daledalebo() {
	make_sound(100,MI);
	sleep(0,50);
	make_sound(100,LA);
	sleep(0,50);
	make_sound(100,SI);
	sleep(0,50);
	make_sound(100,SI+40);
	sleep(0,150);
	make_sound(100,SI+40);
	sleep(0,50);
	make_sound(100,SI+40);
	sleep(0,150);
	make_sound(100,SI+40);
	sleep(0,50);
	make_sound(100,SI+40);
	sleep(0,50);
	make_sound(100,SI+40);
	sleep(0,50);
	make_sound(100,SI+40);
	sleep(0,150);
	make_sound(100,SI+40);
	sleep(0,50);
	make_sound(100,SI+40);
	sleep(0,150);
	make_sound(100,SI+40);
	sleep(0,50);
	make_sound(100,SI);
	sleep(0,50);
	make_sound(100,LA);
	sleep(0,50);
	make_sound(500,SI+40);
	sleep(0,50);
	make_sound(500,LA);
	sleep(0,100);
}

void play_startsound() {
	make_sound(5,DO);
	sleep(0,30);
	make_sound(50,MI);
	sleep(0,50);
	make_sound(400,SI+40);
}

void play_gameover() {
    make_sound(400,SI+40);
    sleep(0,50);
    make_sound(350,SOL);
    sleep(0,50);
    make_sound(250,MI);
    sleep(0,50);
    make_sound(200,LA);
    sleep(0,50);
    make_sound(150,SI);
    sleep(0,50);
    make_sound(100,LA);
    sleep(0,50);
    make_sound(200,SOLS);
    sleep(0,50);
    make_sound(150,LAS);
    sleep(0,50);
    make_sound(200,SOLS);
    sleep(0,50);
    make_sound(1000,SOL);
    sleep(0,10);
}

void play_terraria() {
	make_sound(250, SOL5);
	sleep(0, 50);
    make_sound(150, FAS5);
	sleep(0, 50);
    make_sound(250, SI);
	sleep(0, 50);
    make_sound(100, SI);
	sleep(0, 50);
    make_sound(100, DO5);
	sleep(0, 50);
    make_sound(100, MI5);
	sleep(0, 50);
	make_sound(100, RE5);
	sleep(0, 50);
	make_sound(250, SOL);
	sleep(0, 50);
	make_sound(250, SOL);
	sleep(0, 50);
	make_sound(100, SOL);
	sleep(0, 50);
	make_sound(100, RE5);
	sleep(0, 50);
	make_sound(100, SOL5);
	sleep(0, 50);
	make_sound(400, LA5);
	sleep(0, 50);
	make_sound(700, SI5);
}

void play_elhimno(){
    make_sound(800, SOL);
    sleep(0, 50);
    make_sound(600, SI);
    sleep(0, 50);
    make_sound(200, SOL);
    sleep(0, 50);
    make_sound(1200, RE5);
    sleep(0, 150);
    make_sound(400, DO5);
    sleep(0, 50);
    make_sound(200, LA);
    sleep(0, 50);
    make_sound(750, FAS);
    sleep(0, 50);
    make_sound(750, FAS);
    sleep(0, 50);
    make_sound(750, FAS);
    sleep(0, 50);
    make_sound(750, FAS);
    sleep(0, 50);
    make_sound(1200, FAS);
    sleep(0, 50);
    make_sound(150, SOL);
    sleep(0, 50);
    make_sound(150, LA);
    sleep(0, 50);
    make_sound(600, SOL);
}