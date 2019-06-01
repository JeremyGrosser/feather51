AS := sdas8051
LD := sdld
OBJCOPY := objcopy

ASFLAGS := -cj
LDFLAGS := -iu

TARGETS := motortest.ihx motortest.bin

all: $(TARGETS)

%.rel %.lst %.sym: %.a51
	@echo "AS    $@"
	@$(AS) $(ASFLAGS) -slo $^

%.ihx %.map: %.rel
	@echo "LD    $@"
	@$(LD) $(LDFLAGS) $@ $^

%.bin: %.ihx
	@echo "BIN   $@"
	@$(OBJCOPY) -I ihex -O binary $< $@

clean:
	rm -f $(TARGETS) *.lst *.rst *.map *.sym
