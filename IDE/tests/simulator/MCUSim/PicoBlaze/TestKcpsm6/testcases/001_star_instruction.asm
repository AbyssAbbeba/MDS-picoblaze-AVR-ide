

device kcpsm6

org             0

start:
        regbank         a
        load            s1,#5h
        ;; step 2
        ;; reg[1] == 5
        ;; reg[0] == 0
        regbank         b
        ;; step
        ;; reg[16] == 0
        ;; reg[17] == 0

        load            s1,#2h
        ;; step 
        ;; reg[16] == 0
        ;; reg[17] == 2

        regbank         a
        star   s0,s1
        ;; step 2
        ;; reg[1] == 5
        ;; reg[0] == 0
        regbank         b
        ;; step
        ;; reg[16] == 5
        ;; reg[17] == 2

