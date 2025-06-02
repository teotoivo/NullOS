export CPU_ARCH         = x86_64
export CROSS            = $(CPU_ARCH)-elf-
export CC               = $(CROSS)gcc
export CXX              = $(CROSS)g++
export LD               = $(CROSS)ld
export AR               = $(CROSS)ar
export AS               = $(CROSS)as
export ASM              = nasm

export CFLAGS           = -std=gnu17 -Wall -Wextra -g -MMD -MP
export LDFLAGS          = 
export ASMFLAGS         = 

BUILD_DIR               = build
ISO_IMAGE               = $(BUILD_DIR)/nullos.iso
KERNEL_ELF              = $(BUILD_DIR)/kernel.elf

