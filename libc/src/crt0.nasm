section .text
extern main
extern _setup_stdio
extern sys_exit
global _start
_start:
	call _setup_stdio
	call main
	call sys_exit
	push 0
	mov eax, 1
	int 0x80
	pop eax
hang:
	jmp hang
