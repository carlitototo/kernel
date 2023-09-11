#include "io.h"
#include "serial.h"

#define IER 1
#define LSR 5
#define DLAB 3
#define COM1 0x3F8
#define LATCH_LOW 0
#define LATCH_HIGH 1

int nothing_transmitting(void) {
    return inb(COM1 + LSR) & 0x20;
}

void setup_serial(){
    // We want to disable interupt in order to not fuck up during kernel boot
    // IER is the UART register to disable interupt, 0 disables it
    outb(COM1 + IER, 0x0);
    // We initialize the DLAB to 1 in order to modify latch values
    outb(COM1 + DLAB, 0x80);
    // We set the latch values as in order to have a baud of 38400
    // See Divisor Latch Byte Values (common baud rates) table for conversion
    outb(COM1 + LATCH_LOW, 0x3);
    outb(COM1 + LATCH_HIGH, 0x0);
    // Reset DLAB
    outb(COM1 + DLAB, 0x3);
    // We enable interupt again
}

void write(const char message[], size_t size){
    while (!nothing_transmitting()) {
        continue;
    }
    for (size_t i = 0; i < size; i++) {
        outb(COM1, message[i]);
        while (!nothing_transmitting()) {
            continue;
        }
    }
    return;
}
