; MDS PicoBlaze IDE - Demonstration code

; Macro instructions, conditional compilation, constants and scratch-pad ram handling.
; See manual for more info

; Press Start simulation and Animate to run the program

; Tell compiler type of procesor (KCPSM2, KCPSM3, KCPSM6 available)
        DEVICE          KCPSM3

; Constant definitions
; --------------------
counter          SET     00Fh    ; Counter of Px shifts
x                SET     100     ; Some variable
inc_dec          EQU     100 / X ; Flag: Increment/Decrement counter
ram_pointer      SET     00h      ; Assign name to register 3

; Macro instructions
; --------------------
;; Shift the given registers
shift   MACRO    reg0, reg1

        ; Increment / Decrement counter
;        if       inc_dec != 0
;                 inc     counter
;        else
;                 dec     counter
;        endif

        ; Save registers to DATA memory (Scratch-pad ram with range 00h to 3Fh)
;        STORE    s0, ram_pointer + 1
;        STORE    s1, ram_pointer + 1
        
        ; Shift
        LOAD     reg1, reg0
        LOAD     reg0, reg1
ENDM

; Program initialization
; --------------------
        ORG     0h                      ; Define code segment
        JUMP    start                   ; Jump to code initialization

; Program start
; --------------------
start:  
        LOAD     s0, #00Fh              ; Load content to shifted registers
        LOAD     s1, #01Eh              ;
        JUMP    main                    ; Execute main program loop

; Main loop
; --------------------
main:   shift   s0, s1
        JUMP    main

; Program end
; --------------------
        END

; Compiler is case insensitive
; <-- Bookmark   (ctrl + shift + M)
; <-- Breakpoint (ctrl + shift + B)