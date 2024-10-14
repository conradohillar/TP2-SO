#include <shell.h>

#define MAX_TEXT_SIZE 2
#define MIN_TEXT_SIZE 1
#define COMM_BUFF_SIZE 128

static uint64_t scale = 1;
uint64_t testing(uint64_t argc, uint8_t **argv);
static char *status_to_string(process_status status);

void help() {
  uint8_t *supertab = (uint8_t *)"\t\t\t\t";
  printcolor((uint8_t *)"help      ", ORANGE, BLACK);
  printcolor((uint8_t *)" - prints a list of all possible commands\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"divzero   ", ORANGE, BLACK);
  printcolor((uint8_t *)" - tests the divide by zero exception\n", GRAY, BLACK);
  printcolor((uint8_t *)"inopcode  ", ORANGE, BLACK);
  printcolor((uint8_t *)" - tests the invalid opcode exception\n", GRAY, BLACK);
  printcolor((uint8_t *)"time      ", ORANGE, BLACK);
  printcolor(
      (uint8_t *)" - returns the current system time in HH:MM:SS format\n",
      GRAY, BLACK);
  printcolor((uint8_t *)"regstatus ", ORANGE, BLACK);
  printcolor((uint8_t *)" - prints the values of the CPU registers\n", GRAY,
             BLACK);
  print((uint8_t *)"\t\t\t ");
  printcolor((uint8_t *)"press Ctrl + r to save the register states\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"textprocesses ", ORANGE, BLACK);
  printcolor((uint8_t *)" - Runs test for creating and scheduling processes\n",
             GRAY, BLACK);
  printcolor((uint8_t *)"ps        ", ORANGE, BLACK);
  printcolor((uint8_t *)" - prints the process table\n", GRAY, BLACK);
  printcolor((uint8_t *)"eliminator", ORANGE, BLACK);
  printcolor((uint8_t *)" - play the \"Eliminator\" game\n", GRAY, BLACK);
  printcolor((uint8_t *)"inctext   ", ORANGE, BLACK);
  printcolor((uint8_t *)" - increases the size of the text, (max: 2)\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"dectext   ", ORANGE, BLACK);
  printcolor((uint8_t *)" - decreases the size of the text, (min: 1)\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"clear     ", ORANGE, BLACK);
  printcolor((uint8_t *)" - clears the screen\n", GRAY, BLACK);
  printcolor((uint8_t *)"playsong N", ORANGE, BLACK);
  printcolor((uint8_t *)" - plays the N-th tune, being N:\n", GRAY, BLACK);
  print(supertab);
  printcolor((uint8_t *)"1: Starting beep sound\n", GRAY, BLACK);
  print(supertab);
  printcolor((uint8_t *)"2: Happy birthday\n", GRAY, BLACK);
  print(supertab);
  printcolor((uint8_t *)"3: La Cucaracha\n", GRAY, BLACK);
  print(supertab);
  printcolor((uint8_t *)"4: The Imperial March\n", GRAY, BLACK);
  print(supertab);
  printcolor((uint8_t *)"5: Dale Dale Bo'\n", GRAY, BLACK);
  print(supertab);
  printcolor((uint8_t *)"6: Game Over! sound\n", GRAY, BLACK);
  print(supertab);
  printcolor((uint8_t *)"7: Terraria - Day\n", GRAY, BLACK);
  print(supertab);
  printcolor((uint8_t *)"8: Himno Nacional Argentino\n", GRAY, BLACK);
}

void check_div_by_zero() { divzero_excep_asm(); }

void check_invalid_opcode() { inopcode_excep_asm(); }

void get_registers() { sys_get_registers_asm(); }

void run_eliminator() {
  eliminator_menu();
  sys_clear_screen_asm();
}

void increase_text_size() {
  if (scale < MAX_TEXT_SIZE) {
    sys_set_text_size_asm(++scale);
  } else {
    print((uint8_t *)"Text at maximum size (2)\n");
  }
}

void decrease_text_size() {
  if (scale > MIN_TEXT_SIZE) {
    sys_set_text_size_asm(--scale);
  } else {
    print((uint8_t *)"Text at minimum size (1)\n");
  }
}

void get_time() {
  uint8_t buffer[9];
  sys_get_time_asm(buffer);
  printcolor(buffer, ORANGE, BLACK);
  putchar('\n');
}

void clear() { sys_clear_screen_asm(); }

void test_processes() {
  sys_create_process_asm(test_prio, 0, NULL, (uint8_t *)"test_priority", 1);
}

static char *status_to_string(process_status status) {
  switch (status) {
  case RUNNING:
    return "RUNNING";
  case READY:
    return "READY";
  case BLOCKED:
    return "BLOCKED";
  case KILLED:
    return "KILLED";
  case ZOMBIE:
    return "ZOMBIE";

  default:
    "invalid-status";
  }
}

void ps() {
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
    print(ps->info[i].in_fg ? "FG" : "BG");
    print((uint8_t *)"\n");
  }
  sys_free_ps_asm(ps);
}

void play_song(uint8_t idx) { song_dispatcher(idx); }

static uint8_t *commands[] = {
    (uint8_t *)"help",          (uint8_t *)"divzero",   (uint8_t *)"inopcode",
    (uint8_t *)"time",          (uint8_t *)"regstatus", (uint8_t *)"eliminator",
    (uint8_t *)"inctext",       (uint8_t *)"dectext",   (uint8_t *)"clear",
    (uint8_t *)"testprocesses", (uint8_t *)"ps"};

static void (*functions[])(void) = {help,
                                    check_div_by_zero,
                                    check_invalid_opcode,
                                    get_time,
                                    get_registers,
                                    run_eliminator,
                                    increase_text_size,
                                    decrease_text_size,
                                    clear,
                                    test_processes,
                                    ps};

uint64_t get_command(uint8_t *str) {
  for (int i = 0; i < (sizeof(commands) / sizeof(uint8_t *)); i++) {
    if (strcmp(commands[i], str) == 0) {
      functions[i]();
      return 1;
    }
  }
  uint8_t *command10 = (uint8_t *)"playsong";
  for (int j = 0; j < strlen(command10); j++)
    if (str[j] != command10[j])
      return 0;
  uint8_t idx = str[9] - '0';
  if (idx >= 1 && idx <= 8) {
    play_song(idx);
    return 1;
  }
  return 0;
}

void run_shell() {

  uint8_t command_buffer[COMM_BUFF_SIZE];

  play_startsound();

  sys_set_text_size_asm(1);
  scale = 1;
  printcolor((uint8_t *)"Type \"help\" to see a list of commands\n", WHITE,
             BLUE);

  while (1) {
    uint64_t buff_pos = 0;
    printcolor((uint8_t *)">: ", GREEN, BLACK);
    uint8_t c;

    while (((c = getchar()) != '\n' || buff_pos == 0) &&
           buff_pos < COMM_BUFF_SIZE) {

      if (c == '\b') {
        if (buff_pos > 0) {
          putchar('\b');
          buff_pos--;
        } else
          make_sound(120, 70);
      } else if (c != '\n') {
        command_buffer[buff_pos++] = c;
        putchar(c);
      }
    }

    putchar('\n');
    command_buffer[buff_pos] = '\0';

    if (get_command(command_buffer) == 0) {
      putcharcolor('\"', RED, BLACK);
      printcolor(command_buffer, RED, BLACK);
      putcharcolor('\"', RED, BLACK);
      printcolor((uint8_t *)" is not a command\n", RED, BLACK);
    }
  }
}

uint64_t testing(uint64_t argc, uint8_t **argv) {
  printcolor((uint8_t *)"Testing\n", ORANGE, BLACK);
  return 0;
}
