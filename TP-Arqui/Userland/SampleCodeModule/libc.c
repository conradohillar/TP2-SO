#include <libc.h>
#include <syscaller.h>

uint64_t printcolor(uint8_t *str, uint64_t fore_color, uint64_t back_color) {
  return sys_write_asm(STDOUT, str, strlen(str), fore_color, back_color);
}

uint64_t print(uint8_t *str) {
  return printcolor(str, STD_FORE_COLOR, STD_BACK_COLOR);
}

void print_at(uint8_t *buffer, uint64_t size, uint32_t fore_color,
              uint32_t back_color, uint32_t x, uint32_t y, uint64_t scale) {
  uint64_t color = fore_color;
  color <<= 32;
  color += back_color;
  uint64_t pos = y;
  pos <<= 32;
  pos += x;
  sys_write_at_asm(buffer, size, color, pos, scale);
}

uint64_t scan(uint8_t *buffer, uint64_t max_length) {
  uint64_t i = 0;
  while (i < max_length) {
    uint8_t c = getchar();
    if (c == '\n') {
      break;
    }
    buffer[i] = c;
    i++;
  }
  buffer[i] = '\0';
  return i;
}

uint8_t getchar(void) {
  uint8_t buff[1];
  sys_read_asm(STDIN, buff, 1);
  return buff[0];
}

uint64_t putcharcolor(uint8_t c, uint32_t fore_color, uint32_t back_color) {
  uint8_t buff[1] = {c};
  sys_write_asm(STDOUT, buff, 1, fore_color, back_color);
  return c;
}

uint64_t putchar(uint8_t c) {
  return putcharcolor(c, STD_FORE_COLOR, STD_BACK_COLOR);
}

void print_spaces(uint64_t spaces) {
  for (int i = 0; i < spaces; i++) {
    print((uint8_t *)" ");
  }
}

uint64_t strlen(uint8_t *string) {
  uint64_t len = 0;
  while (string[len] != '\0')
    len++;
  return len;
}

uint64_t strcmp(uint8_t *str1, uint8_t *str2) {
  uint64_t i = 0;
  while (str1[i] != '\0' && str2[i] != '\0') {
    if (str1[i] != str2[i])
      break;
    i++;
  }
  return str1[i] - str2[i];
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

void itoh(uint64_t num, uint8_t *str) {
  uint8_t hex[16] = "0123456789ABCDEF";
  str[0] = '0';
  str[1] = 'x';
  str[2] = hex[(num >> 60) & 0xF];
  str[3] = hex[(num >> 56) & 0xF];
  str[4] = hex[(num >> 52) & 0xF];
  str[5] = hex[(num >> 48) & 0xF];
  str[6] = hex[(num >> 44) & 0xF];
  str[7] = hex[(num >> 40) & 0xF];
  str[8] = hex[(num >> 36) & 0xF];
  str[9] = hex[(num >> 32) & 0xF];
  str[10] = hex[(num >> 28) & 0xF];
  str[11] = hex[(num >> 24) & 0xF];
  str[12] = hex[(num >> 20) & 0xF];
  str[13] = hex[(num >> 16) & 0xF];
  str[14] = hex[(num >> 12) & 0xF];
  str[15] = hex[(num >> 8) & 0xF];
  str[16] = hex[(num >> 4) & 0xF];
  str[17] = hex[num & 0xF];
  str[18] = '\0';
}

void sleep(uint64_t secs, uint64_t millis) { sys_sleep_asm(secs, millis); }

uint64_t atoi(uint8_t *str) {
  uint64_t digits;
  uint64_t ans = 0;

  for (digits = 0; str[digits] != '\0'; digits++)
    ;

  uint64_t mult = 1;

  while (digits > 0) {
    ans += (str[--digits] - '0') * mult;
    mult *= 10;
  }
  return ans;
}

/*
    AUXILIARY FUNCTIONS NOT RELATED TO THE STANDARD C LIBRARY
*/

void check_keyboard(uint8_t *buffer) { sys_check_keyboard_asm(buffer); }

uint64_t get_ticks() { return sys_get_ticks_asm(); }

void make_sound(uint64_t millis, uint32_t freq) {
  sys_make_sound_asm(millis, freq);
}
