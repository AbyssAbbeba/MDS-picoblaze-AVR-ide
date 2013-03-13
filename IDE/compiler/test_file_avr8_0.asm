.EQU io_offset = 0x23
xxx     equ     23h
.EQU porta = io_offset + 2
.DEF abc=R0

.CSEG                           ; Start code segment

    clr r2                      ; Clear register 2
    out porta,r2                ; Write to Port A
    clr abc
