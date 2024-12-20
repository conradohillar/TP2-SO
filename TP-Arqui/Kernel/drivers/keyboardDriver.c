// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboardDriver.h>
#include <pipes.h>
#include <process.h>

extern processManagerADT my_pm;
extern schedulerADT my_scheduler;
extern pipeManagerADT my_pipe_manager;

static uint8_t qwerty_ES_lowercase[] = {
    '\0', '\e', '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8', '9',  '0',
    '\'', '\0', '\b', '\t', 'q',  'w',  'e',  'r',  't',  'y', 'u',  'i',
    'o',  'p',  '\0', '+',  '\n', '\0', 'a',  's',  'd',  'f', 'g',  'h',
    'j',  'k',  'l',  '\0', '{',  '|',  '\0', '}',  'z',  'x', 'c',  'v',
    'b',  'n',  'm',  ',',  '.',  '-',  '\0', '\0', '\0', ' ', '\0',
};

static uint8_t qwerty_ES_uppercase[] = {
    '\0', '\e', '!',  '"',  '#',  '$',  '%',  '&',  '/',  '(', ')',  '=',
    '?',  '\0', '\b', '\t', 'Q',  'W',  'E',  'R',  'T',  'Y', 'U',  'I',
    'O',  'P',  '\0', '*',  '\n', '\0', 'A',  'S',  'D',  'F', 'G',  'H',
    'J',  'K',  'L',  '\0', '[',  '\0', '\0', ']',  'Z',  'X', 'C',  'V',
    'B',  'N',  'M',  ';',  ':',  '_',  '\0', '\0', '\0', ' ', '\0',
};

//  pos       0      1       2       3
//  key     ctr-l   sh-l  capslock  sh-r
uint8_t state[4] = {0};

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
    if (key_scan_code < sizeof(qwerty_ES_lowercase)) {
      ascii = qwerty_ES_lowercase[key_scan_code];
      if (((state[1] || state[3]) != state[2]) && is_letter(ascii)) {
        ascii = qwerty_ES_uppercase[key_scan_code];
      } else if ((state[1] || state[3]))
        ascii = qwerty_ES_uppercase[key_scan_code];
    }
  }
  if (state[0] && (ascii == KILL_PROCESS_SHORTCUT ||
                   ascii == KILL_PROCESS_SHORTCUT - ('a' - 'A'))) {

    uint8_t *name = kill_process_in_fg(my_pm);
    if (name != NULL) {
      put_string_nt((uint8_t *)"\nProcess \"", YELLOW, BLACK);
      put_string_nt(name, YELLOW, BLACK);
      put_string_nt((uint8_t *)"\" terminated by Ctrl + C\n", YELLOW, BLACK);
    }
    ascii = 0;
    write_pipe(my_pipe_manager, get_pipe(my_pipe_manager, STDIN), &ascii, 1);
    return;
  }
  if (state[0] &&
      (ascii == EOF_SHORTCUT || ascii == EOF_SHORTCUT - ('a' - 'A'))) {
    ascii = 0;
    write_pipe(my_pipe_manager, get_pipe(my_pipe_manager, STDIN), &ascii, 1);
    return;
  }

  if (state[0]) {
    ascii = 0;
  }

  // Only write to the pipe if there is space
  pipe_t *stdin_pipe = get_pipe(my_pipe_manager, STDIN);

  if (ascii && stdin_pipe->to_read_count < PIPE_BUFFER_SIZE) {
    write_pipe(my_pipe_manager, stdin_pipe, &ascii, 1);
  }
  ascii = 0;
}

uint32_t get_key() {
  uint32_t aux = key_scan_code;
  key_scan_code = '\0';
  return aux;
}

uint8_t get_ascii() {
  uint8_t aux = ascii;
  ascii = 0;
  return aux;
}
