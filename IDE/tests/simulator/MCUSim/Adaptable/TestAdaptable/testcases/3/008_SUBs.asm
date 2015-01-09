
device "003_picoblazecopy.procdef"
ORG 0


START:

;sA = 4B which is not zero (Z=0) and with no underflow (C=0).
;sA = 00 which is zero (Z=1) but there was no underflow (C=0).
;8E – B5 = 1D9
;sA = D9 which is not zero (Z=0) but there was an underflow (C=1).
;This is equivalent to 142 – 181 = -39 where D9 hex is the twos complement representation of -39.
;However, it is the users responsibility to implement and interpret signed arithmetic values and operations
LOAD sA, #8Eh                    ;8E - 43 = 4B
SUB sA, #43h                      ;
LOAD sA, #8Eh                     ;8E - 8E = 00
SUB sA, sA
LOAD sA, #8Eh
SUB sA, #B5h
                                ;; step
                                ;; reg[10] == 0x8E
                                ;; step
                                ;; flag[z] == false
                                ;; flag[c] == false
                                ;; step 2
                                ;; flag[z] == true
                                ;; flag[c] == false
                                ;; step 2
                                ;; flag[z] == false
                                ;; flag[c] == true
                                
;Carry from previous operation [sB, sA] = A2 7B - A1 B9 = 00C2 7B – B9 = (-)C2 A2 – A1 - 1 = 00
;sA = C2, Z=0, C=1.
;sB = 00, Z=0, C=0.
;LOAD sA, 7B
;LOAD sB, A2
;SUB sA, B9
;SUBCY sB, A1



;Zero from previous operation
;[sB, sA] = A2 7B - A2 7B = 0000
;7B – 7B = 00 sA = 00, Z=1, C=0.
;A2 – A2 - 0 = 00 sB = 00, Z=1, C=0.
;LOAD sA, 7B
;LOAD sB, A2
;SUB sA, sA
;SUBCY sB, sB

