include Makefile.inc
USE_BUDDY ?= 1

KERNEL=kernel.bin
SOURCES=$(wildcard *.c) $(wildcard ./drivers/*.c) $(wildcard ./processes/*.c) $(wildcard ./semaphores/*.c) $(wildcard ./tests/*.c)

ifeq ($(USE_BUDDY), 1)
    SOURCES += ./memory/buddy.c
else
    SOURCES += ./memory/freeArray.c
endif

SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) 

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf $(OBJECTS) $(OBJECTS_ASM) $(LOADEROBJECT) $(KERNEL) kernel.elf $(wildcard memory/*.o)
.PHONY: all clean