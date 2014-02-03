; MDS PicoBlaze IDE - Demonstration code

; Macro instructions, conditional compilation, constants and scratch-pad ram handling.
; See manual for more info

; Press Start simulation and Animate to run the program

; Tell compiler type of procesor (KCPSM2, KCPSM3, KCPSM6 available)
        ;DEVICE          KCPSM6_
; Program initialization
; --------------------
        ORG     0h                      ; Define code segment
        JUMP    start                   ; Jump to code initialization

; Program start
; --------------------
start:
		enable interrupt
		load	s8,#0b10000000
		load	S9,#0b00111011
		test	0x9,#0x8

           TEST    0x9, #0x8
          JUMP    Z, IF_5
                      add     s3,#1
                                ; else
          JUMP    IF_5_END
                   IF_5:
                       sub     s3,#1
                         ;   endif
          IF_5_END:



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
