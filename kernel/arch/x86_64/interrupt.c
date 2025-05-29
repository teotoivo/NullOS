#include "core/panic.h"
#include "core/tty.h"
#include <stdint.h>

// Define the CPU status struct matching our stub layout
typedef struct cpu_status_t
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t vector_number;
    uint64_t error_code;
    uint64_t iret_rip;
    uint64_t iret_cs;
    uint64_t iret_flags;
    uint64_t iret_rsp;
    uint64_t iret_ss;
} cpu_status_t;

// Forward declaration from idt.c
extern void idt_init(void);

// Initialize IDT (and GDT elsewhere)
void interrupt_init(void)
{
    idt_init();
}

static void handle_page_fault(cpu_status_t* ctx)
{
    (void)ctx;
    uint64_t fault_addr;

    /* CR2 holds the faulting linear address */
    asm volatile("mov %%cr2, %0" : "=r"(fault_addr));

    tty_write_str("Page fault at 0x");
    tty_write_str(", code 0x");
    tty_write_str("\n");

    kernel_panic("Fatal page fault");
}

static void handle_breakpoint(void)
{
    tty_write_str("#BP handler worked!\n");
}

static void handle_unexpected(uint64_t vector)
{
    (void)vector;
    tty_write_str("Unhandled interrupt #");
    tty_write_str("\n");

    while (1)
	asm volatile("hlt");
}

void interrupt_dispatch(cpu_status_t* ctx)
{
    switch (ctx->vector_number)
    {
    case 3:
	handle_breakpoint();
	break;

    case 14:
	handle_page_fault(ctx);
	break;

    default:
	handle_unexpected(ctx->vector_number);
	break;
    }
}
