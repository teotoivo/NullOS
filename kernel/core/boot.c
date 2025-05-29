#include <limine.h>

__attribute__((used, section(".limine_requests"))) volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests"))) volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};
