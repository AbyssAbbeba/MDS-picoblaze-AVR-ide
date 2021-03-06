; Compiler load case for Assembler
; instruction opcodes

device kcpsm1

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
loadsymb13    EQU        Ch
loadsymb14    EQU        0x05
loadsymb15    EQU        2
loadsymb16    EQU        3d
loadsymb17    CODE        0h + 2h
loadsymb18    CODE        2h
loadsymb19    CODE        2h
loadsymb20    CODE        2h
loadsymb21    DEFINE        3h
loadsymb22    DEFINE        4h
loadsymb23    DEFINE        5h
loadsymb24    DEFINE        loadsymb1 + loadsymb11



Start:        

; Shiload and rotate
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
        

        END

