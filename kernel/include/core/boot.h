#pragma once

#include <limine.h>

// Accessible from anywhere that needs to read framebuffer, etc.
extern volatile struct limine_framebuffer_request framebuffer_request;
extern volatile uint64_t limine_base_revision[];
