global install_gdt
gdtr:
dw 0
dd 0

install_gdt:
	mov eax, [esp + 4]
	mov [gdtr + 2], eax
	mov ax, [esp + 8]
	mov [gdtr], ax
	lgdt [gdtr]
	ret

global tss_flush
tss_flush:
	mov ax, 0x2B
	ltr ax
	ret

global enter_usermode
enter_usermode:
	mov ecx, [ebp + 8]
	mov ax, 0x23
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push 0x23
	push eax
	pushf
	push 0x1b
	push ecx
	iret