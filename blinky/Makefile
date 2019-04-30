AS := sdas8051
LD := sdld
PACKIHX := packihx

ASFLAGS := -cj
LDFLAGS := -iu

TARGETS := blinky.ihx

all: $(TARGETS)

%.rel %.lst %.sym: %.a51
	@echo "AS    $^"
	$(AS) $(ASFLAGS) -slo $^

%.hex %.map: %.rel
	@echo "LD	$^"
	$(LD) $(LDFLAGS) $@ $^

%.ihx: %.hex
	@echo "IHX	$^"
	$(PACKIHX) $@ >$^

clean:
	rm -f $(TARGETS) *.lst *.rst *.map *.sym
