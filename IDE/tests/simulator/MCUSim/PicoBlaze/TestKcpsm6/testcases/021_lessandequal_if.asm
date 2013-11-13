; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm6
org     0

start:
        load s0,#10
        load s1,#1
        ;; step 2
        ;; reg[0] == 10
        ;; reg[1] == 1

        if   s0  <= 1
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

        if   0  <= 1
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

        if   s0  <= #1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next3
        ;; step 2
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step
        ;; step 2
        ;; reg[3] == 253
next3:

        if   0  <= #1
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
        ;; reg[3] == 252
next4:

        if   s0  <= #50
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next5
        ;; step 2
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step
        ;; step 2
        ;; reg[3] == 253

next5:

        if   s0  <= #10
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next6
        ;; step 2
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step
        ;; step 2
        ;; reg[3] == 255

next6:

        if   s0 == s0
                add     s3,#1
            else
                sub     s3,#1
        endif

        jump    next7

next7:
        if      #10 == #10
                add     s3,#1
            else
                sub     s3,#1
        endif

        jump    $




        