; Compiler test case for Assembler
; instruction opcodes
device kcpsm1
TESTsymb9               AUTOREG
TESTsymb13              AUTOREG
TESTsymb10              AUTOREG
TESTsymb11              AUTOREG
TESTsymb12              AUTOREG

Start:        
        JUMP        Z, 0x000
        JUMP        NZ,0x000
        JUMP        C, 0x000
        JUMP        NC,0x000
    
    ; Others
        CALl      Podprog
        CALL      Z,Podprog
        CALL      NZ,Podprog
        CALL      C,Podprog
        CALL      NC,Podprog

        STORE     TESTsymb9,@TESTsymb13
        STORE     TESTsymb10,@10
        FETCH     TESTsymb11,@TESTsymb13
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
