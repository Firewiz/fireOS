export PATH := gcc-cross/bin/:$(PATH)
CC=i686-elf-gcc
AS=nasm

CFLAGS=-ffreestanding -O2 -nostdlib -Wall -Wextra
ASFLAGS=-felf32

HDRS=vga.h stdlib.h asmintr.h
OBJS=kernel.o vga.o stdlib.o asmintr.o boot.o

all: fireos.bin
fireos.bin: $(OBJS) $(HDRS)
	$(CC) -T linker.ld -o fireos.bin -ffreestanding -O2 -nostdlib $(OBJS) -lgcc
