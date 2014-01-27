; KCPSM3 Program - Pulse Width Modulation (PWM) Control on the Spartan-3E Starter Kit.
device kcpsm1
; Ken Chapman - Xilinx Ltd
;
; Version v1.00 - 22nd May 2006
;
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
led_port                EQU             0x80                    ;8 simple LEDs
led0                    EQU             0x01                    ;     LED 0 - bit0
led1                    EQU             0x02                    ;         1 - bit1
led2                    EQU             0x04                    ;         2 - bit2
led3                    EQU             0x08                    ;         3 - bit3
led4                    EQU             0x10                    ;         4 - bit4
led5                    EQU             0x20                    ;         5 - bit5
led6                    EQU             0x40                    ;         6 - bit6
led7                    EQU             0x80                    ;         7 - bit7
;
;
simple_port             EQU             0x40                    ;4 simple outputs
simple_io9              EQU             0x01                    ;  Header  IO9  - bit0
simple_io10             EQU             0x02                    ;          IO10 - bit1
simple_io11             EQU             0x04                    ;          IO11 - bit2
simple_io12             EQU             0x08                    ;          IO12 - bit3
;
;
;
status_port             EQU             0x00                    ;UART status input
tx_half_full            EQU             0x01                    ;  Transmitter     half full - bit0
tx_full                 EQU             0x02                    ;    FIFO               full - bit1
rx_data_present         EQU             0x04                    ;  Receiver     data present - bit2
rx_half_full            EQU             0x08                    ;    FIFO          half full - bit3
rx_full                 EQU             0x10                    ;                   full - bit4
spare1                  EQU             0x20                    ;                  spare '0' - bit5
spare2                  EQU             0x40                    ;                  spare '0' - bit6
spare3                  EQU             0x80                    ;                  spare '0' - bit7
;
uart_read_port          EQU             0x01                    ;UART Rx data input
;
uart_write_port         EQU             0x20                    ;UART Tx data output
;
;
;
;**************************************************************************************
; Special Register usage
;**************************************************************************************
;
uart_data               REG             sf                      ;used to pass data to and from the UART
;
;
;
;**************************************************************************************
;Scratch Pad Memory Locations
;**************************************************************************************
;
pwm_duty_counter        EQU             0x00                    ;Duty Counter 0 to 255 within 1KHz period (1ms)
pwm_channel0            EQU             0x01                    ;PWM settings for each channel
pwm_channel1            EQU             0x02                    ; Channels 0 to 7 = LEDs 0 to 7
pwm_channel2            EQU             0x03                    ; Channels 8 to 11 = IO9 to IO12
pwm_channel3            EQU             0x04
pwm_channel4            EQU             0x05
pwm_channel5            EQU             0x06
pwm_channel6            EQU             0x07
pwm_channel7            EQU             0x08
pwm_channel8            EQU             0x09
pwm_channel9            EQU             0x0a
pwm_channel10           EQU             0x0b
pwm_channel11           EQU             0x0c
isr_preserve_s0         EQU             0x0d                    ;preserve register contents during Interrupt Service Routine
isr_preserve_s1         EQU             0x0e
isr_preserve_s2         EQU             0x0f
;
;
;
;
;UART character strings will be stored in scratch pad memory ending in carriage return.
;A string can be up to 16 characters with the start location defined by this constant.
;
string_start            EQU             0x30
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
character_a             EQU             0x61
character_b             EQU             0x62
character_c             EQU             0x63
character_d             EQU             0x64
character_e             EQU             0x65
character_f             EQU             0x66
character_g             EQU             0x67
character_h             EQU             0x68
character_i             EQU             0x69
character_j             EQU             0x6a
character_k             EQU             0x6b
character_l             EQU             0x6c
character_m             EQU             0x6d
character_n             EQU             0x6e
character_o             EQU             0x6f
character_p             EQU             0x70
character_q             EQU             0x71
character_r             EQU             0x72
character_s             EQU             0x73
character_t             EQU             0x74
character_u             EQU             0x75
character_v             EQU             0x76
character_w             EQU             0x77
character_x             EQU             0x78
character_y             EQU             0x79
character_z             EQU             0x7a
_character_a            EQU             0x41
_character_b            EQU             0x42
_character_c            EQU             0x43
_character_d            EQU             0x44
_character_e            EQU             0x45
_character_f            EQU             0x46
_character_g            EQU             0x47
_character_h            EQU             0x48
_character_i            EQU             0x49
_character_j            EQU             0x4a
_character_k            EQU             0x4b
_character_l            EQU             0x4c
_character_m            EQU             0x4d
_character_n            EQU             0x4e
_character_o            EQU             0x4f
_character_p            EQU             0x50
_character_q            EQU             0x51
_character_r            EQU             0x52
_character_s            EQU             0x53
_character_t            EQU             0x54
_character_u            EQU             0x55
_character_v            EQU             0x56
_character_w            EQU             0x57
_character_x            EQU             0x58
_character_y            EQU             0x59
_character_z            EQU             0x5a
character_0             EQU             0x30
character_1             EQU             0x31
character_2             EQU             0x32
character_3             EQU             0x33
character_4             EQU             0x34
character_5             EQU             0x35
character_6             EQU             0x36
character_7             EQU             0x37
character_8             EQU             0x38
character_9             EQU             0x39
character_colon         EQU             0x3a
character_stop          EQU             0x2e
character_semi_colon    EQU             0x3b
character_minus         EQU             0x2d
character_divide        EQU             0x2f                    ;'/'
character_plus          EQU             0x2b
character_comma         EQU             0x2c
character_less_than     EQU             0x3c
character_greater_than  EQU             0x3e
character_equals        EQU             0x3d
character_space         EQU             0x20
character_cr            EQU             0x0d                    ;carriage return
character_question      EQU             0x3f                    ;'?'
character_dollar        EQU             0x24
character_exclaim       EQU             0x21                    ;'!'
character_bs            EQU             0x08                    ;Back Space command character
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
cold_start:             LOAD            s0, #0x05               ;5/256 = 2%
                        STORE           s0, pwm_channel0
                        LOAD            s0, #0x0d               ;13/256 = 5%
                        STORE           s0, pwm_channel1
                        LOAD            s0, #0x14               ;26/256 = 8%
                        STORE           s0, pwm_channel2
                        LOAD            s0, #0x26               ;38/256 = 15%
                        STORE           s0, pwm_channel3
                        LOAD            s0, #0x40               ;64/256 = 25%
                        STORE           s0, pwm_channel4
                        LOAD            s0, #0x58               ;88/256 = 34%
                        STORE           s0, pwm_channel5
                        LOAD            s0, #0x80               ;128/256 = 50%
                        STORE           s0, pwm_channel6
                        LOAD            s0, #0xff               ;255/256 = 99.6% Maximum possible
                        STORE           s0, pwm_channel7
