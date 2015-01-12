

device "002_comparecy.procdef"

org             0

start:

            LOAD sA, #8Eh
            COMPARE sA, #8Eh
            ; Z, equal
            ;; step 2
            ;; flag[c] == false
            ;; flag[z] == true
            
            LOAD sA, #8Eh
            COMPARE sA, #98h
            ; C, less_than
            ;; step 2
            ;; flag[c] == true
            ;; flag[z] == false

            LOAD sA, #7Bh
            LOAD sB, #A2h
            LOAD sC, #14h
            COMPARE sA, #7Bh
            COMPARECY sB, #A2h
            COMPARECY sC, #14h
            ;; step 6
            ;; reg[10] == 123
            ;; reg[11] == 162
            ;; reg[12] == 20
            ;; flag [z] == true
            ;; flag [c] == false

            LOAD sA, #7Bh
            LOAD sB, #A2h
            COMPARE sA, #7Bh
            COMPARECY sB, #B9h
            ;; step 4
            ;; reg[10] == 123
            ;; reg[11] == 162
            ;; flag [z] == false
            ;; flag [c] == true
           

