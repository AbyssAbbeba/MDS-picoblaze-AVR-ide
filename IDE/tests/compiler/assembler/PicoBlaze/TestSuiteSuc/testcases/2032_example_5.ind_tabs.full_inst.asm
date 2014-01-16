; KCPSM3 Program - Control and calculation for Frequency Generator design using the
;                 Spartan-3E Starter Kit.
; Interfaces with the rotary encoder and LCD display to enable a frequency to be set.
; Converts the BCD frequency value into a binary integer and then performs the high
; precision calculation necessary to derive the control numbers required by the high
; performance Direct Digital Synthesis (DDS) circuit implemented in hardware.
;
; LEDs are connected and used as edit mode indicators.
;
; Substantial comments are included in line with the code below and should be used
; in conjunction with the documentation provided with the complete reference design.
;
;
;
; Ken Chapman - Xilinx Ltd
;
; Version v1.00 - 13th July 2006
;
;**************************************************************************************
;Port definitions
;**************************************************************************************
;
;
;
led_port		EQU		0x80			;8 simple LEDs
led0			EQU		0x01			;     LED 0 - bit0
led1			EQU		0x02			;         1 - bit1
led2			EQU		0x04			;         2 - bit2
led3			EQU		0x08			;         3 - bit3
led4			EQU		0x10			;         4 - bit4
led5			EQU		0x20			;         5 - bit5
led6			EQU		0x40			;         6 - bit6
led7			EQU		0x80			;         7 - bit7
;
;
rotary_port		EQU		0x00			;Read status of rotary encoder
rotary_left		EQU		0x01			; Direction of last move Left=1 Right=0  - bit0
rotary_press		EQU		0x02			;     Centre press contact (active High) - bit1
;
;
;LCD interface ports
;
;The master enable signal is not used by the LCD display itself
;but may be required to confirm that LCD communication is active.
;This is required on the Spartan-3E Starter Kit if the StrataFLASH
;is used because it shares the same data pins and conflicts must be avoided.
;
lcd_output_port		EQU		0x40			;LCD character module output data and control
lcd_e			EQU		0x01			;   active High Enable        E - bit0
lcd_rw			EQU		0x02			;   Read=1 Write=0           RW - bit1
lcd_rs			EQU		0x04			;   Instruction=0 Data=1     RS - bit2
lcd_drive		EQU		0x08			;   Master enable (active High) - bit3
lcd_db4			EQU		0x10			;   4-bit              Data DB4 - bit4
lcd_db5			EQU		0x20			;   interface          Data DB5 - bit5
lcd_db6			EQU		0x40			;                      Data DB6 - bit6
lcd_db7			EQU		0x80			;                      Data DB7 - bit7
;
;
lcd_input_port		EQU		0x01			;LCD character module input data
lcd_read_db4		EQU		0x10			;    4-bit           Data DB4 - bit4
lcd_read_db5		EQU		0x20			;    interface       Data DB5 - bit5
lcd_read_db6		EQU		0x40			;                    Data DB6 - bit6
lcd_read_db7		EQU		0x80			;                    Data DB7 - bit7
;
;
;
;DDS control ports
;
;DDS control word is 32-bits
;
dds_control0_port	EQU		0x02			; dds_control_word(7:0)
dds_control1_port	EQU		0x04			; dds_control_word(15:8)
dds_control2_port	EQU		0x08			; dds_control_word(23:16)
dds_control3_port	EQU		0x10			; dds_control_word(31:24)
;
;Frequency scaling control word is 5-bits
;
dds_scaling_port	EQU		0x20			; dds_scaling_word(4:0)
;
;
;**************************************************************************************
;Special Register usage
;**************************************************************************************
;
;**************************************************************************************
;Scratch Pad Memory Locations
;**************************************************************************************
;
rotary_status		EQU		0x00			;Status of rotary encoder
rotary_event		EQU		0x80			;  flag set by interrupt in 'rotary_status' - bit7
;
isr_preserve_s0		EQU		0x01			;Preserve s0 contents during ISR
;
led_pattern		EQU		0x02			;LED pattern used in rotation mode
;
;
;BCD digits representing selected and displayed frequency
;
bcd_digit0		EQU		0x03			; value           1
bcd_digit1		EQU		0x04			;                10
bcd_digit2		EQU		0x05			;               100
bcd_digit3		EQU		0x06			;             1,000
bcd_digit4		EQU		0x07			;            10,000
bcd_digit5		EQU		0x08			;           100,000
bcd_digit6		EQU		0x09			;         1,000,000
bcd_digit7		EQU		0x0a			;        10,000,000
bcd_digit8		EQU		0x0b			;       100,000,000
;
;
;Binary integer representation of BCD value
;
frequency0		EQU		0x0c			;LS byte
frequency1		EQU		0x0d
frequency2		EQU		0x0e
frequency3		EQU		0x0f			;MS byte
;
;
;Control of frequency selection values
;
cursor_position		EQU		0x10			; Pointer to edit position on LCD
edit_digit_pointer	EQU		0x11			; BCD digit to be changed
;
;
;
;80-bit product resulting from 32-bit frequency x 48-bit scaling constant
;
product0		EQU		0x12			;LS byte
product1		EQU		0x13
product2		EQU		0x14
product3		EQU		0x15
product4		EQU		0x16
product5		EQU		0x17
product6		EQU		0x18
product7		EQU		0x19
product8		EQU		0x1a
product9		EQU		0x1b			;MS byte
;
;Local copies of the DDS control word and DDS scaling word
;
dds_control0		EQU		0x1c			; dds_control_word(7:0)
dds_control1		EQU		0x1d			; dds_control_word(15:8)
dds_control2		EQU		0x1e			; dds_control_word(23:16)
dds_control3		EQU		0x1f			; dds_control_word(31:24)
dds_scaling		EQU		0x20			; dds_scaling_word(4:0)
;
;**************************************************************************************
; Useful data constants
;**************************************************************************************
;
; To convert the frequency into a DDS control value a high precision scaling
; factor is used. This is a 48-bit number which converts the frequency presented
; as an 32-bit integer into the 32-bit value required by the phase accumulator
; to synthesize the desired frequency. The scaling factor is derived using the
; following method. First I will consider the scaling factor which results in the
; desired frequency being generated directly at the output of the phase accumulator
; which is suitable for low frequencies in which a few ns of jitter is acceptable.
;
; 'Fpa' is frequency generated by the MSB of the phase accumulator.
; 'p' is number of phase accumulator which in this case is 32 bits.
; 'clk' is the input clock frequency to the phase accumulator which is 200MHz.
; 'N' is the DDS control word value which is also 'p' bits (32 in this case).
;
; Frequency at MSB of phase accumulator is then
;
;       Fpa = clk x N / (2^p)
;
; Note that the maximum value allowed for 'N' is (2^p)/2 which results in Fpa=clk/2.
;  for 'N' greater than that value 'Fpa' would decrease in frequency (aliasing).
;
;
; By simple reorganisation of the equation we can compute 'N'
;
;       N = Fpa x (2^p) / clk
;
;
; Now it is easier to approach the next step using specific example.
;
; So for a frequency of Fpa = 1MHz then
;       N = 1MHz x (2^32)/200MHz = 21474836.48
;
; We must use the nearest 32-bit integer value 21474836 and this in turn
; is best viewed as the 32-bit hexadecimal value 0147AE14.
;
; In this case the value we have to work with is a 32-bit integer frequency
; value of 1 million which is 000F4240.
;
; So now we need to translate the value 000F4240 into 0147AE14. This is
; where a 48-bit scaling value is used together with a full precision multiplier
; as this ensures adequate accuracy of the final frequency.
;
;        32-bit frequency value                  ffffffff
;        48-bit scaling value              x ssssssssssss
;                                    --------------------
;        80-bit product              nnnnnnnnnnnnnnnnnnnn
;
; The art is to organise the scaling factor into the range where the most is made of
; the 48-bit resolution available but which will result in the correct 32-bit output.
; The way this is achieved is the select an appropriate 32-bits from the available 80-bit
; product for use as 'N' and truncate 'y' least significant bits.
;
; From this we can deduce that for a target frequency 'Ft' at the input then the
; scaling value 'S' is given by
;
;    S = N x (2^y) / Ft    with the condition that S < 2^48 but as large as possible
;
; For best accuracy we calculate 'S' using the full precision value of 'N' divided
; by Ft and then multiply continuously by 2 until we reach the biggest value less
; that 2^48. The number of multiplications by 2 indicating the value of 'y'.
;
; In this case we find that 'y' is 43.....
;       S = 21474836.48 x (2^43) / 1000000 = 21.47483648 x (2^43)
;                                          = 188894659314785.80854784
;
;  ...round to nearest integer and convert to hexadecimal S = ABCC77118462
;
; N will be taken from the 80 bit product by removing the 43 LSBs and the 5 MSBs
; to leave the 32 active bits required. This is best achieved by shifting left
; by 5 places (multiply by 2^5=32) and keeping the upper 32-bits.
;
;
; Sanity check....
;   Note that most calculators do not support >64 bit values to you will either
;   need to decompose your calculation and perform some of it manually or trust
;   the PicoBlaze implementation :-)
;
;     Ft = 1MHz =                         000F4240
;             S =                  x  ABCC77118462
;                             --------------------
;                             000A3D70A3D70A405C80
;
;     shift left 5 places                     x 20
;                             --------------------
;                             0147AE147AE1480B9000
;
; As expected, the most significant 32-bit (4 bytes) are 0147AE14 hex which is
; the DDS control word for 1MHz calculated previously.
;
; ***
;
; Now I will consider how this needs to be modified for the circuit presented
; which has a second DCM connected to the output of the phase accumulator to
; multiply the synthesized frequency and reduce cycle to cycle jitter at
; the same time. There is then a clock divider circuit connected to the output
; of the DCM which allows lower frequencies to be formed a different way (more of
; that later). As a minimum that divider circuit will divide by 2 which ensures that
; a square wave is presented to the clocked put pin. So in this circuit the fundamental
; multiplication factor is 8 formed by a 16 times multiplication by the DCM (256/16) and
; then a divide by 2.
;
; The overall multiplication factor of this sebsequent circuit means that for final
; output from the DCM to be the desired frequency, the output from the phase accumulator
; needs to be the same number of times smaller. This is not a bad thing because the
; percentage jitter of waveforms produced by the phase accumulator is better for lower
; frequencies made from more clock cycles.
;
; So we modify the basic equation to
;
;    Fout = Frequency at output of DCM
;       M = Multiplying factor of DCM
;
;    Fout = M x Fpa = M x clk x N / (2^p)
;
;
; By simple reorganisation of the equation we can compute 'N'
;
;       N = Fout x (2^p) / (clk x M)
;
;
; In this design M=8, p=32, clk=200MHz
;
; So now consider generating a nominal maximum frequency of 100MHz which will require
; the frequency synthesized by the phase accumulator to be 12.5MHz.
;
;       N = 100MHz x (2^32) / (200MHz x 8) = 268435456 = 10000000 Hex
;
; This all seems like a very convenient number but it simply reflects that 12.5MHz
; is a perfect division of the 200MHz clock and that that output from the phase
; accumulator will be formed perfectly of 16 of the 200MHz clock periods every time
; (8 Low and 8 High) with no additional jitter.
;
; So now we work out the scaling factor with the same rules as used previously that
; the scaling factor should be as large as possible within the 48-bits allocated.
;
;    S = N x (2^y) / Ft    with the condition that S < 2^48 but as large as possible
;
; In this case Ft = 100MHz = 055FE100 and the biggest value for S is found when using
; y=46
;
;    S = 268435456 x (2^46) / 100000000 = 2.68435456 x (2^46)
;                                       = 188894659314785.80854784
;
;      round to 188894659314786  = ABCC77118462
;
; Actually this is the exact same scaling constant as previously because the
; frequency to be synthesized by the phase accumulator is 8 times smaller but the
; value of 'S' is deliberate scaled to be as large as possible. In fact, 'S' in this
; case has been scaled up by a factor of 8 to arrive at the same value. So after
; using the scaling constant to form the 80 bit product, this time we will remove
; the 46 LSBs and the 2 MSBs to leave the 32 active bits required. This is best
; achieved by shifting left by 2 places (multiply by 2^2=4) and keeping the upper
; 32-bits (last time we multiplied by 32 which was 8 times more).
;
;
; Sanity check....
;
;     Ft = 100MHz =                         055FE100
;               S =                  x  ABCC77118462
;                               --------------------
;                               04000000000001242200
;
;       shift left 5 places                     x 20
;                               --------------------
;                               1000000000001C908800
;
; As expected, the most significant 32-bit (4 bytes) are 10000000 hex which is
; the DDS control word for 12.5MHz at the phase accumulator output calculated
; previously.
;
;
; ********
;
;
; 48-bit Scaling factor constant to generate the phase accumulator control word
; from the integer frequency value.
;
;   S = AB CC 77 11 84 62
;
; Notes
;
; The 80-bit product must be shifted left 5 times and then most significant 32-bits
; used to provide DDS control word if the frequency required is to be synthesized
; directly at the output of the phase accumulator.
;
; The 80-bit product must be shifted left 2 times and then most significant 32-bits
; used to provide DDS control word if the frequency required is to be synthesized
; by the phase accumulator followed by a multiplying DCM and divider with overall
; frequency gain of 8 times.
;
scale_constant0		EQU		0x62			;LS byte
scale_constant1		EQU		0x84
scale_constant2		EQU		0x11
scale_constant3		EQU		0x77
scale_constant4		EQU		0xcc
scale_constant5		EQU		0xab			;MS byte
;
;
;
; ************************
;
;Constant to define a software delay of 1us. This must be adjusted to reflect the
;clock applied to KCPSM3. Every instruction executes in 2 clock cycles making the
;calculation highly predictable. The '6' in the following equation even allows for
;'CALL delay_1us' instruction in the initiating code.
;
; delay_1us_constant =  (clock_rate - 6)/4       Where 'clock_rate' is in MHz
;
;Example: For a 50MHz clock the constant value is (10-6)/4 = 11  (0B Hex).
;For clock rates below 10MHz the value of 1 must be used and the operation will
;become lower than intended.
;
delay_1us_constant	EQU		0x0b
;
;
;
;ASCII table
;
character_a		EQU		0x61
character_b		EQU		0x62
character_c		EQU		0x63
character_d		EQU		0x64
character_e		EQU		0x65
character_f		EQU		0x66
character_g		EQU		0x67
character_h		EQU		0x68
character_i		EQU		0x69
character_j		EQU		0x6a
character_k		EQU		0x6b
character_l		EQU		0x6c
character_m		EQU		0x6d
character_n		EQU		0x6e
character_o		EQU		0x6f
character_p		EQU		0x70
character_q		EQU		0x71
character_r		EQU		0x72
character_s		EQU		0x73
character_t		EQU		0x74
character_u		EQU		0x75
character_v		EQU		0x76
character_w		EQU		0x77
character_x		EQU		0x78
character_y		EQU		0x79
character_z		EQU		0x7a
_character_a		EQU		0x41
_character_b		EQU		0x42
_character_c		EQU		0x43
_character_d		EQU		0x44
_character_e		EQU		0x45
_character_f		EQU		0x46
_character_g		EQU		0x47
_character_h		EQU		0x48
_character_i		EQU		0x49
_character_j		EQU		0x4a
_character_k		EQU		0x4b
_character_l		EQU		0x4c
_character_m		EQU		0x4d
_character_n		EQU		0x4e
_character_o		EQU		0x4f
_character_p		EQU		0x50
_character_q		EQU		0x51
_character_r		EQU		0x52
_character_s		EQU		0x53
_character_t		EQU		0x54
_character_u		EQU		0x55
_character_v		EQU		0x56
_character_w		EQU		0x57
_character_x		EQU		0x58
_character_y		EQU		0x59
_character_z		EQU		0x5a
character_0		EQU		0x30
character_1		EQU		0x31
character_2		EQU		0x32
character_3		EQU		0x33
character_4		EQU		0x34
character_5		EQU		0x35
character_6		EQU		0x36
character_7		EQU		0x37
character_8		EQU		0x38
character_9		EQU		0x39
character_colon		EQU		0x3a
character_stop		EQU		0x2e
character_semi_colon	EQU		0x3b
character_minus		EQU		0x2d
character_divide	EQU		0x2f			;'/'
character_plus		EQU		0x2b
character_comma		EQU		0x2c
character_less_than	EQU		0x3c
character_greater_than	EQU		0x3e
character_equals	EQU		0x3d
character_space		EQU		0x20
character_cr		EQU		0x0d			;carriage return
character_question	EQU		0x3f			;'?'
character_dollar	EQU		0x24
character_exclaim	EQU		0x21			;'!'
character_bs		EQU		0x08			;Back Space command character
;
;
;
;
;
;**************************************************************************************
;Initialise the system
;**************************************************************************************
;
cold_start:		CALL		lcd_reset		;initialise LCD display
;
;Write 'Frequency Generator' to LCD display and display for 4 seconds
;
			LOAD		s5, #0x10		;Line 1 position 0
			CALL		lcd_cursor
			CALL		disp_frequency
			LOAD		s5, #0x22		;Line 2 position 2
			CALL		lcd_cursor
			CALL		disp_generator
			CALL		delay_1s		;wait 4 seconds
			CALL		delay_1s
			CALL		delay_1s
			CALL		delay_1s
			CALL		lcd_clear		;clear screen