;
; Initial values for simple outputs match documentation example
;
                        LOAD            s0, #0x11               ;17/256 = 7%
                        STORE           s0, pwm_channel8
                        LOAD            s0, #0xbc               ;188/256 = 73%
                        STORE           s0, pwm_channel9
                        LOAD            s0, #0xef               ;239/256 = 93%
                        STORE           s0, pwm_channel10
                        LOAD            s0, #0x22               ;34/256 = 13%
                        STORE           s0, pwm_channel11
;
                        ENABLE          interrupt               ;interrupts used to drive servo
;
                        CALL            send_welcome            ;Write welcome message to UART
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
warm_start:             CALL            send_prompt             ;Prompt 'KCPSM3>'
                        CALL            receive_string          ;obtain input string of up to 16 characters
                        CALL            upper_case_string       ;convert string to upper case
;
                        LOAD            se, #string_start       ;sE is memory pointer
                        FETCH           s0, @se                 ;test for carriage return
                        COMPARE         s0, #character_cr
                        JUMP            z, warm_start
                        COMPARE         s0, #_character_l       ;test for 'L' of 'LD' command
                        JUMP            z, ld_command
                        COMPARE         s0, #_character_i       ;test for 'I' of 'IO' command
                        JUMP            z, io_command
bad_command:            CALL            send_cr                 ;no valid command entered
                        CALL            send_error
                        JUMP            warm_start
;
;Processing potential 'LD' command
;
ld_command:             CALL            read_next_char
                        COMPARE         s0, #_character_d       ;test for 'D' of 'LD' command
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;test for LED number
                        CALL            _1char_to_value
                        JUMP            c, bad_command
                        COMPARE         s0, #0x08               ;test for number in range 0 to 7
                        JUMP            nc, bad_command
                        LOAD            sd, s0                  ;convert number into memory pointer in sD
                        ADD             sd, #pwm_channel0
