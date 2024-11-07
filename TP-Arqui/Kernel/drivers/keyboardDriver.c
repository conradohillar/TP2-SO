// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboardDriver.h>
#include <pipes.h>
#include <process.h>

extern processManagerADT my_pm;
extern schedulerADT my_scheduler;
extern pipeManagerADT my_pipe_manager;

static uint8_t qwerty_US[] = {
    '\0', '\e', '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8', '9',  '0',
    '-',  '=',  '\b', '\t', 'q',  'w',  'e',  'r',  't',  'y', 'u',  'i',
    'o',  'p',  '[',  ']',  '\n', '\0', 'a',  's',  'd',  'f', 'g',  'h',
    'j',  'k',  'l',  ';',  '\'', '`',  '\0', '\\', 'z',  'x', 'c',  'v',
    'b',  'n',  'm',  ',',  '.',  '/',  '\0', '*',  '\0', ' ', '\0',
};

// es un vector de unos o ceros. uno dice que la tecla esta presionada y cero
// que no
//  pos      0       1       2           3
// tecla     ctr l   sh l    capslock    sh r
uint8_t state[3] = {0};

static uint32_t key_scan_code = '\0';
static uint8_t ascii = '\0';

uint8_t is_letter(uint8_t c) { return (('a' <= c) && (c <= 'z')); }

void keyboard_handler() {
  uint32_t read = read_keyboard();
  switch (read) {
  case 0x1D:
    state[0] = 1;
    break;
  case 0x2A:
    state[1] = 1;
    break;
  case 0x36:
    state[3] = 1;
    break;
  case 0x3A:
    state[2] = (!state[2]);
    break;
  case 0x9D:
    state[0] = 0;
    break;
  case 0xAA:
    state[1] = 0;
    break;
  case 0xB6:
    state[3] = 0;
    break;

  default:
    key_scan_code = read;
    if (key_scan_code < sizeof(qwerty_US) && key_scan_code >= 0) {
      ascii = qwerty_US[key_scan_code];
      if ((((state[1] || state[3]) && !state[2]) ||
           (state[2] && !(state[1] || state[3]))) &&
          is_letter(ascii))
        ascii -= 'a' - 'A';
    }
    if (state[0] && (ascii == SAVE_REGS_SHORTCUT)) {
      save_registers();
    }
    if (state[0] && (ascii == KILL_PROCESS_SHORTCUT)) {
      uint64_t pid = getpid(my_pm);
      process_control_block *pcb;
      while (pid != INIT_PID) {
        pcb = get_PCB(my_pm, pid);
        if (pcb->in_fg) {
          break;
        }
        pid = pcb->parent_pid;
      }
      if (pid) {
        kill(my_pm, pid);
      }
    }
    if (state[0]) {
      ascii = 0;
    }
  }

  // Only write to the pipe if there is space
  pipe_t *stdin_pipe = get_pipe(my_pipe_manager, STDIN);

  if (ascii && stdin_pipe->to_read_count < PIPE_BUFFER_SIZE) {
    write_pipe(my_pipe_manager, stdin_pipe, &ascii, 1);
    ascii = 0;
  }
}

uint32_t get_key() {
  uint32_t aux = key_scan_code;
  key_scan_code = '\0';
  return aux;
}

/*
Retorna el valor de ascii correspondiente al scancode
de la tecla presionada o '\0' si no es imprimible.
*/
uint8_t get_ascii() {
  uint8_t aux = ascii;
  ascii = 0;
  return aux;
}
