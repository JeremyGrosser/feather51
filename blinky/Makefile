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

%.ihx %.map: %.rel
	@echo "LD	$^"
	$(LD) $(LDFLAGS) $@ $^

clean:
	rm -f $(TARGETS) *.lst *.rst *.map *.sym
