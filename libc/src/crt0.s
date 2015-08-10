section .text
extern main
global _start
_start:
	call main
	mov eax, 1
	int 0x80
	int 0x81
hang:
	jmp hang