;
;
;Initial frequency of 100MHz
;
			LOAD		s0, #0x00
			LOAD		s1, #0x01
			STORE		s0, bcd_digit0
			STORE		s0, bcd_digit1
			STORE		s0, bcd_digit2
			STORE		s0, bcd_digit3
			STORE		s0, bcd_digit4
			STORE		s0, bcd_digit5
			STORE		s0, bcd_digit6
			STORE		s0, bcd_digit7
			STORE		s1, bcd_digit8
;
			LOAD		s0, #0x04		;Start position for editing frequency is 1MHz digit
			STORE		s0, cursor_position
			LOAD		s0, #bcd_digit6
			STORE		s0, edit_digit_pointer
;
;
			ENABLE		interrupt		;interrupts are used to detect rotary controller
			CALL		delay_1ms
			LOAD		s0, #0x00		;clear the status of any spurious rotary events
			STORE		s0, rotary_status	;   as a result of system turning on.
;
;**************************************************************************************
; Main program
;**************************************************************************************
;
; The main program is centred on the task of editing the frequency. It waits until the
; rotary control is used and then makes the appropriate changes. If the actual digit
; digit value is changed then the calculation to drive the DDS is performed each time.
;
; The start state is that of allowing the edit cursor position to be moved. Rotary
; inputs are detected by the interrupt service routine and set a flag bit which the
; main program then uses to adjust the cursor position and pointer to the corresponding
; BCD digit in memory.
;
; A press of the rotary control is detected by polling and used to change to the digit
; editing mode.
;
;
move_mode:		CALL		compute_dds_words	;compute DDS control values
			CALL		display_freq		;refresh display with cursor position shown
			LOAD		s0, #led0		;indicate move mode on LEDs
			OUTPUT		s0, led_port
