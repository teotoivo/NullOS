.section .text
.globl isr_stub_table
.globl interrupt_stub
.extern interrupt_dispatch

.align 4
isr_stub_table:
	.set v, 0
	.rept 256
		.p2align 4
		movabsq $v, %rax		/* Load vector into %rax */
		jmp interrupt_stub
		.set v, v+1
	.endr
interrupt_stub:
	/* Save GPRs */
	pushq	%rax
	pushq	%rbx
	pushq	%rcx
	pushq	%rdx
	pushq	%rsi
	pushq	%rdi
	pushq	%rbp
	pushq	%r8
	pushq	%r9
	pushq	%r10
	pushq	%r11
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

	/* Push context metadata */
	pushq	%rax		/* vector */
	pushq	$0		/* dummy error code */

	subq	$8, %rsp	/* red zone */

	lea	16(%rsp), %rdi	/* point to start of cpu_ctx_t (skip vector + err_code) */
	call	interrupt_dispatch

	addq	$8, %rsp	/* red zone */

	/* Restore GPRs */
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%r11
	popq	%r10
	popq	%r9
	popq	%r8
	popq	%rbp
	popq	%rdi
	popq	%rsi
	popq	%rdx
	popq	%rcx
	popq	%rbx
	popq	%rax

	addq $16, %rsp

	iretq

