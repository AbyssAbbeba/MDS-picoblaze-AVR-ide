;==============================================================================;
; Settings TEST
    device "003_picoblazecopy_mem.procdef"
;==============================================================================;
;
; Tested:   Memory, Word size, registers, data memory settings
;
;
;
; Begin - DEFINITIONS
;==============================================================================;
#define PROG_MEM_SIZE           2045
#define DATA_MEM_SIZE           64
#define WORD_SIZE               18
#define REG_FILE_SIZE           16
#define NUMBER_OF_BANKS         1
#define PORT_ADR_WIDTH          8
#define PORT_DATA_WIDTH         8


; START OF PROGRAM
;==============================================================================;

    ORG         0x0

;
; REGISTER FILE
;==============================================================================;
    LOAD        0,#10
    ;; step
    ;; REG[0] == 10
    ;; PC == 1

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
    ;; PC == 3


;
; Program memory
;==============================================================================;
    ORG     PROG_MEM_SIZE - 1       
    LOAD    0, #55
    ;; step
    ;; reg[0] == 55
    ;; PC == 511


