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

void tty_write_dec(uint64_t num)
{
    /* print unsigned decimal */
    char buf[20];
    int i = 0;

    if (num == 0)
    {
	tty_write_char('0');
	return;
    }

    while (num > 0)
    {
	buf[i++] = '0' + (num % 10);
	num /= 10;
    }

    /* digits are in reverse order */
    while (i-- > 0)
	tty_write_char(buf[i]);
}

void tty_write_hex(uint64_t num)
{
    /* print uppercase hex without 0x prefix */
    char buf[16];
    int i = 0;
    uint64_t tmp = num;

    if (tmp == 0)
    {
	tty_write_char('0');
	return;
    }

    while (tmp > 0)
    {
	uint8_t d = tmp & 0xF;
	if (d < 10)
	    buf[i++] = '0' + d;
	else
	    buf[i++] = 'A' + (d - 10);
	tmp >>= 4;
    }

    while (i-- > 0)
	tty_write_char(buf[i]);
}
