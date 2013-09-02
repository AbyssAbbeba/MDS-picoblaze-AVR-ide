

    ORG         0x00
    JUMP        Start

    
Start:
; Aritmetics
    LOAD        s0,#10
    LOAD        s1,#20
    ADD         s0,s1

    SUB         s0,s1
    CALL        Podprog

;----------------------
Podprog:
    LOAD        s3,#50
    RETURN

    
    