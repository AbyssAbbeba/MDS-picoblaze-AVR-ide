

device "005_indir_call.procdef"
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
        LOAD sA, #0b00000001
        LOAD sB, #0b00000001
        ;; step 2
        ;; pc == 2
        CALL@ (sA,sB)
        ;; step
        ;; pc == 257
        jump    $

 org            0b100000001
        LOAD    s0,#11
        RETURN
        
       

        ;; step
        ;; pc == 258
        ;; reg[0] == 11
        ;; stack[0] == 3