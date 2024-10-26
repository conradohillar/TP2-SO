// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/textMode.h"
#include <kernel.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

schedulerADT my_scheduler = NULL;
processManagerADT my_pm = NULL;
semaphoreManagerADT my_sm = NULL;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;

void clearBSS(void *bssAddress, uint64_t bssSize) {
  memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
  return (void *)((uint64_t)&endOfKernel +
                  PageSize * 8       // The size of the stack itself, 32KiB
                  - sizeof(uint64_t) // Begin at the top of the stack
  );
}

void *initializeKernelBinary() {
  void *moduleAddresses[] = {sampleCodeModuleAddress, sampleDataModuleAddress};
  loadModules(&endOfKernelBinary, moduleAddresses);
  clearBSS(&bss, &endOfKernel - &bss);
  return getStackBase();
}
int main() {
  _cli();

  load_idt();

  memoryManagerADT mem_manager = create_memory_manager(
      (void *)MEM_START_ADDRESS, (memoryManagerADT)MEM_MANAGER_ADDRESS);
  if (mem_manager == NULL) {
    put_string_nt((uint8_t *)"Failed to initialize Memory Manager\n", 0xFF0000,
                  0x000000);
    return -1;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Testeo del Memory Manager

  //   put_string_nt ((uint8_t *)"Creando variables\n", 0x00FF00, 0x000000);

  //   uint64_t argc = 2;
  //   char *argv[] = {"1000"};

  //   put_string_nt ((uint8_t *)"Ejecutando test_mm\n", 0x00FF00, 0x000000);

  //   test_mm(argc - 1, argv);

  //   put_string_nt ((uint8_t *)"Error de test_mm\n", 0xFF0000, 0x000000);

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Testeo de los procesos
  my_scheduler = create_scheduler();
  if (my_scheduler == NULL) {
    put_string_nt((uint8_t *)"Error creando scheduler\n", 0xFF0000, 0x000000);
    return -1;
  }
  
  my_pm = create_process_manager(my_scheduler);
  if (my_pm == NULL) {
    put_string_nt((uint8_t *)"Error creando process manager\n", 0xFF0000,
                  0x000000);
    return -1;
  }

  my_sm = create_semaphore_manager();
  if (my_sm == NULL) {
    put_string_nt((uint8_t *)"Error creando semaphore manager\n", 0xFF0000,
                  0x000000);
    return -1;
  }

  _sti();

  destroy_semaphore_manager(my_sm);
  put_string_nt((uint8_t *)"\nLiberando my_sm\n", 0x00FF00, 0x000000);
  destroy_process_table(my_pm);
  put_string_nt((uint8_t *)"\nLiberando my_pm\n", 0x00FF00, 0x000000);

  return 0;
}
