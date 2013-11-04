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
        load    s2,#9
        load    s0,#10
        load    s1,#7
        load    s2,#9
        load    s0,#10
        load    s1,#7       
		 load    s2,#9
        load    s0,#10
        load    s1,#7
        jump    calli

calli:

		call	hop1	
		jump	calli

hop1:
		call 	hop2
		return
hop2:
		return
