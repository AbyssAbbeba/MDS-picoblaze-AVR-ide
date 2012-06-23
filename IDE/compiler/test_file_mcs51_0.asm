
xxx	MACRO	x, y, z
lbl0:	add	A, #2 + 3		; xxx
	aCaLl	15 * 4 - '@' - '\0' - '\n'

	yyy	macro	x, y, z
	lbl1:	add	A, #(2 + 3)	; xxx
		ACALL	0x10 + 15 * 4 - 010 / 0b010

	ENDM
	mov	A, 15 * 4
	mov	15 * 4, A

	mul	AB
	nop
add x, y, z	; <-- SYNTAX ERROR

	jmp	lbl

endm

; endm
; macro
; if
; else


my_label:
if 0 - 4
  mov  A, A + 2
  MOV  a - 2, A	; xxx
  Mov  A, a
elseif(4 AND 0)
  moV  A, #A + 2
  mOv  A - 2, #A
  MoV  A, #A:
else
  .DB  2, a, '\a', 'a', 'A', "a", 'aaa', "a\a\tA\x10\177\u1234"	; <-- ERROR (\Uabcd0123 is nonsense)
endif
  xxx 1h, 0x2, '3' 2

  end x

  nonsense bla_bla bla	; <-- This is ok, it's after the `END' directive
