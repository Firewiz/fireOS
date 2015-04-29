export PATH := gcc-cross/bin/:$(PATH)
CC=i686-elf-gcc
AS=nasm

CFLAGS=-ffreestanding -O2 -nostdlib -Wall -Wextra
ASFLAGS=-felf32

HDRS=vga.h stdlib.h asmintr.h
OBJS=kernel.o vga.o stdlib.o asmintr.o boot.o
PROG=fireos.bin

all:$(PROG)
$(PROG):$(OBJS) $(HDRS)
	$(CC) -T linker.ld -o $(PROG) -ffreestanding -O2 -nostdlib $(OBJS) -lgcc
clean:
	rm *.o $(PROG)
