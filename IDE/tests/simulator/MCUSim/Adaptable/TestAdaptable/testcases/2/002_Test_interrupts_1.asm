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
        INTERRUPT ENABLE
        ;;      step
        CHANGEFLAGS
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