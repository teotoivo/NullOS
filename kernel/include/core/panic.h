#pragma once

#include <stdint.h>

__attribute__((noreturn)) void kernel_panic(const char* msg);
