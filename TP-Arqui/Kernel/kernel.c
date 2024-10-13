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

  memoryManagerADT mem_manager =
      create_memory_manager(MEM_START_ADDRESS, MEM_MANAGER_ADDRESS);
  if (mem_manager == NULL) {
    put_string_nt("Failed to initialize Memory Manager\n", 0xFF0000, 0x000000);
    return -1;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Testeo del Memory Manager

  //   put_string_nt("Creando variables\n", 0x00FF00, 0x000000);

  //   uint64_t argc = 2;
  //   char *argv[] = {"1000"};

  //   put_string_nt("Ejecutando test_mm\n", 0x00FF00, 0x000000);

  //   test_mm(argc - 1, argv);

  //   put_string_nt("Error de test_mm\n", 0xFF0000, 0x000000);

  /////////////////////////////////////////////////////////////////////////////////////////////////
  // Testeo de los procesos
  my_scheduler = create_scheduler();
  if (my_scheduler == NULL) {
    put_string_nt("Error creando scheduler\n", 0xFF0000, 0x000000);
    return -1;
  }

  my_pm = create_process_manager(my_scheduler);
  if (my_pm == NULL) {
    put_string_nt("Error creando process manager\n", 0xFF0000, 0x000000);
    return -1;
  }

  _sti();

  destroy_process_table(my_pm);
  put_string_nt("\nLiberando my_pm\n", 0x00FF00, 0x000000);

  return 0;
}
