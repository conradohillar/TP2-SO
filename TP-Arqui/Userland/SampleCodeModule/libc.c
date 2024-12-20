// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <libc.h>
#include <stddef.h>
#include <syscaller.h>
#include <tests.h>

void play_startsound() {
  make_sound(5, 262);
  sleep(0, 30);
  make_sound(50, 330);
  sleep(0, 50);
  make_sound(400, 534);
}

uint64_t printcolor(uint8_t *str, uint64_t fore_color, uint64_t back_color) {
  int16_t fd = sys_get_fd_asm(STDOUT);
  if (fd < 0) {
    return 0;
  }
  uint64_t len = strcmp(str, (uint8_t *)"\0") == 0 ? 1 : strlen(str);
  return sys_write_asm((uint64_t)fd, str, len, fore_color, back_color);
}

uint64_t print(uint8_t *str) {
  return printcolor(str, STD_FORE_COLOR, STD_BACK_COLOR);
}

uint64_t printerr(uint8_t *str) {
  int16_t fd = sys_get_fd_asm(STDERR);
  if (fd < 0) {
    return 0;
  }
  return sys_write_asm((uint64_t)fd, str, strlen(str), RED, BLACK);
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
  int16_t fd = sys_get_fd_asm(STDIN);
  if (fd < 0) {
    return 0;
  }
  uint8_t buff[1];
  sys_read_asm((uint64_t)fd, buff, 1);
  return buff[0];
}

uint64_t putcharcolor(uint8_t c, uint32_t fore_color, uint32_t back_color) {
  int16_t fd = sys_get_fd_asm(STDOUT);
  if (fd < 0) {
    return 0;
  }
  uint8_t buff[1] = {c};
  sys_write_asm((uint64_t)fd, buff, 1, fore_color, back_color);
  return c;
}

uint64_t putcharerr(uint8_t c) {
  int16_t fd = sys_get_fd_asm(STDERR);
  if (fd < 0) {
    return 0;
  }
  uint8_t buff[1] = {c};
  sys_write_asm((uint64_t)fd, buff, 1, RED, BLACK);
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

int64_t strcmp(uint8_t *str1, uint8_t *str2) {
  uint64_t i = 0;
  while (str1[i] != '\0' && str2[i] != '\0') {
    if (str1[i] != str2[i])
      break;
    i++;
  }
  return str1[i] - str2[i];
}

// Function to split a string into words separated by spaces or tabs, returns
// the amount of words found or -1 if there's an error
int8_t split_string(const uint8_t *str,
                    uint8_t words[MAX_PARAMS][MAX_PARAM_LENGTH]) {
  int i = 0, j = 0, k = 0;

  // Traverse the string
  while (str[i] != '\0') {
    if (k >= MAX_PARAMS) {
      return -1;
    }
    // Skip leading spaces or tabs
    while (str[i] == ' ' || str[i] == '\t') {
      i++;
    }

    // Copy characters to the current word until a space or tab is encountered
    if (str[i] != '\0') {
      j = 0; // Reset for the next word
      while (str[i] != ' ' && str[i] != '\t' && str[i] != '\0') {
        if (j >= MAX_PARAM_LENGTH) {
          return -1;
        }
        words[k][j++] = str[i++];
      }
      words[k][j] = '\0'; // Null-terminate the current word
      k++;                // Move to the next word
    }
  }

  // return the word count
  return k;
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

void double_to_ascii(double num, uint8_t *str) {
  num = ((int)(num * 100 + 0.5)) / 100.0;
  uint32_t int_part = (uint32_t)num;
  double dec_part = num - int_part;
  uint32_t dec_digits = 0;
  uint32_t dec_part_int = (uint32_t)(dec_part * 1000);
  uint8_t plus = (dec_part_int % 10 >= 5);
  dec_part_int /= 10;
  dec_part_int += plus;
  while (dec_part_int % 10 == 0) {
    dec_part_int /= 10;
    dec_digits++;
  }
  itoa(int_part, str);
  uint32_t len = strlen(str);
  str[len] = '.';
  itoa(dec_part_int, str + len + 1);
  len = strlen(str);
  str[len] = '\0';
}

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

uint8_t *to_lower(uint8_t *str) {
  uint64_t i = 0;
  while (str[i] != '\0') {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] += 32;
    }
    i++;
  }
  return str;
}

/*
    AUXILIARY FUNCTIONS NOT RELATED TO THE STANDARD C LIBRARY
*/
void sleep(uint64_t secs, uint64_t millis) { sys_sleep_asm(secs, millis); }

void check_keyboard(uint8_t *buffer) { sys_check_keyboard_asm(buffer); }

uint64_t get_ticks() { return sys_get_ticks_asm(); }

void make_sound(uint64_t millis, uint32_t freq) {
  sys_make_sound_asm(millis, freq);
}

