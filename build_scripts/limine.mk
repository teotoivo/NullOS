limine_branch       = v9.x-binary
limine_dir          = $(build_dir)/limine
limine_bin          = $(limine_dir)/limine


$(limine_dir):
	git clone https://github.com/limine-bootloader/limine.git --branch=$(limine_branch) --depth=1 $@

$(limine_bin): $(limine_dir)
	$(MAKE) -C $(limine_dir)

