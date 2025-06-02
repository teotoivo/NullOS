#include "gdt.h"
/* Raw 64-bit descriptors */
static uint64_t gdt_table[GDT_ENTRIES];
static struct gdtr gdtr;

/*
 * Build a full 64-bit GDT descriptor. All segments are flat in long mode,
 * so limit=0xFFFFF and base=0. We encode flags directly into the 64-bit.
 */
static uint64_t create_descriptor(uint8_t type, uint8_t dpl, bool long_mode)
{
    uint64_t desc = 0;
    /* type(4), S=1, DPL(2), P=1 at bits 40..47 */
    desc |= (uint64_t)(type & 0xF) << 40;
    desc |= (uint64_t)(1) << 44;
    desc |= (uint64_t)(dpl & 3) << 45;
    desc |= (uint64_t)(1) << 47;
    /* long mode (L bit at 53) */
    if (long_mode)
	desc |= (uint64_t)1 << 53;
    /* granularity=1 (4KiB) at bit 55 */
    desc |= (uint64_t)1 << 55;
    return desc;
}

void gdt_init(void)
{
    /* Null descriptor */
    gdt_table[0] = 0;
    /* Kernel code */
    gdt_table[1] = create_descriptor(0xB, 0, true);
    /* Kernel data */
    gdt_table[2] = create_descriptor(0x3, 0, true);
    /* User code */
    gdt_table[3] = create_descriptor(0xB, 3, true);
    /* User data */
    gdt_table[4] = create_descriptor(0x3, 3, true);

    /* Load GDTR */
    gdtr.limit = sizeof(gdt_table) - 1;
    gdtr.base = (uint64_t)&gdt_table;
    asm volatile("lgdt %0" : : "m"(gdtr));

    /* Reload data segments with immediate selectors */
    asm volatile("mov $0x10, %%ax\n"
		 "mov %%ax, %%ds\n"
		 "mov %%ax, %%es\n"
		 "mov %%ax, %%fs\n"
		 "mov %%ax, %%gs\n"
		 "mov %%ax, %%ss\n"
		 :
		 :
		 : "ax");

    /* Far jump to reload CS with immediate code selector */
    asm volatile("pushq $0x08\n"
		 "leaq 1f(%%rip), %%rax\n"
		 "pushq %%rax\n"
		 "lretq\n"
		 "1:\n"
		 :
		 :
		 : "rax");
}