int64_t loop_fn(uint64_t argc, uint8_t *argv[]) {
  if (argc) {
    sys_set_fd_asm(STDOUT, satoi(argv[0]));
  }

  uint64_t pid = sys_getpid_asm();
  uint8_t aux[10];
  itoa(pid, aux);
  while (1) {
    print((uint8_t *)"\nHola soy el proceso loop con pid: ");
    print(aux);
    print((uint8_t *)"\n");
    sleep(1, 0);
  }
  return -1;
}

static uint8_t *status_to_string(process_status status) {
  switch (status) {
  case RUNNING:
    return (uint8_t *)"RUNNING";
  case READY:
    return (uint8_t *)"READY";
  case BLOCKED:
    return (uint8_t *)"BLOCKED";
  case KILLED:
    return (uint8_t *)"KILLED";
  case ZOMBIE:
    return (uint8_t *)"ZOMBIE";

  default:
    return (uint8_t *)"invalid-status";
  }
}

int64_t ps_fn(uint64_t argc, uint8_t *argv[]) {
  if (argc) {
    sys_set_fd_asm(STDOUT, satoi(argv[0]));
  }

  ps_struct *ps = (ps_struct *)sys_ps_asm();

  printcolor((uint8_t *)"PID\tNAME\t\t\t\tSTACK_POINTER\t\t BASE_"
                        "POINTER\t\t  STATE\t   PRIORITY\tIN_FG\n",
             GRAY, BLACK);
  for (int i = 0; i < ps->count; i++) {
    uint8_t pid[20] = {0};
    itoa(ps->info[i].pid, pid);
    print(pid);
    print_spaces(7 - strlen(pid));
    print(ps->info[i].name);
    print_spaces(20 - strlen(ps->info[i].name));
    uint8_t sp[20] = {0};
    itoh(ps->info[i].sp, sp);
    print(sp);
    print((uint8_t *)"\t");
    uint8_t bp[20] = {0};
    itoh(ps->info[i].bp, bp);
    print(bp);
    print((uint8_t *)"\t");
    print((uint8_t *)status_to_string(ps->info[i].state));
    print_spaces(12 - strlen(status_to_string(ps->info[i].state)));
    uint8_t prio[20] = {0};
    itoa(ps->info[i].priority, prio);
    print(prio);
    print((uint8_t *)"\t\t   ");
    print((uint8_t *)(ps->info[i].in_fg ? "FG" : "BG"));
    print((uint8_t *)"\n");
  }
  if (argc) {
    print((uint8_t *)"\0");
    sys_set_fd_asm(STDOUT, STDOUT);
  }
  sys_free_ps_asm(ps);
  return 0;
}

int64_t mem_fn(uint64_t argc, uint8_t *argv[]) {
  if (argc) {
    sys_set_fd_asm(STDOUT, satoi(argv[0]));
  }

  mem_info *info = (mem_info *)sys_mem_status_asm();

  print((uint8_t *)"\n MEMORY STATUS: \n\n");
  print((uint8_t *)"  Memory manager type: ");
  printcolor(info->memory_manager, GRAY, BLACK);
  print((uint8_t *)"\n");
  print((uint8_t *)"  Memory start address: ");
  uint8_t mem_start_address[20] = {0};
  itoh(info->mem_start_address, mem_start_address);
  printcolor(mem_start_address, GRAY, BLACK);
  print((uint8_t *)"\n\n  Total memory: ");
  uint8_t total_mem[20] = {0};
  itoa(info->total_mem, total_mem);
  printcolor(total_mem, GRAY, BLACK);
  printcolor((uint8_t *)" bytes\n", GRAY, BLACK);
  print((uint8_t *)"  Used memory: ");
  uint8_t used_mem[20] = {0};
  itoa(info->used_mem, used_mem);
  printcolor(used_mem, ORANGE, BLACK);
  printcolor((uint8_t *)" bytes (", ORANGE, BLACK);
  double used_mem_perc = (double)info->used_mem / info->total_mem * 100;
  uint8_t used_mem_perc_str[20] = {0};
  double_to_ascii(used_mem_perc, used_mem_perc_str);
  printcolor(used_mem_perc_str, ORANGE, BLACK);
  printcolor((uint8_t *)"%)\n", ORANGE, BLACK);
  print((uint8_t *)"  Free memory: ");
  uint8_t free_mem[20] = {0};
  itoa(info->free_mem, free_mem);
  printcolor(free_mem, GREEN, BLACK);
  printcolor((uint8_t *)" bytes (", GREEN, BLACK);
  double free_mem_perc = (double)info->free_mem / info->total_mem * 100;
  uint8_t free_mem_perc_str[20] = {0};
  double_to_ascii(free_mem_perc, free_mem_perc_str);
  printcolor(free_mem_perc_str, GREEN, BLACK);
  printcolor((uint8_t *)"%)\n\n", GREEN, BLACK);

  if (argc) {
    print((uint8_t *)"\0");
    sys_set_fd_asm(STDOUT, STDOUT);
  }

  sys_mm_free_asm(info);
  return 0;
}

