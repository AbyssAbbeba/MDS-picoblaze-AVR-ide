character_a		EQU		0x41
character_f		EQU		0x46
character_i		EQU		0x49
character_l		EQU		0x4c
character_p		EQU		0x50
character_s		EQU		0x53
; KCPSM3 Program - LED control with Pulse Width Modulation (PWM).
; Design provided for use with the design 'low_cost_design_authentication_for_spartan_3e.vhd'
; and the Spartan-3E Starter Kit. This design provides the token 'real' application to be
; protected by design authentication.
;
; Ken Chapman - Xilinx Ltd
;
; Version v1.00 - 9th November 2006
;
; This code automatically sequences the LEDs on the board using PWM to change intensity.
; It also checks for correct design authentication and will perform a different sequence if
; the design is not authorised.
device kcpsm2
;
;**************************************************************************************
; NOTICE:
;
; Copyright Xilinx, Inc. 2006.   This code may be contain portions patented by other
; third parties.  By providing this core as one possible implementation of a standard,
; Xilinx is making no representation that the provided implementation of this standard
; is free from any claims of infringement by any third party.  Xilinx expressly
; disclaims any warranty with respect to the adequacy of the implementation, including
; but not limited to any warranty or representation that the implementation is free
; from claims of any third party.  Furthermore, Xilinx is providing this core as a
; courtesy to you and suggests that you contact all third parties to obtain the
; necessary rights to use this implementation.
;
;
;**************************************************************************************
; Port definitions
;**************************************************************************************
;
;
;
led_port		EQU		0x80			;8 simple LEDs
led0			EQU		0x01			;       LD0 - bit0
led1			EQU		0x02			;       LD1 - bit1
led2			EQU		0x04			;       LD2 - bit2
led3			EQU		0x08			;       LD3 - bit3
led4			EQU		0x10			;       LD4 - bit4
led5			EQU		0x20			;       LD5 - bit5
led6			EQU		0x40			;       LD6 - bit6
led7			EQU		0x80			;       LD7 - bit7
;
led_read_port		EQU		0x00			;read back of current LED drive values
;
;
security_request_port	EQU		0x40			;Port to stimulate security KCPSM3 processor
security_interrupt	EQU		0x01			; interrupt - bit0
;
;
;A FIFO buffer links the security KCPSM3 processor to the application KCPSM3 processor.
;  This application processor controls and reads the FIFO.
;  The security processor writes to the FIFO.
;
link_fifo_control_port	EQU		0x20			;FIFO control
link_fifo_reset		EQU		0x01			;     reset - bit0
;
link_fifo_status_port	EQU		0x01			;FIFO status input
link_fifo_data_present	EQU		0x01			;      half full - bit0
link_fifo_half_full	EQU		0x02			;           full - bit1
link_fifo_full		EQU		0x04			;   data present - bit2
;
link_fifo_read_port	EQU		0x02			;read FIFO data
;
;
;
;**************************************************************************************
; Special Register usage
;**************************************************************************************
;
;
;
;
;**************************************************************************************
;Scratch Pad Memory Locations
;**************************************************************************************
;
pwm_duty_counter	EQU		0x00			;Duty Counter 0 to 255 within 1KHz period (1ms)
pwm_channel0		EQU		0x01			;PWM settings for each channel
pwm_channel1		EQU		0x02			; Channels 0 to 7 = LEDs 0 to 7
pwm_channel2		EQU		0x03
pwm_channel3		EQU		0x04
pwm_channel4		EQU		0x05
pwm_channel5		EQU		0x06
pwm_channel6		EQU		0x07
pwm_channel7		EQU		0x08
isr_preserve_s0		EQU		0x0d			;preserve register contents during Interrupt Service Routine
isr_preserve_s1		EQU		0x0e
isr_preserve_s2		EQU		0x0f
;
;
led0_sequence		EQU		0x10			;LED sequence values
led1_sequence		EQU		0x11
led2_sequence		EQU		0x12
led3_sequence		EQU		0x13
led4_sequence		EQU		0x14
led5_sequence		EQU		0x15
led6_sequence		EQU		0x16
led7_sequence		EQU		0x17
;
;
;
;**************************************************************************************
;Useful data constants
;**************************************************************************************
;
;
;
;
;
;
;
;**************************************************************************************
;Initialise the system
;**************************************************************************************
;
; All PWM channels initialise to off (zero).
; Simple I/O outputs will remain off at all times.
;
cold_start:		LOAD		s0, #0x00
			LOAD		s1, #pwm_channel0
