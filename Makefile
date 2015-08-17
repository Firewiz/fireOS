export PATH := gcc-cross/bin/:$(PATH)
CC=i686-elf-gcc
AS=nasm

CFLAGS=-ffreestanding -nostdlib -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -g
ASFLAGS=-felf32 -F stabs -g

OBJS=kernel.o vga.o stdlib.o asmintr.o idt_c.o inthandle.o irq.o keyboard.o disk.o fat.o printf.o malloc.o boot.o idt_s.o mt.o syscall_c.o syscall_s.o paging.o timer.o shell.o elf.o gdt_c.o gdt_s.o
PROG=fireos.bin

all:version.h $(PROG) libc modules
version.h:
	chmod +x mkver
	./mkver
$(PROG):$(OBJS)
	$(CC) -T linker.ld -o $(PROG) -ffreestanding -O2 -nostdlib $(OBJS) -lgcc
	objcopy --only-keep-debug fireos.bin fireos.sym
	objcopy --strip-debug fireos.bin
clean:
	rm -f *.o $(PROG) fireos.sym version.h *~
	cd modules ; make clean
libc: build_libc install_libc
build_libc:
	cd libc ; make
install_libc:

modules: build_modules install_modules
build_modules:
	cd modules ; make
install_modules:
	cp modules/*.exe fs_mount/
	sync
