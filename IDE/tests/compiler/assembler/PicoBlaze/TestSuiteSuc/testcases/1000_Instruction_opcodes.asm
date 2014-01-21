; Compiler test case for Assembler
; instruction opcodes


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


          ORG        0x000
          JUMP        Start
          JUMP        0x000
Start:        
        JUMP        Z, 0x000
        JUMP        NZ,0x000
        JUMP        C, 0x000
        JUMP        NC,0x000
    
     ; Arithmetics
        ADD         testsymb9,#12
        ADDCY       testsymb9,#20
        SUB         testsymb9,#ffh
        SUBCY       testsymb9,#0xff
        COMPARE     testsymb9,#0

        ADD         testsymb9,testsymb9
        ADDCY       testsymb9,testsymb9
        SUB         testsymb9,testsymb9
        SUBCY       testsymb9,testsymb10
        COMPARE     testsymb9,testsymb11

; Logical
        LOAD      testsymb9,#01h
        AND       testsymb9,#0
        OR        testsymb9,#1
        XOR       testsymb9,#2
        TEST      testsymb9,#3

        LOAD      testsymb9,testsymb9
        AND       testsymb9,testsymb9
        OR        testsymb9,testsymb9
        XOR       testsymb9,testsymb9
        TEST      testsymb9,testsymb9
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
    ; Others
        CALl      Podprog
        CALL      Z,Podprog
        CALL      NZ,Podprog
        CALL      C,Podprog
        CALL      NC,Podprog

        STORE     TESTsymb9,TESTsymb13
        STORE     TESTsymb10,@10
        FETCH     TESTsymb11,TESTsymb13
        FETCH     TESTsymb12,@5

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
;JUMP NC,aaa COMPARE sX,kk TEST sX,kk RR sX
;CALL aaa ADD sX,sY LOAD sX,sY SL0 sX
;CALL Z,aaa ADDCY sX,sY AND sX,sY SL1 sX
;CALL NZ,aaa SUB sX,sY OR sX,sY SLX sX
;CALL C,aaa SUBCY sX,sY XOR sX,sY SLA sX
;CALL NC,aaa COMPARE sX,sY TEST sX,sY RL sX
