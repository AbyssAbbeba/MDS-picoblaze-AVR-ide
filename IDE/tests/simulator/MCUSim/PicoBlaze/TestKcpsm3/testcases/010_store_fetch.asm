		device	kcpsm3

main:	add		S0, #2
		add		S1, #1
		store	S0, @S1
		jump	main
__
		end
