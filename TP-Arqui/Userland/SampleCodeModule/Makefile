include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
OBJECTS_ASM=syscaller.o exceptionsTester.o

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(ASM) $(ASMFLAGS) syscaller.asm
	$(ASM) $(ASMFLAGS) exceptionsTester.asm
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld _loader.c $(SOURCES) $(OBJECTS_ASM) -o ../$(MODULE)
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld -Wl,--oformat=elf64-x86-64 _loader.c $(SOURCES) $(OBJECTS_ASM) -o ../0000-sampleCodeModule.elf


clean:
	rm -rf *.o

.PHONY: all clean print
