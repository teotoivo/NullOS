#pragma once

#include <stdint.h>

#define IDT_ENTRIES 256

struct idtr
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

/* Build the first 32 entries, load the IDTR */
void idt_init(void);

/* Your C-level dispatcher */
void interrupt_dispatch(void);
