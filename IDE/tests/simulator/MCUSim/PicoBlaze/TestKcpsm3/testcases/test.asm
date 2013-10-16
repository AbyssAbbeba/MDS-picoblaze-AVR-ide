        org     0
        device kcpsm3


        ;RT_FOR          S0,10..15
        LOAD    0x0, #0xa
FOR_1:
        COMPARE 0x0, #0xf
        JUMP    Z, FOR_1_END
            add              S7,#1
        ; RT_ENDF
        ADD     0x0, #0x1
        JUMP    FOR_1
FOR_1_END:


    ;    RT_FOR          S0,10..50,5
        LOAD    0x0, #0xa
FOR_2:
        COMPARE 0x0, #0x32
JUMP    Z, FOR_2_END
            add              S8,#1
        ;RT_ENDF
        ADD     0x0, #0x5
        JUMP    FOR_2
FOR_2_END:

        jump    $

;; step 20
;; reg[7]  == 5
;; reg[8]  == 8