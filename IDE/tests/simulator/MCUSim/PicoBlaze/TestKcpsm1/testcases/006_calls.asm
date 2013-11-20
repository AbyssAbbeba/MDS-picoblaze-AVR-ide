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
_3:     CALL    _4
        LOAD    S0,#1
        RET
;;STEP
;;PC==14
;;STACK[4]==12
_4:     CALL    _5
        LOAD    S0,#1
        RET
;;STEP
;;PC==17
;;STACK[5]==15
_5:     CALL    _6
        LOAD    S0,#1
        RET
;;STEP
;;PC==20
;;STACK[6]==18
_6:     CALL    _7
        LOAD    S0,#1
        RET
;;STEP
;;PC==23
;;STACK[7]==21
_7:     CALL    _8
        LOAD    S0,#1
        RET
;;STEP
;;PC==26
;;STACK[8]==24
_8:     CALL    _9
        LOAD    S0,#1
        RET
;;STEP
;;PC==29
;;STACK[9]==27
_9:     CALL    _10
        LOAD    S0,#1
        RET
;;STEP
;;PC==32
;;STACK[10]==30
_10:     CALL    _11
        LOAD    S0,#1
        RET
;;STEP
;;PC==35
;;STACK[11]==33
_11:     CALL    _12
        LOAD    S0,#1
        RET
;;STEP
;;PC==38
;;STACK[12]==36
_12:     CALL    _13
        LOAD    S0,#1
        RET
;;STEP
;;PC==41
;;STACK[13]==39
_13:     CALL    _14
        LOAD    S0,#1
        RET
;;STEP
_14:    
        LOAD    S0,#1
        RET
