		device	kcpsm3

		ena

main:	add		S0, #2
		add		S1, #1
		store	S0, @S1
		out		S0, @S1
		call	podp
		jump	main

podp:   call	podp2
		return
podp2:	call	podp3
		return
podp3:	return

int:	jump	$

		org		0x3ff
		jump	int
