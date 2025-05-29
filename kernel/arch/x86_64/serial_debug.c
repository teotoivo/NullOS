#define PORT 0x3f8 // COM1

#include "serial_debug.h"
#include <arch/x86_64/io.h>

int init_serial()
{
    outb(PORT + 1, 0x00); // Disable all interrupts
    outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    outb(PORT + 1, 0x00); //                  (hi byte)
    outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
    outb(PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
    outb(PORT + 0, 0xAE); // Send a test byte

    // Check that we received the same test byte we sent
    if (inb(PORT + 0) != 0xAE)
    {
	return 1;
    }

    // If serial is not faulty set it in normal operation mode:
    // not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled
    outb(PORT + 4, 0x0F);
    return 0;
}

static int transmit_empty()
{
    return inb(PORT + 5) & 0x20;
}

void serial_write_char(char c)
{
    while (!transmit_empty())
	;
    outb(PORT, (uint8_t)c);
}

void serial_write_string(const char* str)
{
    for (; *str; ++str)
	serial_write_char(*str);
}

void serial_write_u32(uint32_t value)
{
    char buf[10];
    int i = 0;

    if (value == 0)
    {
	serial_write_char('0');
	return;
    }

    while (value)
    {
	buf[i++] = '0' + (value % 10);
	value /= 10;
    }
    while (i--)
	serial_write_char(buf[i]);
}

void serial_write_hex32(uint32_t value)
{
    static const char hex[] = "0123456789ABCDEF";

    serial_write_string("0x");
    for (int shift = 28; shift >= 0; shift -= 4)
	serial_write_char(hex[(value >> shift) & 0xF]);
}
