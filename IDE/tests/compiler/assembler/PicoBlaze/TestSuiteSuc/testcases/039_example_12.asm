                     ; KCPSM3 Program - Pulse Width Modulation (PWM) Control on the Spartan-3E Starter Kit.
                     ;
                     ; Ken Chapman - Xilinx Ltd
                     ;
                     ; Version v1.00 - 22nd May 2006
                     ;
                                         device kcpsm6
                    ; warning > added only for testing purposes
                     ; Provides control for 12 channels of PWM with a Pulse Repetition Frequency (PRF) of 1KHz
                     ; and an 8-bit duty cycle resolution (256 steps). Control is provided for each channel
                     ; via the UART interface to the PC running HyperTerminal or similar to enter simple text
                     ; commands.
                     ;
                     ;**************************************************************************************
                     ; Port definitions
                     ;**************************************************************************************
                     ;
                     ;
                     ;
                     CONSTANT LED_port, 80                  ;8 simple LEDs
                     CONSTANT LED0, 01                      ;     LED 0 - bit0
                     CONSTANT LED1, 02                      ;         1 - bit1
                     CONSTANT LED2, 04                      ;         2 - bit2
                     CONSTANT LED3, 08                      ;         3 - bit3
                     CONSTANT LED4, 10                      ;         4 - bit4
                     CONSTANT LED5, 20                      ;         5 - bit5
                     CONSTANT LED6, 40                      ;         6 - bit6
                     CONSTANT LED7, 80                      ;         7 - bit7
                     ;
                     ;
                     CONSTANT simple_port, 40               ;4 simple outputs
                     CONSTANT simple_IO9, 01                ;  Header  IO9  - bit0
                     CONSTANT simple_IO10, 02               ;          IO10 - bit1
                     CONSTANT simple_IO11, 04               ;          IO11 - bit2
                     CONSTANT simple_IO12, 08               ;          IO12 - bit3
                     ;
                     ;
                     ;
                     CONSTANT status_port, 00               ;UART status input
                     CONSTANT tx_half_full, 01              ;  Transmitter     half full - bit0
                     CONSTANT tx_full, 02                   ;    FIFO               full - bit1
                     CONSTANT rx_data_present, 04           ;  Receiver     data present - bit2
                     CONSTANT rx_half_full, 08              ;    FIFO          half full - bit3
                     CONSTANT rx_full, 10                   ;                   full - bit4
                     CONSTANT spare1, 20                    ;                  spare '0' - bit5
                     CONSTANT spare2, 40                    ;                  spare '0' - bit6
                     CONSTANT spare3, 80                    ;                  spare '0' - bit7
                     ;
                     CONSTANT UART_read_port, 01            ;UART Rx data input
                     ;
                     CONSTANT UART_write_port, 20           ;UART Tx data output
                     ;
                     ;
                     ;
                     ;**************************************************************************************
                     ; Special Register usage
                     ;**************************************************************************************
                     ;
                     NAMEREG sF, UART_data                  ;used to pass data to and from the UART
                     ;
                     ;
                     ;
                     ;**************************************************************************************
                     ;Scratch Pad Memory Locations
                     ;**************************************************************************************
                     ;
                     CONSTANT PWM_duty_counter, 00          ;Duty Counter 0 to 255 within 1KHz period (1ms)
                     CONSTANT PWM_channel0, 01              ;PWM settings for each channel
                     CONSTANT PWM_channel1, 02              ; Channels 0 to 7 = LEDs 0 to 7
                     CONSTANT PWM_channel2, 03              ; Channels 8 to 11 = IO9 to IO12
                     CONSTANT PWM_channel3, 04
                     CONSTANT PWM_channel4, 05
                     CONSTANT PWM_channel5, 06
                     CONSTANT PWM_channel6, 07
                     CONSTANT PWM_channel7, 08
                     CONSTANT PWM_channel8, 09
                     CONSTANT PWM_channel9, 0A
                     CONSTANT PWM_channel10, 0B
                     CONSTANT PWM_channel11, 0C
                     CONSTANT ISR_preserve_s0, 0D           ;preserve register contents during Interrupt Service Routine
                     CONSTANT ISR_preserve_s1, 0E
                     CONSTANT ISR_preserve_s2, 0F
                     ;
                     ;
                     ;
                     ;
                     ;UART character strings will be stored in scratch pad memory ending in carriage return.
                     ;A string can be up to 16 characters with the start location defined by this constant.
                     ;
                     CONSTANT string_start, 30
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
                     ;ASCII table
                     ;
                     CONSTANT character_a, 61
                     CONSTANT character_b, 62
                     CONSTANT character_c, 63
                     CONSTANT character_d, 64
                     CONSTANT character_e, 65
                     CONSTANT character_f, 66
                     CONSTANT character_g, 67
                     CONSTANT character_h, 68
                     CONSTANT character_i, 69
                     CONSTANT character_j, 6A
                     CONSTANT character_k, 6B
                     CONSTANT character_l, 6C
                     CONSTANT character_m, 6D
                     CONSTANT character_n, 6E
                     CONSTANT character_o, 6F
                     CONSTANT character_p, 70
                     CONSTANT character_q, 71
                     CONSTANT character_r, 72
                     CONSTANT character_s, 73
                     CONSTANT character_t, 74
                     CONSTANT character_u, 75
                     CONSTANT character_v, 76
                     CONSTANT character_w, 77
                     CONSTANT character_x, 78
                     CONSTANT character_y, 79
                     CONSTANT character_z, 7A
                     CONSTANT character_A, 41
                     CONSTANT character_B, 42
                     CONSTANT character_C, 43
                     CONSTANT character_D, 44
                     CONSTANT character_E, 45
                     CONSTANT character_F, 46
                     CONSTANT character_G, 47
                     CONSTANT character_H, 48
                     CONSTANT character_I, 49
                     CONSTANT character_J, 4A
                     CONSTANT character_K, 4B
                     CONSTANT character_L, 4C
                     CONSTANT character_M, 4D
                     CONSTANT character_N, 4E
                     CONSTANT character_O, 4F
                     CONSTANT character_P, 50
                     CONSTANT character_Q, 51
                     CONSTANT character_R, 52
                     CONSTANT character_S, 53
                     CONSTANT character_T, 54
                     CONSTANT character_U, 55
                     CONSTANT character_V, 56
                     CONSTANT character_W, 57
                     CONSTANT character_X, 58
                     CONSTANT character_Y, 59
                     CONSTANT character_Z, 5A
                     CONSTANT character_0, 30
                     CONSTANT character_1, 31
                     CONSTANT character_2, 32
                     CONSTANT character_3, 33
                     CONSTANT character_4, 34
                     CONSTANT character_5, 35
                     CONSTANT character_6, 36
                     CONSTANT character_7, 37
                     CONSTANT character_8, 38
                     CONSTANT character_9, 39
                     CONSTANT character_colon, 3A
                     CONSTANT character_stop, 2E
                     CONSTANT character_semi_colon, 3B
                     CONSTANT character_minus, 2D
                     CONSTANT character_divide, 2F          ;'/'
                     CONSTANT character_plus, 2B
                     CONSTANT character_comma, 2C
                     CONSTANT character_less_than, 3C
                     CONSTANT character_greater_than, 3E
                     CONSTANT character_equals, 3D
                     CONSTANT character_space, 20
                     CONSTANT character_CR, 0D              ;carriage return
                     CONSTANT character_question, 3F        ;'?'
                     CONSTANT character_dollar, 24
                     CONSTANT character_exclaim, 21         ;'!'
                     CONSTANT character_BS, 08              ;Back Space command character
                     ;
                     ;
                     ;
                     ;
                     ;
                     ;**************************************************************************************
                     ;Initialise the system
                     ;**************************************************************************************
                     ;
                     ; Each PWM channels will be set to a different initial value just for the purposes
                     ; of demonstration. In practice, the initial duty values will depend on the requirements
                     ; of a given system but completely off (zero) is normally the safe option.
                     ;
                     ; Note that it is difficult to distinguish difference between the intensity of LEDs driven
                     ; with duty factors more than 40% (40% = 102/256 or 66Hex). So using relatively small values
                     ; will better demonstrate the PWM control of intensity.
                     ;
                     ; Initial values for LEDs give graduated intensity. Large change required for brighter LEDs.
                     ;
         cold_start: LOAD s0, 05                            ;5/256 = 2%
                     STORE s0, PWM_channel0
                     LOAD s0, 0D                            ;13/256 = 5%
                     STORE s0, PWM_channel1
                     LOAD s0, 14                            ;26/256 = 8%
                     STORE s0, PWM_channel2
                     LOAD s0, 26                            ;38/256 = 15%
                     STORE s0, PWM_channel3
                     LOAD s0, 40                            ;64/256 = 25%
                     STORE s0, PWM_channel4
                     LOAD s0, 58                            ;88/256 = 34%
                     STORE s0, PWM_channel5
                     LOAD s0, 80                            ;128/256 = 50%
                     STORE s0, PWM_channel6
                     LOAD s0, FF                            ;255/256 = 99.6% Maximum possible
                     STORE s0, PWM_channel7
                     ;
                     ; Initial values for simple outputs match documentation example
                     ;
                     LOAD s0, 11                            ;17/256 = 7%
                     STORE s0, PWM_channel8
                     LOAD s0, BC                            ;188/256 = 73%
                     STORE s0, PWM_channel9
                     LOAD s0, EF                            ;239/256 = 93%
                     STORE s0, PWM_channel10
                     LOAD s0, 22                            ;34/256 = 13%
                     STORE s0, PWM_channel11
                     ;
                     ENABLE INTERRUPT                       ;interrupts used to drive servo
                     ;
                     CALL send_welcome                      ;Write welcome message to UART
                     ;
                     ;
                     ;
                     ;**************************************************************************************
                     ; Main program
                     ;**************************************************************************************
                     ;
                     ; Provides a prompt to which an input with one of the following formats is expected...
                     ;
                     ;     LDn hh
                     ;
                     ;     IOk hh
                     ;     IOkk hh
                     ;
                     ;
                     ;  Where
                     ;     'LD' is a command to set one of the LED channels.
                     ;     'IO' is a command to set one of the simple I/O outputs on J4.
                     ;     'n' is an LED number in the range 0 to 7.
                     ;     'k' or 'kk' is a simple I/O number in the range 9 to 12.
                     ;     'hh' is a 2 digit hex value to specify the PWM duty factor (range 00 to FF).
                     ;
                     ; The input allows a degree of editing to be performed and upper and lower case letters
                     ; to be used.
                     ;
         warm_start: CALL send_prompt                       ;Prompt 'KCPSM3>'
                     CALL receive_string                    ;obtain input string of up to 16 characters
                     CALL upper_case_string                 ;convert string to upper case
                     ;
                     LOAD sE, string_start                  ;sE is memory pointer
                     FETCH s0, (sE)                         ;test for carriage return
                     COMPARE s0, character_CR
                     JUMP Z, warm_start
                     COMPARE s0, character_L                ;test for 'L' of 'LD' command
                     JUMP Z, LD_command
                     COMPARE s0, character_I                ;test for 'I' of 'IO' command
                     JUMP Z, IO_command
        bad_command: CALL send_CR                           ;no valid command entered
                     CALL send_Error
                     JUMP warm_start
                     ;
                     ;Processing potential 'LD' command
                     ;
         LD_command: CALL read_next_char
                     COMPARE s0, character_D                ;test for 'D' of 'LD' command
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;test for LED number
                     CALL 1char_to_value
                     JUMP C, bad_command
                     COMPARE s0, 08                         ;test for number in range 0 to 7
                     JUMP NC, bad_command
                     LOAD sD, s0                            ;convert number into memory pointer in sD
                     ADD sD, PWM_channel0
    read_duty_value: CALL read_next_char                    ;test for a space
                     COMPARE s0, character_space
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;read two character hex value
                     LOAD s3, s0
                     CALL read_next_char
                     LOAD s2, s0
                     CALL ASCII_byte_to_hex                 ;convert to value in s0
                     JUMP C, bad_command
                     LOAD sC, s0                            ;remember value
                     CALL read_next_char                    ;test for carriage return to end command
                     COMPARE s0, character_CR
                     JUMP NZ, bad_command
                     STORE sC, (sD)                         ;store new PWM duty factor for an LED
                     CALL send_OK
                     JUMP warm_start
                     ;
                     ;Processing potential 'LD' command
                     ;
         IO_command: CALL read_next_char
                     COMPARE s0, character_O                ;test for '0' of 'IO' command
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;test for IO number
                     COMPARE s0, character_1                ;first number must either be '1' or '9'
                     JUMP Z, next_IO_number
                     COMPARE s0, character_9
                     JUMP NZ, bad_command
                     LOAD sD, PWM_channel8                  ;IO9 is controlled by PWM channel8
                     JUMP read_duty_value
     next_IO_number: CALL read_next_char                    ;read next number for IO10 to IO12
                     CALL 1char_to_value
                     JUMP C, bad_command
                     COMPARE s0, 03                         ;test for number in range 0 to 2
                     JUMP NC, bad_command
                     LOAD sD, s0                            ;convert number into memory pointer in sD
                     ADD sD, PWM_channel9
                     JUMP read_duty_value
                     ;
                     ;Read next character from scratch pad memory
                     ;
     read_next_char: ADD sE, 01
                     FETCH s0, (sE)                         ;test for space
                     RETURN
                     ;
                     ;
                     ;
                     ;**************************************************************************************
                     ; UART communication routines
                     ;**************************************************************************************
                     ;
                     ; Read one character from the UART
                     ;
                     ; Character read will be returned in a register called 'UART_data'.
                     ;
                     ; The routine first tests the receiver FIFO buffer to see if data is present.
                     ; If the FIFO is empty, the routine waits until there is a character to read.
                     ; As this could take any amount of time the wait loop could include a call to a
                     ; subroutine which performs a useful function.
                     ;
                     ;
                     ; Registers used s0 and UART_data
                     ;
     read_from_UART: INPUT s0, status_port                  ;test Rx_FIFO buffer
                     TEST s0, rx_data_present               ;wait if empty
                     JUMP NZ, read_character
                     JUMP read_from_UART
     read_character: INPUT UART_data, UART_read_port        ;read from FIFO
                     RETURN
                     ;
                     ;
                     ;
                     ; Transmit one character to the UART
                     ;
                     ; Character supplied in register called 'UART_data'.
                     ;
                     ; The routine first tests the transmit FIFO buffer to see if it is full.
                     ; If the FIFO is full, then the routine waits until it there is space.
                     ;
                     ; Registers used s0
                     ;
       send_to_UART: INPUT s0, status_port                  ;test Tx_FIFO buffer
                     TEST s0, tx_full                       ;wait if full
                     JUMP Z, UART_write
                     JUMP send_to_UART
         UART_write: OUTPUT UART_data, UART_write_port
                     RETURN
                     ;
                     ;
                     ;
                     ;
                     ;Receive ASCII string from UART
                     ;
                     ;An ASCII string will be read from the UART and stored in scratch pad memory
                     ;commencing at the location specified by a constant named 'string_start'.
                     ;The string will have a maximum length of 16 characters including a
                     ;carriage return (0D) denoting the end of the string.
                     ;
                     ;As each character is read, it is echoed to the UART transmitter.
                     ;Some minor editing is supported using backspace (BS=08) which is used
                     ;to adjust what is stored in scratch pad memory and adjust the display
                     ;on the terminal screen using characters sent to the UART transmitter.
                     ;
                     ;A test is made for the receiver FIFO becoming full. A full status is treated as
                     ;a potential error situation and will result in a 'Overflow Error' message being
                     ;transmitted to the UART, the receiver FIFO being purged of all data and an
                     ;empty string being stored (carriage return at first location).
                     ;
                     ;Registers used s0, s1, s2 and 'UART_data'.
                     ;
     receive_string: LOAD s1, string_start                  ;locate start of string
                     LOAD s2, s1                            ;compute 16 character address
                     ADD s2, 10
  receive_full_test: INPUT s0, status_port                  ;test Rx_FIFO buffer for full
                     TEST s0, rx_full
                     JUMP NZ, read_error
                     CALL read_from_UART                    ;obtain and echo character
                     CALL send_to_UART
                     STORE UART_data, (s1)                  ;write to memory
                     COMPARE UART_data, character_CR        ;test for end of string
                     RETURN Z
                     COMPARE UART_data, character_BS        ;test for back space
                     JUMP Z, BS_edit
                     ADD s1, 01                             ;increment memory pointer
                     COMPARE s1, s2                         ;test for pointer exceeding 16 characters
                     JUMP NZ, receive_full_test             ;next character
                     CALL send_backspace                    ;hold end of string position on terminal display
            BS_edit: SUB s1, 01                             ;memory pointer back one
                     COMPARE s1, string_start               ;test for under flow
                     JUMP C, string_start_again
                     CALL send_space                        ;clear character at current position
                     CALL send_backspace                    ;position cursor
                     JUMP receive_full_test                 ;next character
 string_start_again: CALL send_greater_than                 ;restore '>' at prompt
                     JUMP receive_string                    ;begin again
                     ;Receiver buffer overflow condition
         read_error: CALL send_CR                           ;Transmit error message
                     STORE UART_data, string_start          ;empty string in memory (start with CR)
                     CALL send_Overflow_Error
                     CALL send_CR
 clear_UART_Rx_loop: INPUT s0, status_port                  ;test Rx_FIFO buffer for data
                     TEST s0, rx_data_present
                     RETURN Z                               ;finish when buffer is empty
                     INPUT UART_data, UART_read_port        ;read from FIFO and ignore
                     JUMP clear_UART_Rx_loop
                     ;
                     ;
                     ;**************************************************************************************
                     ; Useful ASCII conversion and handling routines
                     ;**************************************************************************************
                     ;
                     ;
                     ;
                     ; Convert character to upper case
                     ;
                     ; The character supplied in register s0.
                     ; If the character is in the range 'a' to 'z', it is converted
                     ; to the equivalent upper case character in the range 'A' to 'Z'.
                     ; All other characters remain unchanged.
                     ;
                     ; Registers used s0.
                     ;
         upper_case: COMPARE s0, 61                         ;eliminate character codes below 'a' (61 hex)
                     RETURN C
                     COMPARE s0, 7B                         ;eliminate character codes above 'z' (7A hex)
                     RETURN NC
                     AND s0, DF                             ;mask bit5 to convert to upper case
                     RETURN
                     ;
                     ;
                     ;
                     ; Convert string held in scratch pad memory to upper case.
                     ;
                     ; Registers used s0, s1
                     ;
  upper_case_string: LOAD s1, string_start
           ucs_loop: FETCH s0, (s1)
                     COMPARE s0, character_CR
                     RETURN Z
                     CALL upper_case
                     STORE s0, (s1)
                     ADD s1, 01
                     JUMP ucs_loop
                     ;
                     ;
                     ; Convert character '0' to '9' to numerical value in range 0 to 9
                     ;
                     ; The character supplied in register s0. If the character is in the
                     ; range '0' to '9', it is converted to the equivalent decimal value.
                     ; Characters not in the range '0' to '9' are signified by the return
                     ; with the CARRY flag set.
                     ;
                     ; Registers used s0.
                     ;
     1char_to_value: ADD s0, C6                             ;reject character codes above '9' (39 hex)
                     RETURN C                               ;carry flag is set
                     SUB s0, F6                             ;reject character codes below '0' (30 hex)
                     RETURN                                 ;carry is set if value not in range
                     ;
                     ;
                     ;
                     ; Convert the HEX ASCII characters contained in 's3' and 's2' into
                     ; an equivalent hexadecimal value in register 's0'.
                     ;     The upper nibble is represented by an ASCII character in register s3.
                     ;     The lower nibble is represented by an ASCII character in register s2.
                     ;
                     ; Input characters must be in the range 00 to FF hexadecimal or the CARRY flag
                     ; will be set on return.
                     ;
                     ; Registers used s0, s2 and s3.
                     ;
  ASCII_byte_to_hex: LOAD s0, s3                            ;Take upper nibble
                     CALL ASCII_to_hex                      ;convert to value
                     RETURN C                               ;reject if out of range
                     LOAD s3, s0                            ;remember value
                     SL0 s3                                 ;multiply value by 16 to put in upper nibble
                     SL0 s3
                     SL0 s3
                     SL0 s3
                     LOAD s0, s2                            ;Take lower nibble
                     CALL ASCII_to_hex                      ;convert to value
                     RETURN C                               ;reject if out of range
                     OR s0, s3                              ;merge in the upper nibble with CARRY reset
                     RETURN
                     ;
                     ;
                     ; Routine to convert ASCII data in 's0' to an equivalent HEX value.
                     ;
                     ; If character is not valid for hex, then CARRY is set on return.
                     ;
                     ; Register used s0
                     ;
       ASCII_to_hex: ADD s0, B9                             ;test for above ASCII code 46 ('F')
                     RETURN C
                     SUB s0, E9                             ;normalise 0 to 9 with A-F in 11 to 16 hex
                     RETURN C                               ;reject below ASCII code 30 ('0')
                     SUB s0, 11                             ;isolate A-F down to 00 to 05 hex
                     JUMP NC, ASCII_letter
                     ADD s0, 07                             ;test for above ASCII code 46 ('F')
                     RETURN C
                     SUB s0, F6                             ;convert to range 00 to 09
                     RETURN
       ASCII_letter: ADD s0, 0A                             ;convert to range 0A to 0F
                     RETURN
                     ;
                     ;
                     ;
                     ;**************************************************************************************
                     ; Text messages
                     ;**************************************************************************************
                     ;
                     ;
                     ; Send Carriage Return to the UART
                     ;
            send_CR: LOAD UART_data, character_CR
                     CALL send_to_UART
                     RETURN
                     ;
                     ; Send a space to the UART
                     ;
         send_space: LOAD UART_data, character_space
                     CALL send_to_UART
                     RETURN
                     ;
                     ;
                     ;
                     ;Send a back space to the UART
                     ;
     send_backspace: LOAD UART_data, character_BS
                     CALL send_to_UART
                     RETURN
                     ;
                     ;
                     ; Send 'PicoBlaze Servo Control' string to the UART
                     ;
       send_welcome: CALL send_CR
                     CALL send_CR
                     LOAD UART_data, character_P
                     CALL send_to_UART
                     LOAD UART_data, character_i
                     CALL send_to_UART
                     LOAD UART_data, character_c
                     CALL send_to_UART
                     LOAD UART_data, character_o
                     CALL send_to_UART
                     LOAD UART_data, character_B
                     CALL send_to_UART
                     LOAD UART_data, character_l
                     CALL send_to_UART
                     LOAD UART_data, character_a
                     CALL send_to_UART
                     LOAD UART_data, character_z
                     CALL send_to_UART
                     LOAD UART_data, character_e
                     CALL send_to_UART
                     CALL send_space
                     LOAD UART_data, character_P
                     CALL send_to_UART
                     LOAD UART_data, character_W
                     CALL send_to_UART
                     LOAD UART_data, character_M
                     CALL send_to_UART
                     CALL send_space
                     LOAD UART_data, character_C
                     CALL send_to_UART
                     LOAD UART_data, character_o
                     CALL send_to_UART
                     LOAD UART_data, character_n
                     CALL send_to_UART
                     LOAD UART_data, character_t
                     CALL send_to_UART
                     LOAD UART_data, character_r
                     CALL send_to_UART
                     LOAD UART_data, character_o
                     CALL send_to_UART
                     LOAD UART_data, character_l
                     CALL send_to_UART
                     CALL send_CR
                     CALL send_CR
                     RETURN
                     ;
                     ;
                     ;Send 'KCPSM3>' prompt to the UART
                     ;
        send_prompt: CALL send_CR                           ;start new line
                     LOAD UART_data, character_K
                     CALL send_to_UART
                     LOAD UART_data, character_C
                     CALL send_to_UART
                     LOAD UART_data, character_P
                     CALL send_to_UART
                     LOAD UART_data, character_S
                     CALL send_to_UART
                     LOAD UART_data, character_M
                     CALL send_to_UART
                     LOAD UART_data, character_3
                     CALL send_to_UART
                     ;
                     ;Send '>' character to the UART
                     ;
  send_greater_than: LOAD UART_data, character_greater_than
                     CALL send_to_UART
                     RETURN
                     ;
                     ;
                     ;Send 'Overflow Error' to the UART
                     ;
