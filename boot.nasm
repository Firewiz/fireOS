; boot.s
; Main boot loader.

; Multiboot header
MBALIGN		equ 1<<0
MEMINFO		equ 1<<1
FLAGS		equ MBALIGN | MEMINFO
MAGIC		equ 0x1BADB002
CHECKSUM	equ -(MAGIC + FLAGS)

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bootstrap_stack, nobits
align 4
stack_bottom:			; allocate 16 KB of stack
resb 16384
stack_top:

section .text
global _start
; Main entry point to the OS.
_start:
	mov esp, stack_top	; initialize stack pointer to point
                                ; somewhere useful
extern kernel_main
	call kernel_main	; main C entry point
.hang:
	hlt
	jmp .hang
	