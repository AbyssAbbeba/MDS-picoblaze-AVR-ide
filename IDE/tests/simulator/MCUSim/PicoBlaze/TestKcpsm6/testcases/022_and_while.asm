; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm6
org     0


; four

start:

        load    s0,#0b10000001
        load    s1,#0b00000001
        ;; step 2
        ;; reg[0] == 129
        ;; reg[1] == 1

        while   s0  & #0b00000001
            sub     s0,#1
        endw

        add     s0,#1
        jump    next1
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # jump to test
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == true
        ;; step         # sub
        ;; step         # jump next 

next1:

        while   0  & #0b00000001
            sub     s0,#1
        endw

        add     s0,#1
        jump    next2
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # jump to test
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == true
        ;; step         # sub
        ;; step         # jump next 

next2:

        while   s0  & s1
            sub     s0,#1
        endw

        add     s0,#1
        jump    next3
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # jump to test
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == true
        ;; step         # sub
        ;; step         # jump next  

next3:

        while   0  & 1
            sub     s0,#1
        endw

        add     s0,#1
        jump    next4
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # jump to test
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == true
        ;; step         # sub
        ;; step         # jump next  

next4:

        while   s0 & s1
            sub     s0,#1
        endw

        add     s0,#1
        jump    $
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # jump to test
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == true
        ;; step         # sub
        ;; step         # jump next 
