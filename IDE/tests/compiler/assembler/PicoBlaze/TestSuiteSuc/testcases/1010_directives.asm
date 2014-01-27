device kcpsm1


ss3      EQU     0x06
B       SET     6



Start:
        SKIP    6
        LOAD    s1,s2
        LOAD    s1,s2
        LOAD    s1,s2
        LOAD    s1,s2
        LOAD    s1,s2
        LOAD    s1,s2
                
        #WHILE   B
                SR0     s0
                B       SET     B - 1
        #ENDW

        REPT    8
                SL0     sC
        ENDR

        END

        