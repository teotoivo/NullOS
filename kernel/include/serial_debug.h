#pragma once

#include "stdint.h"

void serial_init();
void serial_write_char(char c);
void serial_write_string(const char *str);
void serial_write_u32(uint32_t value);
void serial_write_hex32(uint32_t value);
