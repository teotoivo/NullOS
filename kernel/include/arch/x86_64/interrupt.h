#pragma once
#include <stdint.h>

typedef struct cpu_ctx
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rsi, rdi, rbp, rdx, rcx, rbx, rax;
    uint64_t vector;
    uint64_t err_code;
    uint64_t rip, cs, rflags, rsp, ss;
} cpu_ctx_t;

void idt_init(void);
cpu_ctx_t* interrupt_dispatch(cpu_ctx_t* ctx);