move_wait:		INPUT		s0, rotary_port		;read rotary encoder
			TEST		s0, #rotary_press	;test for press of button which changes mode
			JUMP		nz, edit_mode
			FETCH		s0, rotary_status	;check for any rotation of rotary control
			TEST		s0, #rotary_event
			JUMP		z, move_wait
;
			AND		s0, #0x7f		;clear flag now that action it is being processed
			STORE		s0, rotary_status
			FETCH		sa, cursor_position	;read current position
			FETCH		sb, edit_digit_pointer
			TEST		s0, #rotary_left	;determine direction to move cursor
			JUMP		z, move_right
;
move_left:		COMPARE		sb, #bcd_digit8		;can not move left of 100MHz digit
			JUMP		z, move_mode
			ADD		sb, #0x01		;move to next higher BCD digit
			SUB		sa, #0x01		;move cursor to match digit to be edited
			COMPARE		sa, #0x09		;must skip over space separator
			JUMP		z, skip_left
			COMPARE		sa, #0x05		;must skip over decimal point
			JUMP		nz, edit_point_update
skip_left:		SUB		sa, #0x01		;move cursor further left
			JUMP		edit_point_update
;
move_right:		COMPARE		sb, #bcd_digit0		;can not move right of 1Hz digit
			JUMP		z, move_mode
			SUB		sb, #0x01		;move to next lower BCD digit
			ADD		sa, #0x01		;move cursor to match digit to be edited
			COMPARE		sa, #0x09		;must skip over space separator
			JUMP		z, skip_right
			COMPARE		sa, #0x05		;must skip over decimal point
			JUMP		nz, edit_point_update
skip_right:		ADD		sa, #0x01		;move cursor further right
;
edit_point_update:	STORE		sa, cursor_position	;update edit value in memory
			STORE		sb, edit_digit_pointer
			JUMP		move_mode
;
;
; The edit mode is reached by pressing the rotary control. Since this is a simple switch
; a software de-bounce delay is used to wait for the knob to be released fully before
; entering the digit editing mode fully.
;
; In this mode rotations of the detected by the interrupt service routine are used to
; increment or decrement the digit value at the cursor position with carry/borrow to
; the left.
;
; A new press of the rotary control is detected by polling and used to change back to the
; cursor moving mode.
;
;
edit_mode:		CALL		wait_switch_release	;wait for switch press to end
edit_display:		CALL		compute_dds_words	;compute DDS control values
			CALL		display_freq		;refresh display with new values
			LOAD		s0, #led1		;indicate edit mode on LEDs
			OUTPUT		s0, led_port
