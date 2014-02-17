; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm3
org     0

start:
        load s1,#10
        load s0,#1
        ;; step 2
        ;; reg[1] == 10
        ;; reg[0] == 1

        if   s0  >= 1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next1
        ;; step 2
        ;; flag[z] == false
        ;; flag[c] == true
        ;; step
        ;; step 2
        ;; reg[3] == 255
next1:

        if   0  >= 1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next2
        ;; step 2
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step
        ;; step 2
        ;; reg[3] == 254
next2:

        if   s0  >= #1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next3
        ;; step 2
        ;; pc == 19
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step
        ;; step 2
        ;; reg[3] == 255
next3:

        if   0  >= #1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next4
        ;; step 2
        ;; flag[z] == true     
        ;; flag[c] == true
        ;; step
        ;; step 2
        ;; reg[3] == 0
next4:

        if   s0  >= #50
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next5
        ;; step 2
        ;; flag[z] == false
        ;; flag[c] == true
        ;; step
        ;; step 3
        ;; reg[3] == 0

next5:

        if   s0  >= #1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    $
        ;; step 2
        ;; flag[z] == true
        ;; flag[c] == true
        ;; step
        ;; step 2
        ;; reg[3] == 0 