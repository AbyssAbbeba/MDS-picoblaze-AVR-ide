; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm3
org     0

start:

        load    s0,#10
        load    s1,#1
        
        if   s0  != #100
            add     s4,#1
            else
            sub     s4,#1
        endif

        jump    hop
        ;; step 3
        ;; flag[z] == false
        ;; flag[c] == true
        ;; step
        ;; step 2
        ;; reg[4] == 1

hop:        

        load s0,#10
        load s1,#1
        ;; step 2
        ;; reg[0] == 10
        ;; reg[1] == 1

        if   s0  != 1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next1
        ;; step
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step
        ;; step 2
        ;; reg[3] == 1
next1:

        if   0  != 1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next2
        ;; step
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step
        ;; step 2
        ;; reg[3] == 2
next2:


        if   s0  != #1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next3
        ;; step
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step
        ;; step 2
        ;; reg[3] == 3
next3:


        if   0  != #1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next4
        ;; step 2
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step
        ;; step 2
        ;; reg[3] == 4


; one try with different condition
next4:

        if   s0  != #10
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    $
        ;; step
        ;; flag[z] == true
        ;; flag[c] == false
        ;; step
        ;; step 2
        ;; reg[3] == 3

