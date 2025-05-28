#include "core/tty.h"
#include "serial_debug.h"
#include "drivers/framebuffer.h"

static uint32_t active_backends = 0;

void tty_init(void)
{
    // Detect or assume defaults
    active_backends = TTY_BACKEND_SERIAL;
}

void tty_set_backends(uint32_t backends)
{
    active_backends = backends;
}

void tty_enable_backend(tty_backend_t backend)
{
    active_backends |= backend;
}

void tty_disable_backend(tty_backend_t backend)
{
    active_backends &= ~backend;
}

void tty_write_char(char c)
{
    if (active_backends & TTY_BACKEND_SERIAL)
	serial_write_char(c);

    if (active_backends & TTY_BACKEND_FRAMEBUFFER)
	framebuffer_putc(c);
}

void tty_write(const char* str, size_t len)
{
    for (size_t i = 0; i < len; i++)
	tty_write_char(str[i]);
}

void tty_write_str(const char* str)
{
    while (*str)
	tty_write_char(*str++);
}
