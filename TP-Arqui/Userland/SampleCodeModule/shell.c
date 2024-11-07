// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>

#define MAX_TEXT_SIZE 2
#define MIN_TEXT_SIZE 1
#define COMM_BUFF_SIZE 128

static uint64_t scale = 1;
static uint8_t *status_to_string(process_status status);

void help() {
  uint8_t *supertab = (uint8_t *)"\t\t\t\t";
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
  printcolor((uint8_t *)"regstatus  ", ORANGE, BLACK);
  printcolor((uint8_t *)" - prints the values of the CPU registers\n", GRAY,
             BLACK);
  print((uint8_t *)"\t\t\t ");
  printcolor((uint8_t *)"press Ctrl + r to save the register states\n", GRAY,
             BLACK);
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
  printcolor((uint8_t *)"ps         ", ORANGE, BLACK);
  printcolor((uint8_t *)" - prints the process table\n", GRAY, BLACK);
  printcolor((uint8_t *)"mem        ", ORANGE, BLACK);
  printcolor((uint8_t *)" - prints the current status of the memory\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"loop       ", ORANGE, BLACK);
  printcolor((uint8_t *)" - prints hello and its pid every 3 seconds\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"kill PID   ", ORANGE, BLACK);
  printcolor((uint8_t *)" - kills the process with specified PID\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"block PID  ", ORANGE, BLACK);
  printcolor((uint8_t *)" - blocks the process with specified PID\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"unblock PID", ORANGE, BLACK);
  printcolor((uint8_t *)" - unblocks the process with specified PID\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"eliminator ", ORANGE, BLACK);
  printcolor((uint8_t *)" - play the \"Eliminator\" game\n", GRAY, BLACK);
  printcolor((uint8_t *)"inctext    ", ORANGE, BLACK);
  printcolor((uint8_t *)" - increases the size of the text, (max: 2)\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"dectext    ", ORANGE, BLACK);
  printcolor((uint8_t *)" - decreases the size of the text, (min: 1)\n", GRAY,
             BLACK);
  printcolor((uint8_t *)"clear      ", ORANGE, BLACK);
  printcolor((uint8_t *)" - clears the screen\n", GRAY, BLACK);
  printcolor((uint8_t *)"playsong N ", ORANGE, BLACK);
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

int64_t mem_writer(uint64_t argc, uint8_t *argv[]) {
  uint16_t new_fd = satoi(argv[0]);
  sys_set_fd_asm(STDOUT, new_fd);
  sys_mem_status_asm();
  return 0;
}

int64_t mem_reader(uint64_t argc, uint8_t *argv[]) {
  uint16_t new_fd = satoi(argv[0]);
  sys_set_fd_asm(STDIN, new_fd);
  while (1) {
    uint8_t buffer[2];
    while (1) {
      uint64_t count = sys_read_asm(new_fd, buffer, 1);
      buffer[count] = '\0';
      print(buffer);
    }
  }
  return -1;
}

void mem() {
  uint16_t pipe_id = sys_create_pipe_asm();
  uint8_t aux[10];
  itoa(pipe_id, aux);
  uint8_t *argv[] = {aux};
  uint64_t pid =
      sys_create_process_asm(mem_writer, 1, argv, (uint8_t *)"mem_writer", 0);
  uint64_t pid2 =
      sys_create_process_asm(mem_reader, 1, argv, (uint8_t *)"mem_reader", 0);
  sys_waitpid_asm(pid);
  sys_kill_asm(pid2);
  sys_destroy_pipe_asm(pipe_id);
}

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
  uint8_t *argv[] = {(uint8_t *)"8"};
  uint64_t pid = sys_create_process_asm(test_processes_fn, 1, argv,
                                        (uint8_t *)"test_priority", 1);
  sys_waitpid_asm(pid);
}

void test_priority() {
  uint64_t pid =
      sys_create_process_asm(test_prio, 0, NULL, (uint8_t *)"test_priority", 1);
  sys_waitpid_asm(pid);
}

void test_semaphores() {
  print((uint8_t *)"Running synchronization test...\n");
  char *argv_synchro[] = {"3", "1", NULL};
  char *argv_asynchro[] = {"3", "0", NULL};

  int64_t value;
  int64_t pid =
      sys_create_process_asm(test_sem_synchro_fn, 2, (uint8_t **)argv_synchro,
                             (uint8_t *)"test_semaphores", 1);
  value = sys_waitpid_asm(pid);
  // Desde aca
  print((uint8_t *)"\n Using semaphore -> Global: ");
  uint8_t aux[10];
  if (value < 0) {
    print((uint8_t *)"negativo\n");
    return;
  } else {
    itoa(value, aux);
    print(aux);
  }
  // Hasta aca, se puede borrar cuando funcione
  if (value == 0) {
    print((uint8_t *)"\n (1 / 2) Successfully Passed\n");
  } else {
    print((uint8_t *)"\n(1 / 2) NOT Passed\n");
  }
  pid =
      sys_create_process_asm(test_sem_synchro_fn, 2, (uint8_t **)argv_asynchro,
                             (uint8_t *)"test_semaphores", 1);
  value = sys_waitpid_asm(pid);
  // Desde aca
  print((uint8_t *)"\n Without semaphore -> Global: ");
  if (value < 0) {
    print((uint8_t *)"negativo\n");
    return;
  } else {
    itoa(value, aux);
    print(aux);
  }
  // Hasta aca, se puede borrar cuando funcione
  if (value != 0) {
    print((uint8_t *)"\n (2 / 2) Successfully Passed\n");
  } else {
    print((uint8_t *)"(2 / 2) NOT Passed\n");
  }
  return;
}

void test_ipc() {
  uint64_t pid =
      sys_create_process_asm(test_ipc_fn, 0, NULL, (uint8_t *)"test_ipc", 1);
  sys_waitpid_asm(pid);
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
    print((uint8_t *)(ps->info[i].in_fg ? "FG" : "BG"));
    print((uint8_t *)"\n");
  }
  sys_free_ps_asm(ps);
}

void loop() {
  sys_create_process_asm(loop_fn, 0, NULL, (uint8_t *)"loop", 0);
  return;
}

void play_song(uint8_t id, uint8_t aux) {
  if (id >= MIN_SONG_ID && id <= MAX_SONG_ID)
    song_dispatcher(id);
}

static uint8_t *commands[] = {
    (uint8_t *)"help",     (uint8_t *)"divzero",   (uint8_t *)"inopcode",
    (uint8_t *)"time",     (uint8_t *)"regstatus", (uint8_t *)"eliminator",
    (uint8_t *)"inctext",  (uint8_t *)"dectext",   (uint8_t *)"clear",
    (uint8_t *)"testproc", (uint8_t *)"testprio",  (uint8_t *)"ps",
    (uint8_t *)"testsem",  (uint8_t *)"testipc",   (uint8_t *)"mem",
    (uint8_t *)"loop"};

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
                                    test_priority,
                                    ps,
                                    test_semaphores,
                                    test_ipc,
                                    mem,
                                    loop};

uint64_t get_command(uint8_t *str) {
  for (int i = 0; i < (sizeof(commands) / sizeof(uint8_t *)); i++) {
    if (strcmp(commands[i], str) == 0) {
      functions[i]();
      return 1;
    }
  }

  uint8_t *command;
  shell_fn function;

  uint8_t *command1 = (uint8_t *)"playsong";
  uint8_t *command2 = (uint8_t *)"kill";
  uint8_t *command3 = (uint8_t *)"block";
  uint8_t *command4 = (uint8_t *)"unblock";
  uint8_t *command5 = (uint8_t *)"nice";

  switch (str[0]) {
  case 'p':
    command = command1;
    function = play_song;
    break;
  case 'k':
    command = command2;
    function = kill;
    break;
  case 'b':
    command = command3;
    function = block;
    break;
  case 'u':
    command = command4;
    function = unblock;
    break;
  case 'n':
    command = command5;
    function = nice;
    break;
  default:

    break;
  }

  for (int j = 0; j < strlen(command); j++) {
    if (str[j] != command[j])
      return 0;
  }

  uint8_t idx = str[strlen(command) + 1] - '0';
  if (str[strlen(command) + 3] != 0) {
    uint8_t aux = str[strlen(command) + 3] - '0';
    function(idx, aux);
    return 1;
  }
  function(idx, 0);

  return 1;
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
    uint8_t buff[1];

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

void kill(uint8_t id, uint8_t aux) { sys_kill_asm(id); }

void block(uint8_t id, uint8_t aux) { sys_block_asm(id); }

void unblock(uint8_t id, uint8_t aux) { sys_unblock_asm(id); }

void nice(uint8_t id, uint8_t aux) { sys_set_priority_asm(id, aux); }