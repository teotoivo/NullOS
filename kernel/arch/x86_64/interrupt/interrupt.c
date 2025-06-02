#include "interrupt/interrupt.h"
#include "cpu/io.h"
#include "cpu/pic.h"
#include "core/panic.h"
#include "core/tty.h"
#include <stdint.h>

#define UNUSED(x) ((void)(x))

/* Forward-declare arch IDT init */
extern void idt_init(void);

/* Table of C-level handlers */
static void (*irq_handlers[IDT_ENTRIES])(cpu_ctx_t* ctx);

/* Default for non-exception, non-registered vectors */
static void default_handler(uint64_t vec)
{
    tty_write_str("Unhandled interrupt #");
    tty_write_dec(vec);
    tty_write_str("\n");
}

static void exception_handler(cpu_ctx_t* ctx)
{
    tty_write_str("Exception #");
    tty_write_dec(ctx->vector);
    tty_write_str(", error code 0x");
    tty_write_hex(ctx->err_code);
    tty_write_str(", RIP 0x");
    tty_write_hex(ctx->rip);
    tty_write_str("\n");
    tty_write_str("CS = 0x");
    tty_write_hex(ctx->cs);
    tty_write_str("\n");
    tty_write_str("Stack dump:\n");

    uint64_t* sp = (uint64_t*)ctx;
    for (int i = 0; i < 16; ++i)
    {
	tty_write_hex(sp[i]);
	tty_write_str(" ");
	if (i % 4 == 3)
	    tty_write_str("\n");
    }
    kernel_panic("Unhandled CPU exception");
}

void set_irq_handler(int vector, void (*handler)(cpu_ctx_t* ctx))
{
    if (vector >= 0 && vector < IDT_ENTRIES && handler)
	irq_handlers[vector] = handler;
}

cpu_ctx_t* interrupt_dispatch(cpu_ctx_t* ctx)
{
    void (*handler)(cpu_ctx_t*) = irq_handlers[ctx->vector];

    if (handler)
	handler(ctx);
    else
	default_handler(ctx->vector);

    /* Return new stack pointer (in RAX) back to the stub */
    return ctx;
}

void handle_breakpoint(cpu_ctx_t* ctx)
{
    UNUSED(ctx);
    tty_write_str("#BP handler worked!\n");
}

void handle_page_fault(cpu_ctx_t* ctx)
{
    uint64_t fault_addr;

    asm volatile("mov %%cr2, %0" : "=r"(fault_addr));
    tty_write_str("Page fault at 0x");
    tty_write_hex(fault_addr);
    tty_write_str(", code 0x");
    tty_write_hex(ctx->err_code);
    tty_write_str("\n");
    kernel_panic("Fatal page fault");
}

void handle_key_press(cpu_ctx_t* ctx)
{
    (void)ctx;

    uint8_t scancode = inb(0x60);

    tty_write_str("key pressed (sc=");
    tty_write_hex(scancode);
    tty_write_str(")\n");

    outb(PIC1_CMD, 0x20);
}

void interrupt_init(void)
{
    for (int i = 0; i < 31; ++i)
	irq_handlers[i] = exception_handler;

    idt_init();

    set_irq_handler(3, handle_breakpoint);
    set_irq_handler(14, handle_page_fault);

    set_irq_handler(33, handle_key_press);
}
