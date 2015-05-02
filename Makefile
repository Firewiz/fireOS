export PATH := gcc-cross/bin/:$(PATH)
CC=i686-elf-gcc
AS=nasm

<<<<<<< HEAD
CFLAGS=-ffreestanding -O2 -nostdlib -Wall -Wextra
ASFLAGS=-felf32

HDRS=vga.h stdlib.h asmintr.h
OBJS=kernel.o vga.o stdlib.o asmintr.o boot.o
=======
CFLAGS=-ffreestanding -nostdlib -Wall -Wextra -g
ASFLAGS=-felf32 -F stabs -g

HDRS=vga.h stdlib.h asmintr.h idt.h
OBJS=kernel.o vga.o stdlib.o asmintr.o boot.o idt_s.o idt_c.o inthandle.o irq.o keyboard.o disk.o
>>>>>>> dev
PROG=fireos.bin

all:version.h $(PROG)
version.h:
	chmod +x mkver
	./mkver
$(PROG):$(OBJS) $(HDRS)
	$(CC) -T linker.ld -o $(PROG) -ffreestanding -O2 -nostdlib $(OBJS) -lgcc
clean:
	rm -f *.o $(PROG) version.h *~
