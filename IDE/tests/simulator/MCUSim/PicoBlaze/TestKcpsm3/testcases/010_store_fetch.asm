
device kcpsm3
      ;   FOR     s0,2
hoj:      
	  ADD		0x0,#1
		store	0x0,@5
add S5, #1
		jump	hoj
FOR_3:
      COMPARE 0x0, #0x2
      ADD 0x0, #1
      JUMP    Z, FOR_3_END
                                 ; FOR         s0,1
FOR_4:
     COMPARE 0x0, #0x1
     JUMP    Z, FOR_4_END
        add         s9,#1
                                ;  ENDF
      ADD     0x0, #0x1
      JUMP    FOR_4
FOR_4_END:
         add     sA,#1
                          ;   ENDF
      ADD     0x0, #0x1
     JUMP    FOR_3
       FOR_3_END:
	JUMP FOR_3