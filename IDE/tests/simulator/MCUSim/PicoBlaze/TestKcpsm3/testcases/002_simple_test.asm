

   DEVICE      KCPSM3

    ORG         0x00

Start:
        LOAD    0x00,#1
        LOAD    0x00,#55
        JUMP    HOP1

HOP1:   JUMP    HOP2
HOP2:   JUMP    HOP3
HOP3:   JUMP    HOP4
        LOAD    s0,#1
HOP4:   JUMP    HOP5
HOP5:   JUMP    HOP6
        LOAD    s0,#1
        LOAD    s0,#1
        LOAD    s0,#1
HOP6:   JUMP    HOP7
HOP7:   JUMP    HOP8
        LOAD    s0,#1
        LOAD    s0,#12
HOP8:   

END