/*
 * A simple basic test code solely for debugging purposes.
 */

;include "kcpsm3_mac.inc"


; jump            ; <-- error
; load x, y       ; <-- error
; local           ; <-- error


main:
        LOAD 4,#5
        LOAD 5,#6
        ADD 4,#5

mylbl:  
        LOAD 4,#6
        ADD 5,4

        JUMP  main

        END                     ; End of assembly.
