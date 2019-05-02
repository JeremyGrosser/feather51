.module device

; EFM8UB1 SFRs
WDTCN       = 0x97
CKCON0      = 0x8E
SFRPAGE     = 0xA7
P3          = 0xB0
; page 0x20
REG1CN      = 0xC6
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

    orl CKCON0,#0x02        ; Timer 0 uses SYSCLK

setup_gpio:
    anl P3MDOUT,#0xFD       ; P3.1 is an open drain output (LED)

    mov SFRPAGE,#0x00

    setb EA                 ; re-enable global interrupts

    ajmp main
