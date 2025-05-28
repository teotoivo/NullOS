#include "core/tty.h"
#include <core/panic.h>

void kernel_panic(const char* msg)
{
    tty_write_str(msg);
    for (;;)
	__asm__("hlt");
}
