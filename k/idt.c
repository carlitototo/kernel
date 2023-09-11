#include "idt.h"
#include <stdint.h>

#define FLAG_SET(x, flag) x |= (flag)
#define FLAG_UNSET(x, flag) x &= ~(flag)

typedef struct
{
    uint16_t BaseLow;
    uint16_t SegmentSelector;
    uint8_t Reserved;
    uint8_t Flags;
    uint16_t BaseHigh;
} __attribute__((packed)) idt;

typedef struct
{
    uint16_t limit;
    idt* base;
} __attribute__((packed)) idt_r;


idt IDT[256];

void IDT_SetGate(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags)
{
    IDT[interrupt].BaseLow = ((uint32_t)base) & 0xFFFF;
    IDT[interrupt].SegmentSelector = segmentDescriptor;
    IDT[interrupt].Reserved = 0;
    IDT[interrupt].Flags = flags;
    IDT[interrupt].BaseHigh = ((uint32_t)base >> 16) & 0xFFFF;
}

void IDT_EnableGate(int interrupt)
{
    FLAG_SET(IDT[interrupt].Flags, IDT_FLAG_PRESENT);
}

void IDT_DisableGate(int interrupt)
{
    FLAG_UNSET(IDT[interrupt].Flags, IDT_FLAG_PRESENT);
}

void IDT_Initialize()
{
    idt_r idtr;
    idtr.base = IDT; /* idt base address */
    idtr.limit = sizeof(IDT) - 1; /* idt size - 1 */
    __asm__ volatile("lidt %0\n"
            : /* no output */
            : "m" (idtr)
            : "memory");
}