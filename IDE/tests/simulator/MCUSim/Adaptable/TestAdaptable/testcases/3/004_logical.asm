device "003_picoblazecopy.procdef"


    ORG         0x00
    JUMP        Start
Start:
    ;; STEP
    ;; step 2
    ;; reg[0] == 1
    ;; step
    ;; reg[0] == 0
    LOAD        s0, #0
    XOR         s0, #01h
    XOR         s0, #01h



    
    ;; STEP
    ;; REG[0] == 1
    ;;STEP
    ;; REG[1]== 2
    ;;STEP
    ;; REG[2]== 4
    ;;STEP
    ;; REG[3]== 8

; Logical
    LOAD        s0,#1
    LOAD        s1,#2
    LOAD        s2,#4
    LOAD        s3,#8
    ;;STEP
    ;; REG[0] == 1
    ;;STEP
    ;;REG[0] == 0

    AND         s0,#1
    AND         s0,s1

    ;;STEP
    ;; REG[0]==2
    ;;STEP
    ;;REG[1] == 2
    
    OR          s0,#2
    OR          s1,s1

    ;;STEP
    ;;REG[0] == 2
    ;;STEP 
    ;; REG[1] == 2


    
    