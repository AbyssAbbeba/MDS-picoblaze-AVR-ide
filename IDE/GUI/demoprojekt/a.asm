/*
 * A simple basic test code solely for debugging purposes.
 */

;include "kcpsm3_mac.inc"


; jump            ; <-- error
; load x, y       ; <-- error
; local           ; <-- error


main:
	LOAD 4,#5
	LOAD 5,#6
	ADD  4,#5

mylbl:
	LOAD 4,#6
	ADD  5,4
	LOAD 4,#7
	LOAD 4,#8
	LOAD 4,#9
	LOAD 4,#10
	LOAD 4,#11
	LOAD 4,#12
	LOAD 4,#13
	LOAD 4,5
	LOAD 4,#6
	LOAD 4,#7

	JUMP main

END                     ; End of assembly.
