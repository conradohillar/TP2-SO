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
  load_idt();

  put_string_nt("Inicializando Memory Manager\n", 0x00FF00, 0x000000);
  MemoryManagerADT mem_manager =
      create_memory_manager(MEM_START_ADDRESS, MEM_MANAGER_ADDRESS);
  if (mem_manager == '\0') {
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
  put_string_nt("Creando my_scheduler\n", 0x00FF00, 0x000000);
  my_scheduler = create_scheduler();
  if (my_scheduler == NULL) {
    put_string_nt("Error creando my_scheduler\n", 0xFF0000, 0x000000);
    return -1;
  }
  put_string_nt("Creando process manager\n", 0x00FF00, 0x000000);
  my_pm = create_process_manager();
  if (my_pm == NULL) {
    put_string_nt("Error creando process manager\n", 0xFF0000, 0x000000);
    return -1;
  }
  // put_string_nt("Ejecutando test_processes\n", 0x00FF00, 0x000000);
  // char *argv[] = {"64"};
  // test_processes(1, argv, my_pm, my_scheduler);

  create_process(my_scheduler, my_pm, &initializeKernelBinary, NULL, 0,
                 "test_processes1", 0, 1, 1, 1);

  create_process(my_scheduler, my_pm, &initializeKernelBinary, NULL, 0,
                 "test_processes2", 0, 1, 1, 1);

  process_control_block *pcb = getPCB(my_pm, 1);
  if (pcb == NULL) {
    put_string_nt("Error obteniendo pcb\n", 0xFF0000, 0x000000);
    return -1;
  }
  if (pcb->status != READY) {
    put_string_nt("Error en el estado del pcb\n", 0xFF0000, 0x000000);
    return -1;
  }

  print_scheduler(my_scheduler);

  put_string_nt("Removing ...\n", 0x00FF00, 0x000000);

  remove_from_scheduler(my_scheduler, pcb);

  print_scheduler(my_scheduler);

  mm_free(my_pm);
  put_string_nt("Liberando my_pm\n", 0x00FF00, 0x000000);
  mm_free(my_scheduler);
  put_string_nt("Liberando my_scheduler\n", 0x00FF00, 0x000000);
  /////////////////////////////////////////////////////////////////////////////////////////////////

  // ((EntryPoint)sampleCodeModuleAddress)();
  return 0;
}
