// arch/x86_64/pic.c
#include <stdint.h>
#include "arch/x86_64/io.h"
#include "arch/x86_64/pic.h"

void pic_init(void)
{
    // start init
    outb(PIC1_CMD, 0x11);
    outb(PIC2_CMD, 0x11);

    // remap master→0x20, slave→0x28
    outb(PIC1_DATA, 0x20);
    outb(PIC2_DATA, 0x28);

    // tell master that slave is on IR2, and slave its cascade identity
    outb(PIC1_DATA, 1 << 2);
    outb(PIC2_DATA, 2);

    // 8086 mode
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // mask all except IRQ1 (keyboard)
    // bit=1→masked; ~(1<<1)=0xFD
    outb(PIC1_DATA, 0xFD);
    outb(PIC2_DATA, 0xFF);
}
