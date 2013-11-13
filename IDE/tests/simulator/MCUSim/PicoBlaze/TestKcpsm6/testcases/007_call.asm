

device kcpsm6

org             0

start:
                LOAD    s5, #10
                CALL    print_decimal
                JUMP    $

print_decimal:  COMPARE s5, #10
                JUMP C, start
                RETURN
                

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
        ;; step
      
        ;; step 
        
      
      
        ;; step
      
        ;; step
        ;; pc == 2