read_duty_value:        CALL            read_next_char          ;test for a space
                        COMPARE         s0, #character_space
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;read two character hex value
                        LOAD            s3, s0
                        CALL            read_next_char
                        LOAD            s2, s0
                        CALL            ascii_byte_to_hex       ;convert to value in s0
                        JUMP            c, bad_command
                        LOAD            sc, s0                  ;remember value
                        CALL            read_next_char          ;test for carriage return to end command
                        COMPARE         s0, #character_cr
                        JUMP            nz, bad_command
                        STORE           sc, @sd                 ;store new PWM duty factor for an LED
                        CALL            send_ok
                        JUMP            warm_start
;
;Processing potential 'LD' command
;
io_command:             CALL            read_next_char
                        COMPARE         s0, #_character_o       ;test for '0' of 'IO' command
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;test for IO number
                        COMPARE         s0, #character_1        ;first number must either be '1' or '9'
                        JUMP            z, next_io_number
                        COMPARE         s0, #character_9
                        JUMP            nz, bad_command
                        LOAD            sd, #pwm_channel8       ;IO9 is controlled by PWM channel8
                        JUMP            read_duty_value
next_io_number:         CALL            read_next_char          ;read next number for IO10 to IO12
                        CALL            _1char_to_value
                        JUMP            c, bad_command
                        COMPARE         s0, #0x03               ;test for number in range 0 to 2
                        JUMP            nc, bad_command
                        LOAD            sd, s0                  ;convert number into memory pointer in sD
                        ADD             sd, #pwm_channel9
                        JUMP            read_duty_value
;
;Read next character from scratch pad memory
;
read_next_char:         ADD             se, #0x01
                        FETCH           s0, @se                 ;test for space
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
read_from_uart:         INPUT           s0, status_port         ;test Rx_FIFO buffer
                        TEST            s0, #rx_data_present    ;wait if empty
                        JUMP            nz, read_character
                        JUMP            read_from_uart
read_character:         INPUT           uart_data, uart_read_port ;read from FIFO
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
send_to_uart:           INPUT           s0, status_port         ;test Tx_FIFO buffer
                        TEST            s0, #tx_full            ;wait if full
                        JUMP            z, uart_write
                        JUMP            send_to_uart
uart_write:             OUTPUT          uart_data, uart_write_port
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
receive_string:         LOAD            s1, #string_start       ;locate start of string
                        LOAD            s2, s1                  ;compute 16 character address
                        ADD             s2, #0x10
receive_full_test:      INPUT           s0, status_port         ;test Rx_FIFO buffer for full
                        TEST            s0, #rx_full
                        JUMP            nz, read_error
                        CALL            read_from_uart          ;obtain and echo character
                        CALL            send_to_uart
                        STORE           uart_data, @s1          ;write to memory
                        COMPARE         uart_data, #character_cr ;test for end of string
                        RETURN          z
                        COMPARE         uart_data, #character_bs ;test for back space
                        JUMP            z, bs_edit
                        ADD             s1, #0x01               ;increment memory pointer
                        COMPARE         s1, s2                  ;test for pointer exceeding 16 characters
                        JUMP            nz, receive_full_test   ;next character
                        CALL            send_backspace          ;hold end of string position on terminal display
bs_edit:                SUB             s1, #0x01               ;memory pointer back one
                        COMPARE         s1, #string_start       ;test for under flow
                        JUMP            c, string_start_again
                        CALL            send_space              ;clear character at current position
                        CALL            send_backspace          ;position cursor
                        JUMP            receive_full_test       ;next character
string_start_again:     CALL            send_greater_than       ;restore '>' at prompt
                        JUMP            receive_string          ;begin again
;Receiver buffer overflow condition
read_error:             CALL            send_cr                 ;Transmit error message
                        STORE           uart_data, string_start ;empty string in memory (start with CR)
                        CALL            send_overflow_error
                        CALL            send_cr
clear_uart_rx_loop:     INPUT           s0, status_port         ;test Rx_FIFO buffer for data
                        TEST            s0, #rx_data_present
                        RETURN          z                       ;finish when buffer is empty
                        INPUT           uart_data, uart_read_port ;read from FIFO and ignore
                        JUMP            clear_uart_rx_loop
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
upper_case:             COMPARE         s0, #0x61               ;eliminate character codes below 'a' (61 hex)
                        RETURN          c
                        COMPARE         s0, #0x7b               ;eliminate character codes above 'z' (7A hex)
                        RETURN          nc
                        AND             s0, #0xdf               ;mask bit5 to convert to upper case
                        RETURN
