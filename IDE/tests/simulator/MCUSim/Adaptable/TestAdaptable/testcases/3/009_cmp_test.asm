; testcase for instructions compare and TEST

device "003_picoblazecopy.procdef"

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

    ORG         0x000


Start:

            LOAD        s0,#20
            LOAD        s1,#1
            LOAD        s2,#50
            LOAD        s3,#50
            LOAD        s4,#20
            LOAD        s5,#1
            ;; step 6
            ;; reg[0]==20
            ;; reg[1]==1
            ;; reg[2]==50
            ;; reg[3]==50
            ;; reg[4]==20
            ;; reg[5]==1
            COMPARE     s0,#20
            ;; step
            ;; interrupt
            ;; flag[z] != false
            ;; flag[c] != true
            ;; flag[pz] == false
            ;; flag[pc] == false
            ;; flag[i] == false
            ;; flag[ie] == false
            ;; pc == 7
            ;; step
            ;; pc == 8
            
            
        