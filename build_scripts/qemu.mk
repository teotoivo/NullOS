qemu           ?= qemu-system-x86_64
qemu_base_opts := -m 512M -serial stdio -no-reboot -no-shutdown
qemu_opts_run  := -cdrom $(ISO_IMAGE) $(qemu_base_opts)
qemu_opts_dbg  := $(qemu_opts_run) -s -S

run: iso
	@echo "Launching QEMU…"
	$(qemu) $(qemu_opts_run)

debug: iso
	@echo "Launching QEMU for debugging…"
	$(qemu) $(qemu_opts_dbg)

gdb: $(kernel_elf)
	@echo "Starting GDB…"
	gdb $(kernel_elf) -x .gdbinit
