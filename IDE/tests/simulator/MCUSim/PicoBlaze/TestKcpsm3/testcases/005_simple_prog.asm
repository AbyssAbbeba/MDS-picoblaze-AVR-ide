
; Program initialization
; --------------------
        ORG     0h                      ; Define code segment
        JUMP    start                   ; Jump to code initialization

; Program start
; --------------------
start:
        LOAD    s0, #15              ; Load content to shifted registers
        LOAD    s1, #30              ;
        LOAD    s3, #1
        JUMP    main                    ; Execute main program loop
; Main loop
; --------------------
main:

; Shifting to the right
        RR     s3
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
        JUMP    main

; Program end
; --------------------
        END

; Compiler is case insensitive
; <-- Bookmark   (ctrl + shift + M)
; <-- Breakpoint (ctrl + shift + B)
    
    