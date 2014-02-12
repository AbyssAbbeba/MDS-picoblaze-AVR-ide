; Demonstration project for PicoBlaze microcontroler
; See manual for more info

; Press Start simulation and Animate to run the program
		DEVICE	KCPSM3
;
;---------------------------------------------------
; 		SYMBOL DEFINITIONS
;----------------------
numero		EQU		0b01010101
cnt			SET		1
uart_byte	REG		sD
;---------------------------------------------------
; 		MACRO DECLARATIONS
;----------------------------------------------------
;
roll	MACRO	num				; rotate macro declaration

		LOAD	s6,#num

		REPT	7				; rotate with register s6
		RL		s6
		ENDR
						
		ENDM
;------------
wait	MACRO					; rotate macro declaration
		local		waitin
		
		LOAD	sF,#255
waitin:	
		SUB		sF,#1
		JUMP	NZ,waitin					
		ENDM

;==============================================================================;
; Macro for sending character via UART
; Parameters: 1
;==============================================================================;
Sendchar            MACRO       char                 ; One parameter
					load		s8,#char
					out			sB,@s8
                    ENDM

;==============================================================================;
; Send 0D and 0A character pair via UART
; Macros used: SendChar
;==============================================================================;
SendCRLF            MACRO       
					load		sC,#18
                    ENDM  
;---------------------------------------------------
; 		MAIN PROGRAM EXECUTION
;----------------------------------------------------
;
MAIN:			
		add		S0, #2
		add		S1, #1
		store	S0, @S1
 		out		S0, @S1
		in		S0, @S1
;--------------------------------------------------
		FOR		sA,1..3,1
			LOAD	s3,#154
			ADD		s3,#15 + cnt
			cnt		SET		cnt + 1
		ENDF
;------------------------------------------------------
		ena							; enable interrupt
		load	uart_byte,#1
;-----------------------------------------------------
        IF  		uart_byte == #1 
				LOAD	s7,#12
				ADD		s7,#14 + cnt		
				SR0				sB

         ELSEIF  	uart_byte == #2	
             SendChar  'H'
             SendChar  'E'
             SendChar  'L'
             SendChar  'L'
             SendChar  'O'
             SendChar  '!'
             SendCRLF
         ENDIF
;-----------------------------------------------------
        load            uart_byte,#5h
        load            s6,#2h
;-----------------------------------------------------
		WHILE		uart_byte != #1
		SUB			uart_byte,#1		
		ENDW




		call	Z,podp
		jump	main

podp:   
		call	podp2
		return
podp2:	
		call	podp3
		return
podp3:	return
;---------------------------------------------------
; 		INTERRUPT SERVICE	ROUTINE
;----------------------------------------------------
;
int:	load	sE,#0xff
		wait
		returni	enable
;---------------------------------------------------
; 		INTERRUPT VECTOR
;----------------------------------------------------
;	
		org		0x3ff
		jump	int
;		end of program