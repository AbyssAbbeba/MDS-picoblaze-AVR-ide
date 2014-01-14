        device  kcpsm3

/*
 * A simple basic test code solely for debugging purposes.
 */

_S3     REG     1
abcd    DEFINE  5 / {0}
ab_cd   DEFINE  abcd({0}) + 1
abcdef  DEFINE  4 % ab_cd({0})
abcde   DEFINE  {0} * 2 + abcdef({1})
u       EQU     1                       ; <-- Set this to `0' to produce a compilation error,
a       EQU     5 + 0 * abcde(0x7, 8)
ab      DEFINE  1 + abcde('a', {0})

_X_     define  2 + {0}
XXX     define  1 * {0} + {1}/{2}+5
_Y_     define  1 + {0} + 2
o0o     EQU     _X_(xXx(1,2,_Y_(1, 1),8)) + 2
        LOAD    0x5, #o0o

_a      MACRO   a, b, c
        ADD     S1, #a
        _b      b, c
ENDM

_b      MACRO   b, c
        ADD     S2, #b
        _c      c
ENDM

_c      MACRO   c
        ADD     _S3, #c
ENDM

xyz:    _a      1, 2, ab(u)

        LOAD    S0, #a
        JUMP    $

REPT            2
    IF A == 2
        LD      A, A - 100
    ELSEIF A > #4
        LD      A, #2
    ELSE
        LD      A, #3
    ENDIF

    WHILE A > #2
        ADD     S2, #5
    ENDW

    FOR S0, 10
        ADDCY   S2, #5
    ENDF
ENDR

i       SET     6
j       SET     3

REPT            i
        LOAD    1, #0x5a
    REPT        3
        LOAD    0x5, 1
    ENDR
ENDR

#WHILE          i
        LOAD    1, #0x5a
    #WHILE      j
        LOAD    1, #0x5
    j   SET     j - 1
    #ENDW
i       SET     i - 1
j       SET     3
#ENDW

lbl_X:  JUMP lbl_Y
lbl_Y:  CALL lbl_X


cccp    EQU     ( 2h + 4d )     ; Define a compile-time constant.
eu      EQU     3h              ; Define another compile-time constant.

include "044_random_test.0.inc"
b:      ADD     cccp, 1

        DB      4, 5, "456\0\n", 'x'


abc     MACRO   pa, pb, pc
        ADD     cccp, 1
ENDM

; jump            ; <-- error
; load x, y       ; <-- error
; local           ; <-- error
nolist
        REPT    3
            ADD     cccp, 1
        ENDR
        SKIP    1
list
#if ( 2 + ( 1 - 3 ) )
        SR0     eu
#elseif ( 3 * 4 )
        SR0     cccp
        #ifn ( 3 - 2 )
                ADD     eu, cccp
        #else
                org     5
                SUB     eu, #cccp
        #endif
#else
        ENABLE INTERRUPT
        DISABLE INTERRUPT
#endif

        ORG     0x100

i       SET     6

        REPT    i
            LOAD    cccp, #0x5a
        ENDR

        #WHILE  i
            LOAD    cccp, #0x5a
            i       SET     i - 1
        #ENDW

main:   LOAD    cccp, #0x5a
        usa     eu;, cccp
        JUMP    mylbl
        JUMP    main
mylbl:  JUMP    $

        UNDEF   cccp
#ifdef  cccp
        LOAD    cccp, #0x5a
#else
        LOAD    10, #0x5a
#endif
        RR      12

        DB      0x2060E         ; <-- "SR0 6"


; ==========================================
        END               ; End of assembly.
; ==========================================
