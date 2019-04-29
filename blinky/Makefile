AS := sdas8051
LD := sdld
PACKIHX := packihx

ASFLAGS := -cj
LDFLAGS := -i

TARGETS := blinky.ihx

all: $(TARGETS)

%.rel: %.a51
	@echo "AS    $^"
	$(AS) $(ASFLAGS) -o $^

%.lst: %.a51
	@echo "AS    $^"
	$(AS) $(ASFLAGS) -l $^

%.sym: %.a51
	@echo "AS	$^"
	$(AS) $(ASFLAGS) -s $^

%.hex: %.rel
	@echo "LD	$^"
	$(LD) $(LDFLAGS) $@ $^

%.ihx: %.hex
	@echo "IHX	$^"
	$(PACKIHX) $@ >$^

clean:
	rm -f $(TARGETS)
