; MDS PicoBlaze IDE - Demonstration code

; Macro instructions, conditional compilation, constants and scratch-pad ram handling.
; See manual for more info

; Press Start simulation and Animate to run the program

; Tell compiler type of procesor (KCPSM2, KCPSM3, KCPSM6 available)
        ;DEVICE          KCPSM6

; Program initialization
; --------------------
        ORG     0h                      ; Define code segment
        JUMP    start                   ; Jump to code initialization

; Program start
; --------------------
start:
		enable interrupt
		load	s1,#10
        load    s2,#9
        load    s0,#10

        while	s1 != #10
			add		s1,#1
		endw

		jump	start

ISR:	
		store   s1, 0
		returni enable



		ORG		3ffh
		jump	ISR
