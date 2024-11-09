#include <../include/tests.h>

int64_t test_ipc_1(uint64_t argc, uint8_t *argv[]) {
  if (argc == 2) {
    sys_set_fd_asm(STDOUT, satoi(argv[1]));
  }

  uint16_t pipe_id = satoi(argv[0]);
  uint8_t *message = (uint8_t *)"Hello from test_ipc_1";
  print((uint8_t *)"\nWriting message from test_ipc_1: ");
  printcolor(message, RED, BLACK);
  uint64_t chars = sys_write_asm(pipe_id, message, 21, 0, 0);
  uint8_t itoa_buffer[10];
  itoa(chars, itoa_buffer);
  print((uint8_t *)"\nChars written by test_ipc_1: ");
  printcolor(itoa_buffer, YELLOW, BLACK);
  print((uint8_t *)"\n");

  if (argc == 2) {
    sys_set_fd_asm(STDOUT, STDOUT);
  }
  return 0;
}

int64_t test_ipc_2(uint64_t argc, uint8_t *argv[]) {
  if (argc == 2) {
    sys_set_fd_asm(STDOUT, satoi(argv[1]));
  }

  uint16_t pipe_id = satoi(argv[0]);
  uint8_t buffer[50];
  uint64_t chars1 = sys_read_asm(pipe_id, buffer, 21);
  print((uint8_t *)"\nMessage received by test_ipc_2:\n");
  printcolor(buffer, ORANGE, BLACK);
  uint64_t chars2 = sys_read_asm(pipe_id, buffer, 21);
  printcolor(buffer, GREEN, BLACK);
  print((uint8_t *)"\n");
  uint8_t itoa_buffer[10];
  itoa(chars1 + chars2, itoa_buffer);
  print((uint8_t *)"Chars read by test_ipc_2: ");
  printcolor(itoa_buffer, YELLOW, BLACK);
  print((uint8_t *)"\n");

  if (argc == 2) {
    print((uint8_t *)"\0");
    sys_set_fd_asm(STDOUT, STDOUT);
  }
  return 0;
}

int64_t test_ipc_fn(uint64_t argc, uint8_t *argv[]) {
  uint16_t pipe_id = sys_create_pipe_asm();
  uint8_t itoa_buffer[10];
  itoa(pipe_id, itoa_buffer);

  uint8_t *args[2] = {itoa_buffer, (uint8_t *)"'\0"};
  uint8_t my_argc = 1;

  if (argc) {
    my_argc = 2;
    args[1] = argv[0];
  }

  print((uint8_t *)"EXECUTING IPC TEST\n");
  uint64_t pid_1 = sys_create_process_asm(test_ipc_1, my_argc, args,
                                          (uint8_t *)"test_ipc_1", 1);
  sys_waitpid_asm(pid_1);
  uint64_t pid2 = sys_create_process_asm(test_ipc_1, my_argc, args,
                                         (uint8_t *)"test_ipc_1", 1);
  sys_waitpid_asm(pid2);
  uint64_t pid_3 = sys_create_process_asm(test_ipc_2, my_argc, args,
                                          (uint8_t *)"test_ipc_2", 1);
  sys_waitpid_asm(pid_3);
  sys_destroy_pipe_asm(pipe_id);
  return 0;
}