clear_loop:		STORE		s0, @s1
			COMPARE		s1, #pwm_channel7
			JUMP		z, enable_int
			ADD		s1, #0x01
			JUMP		clear_loop
;
enable_int:		ENABLE		interrupt		;interrupts used to set PWM frequency
;
;
; Initialise LED pattern sequence
;
			LOAD		s0, #0x01		;trigger to start wave pattern
			STORE		s0, led0_sequence
			LOAD		s0, #0x00
			STORE		s0, led1_sequence
			STORE		s0, led2_sequence
			STORE		s0, led3_sequence
			STORE		s0, led4_sequence
			STORE		s0, led5_sequence
			STORE		s0, led6_sequence
			STORE		s0, led7_sequence
;
;
; Reset authentication check counter
;
			LOAD		sf, #0x00
;
;
;**************************************************************************************
; Main program
;**************************************************************************************
;
; Provides a pattern of interest on the LEDs :-)
;
; Each LED increases intensity in 8 steps and then decreases intensity in 8 steps until it is off.
; The middle LEDs (LD2 to LD5) each start to turn on when either neighbour is turned half on and increasing
; to provide the effect of a passing a 'wave' of light passing from side to side. The pair of LEDs at each
; (LD0, Ld1 and LD6, LD7) are required to reflect the 'wave' so that the pattern continues.
;
; I'm sure this code cold be written in more elegant way, but I leave that as an exercise to you :-)
;
;
; Using a simple software counter (implemented by register sF) the design occasionally requests an
; authorisation message from the authentication processor. If it receives a PASS message it continues
; normally but if it receives a FAIL message the LED pattern is changed.
;
;
;
warm_start:		ADD		sf, #0x01		;authentication check timer
			JUMP		c, authentication_check	;Check made approximately every 8 seconds.
;
normal_led_sequence:	LOAD		s2, #0x03		;simple delay loop (delay will be increased by ISR processing)
delay_s2_loop:		LOAD		s1, #0xff
delay_s1_loop:		LOAD		s0, #0xff
delay_s0_loop:		SUB		s0, #0x01
			JUMP		nc, delay_s0_loop
			SUB		s1, #0x01
			JUMP		nc, delay_s1_loop
			SUB		s2, #0x01
			JUMP		nc, delay_s2_loop
;
;Pattern generation
;
			FETCH		s0, led0_sequence	;read sequence for LED0
			COMPARE		s0, #0x00
			JUMP		z, test_led0_start
			SUB		s0, #0x20		;Count longer to ensure end stops then reset count if maximum
			JUMP		z, update_led0
			ADD		s0, #0x20
inc_led0:		ADD		s0, #0x01		;increment counter
			JUMP		update_led0
test_led0_start:	FETCH		s1, led1_sequence	;start LED0 if LED1 = 4
			COMPARE		s1, #0x04
			JUMP		z, inc_led0
update_led0:		STORE		s0, led0_sequence
			CALL		led_to_duty
			STORE		s1, pwm_channel0
;
			FETCH		s1, led0_sequence	; refresh LED1 if LED0 = 11 (0B hex) to reflect wave
			COMPARE		s1, #0x0b
			JUMP		nz, normal_led1
			LOAD		s0, #0x04
			JUMP		update_led1
normal_led1:		FETCH		s0, led1_sequence	;read sequence for LED1
			COMPARE		s0, #0x00
			JUMP		z, test_led1_start
			SUB		s0, #0x10		;reset count if maximum
			JUMP		z, update_led1
			ADD		s0, #0x10
inc_led1:		ADD		s0, #0x01		;increment counter
			JUMP		update_led1
test_led1_start:	FETCH		s1, led0_sequence	;start LED1 if LED0 = 11 (0B hex) to reflect wave
			COMPARE		s1, #0x0b
			JUMP		z, inc_led1
			FETCH		s1, led2_sequence	;start LED1 if LED2 = 4
			COMPARE		s1, #0x04
			JUMP		z, inc_led1
update_led1:		STORE		s0, led1_sequence
			CALL		led_to_duty
			STORE		s1, pwm_channel1
;
			FETCH		s0, led2_sequence	;read sequence for LED2
			COMPARE		s0, #0x00
			JUMP		z, test_led2_start
			SUB		s0, #0x10		;reset count if maximum
			JUMP		z, update_led2
			ADD		s0, #0x10
inc_led2:		ADD		s0, #0x01		;increment counter
			JUMP		update_led2
test_led2_start:	FETCH		s1, led1_sequence	;start LED2 if LED1 = 4
			COMPARE		s1, #0x04
			JUMP		z, inc_led2
			FETCH		s1, led3_sequence	;start LED2 if LED3 = 4
			COMPARE		s1, #0x04
			JUMP		z, inc_led2
