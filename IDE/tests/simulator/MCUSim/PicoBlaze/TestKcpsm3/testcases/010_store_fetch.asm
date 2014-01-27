		device	kcpsm3
;-------------------------
numero	equ		0b01010101
;-------------------------
roll	macro	num

		load s6, #num
		rept	7
			rl  	s6
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

        load            s5,#5h
        load            s6,#2h
		roll			numero

		COMPARE     s0,#20
		jump	main

podp:   call	podp2
		return
podp2:	call	podp3
		return
podp3:	return

int:	jump	$

		org		0x3ff
		jump	int
