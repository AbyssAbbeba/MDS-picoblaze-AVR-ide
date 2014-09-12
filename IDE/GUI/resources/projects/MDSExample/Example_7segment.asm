; MDS PicoBlaze IDE - Demonstration code

; 7-segment display demonstration code
; See manual for more info

; Web page: www.moravia-microsystems.com
; --------------------
; You can easily modify this example and use it in your aplication.
;
; 1) Press Start simulation
; 2) click on top menu Tools -> 7 Segment simulation
; 3) Press Animate program

; --------------------
; Symbol definition
; --------------------
ZERO    equ     0b11000000
ONE     equ     0b11111001
TWO     equ     0b10100100
THREE   equ     0b10110000
FOUR    equ     0b10011001
FIVE    equ     0b10010010
SIX     equ     0b10000010
SEVEN   equ     0b11111000
EIGHT   equ     0b10000000
NINE    equ     0b10011000

addr    equ     0x00

; --------------------
; Macro definition
; --------------------
display macro   number
        load    s1,#number
        out     s1,addr
endm
; --------------------        
; Program initialization
; --------------------
        org     0h
        jump    loop

; --------------------        
; Main loop
; --------------------
    
loop:
        display ZERO
        display ONE
        display TWO
        display THREE
        display FOUR
        display FIVE
        display SIX
        display SEVEN
        display EIGHT
        display NINE
        jump        loop

; end of program
; you can also try LED panel for checking output values      