      ;   FOR     s0,2
      LOAD    0x0, #0x0
FOR_3:
      COMPARE 0x0, #0x2
      JUMP    Z, FOR_3_END
                                 ; FOR         s0,1
      LOAD    0x0, #0x0
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