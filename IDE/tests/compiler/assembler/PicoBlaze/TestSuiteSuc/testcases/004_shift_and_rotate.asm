; Compiler test case for Assembler
; instruction opcodes

DEVICE          KCMPS3

TESTsymb1    SET        1h
TESTsymb2    SET        0x11
TESTsymb3    SET        1
TESTsymb4    SET        3d
TESTsymb5    EQU        4h
TESTsymb6    EQU        0x05
TESTsymb7    EQU        6
TESTsymb8    EQU        7
TESTsymb9    REG        8h
TESTsymb10    REG        9h
TESTsymb11    REG        Ah
TESTsymb12    REG        Bh
TESTsymb13    DATA        Ch
TESTsymb14    DATA        0x05
TESTsymb15    DATA        2
TESTsymb16    DATA        3d
TESTsymb17    CODE        0h + 2h
TESTsymb18    CODE        2h
TESTsymb19    CODE        2h
TESTsymb20    CODE        2h
TESTsymb21    DEFINE        3h
TESTsymb22    DEFINE        4h
TESTsymb23    DEFINE        5h
TESTsymb24    DEFINE        TESTsymb1 + TESTsymb11



Start:        

; Shift and rotate
        SR0       TESTsymb10
        SR1       TESTsymb10
        SRX        TESTsymb10
        SRA        TESTsymb10
        RR         TESTsymb10

        SL0       TESTsymb10
        SL1       TESTsymb10
        SLX       TESTsymb10
        SLA       TESTsymb10
        RL        TESTsymb10
        
scratchpad_transfers:
STORE           s0, 04
; Write register sX to RAM location 04
FETCH           s1, 04
; Read RAM location 04 into register sX
        END

