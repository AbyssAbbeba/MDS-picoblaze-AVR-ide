device "004_indir_jump.procdef"

org             0

start:
        LOAD sA, #0b00000001
        LOAD sB, #0b00000001
        ;; step 2
        ;; pc == 2
        JUMP@ (sA, sB)
        ;; step
        ;; pc == 257

 org            0b100000001
        LOAD    s0,#11

        jump    $

        ;; step
        ;; pc == 258
        ;; reg[0] == 11
