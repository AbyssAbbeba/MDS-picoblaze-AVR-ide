

device "002_jumps.procdef"
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
    ORG         0x00
        
Start:
       
        ;; PC == 0
      
        LOAD    0x00,#1
        ;; STEP
        ;; PC == 1
        ;; REG[0x00] == 1  
        LOAD    0x00,#55
        ;; STEP
        ;; PC == 2
        ;; REG[0x00] == 55        
        JUMP    HOP1
        ;; STEP
        ;; PC == 3
HOP1:   JUMP    HOP2
        ;; STEP
        ;; PC == 4
HOP2:   JUMP    HOP3
        ;; STEP
        ;; PC == 5
HOP3:   JUMP    HOP4


        LOAD    s0,#1
        ;; STEP
        ;; PC == 7
HOP4:   JUMP    HOP5
        ;; STEP
        ;; PC == 8
HOP5:   JUMP    HOP6
        ;; STEP
        ;; PC == 0x0C

        LOAD    s0,#1
        ;;STEP
        ;;PC == 13
        LOAD    s0,#1
        LOAD    s0,#1
HOP6:   JUMP    HOP7
HOP7:   JUMP    HOP8
        LOAD    s0,#1
        LOAD    s0,#12
HOP8:   
        ;; STEP
        ;; PC == 16
END