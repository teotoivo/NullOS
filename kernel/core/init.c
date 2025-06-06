#include <limine.h>
#include <drivers/framebuffer.h>
#include <core/panic.h>
#include <stdbool.h>
#include <core/tty.h>
#include <serial_debug.h>
#include <main.h>

void kernel_init(void)
{
    framebuffer_init();
    init_serial();

    tty_init();
    tty_enable_backend(TTY_BACKEND_SERIAL | TTY_BACKEND_FRAMEBUFFER);

    kmain();
}
