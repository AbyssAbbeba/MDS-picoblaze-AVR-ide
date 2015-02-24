;==============================================================================;
; SWAP OPERATIONS
    device "001_StackTest.procdef"
;==============================================================================;
;
; Tested: STACK,   designated, datamemory, register file stack
;
;
; Begin - DEFINITIONS
;==============================================================================;
S0      REG     0x0
S1      REG     0x1
S2      REG     0x2
S3      REG     0x3
S0      REG     0x4
S1      REG     0x5
S2      REG     0x6
S3      REG     0x7

; Picoblaze copy with added instructions

;ignore this
; PROG_MEM_SIZE           EQU           512
; DATA_MEM_SIZE           EQU           1
; WORD_SIZE               EQU           18
; REG_FILE_SIZE           EQU           1
; NUMBER_OF_BANKS         EQU           1
; PORT_ADR_WIDTH          EQU           1
; PORT_DATA_WIDTH         EQU           1

; used for regbank instructions
A define 0
B define 1

; START OF PROGRAM
;==============================================================================;

; Tested: ; Tested: STACK,   designated, datamemory, register file stack

;
; 
;==============================================================================;
    ORG         0x0
    ;; PC == 0
    LOAD        s0,#0x01
    ;; step
    ;; REG[0] == 1
    ;; PC == 1
    LOAD        s1,#0x02
    ;; step
    ;; REG[1] == 2
    ;; PC == 2
    CALL        FirstSub
    ;; step
    ;; stack[0] == 0x03
    ;; PC == 3
    LOAD        s1, #0

    ORG         0x100
    
FirstSub:
    LOAD        s1, #0
    RETURN
    ;; step 
    ;; stack[0] == 0x00
    ;; PC == 0x101

    ;; step 
    ;; stack[0] == 0x00
    ;; PC == 0x101
  