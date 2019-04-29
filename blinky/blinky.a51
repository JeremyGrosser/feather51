.include "efm8ub1.a51"

.module blinky
.area CODE (REL)

main:
    mov TMOD,#0x01          ; Timer 0 is a 16-bit timer
    setb ET0                ; enable Timer 0 interrupts
    setb TR0                ; run Timer 0

idle:
    mov PCON,#0x01        ; idle mode, wait for interrupt
    mov PCON,PCON         ; dummy 3 cycle instruction
    ajmp idle

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
