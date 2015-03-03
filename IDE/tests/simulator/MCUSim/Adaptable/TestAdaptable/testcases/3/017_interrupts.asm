; TEST CASE
; testing interrupts behavior
device "017_interrupts.procdef"

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

org     0

start:

        ;;      flag[ie] == false
        ;;      step
        ;;      reg[0] == 10
        ;;      interrupt
        ;;      flag[ie] == false
        ;;       pc == 1
        LOAD    S0, #10
        ;; step 2
        ;; pc == 3
        ;; flag[ie] == true
        ENA 
        ;; step
        ;; reg[0] == 249
WAIT:   LOAD    s0,#250
        ;; step
        ;; flag[ie] == true
        ;; interrupt
        ;; pc == 5
        SUB     s0,#1
        COMPARE     s0,#1
        jumpNZ  WAIT
        DIS
        
INTERRUPT:
            ;; step
            ;; pc == 4095
            
            STORE       s0,0
            STORE       s1,1
            INPUT          S2,0
            OUTPUT         S2,0
            RETURN

org     3ffh
jump    INTERRUPT

            
        END