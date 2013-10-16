; TEST CASE
; testing run time directives, rtif rtfor, rtwhile
device kcpsm3
org     0
B       SET     15

start:
        LOAD    S0, #10
        ;; step 3
        ;; reg[0] == 10
        ;; reg[1] == 0
        
        RT_WHILE   S0 == #10
            LOAD    S0,#B
            B   SET     B-1
        RT_ENDW

        
        
;----------------------------------------------------------------
        
        RT_WHILE   S0 != #10
            LOAD    S0,#B
            B   SET     B-1
        RT_ENDW

;---------------------------------------------------------------

        RT_WHILE   S0 < #10
            LOAD    S0,#B
            B   SET     B-1
        RT_ENDW

;--------------------------------------------------------------------

        RT_WHILE   S0 > #10
            LOAD    S0,#B
            B   SET     B-1
        RT_ENDW

;----------------------------------------------------------------

        RT_WHILE   S0 <= #10
            LOAD    S0,#B
            B   SET     B-1
        RT_ENDW

;--------------------------------------------------------------

        RT_WHILE   S0 >= #10
            LOAD    S0,#B
            B   SET     B-1
        RT_ENDW

;-------------------------------------------------------------

        RT_WHILE   S0 & #10
            LOAD    S0,#B
            B   SET     B-1
        RT_ENDW
        
; -----------------------------------------------------

        RT_WHILE   S0 !& #10
            LOAD    S0,#B
            B   SET     B-1
        RT_ENDW

