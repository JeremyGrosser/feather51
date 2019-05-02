Revision A
- User LED should be relocated next to the USB connector
- Add a PTC fuse on VBUS
- Find smaller protection diodes
- Replace reset button with a right angle variant
- Add copper rings for clearance around mounting holes
- Find a 12V tolerant battery charge controller or add a jumper to disable it

Production image
- FORTH interpreter on UART/USB?

Test fixture specification
- Pogo pins connecting to the target board
- 3D printed spacer to align the test fixture, target board, and pogo pins. Use
  the mounting holes.
- A microswitch applies power to the target and begins testing when the pogo
  pins are compressed.
- Each test should light up a green LED on passing, or a red LED upon failure.
  The testing process should halt immediately if any test fails.
- Tests:
    1. Query the target C2 interface for chip revision and serial number
    2. Poke the port SFRs to configure each pin as an output and toggle them individually
    3. Configure all pins as inputs, toggle input values and read pin state
    4. Erase flash, program production image
    5. Issue reset, wait for acknowledgement of MBIST pass on UART
- Write the serial number and test results to nonvolatile storage (microSD card)
