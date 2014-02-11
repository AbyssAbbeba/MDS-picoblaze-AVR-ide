; MDS PicoBlaze IDE - Demonstration code
; See manual for more info

; Simple example of waiting loop
device kcpsm1cpld

TEMP1   autoreg
TEMP2   autoreg
TEMP3   autoreg

; Press Start simulation and Animate to run the program

                    ORG       0x000
                    JUMP      Start
; Begining of main loop
Start:              CALL      wait_1s
                    CALL      wait_100ms
                    JUMP      $                   ; Infinite loop, $ is translated as number of curent line
                   


; Subroutines-----------------------------------------------------------------
; wait_time = (4 + (((2 * Temp1) + 2) * Temp2 + 2) * Temp3) * 2 * clk_period
;   1s  (10 MHz, Temp1 = 250, Temp2 = 249, Temp3 = 40)

wait_1s:            LOAD      Temp1, #250          ; Load Temp1 register
                    LOAD      Temp2, #249          ; Load Temp2 register
                    LOAD      Temp3, #200          ; Load Temp3 register
wait_1s_i:          SUB       Temp1, 1
                    JUMP      NZ, wait_1s_i
                    SUB       Temp2, 1
                    JUMP      NZ, wait_1s_i
                    SUB       Temp3, 1
                    JUMP      NZ, wait_1s_i
                    RETURN
;-----------------------------------------------------------------------------
wait_100ms:         LOAD      Temp1, #250          ; Load Temp1 register
                    LOAD      Temp2, #249          ; Load Temp2 register
                    LOAD      Temp3, #20           ; Load Temp3 register
wait_100ms_i:       SUB       Temp1, 1
                    JUMP      NZ, wait_100ms_i
                    SUB       Temp2, 1
                    JUMP      NZ, wait_100ms_i
                    SUB       Temp3, 1
                    JUMP      NZ, wait_100ms_i
                    RETURN
; <-- Bookmark   (ctrl + shiload + M)
; <-- Breakpoint (ctrl + shiload + B)
                    END                            ; End of program
; -----------------------------------------
; -----------------------------------------
