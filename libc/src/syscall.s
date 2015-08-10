global sys_exit
sys_exit:
	mov eax, 1
	jmp i80
global sys_write
sys_write:
	mov eax, 4
	jmp i80
i80:
	int 0x80
	ret