#include <stdbool.h>
#include "io.h"

// Handy register number defines
#define DATA 0
#define ERROR_R 1
#define SECTOR_COUNT 2
#define LBA_LOW 3
#define LBA_MID 4
#define LBA_HIGH 5
#define DRIVE_SELECT 6
#define COMMAND_REGISTER 7

// Control register defines
#define CONTROL 0x206

#define ALTERNATE_STATUS 0

static __inline void insw(uint16_t __port, void *__buf, unsigned long __n) {
    __asm__ __volatile__("cld; rep; insw"
            : "+D"(__buf), "+c"(__n)
            : "d"(__port));
}

static __inline__ void outsw(uint16_t __port, const void *__buf, unsigned long __n) {
    __asm__ __volatile__("cld; rep; outsw"
            : "+S"(__buf), "+c"(__n)
            : "d"(__port));
}

// This code is to wait 400 ns thanks to dark magic
static void ata_io_wait(const uint8_t p) {
    inb(p + CONTROL + ALTERNATE_STATUS);
    inb(p + CONTROL + ALTERNATE_STATUS);
    inb(p + CONTROL + ALTERNATE_STATUS);
    inb(p + CONTROL + ALTERNATE_STATUS);
}

// Reads sectors starting from lba to buffer
int read_cdrom(uint16_t port, bool slave, uint32_t lba, uint32_t sectors, uint16_t *buffer) {
    // The command
    volatile uint8_t read_cmd[12] = {0xA8, 0,
                                     (lba >> 0x18) & 0xFF, (lba >> 0x10) & 0xFF, (lba >> 0x08) & 0xFF,
                                     (lba >> 0x00) & 0xFF,
                                     (sectors >> 0x18) & 0xFF, (sectors >> 0x10) & 0xFF, (sectors >> 0x08) & 0xFF,
                                     (sectors >> 0x00) & 0xFF,
                                     0, 0};

    outb(port + DRIVE_SELECT, 0xA0 & (slave << 4)); // Drive select
    ata_io_wait(port);
    outb(port + ERROR_R, 0x00);
    outb(port + LBA_MID, 2048 & 0xFF);
    outb(port + LBA_HIGH, 2048 >> 8);
    outb(port + COMMAND_REGISTER, 0xA0); // Packet command
    ata_io_wait(port); // We need to wait for 400ns, why ? Don't ask why, just wait

    // Wait for status
    while (1) {
        uint8_t status = inb(port + COMMAND_REGISTER);
        if ((status & 0x01) == 1)
            return 1;
        if (!(status & 0x80) && (status & 0x08))
            break;
        ata_io_wait(port);
    }

    // Send command
    outsw(port + DATA, (uint16_t *) read_cmd, 6);

    // Read words
    for (uint32_t i = 0; i < sectors; i++) {
        // Wait until ready
        while (1) {
            uint8_t status = inb(port + COMMAND_REGISTER);
            if (status & 0x01)
                return 1;
            if (!(status & 0x80) && (status & 0x08))
                break;
        }

        int size = inb(port + LBA_HIGH) << 8
                   | inb(port + LBA_MID); // Get the size of transfer

        insw(port + DATA, (uint16_t *) ((uint8_t *) buffer + i * 0x800), size / 2); // Read it
    }

    return 0;
}