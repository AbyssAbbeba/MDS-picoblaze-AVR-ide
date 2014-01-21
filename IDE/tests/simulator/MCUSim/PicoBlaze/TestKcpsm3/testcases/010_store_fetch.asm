		device	kcpsm3

		ena

main:	add		S0, #2
		add		S1, #1
		store	S0, @S1
		jump	main

int:	jump	$

		org		0x3ff
		jump	int
