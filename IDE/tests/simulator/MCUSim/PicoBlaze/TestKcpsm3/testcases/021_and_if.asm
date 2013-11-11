device  kcpsm3

start:


        load s0,#0b00111011
        load s1,#0b01010101
        load s2,#0b01010101
        ;; step 3
        ;; reg[0] == 59
        ;; reg[1] == 85
        ;; reg[2] == 85

        if   s0  & 1
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

        if   0  & 1
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
        ;; reg[3] == 1
next2:


        if   s0  & #1
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
        ;; reg[3] == 2
next3:


        if   0  & #1
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
        ;; reg[3] == 3


; one try with different condition
next4:

        if   s0  & #10
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    $
        ;; step
        ;; flag[z] == false
        ;; flag[c] == false
        ;; step
        ;; step 2
        ;; reg[3] == 4
