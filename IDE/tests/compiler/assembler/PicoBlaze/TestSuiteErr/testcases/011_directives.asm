; test case for directives
s0      AUTOREG
s1      AUTOREG
s2      AUTOREG
s3      AUTOREG
s4      AUTOREG
s5      AUTOREG
s6      AUTOREG
s7      AUTOREG
s8      AUTOREG
s9      AUTOREG

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

NAMEREG         d1,546
NAMEREG         d2,0xffff
NAMEREG         d3,12
NAMEREG         d4,0
NAMEREG         d5,0b00000000
NAMEREG         d6,0b001
NAMEREG         d7,0b01
NAMEREG         d8,0b00010101
NAMEREG         d9,0b11110
NAMEREG         d10,1h
NAMEREG         d11,FEh
NAMEREG         d12,0x03


f1      REG     580
f2      REG     0xFFFFFF
f3      REG     0
f4      REG     0b11001
f5      REG     0b0001
f6      REG     0b00010001
f7      REG     0b00000001
f8      REG     3
f9      REG     1h
f10     REG     11h
f11     REG     AAh
f12     REG     0x04

g1      DATA     580
g2      DATA     0xFFFFFF
g3      DATA     0
g4      DATA     0b11001
g5      DATA     0b0001
g6      DATA     0b00010001
g7      DATA     0b00000001
g8      DATA     3
g9      DATA     1h
g10     DATA     11h
g11     DATA     AAh
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
i12     PORT     0x04 | 0x11

; expresions test
ss1     EQU     +5
ss2     EQU     -5
ss3     EQU     +5h
ss4     EQU     -0x5
ss5     EQU     -5++5
ss6     EQU     +5+-5++45-66
ss7     EQU     ~0xFF
ss8     EQU     (0xFF & 0x00) & 0x0F * 5 / 5 % 2 | 0x01
ss9     EQU     (12 * 3) + (112 / 10)
ss10    EQU     (25 + 36) - (18)
ss11    EQU     -5 * +5
ss12    EQU     0x01 << 1
ss13    EQU     0x02 >> 1
ss14    EQU     0xff ^ 0x0f


;--- CODE ------------- CODE -----------------CODE -----------------
        ORG     0x000
        JUMP    Start
        ORG     0x3FF


Start:
        SKIP    6
        LOAD    s1,s2
        LOAD    s1,s2
        LOAD    s1,s2
        LOAD    s1,s2

        INPUT   s0,#0x01
        OUTput





        
        END

        