edit_wait:		INPUT		s0, rotary_port		;read rotary encoder
			TEST		s0, #rotary_press	;test for press of button which changes mode
			JUMP		nz, end_edit_mode
			FETCH		s0, rotary_status	;check for any rotation of rotary control
			TEST		s0, #rotary_event
			JUMP		z, edit_wait
;
			AND		s0, #0x7f		;clear flag now that action it is being processed
			STORE		s0, rotary_status
			FETCH		sb, edit_digit_pointer	;read pointer to BCD digit for initial change
			TEST		s0, #rotary_left	;determine direction to increment or decrement
			JUMP		z, inc_digit
;
; Decrement the value starting at the current position and borrowing from the left.
; However the value needs to bottom out at all 0's from the editing position.
;
;
dec_digit:		FETCH		sa, @sb			;read digit value at pointer position
			SUB		sa, #0x01		;decrement digit
			COMPARE		sa, #0xff		;test for borrow from next digit
			JUMP		z, dec_borrow
			STORE		sa, @sb			;store decremented digit value
			JUMP		edit_display		;decrement task complete
dec_borrow:		LOAD		sa, #0x09		;current digit rolls over to nine
			STORE		sa, @sb			;store '9' digit value
			COMPARE		sb, #bcd_digit8		;check if working on 100MHz digit
			JUMP		z, set_min_value
			ADD		sb, #0x01		;increment pointer to next most significant digit
			JUMP		dec_digit		;decrement next digit up.
;
set_min_value:		FETCH		sb, edit_digit_pointer	;Must fill digits from insert to MS-Digit with 000...
			LOAD		sa, #0x00
fill_min:		STORE		sa, @sb
			COMPARE		sb, #bcd_digit8		;check if filled to 100MHz digit
			JUMP		z, edit_display
			ADD		sb, #0x01		;fill next higher digit
			JUMP		fill_min
;
; Increment the value starting at the current position and carrying to the left.
; However the value needs to saturate to all 9's from the editing position.
;
inc_digit:		FETCH		sa, @sb			;read digit value at pointer position
			ADD		sa, #0x01		;increment digit
			COMPARE		sa, #0x0a		;test for carry to next digit
			JUMP		z, inc_carry
			STORE		sa, @sb			;store incremented digit value
			JUMP		edit_display		;increment task complete
inc_carry:		LOAD		sa, #0x00		;current digit rolls over to zero
			STORE		sa, @sb			;store zero digit value
			COMPARE		sb, #bcd_digit8		;check if working on 100MHz digit
			JUMP		z, set_max_value
			ADD		sb, #0x01		;increment pointer to next most significant digit
			JUMP		inc_digit		;increment next digit up.
;
set_max_value:		FETCH		sb, edit_digit_pointer	;Must fill digits from insert to MS-Digit with 999...
			LOAD		sa, #0x09
fill_max:		STORE		sa, @sb
			COMPARE		sb, #bcd_digit8		;check if filled to 100MHz digit
			JUMP		z, edit_display
			ADD		sb, #0x01		;fill next higher digit
			JUMP		fill_max
;
end_edit_mode:		CALL		wait_switch_release	;wait for end of switch press
			JUMP		move_mode		;then go to move cursor mode
;
;
; Routine to poll the press switch with de-bounce delay and wait for it to be
; released. Any rotation inputs detected by the interrupt
; service routine are cleared before returning.
;
wait_switch_release:	CALL		delay_20ms		;delay to aid switch de-bounce
			INPUT		s0, rotary_port		;read rotary encoder
			TEST		s0, #rotary_press	;test if button is still being pressed
			JUMP		nz, wait_switch_release
			LOAD		s0, #0x00		;clear flag indicating any rotary events
			STORE		s0, rotary_status
			RETURN
;
;**************************************************************************************
; Compute DDS control words from currently display frequency value
;**************************************************************************************
;
; This routine reads the current BCD value and converts it into a 32-bit binary
; integer. It then multiplies it by a 48-bit scaling factor (see notes in the
; constants section above) to form a full precision 80-bit product.
;
; From this product the 32-bit DDS control word must be extracted. For frequencies of
; 50MHz or above the DDS control word is formed by shifting the product left by 2 places
; (multiply by 4) and then keeping only the most significant 32-bits (4 bytes).
;
; Also for frequencies of 50MHz and above, there is no additional division performed
; after the DCM which multiplies frequency and reduces the jitter. Therefore the DDS_scaling
; word will be set to zero and the output of the DCM will divide by 2.
;
;   Freq     DDS control word    DDS Scaling      Synthesized Frequency
;                                                  of Phase Accumulator
;
;  50MHz      08000000              00                   6.25MHz
; 100MHz      10000000              00                  12.50MHz
;
; You will notice that for frequencies of 50MHz and above, the upper byte of the
; DDS control word is 08 hex or greater. In other words, bit3 and/or bit4 of that byte
; are High (bits 27 and/or 28 of the full 32-bit word). This is the indication that
; the control words are complete.
;
; For frequencies below 50MHz an additional process is required. The reason for this
; becomes clear if we think about the lowest frequency of 1Hz. In that case the 80-bit
; product is the same as the 48-bit scaling constant 00000000ABCC77118462. Once this
; has been multiplied by 4 (shifted left 2 places) it becomes 00000002AF31DC461188 and the
; most significant 32-bits are only 00000002 hex. If we put this back into the basic
; equations for the phase accumulator we find that the output frequency of the phase
; accumulator would be
;
;    Fout = M x clk x N / (2^p)
;
;         = 8 x 200MHz x 2 / (2^32) = 0.745 Hz
;
; There are two important observations we can make. Firstly we have lost accuracy because
; the resolution of the DDS control word has become too granular at low amplitudes.
; Secondly this would never even work because the frequency synthesized by the phase
; accumulator would be 0.745/8 = 0.0931 Hz which is seriously slow and a way below the
; frequency at which the DCM can even work.
;
; The solution to both of these issues is to ensure that the DDS control word is always
; formed to be in the range that would result in an output of 50MHz or above. In other
; words to keep the phase accumulator output in the range 6.25MHz to 12.5MHz such that
; the DCM is able to work and only has to deal with one octave of input variation. This
; can be achieved by shifting the 80-bit product left more times until bits 27 and 28
; of the most significant 32-bits are not zero.
;
; For each shift left the synthesized frequency is being doubled and therefore the final
; output from the DCM must be divided by a further factor of 2. This is achieved using
; a multiplexer which is guided to select the appropriate output from a simple binary
; counter.
;
; Returning to the example of 1Hz, the 80-bit product will be shifted left by the default
; 2 places (multiplied by 4), but will then need to be shifted left by a further 26 places
; which is like multiplying by 67108864 (04000000 hex).
;
;                            00000000ABCC77118462
;                          x                    4
;                            --------------------
;                            00000002AF31DC461188
;
;
;                          x             04000000
;                            --------------------
;                            0ABCC771184620000000
;
; So now the DDS control word is 0ABCC771 (180143985 decimal)and the frequency synthesized
; by the phase accumulator will be....
;
;   Fpa = clk x N / (2^p) = 200MHz x 180143985 / (2^32) = 8388608Hz
;
; The DCM will multiply this by a factor of 16 to give 134217728Hz and this will then
; be divided by the counter of which the 26th bit selected (26 decimal = 1A hex).
;
;   Fout = Fpa x 16 / (2^(D+1)) =  8388608Hz x 16 / (2^(26+1)) = 0.99999999947 Hz
;
;     'D' is the DDS Scaling factor
;         Note that bit0 of a counter is a clock division by 2 and hence the 'D+1'
;
; Clearly this implementation style has provided much greater accuracy and enables
; the DCM to work for all desired frequencies.
;
;
;   Freq     DDS control word    DDS Scaling      Synthesized Frequency
;                                                  of Phase Accumulator
;
; 100 MHz      10000000              00                  12.50MHz
;  50 MHz      08000000              00                   6.25MHz
;  25 MHz      08000000              01                   6.25MHz
;  12.5 MHz    08000000              02                   6.25MHz
;
;    1Hz       0ABCC771              1A                   8.388608 MHz
;
;
;
; In order to ensure the DCM is always provided with a frequency in an acceptable
; range, the value of absolute zero is never implemented and instead just a very low
; frequency is produced.
;   6.25MHz x 16 / (2^31+1) = 0.0233 Hz
;       which is 1 cycle every 43 seconds and that is pretty slow  :-)
;
;
;
;
compute_dds_words:	CALL		bcd_to_integer		;convert BCD display value to 32-bit value
			CALL		scale_frequency		;80-bit product of 32-bit frequency x 48-bit scaling value
			FETCH		sa, product9		;read the upper part of the 80-bit product into [sA,s9,s8,s7,s6,s5,s4]
			FETCH		s9, product8		; The least significant 24-bits of the 80-bit product will never
			FETCH		s8, product7		; be used for frequencies above 1Hz.
			FETCH		s7, product6		;The final 32-bit DDS control word will be formed in
			FETCH		s6, product5		; [sA,s9,s8,s7]
			FETCH		s5, product4
			FETCH		s4, product3
			CALL		shift80_left		;multiply DDS control word by 4 to achieve default value
			CALL		shift80_left
			LOAD		sb, #0x00		;default scaling factor is 2 (select counter bit0)
