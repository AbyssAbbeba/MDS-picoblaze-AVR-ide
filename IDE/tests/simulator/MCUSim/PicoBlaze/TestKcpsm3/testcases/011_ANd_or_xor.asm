device kcpsm3

        ; file for testing and or xor instructions
        org 00h



Start:
; Logical
    LOAD        s0,#1
    LOAD        s1,#2
    LOAD        s2,#4
    LOAD        s3,#8
    ;;STEP 4
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