update_led2:		STORE		s0, led2_sequence
			CALL		led_to_duty
			STORE		s1, pwm_channel2
;
;
			FETCH		s0, led3_sequence	;read sequence for LED3
			COMPARE		s0, #0x00
			JUMP		z, test_led3_start
			SUB		s0, #0x10		;reset count if maximum
			JUMP		z, update_led3
			ADD		s0, #0x10
inc_led3:		ADD		s0, #0x01		;increment counter
			JUMP		update_led3
test_led3_start:	FETCH		s1, led2_sequence	;start LED3 if LED2 = 4
			COMPARE		s1, #0x04
			JUMP		z, inc_led3
			FETCH		s1, led4_sequence	;start LED3 if LED4 = 4
			COMPARE		s1, #0x04
			JUMP		z, inc_led3
update_led3:		STORE		s0, led3_sequence
			CALL		led_to_duty
			STORE		s1, pwm_channel3
;
			FETCH		s0, led4_sequence	;read sequence for LED4
			COMPARE		s0, #0x00
			JUMP		z, test_led4_start
			SUB		s0, #0x10		;reset count if maximum
			JUMP		z, update_led4
			ADD		s0, #0x10
inc_led4:		ADD		s0, #0x01		;increment counter
			JUMP		update_led4
test_led4_start:	FETCH		s1, led3_sequence	;start LED4 if LED3 = 4
			COMPARE		s1, #0x04
			JUMP		z, inc_led4
			FETCH		s1, led5_sequence	;start LED4 if LED5 = 4
			COMPARE		s1, #0x04
			JUMP		z, inc_led4
update_led4:		STORE		s0, led4_sequence
			CALL		led_to_duty
			STORE		s1, pwm_channel4
;
			FETCH		s0, led5_sequence	;read sequence for LED5
			COMPARE		s0, #0x00
			JUMP		z, test_led5_start
			SUB		s0, #0x10		;reset count if maximum
			JUMP		z, update_led5
			ADD		s0, #0x10
inc_led5:		ADD		s0, #0x01		;increment counter
			JUMP		update_led5
test_led5_start:	FETCH		s1, led4_sequence	;start LED5 if LED4 = 4
			COMPARE		s1, #0x04
			JUMP		z, inc_led5
			FETCH		s1, led6_sequence	;start LED5 if LED6 = 4
			COMPARE		s1, #0x04
			JUMP		z, inc_led5
update_led5:		STORE		s0, led5_sequence
			CALL		led_to_duty
			STORE		s1, pwm_channel5
;
			FETCH		s1, led7_sequence	; refresh LED6 if LED7 = 11 (0B hex) to reflect wave
			COMPARE		s1, #0x0b
			JUMP		nz, normal_led6
			LOAD		s0, #0x04
			JUMP		update_led6
normal_led6:		FETCH		s0, led6_sequence	;read sequence for LED6
			COMPARE		s0, #0x00
			JUMP		z, test_led6_start
			SUB		s0, #0x10		;reset count if maximum
			JUMP		z, update_led6
			ADD		s0, #0x10
inc_led6:		ADD		s0, #0x01		;increment counter
			JUMP		update_led6
test_led6_start:	FETCH		s1, led5_sequence	;start LED6 if LED5 = 4
			COMPARE		s1, #0x04
			JUMP		z, inc_led6
update_led6:		STORE		s0, led6_sequence
			CALL		led_to_duty
			STORE		s1, pwm_channel6
;
			FETCH		s0, led7_sequence	;read sequence for LED7
			COMPARE		s0, #0x00
			JUMP		z, test_led7_start
			SUB		s0, #0x20		;Count longer to ensure end stops then reset count if maximum
			JUMP		z, update_led7
			ADD		s0, #0x20
inc_led7:		ADD		s0, #0x01		;increment counter
			JUMP		update_led7
test_led7_start:	FETCH		s1, led6_sequence	;start LED7 if LED6 = 4
			COMPARE		s1, #0x04
			JUMP		z, inc_led7
update_led7:		STORE		s0, led7_sequence
			CALL		led_to_duty
			STORE		s1, pwm_channel7
			JUMP		warm_start
