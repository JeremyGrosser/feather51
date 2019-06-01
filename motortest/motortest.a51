; EFM8UB1 SFRs
WDTCN       = 0x97
CKCON0      = 0x8E
SFRPAGE     = 0xA7
CLKSEL      = 0xA9
P3          = 0xB0
; page 0x20
REG1CN      = 0xC6
XBR2        = 0xE3
P2SKIP      = 0xCC
P2MDOUT     = 0xA6
P3MDOUT     = 0x9C

; Interrupt vector
.area INTV (ABS)
.org 0x0000
_int_reset:
    ljmp _start
.org 0x0003
_int_ex0:
    reti
.org 0x000b
_int_t0:
    ljmp T0_ISR
    .ds 5

.area CSEG (ABS,CON)
.org 0x00A0 ; code starts after the interrupt vector

_start:
    clr EA                  ; disable global interrupts
    mov WDTCN,#0xDE         ; disable watchdog
    mov WDTCN,#0xAD

    mov SFRPAGE,#0x20
    orl REG1CN,#0x84        ; Disable the internal voltage regulator. VREGIN
                            ; and VDD are tied together and we use an external
                            ; LDO. (RM 7.8)

    mov CLKSEL,#0x43        ; SYSCLK is HFOSC1/16 (3 MHz)
    orl CKCON0,#0x02        ; Timer 0 uses SYSCLK/48 (62.5 KHz)

    orl XBR2,#0x70          ; disable weak pullups on P2
    mov P2SKIP,#0x0F        ; Don't assign P2 to crossbar ports
    mov P3MDOUT,#0x02       ; P3.1 is a push-pull output (LED)
    mov SFRPAGE,#0x00

    mov P2MDOUT,#0x00       ; P2.0, P2.1, P2.2, P2.3 are open-drain
    mov P2,#0x00            ; All motor pins set low

    setb EA                 ; re-enable global interrupts

main:
    mov TMOD,#0x01          ; Timer 0 is a 16-bit timer
    setb ET0                ; enable Timer 0 interrupts
    setb TR0                ; run Timer 0

idle:
    mov PCON,#0x01          ; idle mode, wait for interrupt
    mov PCON,PCON           ; dummy 3 cycle instruction
    sjmp idle

T0_ISR:
    cpl P3.1                ; toggle LED
    cpl P2.0
    mov TH0,#0xE7
    mov TL0,#0x96
    clr TF0                 ; reset Timer 0 interrupt
    reti
