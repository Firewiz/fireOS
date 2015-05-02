extern int_handler

%macro isr_noerr 1
global isr_%1
isr_%1:
	cli
	push byte 0
	push %1
	jmp isr_common_stub
%endmacro

%macro isr_err 1
global isr_%1
isr_%1:
	cli
	push %1
	jmp isr_common_stub
%endmacro

%macro set_handler 1
	push dword %1
	push dword isr_%1
	call idt_set_gate ;; stack: %esp . isr_%1 %1
	add esp, 8
%endmacro

	isr_noerr 0
	isr_noerr 1
	isr_noerr 2
	isr_noerr 3
	isr_noerr 4
	isr_noerr 5
	isr_noerr 6
	isr_noerr 7
	isr_err 8
	isr_noerr 9
	isr_err 10
	isr_err 11
	isr_err 12
	isr_err 13
	isr_err 14
	isr_noerr 15
	isr_noerr 16
	isr_noerr 17
	isr_noerr 18
%assign i 19
%rep 237
	isr_noerr i
%assign i i+1
%endrep
	
isr_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, int_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret
	
global idt
idt:
resb 256*8

global idtp
idtp:
dw (256*8) - 1
dd idt

global setup_idt
setup_idt:
	lidt [idtp]
%assign i 0
%rep 256
	set_handler i
%assign i i+1
%endrep
	ret

global idt_set_gate
idt_set_gate:
	mov eax, [esp + 4]	; handler address
	mov edx, [esp + 8]	; gate number
	imul edx, 8
	add edx, idt
	mov word [edx],ax
	add edx, 2
	mov word [edx],0x0008
	add edx, 2
	mov byte [edx], 0x00
	add edx, 1
	mov byte [edx], 0x8E
	add edx, 1
	shr eax, 16
	mov [edx],ax
	mov eax, 0		; set return value
	ret