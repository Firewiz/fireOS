	.file	"idt_c.c"
	.comm	int_handlers,1024,64
	.text
	.globl	install_handler
	.type	install_handler, @function
install_handler:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$4, %esp
	movl	12(%ebp), %eax
	movb	%al, -4(%ebp)
	movzbl	-4(%ebp), %eax
	movl	8(%ebp), %edx
	movl	%edx, int_handlers(,%eax,4)
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	install_handler, .-install_handler
	.section	.rodata
.LC0:
	.string	"Unknown interrupt\n"
	.text
	.globl	int_handler
	.type	int_handler, @function
int_handler:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	48(%eax), %eax
	movl	int_handlers(,%eax,4), %eax
	testl	%eax, %eax
	je	.L3
	movl	8(%ebp), %eax
	movl	48(%eax), %eax
	movl	int_handlers(,%eax,4), %eax
	movl	8(%ebp), %edx
	movl	48(%edx), %edx
	subl	$8, %esp
	pushl	8(%ebp)
	pushl	%edx
	call	*%eax
	addl	$16, %esp
	jmp	.L2
.L3:
	subl	$12, %esp
	pushl	$.LC0
	call	vga_write
	addl	$16, %esp
.L2:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	int_handler, .-int_handler
	.ident	"GCC: (GNU) 4.9.1"
