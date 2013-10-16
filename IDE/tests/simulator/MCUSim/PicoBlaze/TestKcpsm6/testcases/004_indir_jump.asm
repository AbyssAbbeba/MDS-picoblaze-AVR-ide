

device kcmps6

org             0

start:
        LOAD sA, #0b00001001
        LOAD sB, #0b00000001

        JUMP@ (sA, sB)


 org            0b100100000001
        LOAD    s0,#11

        jump    $
