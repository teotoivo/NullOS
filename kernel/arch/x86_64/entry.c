#include "arch/x86_64/cpu/gdt.h"
#include "arch/x86_64/interrupt/interrupt.h"
#include <core/init.h>
#include <arch/x86_64/cpu/pic.h>

void _start(void)
{
    gdt_init();
    pic_init();
    interrupt_init();
    asm volatile("sti");

    kernel_init();
}
