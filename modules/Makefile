export PATH := ../gcc-cross/bin/:$(PATH)
CC=i686-elf-gcc
AS=nasm

CFLAGS=-nostdlib -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -g -I../libc/include
ASFLAGS=-felf32 -F stabs -g
LDFLAGS=-L../libc/lib -lc

hello_c.exe: hello_c.o
	$(CC) $(CFLAGS) hello_c.o -o hello_c.exe $(LDFLAGS) $(LIBS)
clean:
	rm *.o hello
