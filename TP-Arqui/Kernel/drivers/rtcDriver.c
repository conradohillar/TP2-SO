// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <rtcDriver.h>


void display_rtc(){
    uint8_t time[9];
    time_to_string(time);
    put_string(time, 0xFFFFFF, 0x000000, 2);
}


void time_to_string(uint8_t * str) {
    uint8_t hours = bcd_sub_hours24(get_hours(),3);    // UTC-3
    uint8_t minutes = get_minutes();
    uint8_t seconds = get_seconds();

    num_to_time(hours, str, 0, ':');
    num_to_time(minutes, str, 3, ':');
    num_to_time(seconds, str, 6, '\0');
}


void num_to_time(uint8_t num, uint8_t vec[], uint32_t pos, uint8_t c){
    uint8_t aux = num;
    vec[pos] = ((aux & 0xF0) >> 4) + '0';
    aux = num;
    vec[pos + 1] = (aux & 0x0F) + '0';
    vec[pos + 2] = c;
}


uint8_t bcd_sub_hours24(uint8_t bcdHours, uint8_t hoursToSub) {
    uint8_t hoursDec = ((bcdHours >> 4) * 10) + (bcdHours & 0x0F);
    hoursDec = (hoursDec + 24 - hoursToSub) % 24;
    return ((hoursDec / 10) << 4) | (hoursDec % 10);
}