;
;
; Convert LED sequence number into PWM intensity figure
;
; LEDs duty cycle values are 0,1,2,4,8,16,32 and 64 because they appear to give what
; appears to be a fairly liner change in intensity and provides a simple way to set
; the duty value.
;
; Provide sequence value in register s0 and intensity will be
; returned in register s1.
;
; s0   s1
; 00   00
; 01   01
; 02   02
; 03   04
; 04   08
; 05   10
; 06   20
; 07   40
; 08   80
; 09   40
; 0A   20
; 0B   10
; 0C   08
; 0D   04
; 0E   02
; 0F   01
; 10   00  and zero for all larger values of s0
;
led_to_duty:		LOAD		s1, #0x00
			COMPARE		s0, #0x00		;test for zero
			RETURN		z
			LOAD		s1, #0x01		;inject '1'
go_up_loop:		SUB		s0, #0x01
			RETURN		z
			SL0		s1			;multiply by 2
			JUMP		c, go_down
			JUMP		go_up_loop
go_down:		LOAD		s1, #0x40
go_down_loop:		SUB		s0, #0x01
			RETURN		z
			SR0		s1			;divide by 2
			JUMP		go_down_loop
;
;
;
;**************************************************************************************
; Authentication Check and fail procedure
;**************************************************************************************
;
; The authentication check is performed by issuing and interrupt to the authentication
; processor and then observing the simple text string that it returns via the link FIFO
; buffer.
;
; PASS - Design is authorised to work.
; FAIL - Design is not authorised and should stop working normally.
;
;
;ASCII character values that are used in messages
;
; >>>>> (line moved to the beginning) <<<<<
; >>>>> (line moved to the beginning) <<<<<
; >>>>> (line moved to the beginning) <<<<<
; >>>>> (line moved to the beginning) <<<<<
; >>>>> (line moved to the beginning) <<<<<
; >>>>> (line moved to the beginning) <<<<<
;
;
authentication_check:	LOAD		s0, #link_fifo_reset	;clear link FIFO to ensure no unexpected characters
			OUTPUT		s0, link_fifo_control_port
			LOAD		s0, #0x00
			OUTPUT		s0, link_fifo_control_port
;
			LOAD		s0, #security_interrupt	;generate interrupt to authentication processor
			OUTPUT		s0, security_request_port
			LOAD		s0, #0x00
			OUTPUT		s0, security_request_port
;
			CALL		read_link_fifo		;read each character and compare
			COMPARE		s0, #character_p
			JUMP		nz, fail_confirm
			CALL		read_link_fifo
			COMPARE		s0, #character_a
			JUMP		nz, fail_confirm
			CALL		read_link_fifo
			COMPARE		s0, #character_s
			JUMP		nz, fail_confirm
			CALL		read_link_fifo
			COMPARE		s0, #character_s
			JUMP		nz, fail_confirm
			JUMP		normal_led_sequence	;Continue normal operation for PASS message
;
;
; To confirm that the authentication is really a FAIL message
; another request is made to the authentication processor and tested.
;
fail_confirm:		LOAD		s0, #0xff		;short delay to ensure authentication processor is ready
request_delay:		SUB		s0, #0x01		;   to respond to new interrupt request
			JUMP		nz, request_delay
;
			LOAD		s0, #link_fifo_reset	;clear link FIFO to ensure no unexpected characters
			OUTPUT		s0, link_fifo_control_port
			LOAD		s0, #0x00
			OUTPUT		s0, link_fifo_control_port
;
			LOAD		s0, #security_interrupt	;generate interrupt to authentication processor
			OUTPUT		s0, security_request_port
			LOAD		s0, #0x00
			OUTPUT		s0, security_request_port
;
			CALL		read_link_fifo		;read each character and compare
			COMPARE		s0, #character_f
			JUMP		nz, normal_led_sequence
			CALL		read_link_fifo
			COMPARE		s0, #character_a
			JUMP		nz, normal_led_sequence
			CALL		read_link_fifo
			COMPARE		s0, #character_i
			JUMP		nz, normal_led_sequence
			CALL		read_link_fifo
			COMPARE		s0, #character_l
			JUMP		nz, normal_led_sequence
;
;
; When the design fails to authenticate the LEDs will appear to
; turn on and then slowly fade to off using PWM.
;
failed_led_sequence:	LOAD		s0, #0xff		;maximum intensity on all LEDs
			LOAD		s4, #0x00		;reset fade rate control
all_led_fade:		LOAD		s1, #pwm_channel0
all_led_fade_loop:	STORE		s0, @s1
			COMPARE		s1, #pwm_channel7
			JUMP		z, decay_leds
			ADD		s1, #0x01
			JUMP		all_led_fade_loop
