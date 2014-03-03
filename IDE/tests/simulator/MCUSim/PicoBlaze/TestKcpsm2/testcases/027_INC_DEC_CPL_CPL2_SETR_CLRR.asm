; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm2
org     0


            load                s0,#255
            load                s1,#1
            ;; step                     # load
            ;; step                     # load
            inc                 s0
            ;; step                     # inc
            ;; reg[0] == 0
            ;; flag[z] == true
            ;; flag[c] == true
            dec                 s1
            ;; step                     # dec
            ;; reg[1] == 0
            ;; flag[z] == true
            ;; flag[c] == false
            dec                 s1
            ;; step                     # dec
            ;; reg[1] == 255
            ;; flag[z] == false
            ;; flag[c] == true
            load                s4,#0b01010101
            ;; step                     # load
            cpl                 s4
            ;; step                     # cpl
            ;; reg[4] == 0b10101010
            ;; flag[c] == true
            ;; flag[z] == false
            cpl2                s4
            ;; step                     # cpl2
            ;; reg[4] == 0b01010110
            ;; flag[c] == false
            ;; flag[z] == false
            setr                s1
            ;; step                     # setr
            ;; reg[1] == 0xff
            ;; flag[c] == false
            ;; flag[z] == false
            clrr                s1 
            ;; step                     # clrr
            ;; reg[1] == 0x00
            ;; flag[c] == false
            ;; flag[z] == true
            














            
            