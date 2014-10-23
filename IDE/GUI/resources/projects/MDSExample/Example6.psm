; ----------------------------------------------------------------------------------------------------------------------
; MDS for PicoBlaze - Demonstration code VI: User defined macro instructions.
; ----------------------------------------------------------------------------------------------------------------------
;
; This example code demonstrates usage of user defined macros. Examples used here serve only as demonstration, they
; are not meant to have any usage in real life application but they certainly should give you some idea on how to use
; macros in this assembly language.
;
; IMPORTANT NOTE:
;   This is a pretty advanced topic, even entire books have been written about using macros and similar techniques
;   applied in the assembly language; this example code only illustrates how that might look like, and demonstrates that
;   macros really work here but if you are unsure how exactly that works, it might be advisable to avoid using it at
;   all, at least before you get to know all the pitfalls and limitations of this. On the other hand when you master
;   this technique, you will probably get much more efficient in writing assembly code, it's a powerful feature, one
;   just needs to know how exactly to use it properly.
;
; ----------------------------------------------------------------------------------------------------------------------

            ; Start at address 0x000.
            org         0x000

; ======================================================================================================================
; A simple example: macro without parameters.
; ======================================================================================================================

; Define macro named `backup' contaning three loads: S10 <- S0, S11 <- S1, and S12 <- S2.
backup      macro
            ld          S3, S0
            ld          S4, S1
            ld          S5, S2
endm

; Define macro named `restore' to do the exact opposite of what the `backup' does.
restore     macro
            ld          S0, S3
            ld          S1, S4
            ld          S2, S5
endm

            ; Now lets load S0..S2 with some values.
            ld          S0, #1
            ld          S1, #2
            ld          S2, #3
            ; Backup current content of S0..S2.
            backup

            ; And load S0..S2 with another values.
            ld          S0, #10
            ld          S1, #20
            ld          S2, #30
            ; Restore the previous content of S0..S2.
            restore

; ----------------------------------------------------------------------------------------------------------------------

; Wait 100 instruction cycles where you want.
w100        macro
            nop
            ld          S0, #49
            djnz        S0, $-1
endm

            ; Wait 100 instruction cycles here.
            w100

; ======================================================================================================================
; A little more advanced example: macro with parameters.
; ======================================================================================================================

; Double load, loads two different registers with two different values at once.
dload       macro       reg0, reg1, val0, val1
            ld          reg0, #val0
            ld          reg1, #val1
endm

            ; Lets give it a try...
            dload       S0, S1, 0x11, 0x22

; ----------------------------------------------------------------------------------------------------------------------

; Copy content of registers at addresses [source, source+4] to registers at addresses [target, target+4].
copy5       macro       target, source
            ld          target + 0, source + 0
            ld          target + 1, source + 1
            ld          target + 2, source + 2
            ld          target + 3, source + 3
            ld          target + 4, source + 4
endm

; Put the given number of NOPs (No Operation instruction) at the place where this macro is expanded.
nops        macro       number
            repeat      number
                nop
            endr
endm

            ; Put 10 NOPs here.
            nops        10

; Wait for the given number number of instruction cycles, and use the given register as iterator for the delay loop.
wait        macro       register, cycles
            for         register, ( cycles - 1 ) / 4
                nop
            endf
endm

; ======================================================================================================================
; A pretty complex example: macro with local symbol(s), and forced exit combined with conditions and loops.
; ======================================================================================================================

; Fill registers at addresses in range [target, target+length] with the given value.
fill        macro       target, value, length
            local       n
    n       set         0

        #if ( length == 0 )
            warning     "Zero length doesn't make sense here."
        #elseif ( target == S1 )
            warning     "I won't accept S1 as target ... for some reason, I guess."
            exitm
        #endif

        #while n < length
            ld          target + n, #value
    n       set         n + 1
        #endw
endm

; Global label named simply `label'.
label:      jump        $

; Copy contents of registers addresses in range [source, source+number] to registers at [target, target+number].
copy        macro       target, source, number

            local       n
    n       set         0

    ; Local label with same name a global label delared before.
            local       label
            jump        label
    label:

        #while n < number
            ld          target + n, source + n
    n       set         n + 1
        #endw
endm

; Lets have some fun here...
loop:       fill        S0, 0x55, 8    ; S0..S7 <- #0x55
            copy        S8, S0, 8      ; S8..SA <- S0..S7

            fill        S0, 0xAA, 8    ; S0..S7 <- #0xAA
            copy        S8, S0, 8      ; S8..SA <- S0..S7

            jump        loop            ; And again...


; ======================================================================================================================

            ; End of assembly.
            end

; ----------------------------------------------------------------------------------------------------------------------
; Notes:
;   1) Expressions in conditions, for loops, etc. may be enclosed in parenthesis, however, it is not mandatory.
; ----------------------------------------------------------------------------------------------------------------------
