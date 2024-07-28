//ensure contents of header are included only once
#pragma once

#include "common.h"

//8 bit data bus, 16bit address bus. You pass in a 16bit adress u get an 8bit data bus back
u8 bus_read(u16 address);
void bus_write(u16 address, u8 value);

u16 bus_read16(u16 address);
void bus_write16(u16 address, u16 value);