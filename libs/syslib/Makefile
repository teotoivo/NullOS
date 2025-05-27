OBJDIR := ../../../build/libs/syslib
SRCS   := $(wildcard *.c)
OBJS   := $(SRCS:%=$(OBJDIR)/%.o)
TARGET := ../../../build/libs/libsyslib.a

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(AR) rcs $@ $(OBJS)

$(OBJDIR)/%.c.o: %.c
	@mkdir -p $(@D)
	$(CC) $(COMMON_CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(TARGET)

