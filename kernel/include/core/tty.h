#pragma once

#include <stdint.h>
#include <stddef.h>

typedef enum
{
    TTY_BACKEND_SERIAL = (1 << 0),
    TTY_BACKEND_FRAMEBUFFER = (1 << 1)
    // Add more backends as needed
} tty_backend_t;

void tty_init(void);

/*
 * Enable one or more backends. Use bitwise OR.
 * e.g., tty_set_backends(TTY_BACKEND_SERIAL | TTY_BACKEND_FRAMEBUFFER);
 */
void tty_set_backends(uint32_t backends);

void tty_enable_backend(tty_backend_t backend);
void tty_disable_backend(tty_backend_t backend);

/*
 * Core TTY output functions.
 */
void tty_write_char(char c);
void tty_write(const char* str, size_t len);
void tty_write_str(const char* str);
