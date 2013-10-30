; MDS PicoBlaze IDE - Demonstration code

; Macro instructions, conditional compilation, constants and scratch-pad ram handling.
; See manual for more info

; Press Start simulation and Animate to run the program

; Tell compiler type of procesor (KCPSM2, KCPSM3, KCPSM6 available)
        ;DEVICE          KCPSM6

; Program initialization
; --------------------
        ORG     0h                      ; Define code segment
        JUMP    start                   ; Jump to code initialization

; Program start
; --------------------
start:
                    load s1,#10
                    load s0,#1

                    if   s0  >= #50
                        add     s3,#1
                        else
                        sub     s3,#1
                    endif
