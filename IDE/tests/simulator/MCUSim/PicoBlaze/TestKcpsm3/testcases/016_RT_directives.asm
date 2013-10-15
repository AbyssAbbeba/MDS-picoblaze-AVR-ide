; TEST CASE
; testing run time directives, rtif rtfor, rtwhile
device kcpsm3
org     0

start:
        LOAD    S0, #10
        ;; step 3
        ;; reg[0] == 10
        ;; reg[1] == 0
        
        RT_IF   S0 == #10
            LOAD    S6,#00h
        RT_ELSE
            LOAD    S6,#ffh
        RT_ENDIF

; ---------------------------------------------------
; allowed conditions
; == !=  <  >   <=  >=  &     !&


        RT_IF           S0 != #10
            LOAD    S5,#5
        RT_ELSE
            LOAD    S6,#6
        RT_ENDIF

        
;-----------------------------------------------------

        RT_IF           S0 > #10
            LOAD    S5,#5
        RT_ELSE
            LOAD    S6,#6
        RT_ENDIF

        
;-----------------------------------------------------
        RT_IF           S0 < #10
                
            LOAD    S5,#5
        RT_ELSE
            LOAD    S6,#6
        RT_ENDIF

        
;--------------------------------------------------------
        RT_IF           S0 >= #10
            LOAD    S5,#5
        RT_ELSE
            LOAD    S6,#6
        RT_ENDIF

        
;-------------------------------------------------------------
        RT_IF           S0 <= #10
            LOAD    S5,#5
        RT_ELSE
            LOAD    S6,#6
        RT_ENDIF

        
;----------------------------------------------------------        
        RT_IF           S0 & #10
            LOAD    S5,#5
        RT_ELSE
            LOAD    S6,#6
        RT_ENDIF

        
;------------------------------------------------------------
   ;     RT_IF           S0 !& #10
   ;         LOAD    S5,#5
   ;     RT_ELSE
   ;         LOAD    S6,#6
    ;    RT_ENDIF

        
;-----------------------------------------------------------------