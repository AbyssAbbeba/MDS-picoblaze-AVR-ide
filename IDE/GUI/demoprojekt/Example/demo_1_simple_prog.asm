 

;
; PicoBlaze kod pro pocatecni­ inicializaci A/D prevodniku ADS6445 a zdroje hodin LMK4011 + SPUŠTĚNÍ TEST SEKVENCÍ

; A/D převodnik - 16bit registry
;				  Max 20 MHz
;				  A4-A0 + D10-D0	
;
; LMK4011 -		  32bit registry
; 				  Max 20 MHz
;				  D27-D0 + A3-A0
;
; Martin Madron 20.2.2014
;
;

; Start
;##############################################################################;
TEMP1                   REG             S0                      ; temporary data register
TEMP2                   REG             S1                      ; temporary data register
TEMP3                   REG             S2                      ; temporary data register
RXDATA                  REG             S3                      ; temporary data register
TXDATA                  REG             S4                      ; temporary data register

READ_DATA               EQU             12h                     ;  data register port ID	
; PORT_IDs
TX_data_register_01		PORT			01h          ;  data register port ID
TX_data_register_02		PORT			02h          ;  data register port ID
TX_data_register_03		PORT			04h          ;  data register port ID
TX_data_register_04		PORT			08h          ;  data register port ID
; Inicializace
;##############################################################################;
					ORG		000h
					JUMP	Start
; Čekání 1s pro inicializaci obvodů
Start:              CALL      wait_1s             ; wait for 1s
; end of initialization

;  Main 
main_loop:          CALL      AD_busy_check             ; CONFIGURE A/D CONVERTER
                    CALL      AD_config                ; 
					CALL	  LMK_busy_check
					CALL	  LMK_config
config_done:					
                    JUMP      config_done

;##############################################################################;
;  General purpose procedures
;##############################################################################;

; Basic output procedures (CALLs)
AD_busy_check:      INPUT     RXdata, Read_data    ; checking SPI status
                    TEST      RXdata, #01h            ; test bit 1 
                    RETURN    NZ                    ; wait for SPI controler to be ready
                    JUMP	  AD_busy_check        ; Return from procedure

;==============================================================================;
LMK_busy_check:		INPUT     RXdata, Read_data    ; checking SPI status
                    TEST      RXdata, #01h            ; test bit 1 
                    RETURN    NZ                    ; wait for SPI controler to be ready
                    JUMP	  LMK_busy_check        ; Return from procedure	
;==============================================================================;
; Configure sequence for A/D 
; Registers used: TXdata
;==============================================================================;
					; first 2 x 16bit data
AD_config:          LOAD	  TXdata, #00h	
					OUTPUT    TXdata, TX_data_register_01        ; sends 0-7bit data
					LOAD	  TXdata, #00h	
					OUTPUT    TXdata, TX_data_register_02        ; sends 7-15bit data
					LOAD	  TXdata, #04h	
					OUTPUT    TXdata, TX_data_register_03        ; sends 15-23bit data
					LOAD	  TXdata, #11h	
					OUTPUT    TXdata, TX_data_register_04        ; sends 23-31bit data
                    CALL	  AD_busy_check	     			     ; wait for tx to be completed
                    RETURN                          			 ; Return from procedure

;==============================================================================;
					; first 32bit data
LMK_config:         LOAD	  TXdata, #0Dh	
					OUTPUT    TXdata, TX_data_register_01        ; sends 0-7bit data
					LOAD	  TXdata, #0Fh	
					OUTPUT    TXdata, TX_data_register_02        ; sends 7-15bit data
					LOAD	  TXdata, #04h	
					OUTPUT    TXdata, TX_data_register_03        ; sends 15-23bit data
					LOAD	  TXdata, #12h	
					OUTPUT    TXdata, TX_data_register_04        ; sends 23-31bit data
                    CALL	  LMK_busy_check	     			 ; wait for tx to be completed
					; next 32bit data
					LOAD	  TXdata, #11h	
					OUTPUT    TXdata, TX_data_register_01        ; sends 0-7bit data
					LOAD	  TXdata, #48h	
					OUTPUT    TXdata, TX_data_register_02        ; sends 7-15bit data
					LOAD	  TXdata, #44h	
					OUTPUT    TXdata, TX_data_register_03        ; sends 15-23bit data
					LOAD	  TXdata, #14h
					OUTPUT    TXdata, TX_data_register_04        ; sends 23-31bit data
                    CALL	  LMK_busy_check	     			 ; wait for tx to be completed
                    RETURN                          			 ; Return from procedure
;==============================================================================;
; Send character stored in chreg via UART
; Registers used: Temp1, chreg
;==============================================================================;

;==============================================================================;
; Reads a single character from UART (waits on receive when none is prepared)
; Registers used: Temp1, chreg
;==============================================================================;
                              ; Return from procedure

;==============================================================================;
; Procedure for sending content of "chreg" register in hexadecimal ASCII format
;   (two characters, one for each nibble)
; Reister used: Temp1, Temp2, chreg
; Procedures used: SendChar
;==============================================================================;

;==============================================================================;
; Send 0D and 0A character pair via UART
; Registers used: Temp1, chreg
; Procedures used: SendChar
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
;   1s @ (10 MHz, Temp1 = 250, Temp2 = 249, Temp3 = 40)
;
; cekaci smycky
;==============================================================================;
; jedna sec
WAIT_1S:                LOAD            TEMP1, #1             ; Load Temp1 register
                        LOAD            TEMP2, #1             ; Load Temp2 register
                        LOAD            TEMP3, #1             ; Load Temp3 register
WAIT_1S_I:              SUB             TEMP1, #1
                        JUMP            NZ, WAIT_1S_I
                        SUB             TEMP2, #1
                        JUMP            NZ, WAIT_1S_I
                        SUB             TEMP3, #1
                        JUMP            NZ, WAIT_1S_I
                        RET
; 100 ms
;=====================================================================================
WAIT_100MS:             LOAD            TEMP1, #1             ; Load Temp1 register
                        LOAD            TEMP2, #1             ; Load Temp2 register
                        LOAD            TEMP3, #1             ; Load Temp3 register
WAIT_100MS_I:           SUB             TEMP1, #1
                        JUMP            NZ, WAIT_100MS_I
                        SUB             TEMP2, #1
                        JUMP            NZ, WAIT_100MS_I
                        SUB             TEMP3, #1
                        JUMP            NZ, WAIT_100MS_I
                        RET

;##############################################################################;
