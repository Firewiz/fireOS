export PATH := ../gcc-cross/bin:$(PATH)
CC=i686-elf-gcc
AR=i686-elf-ar
AS=nasm

CFLAGS=-nostdlib -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -g -I./include/
ASFLAGS=-felf32 -F stabs -g

OBJS=src/crt0.o src/stdio.o src/string.o src/syscall.o src/stdlib.o src/unistd.o
LIB=libc.a

all:$(LIB)
$(LIB):$(OBJS)
	$(AR) rcs lib/$(LIB) $(OBJS)
	mv src/crt0.o lib/
clean:
	rm lib/* src/*.o
