

device "003_picoblazecopy.procdef"

org             0

start:
                LOAD    s5, #10
                CALL    print_decimal
                JUMP    $

print_decimal:  COMPARE s5, #10
                JUMP C, start
                LOAD&RETURN s0, #39
                

        ;; step
        ;; pc == 1
        ;; step
        ;; pc == 3
        ;; step 
        ;; pc == 4
        ;; reg[5] == 10
        ;; reg[15] == 0
        ;; flag[c] == false
        ;; flag[z] == true
        
        ;; step 2
        ;; pc == 2
        ;; reg[0] == 39
        ;; reg[16] == 0
        ;; step
        ;; pc == 2
        ;; step
        ;; pc == 2
