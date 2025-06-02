#include "cpu/gdt.h"
#include "interrupt/interrupt.h"
#include <core/init.h>
#include <cpu/pic.h>

void _start(void)
{
    gdt_init();
    pic_init();
    interrupt_init();
    asm volatile("sti");

    kernel_init();
}
