; MDS PicoBlaze IDE - Demonstration code

; Macro instructions, conditional compilation, constants and scratch-pad ram handling.
; See manual for more info

; Press Start simulation and Animate to run the program

; Tell compiler type of procesor (KCPSM2, KCPSM3, KCPSM6 available)
        DEVICE          KCPSM3

; Program initialization
; --------------------
        ORG     0h                      ; Define code segment
        JUMP    start                   ; Jump to code initialization

; Program start
; --------------------
start:  
        LOAD    s0, #00Fh              ; Load content to shifted registers
        LOAD    s1, #01Eh              ;
        LOAD    s3,#01h
        JUMP    main                    ; Execute main program loop

; Main loop
; --------------------
main:
        
; Shifting to the left
        RR     s1
        RR     s3
        RR     s3
        RR     s3
        RR     s3
        RR     s3
        RR     s3
        RR     s3

        ; Shifting to the left
        RL     s3
        RL     s3
        RL     s3
        RL     s3
        RL     s3
        RL     s3
        RL     s3
        RL     s3
call ahoj
ahoj:	call ahoj1
		return
ahoj1:  call ahoj2
		return
ahoj2:  return

; Program end
; --------------------
        END

; Compiler is case insensitive
; <-- Bookmark   (ctrl + shift + M)
; <-- Breakpoint (ctrl + shift + B)