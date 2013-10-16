; TEST CASE
; testing interrupts behavior

org     0

start:

         ;;      flag[ie] == true
        ;;      step
        ;;      reg[0] == 10
        ;;      interrupt
        ;;      flag[ie] == true
        ;;       pc == 1
        LOAD    S0, #10
        ;; step
        ;; pc == 2
        ;; flag[ie] == true
        ENABLE INTERRUPT  
        ;; step
        ;; reg[0] == 250
WAIT:   LOAD    s0,#250
        ;; step
        ;; flag[ie] == true
        ;; interrupt
        ;; pc == 4
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