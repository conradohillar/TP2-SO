// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <registers.h>

static uint64_t registers[17] = {
    0}; // 15,14,13,12,11,10,9,8,si,di,bp,d,c,b,a,ip,sp

static uint8_t saved_regs = 0;

void track_registers() {
  saved_regs = 1;
  get_registers(registers, 0x60);
}

void save_registers() {
  saved_regs = 1;
  get_registers(registers, 0x70);
}

void peek_registers() {
  if (!saved_regs) {
    put_string_nt(
        (uint8_t
             *)"Save registers with Ctrl + r before calling \"regstatus\"\n",
        ORANGE, BLACK);
    return;
  }
  put_string_nt((uint8_t *)"rax: 0x", WHITE, BLACK);
  print_hexa(registers[14]);
  put_string_nt((uint8_t *)"    ", BLACK, BLACK);
  put_string_nt((uint8_t *)"r8:  0x", WHITE, BLACK);
  print_hexa(registers[7]);
  put_char('\n', BLACK, BLACK);

  put_string_nt((uint8_t *)"rbx: 0x", WHITE, BLACK);
  print_hexa(registers[13]);
  put_string_nt((uint8_t *)"    ", BLACK, BLACK);
  put_string_nt((uint8_t *)"r9:  0x", WHITE, BLACK);
  print_hexa(registers[6]);
  put_char('\n', BLACK, BLACK);

  put_string_nt((uint8_t *)"rcx: 0x", WHITE, BLACK);
  print_hexa(registers[12]);
  put_string_nt((uint8_t *)"    ", BLACK, BLACK);
  put_string_nt((uint8_t *)"r10: 0x", WHITE, BLACK);
  print_hexa(registers[5]);
  put_char('\n', BLACK, BLACK);

  put_string_nt((uint8_t *)"rdx: 0x", WHITE, BLACK);
  print_hexa(registers[11]);
  put_string_nt((uint8_t *)"    ", BLACK, BLACK);
  put_string_nt((uint8_t *)"r11: 0x", WHITE, BLACK);
  print_hexa(registers[4]);
  put_char('\n', BLACK, BLACK);

  put_string_nt((uint8_t *)"rdi: 0x", WHITE, BLACK);
  print_hexa(registers[9]);
  put_string_nt((uint8_t *)"    ", BLACK, BLACK);
  put_string_nt((uint8_t *)"r12: 0x", WHITE, BLACK);
  print_hexa(registers[3]);
  put_char('\n', BLACK, BLACK);

  put_string_nt((uint8_t *)"rsi: 0x", WHITE, BLACK);
  print_hexa(registers[8]);
  put_string_nt((uint8_t *)"    ", BLACK, BLACK);
  put_string_nt((uint8_t *)"r13: 0x", WHITE, BLACK);
  print_hexa(registers[2]);
  put_char('\n', BLACK, BLACK);

  put_string_nt((uint8_t *)"rbp: 0x", WHITE, BLACK);
  print_hexa(registers[10]);
  put_string_nt((uint8_t *)"    ", BLACK, BLACK);
  put_string_nt((uint8_t *)"r14: 0x", WHITE, BLACK);
  print_hexa(registers[1]);
  put_char('\n', BLACK, BLACK);

  put_string_nt((uint8_t *)"rsp: 0x", WHITE, BLACK);
  print_hexa(registers[16]);
  put_string_nt((uint8_t *)"    ", BLACK, BLACK);
  put_string_nt((uint8_t *)"r15: 0x", WHITE, BLACK);
  print_hexa(registers[0]);
  put_char('\n', BLACK, BLACK);

  put_string_nt((uint8_t *)"rip: 0x", WHITE, BLACK);
  print_hexa(registers[15]);
  put_char('\n', BLACK, BLACK);

  saved_regs = 0;
}

void print_hexa(uint64_t num) {
  uint8_t buffer[64];
  for (int i = 0; i < 64; i++) {
    buffer[i] = '0';
  }
  uint_to_base(num, buffer, 16);
  put_string_nt(buffer, GRAY, BLACK);
}

uint32_t uint_to_base(uint64_t value, uint8_t *buffer, uint32_t base) {
  uint8_t *p = buffer;
  uint8_t *p1, *p2;
  uint32_t digits = 0;

  // Calculate characters for each digit
  do {
    uint32_t remainder = value % base;
    *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
    digits++;
  } while (value /= base);

  // Complete with '0'
  while (digits < 16) {
    *p++ = '0';
    digits++;
  }

  // Terminate string in buffer.
  *p = 0;

  // Reverse string in buffer.
  p1 = buffer;
  p2 = p - 1;
  while (p1 < p2) {
    uint8_t tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
    p1++;
    p2--;
  }

  return digits;
}
