; Program initialization
; --------------------
        ORG     0h                      ; Define code segment
        JUMP    start                   ; Jump to code initialization

; Program start
; --------------------
                ;; step
                ;; reg[0] == 10

; All these cycles has been manualy tested in MDS
                
start:          load    s0, #10
                    RT_WHILE   S0 == #10
                        SUB    S0,#1
                    RT_ENDW
                        load s1,3
                        jump            $

                        ; good
;----------------------------------------------------------------
        
        RT_WHILE   S0 != #10
            SUB    S0,#B
            B   SET     B-1
        RT_ENDW

        ; good
;---------------------------------------------------------------

        RT_WHILE   S0 < #10
            LOAD    S0,#B
            B   SET     B-1
        RT_ENDW

        ; good

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

