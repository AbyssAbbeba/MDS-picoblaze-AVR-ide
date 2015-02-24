device "005_simple_rotate.procdef"
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
; Program initialization
; --------------------
        ORG     0h                      ; Define code segment
        JUMP    start                   ; Jump to code initialization
;; PC == 0

;; STEP
;; PC == 1


; Program start
; --------------------
start:
        LOAD    s0, #15              ; Load content to shifted registers
;;STEP
;; PC == 2
;; REG[0] == 15
        LOAD    s1, #30
;;STEP
;;PC == 3
;; REG[1]== 30
        LOAD    s3, #1
;;STEP
;;PC == 4
;; REG[3] == 1
        JUMP    main                    ; Execute main program loop
; Main loop
; --------------------
;;STEP
;; PC == 5
;; REG[3]==1
main:
;-----------------------------------------------------------------
; FIRST OPERAND = SOURCE
; SECOND OPERAND = NUMBER OF POSITIONS, CAN BE REGISTER
; THIRD OPERAND ( OPTIONAL) = TARGET ( IF SAVE TO THIRD IS SET)
; ----------------------------------------------------------------

        ; Shifting to the right
        RR     s3,#1
        ;;STEP
        ;;PC == 6
        ;;REG[3] == 0b10000000
        RR     s3,#1
        ;;STEP
        ;;REG[3] == 0b01000000
        RR     s3,#1
        ;;STEP
        ;;REG[3] == 0b00100000
        RR     s3,#1
        ;;STEP
        ;;REG[3] == 0b00010000
        RR     s3,#1
        ;;STEP
        ;;REG[3] == 0b00001000
        RR     s3,#1
        ;;STEP
        ;;REG[3] == 0b00000100
        RR     s3,#1
        ;;STEP
        ;;REG[3] == 0b00000010
        RR     s3,#1
        ;;STEP
        ;;REG[3] == 0b00000001
        ;; PC == 13

        ; Shifting to the left
        RL     s3,#1
        ;;STEP
        ;;REG[3] == 0b00000010
        
        RL     s3,#1
        ;;STEP
        ;;REG[3] == 0b00000100
        
        RL     s3,#1
        ;;STEP
        ;;REG[3] == 0b00001000
        
        RL     s3,#1
        ;;STEP
        ;;REG[3] == 0b00010000
        
        RL     s3,#1
        RL     s3,#1
        RL     s3,#1
        ;;STEP  3
        ;;REG[3] == 0b10000000
        RL     s3,#1
        JUMP    $

    