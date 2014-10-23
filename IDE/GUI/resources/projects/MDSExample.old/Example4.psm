; ----------------------------------------------------------------------------------------------------------------------
; MDS for PicoBlaze - Demonstration code IV: Conditions.
; ----------------------------------------------------------------------------------------------------------------------
; This example covers these compilation time and run time conditions.
; ----------------------------------------------------------------------------------------------------------------------

            ; Start at address 0x000.
            org         0x000

; ======================================================================================================================
; Compilation time conditions.
; ======================================================================================================================

A           equ         4       ; User defined constant named `A' with value 4.
B           equ         A       ; User defined constant named `B' with value of constant `A' which is 4.

; Assemble the following block only if the user defined constant `A' is greater than 5.
#if ( A > 5 )
            ; Print a message to assembler output.
            message     "A is greater than 5"
            ld          S0, #A
; Assemble the block only if the user defined constants `A' and `B' are equal.
#elseif ( A == B )
            ; Print assembler warning.
            warning     "A is equal to B"
            ld          S0, #B
; Assemble only if `A' and `B' are different.
#elseif ( A != B )
            ; Print assembler warning.
            warning     "A is not equal to B"
            ld          S0, # A - B
; Assemble only if none of the preceding conditions was met, obviously this block of code will never get assembled.
#else
    ; Only if constant `C' is defined which isn't.
    #ifdef C
            ; End assembly with an error message.
            error       "C is defined, and that's not good."
    ; Else...
    #else
            message     "C is not defined so continue normally."
            ld          S0, #1
    #endif
#endif


; ======================================================================================================================
; Run time conditions.
; ======================================================================================================================

            ; Load some registers with some values.
            ld          S0, #0x1c
            ld          S1, #0x1b
            ld          S2, #0x10

    ; Execute the following block only if the current content of registers S0 and S1 are equal.
    if S0 == S1
            inc         S2      ; Increment S2.
    ; Execute the block only if the current content of register S0 is greater than 0x1b (note the immediate addressing).
    elseif S0 > #0x1b
        ; If S1 contains ASCII value of character small `a'.
        if S1 == #'a'
            ld          S2, #'A'; Load S2 with ASCII value of `A'.
        ; If S1 contains small `b'.
        elseif S1 == #'b'
            ld          S2, #'B'; Load S2 with ASCII value of `B'.
        ; And small `c'.
        elseif S1 == #'c'
            dec         S2      ; Decrement S2.
        ; And else...
        else
            clrr        S2      ; Clear S2 (put zero in it).
        ; `endif' is mandatory to close the tree of blocks.
        endif
    ; And else...
    else
            cpl         S2      ; Complement S2 (XOR it with 0xff).
    ; And `endif' is still mandatory.
    endif

; ======================================================================================================================


            ; Infinite loop, and end of assembly.
            jump        $
            end

; ----------------------------------------------------------------------------------------------------------------------
; Notes:
;   1) this assembler is CASE INSENSITIVE (i.e. `load s0, s1' is exactly the same as `LOAD S0, S1' or `Load s0, S1'),
;   2) in most cases the assembler doesn't care about white space (e.g. `if(a==(b-4))' is the same as
;      `if ( a == ( b - 4 ) )'.
; ----------------------------------------------------------------------------------------------------------------------
; The next example shows various kinds of loops.
; ----------------------------------------------------------------------------------------------------------------------
