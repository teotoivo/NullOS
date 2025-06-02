.PHONY: all iso clean

all: iso

include build_scripts/config.mk
include build_scripts/limine.mk
include build_scripts/qemu.mk


$(KERNEL_ELF):
	$(MAKE) -C kernel BUILD_DIR=$(abspath $(BUILD_DIR))

iso: $(ISO_IMAGE)

$(ISO_IMAGE): $(KERNEL_ELF) limine.conf $(limine_bin)
	@echo "  CP   kernel"
	@mkdir -p $(BUILD_DIR)/boot/limine $(BUILD_DIR)/EFI/BOOT $(dir $@)
	@cp $(KERNEL_ELF) $(BUILD_DIR)/boot/kernel.elf
	@cp limine.conf $(BUILD_DIR)/
	@cp $(limine_dir)/limine-bios.sys \
	     $(limine_dir)/limine-bios-cd.bin \
	     $(limine_dir)/limine-uefi-cd.bin \
	     $(BUILD_DIR)/boot/limine/
	@cp $(limine_dir)/BOOTX64.EFI $(limine_dir)/BOOTIA32.EFI $(BUILD_DIR)/EFI/BOOT/
	@echo "  ISO  $@"
	@xorriso -as mkisofs -R -J \
	         -b boot/limine/limine-bios-cd.bin \
	         -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
	         -apm-block-size 2048 \
	         --efi-boot boot/limine/limine-uefi-cd.bin \
	         -efi-boot-part --efi-boot-image --protective-msdos-label \
	         $(BUILD_DIR) -o $@
	@$(limine_bin) bios-install $@


clean:
	rm -rf $(BUILD_DIR)
