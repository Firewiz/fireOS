global handle_syscall
extern do_syscall
handle_syscall:
	push ebx
	push esi
	push edi
	mov ebx, [esp + 20] ; address of struct regs
	mov dword [ebx], 1  ; preserve_eax
	mov eax, [ebx + 52] ; syscall number
	mov ecx, [ebx + 48] ; original stack pointer
	mov edx, [ebx + 44] ; number of arguments
	mov ebx, edx
hsc_loop_top:
	test edx, edx
	jz hsc_loop_bottom
	mov edi, edx
	push ebx
	sub ebx, edi
	mov edi, ebx
	add edi, 1
	pop ebx
	mov esi, [ecx + edi * 4]
	push esi
	dec edx
	jmp hsc_loop_top
hsc_loop_bottom:
	push eax
	sti
	call do_syscall
	cli
	pop edx
hsc_loop2_top:
	test ebx, ebx
	jz hsc_loop2_bottom
	pop edx
	dec ebx
	jmp hsc_loop2_top
hsc_loop2_bottom:
	pop edi
	pop esi
	pop ebx
	ret