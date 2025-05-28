#pragma once

#include "stdint.h"

int init_serial();
void serial_write_char(char c);
void serial_write_string(const char* str);
void serial_write_u32(uint32_t value);
void serial_write_hex32(uint32_t value);
