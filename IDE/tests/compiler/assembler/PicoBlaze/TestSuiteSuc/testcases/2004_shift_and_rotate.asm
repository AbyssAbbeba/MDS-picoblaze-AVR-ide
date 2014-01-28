; Compiler load case for Assembler
; instruction opcodes

device kcpsm2

loadsymb1    SET        1h
loadsymb2    SET        0x11
loadsymb3    SET        1
loadsymb4    SET        3d
loadsymb5    EQU        4h
loadsymb6    EQU        0x05
loadsymb7    EQU        6
loadsymb8    EQU        7
loadsymb9    REG        8h
loadsymb10    REG        9h
loadsymb11    REG        Ah
loadsymb12    REG        Bh
loadsymb13    DATA        Ch
loadsymb14    DATA        0x05
loadsymb15    DATA        2
loadsymb16    DATA        3d
loadsymb17    CODE        0h + 2h
loadsymb18    CODE        2h
loadsymb19    CODE        2h
loadsymb20    CODE        2h
loadsymb21    DEFINE        3h
loadsymb22    DEFINE        4h
loadsymb23    DEFINE        5h
loadsymb24    DEFINE        loadsymb1 + loadsymb11



Start:        

; Shift and rotate
        SR0       loadsymb10
        SR1       loadsymb10
        SRX        loadsymb10
        SRA        loadsymb10
        RR         loadsymb10

        SL0       loadsymb10
        SL1       loadsymb10
        SLX       loadsymb10
        SLA       loadsymb10
        RL        loadsymb10
        
scratchpad_transfers:
load           s0, 04h
; Write register sX to RAM location 04
load           s1, 04h
; Read RAM location 04 into register sX
        END

