#include <arch/x86_64/gdt.h>
#include <arch/x86_64/idt.h>

struct interrupt_descriptor
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t flags;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

static struct interrupt_descriptor idt[IDT_ENTRIES];
static struct idtr idtr;

/* Provided by isr.S: address of first stub (vector 0) */
extern char isr_stub_table;

static void set_idt_entry(int vector, void (*handler)(void), uint8_t dpl)
{
    uint64_t addr = (uint64_t)handler;
    idt[vector].offset_low = addr & 0xFFFF;
    idt[vector].selector = GDT_KCODE_SEL;
    idt[vector].ist = 0;
    idt[vector].flags = (0xE /* interrupt gate */) | ((dpl & 3) << 5) | 0x80 /* present */;
    idt[vector].offset_mid = (addr >> 16) & 0xFFFF;
    idt[vector].offset_high = addr >> 32;
    idt[vector].reserved = 0;
}

void idt_init(void)
{
    /* Only populate vectors 0–31 for now */
    for (int i = 0; i < 32; i++)
    {
	/* each stub is 16-bytes apart */
	void* stub = (void*)((uintptr_t)&isr_stub_table + (i * 16));
	set_idt_entry(i, stub, 0);
    }

    /* Load IDTR */
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint64_t)&idt;
    asm volatile("lidt %0" : : "m"(idtr));
}
