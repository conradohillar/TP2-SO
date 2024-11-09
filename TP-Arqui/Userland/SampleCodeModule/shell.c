// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>

#define MAX_TEXT_SIZE 2
#define MIN_TEXT_SIZE 1
#define COMM_BUFF_SIZE 128

static uint64_t scale = 1;

void help(uint8_t in_fg) {
  uint64_t pid =
      sys_create_process_asm(help_fn, 0, NULL, (uint8_t *)"help", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
  return;
}

void check_div_by_zero(uint8_t in_fg) { divzero_excep_asm(); }

void check_invalid_opcode(uint8_t in_fg) { inopcode_excep_asm(); }

void get_registers(uint8_t in_fg) { sys_get_registers_asm(); }

void mem(uint8_t in_fg) {
  uint64_t pid =
      sys_create_process_asm(mem_fn, 0, NULL, (uint8_t *)"mem", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
  return;
}

void run_eliminator(uint8_t in_fg) {
  eliminator_menu();
  sys_clear_screen_asm();
}

void increase_text_size(uint8_t in_fg) {
  if (scale < MAX_TEXT_SIZE) {
    sys_set_text_size_asm(++scale);
  } else {
    print((uint8_t *)"Text at maximum size (2)\n");
  }
}

void decrease_text_size(uint8_t in_fg) {
  if (scale > MIN_TEXT_SIZE) {
    sys_set_text_size_asm(--scale);
  } else {
    print((uint8_t *)"Text at minimum size (1)\n");
  }
}

void get_time(uint8_t in_fg) {
  uint8_t buffer[9];
  sys_get_time_asm(buffer);
  printcolor(buffer, ORANGE, BLACK);
  putchar('\n');
}

void clear(uint8_t in_fg) { sys_clear_screen_asm(); }

void test_processes(uint8_t in_fg) {
  uint8_t *argv[] = {(uint8_t *)"8"};
  uint64_t pid = sys_create_process_asm(test_processes_fn, 1, argv,
                                        (uint8_t *)"test_priority", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
}

void test_priority(uint8_t in_fg) {
  uint64_t pid = sys_create_process_asm(test_prio, 0, NULL,
                                        (uint8_t *)"test_priority", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
}

void test_semaphores(uint8_t in_fg) {
  uint64_t pid = sys_create_process_asm(test_semaphores_fn, 0, NULL,
                                        (uint8_t *)"test_semaphores", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
}

void test_ipc(uint8_t in_fg) {
  uint64_t pid = sys_create_process_asm(test_ipc_fn, 0, NULL,
                                        (uint8_t *)"test_ipc", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
}

void ps(uint8_t in_fg) {
  uint64_t pid = sys_create_process_asm(ps_fn, 0, NULL, (uint8_t *)"ps", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
  return;
}

void loop(uint8_t in_fg) {
  uint64_t pid =
      sys_create_process_asm(loop_fn, 0, NULL, (uint8_t *)"loop", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
  return;
}

void cat(uint8_t in_fg) {
  uint64_t pid =
      sys_create_process_asm(cat_fn, 0, NULL, (uint8_t *)"cat", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
  return;
}

void wc(uint8_t in_fg) {
  uint64_t pid = sys_create_process_asm(wc_fn, 0, NULL, (uint8_t *)"wc", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
  return;
}

void filter(uint8_t in_fg) {
  uint64_t pid =
      sys_create_process_asm(filter_fn, 0, NULL, (uint8_t *)"filter", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
  return;
}

// void pipe_functions(shell_fn fn1, shell_fn fn2) {
//   int16_t pipe_id = sys_create_pipe_asm();
//   if (pipe_id < 0) {
//     printerr((uint8_t *)"Error creating pipe\n");
//     return;
//   }
//   uint8_t aux[10];
//   itoa(pipe_id, aux);
//   uint8_t *args[] = {aux};
//   uint64_t pid1 = sys_create_process_asm(fn1, 1, args, (uint8_t *)"pipe1",
//   0); uint64_t pid2 = sys_create_process_asm(fn2, 1, args, (uint8_t
//   *)"pipe2", 0); sys_waitpid_asm(pid1); sys_kill_asm(pid2);
// }

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
    (uint8_t *)"loop",     (uint8_t *)"cat",       (uint8_t *)"wc",
    (uint8_t *)"filter"};

static shell_fn functions[] = {help,
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
                               loop,
                               cat,
                               wc,
                               filter};
static uint8_t found_command = 0;

uint64_t get_command(uint8_t *str) {
  found_command = 0;
  uint8_t input[MAX_PARAMS][MAX_PARAM_LENGTH] = {0};
  int8_t count = split_string(str, input);
  if (count == -1) {
    printerr((uint8_t *)"Too many arguments or an argument is too long.\n");
    return 0;
  }

  if (count == 1) {
    // Execute in fg
    for (int i = 0; i < (sizeof(commands) / sizeof(uint8_t *)); i++) {
      if (strcmp(commands[i], input[0]) == 0) {
        functions[i](1);
        return 1;
      }
    }
  }

  if (count == 2 && strcmp(input[1], (uint8_t *)"&") == 0) {
    // Execute in bg
    for (int i = 0; i < (sizeof(commands) / sizeof(uint8_t *)); i++) {
      if (strcmp(commands[i], input[0]) == 0) {
        functions[i](0);
        return 1;
      }
    }
  }

  uint8_t *command;
  param_shell_fn function;

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
    if (input[0][j] != command[j])
      return 0;
  }

  if (count == 2) {
    uint8_t idx = satoi(input[1]);
    function(idx, 0);
    return 1;
  }

  if (count == 3) {
    if (strcmp(input[2], (uint8_t *)"|") != 0) {
      uint8_t idx = satoi(input[1]);
      uint8_t aux = satoi(input[2]);
      function(idx, aux);
      return 1;
    } else {
      shell_fn fn1 = NULL, fn2 = NULL;
      for (int i = 0; i < (sizeof(commands) / sizeof(uint8_t *)); i++) {
        if (strcmp(commands[i], input[0]) == 0) {
          fn1 = functions[i];
          break;
        }
      }
      for (int i = 0; i < (sizeof(commands) / sizeof(uint8_t *)); i++) {
        if (strcmp(commands[i], input[3]) == 0) {
          fn2 = functions[i];
          break;
        }
      }
      if (fn1 == NULL || fn2 == NULL) {
        return 0;
      }
      // pipe_functions(fn1, fn2); // TODO: Implement pipe functions
      return 1;
    }
  }

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
      putcharerr('\"');
      printerr(command_buffer);
      putcharerr('\"');
      printerr((uint8_t *)" is not a command\n");
    }
  }
}

void kill(uint8_t pid, uint8_t aux) { sys_kill_asm(pid); }

void block(uint8_t pid, uint8_t aux) { sys_block_asm(pid); }

void unblock(uint8_t pid, uint8_t aux) { sys_unblock_asm(pid); }

void nice(uint8_t pid, uint8_t aux) { sys_set_priority_asm(pid, aux); }