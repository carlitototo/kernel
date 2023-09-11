/*
 * Copyright (c) LSE
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY LSE AS IS AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL LSE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <k/kstd.h>
#include <stdint-gcc.h>

#include "multiboot.h"

#include "io.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "serial.h"
#include "stdio.h"

void k_main(unsigned long magic, multiboot_info_t *info)
{
    (void)magic;
    (void)info;

    char star[4] = "|/-\\";
    // fb = Frame Buffer
    char *fb = (void *)0xb8000;
    setup_serial();


    GDT_Initialize();
    IDT_Initialize();
    ISR_Initialize();

    write("this is a test", 14);
    //crash_me();
    //__asm("int $0x2");
    //int i = 1/0;
    //printf("%i", i);


    for (unsigned i = 0; ; ) {
        *fb = star[i++ % 4];
    }

    for (;;)
        asm volatile ("hlt");
}
