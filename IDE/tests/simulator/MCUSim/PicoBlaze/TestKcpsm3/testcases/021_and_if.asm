; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm3
org     0

start:

        load    s0, #5
        load    s1, #1
               
        if   s0  &  1       
        add     s3,#1
        else
        sub     s3,#1
        endif
        ;; step
        ;; pc == 1
        ;; step
        ;; pc == 2
        load    s4,#8
        ;; step
        ;; pc == 3
        ;; step
        ;; pc == 4
        ;; step
        ;; pc == 5
        ;; step
        ;; pc == 7
        ;; step
        ;; pc == 8
        ;; reg[4] == 8
        ;; reg[3] == 255

        if   s0  &  #10
        else
        endif
        

        if   0   &  1
        else
        endif
        
        if   0   &  #10
        else
        endif
        
        jump   $