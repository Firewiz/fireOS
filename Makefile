export PATH := gcc-cross/bin/:$(PATH)
CC=i686-elf-gcc

all: fireos.bin

boot.o: boot.s
	nasm -felf32 boot.s -o boot.o
kernel.o: kernel.c
	$(CC) -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
fireos.bin: boot.o kernel.o
	$(CC) -T linker.ld -o fireos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
