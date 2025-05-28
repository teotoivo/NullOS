#include "core/panic.h"
#include <drivers/framebuffer.h>
#include <limine.h>
#include <stdint.h>
#include <arch/x86_64/boot.h>
#include <drivers/fonts/font-9x16.h>
#define FONT console_font_9x16

static struct limine_framebuffer* fb = NULL;

void framebuffer_init(void)
{
    if (!LIMINE_BASE_REVISION_SUPPORTED)
	kernel_panic("Unsupported Limine base revision.");

    if (!framebuffer_request.response || framebuffer_request.response->framebuffer_count < 1)
	kernel_panic("Framebuffer not available.");

    fb = framebuffer_request.response->framebuffers[0];

    if (!fb || !fb->address)
	kernel_panic("Framebuffer address is invalid.");

    if (fb->bpp != 32)
	kernel_panic("Unsupported framebuffer BPP. Expected 32.");
}

struct limine_framebuffer* framebuffer_get(void)
{
    return fb;
}

void framebuffer_put_pixel(uint32_t x, uint32_t y, uint32_t color)
{
    if (!fb || x >= fb->width || y >= fb->height)
	return;

    uint8_t* addr = (uint8_t*)fb->address;
    uint32_t* pixel = (uint32_t*)(addr + y * fb->pitch + x * (fb->bpp / 8));
    *pixel = color;
}

void framebuffer_draw_char(char c, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg)
{
    if (!fb || x + FONT_WIDTH > fb->width || y + FONT_HEIGHT > fb->height)
	return;

    uint8_t uc = (uint8_t)c;
    uint8_t* glyph = &FONT[uc * GLYPH_SIZE];

    for (uint32_t row = 0; row < FONT_HEIGHT; ++row)
    {
	uint16_t row_bits;

	if (FONT_WIDTH <= 8)
	{
	    // 1 byte per row
	    row_bits = glyph[row];
	}
	else
	{
	    // 2 bytes per row
	    row_bits = ((uint16_t)glyph[row * 2] << 8) | glyph[row * 2 + 1];
	}

	for (uint32_t col = 0; col < FONT_WIDTH; ++col)
	{
	    uint32_t mask = 1 << ((FONT_WIDTH > 8 ? 15 : 7) - col);
	    uint32_t color = (row_bits & mask) ? fg : bg;
	    framebuffer_put_pixel(x + col, y + row, color);
	}
    }
}

static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;

#define DEFAULT_FG 0xFFFFFF
#define DEFAULT_BG 0x000000

void framebuffer_putc(char c)
{
    if (c == '\n')
    {
	cursor_x = 0;
	cursor_y += FONT_HEIGHT;
	return;
    }

    framebuffer_draw_char(c, cursor_x, cursor_y, DEFAULT_FG, DEFAULT_BG);
    cursor_x += FONT_WIDTH;

    if (cursor_x + FONT_WIDTH > fb->width)
    {
	cursor_x = 0;
	cursor_y += FONT_HEIGHT;
    }

    // TODO: add scrolling logic if cursor_y exceeds fb->height
}
