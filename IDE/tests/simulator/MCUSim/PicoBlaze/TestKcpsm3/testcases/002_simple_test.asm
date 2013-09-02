

   DEVICE      KCPSM3

    ORG         0x00
    JUMP        Start
    


Start:
        LOAD    0x00,#1
        LOAD    0x00,#55
        JUMP    HOP1

HOP1:   JUMP    HOP2
HOP2:   JUMP    HOP3
HOP3:   END