int64_t help_fn(uint64_t argc, uint8_t *argv[]) {
  if (argc) {
    sys_set_fd_asm(STDOUT, satoi(argv[0]));
  }

  printcolor((uint8_t *)"help       ", ORANGE, BLACK);
  printcolor((uint8_t *)" - prints a list of all possible commands\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"divzero    ", ORANGE, BLACK);
  printcolor((uint8_t *)" - tests the divide by zero exception\n", GRAY, BLACK);
  printcolor((uint8_t *)"inopcode   ", ORANGE, BLACK);
  printcolor((uint8_t *)" - tests the invalid opcode exception\n", GRAY, BLACK);
  printcolor((uint8_t *)"time       ", ORANGE, BLACK);
  printcolor(
      (uint8_t *)" - returns the current system time in HH:MM:SS format\n",
      GRAY, BLACK);
  printcolor((uint8_t *)"testproc   ", ORANGE, BLACK);
  printcolor((uint8_t *)" - Runs test for creating and scheduling processes\n",
             GRAY, BLACK);
  printcolor((uint8_t *)"testprio   ", ORANGE, BLACK);
  printcolor((uint8_t *)" - Runs test for changing process priorities\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"testsem    ", ORANGE, BLACK);
  printcolor((uint8_t *)" - Runs test for semaphores\n", GRAY, BLACK);
  printcolor((uint8_t *)"testipc    ", ORANGE, BLACK);
  printcolor((uint8_t *)" - Runs test for inter-process communication\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"testmem N  ", ORANGE, BLACK);
  printcolor((uint8_t *)" - Runs test for memory manager, where N is the max "
                        "memory size\n",
             GRAY, BLACK);
  printcolor((uint8_t *)"philos     ", ORANGE, BLACK);
  printcolor((uint8_t *)" - Runs the dining philosophers problem\n", GRAY,
             BLACK);
  printcolor(
      (uint8_t *)"\t\t\t  Press A to add a philosopher, D to remove one, "
                 "Q to quit\n",
      GRAY, BLACK);
  printcolor((uint8_t *)"ps         ", ORANGE, BLACK);
  printcolor((uint8_t *)" - prints the process table\n", GRAY, BLACK);
  printcolor((uint8_t *)"mem        ", ORANGE, BLACK);
  printcolor((uint8_t *)" - prints the current status of the memory\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"loop       ", ORANGE, BLACK);
  printcolor((uint8_t *)" - prints hello and its pid every 3 seconds\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"cat        ", ORANGE, BLACK);
  printcolor((uint8_t *)" - prints the input from the keyboard\n", GRAY, BLACK);
  printcolor((uint8_t *)"wc         ", ORANGE, BLACK);
  printcolor((uint8_t *)" - counts chars, words and lines in the input (CTRL+D "
                        "to execute)\n",
             GRAY, BLACK);
  printcolor((uint8_t *)"filter     ", ORANGE, BLACK);
  printcolor((uint8_t *)" - filters the input, only printing the letters\n",
             GRAY, BLACK);
  printcolor((uint8_t *)"kill PID   ", ORANGE, BLACK);
  printcolor((uint8_t *)" - kills the process with specified PID\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"kill name N", ORANGE, BLACK);
  printcolor((uint8_t *)" - kills all processes with specified name N\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"block PID  ", ORANGE, BLACK);
  printcolor((uint8_t *)" - blocks the process with specified PID\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"unblock PID", ORANGE, BLACK);
  printcolor((uint8_t *)" - unblocks the process with specified PID\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"nice N M   ", ORANGE, BLACK);
  printcolor((uint8_t *)" - changes the priority of the process with PID N to "
                        "the priority M\n",
             GRAY, BLACK);
  printcolor((uint8_t *)"inctext    ", ORANGE, BLACK);
  printcolor((uint8_t *)" - increases the size of the text, (max: 2)\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"dectext    ", ORANGE, BLACK);
  printcolor((uint8_t *)" - decreases the size of the text, (min: 1)\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"clear      ", ORANGE, BLACK);
  printcolor((uint8_t *)" - clears the screen\n", GRAY, BLACK);

  if (argc) {
    print((uint8_t *)"\0");
    sys_set_fd_asm(STDOUT, STDOUT);
  }
  return 0;
}

int64_t test_semaphores_fn(uint64_t argc, uint8_t *argv[]) {
  if (argc) {
    sys_set_fd_asm(STDOUT, satoi(argv[0]));
  }

  print((uint8_t *)"Running synchronization test...\n");
  char *argv_synchro[] = {"3", "1", NULL};
  char *argv_asynchro[] = {"3", "0", NULL};

  int64_t value;
  int64_t pid =
      sys_create_process_asm(test_sem_synchro_fn, 2, (uint8_t **)argv_synchro,
                             (uint8_t *)"test_semaphores", 0);
  value = sys_waitpid_asm(pid);
  // Desde aca
  print((uint8_t *)"\n Using semaphore -> Global: ");
  uint8_t aux[10];
  if (value < 0) {
    print((uint8_t *)"negativo\n");
    return 0;
  } else {
    itoa(value, aux);
    print(aux);
  }
  // Hasta aca, se puede borrar cuando funcione
  if (value == 0) {
    printcolor((uint8_t *)"\n (1 / 2) Successfully Passed\n", GREEN, BLACK);
  } else {
    printerr((uint8_t *)"\n(1 / 2) NOT Passed\n");
  }
  pid =
      sys_create_process_asm(test_sem_synchro_fn, 2, (uint8_t **)argv_asynchro,
                             (uint8_t *)"test_semaphores", 0);
  value = sys_waitpid_asm(pid);
  // Desde aca
  print((uint8_t *)"\n Without semaphore -> Global: ");
  if (value < 0) {
    print((uint8_t *)"negativo\n");
    return 0;
  } else {
    itoa(value, aux);
    print(aux);
  }
  // Hasta aca, se puede borrar cuando funcione
  if (value != 0) {
    printcolor((uint8_t *)"\n (2 / 2) Successfully Passed\n", GREEN, BLACK);
  } else {
    printerr((uint8_t *)"\n(2 / 2) NOT Passed\n");
  }

  if (argc) {
    print((uint8_t *)"\0");
    sys_set_fd_asm(STDOUT, STDOUT);
  }
  return 0;
}

int64_t cat_fn(uint64_t argc, uint8_t *argv[]) {
  if (argc) {
    sys_set_fd_asm(STDIN, satoi(argv[0]));
  }

  uint8_t buffer[256];
  uint8_t c;
  uint64_t i = 0;
  while (1) {
    c = '\0';
    while (c != '\n') {
      c = getchar();
      if (!c) {
        sys_set_fd_asm(STDIN, STDIN);
        return 0;
      }
      putchar(c);
      buffer[i] = c;
      i = (i + 1) % 256;
    }
    buffer[i] = '\0';
    print(buffer);
    for (int j = 0; j < 256; j++) {
      buffer[j] = '\0';
    }
    i = 0;
  }
  return -1;
}

int64_t wc_fn(uint64_t argc, uint8_t *argv[]) {
  if (argc) {
    sys_set_fd_asm(STDIN, satoi(argv[0]));
  }

  uint64_t lines = 1;
  uint64_t words = 0;
  uint64_t chars = 0;
  uint8_t c;
  uint8_t last_char = '\n';
  while (1) {
    c = '\0';
    while (c != '\n') {
      c = getchar();
      if (!c) {
        if (last_char == '\n') {
          lines--;
        }
        printcolor((uint8_t *)"\nLines: ", ORANGE, BLACK);
        uint8_t aux[10];
        itoa(lines, aux);
        print(aux);
        printcolor((uint8_t *)"\nWords: ", ORANGE, BLACK);
        itoa(words, aux);
        print(aux);
        printcolor((uint8_t *)"\nChars: ", ORANGE, BLACK);
        itoa(chars, aux);
        print(aux);
        print((uint8_t *)"\n");
        sys_set_fd_asm(STDIN, STDIN);
        return 0;
      }
      putchar(c);

      chars += c == '\b' ? -1 : (c != ' ' && c != '\t' && c != '\n');

      if (c == '\n') {
        lines++;
      } else {
        if (c != ' ' && c != '\t') {
          if (last_char == ' ' || last_char == '\t' || last_char == '\n') {
            words++;
          }
        }
      }
      last_char = c;
    }
  }
  return -1;
}

int64_t filter_fn(uint64_t argc, uint8_t *argv[]) {
  if (argc) {
    sys_set_fd_asm(STDIN, satoi(argv[0]));
  }

  uint8_t c;
  while (1) {
    c = getchar();
    if (!c) {
      print((uint8_t *)"\n");
      sys_set_fd_asm(STDIN, STDIN);
      return 0;
    }
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' ||
        c == 'E' || c == 'I' || c == 'O' || c == 'U') {
      putchar(*to_lower(&c));
    }
  }
  return -1;
}

uint8_t is_number(uint8_t *str) {
  uint64_t i = 0;
  while (str[i] != '\0') {
    if (str[i] < '0' || str[i] > '9') {
      return 0;
    }
    i++;
  }
  return 1;
}