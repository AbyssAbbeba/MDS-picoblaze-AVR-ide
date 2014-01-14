

usa     MACRO   x, y
    #ifb        y
    _y   EQU    010
    #else
    _y  EQU     y
    #endif
        LOAD    x, #_y
ENDM
