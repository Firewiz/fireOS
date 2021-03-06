export PATH := gcc-cross/bin/:$(PATH)
CC=i686-elf-gcc
AS=nasm

CFLAGS=-ffreestanding -nostdlib -Wall -Wextra -g
ASFLAGS=-felf32 -F stabs -g

HDRS=vga.h stdlib.h asmintr.h idt.h syscall.h
OBJS=kernel.o vga.o stdlib.o asmintr.o idt_c.o inthandle.o irq.o keyboard.o disk.o fat.o printf.o malloc.o boot.o idt_s.o syscall.o paging.o
PROG=fireos.bin

all:version.h $(PROG) modules
version.h:
	chmod +x mkver
	./mkver
$(PROG):$(OBJS) $(HDRS)
	$(CC) -T linker.ld -o $(PROG) -ffreestanding -O2 -nostdlib $(OBJS) -lgcc
	objcopy --only-keep-debug fireos.bin fireos.sym
	objcopy --strip-debug fireos.bin
clean:
	rm -f *.o $(PROG) fireos.sym version.h *~
	cd modules ; make clean
modules: build_modules install_modules
build_modules:
	cd modules ; make
install_modules:
	cp modules/*.mod fs_mount
	sync
