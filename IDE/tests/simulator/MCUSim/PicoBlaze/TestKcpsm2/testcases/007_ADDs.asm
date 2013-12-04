ORG 0
JUMP    START

device kcpsm2
START:
        ADD     S0,#1

        LOAD    S2,S1
        LOAD    S3,S0
        ADD     S0,S1


        B       SET     5
        
        #WHILE   B
        B       SET     B-1
        ADD     S0,#1
        #ENDW
       