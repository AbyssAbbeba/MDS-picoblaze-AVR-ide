; Compiler test case for Assembler
; instruction opcodes
device kcpsm6


s1       EQU        04h
s2       EQU        0x04
s3       EQU        6
s4       EQU        7


Start:


; Logical
        LOAD      s1,#01h
        LOAD      s1,#5
        LOAD      s1,#05
        LOAD      s1,#0x05
        LOAD      s1,#0b101
        LOAD      s1,#0b00000101
        LOAD      s1,#5D
        LOAD      s1,#5O
        LOAD      s1,#5Q
        LOAD      s1,#5h
        LOAD      s1,#101b
        LOAD      s1,#00000101b
        
        AND       s1,#5
        AND       s1,#05
        AND       s1,#0x05
        AND       s1,#0b101
        AND       s1,#0b00000101
        AND       s1,#5D
        AND       s1,#5O
        AND       s1,#5Q
        AND       s1,#5h
        AND       s1,#101b
        AND       s1,#00000101b
        
        OR        s1,#5
        OR        s1,#5
        OR        s1,#05
        OR        s1,#0x05
        OR        s1,#0b101
        OR        s1,#0b00000101
        OR        s1,#5D
        OR        s1,#5O
        OR        s1,#5Q
        OR        s1,#5h
        OR        s1,#101b
        OR        s1,#00000101b
        
        XOR       s1,#5
        XOR       s1,#5
        XOR       s1,#05
        XOR       s1,#0x05
        XOR       s1,#0b101
        XOR       s1,#0b00000101
        XOR       s1,#5D
        XOR       s1,#5O
        XOR       s1,#5Q
        XOR       s1,#5h
        XOR       s1,#101b
        XOR       s1,#00000101b
        
        TEST      s1,#5
        TEST      s1,#5
        TEST      s1,#05
        TEST      s1,#0x05
        TEST      s1,#0b101
        TEST      s1,#0b00000101
        TEST      s1,#5D
        TEST      s1,#5O
        TEST      s1,#5Q
        TEST      s1,#5h
        TEST      s1,#101b
        TEST      s1,#00000101b

        LOAD      s1,s2
        AND       s1,s2
        OR        s1,s2
        XOR       s1,s2
        TEST      s1,s2

END

