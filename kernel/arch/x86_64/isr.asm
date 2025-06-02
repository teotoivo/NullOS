section .text
global isr_stub_table
global interrupt_stub
extern interrupt_dispatch

align 4
isr_stub_table:
	%assign v 0
	%rep 256
		align 16
		mov rax, v			; Load vector into rax
		jmp interrupt_stub
		%assign v v+1
	%endrep

interrupt_stub:
	; Save general-purpose registers
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	; Push context metadata
	push rax				; vector number
	push qword 0			; dummy error code

	sub rsp, 8				; red zone

	lea rdi, [rsp + 16]		; rdi -> cpu_ctx_t (skip vector and error code)
	call interrupt_dispatch

	add rsp, 8				; restore red zone

	; Restore registers
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax

	add rsp, 16				; pop dummy error code and vector

	iretq

