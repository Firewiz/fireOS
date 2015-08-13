global sys_exit
sys_exit:
	mov eax, 1
	jmp i80
global sys_write
sys_write:
	mov eax, 2
	jmp i80
global sys_getfd
sys_getfd:
	mov eax, 3
	jmp i80
global sys_setfdflag
sys_setfdflag:
	mov eax, 4
	jmp i80
global sys_clearfdflag
sys_clearfdflag:
	mov eax, 5
	jmp i80
global sys_getfdflags
sys_getfdflags:
	mov eax, 6
	jmp i80
global sys_allocbuf
sys_allocbuf:
	mov eax, 7
	jmp i80
global sys_freebuf
sys_freebuf:
	mov eax, 8
	jmp i80

i80:
	int 0x80
	ret