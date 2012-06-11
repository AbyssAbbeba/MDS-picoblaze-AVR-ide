xxx	macro	x, y, z
lbl0:	add	A, #2 + 3		; xxx
	ACALL	15 * 4 - '@' - '\0' - '\n'

	yyy	macro	x, y, z
	lbl1:	add	A, #(2 + 3)	; xxx
		ACALL	0x10 + 15 * 4 - 010 / 0b010

	endm
	mov	A, 15 * 4
	mov	15 * 4, A

	mul	AB
	nop

	jmp	lbl

endm

	DB	2, a, '\a', 'a', 'A', "a", 'aaa', "a\a\tA"
mystr:	DB	"bla \"bla\" \bl\a\\", 7
	DB	"db"

zzz	macro	x, y, z
lbl2:	add	A, #2 + 3		; xxx
	ACALL	15 * yyy

	abc	macro	x, y, z
	lbl3:	add	A, #(2 + 3) * 4 + (10q - 010)	; xxx
		ACALL	15 * 4

	endm
	ACALL	15 * 4

endm

	mov	A, A + 2
	mov	A - 2, A
	mov	A, A
	mov	A, #A + 2
	mov	A - 2, #A
	mov	A, #A

xxx
xxx(3)
xxx ( 3, 'q' )
lbl4:xxx()		; another way to expand a macro
zzz 1, 011b + 2/3, P2.4
zzz(1, 2, 3)
