; test case for directives
DEVICE  KCPSM3


s10     AUTOSPR
s11     AUTOSPR
s12     AUTOSPR
s13     AUTOSPR
s14     AUTOSPR
s15     AUTOSPR
s16     AUTOSPR
s17     AUTOSPR
s18     AUTOSPR
s19     AUTOSPR

a1      EQU     580
a2      EQU     0xFFFFFF
a3      EQU     0
a4      EQU     0b11001
a5      EQU     0b0001
a6      EQU     0b00010001
a7      EQU     0b00000001
a8      EQU     3
a9      EQU     1h
a10     EQU     11h
a11     EQU     AAh
a12     EQU     0x04

b1      SET     580
b2      SET     0xFFFFFF
b3      SET     0
b4      SET     0b11001
b5      SET     0b0001
b6      SET     0b00010001
b7      SET     0b00000001
b8      SET     3
b9      SET     1h
b10     SET     11h
b11     SET     AAh
b12     SET     0x04

CONSTANT        c1,546
CONSTANT        c2,0xffff
CONSTANT        c3,12
CONSTANT        c4,0
CONSTANT        c5,0b00000000
CONSTANT        c6,0b001
CONSTANT        c7,0b01
CONSTANT        c8,0b00010101
CONSTANT        c9,0b11110
CONSTANT        c10,1h
CONSTANT        c11,FEh
CONSTANT        c12,0x03

VARIABLE        d1,546
VARIABLE        d2,0xffff
VARIABLE        d3,12
VARIABLE        d4,0
VARIABLE        d5,0b00000000
VARIABLE        d6,0b001
VARIABLE        d7,0b01
VARIABLE        d8,0b00010101
VARIABLE        d9,0b11110
VARIABLE        d10,1h
VARIABLE        d11,FEh
VARIABLE        d12,0x03

NAMEREG         1,AHOJ
NAMEREG         2,TE
NAMEREG         3,VY
NAMEREG         4,TAM
NAMEREG         5,DOLE
NAMEREG         6,ZDRA
NAMEREG         7,VIM
NAMEREG         8,VAS
NAMEREG         9,SECKY
NAMEREG         10,HOLA
NAMEREG         11,OLA
NAMEREG         12,HEJ


f1      REG     1
f2      REG     2
f3      REG     3
f4      REG     4
f5      REG     5
f6      REG     6
f7      REG     7
f8      REG     8
f9      REG     9
f10     REG     10
f11     REG     11
f12     REG     12

g1      DATA     1
g2      DATA     2
g3      DATA     3
g4      DATA     4
g5      DATA     5
g6      DATA     6
g7      DATA     7
g8      DATA     8
g9      DATA     9
g10     DATA     11h
g11     DATA     Ah
g12     DATA     0x04


h1      CODE     580
h2      CODE     0xFFFFFF
h3      CODE     0
h4      CODE     0b11001
h5      CODE     0b0001
h6      CODE     0b00010001
h7      CODE     0b00000001
h8      CODE     3
h9      CODE     1h
h10     CODE     11h
h11     CODE     AAh
h12     CODE     0x04

i1      PORT     580
i2      PORT     0xFFFFFF
i3      PORT     0
i4      PORT     0b11001
i5      PORT     0b0001
i6      PORT     0b00010001
i7      PORT     0b00000001
i8      PORT     3
i9      PORT     1h
i10     PORT     11h
i11     PORT     AAh
i12     PORT     0x04

m1      DEFINE   h8 + i3
m2      DEFINE   i8 + a11
m3      DEFINE   A3 * A4 * A5 * A6 * A7 + A8 - A9
M4      DEFINE   (b5 - B6)--B10

;--- CODE ------------- CODE -----------------CODE -----------------
        ORG     0x3FF
        ORG     0x000
        JUMP    Start


Start:
        REPT    B6
        SUB     s0,s2                  ; kkkkkkk
        ADD     s1,s2
        ENDR
        ;-----------
        REPT    C10
        SUB     s0,s2                  ; kkkkkkk
        ADD     s1,s2
        INPUT   S0,i1
        ENDR
        ;-------------
XYZ     MACRO   S0,S1,S2,S3,S4,S5,S6,S7,S8,S9,S9
        
            #IF      S0 == S1
            EXITM
            #ENDIF
        ENDM

XYZ0    MACRO   S0

            #IF      S0 == S1
            EXITM
            #ENDIF
        ENDM

XYZ1     MACRO   S0,S1

            #IF      S0 == S1
            EXITM
            #ENDIF
        ENDM
       
XYZ2     MACRO   S0,S1,S2

            #IF      S0 == S1
            EXITM
            #ENDIF
        ENDM

XYZ3     MACRO   S0,S1,S2,S3

            #IF      S0 == S1
            EXITM
            #ENDIF
        ENDM
XYZ4     MACRO   S0,S1,S2,S3,S4

            #IF      S0 == S1
            EXITM
            #ENDIF
        ENDM
XYZ5     MACRO   S0,S1,S2,S3,S4,S5

            #IF      S0 == S1
            EXITM
            #ENDIF
        ENDM
XYZ6     MACRO   S0,S1,S2,S3,S4,S5,S6

            #IF      S0 == S1
            EXITM
            #ENDIF
        ENDM
XYZ7     MACRO   S0,S1,S2,S3,S4,S5,S6,S7

            #IF      S0 == S1
            EXITM
            #ENDIF
        ENDM
XYZ8     MACRO   S0,S1,S2,S3,S4,S5,S6,S7,S8

            #IF      S0 == S1
            EXITM
            #ENDIF
        ENDM
        

        END 
        5555BALBALBALABLABALA

        