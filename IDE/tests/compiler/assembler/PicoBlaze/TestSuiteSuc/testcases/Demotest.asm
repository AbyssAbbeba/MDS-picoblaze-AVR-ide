 ; MDS PicoBlaze IDE - Demonstration code

; Press Start simulation and Animate to run the program

; Tell compiler type of procesor (KCPSM2, KCPSM3, KCPSM6 available)
        DEVICE          KCPSM3

; Constant definitions
; --------------------
LEDs             PORT             0x01            ; VHDL PORT_IDs
Btn              PORT             0x02            ;
; Automaticaly assign registers to symbols
LED_i           AUTOREG                         ; Led working register
BTN_i           AUTOREG                         ; Buttons working register

Temp1           REG       sE
Temp2           REG       sF
Temp3           REG       sD

; Program initialization
; --------------------
                ORG     0h                      ; Define code segment
                JUMP    start                   ; Jump to code initialization

; Macro table
; --------------------
; Waiting loop macro
;-------------------------------------------------------------------------
wait_for        MACRO
                local     waiting

                LOAD      Temp1, #250
waiting:        SUB       Temp1, #1
                JUMP      NZ, waiting

                ENDM
;-----------------------------------------------------------------------

; Program start
; --------------------
start:
        wait_for
        JUMP    main                    ; Execute main program loop

; Main loop
; BTN           000 1 2 3 4 5
; LED_i         000 1 2 3 4 5
; --------------------
main:   INPUT   BTN_i,Btn
        LOAD    LED_i,BTN_i             ; Save buttons status                 ; Button one pressed
        OUTPUT  LED_i,LEDs                          ; JUMP to snake subroutine
        wait_for
        JUMP    main
; Program end
; --------------------
        END

; <-- Bookmark   (ctrl + shift + M)
; <-- Breakpoint (ctrl + shift + B)
