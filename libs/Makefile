SUBLIBS := $(wildcard */)   # e.g. syslib/

.PHONY: all clean $(SUBLIBS)

all: $(SUBLIBS)

$(SUBLIBS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean:
	-$(foreach dir,$(SUBLIBS),$(MAKE) -C $(dir) clean &&) true

