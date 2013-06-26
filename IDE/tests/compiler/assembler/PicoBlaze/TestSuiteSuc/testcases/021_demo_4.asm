; MDS Demonstration code

; Macro instructions
; See manual for more info

; You may find some useful advices how to use this
; IDE more efficiently
; [Main menu] -> [Help] -> [Tip of the day]

; Web page: www.moravia-microsystems.com
; --------------------
; Simple program for comunication via UART, with special macros
; You can easily modify this example and use it in your aplication
;
; Press Start simulation and Animate to run the program
; 

; Asign names to registers
        NAMEREG         s0,temp1              ; temporary data register
        NAMEREG         s1,temp2              ; temporary data register
        NAMEREG         s2,temp3              ; temporary data register
        ; OR
        s3          REG         RXdata        ; RX data
        s4          REG         TXdata        ; TX data
; PORT_IDs
        TX_id       PORT        0x01          ;  data register port ID
        RX_id       PORT        0x02          ;  data register port ID
        UART_stat   PORT        0x04          ; status register port ID
; UART Status register:
;  [2] Tx ready
;  [3] new Rx data
;  [4] Rx buffer overflow
        
;  Macro definition
;==============================================================================;
; UART status checking MACRO (IF TX can be done)
UART_ready_wait     MACRO
                    INPUT       Temp1, UART_stat    ; checking UART status
                    TEST        Temp1, 4            ; test bit 2 (is Tx ready?)
                    JUMP        Z, ($ - 2)
                    ENDM
; UART status checking MACRO (NEW RX data?)
UART_new_data_wait  MACRO
                    INPUT       Temp1, UART_stat    ; checking UART status
                    TEST        Temp1, 8            ; test bit 2 (is Tx ready?)
                    JUMP        Z, ($ - 2)
                    ENDM                    
;==============================================================================;
; Macro for sending character via UART
; Parameters: 1
;==============================================================================;
Sendchar            MACRO       char                 ; One parameter
                    UART_ready_wait                  ; Expand UART_ready_wait MACRO here
                    LOAD        TXdata, char
                    OUTPUT      TXdata, TX_id       ; TX PORT_ID, sending char parameter
                    ENDM
;==============================================================================;
; Reads a single character from UART (waits on receive when none is prepared)
; Registers used: Temp1, chreg
;==============================================================================;
GetChar             MACRO
                    UART_new_data_wait              ; Wait for new data
                    INPUT       RX_data, RX_id       ; TX PORT_ID, sending char parameter
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
;   1s @ (10 MHz, Temp1 = 250, Temp2 = 249, Temp3 = 40)
;
; Waiting loops
;==============================================================================;
wait_for_1s         MACRO
wait_1s:            LOAD      Temp1, 250          ; Load Temp1 register
                    LOAD      Temp2, 249          ; Load Temp2 register
                    LOAD      Temp3, 200          ; Load Temp3 register
wait_1s_i:          SUB       Temp1, 1
                    JUMP      NZ, wait_1s_i
                    SUB       Temp2, 1
                    JUMP      NZ, wait_1s_i
                    SUB       Temp3, 1
                    JUMP      NZ, wait_1s_i
                    ENDM

wait_for_100ms      MACRO
wait_100ms:         LOAD      Temp1, 250          ; Load Temp1 register
                    LOAD      Temp2, 249          ; Load Temp2 register
                    LOAD      Temp3, 20           ; Load Temp3 register
wait_100ms_i:       SUB       Temp1, 1
                    JUMP      NZ, wait_100ms_i
                    SUB       Temp2, 1
                    JUMP      NZ, wait_100ms_i
                    SUB       Temp3, 1
                    JUMP      NZ, wait_100ms_i
                    ENDM

;=======================================================================
; END OF MACRO DEFINITIONS ;;
;=======================================================================
; Main program >>>
                   
; Vectors
        ADDRESS 0x000                             ; RESET vector
        JUMP    Start
        ADDRESS 0x3FF                             ; interrupt vector
        JUMP    INTERRUPT

; Start of main program
Start:
                    wait_for_1s             ; wait for initialization of FPGA circuits

; ---------------------------------------- Main loop

main_loop:          CALL      GetChar             ; get (wait for) new character
                    COMPARE      chreg,$20           ; Space character received?
                    JUMP      NZ,main_loop        ; If not space, get another character
                    IN        chreg,Switch        ; If yes, read content of switches
                    CALL      SendChar            ; And send it via UART
                    JUMP      main_loop





SendByte:           LOAD      Temp2, chreg        ; make a backup of chreg
                    SR0       chreg               ; move upper nibble to lower nibble
                    SR0       chreg
                    SR0       chreg
                    SR0       chreg
                    COMP      chreg, 10           ; if not greater than 9, than it is a number
                    JUMP      C, SendBNum1        ; C is set when Temp < 10 (Temp-10)
                    ADD       chreg, $37          ; when letter, add $37; letter conversion
                    JUMP      SendB1
SendBNum1:          ADD       chreg, $30          ; when number, add $30; number conversion
SendB1:             CALL      SendChar            ; Send Character

                    LOAD      chreg, Temp2        ; load the whole byte again
                    AND       chreg, $0F          ; select second character
                    COMP      chreg, 10           ; if not greater than 9, than number
                    JUMP      C, SendBNum2        ; C is set when Temp < 10 (Temp-10 under 0)
                    ADD       chreg, $37          ; when letter, add $37; letter conversion
                    JUMP      SendB2
SendBNum2:          ADD       chreg, $30          ; when number, add $30; number conversion
SendB2:             CALL      SendChar            ; Send character
                    RET



; AND NOW YOU ARE READY !
; We hope this example can help you use this IDE at his maximum potential
; Click on [Main menu] -> [Project] -> [New] and create your own PicoBlaze project ...
