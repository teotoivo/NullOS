#include "core/tty.h"
#include <core/init.h>

void kmain(void)
{
    tty_write_str("Hello World!\n");
    tty_write_str("Hello From Line 2\n");

    asm volatile("int $11");

    while (1)
    {
	asm volatile("hlt");
    }
}
