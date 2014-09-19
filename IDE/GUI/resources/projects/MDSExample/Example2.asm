; MDS - Demonstration code
; Macro instructions, if-else and constants

; ===================================================
; Constant definitions
; ==================================================
counter         reg     s5      ; Counter of Px shifts
x               set     100     ; Some variable
inc_dec         reg     100 / X ; Flag: Increment/Decrement counter
temp            reg     s6


; Macro instructions
; ==========================================
; Shift the given registers
; ==========================================

shift   macro   reg0, reg1

        ; Increment / Decrement counter
        if      inc_dec <> #0
            inc     counter
        else
            dec     counter
        endif

        ; Shift
        load    temp,reg0
        load    reg0, reg1
        load    reg1, temp
endm

; Program initialization
; --------------------
        org     0h
        jump    start

; Program start
; --------------------
start:  load    s1, #0x0F
        load    s3, #0xF0
        jump    main

; Main loop
; --------------------
main:   shift   s1, s3
        jump    main

; Program end
; --------------------
        end