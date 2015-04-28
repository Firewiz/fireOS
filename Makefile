export PATH := gcc-cross/bin/:$(PATH)
CC=i686-elf-gcc
ASMSRCS=boot.s
ASMOBJS=boot.o
CSRCS=kernel.c vga.c stdlib.c
COBJS=kernel.o vga.o stdlib.o

all: fireos.bin

$(ASMOBJS):$(ASMSRCS)
	nasm -felf32 -o $@ $<
$(COBJS):$(CSRCS)
	$(CC) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra
fireos.bin: $(ASMOBJS) $(COBJS)
	$(CC) -T linker.ld -o fireos.bin -ffreestanding -O2 -nostdlib $(ASMOBJS) $(COBJS) -lgcc
