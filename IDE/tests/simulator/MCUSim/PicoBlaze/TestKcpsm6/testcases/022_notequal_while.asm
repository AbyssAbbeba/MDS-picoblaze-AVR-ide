; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm6
org     0

start:
        load    s2,#9
        load    s0,#10
        load    s1,#9
        ;; step 3
        ;; reg[0] == 10
        ;; reg[1] == 9
        
        while   s1  != #10
            add     s1,#1
        endw

        sub     s1,#1
        jump    next1
        ;; step 2
        ;; flag[z] == false
        ;; flag[c] == true
        ;; step 4
        ;; reg[1] == 10
        ;; step

next1:

        while   1  != #10
            add     s1,#1
        endw

        sub     s1,#1
        jump    next2
        ;; step 2
        ;; flag[z] == false
        ;; flag[c] == true
        ;; step 4
        ;; reg[1] == 10
        ;; step

next2:

        while   s1  != s0
            add     s1,#1
        endw

        sub     s1,#1
        jump    next3
        ;; step 2
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step 4
        ;; reg[1] == 10
        ;; step

next3:

        while   1  != 0
            add     s1,#1
        endw

        sub     s1,#1
        jump    next4
        ;; step 2
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step 4
        ;; reg[1] == 10
        ;; step

next4:

