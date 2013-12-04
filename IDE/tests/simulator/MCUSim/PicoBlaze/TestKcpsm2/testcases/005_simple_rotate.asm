device kcpsm2
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

        ; Shifting to the right
        ;;STEP
        ;;PC == 6
        ;;REG[3] == 0b10000000
        RR     s3
        ;;STEP
        ;;REG[3] == 0b01000000
        RR     s3
        ;;STEP
        ;;REG[3] == 0b00100000
        RR     s3
        ;;STEP
        ;;REG[3] == 0b00010000
        RR     s3
        ;;STEP
        ;;REG[3] == 0b00001000
        RR     s3
        ;;STEP
        ;;REG[3] == 0b00000100
        RR     s3
        ;;STEP
        ;;REG[3] == 0b00000010
        RR     s3
        ;;STEP
        ;;REG[3] == 0b00000001
        ;; PC == 13
        RR     s3

        ; Shifting to the left
        ;;STEP
        ;;REG[3] == 0b00000010
        RL     s3
        ;;STEP
        ;;REG[3] == 0b00000100
        RL     s3
        ;;STEP
        ;;REG[3] == 0b00001000
        RL     s3
        ;;STEP
        ;;REG[3] == 0b00010000
        RL     s3
        RL     s3
        RL     s3
        RL     s3
        ;;STEP  3
        ;;REG[3] == 0b10000000
        RL     s3
        JUMP    main

    