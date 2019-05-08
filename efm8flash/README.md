efm8flash is a tool for flashing EFM8UB1 microcontrollers over USB using the
factory bootloader.

# Getting started

## Runtime dependencies

- libhidapi-libusb0

## Build dependencies

- C compiler
- make
- pkg-config
- libhidapi-dev

## Building from source

    ./configure
    make
    make install

# Usage

    efm8flash flash.bin

## Converting Intel Hex files to binary

Many 8051 assemblers output Intel Hex format binaries. Use objcopy to convert
them to binary.

    objcopy -I ihex -O binary flash.ihx flash.bin

## hex2boot

The hex2boot step shown in the EFM8 documentation is not necessary when using
efm8flash.

## Forcing the device into bootloader mode

If the first byte of flash is not 0xFF upon power on, the EFM8UB1 will execute
user code rather than jumping to the bootloader. To force the device into
bootloader mode in this situation, attach a jumper between C2D and GND, then
press the reset button while the USB cable is connected.
