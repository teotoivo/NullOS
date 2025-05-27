CPU_ARCH   ?= x86_64
CROSS      ?= $(CPU_ARCH)-elf-
CC         := $(CROSS)gcc
CXX        := $(CROSS)g++
LD         := $(CROSS)ld
AR         := $(CROSS)ar
NASM       ?= nasm

COMMON_CFLAGS := -std=gnu17 -ffreestanding -Wall -Wextra -Werror -MMD -MP -g
COMMON_LDFLAGS :=
NASMFLAGS ?= -f elf64 -g

export CC CXX NASM LD AR COMMON_CFLAGS NASMFLAGS COMMON_LDFLAGS CPU_ARCH

SUBDIRS := kernel

.PHONY: all clean iso run $(SUBDIRS)

all: iso


SKIP_DELEGATE := run debug gdb

$(SUBDIRS):
	@if [ "$(filter-out $(SKIP_DELEGATE),$(MAKECMDGOALS))" ]; then \
		$(MAKE) -C $@ $(MAKECMDGOALS); \
	fi

kernel_elf := kernel/build/kernel.elf
iso_image  := build/nullos.iso
$(kernel_elf):
	$(MAKE) -C kernel 
grub_mkrescue ?= grub-mkrescue

iso: $(SUBDIRS) $(iso_image)

$(iso_image): $(kernel_elf) initdisk/boot/grub/grub.cfg
	@echo "  CP   kernel → initdisk"
	@cp $(kernel_elf) initdisk/boot/kernel.elf
	@echo "  ISO  $@"
	@mkdir -p $(@D)
	$(grub_mkrescue) -o $@ initdisk

qemu           ?= qemu-system-x86_64
qemu_opts      ?= -cdrom $(iso_image) -m 512M -serial stdio -no-reboot
qemu_opts_run  ?= $(qemu_opts)
qemu_opts_dbg  ?= -cdrom $(iso_image) -m 512M -serial stdio -no-reboot -s -S

run: iso
	@echo "Launching QEMU…"
	$(qemu) $(qemu_opts_run)

debug: iso
	@echo "Launching QEMU for debugging…"
	$(qemu) $(qemu_opts_dbg)

gdb: $(kernel_elf)
	@echo "Starting GDB..."
	gdb $(kernel_elf) -x .gdbinit

clean:
	@echo "Cleaning all sub-projects…"
	-$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) clean &&) true
	@rm -rf build
	@echo "done."

%:
	@if [ "$@" != "run" ] && [ "$@" != "debug" ] && [ "$@" != "gdb" ]; then \
		$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) $@ &&) true; \
	fi

