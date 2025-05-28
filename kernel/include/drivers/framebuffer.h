#pragma once

#include <stdint.h>
#include <stddef.h>
#include <limine.h>

// Must be called before using any framebuffer functions
void framebuffer_init(void);

// Returns pointer to the initialized framebuffer, or NULL
struct limine_framebuffer* framebuffer_get(void);
void framebuffer_putc(char c);
