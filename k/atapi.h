#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "io.h"

int read_cdrom(uint16_t port, bool slave, uint32_t lba, uint32_t sectors, uint16_t *buffer);