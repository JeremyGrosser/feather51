.module blinky

; EFM8UB1 SFRs
WDTCN       = 0x97
CKCON0      = 0x8E
SFRPAGE     = 0xA7
P3          = 0xB0
; page 0x20
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
.org 0x0080 ; code starts after the interrupt vector

_start:
    clr EA                  ; disable global interrupts
    mov WDTCN,#0xDE         ; disable watchdog
    mov WDTCN,#0xAD
    mov SFRPAGE,#0x20
    anl P3MDOUT,#0xFD       ; P3.1 is an open drain output
    orl CKCON0,#0x02        ; Timer 0 uses SYSCLK
    mov TMOD,#0x01          ; Timer 0 is a 16-bit timer
    setb ET0                ; enable Timer 0 interrupts
    setb EA                 ; enable global interrupts
    setb TR0                ; run Timer 0

main:
    mov PCON,#0x01        ; idle mode, wait for interrupt
    mov PCON,PCON         ; dummy 3 cycle instruction
    ajmp main

T0_ISR:
    jb P3.1,led_off
led_on:
    setb P3.1
    ajmp reset_timer
led_off:
    clr P3.1
reset_timer:
    mov TH0,#0xFF            ; don't use the top 8 bits of T0
    clr TF0                  ; reset Timer 0 interrupt
    reti
