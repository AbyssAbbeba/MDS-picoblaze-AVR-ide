		device	kcpsm3
;-------------------------
roll	macro	num

		rept	7
			num
		endr
endm
;------------------------
		ena

main:	add		S0, #2
		add		S1, #1
		store	S0, @S1
		out		S0, @S1
		call	podp

        IF          S0 != #10
            LOAD    S5,#5
        ELSE
            LOAD    S6,#6
        ENDIF

        regbank         a
        load            s5,#5h
        regbank         b
        load            s6,#2h

		roll			s6

		jump	main

podp:   call	podp2
		return
podp2:	call	podp3
		return
podp3:	return

int:	jump	$

		org		0x3ff
		jump	int
