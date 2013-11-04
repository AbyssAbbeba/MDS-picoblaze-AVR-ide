; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm3
org     0


; four 


start:

        load    s0,#10
        load    s1,#8
        ;; step 2
        ;; reg[0] == 10
        ;; reg[1] == 8
        while   s0  == #10
            add     s0,#1
        endw

        sub     s0,#1
        jump    next1
        ;; step
        ;; pc == 3
        ;; flag[z] == false
        ;; flag[c] == true
        ;; step
        ;; pc == 4
        ;; # add
        ;; step 4
        ;; pc == 4
        ;; step 3
        ;; pc == 6
        ;; step
        ;; pc == 6
next1:
        
        while   0  == 0
            add     s0,#1
        endw

        jump  $


        