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


start:  	LOAD sA, #0b01010100
TEST sA, #0b01010100


load	s0, #1
			load	s1,#1
       ;  if   s0  &  1
             TEST    0x0, 0x1
              JUMP    Z, IF_0
                     add     s3,#1
                            ;   else
              JUMP    IF_0_END
                   IF_0:
                    sub     s3,#1
                         ;    endif
                    IF_0_END:


			load s1,3
			jump            $