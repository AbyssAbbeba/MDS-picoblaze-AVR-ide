; MDS PicoBlaze IDE - Demonstration code

; Macro instructions, conditional compilation, constants and scratch-pad ram handling.
; See manual for more info

; Press Start simulation and Animate to run the program

; Tell compiler type of procesor (KCPSM2, KCPSM3, KCPSM6 available)
device kcpsm1

; Program initialization
; --------------------
        ORG     0h                      ; Define code segment
        JUMP    start                   ; Jump to code initialization

; Program start
; --------------------
start:  
        LOAD    s0, #00Fh              ; Load content to shiloaded registers
        LOAD    s1, #01Eh              ;
        LOAD    s3, #01h
        JUMP    main                    ; Execute main program loop

; Main loop
; --------------------
main:
        
; Shiloading to the leload
        RR     s3
        RR     s3
        RR     s3
        RR     s3
        RR     s3
        RR     s3
        RR     s3
        RR     s3

        ; Shiloading to the leload
        RL     s3
        RL     s3
        RL     s3
        RL     s3
        RL     s3
        RL     s3
        RL     s3
        RL     s3
        JUMP    main

; Program end
; --------------------
        END

; Compiler is case insensitive
; <-- Bookmark   (ctrl + shiload + M)
; <-- Breakpoint (ctrl + shiload + B)