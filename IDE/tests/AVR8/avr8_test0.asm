.device atmega8
.cseg

lbl0:

    rjmp    $
    sbis    0x15, 0x7

.include "inc.asm"

	nop
