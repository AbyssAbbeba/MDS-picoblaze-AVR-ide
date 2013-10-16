; TEST CASE
; testing run time directives, rtif rtfor, rtwhile
device kcpsm3
org     0

;start:
     ;   LOAD    S0, #10
        ;; step 3
        ;; reg[0] == 10
        ;; reg[1] == 0
        
    ;    RT_FOR          S0,10
    ;    add              S6,#1
    ;    RT_ENDF
        
      
        RT_FOR          S0,10..15
        add              S7,#1
        RT_ENDF

        
        RT_FOR          S0,10..50,5
        add              S8,#1
        RT_ENDF

        jump            $
        ;; step 50
        ;; reg[0] == 50
        ;; reg[6] == 10
        ;; reg[7] == 5
        ;; reg[8] == 8