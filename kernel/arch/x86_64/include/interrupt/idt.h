#pragma once

#include <stdint.h>

/* Total supported vectors */
#define IDT_ENTRIES 256

/* IDTR register format */
typedef struct idtr
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idtr_t;

void idt_init(void);
