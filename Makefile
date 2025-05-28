# ───────────────────────── toolchain ─────────────────────────
CPU_ARCH  ?= x86_64
CROSS     ?= $(CPU_ARCH)-elf-
export CC  := $(CROSS)gcc
export CXX := $(CROSS)g++
export LD  := $(CROSS)ld
export AR  := $(CROSS)ar
NASM      ?= nasm
export NASM

COMMON_CFLAGS  := -std=gnu17 -ffreestanding -Wall -Wextra -Werror -MMD -MP -g
COMMON_LDFLAGS :=
NASMFLAGS      ?= -f elf64 -g
export COMMON_CFLAGS COMMON_LDFLAGS NASMFLAGS CPU_ARCH

# ───────────────────────── project paths ─────────────────────
SUBDIRS      := kernel
kernel_elf   := kernel/build/kernel.elf
iso_image    := build/nullos.iso
initdisk_dir := initdisk

# ───────────────────────── Limine (3rd-party) ────────────────
limine_branch ?= v9.x-binary
limine_dir    := limine
limine_bin    := $(limine_dir)/limine

# ───────────────────────── run-time helpers ──────────────────
qemu           ?= qemu-system-x86_64
qemu_base_opts := -m 512M -serial stdio -no-reboot -no-shutdown
qemu_opts_run  := -cdrom $(iso_image) $(qemu_base_opts)
qemu_opts_dbg  := $(qemu_opts_run) -s -S

skip_delegate := run debug gdb

# ───────────────────────── meta targets ──────────────────────
.PHONY: all iso run debug gdb clean $(SUBDIRS)
.DEFAULT_GOAL := iso

all: iso

# Delegate build steps to sub-projects unless explicitly skipped
$(SUBDIRS):
	$(if $(filter-out $(skip_delegate),$(MAKECMDGOALS)),$(MAKE) -C $@ $(filter-out $@,$(MAKECMDGOALS)))

# ───────────────────────── artefacts ─────────────────────────
$(kernel_elf): | $(SUBDIRS)
	$(MAKE) -C kernel			# keeps per-kernel dependency logic local

# ───────────────────────── Limine rules ──────────────────────
$(limine_dir):
	git clone https://github.com/limine-bootloader/limine.git --branch=$(limine_branch) --depth=1 $@

$(limine_bin): | $(limine_dir)
	$(MAKE) -C $(limine_dir)

# ───────────────────────── ISO (Limine) ──────────────────────
iso: $(iso_image)

$(iso_image): $(kernel_elf) limine.conf $(limine_bin)
	@echo "  CP   kernel → initdisk"
	@mkdir -p $(initdisk_dir)/boot/limine $(initdisk_dir)/EFI/BOOT $(@D)
	@cp $(kernel_elf) $(initdisk_dir)/boot/kernel.elf
	@cp limine.conf $(initdisk_dir)/
	@cp $(limine_dir)/limine-bios.sys \
	     $(limine_dir)/limine-bios-cd.bin \
	     $(limine_dir)/limine-uefi-cd.bin \
	     $(initdisk_dir)/boot/limine/
	@cp $(limine_dir)/BOOTX64.EFI $(limine_dir)/BOOTIA32.EFI $(initdisk_dir)/EFI/BOOT/
	@echo "  ISO  $@"
	@xorriso -as mkisofs -R -J \
	         -b boot/limine/limine-bios-cd.bin \
	         -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
	         -apm-block-size 2048 \
	         --efi-boot boot/limine/limine-uefi-cd.bin \
	         -efi-boot-part --efi-boot-image --protective-msdos-label \
	         $(initdisk_dir) -o $@
	@$(limine_bin) bios-install $@

# ───────────────────────── QEMU targets ──────────────────────
run: iso
	@echo "Launching QEMU…"
	$(qemu) $(qemu_opts_run)

debug: iso
	@echo "Launching QEMU for debugging…"
	$(qemu) $(qemu_opts_dbg)

gdb: $(kernel_elf)
	@echo "Starting GDB…"
	gdb $(kernel_elf) -x .gdbinit

# ───────────────────────── cleanup ───────────────────────────
clean:
	@echo "Cleaning sub-projects…"
	-$(foreach d,$(SUBDIRS),$(MAKE) -C $(d) clean &&) true
	@rm -rf build $(limine_dir) $(initdisk_dir) *.dSYM
	@echo "done."

