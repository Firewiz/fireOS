cmake_minimum_required(VERSION 2.6)
project(FireOS C)
set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_ASM_NASM_FLAGS "-f elf32 -F stabs -g")
set(CMAKE_C_FLAGS "-ffreestanding -nostdlib -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -g")
set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf32")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libc/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libc/lib)
set(CMAKE_C_COMPILER gcc-cross/bin/i686-elf-gcc)
enable_language(ASM_NASM)

# Build the kernel (fireos.bin)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/version.h
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/mkver ${CMAKE_CURRENT_SOURCE_DIR}/version.h
  DEPENDS mkver ${CMAKE_CURRENT_SOURCE_DIR}/version.txt
  COMMENT "Generating version file..."
  )
add_custom_target(version DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/version.h)

add_executable(fireos.bin kernel.c vga.c stdlib.c asmintr.c idt_c.c
inthandle.c irq.c keyboard.c disk.c fat.c printf.c malloc.c boot.nasm
idt_s.nasm paging.c timer.c shell.c elf.c gdt_c.c gdt_s.nasm process.c
syscall.c syscall_handlers.c)

add_dependencies(fireos.bin version)

target_link_libraries(fireos.bin "-T linker.ld" -ffreestanding -nostdlib -O2 gcc)

# Build the C library (libc.a)

add_library(c STATIC libc/src/crt0.nasm libc/src/stdio.c libc/src/string.c libc/src/syscall.nasm libc/src/stdlib.c libc/src/unistd.c)

include_directories(libc/include)

# Build init (init.exe)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/fs_mount)

add_executable(init.exe modules/init.c)
add_dependencies(init.exe c)
add_custom_command(
  TARGET init.exe
  POST_BUILD
  COMMAND /bin/sync
  COMMENT "Synchronizing file system..."
  )
target_link_libraries(init.exe "-L../libc/lib" c)