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
	push dword 0 ; load_stack bit
	push dword 0 ; preserve_eax bit
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, int_handler
	call eax
	mov ecx, eax
	pop eax

	pop edx ; preserve_eax
	
	pop eax ; load_stack
	test eax, eax
	jne iret_load_stack
cleanup:
	
	pop gs
	pop fs
	pop es
	pop ds
; at this point, we want to move ECX to EAX if EDX != 0... *after* the
; POPA.
; to do this, we'll fake the stack frame.
	test edx, edx
	je pop_frame
; eax is at esp+28
	mov [esp + 28], ecx
pop_frame:
	popa
	
;	pop edi
;	pop esi
;	pop ebp
;       add esp, 4 
;	pop ebx
;	pop edx
;	pop ecx
;	pop eax
	
	add esp, 8
	sti
	or WORD [esp + 8], 0x200
	iret

iret_load_stack:
;; Current stack: 0 gs 4 fs 8 es 12 ds 16 edi 20 esi 24 ebp 28 esp 32
;; ebx 36 edx 40 ecx 44 eax 48 in 52 ec 56 eip 60 cs 64 eflags 68
;; useresp 72 ss

;; We need to move that stack to useresp.
	mov eax, [esp+68]
;; Now that useresp is in EAX, we can move the rest of the registers
;; over.
	pop ebx
	mov [eax-72], ebx ; gs
	pop ebx
	mov [eax-68], ebx ; fs
	pop ebx
	mov [eax-64], ebx ; es
	pop ebx
	mov [eax-60], ebx ; ds
	pop ebx
	mov [eax-56], ebx ; edi
	pop ebx
	mov [eax-52], ebx ; esi
	pop ebx
	mov [eax-48], ebx ; ebp
	pop ebx
	mov [eax-44], ebx ; esp
	pop ebx
	mov [eax-40], ebx ; ebx
	pop ebx
	mov [eax-36], ebx ; edx
	pop ebx
	mov [eax-32], ebx ; ecx
	pop ebx
	mov [eax-28], ebx ; eax
	pop ebx
	mov [eax-24], ebx ; inum
	pop ebx
	mov [eax-20], ebx ; errcode
	pop ebx
	mov [eax-16], ebx ; eip
	pop ebx
	mov [eax-12], ebx ; cs
	pop ebx
	mov [eax-8], ebx ; eflags
	pop ebx
	mov [eax-4], ebx ; useresp
	pop ebx
	mov [eax], ebx ; ss

; load new stack
	sub eax, 72
	mov esp, eax
	jmp cleanup
	
	
	
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
	mov word [edx],ax	; low offset
	add edx, 2
	mov word [edx],0x0008	; selector
	add edx, 2
	mov byte [edx], 0x00	; unused
	add edx, 1
	mov byte [edx], 0xEE	; type_attr
	add edx, 1
	shr eax, 16
	mov [edx],ax
	mov eax, 0		; set return value
	ret