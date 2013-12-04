.device atmega8
.cseg

main:
	ldi	R16, 0x55
	ldi	R17, 0xE1

	out	0x39, R16	; 0x39 == TIMSK

	add	R17, R16
	rjmp	main
