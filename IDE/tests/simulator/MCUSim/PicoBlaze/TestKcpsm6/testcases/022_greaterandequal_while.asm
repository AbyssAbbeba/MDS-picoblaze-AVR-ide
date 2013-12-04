; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm6
org     0


; four

start:

        load    s0,#10
        load    s1,#11
        ;; step 2
        ;; reg[0] == 10
        ;; reg[1] == 11

        while   s1  >= #10
            sub     s1,#1
        endw

        add     s1,#2
        jump    next1
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; flag[z] == false
        ;; step         # sub
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; flag[z] == true
        ;; step         # sub
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == true
        ;; flag[z] == false
        ;; step         # add 2
        ;; step         # jump to next 
        


next1:

        while   1  >= #10
            sub     s1,#1
        endw

        add     s1,#2
        jump    next2
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; flag[z] == false
        ;; step         # sub
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; flag[z] == true
        ;; step         # sub
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == true
        ;; flag[z] == false
        ;; step         # add 2
        ;; step         # jump to next
 
next2:

        while   s1  >= s0
            sub     s1,#1
        endw

        add     s1,#2
        jump    next3
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; flag[z] == false
        ;; step         # sub
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; flag[z] == true
        ;; step         # sub
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == true
        ;; flag[z] == false
        ;; step         # add 2
        ;; step         # jump to next
 
next3:

        while   1  >= 0
            sub     s1,#1
        endw

        add     s1,#2
        jump    next4
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; flag[z] == false
        ;; step         # sub
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; flag[z] == true
        ;; step         # sub
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == true
        ;; flag[z] == false
        ;; step         # add 2
        ;; step         # jump to next
 
next4:

        while   s1 >= #20
            sub     s1,#1
        endw

        add     s1,#2
        jump    $
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == true
        ;; flag[z] == false
        ;; step         # add2
        ;; step         # jump to next
