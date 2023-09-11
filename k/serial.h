#ifndef SERIAL_H
#define SERIAL_H

#include "io.h"
#include <stddef.h>

int nothing_transmitting(void);
void setup_serial();
void write(const char message[], size_t size);

#endif /* SERIAL_H */