;
;
;
; Convert string held in scratch pad memory to upper case.
;
; Registers used s0, s1
;
upper_case_string:      LOAD            s1, #string_start
ucs_loop:               FETCH           s0, @s1
                        COMPARE         s0, #character_cr
                        RETURN          z
                        CALL            upper_case
                        STORE           s0, @s1
                        ADD             s1, #0x01
                        JUMP            ucs_loop
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
_1char_to_value:        ADD             s0, #0xc6               ;reject character codes above '9' (39 hex)
                        RETURN          c                       ;carry flag is set
                        SUB             s0, #0xf6               ;reject character codes below '0' (30 hex)
                        RETURN                                  ;carry is set if value not in range
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
ascii_byte_to_hex:      LOAD            s0, s3                  ;Take upper nibble
                        CALL            ascii_to_hex            ;convert to value
                        RETURN          c                       ;reject if out of range
                        LOAD            s3, s0                  ;remember value
                        SL0             s3                      ;multiply value by 16 to put in upper nibble
                        SL0             s3
                        SL0             s3
                        SL0             s3
                        LOAD            s0, s2                  ;Take lower nibble
                        CALL            ascii_to_hex            ;convert to value
                        RETURN          c                       ;reject if out of range
                        OR              s0, s3                  ;merge in the upper nibble with CARRY reset
                        RETURN
;
;
; Routine to convert ASCII data in 's0' to an equivalent HEX value.
;
; If character is not valid for hex, then CARRY is set on return.
;
; Register used s0
;
ascii_to_hex:           ADD             s0, #0xb9               ;test for above ASCII code 46 ('F')
                        RETURN          c
                        SUB             s0, #0xe9               ;normalise 0 to 9 with A-F in 11 to 16 hex
                        RETURN          c                       ;reject below ASCII code 30 ('0')
                        SUB             s0, #0x11               ;isolate A-F down to 00 to 05 hex
                        JUMP            nc, ascii_letter
                        ADD             s0, #0x07               ;test for above ASCII code 46 ('F')
                        RETURN          c
                        SUB             s0, #0xf6               ;convert to range 00 to 09
                        RETURN
ascii_letter:           ADD             s0, #0x0a               ;convert to range 0A to 0F
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
send_cr:                LOAD            uart_data, #character_cr
                        CALL            send_to_uart
                        RETURN
;
; Send a space to the UART
;
send_space:             LOAD            uart_data, #character_space
                        CALL            send_to_uart
                        RETURN
;
;
;
;Send a back space to the UART
;
send_backspace:         LOAD            uart_data, #character_bs
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'PicoBlaze Servo Control' string to the UART
;
send_welcome:           CALL            send_cr
                        CALL            send_cr
                        LOAD            uart_data, #_character_p
                        CALL            send_to_uart
                        LOAD            uart_data, #character_i
                        CALL            send_to_uart
                        LOAD            uart_data, #character_c
                        CALL            send_to_uart
                        LOAD            uart_data, #character_o
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_b
                        CALL            send_to_uart
                        LOAD            uart_data, #character_l
                        CALL            send_to_uart
                        LOAD            uart_data, #character_a
                        CALL            send_to_uart
                        LOAD            uart_data, #character_z
                        CALL            send_to_uart
                        LOAD            uart_data, #character_e
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_data, #_character_p
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_w
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_m
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_data, #_character_c
                        CALL            send_to_uart
                        LOAD            uart_data, #character_o
                        CALL            send_to_uart
                        LOAD            uart_data, #character_n
                        CALL            send_to_uart
                        LOAD            uart_data, #character_t
                        CALL            send_to_uart
                        LOAD            uart_data, #character_r
                        CALL            send_to_uart
                        LOAD            uart_data, #character_o
                        CALL            send_to_uart
                        LOAD            uart_data, #character_l
                        CALL            send_to_uart
                        CALL            send_cr
                        CALL            send_cr
                        RETURN
;
;
;Send 'KCPSM3>' prompt to the UART
;
send_prompt:            CALL            send_cr                 ;start new line
                        LOAD            uart_data, #_character_k
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_c
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_p
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_s
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_m
                        CALL            send_to_uart
                        LOAD            uart_data, #character_3
                        CALL            send_to_uart
