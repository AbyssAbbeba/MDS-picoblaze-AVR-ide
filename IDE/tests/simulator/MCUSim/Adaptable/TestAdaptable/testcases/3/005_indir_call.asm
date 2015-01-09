

device "003_picoblazecopy.procdef"

org             0

start:
        LOAD sA, #0b00000001
        LOAD sB, #0b00000001
        ;; step 2
        ;; pc == 2
        CALL@ (sA, sB)
        ;; step
        ;; pc == 257
        jump    $

 org            0b100000001
        LOAD    s0,#11
        RETURN
        
       

        ;; step
        ;; pc == 258
        ;; reg[0] == 11
        ;; stack[0] == 3