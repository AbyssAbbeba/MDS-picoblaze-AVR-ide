; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm3
org     0

start:
        load    s2,#9
        load    s0,#10
        load    s1,#9
        ;; step 3
        ;; reg[0] == 10
        ;; reg[1] == 8
        
        while   s1  != #10
            add     s1,#1
        endw

        jump    next1
        ;; step
        ;; pc == 3
        ;; flag[z] == false
        ;; flag[c] == true
        ;; step 3
        ;; reg[1] == 10
        ;; flag[z] == true
        ;; flag[c] == false
        ;; step
        ;; pc == 6
next1:

