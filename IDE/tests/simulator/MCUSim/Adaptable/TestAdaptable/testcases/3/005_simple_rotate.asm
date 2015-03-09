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
        LOAD    s3, #128
;;STEP
;;PC == 4
;; REG[3] == 128
        JUMP    main                    ; Execute main program loop
; Main loop
; --------------------
;;STEP
;; PC == 5
;; REG[3]==128
main:
;-----------------------------------------------------------------
; FIRST OPERAND = SOURCE
; SECOND OPERAND = NUMBER OF POSITIONS, CAN BE REGISTER
; THIRD OPERAND ( OPTIONAL) = TARGET ( IF SAVE TO THIRD IS SET)
; ----------------------------------------------------------------

        ; Shifting to the right
        ; 4 bit register address, 8bit data address
        ; rotate instructions with rotate positions defined with value of register
        LOAD    s5,#1
        RR     s3,s5
        ;;STEP 2
        ;;PC == 7
        ;;REG[3] == 0b01000000
        RR     s3,s5
        ;;STEP
        ;;REG[3] == 0b00100000
        RR     s3,s5
        ;;STEP
        ;;REG[3] == 0b00010000
        RR     s3,s5
        ;;STEP
        ;;REG[3] == 0b00001000
        RR     s3,s5
        ;;STEP
        ;;REG[3] == 0b00000100
        RR     s3,s5
        ;;STEP
        ;;REG[3] == 0b00000010
        RR     s3,s5
        ;;STEP
        ;;REG[3] == 0b00000001
        ;; PC == 13

        ; Shifting to the left
        RL     s3,s5
        ;;STEP
        ;;REG[3] == 0b00000010
        
        RL     s3,s5
        ;;STEP
        ;;REG[3] == 0b00000100
        
        RL     s3,s5
        ;;STEP
        ;;REG[3] == 0b00001000
        
        RL     s3,s5
        ;;STEP
        ;;REG[3] == 0b00010000
        
        RL     s3,s5
        RL     s3,s5
        RL     s3,s5
        ;;STEP  3
        ;;REG[3] == 0b10000000
        
        ; Shifting to the right
        LOAD    s5,#5
        ;;STEP
        ;;REG[5] == 5
        RR     s3,s5
        ;;STEP
        ;;REG[3] == 0b00000100
        LOAD    s5,#2
        RR     s3,s5
        ;;STEP 2
        ;;REG[3] == 0b00000001
        RL     s3,s5
        ;;STEP
        ;;REG[3] == 0b00000100
        LOAD    s5,#5
        RL      s3,s5
        ;;STEP 2
        ;;REG[3] == 0b10000000

        
        ; Shifting to the right and left with another target register
        LOAD    s5,#1
        ;; STEP
        ;; REG[4] == 0
        ;; REG[5] == 1
        ;; REG[3] == 128

        RRT     s3,s5,s4
        ;;STEP
        ;;REG[4] == 0b01000000
        RRT     s3,s5,s4
        ;;STEP
        ;;REG[4] == 0b00100000

        RLT     s3,s5,s4
        ;;STEP
        ;;REG[4] == 0b00010000
        RLT     s3,s5,s4
        ;;STEP
        ;;REG[4] == 0b00001000
        
        LOAD    s4,#16
        ;; STEP
        ;; REG[4] == 16
                        end

        RRI      s4,#1
        ;; STEP
        ;; REG[4] == 8
                nnbn..gg----rrrr

        ; Shifting to the right and left with another target register
        RRT     s3,#5,s4
        ;;STEP
        ;;REG[4] == 0b00000100
        RRT     s3,#2,s4
        ;;STEP
        ;;REG[4] == 0b00000001
        RLT     s3,#2,s4
        ;;STEP
        ;;REG[4] == 0b00000100
        RLT     s3,#3,s4
        ;;STEP
        ;;REG[4] == 0b10000000
        JUMP    $

    