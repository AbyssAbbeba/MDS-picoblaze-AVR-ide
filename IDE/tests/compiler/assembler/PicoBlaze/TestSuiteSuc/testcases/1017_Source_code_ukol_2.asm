;##############################################################################;
; Description of code
;##############################################################################;
;
; Tutorial code for MPLD 2010/11
; Michal Kubicek 17.4.2011
device kcpsm1
;
;##############################################################################;
; Start of code
;##############################################################################;
; Register name assignment
temp1               EQU       s0                  ; temporary data register
temp2               EQU       s1                  ; temporary data register
temp3               EQU       s2                  ; temporary data register
chreg               EQU       s3                  ; character register
RxDat               EQU       s4                  ; received character register
addr                EQU       s5                  ; address register

; Peripherals
UART_data           EQU        01h                 ; UART data register port ID
UART_stat           EQU        02h                 ; UART control register port ID
; UART Status register:
;  [2] Tx ready
;  [3] new Rx data
;  [4] Rx buffer overflow

BCD_01              EQU        04h                 ; 7 segment, upper two segments
BCD_23              EQU        08h                 ; 7 segment, lower two segments
LED                 EQU        10h                 ; LEDs
Switch              EQU        20h                 ; Switches
Button              EQU        40h                 ; Buttons (3:0)

;##############################################################################;


;##############################################################################;
; Initialization
;##############################################################################;

; initial wait
Start:              CALL      wait_1s             ; wait for peripheral startup
	                LOAD      Temp1,#00           ; LEDs OFF at startup
	                OUT       Temp1,LED           ; write valu to LED register

; end of initialization

;##############################################################################;
;  Main loop
;##############################################################################;
main_loop:          CALL      GetChar             ; get (wait for) new character
                    OUT       chreg,BCD_01        ; load value on 7-segment
                    ADD       chreg,1             ; Increment chreg
                    OUT       chreg,BCD_23        ; load new value on 7-segment
                    CALL      SendChar            ; And send it via UART
                    JUMP      main_loop

;##############################################################################;
;  General purpose procedures
;##############################################################################;

; Basic UART text output procedures (CALLs)

;==============================================================================;
; Send character loadd in chreg via UART
; Registers used: Temp1, chreg
;==============================================================================;
SendChar:           IN        Temp1, UART_stat    ; checking UART status
; load bit 2 (is Tx ready?)
                    JUMP      Z, SendChar         ; wait until Tx is ready
                    OUT       chreg, UART_data    ; Send the character
                    RET                           ; Return from procedure

;==============================================================================;
; Reads a single character from UART (waits on receive when none is prepared)
; Registers used: Temp1, chreg
;==============================================================================;
GetChar:            IN        Temp1, UART_stat    ; checking UART status           ; load bit 3 (new Rx data?)
                    JUMP      Z, GetChar          ; wait for new Rx data
                    IN        chreg, UART_data    ; Read the character
                    RET                           ; Return from procedure

;==============================================================================;
; Procedure for sending content of "chreg" register in hexadecimal ASCII format
;   (two characters, one for each nibble)
; Reister used: Temp1, Temp2, chreg
; Procedures used: SendChar
;==============================================================================;
SendByte:           LOAD      Temp2, chreg        ; make a backup of chreg
                    SR0       chreg               ; move upper nibble to lower nibble
                    SR0       chreg
                    SR0       chreg
                    SR0       chreg
                  ; if not greater than 9, than it is a number
                    JUMP      C, SendBNum1        ; C is set when Temp < 10 (Temp-10)
                    ADD       chreg, #37          ; when letter, add #37; letter conversion
                    JUMP      SendB1
SendBNum1:          ADD       chreg, #30          ; when number, add #30; number conversion
SendB1:             CALL      SendChar            ; Send Character

                    LOAD      chreg, Temp2        ; load the whole byte again
                    AND       chreg, #0Fh          ; select second character
          ; if not greater than 9, than number
                    JUMP      C, SendBNum2        ; C is set when Temp < 10 (Temp-10 under 0)
                    ADD       chreg, #37          ; when letter, add #37; letter conversion
                    JUMP      SendB2
SendBNum2:          ADD       chreg, #30          ; when number, add #30; number conversion
SendB2:             CALL      SendChar            ; Send character
                    RET

;==============================================================================;
; Send 0D and 0A character pair via UART
; Registers used: Temp1, chreg
; Procedures used: SendChar
;==============================================================================;
SendCRLF:           LOAD      chreg, #0D          ; CR character
                    CALL      SendChar            ; Send character
                    LOAD      chreg, #0Ah          ; Load LF character
                    CALL      SendChar            ; Send character
                    RET                           ; Return from procedure

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
;==============================================================================;
wait_1s:            LOAD      Temp1, 250          ; Load Temp1 register
                    LOAD      Temp2, 249          ; Load Temp2 register
                    LOAD      Temp3, 200          ; Load Temp3 register
wait_1s_i:          SUB       Temp1, 1
                    JUMP      NZ, wait_1s_i
                    SUB       Temp2, 1
                    JUMP      NZ, wait_1s_i
                    SUB       Temp3, 1
                    JUMP      NZ, wait_1s_i
                    RET

wait_100ms:         LOAD      Temp1, 250          ; Load Temp1 register
                    LOAD      Temp2, 249          ; Load Temp2 register
                    LOAD      Temp3, 20           ; Load Temp3 register
wait_100ms_i:       SUB       Temp1, 1
                    JUMP      NZ, wait_100ms_i
                    SUB       Temp2, 1
                    JUMP      NZ, wait_100ms_i
                    SUB       Temp3, 1
                    JUMP      NZ, wait_100ms_i
                    RET

;##############################################################################;

