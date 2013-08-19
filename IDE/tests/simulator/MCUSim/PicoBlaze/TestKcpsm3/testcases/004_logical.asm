
    DEVICE      KCPSM3

    ORG         0x00
    JUMP        Start

    
Start:
; Logical
    LOAD        s0,#1
    LOAD        s1,#2
    LOAD        s2,#4
    LOAD        s3,#8

    AND         s0,#1
    AND         s0,s1

    OR          s0,#2
    OR          s1,s1

    XOR         s2,s1
    XOR         s3,s1
    

    CALL        Podprog

;----------------------
Podprog:
    LOAD        s3,#50
    RETURN

    
    