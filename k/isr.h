#pragma once
#include <stdint.h>

typedef struct 
{
    // in the reverse order they are pushed (because stack and shit):
    uint32_t ds;                                            // data segment manually pushed
    uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax;    // pusha (push all)
    uint32_t interrupt, error;                              // interupt pushed manually, error is pushed by CPU
    uint32_t eip, cs, eflags, esp, ss;                      // pushed automatically by CPU
} __attribute__((packed)) Registers;

typedef void (*ISRHandler)(Registers* regs);

void ISR_Initialize();
void ISR_RegisterHandler(int interrupt, ISRHandler handler);
