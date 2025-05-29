.section .text

    .globl isr_stub_table
    .globl interrupt_stub
    .extern interrupt_dispatch

    .align 4
isr_stub_table:

/* --- Macros for ISR stubs --- */
/* no-error vectors: push dummy 0, then vector */
    .macro ISR_NOERR num
    .p2align 4
vector_\num:
	pushq   $0
	pushq   $\num
	jmp     interrupt_stub
    .endm

/* error vectors: CPU pushed error code, so only push vector */
    .macro ISR_ERR num
    .p2align 4
vector_\num:
	pushq   $\num
	jmp     interrupt_stub
    .endm

/* Instantiate stubs for vectors 0-31 */
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
    .rept 12        /* fill vectors 20-31 */
    .p2align 4
    pushq   $0
    pushq   $0       /* vector 0 placeholder */
    jmp     interrupt_stub
    .endr

/* --- Common interrupt stub --- */
interrupt_stub:
	/* Save general-purpose registers */
	pushq   %rax
	pushq   %rbx
	pushq   %rcx
	pushq   %rdx
	pushq   %rsi
	pushq   %rdi
	pushq   %rbp
	pushq   %r8
	pushq   %r9
	pushq   %r10
	pushq   %r11
	pushq   %r12
	pushq   %r13
	pushq   %r14
	pushq   %r15

	/* Pass stack pointer as argument */
	movq    %rsp, %rdi
	call    interrupt_dispatch

	/* Restore stack pointer from return value */
	movq    %rax, %rsp

	/* Restore general-purpose registers */
	popq    %r15
	popq    %r14
	popq    %r13
	popq    %r12
	popq    %r11
	popq    %r10
	popq    %r9
	popq    %r8
	popq    %rbp
	popq    %rdi
	popq    %rsi
	popq    %rdx
	popq    %rcx
	popq    %rbx
	popq    %rax

	/* Remove error code + vector (16 bytes) */
	addq    $16, %rsp

	iretq

