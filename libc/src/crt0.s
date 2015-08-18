section .text
extern main
extern _setup_stdio
extern sys_exit
global _start
_start:
	call _setup_stdio
	call main
	push dword 0
	call sys_exit
;	int 0x81
hang:
	jmp hang
