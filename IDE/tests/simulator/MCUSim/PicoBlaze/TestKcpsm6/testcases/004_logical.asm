
    device kcpsm6

    ORG         0x00
    JUMP        Start
    ;; STEP
    ;; STEP
    ;; REG[0] == 1
    ;;STEP
    ;; REG[1]== 2
    ;;STEP
    ;; REG[2]== 4
    ;;STEP
    ;; REG[3]== 8
Start:
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
    ;;REG[2] == 6
    ;;STEP
    ;; REG[3] == 10

    XOR         s2,s1
    XOR         s3,s1
    

    CALL        Podprog

;----------------------
Podprog:
    LOAD        s3,#50
    ;;STEP 2
    ;; REG[3] == 50
    RETURN

    
    