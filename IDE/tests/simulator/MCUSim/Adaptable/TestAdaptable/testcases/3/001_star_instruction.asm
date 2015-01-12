device "001_star_instruction.procdef"

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

a define 0
b define 1


org             0

start:
        regbank         #a
        load            s1,#5h
        ;; step 2
        ;; reg[1] == 5
        ;; reg[0] == 0
        regbank         #b
        ;; step
        ;; reg[16] == 0
        ;; reg[17] == 0

        load            s1,#2h
        ;; step 
        ;; reg[16] == 0
        ;; reg[17] == 2

        regbank         #a
        star   s0,s1
        ;; step 2
        ;; reg[1] == 5
        ;; reg[0] == 0
        regbank         #b
        ;; step
        ;; reg[16] == 5
        ;; reg[17] == 2
        star   s2,#50
        regbank         #a
        ;; step 2
        ;; reg[2] == 50