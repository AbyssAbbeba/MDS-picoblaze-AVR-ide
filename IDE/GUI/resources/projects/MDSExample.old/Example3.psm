; ----------------------------------------------------------------------------------------------------------------------
; MDS for PicoBlaze - Demonstration code III: Some time saving features.
; ----------------------------------------------------------------------------------------------------------------------
; This example covers instruction shortcuts and numeral system radix.
; ----------------------------------------------------------------------------------------------------------------------

            ; Start at address 0x000.
            org         0x000


; ======================================================================================================================
; Radix, and several ways how to write a number.
; ======================================================================================================================

            ; Hexadecimal.
            ld          S0, #0xAB       ; prefix notation (`0x...')
            ld          S0, #0ABh       ; suffix notation (`...h')
            ; Decimal.
            ld          S0, #123        ; no prefix - default radix - decimal
            ld          S0, #123d       ; suffix notation (`d')
            ; Octal.
            ld          S0, #076        ; prefix notation (`0...')
            ld          S0, #76q        ; suffix notation (`...q')
            ld          S0, #76o        ; suffix notation (`...o')
            ; Binary.
            ld          S0, #0b1100     ; prefix notation (`0b...')
            ld          S0, #1100b      ; suffix notation (`...b')
            ; ASCII.
            ld          S0, #'A'        ; character capital A
            ld          S0, #'\t'       ; tab character


; ======================================================================================================================
; Instruction shortcuts.
; When you don't want to write long instruction mnemonics like "enable interrupt" and use short ones instead like "ena".
; ======================================================================================================================

            load        S1, S0          ; S1 <- S0 (copy contents of S0 to S1)
            ld          S1, S0          ; Do the same as above, `ld' is shortcut for 'load'.

; The same way you can write `ld' instead of `load', you can also use:
;   * `st'      instead of `store'
;   * `ft'      instead of `fetch'
;   * `rb'      instead of `regbank'
;   * `in'      instead of `input'
;   * `out'     instead of `output'
;   * `outk'    instead of `outpuk'
;   * `cmp'     instead of `compare'
;   * `cmpcy'   instead of `comparecy'
;   * `ena'     instead of `enable interrupt'
;   * `dis'     instead of `disable interrupt'
;   * `ret'     instead of `return'
;   * `ldret'   instead of `load & return'
;   * `retie'   instead of `return enable'
;   * `retid'   instead of `return disable'


; ======================================================================================================================
; Pseudo-instructions.
; ======================================================================================================================

            nop                     ; No operation, the simplest pseudo-instruction.
            cpl         S1          ; Complement (XOR with 0xFF).
            cpl2        S1          ; 2nd arithmetic complement (+1 and XOR 0xFF)
            inc         S1          ; Increment (add 1).
            dec         S1          ; Decrement (subtract 1).
            setr        S1          ; Set register (load with 0xFF).
            clrr        S1          ; Clear register (load with 0x00).
            setb        S1, 2       ; Set bit number 2 in register S1.
            clrb        S1, 2       ; Clear bit number 2 in register S1.
            notb        S1, 2       ; Invert bit number 2 in register S1.
label:      djnz        S1, label   ; Decrement and jump if not zero.
            ijnz        S1, label   ; Increment and jump if not zero.

; ======================================================================================================================


            ; Infinite loop, and end of assembly.
            jump        $
            end

; ----------------------------------------------------------------------------------------------------------------------
; In the next example you will see conditions.
; ----------------------------------------------------------------------------------------------------------------------
