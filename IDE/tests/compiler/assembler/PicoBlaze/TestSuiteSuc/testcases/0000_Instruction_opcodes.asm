; Compiler load case for Assembler
; instruction opcodes
device kcpsm1cpld

loadsymb1    SET        1h
loadsymb2    SET        0x11
loadsymb3    SET        1
loadsymb4    SET        3d
loadsymb5    EQU        4h
loadsymb6    EQU        0x05
loadsymb7    EQU        6
loadsymb8    EQU        7
loadsymb9    REG        1h
loadsymb10    REG        2h
loadsymb11    REG        3h
loadsymb12    REG        4h
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


          ORG        0x000
          JUMP        Start
          JUMP        0x000
Start:        
        JUMP        Z, 0x000
        JUMP        NZ,0x000
        JUMP        C, 0x000
        JUMP        NC,0x000
    
     ; Arithmetics
        ADD         loadsymb9,#12
        ADDCY       loadsymb9,#20
        SUB         loadsymb9,#ffh
        SUBCY       loadsymb9,#0xff


        ADD         loadsymb9,loadsymb9
        ADDCY       loadsymb9,loadsymb9
        SUB         loadsymb9,loadsymb9
        SUBCY       loadsymb9,loadsymb10


; Logical
        LOAD      loadsymb9,#01h
        AND       loadsymb9,#0
        OR        loadsymb9,#1
        XOR       loadsymb9,#2


        LOAD      loadsymb9,loadsymb9
        AND       loadsymb9,loadsymb9
        OR        loadsymb9,loadsymb9
        XOR       loadsymb9,loadsymb9

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
    ; Others
        CALl      Podprog
        CALL      Z,Podprog
        CALL      NZ,Podprog
        CALL      C,Podprog
        CALL      NC,Podprog

Podprog:
        RETURN
        RETURN    Z
        RETURN    NZ
        RETURN    C
        RETURN    NC


  ;  JUMP aaa ADD sX,kk LOAD sX,kk SR0 sX
;JUMP Z,aaa ADDCY sX,kk AND sX,kk SR1 sX
;JUMP NZ,aaa SUB sX,kk OR sX,kk SRX sX
;JUMP C,aaa SUBCY sX,kk XOR sX,kk SRA sX
;JUMP NC,aaa load sX,kk load sX,kk RR sX
;CALL aaa ADD sX,sY LOAD sX,sY SL0 sX
;CALL Z,aaa ADDCY sX,sY AND sX,sY SL1 sX
;CALL NZ,aaa SUB sX,sY OR sX,sY SLX sX
;CALL C,aaa SUBCY sX,sY XOR sX,sY SLA sX
;CALL NC,aaa load sX,sY load sX,sY RL sX
