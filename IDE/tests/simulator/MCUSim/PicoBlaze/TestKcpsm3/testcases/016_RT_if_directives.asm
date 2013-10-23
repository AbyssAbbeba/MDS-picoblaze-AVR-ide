; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm3
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