decay_leds:		LOAD		s1, s4			;software delay starts quickly and slows down because LEDs are non-linear.
wait_s1:		LOAD		s2, #0x18
wait_s2:		LOAD		s3, #0xff
wait_s3:		SUB		s3, #0x01
			JUMP		nz, wait_s3
			SUB		s2, #0x01
			JUMP		nz, wait_s2
			SUB		s1, #0x01
			JUMP		nz, wait_s1
			COMPARE		s0, #0x00		;test for fully off
			JUMP		z, stop_completely
			SUB		s0, #0x01		;fade LEDs
			ADD		s4, #0x01		;slow fade rate as intensity decreases
			JUMP		all_led_fade
;
stop_completely:	JUMP		stop_completely
;
;**************************************************************************************
; Read Byte from Link FIFO
;**************************************************************************************
;
; The routine first tests the FIFO buffer to see if data is present.
; If the FIFO is empty, the routine waits until there is a character to read.
; the read value is returned in register s0.
;
;
read_link_fifo:		INPUT		s0, link_fifo_status_port	;test FIFO buffer
			TEST		s0, #link_fifo_data_present	;wait if empty
			JUMP		z, read_link_fifo
			INPUT		s0, link_fifo_read_port	;read data from FIFO
			RETURN
;
;
;**************************************************************************************
; Interrupt Service Routine (ISR)
;**************************************************************************************
;
; Interrupts occur at 3.92us intervals and are used to generate the PWM pulses generated
; at a PRF of 1KHz. The 3.92us interrupt rate corresponds with a resolution of 256 steps
; over the 1ms associated with the 1KHz PRF.
;
; The ISR is self contained and all registers used are preserved. Scratch pad memory
; locations are used to determine the desired duty factor for each of 8 channels.
;
; Note that an interrupt is generated every 196 clock cycles. This means that there is
; only time to execute 98 instructions between each interrupt. This ISR is 35 instructions
; long. A further 3 instructions are also consumed by the interrupt process
; (abandoned instruction, virtual CALL to 3FF and the interrupt vector JUMP) and hence
; PicoBlaze has approximately 63% of its time available for other tasks in the main program.
;
; Although a loop would normal be employed in software to process each of 8 channels,
; the implementation of a loop would increase the number of instructions which needed to
; be executed significantly reduce the time available for the main program to operate.
; Consequently the code is written out in a linear fashion which consumes more program
; space but which executes faster.
;
isr:			STORE		s0, isr_preserve_s0	;preserve registers to be used
			STORE		s1, isr_preserve_s1
			STORE		s2, isr_preserve_s2
;Determine the number of steps currently through the 1ms PWM cycle
			FETCH		s1, pwm_duty_counter	;read 8-bit counter of steps
			ADD		s1, #0x01		;increment counter (will roll over to zero)
			STORE		s1, pwm_duty_counter	;update count value in memory for next interrupt.
;Read duty factor for each channel and compare it with the duty counter and set or
;reset a bit in register s2 accordingly.
			FETCH		s0, pwm_channel7	;read desired setting of pulse width
			COMPARE		s1, s0			;set carry flag if duty factor > duty counter
			SLA		s2			;shift carry into register s2
			FETCH		s0, pwm_channel6	;read desired setting of pulse width
			COMPARE		s1, s0			;set carry flag if duty factor > duty counter
			SLA		s2			;shift carry into register s2
			FETCH		s0, pwm_channel5	;read desired setting of pulse width
			COMPARE		s1, s0			;set carry flag if duty factor > duty counter
			SLA		s2			;shift carry into register s2
			FETCH		s0, pwm_channel4	;read desired setting of pulse width
			COMPARE		s1, s0			;set carry flag if duty factor > duty counter
			SLA		s2			;shift carry into register s2
			FETCH		s0, pwm_channel3	;read desired setting of pulse width
			COMPARE		s1, s0			;set carry flag if duty factor > duty counter
			SLA		s2			;shift carry into register s2
			FETCH		s0, pwm_channel2	;read desired setting of pulse width
			COMPARE		s1, s0			;set carry flag if duty factor > duty counter
			SLA		s2			;shift carry into register s2
			FETCH		s0, pwm_channel1	;read desired setting of pulse width
			COMPARE		s1, s0			;set carry flag if duty factor > duty counter
			SLA		s2			;shift carry into register s2
			FETCH		s0, pwm_channel0	;read desired setting of pulse width
			COMPARE		s1, s0			;set carry flag if duty factor > duty counter
			SLA		s2			;shift carry into register s2
			OUTPUT		s2, led_port		;drive LEDs
			FETCH		s0, isr_preserve_s0	;restore register values
			FETCH		s1, isr_preserve_s1
			FETCH		s2, isr_preserve_s2
			RETURNI		enable
;
;
;**************************************************************************************
; Interrupt Vector
;**************************************************************************************
;
			ORG		0x3ff
			JUMP		isr
;
;