normalise_loop:		TEST		sa, #0x18		;Test bits 27 and 28 of 32-bit DDS control word
			JUMP		nz, store_dds_words	;DDS control word is normalised to above 50MHz output
			CALL		shift80_left		;multiply DDS control word by 2
			ADD		sb, #0x01		;Divide final value by 2 to compensate
			COMPARE		sb, #0x1f		;Test for maximum division factor
			JUMP		nz, normalise_loop
			LOAD		sa, #0x08		;Set for minimum frequency
			LOAD		s9, #0x00		; with phase accumulator set to generate 6.25MHz
			LOAD		s8, #0x00
			LOAD		s7, #0x00
store_dds_words:	STORE		s7, dds_control0	;store local copy of control word
			STORE		s8, dds_control1	;store local copy of control word
			STORE		s9, dds_control2	;store local copy of control word
			STORE		sa, dds_control3	;store local copy of control word
			STORE		sb, dds_scaling
			CALL		drive_dds_words		;output control words to DDS circuit
			RETURN
;
shift80_left:		SL0		s4			;shift (most of the) 80-bit value in
			SLA		s5			;  [sA,s9,s8,s7,s6,s5,s4] left 1 place
			SLA		s6
			SLA		s7
			SLA		s8
			SLA		s9
			SLA		sa
			RETURN
;
;**************************************************************************************
; Set DDS control words
;**************************************************************************************
;
; Because multiple ports are used, the idea is to update all of them in
; rapid succession to avoid too much disturbance in the frequency synthesis.
;
; dds_control_word should be supplied in register set [sA,s9,s8,s7]
; dds_scaling_word should be supplied in register s6.
;
drive_dds_words:	FETCH		s7, dds_control0
			FETCH		s8, dds_control1
			FETCH		s9, dds_control2
			FETCH		sa, dds_control3
			FETCH		s6, dds_scaling
			OUTPUT		s7, dds_control0_port
			OUTPUT		s8, dds_control1_port
			OUTPUT		s9, dds_control2_port
			OUTPUT		sa, dds_control3_port
			OUTPUT		s6, dds_scaling_port
			RETURN
;
;
;**************************************************************************************
; Display frequency on top line of the LCD and DDS data on the lower line
;**************************************************************************************
;
; The BCD value should be stored in scratch pad memory in 9 ascending locations
; called BCD_digit0 to BCD_digit8.
;
; The value is displayed in the format      xxx.xxx xxxMHz
;
; However, the most significant 2 digits will be blanked if zero.
;
; registers used s0,s1,s2,s3,s4,s5,s6,s7
;
;
display_freq:		CALL		display_dds_data	;display DDS information on lower line
			LOAD		s5, #0x12		;Line 1 position 2
			CALL		lcd_cursor
			FETCH		s5, bcd_digit8		;read 100MHz digit
			COMPARE		s5, #0x00		;test for blanking
			JUMP		z, blank_100m_digit
			CALL		display_digit		;display non zero digit
			FETCH		s5, bcd_digit7		;read 10MHz digit and display
			CALL		display_digit
			JUMP		disp_1m_digit
;
blank_100m_digit:	CALL		display_space		;blank 100MHz digit
			FETCH		s5, bcd_digit7		;read 10MHz digit
			COMPARE		s5, #0x00		;test for blanking
			JUMP		z, blank_10m_digit
			CALL		display_digit		;display non zero digit
			JUMP		disp_1m_digit
;
blank_10m_digit:	CALL		display_space		;blank 10MHz digit
;
disp_1m_digit:		FETCH		s5, bcd_digit6		;read 1MHz digit and display
			CALL		display_digit
			LOAD		s5, #character_stop	;display decimal point
			CALL		lcd_write_data
;
			LOAD		s2, #bcd_digit5		;set pointer to 100KHz digit
			CALL		display_3_digits
			CALL		display_space
			LOAD		s2, #bcd_digit2		;set pointer to 100Hz digit
			CALL		display_3_digits
			LOAD		s5, #_character_m	;display 'MHz'
			CALL		lcd_write_data
			LOAD		s5, #_character_h
			CALL		lcd_write_data
			LOAD		s5, #character_z
			CALL		lcd_write_data
;
			FETCH		s5, cursor_position	;reposition edit cursor on display
			ADD		s5, #0x10		;on line 1
			CALL		lcd_cursor
			RETURN
;
display_3_digits:	LOAD		s3, #0x03		;3 digits to display
_3digit_loop:		FETCH		s5, @s2
			CALL		display_digit
			SUB		s2, #0x01		;decrement digit pointer
			SUB		s3, #0x01		;count digits displayed
			JUMP		nz, _3digit_loop
			RETURN
;
display_digit:		ADD		s5, #0x30		;convert BCD to ASCII character
			CALL		lcd_write_data
			RETURN
;
display_space:		LOAD		s5, #character_space
			CALL		lcd_write_data
			RETURN
