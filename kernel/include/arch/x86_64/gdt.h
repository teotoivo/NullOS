#ifndef ARCH_X86_64_GDT_H
#define ARCH_X86_64_GDT_H

#include <stdint.h>
#include <stdbool.h>

/* GDT layout: null, kcode, kdata, ucode, udata */
#define GDT_ENTRIES 5
#define GDT_NULL_SEL 0x00
#define GDT_KCODE_SEL 0x08
#define GDT_KDATA_SEL 0x10
#define GDT_UCODE_SEL 0x18
#define GDT_UDATA_SEL 0x20

struct gdtr
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

/* Build and load the GDT, then reload CS/DS/ES/SS/FS/GS */
void gdt_init(void);

#endif /* ARCH_X86_64_GDT_H */
