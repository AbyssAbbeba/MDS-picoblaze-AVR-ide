ORG 0
JUMP    START


START:
        ADD     S0,#1
        ADDCY   S1,#1
        LOAD    S2,S1
        LOAD    S3,S0
        ADD     S0,S1

        REPT    8
        ADD     S5,#1
        ENDR
       