; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm3
org     0

start:
        LOAD    S0, #10
        ;; step 
        ;; reg[0] == 10
        
        FOR          S0,2
        add              S6,#1
        ENDF
        ;; step         # load
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # add
        ;; reg[6] == 1
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # add
        ;; reg[6] == 2
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == true
        
      
        FOR          S0,10..12
        add              S7,#1
        ENDF
        ;; step         # load
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # add
        ;; reg[7] == 1
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # add
        ;; reg[7] == 2
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == true
        
        FOR          S0,10..20,5
        add              S8,#1
        ENDF

        jump            next1
        ;; step         # load
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # add
        ;; reg[8] == 1
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # add
        ;; reg[8] == 2
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == true
        ;; step         # jump to next for

        ;; # test of nested for
next1:
        FOR     s0,2
            FOR         s0,1
                add         s7,#1
            ENDF
            add     s7,#1
        ENDF
        ;; step         # load  1
        ;; step         # compare 1
        ;; step         # jump z 1
            ;; flag[z] == false
        ;; step         # load  2
        ;; step         # compare 2
        ;; step         # jump z 2
            ;; flag[z] == false
        ;; step         # add 2
            ;; reg[7] == 0
        ;; step         # add 2
        ;; step         # jump to compare 2
        ;; step         # compare       2
        ;; step         # jump z 2
            ;; flag[z] == true
        ;; step         # add 1
        ;; step         # add 1
        ;; step         # jump to compare 1
        ;; step         # compare 1
        ;; step         # jump z 1
            ;; flag[z] == false
        ;; step         # load  2
        ;; step         # compare 2
        ;; step         # jump z 2
            ;; flag[z] == false
        ;; step         # add 2
            ;; reg[7] == 0
        ;; step         # add 2
        ;; step         # jump to compare 2
        ;; step         # compare       2
        ;; step         # jump z 2
            ;; flag[z] == true
        ;; step         # add 1
        ;; step         # add 1
        ;; step         # jump to compare 1
        ;; step         # compare 1
        ;; step         # jump z 1
            ;; flag[z] == true
        
 

        