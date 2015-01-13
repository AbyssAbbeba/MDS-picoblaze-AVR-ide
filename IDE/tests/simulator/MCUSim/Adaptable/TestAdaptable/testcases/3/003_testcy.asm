
device "003_testcy.procdef"
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


org             0

start:
        LOAD sA, #CAh
        LOAD sB, #52h
        TEST sA, #FFh
        TESTCY sB, #FFh
        ;; step 3
        ;; reg[10] == 202
        ;; reg[11] == 82
        ;; flag[c] == false
        ;; flag[z] == false

        ;; step
        ;; flag[c] == true
        ;; flag[z] == false


; bits in total are ‘1’ so parity is odd. Z=0, C=1.
        LOAD sA, #CAh
        LOAD sB, #52h
        TEST sA, #0b00000100
        TESTCY sB, #0b00100000
        ;; step 4
        ;; reg[10] == 202
        ;; reg[11] == 82
        ;; flag[z] == true
        ;; flag[c] == false
