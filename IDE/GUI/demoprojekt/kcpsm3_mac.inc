
warning "This is only a simple basic test code solely for debugging purposes."

usa     MACRO   abc, xyz
        LOAD    abc, 010
        IFDEF xyz
            STORE   xyz, #abc
        ELSEIFB xyz
            STORE   abc, #abc
        ENDIF

    my_local    EQU     123h
    LOCAL       my_local
    LOCAL       my_local2
    my_local2   EQU     456h
ENDM
