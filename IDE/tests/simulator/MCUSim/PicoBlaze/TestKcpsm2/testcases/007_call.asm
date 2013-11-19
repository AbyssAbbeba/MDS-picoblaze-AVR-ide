

device kcpsm2

org             0

start:
                LOAD    s5, #10
                CALL    print_decimal
                JUMP    $

print_decimal:  load s5, #10
                JUMP C, start
                RETURN
                

        ;; step
        ;; pc == 1
        ;; step
        ;; pc == 3
        ;; step 
        ;; pc == 4

        ;; step
      
        ;; step 
        
      
      

