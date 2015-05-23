BITS 32
section .text
_start:
	mov ecx, string
	mov eax, 0
	int 0x80
	ret
section .rodata
string: db "Hello World!"
	db 10
	