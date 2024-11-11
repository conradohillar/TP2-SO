// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>

#define MAX_TEXT_SIZE 2
#define MIN_TEXT_SIZE 1
#define COMM_BUFF_SIZE 128

static uint64_t scale = 1;

static int64_t fn_wrapper(uint64_t argc, uint8_t *argv[], fn function,
                          uint8_t *name) {
  uint64_t child_argc = 0;
  uint8_t *child_argv[1] = {(uint8_t *)'\0'};
  if (argc == 2) {
    child_argc = 1;
    child_argv[0] = argv[1];
  }
  uint8_t in_fg = satoi(argv[0]);
  uint64_t pid =
      sys_create_process_asm(function, child_argc, child_argv, name, in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
  return 1;
}

int64_t help(uint64_t argc, uint8_t *argv[]) {
  return fn_wrapper(argc, argv, help_fn, (uint8_t *)"help");
}

int64_t check_div_by_zero(uint64_t argc, uint8_t *argv[]) {
  divzero_excep_asm();
  return 1;
}

int64_t check_invalid_opcode(uint64_t argc, uint8_t *argv[]) {
  inopcode_excep_asm();
  return 1;
}

int64_t get_registers(uint64_t argc, uint8_t *argv[]) {
  sys_get_registers_asm();
  return 1;
}

int64_t mem(uint64_t argc, uint8_t *argv[]) {
  return fn_wrapper(argc, argv, mem_fn, (uint8_t *)"mem");
}

int64_t philos(uint64_t argc, uint8_t *argv[]) {
  return fn_wrapper(argc, argv, philos_fn, (uint8_t *)"philos");
}

int64_t run_eliminator(uint64_t argc, uint8_t *argv[]) {
  eliminator_menu();
  sys_clear_screen_asm();
  return 1;
}

int64_t increase_text_size(uint64_t argc, uint8_t *argv[]) {
  if (scale < MAX_TEXT_SIZE) {
    sys_set_text_size_asm(++scale);
  } else {
    print((uint8_t *)"Text at maximum size (2)\n");
  }
  return 1;
}

int64_t decrease_text_size(uint64_t argc, uint8_t *argv[]) {
  if (scale > MIN_TEXT_SIZE) {
    sys_set_text_size_asm(--scale);
  } else {
    print((uint8_t *)"Text at minimum size (1)\n");
  }
  return 1;
}

int64_t get_time(uint64_t argc, uint8_t *argv[]) {
  uint8_t buffer[9];
  sys_get_time_asm(buffer);
  printcolor(buffer, ORANGE, BLACK);
  putchar('\n');
  return 1;
}

int64_t clear(uint64_t argc, uint8_t *argv[]) {
  sys_clear_screen_asm();
  return 1;
}

int64_t test_processes(uint64_t argc, uint8_t *argv[]) {
  uint8_t my_argc = 1;
  uint8_t *args[2] = {(uint8_t *)"8", (uint8_t *)"\0"};
  if (argc == 2) {
    my_argc = 2;
    args[1] = argv[1];
  }
  uint8_t in_fg = satoi(argv[0]);
  uint64_t pid = sys_create_process_asm(test_processes_fn, my_argc, args,
                                        (uint8_t *)"test_processes", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
  return 1;
}

int64_t test_priority(uint64_t argc, uint8_t *argv[]) {
  return fn_wrapper(argc, argv, test_prio_fn, (uint8_t *)"test_priority");
}

int64_t test_semaphores(uint64_t argc, uint8_t *argv[]) {
  return fn_wrapper(argc, argv, test_semaphores_fn, (uint8_t *)"test_sem");
}

int64_t test_ipc(uint64_t argc, uint8_t *argv[]) {
  return fn_wrapper(argc, argv, test_ipc_fn, (uint8_t *)"test_ipc");
}

int64_t test_mm(uint64_t argc, uint8_t *argv[]) {
  uint8_t my_argc = 1;
  uint8_t *args[2] = {argv[1], (uint8_t *)"\0"};
  if (argc == 3) {
    my_argc = 2;
    args[1] = argv[2];
  }
  uint8_t in_fg = satoi(argv[0]);
  uint64_t pid = sys_create_process_asm(test_mm_fn, my_argc, args,
                                        (uint8_t *)"test_mm", in_fg);
  if (in_fg) {
    sys_waitpid_asm(pid);
  }
  return 1;
}

int64_t ps(uint64_t argc, uint8_t *argv[]) {
  return fn_wrapper(argc, argv, ps_fn, (uint8_t *)"ps");
}

int64_t loop(uint64_t argc, uint8_t *argv[]) {
  return fn_wrapper(argc, argv, loop_fn, (uint8_t *)"loop");
}

int64_t cat(uint64_t argc, uint8_t *argv[]) {
  return fn_wrapper(argc, argv, cat_fn, (uint8_t *)"cat");
}

int64_t wc(uint64_t argc, uint8_t *argv[]) {
  return fn_wrapper(argc, argv, wc_fn, (uint8_t *)"wc");
}

int64_t filter(uint64_t argc, uint8_t *argv[]) {
  return fn_wrapper(argc, argv, filter_fn, (uint8_t *)"filter");
}

void pipe_functions(fn fn1, fn fn2) {
  int16_t pipe_id = sys_create_pipe_asm();
  if (pipe_id < 0) {
    printerr((uint8_t *)"Error creating pipe\n");
    return;
  }
  uint8_t aux[10];
  itoa(pipe_id, aux);
  uint8_t *args_bg[] = {(uint8_t *)"0", aux};
  uint8_t *args_fg[] = {(uint8_t *)"1", aux};
  uint64_t pid1 =
      sys_create_process_asm(fn1, 2, args_bg, (uint8_t *)"pipe1", 0);
  uint64_t pid2 =
      sys_create_process_asm(fn2, 2, args_fg, (uint8_t *)"pipe2", 1);
  sys_waitpid_asm(pid2);
  sys_kill_asm(pid1);
  sys_destroy_pipe_asm(pipe_id);
}

void play_song(uint8_t id) {
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
    (uint8_t *)"filter",   (uint8_t *)"philos"};

static fn functions[] = {help,
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
                         filter,
                         philos};

uint64_t get_command(uint8_t *str) {
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
        uint8_t *args[] = {(uint8_t *)"1"};
        functions[i](1, args);
        return 1;
      }
    }
    return 0;
  }

  if (count == 2) {

    if (strcmp(input[1], (uint8_t *)"&") == 0) {
      // Execute in bg
      for (int i = 0; i < (sizeof(commands) / sizeof(uint8_t *)); i++) {
        if (strcmp(commands[i], input[0]) == 0) {
          uint8_t *args[] = {(uint8_t *)"0"};
          return functions[i](1, args);
          return 1;
        }
      }
    }

    uint8_t *command;
    one_param_fn function;

    uint8_t *command1 = (uint8_t *)"playsong";
    uint8_t *command2 = (uint8_t *)"kill";
    uint8_t *command3 = (uint8_t *)"block";
    uint8_t *command4 = (uint8_t *)"unblock";

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

    default:

      break;
    }

    if (!is_number(input[1])) {
      return 0;
    }
    if (strcmp(input[0], (uint8_t *)"testmem") == 0) {
      uint8_t *args[] = {(uint8_t *)"1", input[1]};
      uint64_t argc = 2;
      test_mm(argc, args);
      return 1;
    }

    for (int j = 0; j < strlen(command); j++) {
      if (input[0][j] != command[j]) {
        return 0;
      }
    }

    uint8_t id = satoi(input[1]);
    function(id);
    return 1;
  }

  if (count == 3) {
    if ((strcmp(input[0], (uint8_t *)"testmem") == 0) && is_number(input[1]) &&
        (strcmp(input[2], (uint8_t *)"&") == 0)) {
      uint8_t *args[] = {(uint8_t *)"0", input[1]};
      uint64_t argc = 2;
      test_mm(argc, args);
      return 1;
    }
    if (strcmp(input[0], (uint8_t *)"nice") == 0) {
      uint8_t pid = satoi(input[1]);
      uint8_t new_priority = satoi(input[2]);
      nice(pid, new_priority);
      return 1;
    } else if (strcmp(input[1], (uint8_t *)"|") == 0) {
      fn fn1 = NULL, fn2 = NULL;
      for (int i = 0; i < (sizeof(commands) / sizeof(uint8_t *)); i++) {
        if (strcmp(commands[i], input[0]) == 0) {
          fn1 = functions[i];
          break;
        }
      }
      for (int i = 0; i < (sizeof(commands) / sizeof(uint8_t *)); i++) {
        if (strcmp(commands[i], input[2]) == 0) {
          fn2 = functions[i];
          break;
        }
      }
      if (fn1 == NULL || fn2 == NULL) {
        return 0;
      }
      pipe_functions(fn1, fn2);
      return 1;
    } else if (strcmp(input[0], (uint8_t *)"kill") == 0 &&
               strcmp(input[1], (uint8_t *)"name") == 0) {
      kill_by_name(input[2]);
      return 1;
    }
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
    kill_zombies();
    uint64_t buff_pos = 0;
    printcolor((uint8_t *)">: ", GREEN, BLACK);
    uint8_t c;

    while (((c = getchar()) != '\n' || buff_pos == 0) &&
           buff_pos < COMM_BUFF_SIZE) {

      if (c == '\b') {
        if (buff_pos > 0 && command_buffer[buff_pos - 1] != '\0') {
          putchar('\b');
          buff_pos--;
        } else
          make_sound(120, 70);
      } else if (c != '\n' && c != '\0') {
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
      printerr((uint8_t *)" is not a valid command\n");
    }
  }
}

void kill(uint8_t pid) { sys_kill_asm(pid); }

void kill_by_name(uint8_t *name) { sys_kill_by_name_asm(name); }

void kill_zombies() {
  ps_struct *ps = (ps_struct *)sys_ps_asm();
  for (int i = 0; i < ps->count; i++) {
    if (ps->info[i].state == ZOMBIE) {
      sys_kill_asm(ps->info[i].pid);
    }
  }
  sys_free_ps_asm(ps);
}

void block(uint8_t pid) { sys_block_asm(pid); }

void unblock(uint8_t pid) { sys_unblock_asm(pid); }

void nice(uint8_t pid, uint8_t new_priority) {
  sys_set_priority_asm(pid, new_priority);
}