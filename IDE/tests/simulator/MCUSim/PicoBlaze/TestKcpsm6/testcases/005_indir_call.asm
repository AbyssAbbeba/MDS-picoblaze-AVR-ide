

device kcmps6

org             0

start:
        LOAD sA, #CAh
        LOAD sB, #52h

        CALL@ (sA, sB)


 org            0b100100000001
        LOAD    s0,#11
        RETURN
        
        jump    $