USE_BUDDY ?= 1

all:  bootloader kernel userland image toolchain

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all USE_BUDDY=$(USE_BUDDY)
	# default value: USE_BUDDY=1, change to 0 if you want to use the freeArray memory manager
userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

toolchain:
	cd Toolchain; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image collections kernel userland all clean
