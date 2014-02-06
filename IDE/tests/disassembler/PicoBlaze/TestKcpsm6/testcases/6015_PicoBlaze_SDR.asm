
;
; PicoBlaze kód pro počáteční inicializaci A/D převodníku ADS6445 a zdroje hodin LMK4011

; A/D převodník - 16bit registry
;				  Max 20 MHz
;				  A4-A0 + D10-D0	
;
; LMK4011 -		  32bit registry
; 				  Max 20 MHz
;				  D27-D0 + A3-A0
;
; Martin Madron 30.4.2013
;
;
device kcpsm6
; Start
;##############################################################################;
; Přiřazení jmen registrům
	NAMEREG		s0,temp1              ; temporary data register
	NAMEREG		s1,temp2              ; temporary data register
	NAMEREG		s2,temp3              ; temporary data register
	NAMEREG		s3,RXdata             ; temporary data register
	NAMEREG		s4,TXdata             ; temporary data register

; PORT_IDs
	CONSTANT	TX_data_01, 01          ;  data register port ID
	CONSTANT	TX_data_02, 02          ;  data register port ID
	CONSTANT	TX_data_03, 04          ;  data register port ID
	CONSTANT	TX_data_04, 08          ;  data register port ID
	CONSTANT	Read_data, 12           ;  data register port ID

CHREG                   equ             4
SWITCH                   equ             4
UART_STAT                   equ             4
UART_DATA                   equ             4

	
; Inicializace
;##############################################################################;
	ADDRESS	000
	JUMP	Start
; Čekání 1s pro inicializaci obvodů
Start:              CALL      wait_1s             ; wait for 1s
; end of initialization

;  Main 
main_loop:          CALL      GetChar             ; get (wait for) new character
                    COMPARE      chreg,#20           ; Space character received?
                    JUMP      NZ,main_loop        ; If not space, get another character
                    IN        chreg,Switch        ; If yes, read content of switches
                    CALL      SendChar            ; And send it via UART
                    JUMP      main_loop

;##############################################################################;
;  General purpose procedures
;##############################################################################;

; Basic UART text output procedures (CALLs)

;==============================================================================;
; Send character stored in chreg via UART
; Registers used: Temp1, chreg
;==============================================================================;
SendChar:           IN        Temp1, UART_stat    ; checking UART status
                    TEST      Temp1, 4            ; test bit 2 (is Tx ready?)
                    JUMP      Z, SendChar         ; wait until Tx is ready
                    OUT       chreg, UART_data    ; Send the character
                    RET                           ; Return from procedure

;==============================================================================;
; Reads a single character from UART (waits on receive when none is prepared)
; Registers used: Temp1, chreg
;==============================================================================;
GetChar:            IN        Temp1, UART_stat    ; checking UART status
                    TEST      Temp1, 8            ; test bit 3 (new Rx data?)
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
                    CMP      chreg, 10           ; if not greater than 9, than it is a number
                    JUMP      C, SendBNum1        ; C is set when Temp < 10 (Temp-10)
                    ADD       chreg, #37          ; when letter, add #37; letter conversion
                    JUMP      SendB1
SendBNum1:          ADD       chreg, #30          ; when number, add #30; number conversion
SendB1:             CALL      SendChar            ; Send Character

                    LOAD      chreg, Temp2        ; load the whole byte again
                    AND       chreg, #0Fh          ; select second character
                    CMP      chreg, 10           ; if not greater than 9, than number
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
;   1s @ (10 MHz, Temp1 = 250, Temp2 = 249, Temp3 = 40)
;
; ČEKACÍ SMYČKY
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