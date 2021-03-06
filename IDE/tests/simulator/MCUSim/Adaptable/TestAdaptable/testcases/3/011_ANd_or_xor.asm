device "011_ANd_or_xor.procdef"

S0      REG     0x0
S1      REG     0x1
S2      REG     0x2
S3      REG     0x3
S4      REG     0x4
S5      REG     0x5
S6      REG     0x6
S7      REG     0x7
S8      REG     0x8
S9      REG     0x9
SA      REG     0xA
SB      REG     0xB
SC      REG     0xC
SD      REG     0xD
SE      REG     0xE
SF      REG     0xF


        ; file for testing and or xor instructions
        org 00h



Start:
; Logical
    LOAD        s0,#1
    LOAD        s1,#2
    LOAD        s2,#4
    LOAD        s3,#8
    ;;STEP 4
    ;; REG[0] == 1

    AND         s0,#1
    AND         s0,s1

    ;;STEP
    ;; REG[0]==1
    ;;STEP
    ;;REG[1] == 2

    OR          s0,#2
    OR          s1,s1

    ;;STEP
    ;;REG[0] == 2
    ;;STEP
    ;; REG[1] == 2

    XOR         s2,s1
    XOR         s3,s1
    ;; step
    ;; reg[2] == 6
    ;; step
    ;; reg[3] == 10