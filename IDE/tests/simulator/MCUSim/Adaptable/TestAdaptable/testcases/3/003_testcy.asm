
device "003_picoblazecopy.procdef"

org             0

start:
        LOAD sA, #CAh
        LOAD sB, #52h
        TEST sA, #FFh
        TESTCY sB, #FFh
        ;; step 3
        ;; reg[10] == 202
        ;; reg[11] == 82
        ;; flag[c] == false
        ;; flag[z] == false

        ;; step
        ;; flag[c] == true
        ;; flag[z] == false


; bits in total are ‘1’ so parity is odd. Z=0, C=1.
        LOAD sA, #CAh
        LOAD sB, #52h
        TEST sA, #0b00000100
        TESTCY sB, #0b00100000
        ;; step 4
        ;; reg[10] == 202
        ;; reg[11] == 82
        ;; flag[z] == true
        ;; flag[c] == false
