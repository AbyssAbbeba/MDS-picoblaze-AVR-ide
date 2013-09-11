

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

    
    