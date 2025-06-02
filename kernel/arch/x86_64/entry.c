#include "gdt.h"
#include "interrupt.h"
#include <core/init.h>
#include <pic.h>

void _start(void)
{
    gdt_init();
    pic_init();
    interrupt_init();
    asm volatile("sti");

    kernel_init();
}
