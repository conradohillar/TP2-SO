#include <../include/tests.h>

int64_t test_ipc_1(uint64_t argc, uint8_t *argv[]) {
  uint16_t pipe_id = satoi(argv[0]);
  uint64_t chars =
      sys_write_asm(pipe_id, (uint8_t *)"Hello from test_ipc_1", 21, 0, 0);
  uint8_t itoa_buffer[10];
  itoa(chars, itoa_buffer);
  print((uint8_t *)"\nChars written by test_ipc_1: ");
  print(itoa_buffer);
  return 0;
}

int64_t test_ipc_2(uint64_t argc, uint8_t *argv[]) {
  uint16_t pipe_id = satoi(argv[0]);
  uint8_t buffer[21];
  sys_read_asm(pipe_id, buffer, 20);
  print((uint8_t *)"\nMessage received by test_ipc_2: ");
  print(buffer);
  return 0;
}

int64_t test_ipc_fn(uint64_t argc, uint8_t *argv[]) {
  print((uint8_t *)"EXECUTING IPC TEST 1\n");
  uint16_t pipe_id = sys_create_pipe_asm();

  uint8_t itoa_buffer[10];
  itoa(pipe_id, itoa_buffer);

  uint8_t *args[] = {itoa_buffer};

  uint64_t pid_1 =
      sys_create_process_asm(test_ipc_1, 0, args, (uint8_t *)"test_ipc_1", 1);
  uint64_t pid_2 =
      sys_create_process_asm(test_ipc_2, 0, args, (uint8_t *)"test_ipc_2", 1);
  sys_waitpid_asm(pid_1);
  sys_waitpid_asm(pid_2);
  return 0;
}