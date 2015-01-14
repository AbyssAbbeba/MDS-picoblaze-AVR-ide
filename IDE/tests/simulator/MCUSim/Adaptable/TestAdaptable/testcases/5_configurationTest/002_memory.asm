;==============================================================================;
; Settings TEST
    device "002_memory.procdef"
;==============================================================================;
;
; Tested:   Memory, Word size, registers, data memory settings
;
;
;
; Begin - DEFINITIONS
;==============================================================================;
PROG_MEM_SIZE           EQU           2056
DATA_MEM_SIZE           EQU           64
WORD_SIZE               EQU           18
REG_FILE_SIZE           EQU           16
NUMBER_OF_BANKS         EQU           1
PORT_ADR_WIDTH          EQU           8
PORT_DATA_WIDTH         EQU           8


; START OF PROGRAM
;==============================================================================;

    ORG         0x0

;
; REGISTER FILE
;==============================================================================;
    LOAD        0,#10
    LOAD        1,#10
    LOAD        2,#10
    LOAD        3,#10
    LOAD        4,#10
    LOAD        5,#10
    LOAD        6,#10
    LOAD        7,#10
    LOAD        8,#10
    LOAD        9,#10
    LOAD        10,#10
    LOAD        11,#10
    LOAD        12,#10
    LOAD        13,#10
    LOAD        14,#10
    LOAD        15,#10
    ;; step 16
    ;; REG[0] == 10
    ;; REG[1] == 10
    ;; REG[2] == 10
    ;; REG[3] == 10
    ;; REG[4] == 10
    ;; REG[5] == 10
    ;; REG[6] == 10
    ;; REG[7] == 10
    ;; REG[8] == 10
    ;; REG[9] == 10
    ;; REG[10] == 10
    ;; REG[11] == 10
    ;; REG[12] == 10
    ;; REG[13] == 10
    ;; REG[14] == 10
    ;; REG[15] == 10
    ;; PC == 16

;
; BANKS
;==============================================================================;
; none
;
; PORTS
;==============================================================================;
    OUTPUT      0, 0

    ;; step
    ;; PORT[0] == 10

; DATA MEMORY
;==============================================================================;
    LOAD        0, #5
    STORE       0, 0
    ;;  step 2
    ;;  DATA[0] == 5
    ;; PC == 19
    LOAD        1, #20
    STORE       1, DATA_MEM_SIZE - 1
    ;;  step 2
    ;;  DATA[63] == 20
    ;; PC == 21
    JUMP        EndProg
    ;; step
;
; Program memory
;==============================================================================;
    ORG     PROG_MEM_SIZE - 1

EndProg:
    LOAD    0, #55
    ;; step
    ;; reg[0] == 55
    ;; PC == 2055


