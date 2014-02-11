ORG 0
JUMP    START
                        ;; step 5
                        
device kcpsm1cpld
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
                        

LOAD s1,#8Eh ;#8E + 43 = D1 s1 = D1 which is not zero (Z=0) and with no overflow (C=0).
ADD s1,#43h
                        ;; step 2
                        ;; flag[z] == false
                        ;; flag[c] == false
                        ;; reg[10] == 0xD1


LOAD s1,#8Eh; #8E + 8E = 11C s1 = 1C which is not zero (Z=0) but there was an overflow (C=0).
ADD s1,s1
                        ;; step 2
                        ;; flag[z] == false
                        ;; flag[c] == true
                        ;; reg[10] == 0x1C
LOAD s1,#8Eh ;#8E + 72 = 100 s1 = 00 which is zero (Z=1) but there was also an overflow that made this happen (C=1)
ADD s1,#72h
                        ;; step 2
                        ;; flag[z] == true
                        ;; flag[c] == true
                        ;; reg[10] == 0x00

 ;Carry from previous operation
;[sB, s1] = A2 7B + 5E 1A = 10095
                        ;; step 2
 LOAD s1, #7Bh
LOAD s2, #A2h
                        
;A2 + 5E + 0 = 100 sB = 00, Z=0, C=1
;7B + 1A = 95 s1 = 95, Z=0, C=0.
;ADD s1, #85
;ADDCY sB, #5D


