;==============================================================================;
; SWAP OPERATIONS
    device "001_swapInstruction.procdef"
;==============================================================================;
;
; Tested:   SWAP, CB_SWAP, SWAP_BIT, CB_SWAP_BIT
;
;
;
; Begin - DEFINITIONS
;==============================================================================;
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
; Picoblaze copy with added instructions

;ignore this
; PROG_MEM_SIZE           EQU           512
; DATA_MEM_SIZE           EQU           1
; WORD_SIZE               EQU           18
; REG_FILE_SIZE           EQU           1
; NUMBER_OF_BANKS         EQU           1
; PORT_ADR_WIDTH          EQU           1
; PORT_DATA_WIDTH         EQU           1


; START OF PROGRAM
;==============================================================================;

    ORG         0x0

;
; REGISTER FILE
;==============================================================================;
    LOAD        s0,#10
    ;; step
    ;; REG[0] == 10
    ;; PC == 1
    LOAD        s1,#15
    ;; step
    ;; REG[1] == 15
    ;; PC == 2

    ; swap registers
    SWAP        s0,s1
    ;; step
    ;; REG[0] == 15
    ;; REG[1] == 10
    ;; PC == 3
    