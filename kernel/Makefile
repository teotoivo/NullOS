OBJDIR ?= build
SRCS   ?= $(shell find . -name '*.c' -o -name '*.asm' -o -name '*.S')
SRCS   := $(SRCS:./%=%)

OBJS   := $(SRCS:%=$(OBJDIR)/%.o)
DEPS   := $(OBJS:.o=.d)
TARGET := $(OBJDIR)/kernel.elf
LINKER_SCRIPT := linker.ld

COMMON_CFLAGS += -Iinclude \
	-ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-PIC \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -mcmodel=kernel

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) $(LINKER_SCRIPT)
	@echo "Linking $@"
	@mkdir -p $(@D)
	$(LD) $(COMMON_LDFLAGS) -T $(LINKER_SCRIPT) $(OBJS) -o $@

$(OBJDIR)/%.c.o: %.c
	@echo "  CC   $<"
	@mkdir -p $(@D)
	$(CC) $(COMMON_CFLAGS) -c $< -o $@

$(OBJDIR)/%.asm.o: %.asm
	@echo "  NASM   $<"
	@mkdir -p $(@D)
	$(NASM) $(NASMFLAGS) $< -o $@


$(OBJDIR)/%.S.o: %.S
	@echo "  AS   $<"
	@mkdir -p $(@D)
	$(AS) $< -o $@


clean:
	@rm -rf $(OBJDIR) $(TARGET)