;
;
;**************************************************************************************
; Convert 9 digit BCD frequency into 32-bit binary integer
;**************************************************************************************
;
;Both values are stored in scratch pad memory
;    BCD values in ascending locations BCD_digit0 to BCD_digit8
;    Binary frequency in ascending locations frequency0 to frequency3
;
;Each digit is read in turn and its value is determined by repeated
;decrement until reaching zero. Each decrement causes a value to be added
;to the memory locations forming the frequency value as binary integer.
;The process requires approximately 1600 instructions to convert the highest
;value 999,999,999 which is approximately 64us at 50MHz clock rate.
;
;Registers used s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,sA,sB
;
bcd_to_integer:		LOAD		s2, #0x09		;9 digits to convert
			LOAD		s0, #0x00		;clear frequency value ready to accumulate result
			STORE		s0, frequency0
			STORE		s0, frequency1
			STORE		s0, frequency2
			STORE		s0, frequency3
			LOAD		sb, #0x00		;initialise BCD digit weighting [sB,sA,s9,s8] to 1
			LOAD		sa, #0x00
			LOAD		s9, #0x00
			LOAD		s8, #0x01
			LOAD		s3, #bcd_digit0		;locate LS-digit
next_bcd_to_int_digit:	FETCH		s1, @s3
bcd_digit_convert:	COMPARE		s1, #0x00		;test for zero
			JUMP		z, next_digit_value
			FETCH		s0, frequency0		;add 32-bit digit weighting to memory value
			ADD		s0, s8
			STORE		s0, frequency0
			FETCH		s0, frequency1
			ADDCY		s0, s9
			STORE		s0, frequency1
			FETCH		s0, frequency2
			ADDCY		s0, sa
			STORE		s0, frequency2
			FETCH		s0, frequency3
			ADDCY		s0, sb
			STORE		s0, frequency3
			SUB		s1, #0x01		;decrement digit value
			JUMP		bcd_digit_convert
;Increase weighting by 10x
next_digit_value:	LOAD		s7, sb			;copy existing weighting
			LOAD		s6, sa
			LOAD		s5, s9
			LOAD		s4, s8
			SL0		s8			;multiply weight by 4x (shift left 2 places)
			SLA		s9
			SLA		sa
			SLA		sb
			SL0		s8
			SLA		s9
			SLA		sa
			SLA		sb
			ADD		s8, s4			;add previous weight to form 5x multiplication
			ADDCY		s9, s5
			ADDCY		sa, s6
			ADDCY		sb, s7
			SL0		s8			;multiply weight by 2x (shift left 1 places)
			SLA		s9
			SLA		sa
			SLA		sb			;weight value is now 10x previous value
			ADD		s3, #0x01		;move to next digit for conversion
			SUB		s2, #0x01
			JUMP		nz, next_bcd_to_int_digit
			RETURN
;
;
;**************************************************************************************
; 32-bit x 48-bit multiply to scale the integer frequency
;**************************************************************************************
;
;Multiply the 32-bit frequency binary integer by the 48-bit scaling factor
;to form a full precision 80-bit product.
;
;The frequency binary integer is stored in scratch pad memory using ascending
;locations frequency0 to frequency3
;
;The product will be stored in scratch pad memory using ascending
;locations product0 to product9
;
;The scaling factor is provided directly as constants
; scale_constant0 to scale_constant5
;
;The multiplication is performed as a 32-bit 'shift and add' process in which the
;integer frequency is examined LSB first using a register set [sB,sA,s9,s8] and
;a scaling accumulator is formed directly in the 'product' memory locations.
;
;The process requires up to 1772 instructions which is 3544 clock cycle or
;approximately 71us at 50MHz clock rate.
;
;Registers used s0,s1,s8,s9,sA,sB (s1,s8,s9,sA,sB clear on return)
;
scale_frequency:	LOAD		s0, #0x00		;clear accumulator section of 'product'
			STORE		s0, product9
			STORE		s0, product8
			STORE		s0, product7
			STORE		s0, product6
			STORE		s0, product5
			STORE		s0, product4
			FETCH		sb, frequency3		;read frequency integer value
			FETCH		sa, frequency2
			FETCH		s9, frequency1
			FETCH		s8, frequency0
			LOAD		s1, #0x20		;32-bit multiply
scale_mult_bit:		SR0		sb			;shift right frequency integer
			SRA		sa
			SRA		s9
			SRA		s8
			JUMP		nc, product_shift	;no add if bit is zero (note carry is zero)
			FETCH		s0, product4		;addition of scaling factor to most significant bits of product
			ADD		s0, #scale_constant0
			STORE		s0, product4
			FETCH		s0, product5
			ADDCY		s0, #scale_constant1
			STORE		s0, product5
			FETCH		s0, product6
			ADDCY		s0, #scale_constant2
			STORE		s0, product6
			FETCH		s0, product7
			ADDCY		s0, #scale_constant3
			STORE		s0, product7
			FETCH		s0, product8
			ADDCY		s0, #scale_constant4
			STORE		s0, product8
			FETCH		s0, product9
			ADDCY		s0, #scale_constant5
			STORE		s0, product9		;carry holds any overflow of addition
product_shift:		FETCH		s0, product9		;Divide product by 2 (shift right by 1)
			SRA		s0			;overflow of addition included in shift
			STORE		s0, product9
			FETCH		s0, product8
			SRA		s0
			STORE		s0, product8
			FETCH		s0, product7
			SRA		s0
			STORE		s0, product7
			FETCH		s0, product6
			SRA		s0
			STORE		s0, product6
			FETCH		s0, product5
			SRA		s0
			STORE		s0, product5
			FETCH		s0, product4
			SRA		s0
			STORE		s0, product4
			FETCH		s0, product3
			SRA		s0
			STORE		s0, product3
			FETCH		s0, product2
			SRA		s0
			STORE		s0, product2
			FETCH		s0, product1
			SRA		s0
			STORE		s0, product1
			FETCH		s0, product0
			SRA		s0
			STORE		s0, product0
			SUB		s1, #0x01		;move to next bit
			JUMP		nz, scale_mult_bit
			RETURN
;
;**************************************************************************************
; Display DDS control information on the lower line of the LCD display.
;**************************************************************************************
;
;Display the 32-bit DDS control word and 8-bit DDS scaling word.
;
display_dds_data:	LOAD		s5, #0x20		;Line 2 position 0
			CALL		lcd_cursor
			LOAD		s5, #_character_n
			CALL		lcd_write_data
			LOAD		s5, #character_equals
			CALL		lcd_write_data
			LOAD		s7, #dds_control3	;pointer to most significant byte in memory
			CALL		display_hex_32_bit
			CALL		display_space
			LOAD		s5, #_character_d
			CALL		lcd_write_data
			LOAD		s5, #character_equals
			CALL		lcd_write_data
			FETCH		s0, dds_scaling
			CALL		display_hex_byte
			RETURN
;
;**************************************************************************************
; Routines to display hexadecimal values on LCD display
;**************************************************************************************
;
;
; Convert hexadecimal value provided in register s0 into ASCII characters
;
; The value provided must can be any value in the range 00 to FF and will be converted into
; two ASCII characters.
;     The upper nibble will be represented by an ASCII character returned in register s3.
;     The lower nibble will be represented by an ASCII character returned in register s2.
;
; The ASCII representations of '0' to '9' are 30 to 39 hexadecimal which is simply 30 hex
; added to the actual decimal value. The ASCII representations of 'A' to 'F' are 41 to 46
; hexadecimal requiring a further addition of 07 to the 30 already added.
;
; Registers used s0, s2 and s3.
;
hex_byte_to_ascii:	LOAD		s2, s0			;remember value supplied
			SR0		s0			;isolate upper nibble
			SR0		s0
			SR0		s0
			SR0		s0
			CALL		hex_to_ascii		;convert
			LOAD		s3, s0			;upper nibble value in s3
			LOAD		s0, s2			;restore complete value
			AND		s0, #0x0f		;isolate lower nibble
			CALL		hex_to_ascii		;convert
			LOAD		s2, s0			;lower nibble value in s2
			RETURN
