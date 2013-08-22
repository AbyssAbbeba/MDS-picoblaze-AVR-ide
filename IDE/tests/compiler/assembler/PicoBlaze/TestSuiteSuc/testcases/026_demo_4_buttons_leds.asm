; MDS PicoBlaze IDE - Demonstration code

; Simple code, for interfacing buttons and LEDs.
; VHDL code is included
; Button 1 pressed = Snake animation
; Button 2 pressed = Blink for ten times

; Press Start simulation and Animate to run the program


; Tell compiler type of procesor (KCPSM2, KCPSM3, KCPSM6 available)
        DEVICE          KCPSM3

; Constant definitions
; --------------------
; PORT IDs definitions

LEDs            PORT            0x01            ; VHDL PORT_IDs
Btn             PORT            0x02            ;
; Automaticaly assign registers to symbols
LED_i           AUTOREG                         ; Led working register
BTN_i           AUTOREG                         ; Buttons working register
N               SET             10              ; Number of LED blinks

OFF             AUTOREG                         ; Off register
ON              AUTOREG                         ; On register
SNAKE           AUTOREG                         ; Snake register
; Declaration of registers, used in waiting loop
Temp1           AUTOREG
Temp2           AUTOREG
Temp3           AUTOREG


; Macro table
; --------------------
; Waiting loop macro

wait_for_100ms      MACRO

                    LOCAL     wait_100ms
                    LOCAL     wait_100ms_i
                    
wait_100ms:         LOAD      Temp1, #250          ; Load Temp1 register
                    LOAD      Temp2, #249          ; Load Temp2 register
                    LOAD      Temp3, #20           ; Load Temp3 register
wait_100ms_i:       SUB       Temp1, #1
                    JUMP      NZ, wait_100ms_i
                    SUB       Temp2, #1
                    JUMP      NZ, wait_100ms_i
                    SUB       Temp3, #1
                    JUMP      NZ, wait_100ms_i
                    ENDM
                    
; Program initialization
; --------------------
        ORG     0h                      ; Define code segment
        JUMP    start                   ; Jump to code initialization

                    
; Subroutines
;-------------------------
Snake_i:        OUTPUT          Snake,LEDs        ; Turn LEDs OFF
                                                  ; REPT directive is very good for repeating blocks of code
                REPT            7                 ; Moving right
                RR              Snake              
                ENDR
                
                REPT            7                 ; Moving left
                RL              Snake
                ENDR
                
; Blinks with LEDs for N times.                 
;--------------------------
Blink_i:        WHILE   N
                N       SET     N - 1           ; Blinks defined by N

                OUTPUT          ON,LEDs         ; Turn LEDs ON          
                wait_for_100ms
                OUTPUT          OFF,LEDs        ; Turn LEDs OFF
                wait_for_100ms
                
                ENDW
                RETURN
                

; Program start
; --------------------
start:  
        wait_for_100ms                  ; FPGA circuits startup time
        LOAD    ON,  #0xFF              ; Declare ON, snake and OFF
        LOAD    OFF, #0x00              ;
        LOAD    Snake, #0x01            ;
        JUMP    main                    ; Execute main program loop

; Main loop
; --------------------
main:   INPUT   BTN_i,Btn               ; Save buttons status
        TEST    BTN_i,1                 ; Button one pressed
        JUMP    C,Snake_i               ; JUMP to snake subroutine
        TEST    BTN_i,2                 ; Button two pressed
        JUMP    C,Blink_i               ; JUMP to Blink subroutine
        JUMP    main
; Program end
; --------------------
        END

; <-- Bookmark   (ctrl + shift + M)
; <-- Breakpoint (ctrl + shift + B)