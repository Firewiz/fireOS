export PATH := ../gcc-cross/bin/:$(PATH)
CC=i686-elf-gcc
AS=nasm

CFLAGS=-nostdlib -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -g -I../libc/include
ASFLAGS=-felf32 -F stabs -g
LDFLAGS=-L../libc/lib -lc

all: hello_c.exe echo.exe init.exe

hello_c.exe: hello_c.o
	$(CC) $(CFLAGS) hello_c.o -o hello_c.exe $(LDFLAGS) $(LIBS)
echo.exe: echo.o
	$(CC) $(CFLAGS) echo.o -o echo.exe $(LDFLAGS) $(LIBS)
init.exe: init.o
	$(CC) $(CFLAGS) init.o -o init.exe $(LDFLAGS) $(LIBS)
clean:
	rm *.o *.exe