;
; Convert hexadecimal value provided in register s0 into ASCII character
;
;Register used s0
;
hex_to_ascii:		SUB		s0, #0x0a		;test if value is in range 0 to 9
			JUMP		c, number_char
			ADD		s0, #0x07		;ASCII char A to F in range 41 to 46
number_char:		ADD		s0, #0x3a		;ASCII char 0 to 9 in range 30 to 40
			RETURN
;
;
; Display the two character HEX value of the register contents 's0' on the LCD
; at the current cursor position.
;
; Registers used s0, s1, s2, s3, s4, s5
;
display_hex_byte:	CALL		hex_byte_to_ascii
			LOAD		s5, s3
			CALL		lcd_write_data
			LOAD		s5, s2
			CALL		lcd_write_data
			RETURN
;
;
;
; Display the 32-bit value stored in 4 ascending memory locations as an 8 character
; HEX value at the current cursor position. Register s7 must contain the memory
; location of the most significant byte (which is also the highest address).
;
; Registers used s0, s1, s2, s3, s4, s5, s6, s7
;
display_hex_32_bit:	LOAD		s6, #0x04		;4 bytes to display
disp32_loop:		FETCH		s0, @s7			;read byte
			CALL		display_hex_byte	;display byte
			SUB		s7, #0x01		;decrement pointer
			SUB		s6, #0x01		;count bytes displayed
			RETURN		z
			JUMP		disp32_loop
;
;
;**************************************************************************************
;LCD text messages
;**************************************************************************************
;
;
;Display 'Frequency' on LCD at current cursor position
;
disp_frequency:		LOAD		s5, #_character_f
			CALL		lcd_write_data
			LOAD		s5, #character_r
			CALL		lcd_write_data
			LOAD		s5, #character_e
			CALL		lcd_write_data
			LOAD		s5, #character_q
			CALL		lcd_write_data
			LOAD		s5, #character_u
			CALL		lcd_write_data
			LOAD		s5, #character_e
			CALL		lcd_write_data
			LOAD		s5, #character_n
			CALL		lcd_write_data
			LOAD		s5, #character_c
			CALL		lcd_write_data
			LOAD		s5, #character_y
			CALL		lcd_write_data
			RETURN
;
;Display 'Generator' on LCD at current cursor position
;
disp_generator:		LOAD		s5, #_character_g
			CALL		lcd_write_data
			LOAD		s5, #character_e
			CALL		lcd_write_data
			LOAD		s5, #character_n
			CALL		lcd_write_data
			LOAD		s5, #character_e
			CALL		lcd_write_data
			LOAD		s5, #character_r
			CALL		lcd_write_data
			LOAD		s5, #character_a
			CALL		lcd_write_data
			LOAD		s5, #character_t
			CALL		lcd_write_data
			LOAD		s5, #character_o
			CALL		lcd_write_data
			LOAD		s5, #character_r
			CALL		lcd_write_data
			CALL		display_space
			LOAD		s5, #character_v
			CALL		lcd_write_data
			LOAD		s5, #character_1
			CALL		lcd_write_data
			LOAD		s5, #character_stop
			CALL		lcd_write_data
			LOAD		s5, #character_2
			CALL		lcd_write_data
			RETURN
;
;
;
;
;**************************************************************************************
;Software delay routines
;**************************************************************************************
;
;
;
;Delay of 1us.
;
;Constant value defines reflects the clock applied to KCPSM3. Every instruction
;executes in 2 clock cycles making the calculation highly predictable. The '6' in
;the following equation even allows for 'CALL delay_1us' instruction in the initiating code.
;
; delay_1us_constant =  (clock_rate - 6)/4       Where 'clock_rate' is in MHz
;
;Registers used s0
;
delay_1us:		LOAD		s0, #delay_1us_constant
wait_1us:		SUB		s0, #0x01
			JUMP		nz, wait_1us
			RETURN
;
;Delay of 40us.
;
;Registers used s0, s1
;
delay_40us:		LOAD		s1, #0x28		;40 x 1us = 40us
wait_40us:		CALL		delay_1us
			SUB		s1, #0x01
			JUMP		nz, wait_40us
			RETURN
;
;
;Delay of 1ms.
;
;Registers used s0, s1, s2
;
delay_1ms:		LOAD		s2, #0x19		;25 x 40us = 1ms
wait_1ms:		CALL		delay_40us
			SUB		s2, #0x01
			JUMP		nz, wait_1ms
			RETURN
;
;Delay of 20ms.
;
;Delay of 20ms used during initialisation.
;
;Registers used s0, s1, s2, s3
;
delay_20ms:		LOAD		s3, #0x14		;20 x 1ms = 20ms
wait_20ms:		CALL		delay_1ms
			SUB		s3, #0x01
			JUMP		nz, wait_20ms
			RETURN
;
;Delay of approximately 1 second.
;
;Registers used s0, s1, s2, s3, s4
;
delay_1s:		LOAD		s4, #0x32		;50 x 20ms = 1000ms
wait_1s:		CALL		delay_20ms
			SUB		s4, #0x01
			JUMP		nz, wait_1s
			RETURN
;
;
;
;**************************************************************************************
;LCD Character Module Routines
;**************************************************************************************
;
;LCD module is a 16 character by 2 line display but all displays are very similar
;The 4-wire data interface will be used (DB4 to DB7).
;
;The LCD modules are relatively slow and software delay loops are used to slow down
;KCPSM3 adequately for the LCD to communicate. The delay routines are provided in
;a different section (see above in this case).
;
;
;Pulse LCD enable signal 'E' high for greater than 230ns (1us is used).
;
;Register s4 should define the current state of the LCD output port.
;
;Registers used s0, s4
;
lcd_pulse_e:		XOR		s4, #lcd_e		;E=1
			OUTPUT		s4, lcd_output_port
			CALL		delay_1us
			XOR		s4, #lcd_e		;E=0
			OUTPUT		s4, lcd_output_port
			RETURN
;
;Write 4-bit instruction to LCD display.
;
;The 4-bit instruction should be provided in the upper 4-bits of register s4.
;Note that this routine does not release the master enable but as it is only
;used during initialisation and as part of the 8-bit instruction write it
;should be acceptable.
;
;Registers used s4
;
lcd_write_inst4:	AND		s4, #0xf8		;Enable=1 RS=0 Instruction, RW=0 Write, E=0
			OUTPUT		s4, lcd_output_port	;set up RS and RW >40ns before enable pulse
			CALL		lcd_pulse_e
			RETURN
;
;
;Write 8-bit instruction to LCD display.
;
;The 8-bit instruction should be provided in register s5.
;Instructions are written using the following sequence
; Upper nibble
; wait >1us
; Lower nibble
; wait >40us
;
;Registers used s0, s1, s4, s5
;
lcd_write_inst8:	LOAD		s4, s5
			AND		s4, #0xf0		;Enable=0 RS=0 Instruction, RW=0 Write, E=0
			OR		s4, #lcd_drive		;Enable=1
			CALL		lcd_write_inst4		;write upper nibble
			CALL		delay_1us		;wait >1us
			LOAD		s4, s5			;select lower nibble with
			SL1		s4			;Enable=1
			SL0		s4			;RS=0 Instruction
			SL0		s4			;RW=0 Write
			SL0		s4			;E=0
			CALL		lcd_write_inst4		;write lower nibble
			CALL		delay_40us		;wait >40us
			LOAD		s4, #0xf0		;Enable=0 RS=0 Instruction, RW=0 Write, E=0
			OUTPUT		s4, lcd_output_port	;Release master enable
			RETURN
