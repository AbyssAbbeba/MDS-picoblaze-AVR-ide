; tEST SCRIPT FOR SUBROUTINES
device kcpsm1
org 0x00
;; PC == 0

;;STEP
START:  CALL    _1    
;;PC == 2
;;STACK[0] == 1
        JUMP    START
;;STEP
;;PC==5
;;STACK[1]==3
_1:     CALL    _2
        LOAD    S0,#1
        RET
;;STEP
;;PC==8
;;STACK[2]==6
_2:     CALL    _3
        LOAD    S0,#1
        RET
;;STEP
;;PC==11
;;STACK[3]==9
_3:     call    _4
        LOAD    S0,#1
        RET

_4:     load    s7,#3