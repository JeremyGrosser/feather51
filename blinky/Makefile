all: blinky.hex

blinky.hex:
	sdas8051 -closj blinky.asm 
	sdld -muni blinky.ihx blinky.rel
	packihx blinky.ihx >blinky.hex

clean:
	rm -f *.hex *.ihx *.lst *.map *.rel *.rst *.sym
