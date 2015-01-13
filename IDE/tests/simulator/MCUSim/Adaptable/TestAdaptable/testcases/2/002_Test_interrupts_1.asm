;==============================================================================;
; Settings TEST
    device "002_Test_settings_1.procdef"
;==============================================================================;
;
; Tested:   Memory, Word size, registers, data memory settings
;
;
;
; Begin
;==============================================================================;
; TEST INTERRUPT VECTOR, INTERRUPT FLAGS, INTERUPT ENABLE, AUTOMATIC REENABLE AND
; AUTOMATIC REDISABLE
;==============================================================================;
i_vector        EQU     0x3ff
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
;==============================================================================;

        org 0x000
        JUMP start
        org i_vector
        JUMP isr

;==============================================================================;
start:
        ;;      step
        ;;      pc == 1
        ;;      flag[ie] == false
        ;;      interrupt
        LOAD    0,#2
        ;;      step
        ;;      pc == 2
        ENA
        ;;      step
      ;  CHANGEFLAGS
        ;;      step
        ;;      pc == 3
        ;;      flag[ie] == true
        ;;      flag[c] == true
        ;;      flag[z] == true
        ;;      flag[c] == true
        ;;      flag[c] == true
        ;;      interrupt
        LOAD    0,#5
        ;;      step
        ;;      pc == 4
        ;;      flag[ie] == true
        ;; step
        ;; reg[0] == 249


isr:
        load    0,#3
        returnIE
;==============================================================================;        