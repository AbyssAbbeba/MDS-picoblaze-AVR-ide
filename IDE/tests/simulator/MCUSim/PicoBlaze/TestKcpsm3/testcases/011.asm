
; Press Start simulation and Animate to run the program

                    ORG       0x000
                    JUMP      Start
; Declaration of some registers
Temp1         REG       s0
Temp2         REG       s1
Temp3         REG       s2

; Begining of main loop
Start:          CALL      pod1
                            ; End of program

pod2:           CALL    POD3
                RET

pod3:           CALL    POD4
                RET

pod4:           CALL    POD5
                RET

pod5:           CALL    POD6
                RET

pod6:           RET