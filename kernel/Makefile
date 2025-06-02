# Expected external variable: CPU_ARCH (e.g., x86_64)
ARCH_DIR        = arch/$(CPU_ARCH)
ARCH_INC        = -I$(ARCH_DIR)/include

RAW_SRCS        = $(shell find . \( -name '*.c' -o -name '*.asm' \))
SRCS            = $(RAW_SRCS:./%=%)

OBJS            = $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS            = $(OBJS:.o=.d)
TARGET          = $(BUILD_DIR)/kernel.elf
LINKER_SCRIPT   = linker.ld

KERNEL_CFLAGS = -Iinclude \
	-mcmodel=kernel \
	-ffreestanding \
	-nostdlib

KERNEL_ASMFLAGS = -f elf64

.PHONY: all always clean

all: $(TARGET)

$(TARGET): $(OBJS) $(LINKER_SCRIPT)
	@echo "Linking $@"
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) -T $(LINKER_SCRIPT) $(OBJS) -o $@

# C compilation: add ARCH_INC if file is in arch/$(CPU_ARCH)/
$(BUILD_DIR)/%.c.o: %.c
	@echo "  CC   $<"
	@mkdir -p $(@D)
ifeq ($(filter $(ARCH_DIR)/%,$<),$<)
	$(CC) $(CFLAGS) $(KERNEL_CFLAGS) $(ARCH_INC) -c $< -o $@
else
	$(CC) $(CFLAGS) $(KERNEL_CFLAGS) -c $< -o $@
endif

# Assembly compilation
$(BUILD_DIR)/%.asm.o: %.asm
	@echo "  NASM   $<"
	@mkdir -p $(@D)
	$(ASM) $(ASMFLAGS) $(KERNEL_ASMFLAGS) $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)

