; TEST CASE
; testing run time directives, rtif rtfor, rtwhile

org     0

start:
        LOAD    S0, #10
        OUT     S0, 2
        IN      S1, 1

        RT_IF   S0 == 10
            LOAD    S5,#5
        RT_ELSE
            LOAD    S6,#6
        RT_ENDIF


        RT_IF           S0 == 10
            LOAD    S5,#5
        RT_ELSEIF       S0 != 10
            LOAD    S6,#6
        RT_ENDIF

        END