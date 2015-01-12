device "003_aritmetics.asm.procdef"
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
    JUMP        Start
    ;;STEP
    ;;PC==1
    
Start:
; Aritmetics
    LOAD        s0,#10
    LOAD        s1,#20
    ADD         s0,s1
    ;;STEP 3
    ;; REG[0]== 30
    ;;STEP
    ;; REG[0] == 10

    SUB         s0,s1
    CALL        Podprog

;----------------------
Podprog:
    LOAD        s3,#50
    RETURN

    
    