;
;Send '>' character to the UART
;
send_greater_than:      LOAD            uart_data, #character_greater_than
                        CALL            send_to_uart
                        RETURN
;
;
;Send 'Overflow Error' to the UART
;
send_overflow_error:    LOAD            uart_data, #_character_o
                        CALL            send_to_uart
                        LOAD            uart_data, #character_v
                        CALL            send_to_uart
                        LOAD            uart_data, #character_e
                        CALL            send_to_uart
                        LOAD            uart_data, #character_r
                        CALL            send_to_uart
                        LOAD            uart_data, #character_f
                        CALL            send_to_uart
                        LOAD            uart_data, #character_l
                        CALL            send_to_uart
                        LOAD            uart_data, #character_o
                        CALL            send_to_uart
                        LOAD            uart_data, #character_w
                        CALL            send_to_uart
send_space_error:       CALL            send_space
;
;Send 'Error' to the UART
;
send_error:             LOAD            uart_data, #_character_e
                        CALL            send_to_uart
                        LOAD            uart_data, #character_r
                        CALL            send_to_uart
                        CALL            send_to_uart
                        LOAD            uart_data, #character_o
                        CALL            send_to_uart
                        LOAD            uart_data, #character_r
                        CALL            send_to_uart
                        JUMP            send_cr
;
;
;Send 'OK' to the UART
;
send_ok:                CALL            send_cr
                        LOAD            uart_data, #_character_o
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_k
                        CALL            send_to_uart
                        JUMP            send_cr
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
isr:                    STORE           s0, isr_preserve_s0     ;preserve registers to be used
                        STORE           s1, isr_preserve_s1
                        STORE           s2, isr_preserve_s2
;Determine the number of steps currently through the 1ms PWM cycle
                        FETCH           s1, pwm_duty_counter    ;read 8-bit counter of steps
                        ADD             s1, #0x01               ;increment counter (will roll over to zero)
                        STORE           s1, pwm_duty_counter    ;update count value in memory for next interrupt.
;Read duty factor for each channel and compare it with the duty counter and set or
;reset a bit in register s2 accordingly.
                        FETCH           s0, pwm_channel11       ;read desired setting of pulse width
                        COMPARE         s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA             s2                      ;shift carry into register s2
                        FETCH           s0, pwm_channel10       ;read desired setting of pulse width
                        COMPARE         s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA             s2                      ;shift carry into register s2
                        FETCH           s0, pwm_channel9        ;read desired setting of pulse width
                        COMPARE         s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA             s2                      ;shift carry into register s2
                        FETCH           s0, pwm_channel8        ;read desired setting of pulse width
                        COMPARE         s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA             s2                      ;shift carry into register s2
                        OUTPUT          s2, simple_port         ;drive pins on connector J4
                        FETCH           s0, pwm_channel7        ;read desired setting of pulse width
                        COMPARE         s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA             s2                      ;shift carry into register s2
                        FETCH           s0, pwm_channel6        ;read desired setting of pulse width
                        COMPARE         s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA             s2                      ;shift carry into register s2
                        FETCH           s0, pwm_channel5        ;read desired setting of pulse width
                        COMPARE         s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA             s2                      ;shift carry into register s2
                        FETCH           s0, pwm_channel4        ;read desired setting of pulse width
                        COMPARE         s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA             s2                      ;shift carry into register s2
                        FETCH           s0, pwm_channel3        ;read desired setting of pulse width
                        COMPARE         s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA             s2                      ;shift carry into register s2
                        FETCH           s0, pwm_channel2        ;read desired setting of pulse width
                        COMPARE         s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA             s2                      ;shift carry into register s2
                        FETCH           s0, pwm_channel1        ;read desired setting of pulse width
                        COMPARE         s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA             s2                      ;shift carry into register s2
                        FETCH           s0, pwm_channel0        ;read desired setting of pulse width
                        COMPARE         s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA             s2                      ;shift carry into register s2
                        OUTPUT          s2, led_port            ;drive LEDs
                        FETCH           s0, isr_preserve_s0     ;restore register values
                        FETCH           s1, isr_preserve_s1
                        FETCH           s2, isr_preserve_s2
                        RETURNI         enable
;
;
;**************************************************************************************
; Interrupt Vector
;**************************************************************************************
;
                        ORG             0x3ff
                        JUMP            isr
;
;

