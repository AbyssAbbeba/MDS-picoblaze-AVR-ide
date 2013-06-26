; MCU 8051 IDE - Demonstration code
; Macro instructions, conditional compilation and constants
; See manual for more info
; Press Start simulation and Animate to run the program

; Constant definitions
; --------------------
counter         idata   00Fh    ; Counter of Px shifts
x               set     100     ; Some variable
inc_dec         equ     100 / X ; Flag: Increment/Decrement counter

                cseg at 1FFh    ; Code segment starts at 0x1FF
something:      db      4d      ; Reserve 4 bytes in this segment

; Macro instructions
; --------------------

;; Shift the given registers
shift   macro   reg0, reg1

        ; Increment / Decrement counter
        mov     A, counter
        if      inc_dec <> 0
                inc     A
        else
                dec     A
        endif

        mov     counter, A


        ; Shift
        mov     reg1, reg0
        mov     reg0, reg1
        setb    C
        mov     A, reg0
        rl      A
        mov     reg0, A
endm

; Program initialization
; --------------------
        org     0h
        sjmp    start

; Program start
; --------------------
start:  mov     P1, #00Fh
        mov     P3, #01Eh
        sjmp    main

; Main loop
; --------------------
main:   shift   P1, P3
        sjmp    main

; Program end
; --------------------
        end
; <-- Bookmark   (ctrl + shift + M)
; <-- Breakpoint (ctrl + shift + B)