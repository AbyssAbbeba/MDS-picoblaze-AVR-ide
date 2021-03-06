; MDS PicoBlaze IDE - Demonstration code

; Macro instructions
; See manual for more info

; You may find some useful advices how to use this
; IDE more efficiently
; [Main menu] -> [Help] -> [Tip of the day]

; Web page: www.moravia-microsystems.com
; --------------------
; Simple program for comunication with UART.
; You can easily modify this example and use it in your aplication.
; VHDL code of UART is included
;
; Press Start simulation and Animate to run the program
;
; Tell compiler type of procesor (KCPSM2, KCPSM3, KCPSM6 available)
device kcpsm1
        
; Asign names to registers
        NAMEREG         s0,temp1              ; temporary EQU register
        NAMEREG         s1,temp2              ; temporary EQU register
        NAMEREG         s2,temp3              ; temporary EQU register
        ; OR
        RX_EQU        AUTOREG   AT 3          ; RX EQU
        TXEQU        AUTOREG               ; TX EQU
        LED_reg       AUTOREG           ; Leds EQU register

; PORT_IDs
        TX_id       PORT        0x01          ;  EQU register port ID
        RX_id       PORT        0x02          ;  EQU register port ID
        UART_stat   PORT        0x04          ; status register port ID
        LED_id      PORT        0x08          ; Led register
; UART Status register:
;  [2] Tx ready
;  [3] new Rx EQU
;  [4] Rx buffer overflow
        
;  Macro definition
;==============================================================================;
; UART status checking MACRO (IF TX can be done)
UART_ready_wait     MACRO
                    INPUT       Temp1, UART_stat    ; checking UART status
                    load        Temp1, 4            ; load bit 2 (is Tx ready?)
                    JUMP        Z, ($ - 2)
                    ENDM
; UART status checking MACRO (NEW RX EQU?)
UART_new_EQU_wait  MACRO
                    INPUT       Temp1, UART_stat    ; checking UART status
                    load        Temp1, 8            ; load bit 2 (is Tx ready?)
                    JUMP        Z, ($ - 2)
                    ENDM                    
;==============================================================================;
; Macro for sending character via UART
; Parameters: 1
;==============================================================================;
Sendchar            MACRO       char                 ; One parameter
                    UART_ready_wait                  ; Expand UART_ready_wait MACRO here
                    LOAD        TXEQU, char
                    OUTPUT      TXEQU, TX_id       ; TX PORT_ID, sending char parameter
                    ENDM
;==============================================================================;
; Reads a single character from UART (waits on receive when none is prepared)
; Registers used: Temp1, chreg
;==============================================================================;
GetChar             MACRO
                    UART_new_EQU_wait              ; Wait for new EQU
                    INPUT       RX_EQU, RX_id       ; TX PORT_ID, sending char parameter
                    ENDM
;==============================================================================;
; Send 0D and 0A character pair via UART
; Macros used: SendChar
;==============================================================================;
SendCRLF            MACRO       
                    SendChar  0x0D          ; CR character
                    SendChar  0x0A          ; CR character
                    ENDM                           ; Return from procedure                   
;==============================================================================;

;==============================================================================;
; Procedures for waiting for specified time
; Duration is set in registers Temp1, Temp2 and Temp3
;
;   clock           instruction execution time
;   10.00 MHz              200.00 ns
;   19.44 MHz              102.88 ns
;   38.88 MHz               51.44 ns
;   50.00 MHz               40.00 ns
;  100.00 MHz               20.00 ns
;
; wait_time = (4 + (((2 * Temp1) + 2) * Temp2 + 2) * Temp3) * 2 * clk_period
;   1s  (10 MHz, Temp1 = 250, Temp2 = 249, Temp3 = 40)
;
; Waiting loops
;==============================================================================;
wait_for_1s         MACRO

                    LOCAL     wait_1s
                    LOCAL     wait_1s_i

wait_1s:            LOAD      Temp1, #250          ; Load Temp1 register
                    LOAD      Temp2, #249          ; Load Temp2 register
                    LOAD      Temp3, #200          ; Load Temp3 register
wait_1s_i:          SUB       Temp1, #1
                    JUMP      NZ, wait_1s_i
                    SUB       Temp2, #1
                    JUMP      NZ, wait_1s_i
                    SUB       Temp3, #1
                    JUMP      NZ, wait_1s_i
                    ENDM
;--------------------------------------------------------------------------
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
;==============================================================================;
; UART RX register:
;  [1] Rotate leds 8x
;  [2] Send "Hello world" via UART
;-------------------------------------------------------------------------------------
                    ADDRESS 0x000                             ; RESET vector
                    JUMP    Start



;-------------------------------------------------------------------------
; Interrupt routine
INTERRUPT:          SendChar  'I'
                    SendChar  'N'
                    SendChar  'T'
                    SendChar  'E'
                    SendChar  'R'
                    SendChar  'R'
                    SendChar  'U'
                    SendChar  'P'
                    SendChar  'T'
                    SendCRLF
                    RETURNI ENABLE
; Start of main program
Start:
                    wait_for_1s             ; wait for initialization of FPGA circuits
                    ENABLE INTERRUPT
; ---------------------------------------- Main loop

main_loop:          GetChar                       ; Receive via UART, get status of switches for example
                    JUMP        main_loop


;=======================================================================
; END OF MACRO DEFINITIONS ;;
;=======================================================================
; Main program >>>

; AND NOW YOU ARE READY !
; We hope this example can help you use this IDE at his maximum potential
; Click on [Main menu] -> [Project] -> [New] and create your own PicoBlaze project ...
