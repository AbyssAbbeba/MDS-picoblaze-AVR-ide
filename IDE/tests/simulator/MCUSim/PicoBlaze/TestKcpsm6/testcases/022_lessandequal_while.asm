; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm6
org     0


; four

start:

        load    s0,#10
        load    s1,#9
        ;; step 2
        ;; reg[0] == 10
        ;; reg[1] == 9

        while   s1  <= #10
            add     s1,#1
        endw

        sub     s1,#2
        jump    next1
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # jump c
        ;; flag[c] == true
        ;; step         # add
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == true
        ;; step         # add
        ;; step         # jump to compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # jump c
        ;; flag[c] == false
        ;; step         # sub
        ;; step         # jump next
next1:

        while   1  <=  #10
            add     s1,#1
        endw

        sub     s1,#2
        jump    next2
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # jump c
        ;; flag[c] == true
        ;; step         # add
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == true
        ;; step         # add
        ;; step         # jump to compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # jump c
        ;; flag[c] == false
        ;; step         # sub
        ;; step         # jump next

next2:

        while   s1  <=  s0
            add     s1,#1
        endw

        sub     s1,#2
        jump    next3
        ;; step
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; step         # add
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; step         # add
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == true
        ;; step         # sub
        ;; step         # jump next
next3:

        while   1  <= 0
            add     s1,#1
        endw

        sub     s1,#2
        jump    next4
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; step         # add
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; step         # add
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == true
        ;; step         # sub
        ;; step         # jump next

next4:
        while   #50 <=  #20
            sub     s1,#1
        endw

        add     s1,#1
        jump    $
        ;; step         # compare
        ;; step         # jump c
        ;; flag[c] == false
        ;; flag[z] == false
        ;; step         # add2
        ;; step         # jump to next
      