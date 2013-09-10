
; Press Start simulation and Animate to run the progra
    ORG       0x000
; Begining of main loop
Start:          CALL      pod2
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