; tEST SCRIPT FOR SUBROUTINES
device "006_calls.procdef"

S0      REG     0x0
S1      REG     0x1
S2      REG     0x2
S3      REG     0x3
S4      REG     0x4
S5      REG     0x5
S6      REG     0x6
S7      REG     0x7
S8      REG     0x8
S9      REG     0x9
SA      REG     0xA
SB      REG     0xB
SC      REG     0xC
SD      REG     0xD
SE      REG     0xE
SF      REG     0xF

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
        RETURN
;;STEP
;;PC==8
;;STACK[2]==6
_2:     CALL    _3
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==11
;;STACK[3]==9
_3:     CALL    _4
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==14
;;STACK[4]==12
_4:     CALL    _5
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==17
;;STACK[5]==15
_5:     CALL    _6
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==20
;;STACK[6]==18
_6:     CALL    _7
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==23
;;STACK[7]==21
_7:     CALL    _8
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==26
;;STACK[8]==24
_8:     CALL    _9
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==29
;;STACK[9]==27
_9:     CALL    _10
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==32
;;STACK[10]==30
_10:     CALL    _11
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==35
;;STACK[11]==33
_11:     CALL    _12
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==38
;;STACK[12]==36
_12:     CALL    _13
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==41
;;STACK[13]==39
_13:     CALL    _14
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==44
;;STACK[14]==42
_14:     CALL    _15
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==47
;;STACK[15]==45
_15:     CALL    _16
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==50
;;STACK[16]==48
_16:     CALL    _17
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==53
;;STACK[17]==51
_17:     CALL    _18
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==56
;;STACK[18]==54
_18:     CALL    _19
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==59
;;STACK[19]==57
_19:     CALL    _20
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==62
;;STACK[20]==60
_20:     CALL    _21
        LOAD    S0,#1
        RETURN
;;STEP
;;PC==63
;;STACK[21]==0
        
_21:    LOAD    S0,#1
        RETURN

