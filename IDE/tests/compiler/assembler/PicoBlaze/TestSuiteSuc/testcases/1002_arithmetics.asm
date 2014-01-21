; Compiler test case for Assembler
; instruction opcodes


s1       EQU        04h
s2       EQU        0x04
s3       EQU        6
s4       EQU        7



START:    
     ; Arithmetics
        ADD        s1,#01h
        ADD        s1,#5
        ADD        s1,#05h
        ADD        s1,#0x05
        ADD        s1,#0b101
        ADD        s1,#0b00000101
        ADD        s1,#5D
        ADD        s1,#5O
        ADD        s1,#5Q
        ADD        s1,#5h
        ADD        s1,#101b
        ADD        s1,#00000101b
        
        
        ADDCY      s1,#01h
        ADDCY      s1,#5
        ADDCY      s1,#05h
        ADDCY      s1,#0x05
        ADDCY      s1,#0b101
        ADDCY      s1,#0b00000101
        ADDCY      s1,#5D
        ADDCY      s1,#5O
        ADDCY      s1,#5Q
        ADDCY      s1,#5h
        ADDCY      s1,#101b
        ADDCY      s1,#00000101b

    
        SUB        s1,#01h
        SUB        s1,#5
        SUB        s1,#05h
        SUB        s1,#0x05
        SUB        s1,#0b101
        SUB        s1,#0b00000101
        SUB        s1,#5D
        SUB        s1,#5O
        SUB        s1,#5Q
        SUB        s1,#5h
        SUB        s1,#101b
        SUB        s1,#00000101b

       
        SUBCY      s1,#01h
        SUBCY      s1,#5
        SUBCY      s1,#05h
        SUBCY      s1,#0x05
        SUBCY      s1,#0b101
        SUBCY      s1,#0b00000101
        SUBCY      s1,#5D
        SUBCY      s1,#5O
        SUBCY      s1,#5Q
        SUBCY      s1,#5h
        SUBCY      s1,#101b
        SUBCY      s1,#00000101b
        

        COMPARE    s1,#01h
        COMPARE    s1,#5
        COMPARE    s1,#05h
        COMPARE    s1,#0x05
        COMPARE    s1,#0b101
        COMPARE    s1,#0b00000101
        COMPARE    s1,#5D
        COMPARE    s1,#5O
        COMPARE    s1,#5Q
        COMPARE    s1,#5h
        COMPARE    s1,#101b
        COMPARE    s1,#00000101b
        
        ADD         s2,s2
        ADDCY       s2,s2
        SUB         s2,s2
        SUBCY       s2,s3
        COMPARE     s2,s4

END


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
