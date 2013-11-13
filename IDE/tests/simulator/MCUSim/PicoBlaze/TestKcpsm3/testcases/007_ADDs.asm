device kcpsm3
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

        B       SET     5
        
        #WHILE   B
        B       SET     B-1
        ADD     S0,#1
        #ENDW
       