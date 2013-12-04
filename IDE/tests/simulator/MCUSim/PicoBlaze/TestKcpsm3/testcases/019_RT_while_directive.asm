; Program initialization
device kcpsm3
; --------------------
        ORG     0h                      ; Define code segment
        JUMP    start                   ; Jump to code initialization

; Program start
; --------------------
B      set      0
                ;; step
                

; All these cycles has been manualy tested in MDS
                
start:          load    s0, #10
                      WHILE   S0 == #10
                        SUB    S0,#1
                      ENDW
                        load s1,3
                        jump            $

                        ; good
;----------------------------------------------------------------
        
          WHILE   S0 != #10
            SUB    S0,#B
            B   SET     B-1
          ENDW

        ; good
;---------------------------------------------------------------

          WHILE   S0 < #10
            LOAD    S0,#B
            B   SET     B-1
          ENDW

        ; good

;--------------------------------------------------------------------

          WHILE   S0 > #10
            LOAD    S0,#B
            B   SET     B-1
          ENDW

;----------------------------------------------------------------

          WHILE   S0 <= #10
            LOAD    S0,#B
            B   SET     B-1
          ENDW

;--------------------------------------------------------------

          WHILE   S0 >= #10
            LOAD    S0,#B
            B   SET     B-1
          ENDW

;-------------------------------------------------------------

          WHILE   S0 & #10
            LOAD    S0,#B
            B   SET     B-1
          ENDW
        
; -----------------------------------------------------

          WHILE   S0 !& #10
            LOAD    S0,#B
            B   SET     B-1
          ENDW

