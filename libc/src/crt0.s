section .text
extern main
extern _setup_stdio
global _start
_start:
	call _setup_stdio
	call main
	mov eax, 1
	int 0x80
	int 0x81
hang:
	jmp hang
