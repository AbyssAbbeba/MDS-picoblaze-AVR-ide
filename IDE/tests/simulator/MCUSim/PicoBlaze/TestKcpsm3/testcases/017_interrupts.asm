; TEST CASE
; testing interrupts behavior
device kcpsm3
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
        ENABLE INTERRUPT  
        ;; step
        ;; reg[0] == 249
WAIT:   LOAD    s0,#250
        ;; step
        ;; flag[ie] == true
        ;; interrupt
        ;; pc == 5
        SUB     s0,#1
        CMP     s0,#1
        jump    NZ,WAIT
         DISABLE INTERRUPT
        
INTERRUPT:
            ;; step
            ;; pc == 1023
            
            STORE       s0,0
            STORE       s1,1
            IN          S2,0
            OUT         S2,0
            RETURNI ENABLE

org     3ffh
jump    INTERRUPT

            
        END