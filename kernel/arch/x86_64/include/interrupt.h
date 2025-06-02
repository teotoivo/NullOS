#pragma once

#include <stdint.h>
#include <idt.h>

/*
 * CPU state saved by interrupt_stub and the processor.
 * Layout must match exactly.
 */
typedef struct cpu_ctx
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;

    uint64_t vector;   // manually pushed in stub
    uint64_t err_code; // dummy or real

    uint64_t rip; // pushed by CPU
    uint64_t cs;
    uint64_t rflags;
    // uint64_t rsp;			// only for user→kernel transition
    // uint64_t ss;
} cpu_ctx_t;

void interrupt_init(void);
cpu_ctx_t* interrupt_dispatch(cpu_ctx_t* ctx);

/* Register a handler for a given vector */
void set_irq_handler(int vector, void (*handler)(cpu_ctx_t* ctx));
