#include "core/tty.h"
#include <core/init.h>

void kmain(void)
{
    kernel_init();

    tty_write_str("Hello World!\n");
    tty_write_str("Hello From Line 2\n");
}
