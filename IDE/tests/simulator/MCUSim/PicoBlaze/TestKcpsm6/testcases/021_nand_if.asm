device kcpsm6

start:


        load s0,#0b00111011
        load s1,#0b01010101
        load s2,#0b01010101
        load s4,#0b10000000
        ;; step 4
        ;; reg[0] == 59
        ;; reg[1] == 85
        ;; reg[2] == 85
        ;; reg[4] == 128

        if   s0  !& 1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next1
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; reg[3] == 255
        ;; step         # jump to end (jump to next)
        ;; step         # jump to next test
next1:

        if   0  !& 1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next2
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; reg[3] == 254
        ;; step         # jump to end (jump to next)
        ;; step         # jump to next test
next2:


        if   s0  !& #1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next3
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; reg[3] == 253
        ;; step         # jump to end (jump to next)
        ;; step         # jump to next test
next3:


        if   0  !& #1
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next4
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; reg[3] == 252
        ;; step         # jump to end (jump to next)
        ;; step         # jump to next test


; one try with different condition
next4:

        if   s0  !& #10
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    next5
        ;; step         # test
        ;; step         # jump z
        ;; flag[z] == true
        ;; step         # add
        ;; reg[3] == 252
        ;; step         # jump to end (jump to next)
        ;; step         # jump to next test

next5:


        if   s0  !& s4
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    $
        ;; step         # test
        ;; pc ==  39
        ;; step         # jump z
        ;; pc == 39
        ;; flag[z] == false
        ;; step         # add
        ;; reg[3] == 252
        ;; step         # jump to end (jump to next)
        ;; step         # jump to next test
