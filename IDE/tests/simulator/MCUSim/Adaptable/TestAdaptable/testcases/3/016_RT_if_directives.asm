; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device "016_RT_if_directives.procdef"

S0      REG     0x0
S1      REG     0x1
S2      REG     0x2
S3      REG     0x3
S4      REG     0x4
S5      REG     0x5
S6      REG     0x6
S7      REG     0x7
S8      REG     0x8
S9      REG     0x9
SA      REG     0xA
SB      REG     0xB
SC      REG     0xC
SD      REG     0xD
SE      REG     0xE
SF      REG     0xF

org     0

start:
        LOAD    S0, #10
        ;; step 3
        ;; reg[0] == 10
        ;; reg[1] == 0
        
        IF   S0 == #10
            LOAD    S6,#00h
        ELSE
            LOAD    S6,#ffh
        ENDIF

; ---------------------------------------------------
; allowed conditions
; == !=  <  >   <=  >=  &     !&


        IF           S0 != #10
            LOAD    S5,#5
        ELSE
            LOAD    S6,#6
        ENDIF

        
;-----------------------------------------------------

        IF           S0 > #10
            LOAD    S5,#5
        ELSE
            LOAD    S6,#6
        ENDIF

        
;-----------------------------------------------------
        IF           S0 < #10
                
            LOAD    S5,#5
        ELSE
            LOAD    S6,#6
        ENDIF

        
;--------------------------------------------------------
        IF           S0 >= #10
            LOAD    S5,#5
        ELSE
            LOAD    S6,#6
        ENDIF

        
;-------------------------------------------------------------
        IF           S0 <= #10
            LOAD    S5,#5
        ELSE
            LOAD    S6,#6
        ENDIF

        
;----------------------------------------------------------        
        IF           S0 & #10
            LOAD    S5,#5
        ELSE
            LOAD    S6,#6
        ENDIF

        
;------------------------------------------------------------
       IF           S0 !& #10
           LOAD    S5,#5
       ELSE
           LOAD    S6,#6
       ENDIF

        
;-----------------------------------------------------------------