send_Overflow_Error: LOAD UART_data, character_O
                     CALL send_to_UART
                     LOAD UART_data, character_v
                     CALL send_to_UART
                     LOAD UART_data, character_e
                     CALL send_to_UART
                     LOAD UART_data, character_r
                     CALL send_to_UART
                     LOAD UART_data, character_f
                     CALL send_to_UART
                     LOAD UART_data, character_l
                     CALL send_to_UART
                     LOAD UART_data, character_o
                     CALL send_to_UART
                     LOAD UART_data, character_w
                     CALL send_to_UART
   send_space_Error: CALL send_space
                     ;
                     ;Send 'Error' to the UART
                     ;
         send_Error: LOAD UART_data, character_E
                     CALL send_to_UART
                     LOAD UART_data, character_r
                     CALL send_to_UART
                     CALL send_to_UART
                     LOAD UART_data, character_o
                     CALL send_to_UART
                     LOAD UART_data, character_r
                     CALL send_to_UART
                     JUMP send_CR
                     ;
                     ;
                     ;Send 'OK' to the UART
                     ;
            send_OK: CALL send_CR
                     LOAD UART_data, character_O
                     CALL send_to_UART
                     LOAD UART_data, character_K
                     CALL send_to_UART
                     JUMP send_CR
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
                     ; locations are used to determine the desired duty factor for each of 12 channels.
                     ;
                     ; Note that an interrupt is generated every 196 clock cycles. This means that there is
                     ; only time to execute 98 instructions between each interrupt. This ISR is 48 instructions
                     ; long. A further 3 instructions are also consumed by the interrupt process
                     ; (abandoned instruction, virtual CALL to 3FF and the interrupt vector JUMP) and hence
                     ; PicoBlaze has approximately half of its time available for other tasks in the main program.
                     ;
                     ; Although a loop would normal be employed in software to process each of 12 channels,
                     ; the implementation of a loop would increase the number of instructions which needed to
                     ; be executed to such an extent that this 12 channel implementation would not be possible.
                     ; Consequently the code is written out in a linear fashion which consumes more program
                     ; space but which executes faster.
                     ;
                ISR: STORE s0, ISR_preserve_s0              ;preserve registers to be used
                     STORE s1, ISR_preserve_s1
                     STORE s2, ISR_preserve_s2
                     ;Determine the number of steps currently through the 1ms PWM cycle
                     FETCH s1, PWM_duty_counter             ;read 8-bit counter of steps
                     ADD s1, 01                             ;increment counter (will roll over to zero)
                     STORE s1, PWM_duty_counter             ;update count value in memory for next interrupt.
                     ;Read duty factor for each channel and compare it with the duty counter and set or
                     ;reset a bit in register s2 accordingly.
                     FETCH s0, PWM_channel11                ;read desired setting of pulse width
                     COMPARE s1, s0                         ;set carry flag if duty factor > duty counter
                     SLA s2                                 ;shift carry into register s2
                     FETCH s0, PWM_channel10                ;read desired setting of pulse width
                     COMPARE s1, s0                         ;set carry flag if duty factor > duty counter
                     SLA s2                                 ;shift carry into register s2
                     FETCH s0, PWM_channel9                 ;read desired setting of pulse width
                     COMPARE s1, s0                         ;set carry flag if duty factor > duty counter
                     SLA s2                                 ;shift carry into register s2
                     FETCH s0, PWM_channel8                 ;read desired setting of pulse width
                     COMPARE s1, s0                         ;set carry flag if duty factor > duty counter
                     SLA s2                                 ;shift carry into register s2
                     OUTPUT s2, simple_port                 ;drive pins on connector J4
                     FETCH s0, PWM_channel7                 ;read desired setting of pulse width
                     COMPARE s1, s0                         ;set carry flag if duty factor > duty counter
                     SLA s2                                 ;shift carry into register s2
                     FETCH s0, PWM_channel6                 ;read desired setting of pulse width
                     COMPARE s1, s0                         ;set carry flag if duty factor > duty counter
                     SLA s2                                 ;shift carry into register s2
                     FETCH s0, PWM_channel5                 ;read desired setting of pulse width
                     COMPARE s1, s0                         ;set carry flag if duty factor > duty counter
                     SLA s2                                 ;shift carry into register s2
                     FETCH s0, PWM_channel4                 ;read desired setting of pulse width
                     COMPARE s1, s0                         ;set carry flag if duty factor > duty counter
                     SLA s2                                 ;shift carry into register s2
                     FETCH s0, PWM_channel3                 ;read desired setting of pulse width
                     COMPARE s1, s0                         ;set carry flag if duty factor > duty counter
                     SLA s2                                 ;shift carry into register s2
                     FETCH s0, PWM_channel2                 ;read desired setting of pulse width
                     COMPARE s1, s0                         ;set carry flag if duty factor > duty counter
                     SLA s2                                 ;shift carry into register s2
                     FETCH s0, PWM_channel1                 ;read desired setting of pulse width
                     COMPARE s1, s0                         ;set carry flag if duty factor > duty counter
                     SLA s2                                 ;shift carry into register s2
                     FETCH s0, PWM_channel0                 ;read desired setting of pulse width
                     COMPARE s1, s0                         ;set carry flag if duty factor > duty counter
                     SLA s2                                 ;shift carry into register s2
                     OUTPUT s2, LED_port                    ;drive LEDs
                     FETCH s0, ISR_preserve_s0              ;restore register values
                     FETCH s1, ISR_preserve_s1
                     FETCH s2, ISR_preserve_s2
                     RETURNI ENABLE
                     ;
                     ;
                     ;**************************************************************************************
                     ; Interrupt Vector
                     ;**************************************************************************************
                     ;
                     ADDRESS 3FF
                     JUMP ISR
                     ;
                     ;