;
;
;
;Write 8-bit data to LCD display.
;
;The 8-bit data should be provided in register s5.
;Data bytes are written using the following sequence
; Upper nibble
; wait >1us
; Lower nibble
; wait >40us
;
;Registers used s0, s1, s4, s5
;
lcd_write_data:		LOAD		s4, s5
			AND		s4, #0xf0		;Enable=0 RS=0 Instruction, RW=0 Write, E=0
			OR		s4, #0x0c		;Enable=1 RS=1 Data, RW=0 Write, E=0
			OUTPUT		s4, lcd_output_port	;set up RS and RW >40ns before enable pulse
			CALL		lcd_pulse_e		;write upper nibble
			CALL		delay_1us		;wait >1us
			LOAD		s4, s5			;select lower nibble with
			SL1		s4			;Enable=1
			SL1		s4			;RS=1 Data
			SL0		s4			;RW=0 Write
			SL0		s4			;E=0
			OUTPUT		s4, lcd_output_port	;set up RS and RW >40ns before enable pulse
			CALL		lcd_pulse_e		;write lower nibble
			CALL		delay_40us		;wait >40us
			LOAD		s4, #0xf0		;Enable=0 RS=0 Instruction, RW=0 Write, E=0
			OUTPUT		s4, lcd_output_port	;Release master enable
			RETURN
;
;
;
;
;Read 8-bit data from LCD display.
;
;The 8-bit data will be read from the current LCD memory address
;and will be returned in register s5.
;It is advisable to set the LCD address (cursor position) before
;using the data read for the first time otherwise the display may
;generate invalid data on the first read.
;
;Data bytes are read using the following sequence
; Upper nibble
; wait >1us
; Lower nibble
; wait >40us
;
;Registers used s0, s1, s4, s5
;
lcd_read_data8:		LOAD		s4, #0x0e		;Enable=1 RS=1 Data, RW=1 Read, E=0
			OUTPUT		s4, lcd_output_port	;set up RS and RW >40ns before enable pulse
			XOR		s4, #lcd_e		;E=1
			OUTPUT		s4, lcd_output_port
			CALL		delay_1us		;wait >260ns to access data
			INPUT		s5, lcd_input_port	;read upper nibble
			XOR		s4, #lcd_e		;E=0
			OUTPUT		s4, lcd_output_port
			CALL		delay_1us		;wait >1us
			XOR		s4, #lcd_e		;E=1
			OUTPUT		s4, lcd_output_port
			CALL		delay_1us		;wait >260ns to access data
			INPUT		s0, lcd_input_port	;read lower nibble
			XOR		s4, #lcd_e		;E=0
			OUTPUT		s4, lcd_output_port
			AND		s5, #0xf0		;merge upper and lower nibbles
			SR0		s0
			SR0		s0
			SR0		s0
			SR0		s0
			OR		s5, s0
			LOAD		s4, #0x04		;Enable=0 RS=1 Data, RW=0 Write, E=0
			OUTPUT		s4, lcd_output_port	;Stop reading 5V device and release master enable
			CALL		delay_40us		;wait >40us
			RETURN
;
;
;Reset and initialise display to communicate using 4-bit data mode
;Includes routine to clear the display.
;
;Requires the 4-bit instructions 3,3,3,2 to be sent with suitable delays
;following by the 8-bit instructions to set up the display.
;
;  28 = '001' Function set, '0' 4-bit mode, '1' 2-line, '0' 5x7 dot matrix, 'xx'
;  06 = '000001' Entry mode, '1' increment, '0' no display shift
;  0E = '00001' Display control, '1' display on, '1' cursor off, '0' cursor blink off
;  01 = '00000001' Display clear
;
;Registers used s0, s1, s2, s3, s4
;
lcd_reset:		CALL		delay_20ms		;wait more that 15ms for display to be ready
			LOAD		s4, #0x30
			CALL		lcd_write_inst4		;send '3'
			CALL		delay_20ms		;wait >4.1ms
			CALL		lcd_write_inst4		;send '3'
			CALL		delay_1ms		;wait >100us
			CALL		lcd_write_inst4		;send '3'
			CALL		delay_40us		;wait >40us
			LOAD		s4, #0x20
			CALL		lcd_write_inst4		;send '2'
			CALL		delay_40us		;wait >40us
			LOAD		s5, #0x28		;Function set
			CALL		lcd_write_inst8
			LOAD		s5, #0x06		;Entry mode
			CALL		lcd_write_inst8
			LOAD		s5, #0x0e		;Display control
			CALL		lcd_write_inst8
lcd_clear:		LOAD		s5, #0x01		;Display clear
			CALL		lcd_write_inst8
			CALL		delay_1ms		;wait >1.64ms for display to clear
			CALL		delay_1ms
			RETURN
;
;Position the cursor ready for characters to be written.
;The display is formed of 2 lines of 16 characters and each
;position has a corresponding address as indicated below.
;
;                   Character position
;           0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
;
; Line 1 - 80 81 82 83 84 85 86 87 88 89 8A 8B 8C 8D 8E 8F
; Line 2 - C0 C1 C2 C3 C4 C5 C6 C7 C8 C9 CA CB CC CD CE CF
;
;This routine will set the cursor position using the value provided
;in register s5. The upper nibble will define the line and the lower
;nibble the character position on the line.
; Example s5 = 2B will position the cursor on line 2 position 11
;
;Registers used s0, s1, s2, s3, s4
;
lcd_cursor:		TEST		s5, #0x10		;test for line 1
			JUMP		z, set_line2
			AND		s5, #0x0f		;make address in range 80 to 8F for line 1
			OR		s5, #0x80
			CALL		lcd_write_inst8		;instruction write to set cursor
			RETURN
set_line2:		AND		s5, #0x0f		;make address in range C0 to CF for line 2
			OR		s5, #0xc0
			CALL		lcd_write_inst8		;instruction write to set cursor
			RETURN
;
;This routine will shift the complete display one position to the left.
;The cursor position and LCD memory contents will not change.
;
;
;Registers used s0, s1, s2, s3, s4, s5
;
lcd_shift_left:		LOAD		s5, #0x18		;shift display left
			CALL		lcd_write_inst8
			RETURN
;
;**************************************************************************************
;Interrupt Service Routine (ISR)
;**************************************************************************************
;
;Interrupts occur when the rotary control has been moved.
;
;The ISR captures the state of the direction which it writes to scratch pad memory (SPM).
;The most significant bit is also set at this location to provide a 'flag' to the
;main body of the program.
;
;
isr:			STORE		s0, isr_preserve_s0	;preserve s0
			INPUT		s0, rotary_port		;read rotary encoder
			OR		s0, #rotary_event	;set flag
			STORE		s0, rotary_status	;put result in SCM
			FETCH		s0, isr_preserve_s0	;restore s0
			RETURNI		enable
;
;
;**************************************************************************************
;Interrupt Vector
;**************************************************************************************
;
			ORG		0x3ff
			JUMP		isr
;
;

