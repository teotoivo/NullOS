#include "arch/x86_64/gdt.h"
#include "arch/x86_64/interrupt.h"
#include <core/init.h>

void _start(void)
{
    gdt_init();
    idt_init();
    asm volatile("sti");

    kernel_init();
}
