device "003_picoblazecopy.procdef"


ORG 0
JUMP    START
                        ;; step 5
                        

START:
        ADD     S0,#1
        LOAD    S2,S1
        LOAD    S3,S0
        ADD     S0,S1
                        ;; reg[0] == 1
                        ;; reg[2] == 0
                        ;; reg[3] == 1
                        
        REPT    8
        ADD     S5,#1
        ENDR
                        ;; step 8
                        ;; reg[5] == 8
                        

LOAD sA,#8Eh ;#8E + 43 = D1 sA = D1 which is not zero (Z=0) and with no overflow (C=0).
ADD sA,#43h
                        ;; step 2
                        ;; flag[z] == false
                        ;; flag[c] == false
                        ;; reg[10] == 0xD1


LOAD sA,#8Eh; #8E + 8E = 11C sA = 1C which is not zero (Z=0) but there was an overflow (C=0).
ADD sA,sA
                        ;; step 2
                        ;; flag[z] == false
                        ;; flag[c] == true
                        ;; reg[10] == 0x1C
LOAD sA,#8Eh ;#8E + 72 = 100 sA = 00 which is zero (Z=1) but there was also an overflow that made this happen (C=1)
ADD sA,#72h
                        ;; step 2
                        ;; flag[z] == true
                        ;; flag[c] == true
                        ;; reg[10] == 0x00

 ;Carry from previous operation
;[sB, sA] = A2 7B + 5E 1A = 10095
                        ;; step 2
 LOAD sA, #7Bh
LOAD sB, #A2h
                        
;A2 + 5E + 0 = 100 sB = 00, Z=0, C=1
;7B + 1A = 95 sA = 95, Z=0, C=0.
;ADD sA, #85
;ADDCY sB, #5D


