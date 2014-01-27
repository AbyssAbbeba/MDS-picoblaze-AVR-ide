; Compiler test case for Assembler
device kcpsm6

;=========================================================
; routine: mult_soft
;  function: 8-bit unsigned multiplier using
;           shift-and-add algorithm
;  input register:
;     s3: multiplicand
;     s4: multiplier
;  output register:
;     s5: upper byte of product
;     s6: lower byte of product
;  temp register: i
;========================================================





mult_soft:
                        LOAD            s5, #00                 ;clear s5
                        LOAD            s6, #08                 ;initialize loop index
mult_loop:
                        SR0             s4                      ;shift lsb to carry
                        JUMP            nc, shift_prod          ;lsb is 0
                        ADD             s5, s3                  ;lsb is 1
shift_prod:
                        SRA             s5                      ;shift upper byte right,
;carry to MSB, LSB to carry
                        SRA             s6                      ;shift lower byte right,
;lsb of s5 to MSB of s6
                        SUB             s6, #01                 ;dec loop index
                        JUMP            nz, mult_loop           ;repeat until i=0
                        RETURN

; UART Transmit Example
;
start:                  LOAD            s1, #45                 ;ASCII "E".
                        CALL            xmit

;Send character.
                        LOAD            s1, #45                 ;ASCII "E".
                        CALL            xmit

;Send character.
                        LOAD            s1, #33                 ;ASCII "3".
                        CALL            xmit

;Send character.
                        LOAD            s1, #38                 ;ASCII "8".
                        CALL            xmit

;Send character.
                        LOAD            s1, #31                 ;ASCII "1".
                        CALL            xmit

;Send character.
                        LOAD            s1, #30                 ;ASCII "0".
                        CALL            xmit

;Send character.
                        LOAD            s1, #20                 ;ASCII " ".
                        CALL            xmit

;Send character.
                        LOAD            s1, #52                 ;ASCII "R".
                        CALL            xmit

;Send character.
                        LOAD            s1, #75                 ;ASCII "u".
                        CALL            xmit

;Send character.
                        LOAD            s1, #6ch                ;ASCII "l".
                        CALL            xmit

;Send character.
                        LOAD            s1, #65                 ;ASCII "e".
                        CALL            xmit
;Send character.

                        LOAD            s1, #73                 ;ASCII "s".
                        CALL            xmit

;Send character.
                        LOAD            s1, #21                 ;ASCII "!".
                        CALL            xmit

;Send character.
                        LOAD            s1, #21                 ;ASCII "!".
                        CALL            xmit

;Send character.
                        LOAD            s1, #21                 ;ASCII "!".
                        CALL            xmit

;Send character.
                        LOAD            s1, #20                 ;ASCII " ".
                        CALL            xmit

;Send character.
                        JUMP            start

; Routine to transmit data via RS-232
; First check UART status.
xmit:                   INPUT           s0, 00                  ;read uart status.
                        AND             s0, #80                 ;Buffer full mask.
                        JUMP            nz, xmit                ;Poll if buffer is full.
; Buffer is not full, transmit byte in s1
                        OUTPUT          s1, 00                  ;Ship byte.
                        RETURN




END


