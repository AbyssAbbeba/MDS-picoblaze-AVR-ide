; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm3
org     0


; four 

start:
        load s0,#10
        load s1,#1
        ;; step 2
        ;; reg[0] == 10
        ;; reg[1] == 1

        if   s0  == 1
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
        ;; reg[3] == 255
next1:

        if   0  == 1
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
        ;; reg[3] == 254
next2:


        if   s0  == #1
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
        ;; reg[3] == 253
next3:


        if   0  == #1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next4
        ;; step
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step
        ;; step 2
        ;; reg[3] == 252


; one try with different condition
next4:

        if   s0  == #10
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
        ;; reg[3] == 253