

device "002_comparecy.procdef"
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

            LOAD sA, #8Eh
            COMPARE sA, #8Eh
            ; Z, equal
            ;; step 2
            ;; flag[c] == false
            ;; flag[z] == true
            
            LOAD sA, #8Eh
            COMPARE sA, #98h
            ; C, less_than
            ;; step 2
            ;; flag[c] == true
            ;; flag[z] == false

            LOAD sA, #7Bh
            LOAD sB, #A2h
            LOAD sC, #14h
            COMPARE sA, #7Bh
            COMPARECY sB, #A2h
            COMPARECY sC, #14h
            ;; step 6
            ;; reg[10] == 123
            ;; reg[11] == 162
            ;; reg[12] == 20
            ;; flag [z] == true
            ;; flag [c] == false

            LOAD sA, #7Bh
            LOAD sB, #A2h
            COMPARE sA, #7Bh
            COMPARECY sB, #B9h
            ;; step 4
            ;; reg[10] == 123
            ;; reg[11] == 162
            ;; flag [z] == false
            ;; flag [c] == true
           

