section .text
global main
main:
	push dword 30   ; length
	push string	; buf
	push dword 1	; fd
	mov eax, 4
	int 0x80
	int 0x81
	mov eax, 1
	int 0x80
	leave
	ret
	
section .rodata
string: db "Hello World! (from userspace)"
	db 10
	