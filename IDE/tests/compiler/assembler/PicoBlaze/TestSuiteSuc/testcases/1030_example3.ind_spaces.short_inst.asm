; KCPSM3 Program - Security control and design authentication.
;
; This program is provided for use with the reference design
; 'low_cost_design_authentication_for_spartan_3e.vhd' implemented on the Spartan-3E Starter
; Kit. The program provides design authentication control over the 'real' application as well
; as providing features to enable evaluation of the design authentication method.
device kcpsm1
; Ken Chapman - Xilinx Ltd
;
; Version v1.00 - 1st November 2006
;
; This code communicates with the StrataFLASH memory to implement a design authentication
; algorithm which then enables the main application design in various ways. To facilitate
; evaluation of design authentication this design also interacts with the LCD display
; and PC (via UART based RS232 link) to indicate authentication status and allow control
; over the authentication validity of the design. Therefore this program includes:-
;
; 1) Code required to check authorisation which would be included in a production design.
; 2) Code required to program the authentication value into FLASH memory which would
;    typically only be implemented in a special design used at a secure programming
;    facility as part of the production programming procedure.
; 3) Code to allow you to see what is happening at all stages which is code that should
;    never be included in a real production design as it reveals secrets that should remain
;    hidden in order to make the task of breaking the security as difficult as possible.
;
; IMPORTANT - Feel free to use this code as a reference for your own security scheme but
;             never use this code unmodified.
;
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
; UART ports
;
; Connection to PC to allow display of progress information and to operate simple
; menu of commands.
;
status_port             EQU     0x00                    ;UART and memory status
tx_half_full            EQU     0x01                    ;  Transmitter     half full - bit0
tx_full                 EQU     0x02                    ;    FIFO            tx_full - bit1
rx_data_present         EQU     0x04                    ;  Receiver     data present - bit2
rx_half_full            EQU     0x08                    ;    FIFO          half full - bit3
rx_full                 EQU     0x10                    ;                    rx_full - bit4
spare1                  EQU     0x20                    ;                  spare '0' - bit5
spare2                  EQU     0x40                    ;                  spare '0' - bit6
strataflash_sts         EQU     0x80                    ;  StrataFLASH           STS - bit7
;
uart_read_port          EQU     0x01                    ;UART Rx data input
;
uart_write_port         EQU     0x08                    ;UART Tx data output
;
;
; LCD Display
;
;The master enable signal is not used by the LCD display itself
;but is used to prevent any contention with the StrataFLASH memory that
;is connected to the same data pins. In this design the StrataFLASH memory is
;used in 8-bit mode so not contention should exist but this master enable
;facilty is then available for anyone wanting to modify the design for use
;with a 16-bit interface.
;
lcd_output_port         EQU     0x20                    ;LCD character module output data and control
lcd_e                   EQU     0x01                    ;   active High Enable        E - bit0
lcd_rw                  EQU     0x02                    ;   Read=1 Write=0           RW - bit1
lcd_rs                  EQU     0x04                    ;   Instruction=0 Data=1     RS - bit2
lcd_drive               EQU     0x08                    ;   Master enable (active High) - bit3
lcd_db4                 EQU     0x10                    ;   4-bit              Data DB4 - bit4
lcd_db5                 EQU     0x20                    ;   interface          Data DB5 - bit5
lcd_db6                 EQU     0x40                    ;                      Data DB6 - bit6
lcd_db7                 EQU     0x80                    ;                      Data DB7 - bit7
;
;
lcd_input_port          EQU     0x03                    ;LCD character module input data
lcd_read_spare0         EQU     0x01                    ;    Spare bits               - bit0
lcd_read_spare1         EQU     0x02                    ;    are zero                 - bit1
lcd_read_spare2         EQU     0x04                    ;                             - bit2
lcd_read_spare3         EQU     0x08                    ;                             - bit3
lcd_read_db4            EQU     0x10                    ;    4-bit           Data DB4 - bit4
lcd_read_db5            EQU     0x20                    ;    interface       Data DB5 - bit5
lcd_read_db6            EQU     0x40                    ;                    Data DB6 - bit6
lcd_read_db7            EQU     0x80                    ;                    Data DB7 - bit7
;
;
;
; StrataFLASH memory ports
;
; The FLASH memory is used to hold the authentication value as well as provide the
; unique serial number from which the authentication algorithm computes the value.
; In practice, the FLASH will also hold the configuration image for the Spartan device.
;
;
sf_data_in_port         EQU     0x02                    ;Read data from StrataFLASH device
;
sf_data_out_port        EQU     0x80                    ;Data to write into StrataFLASH device
;
sf_addr_hi_port         EQU     0x83                    ;StrataFLASH address[21:16] (6 LSB's)
sf_addr_mi_port         EQU     0x82                    ;StrataFLASH address[15:8]
sf_addr_lo_port         EQU     0x81                    ;StrataFLASH address[7:0]
;
sf_control_port         EQU     0x40                    ;StrataFLASH control
sf_read                 EQU     0x01                    ;         active High read - bit0
sf_ce                   EQU     0x02                    ; active Low device enable - bit1
sf_we                   EQU     0x04                    ;         active Low write - bit2
;
;
; Design Authentication enable/disable signals.
;
; Hardware controls over the 'real' application.
;
authentication_control_port EQU 0x10                    ;Design disable control port
security_disable_interrupts EQU 0x01                    ;   active High disable of interrupt generation - bit0
security_disable_outputs EQU    0x02                    ;            active High disable of output pins - bit1
;
; Pseudo Random number generator
;
random_value_port       EQU     0x04                    ;read LFSR counter value
;
;
; Link FIFO buffer
;
; Provides a connection to the 'real' application such that 'soload tokens' in the
; form of short messages to be passed to the 'real' application to enable or disable
; it depending on the authentication status.
;
link_fifo_write_port    EQU     0x04                    ;write data to FIFO
;
;
;**************************************************************************************
; Special Register usage
;**************************************************************************************
;
uart_data               REG     sf                      ;used to pass data to and from the UART
;
;
;
;**************************************************************************************
;Scratch Pad Memory Locations
;**************************************************************************************
;
isr_preserve_s0         EQU     0x00                    ;preserve register contents during Interrupt Service Routine
;
;
serial_number0          EQU     0x10                    ;64-bit serial number of StrataFlash
serial_number1          EQU     0x11                    ;LS-Byte first
serial_number2          EQU     0x12
serial_number3          EQU     0x13
serial_number4          EQU     0x14
serial_number5          EQU     0x15
serial_number6          EQU     0x16
serial_number7          EQU     0x17
;
;
computed_crc0           EQU     0x18                    ;computed 16-bit CRC based on the
computed_crc1           EQU     0x19                    ;  StrataFlash unique serial number (LS-Byte first)
;
;
authentication_crc0     EQU     0x1a                    ;16-bit CRC value read from authentication
authentication_crc1     EQU     0x1b                    ;  area of StrataFLASH memory (LS-Byte first)
;
;
authentication_status   EQU     0x1c                    ;Status of design authentication
;
;
;**************************************************************************************
;Useful data constants
;**************************************************************************************
;
;
;
;Constant to define a soloadware delay of 1us. This must be adjusted to reflect the
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
delay_1us_constant      EQU     0x0b
;
;
;
;
;ASCII table
;
character_a             EQU     0x61
character_b             EQU     0x62
character_c             EQU     0x63
character_d             EQU     0x64
character_e             EQU     0x65
character_f             EQU     0x66
character_g             EQU     0x67
character_h             EQU     0x68
character_i             EQU     0x69
character_j             EQU     0x6a
character_k             EQU     0x6b
character_l             EQU     0x6c
character_m             EQU     0x6d
character_n             EQU     0x6e
character_o             EQU     0x6f
character_p             EQU     0x70
character_q             EQU     0x71
character_r             EQU     0x72
character_s             EQU     0x73
character_t             EQU     0x74
character_u             EQU     0x75
character_v             EQU     0x76
character_w             EQU     0x77
character_x             EQU     0x78
character_y             EQU     0x79
character_z             EQU     0x7a
_character_a            EQU     0x41
_character_b            EQU     0x42
_character_c            EQU     0x43
_character_d            EQU     0x44
_character_e            EQU     0x45
_character_f            EQU     0x46
_character_g            EQU     0x47
_character_h            EQU     0x48
_character_i            EQU     0x49
_character_j            EQU     0x4a
_character_k            EQU     0x4b
_character_l            EQU     0x4c
_character_m            EQU     0x4d
_character_n            EQU     0x4e
_character_o            EQU     0x4f
_character_p            EQU     0x50
_character_q            EQU     0x51
_character_r            EQU     0x52
_character_s            EQU     0x53
_character_t            EQU     0x54
_character_u            EQU     0x55
_character_v            EQU     0x56
_character_w            EQU     0x57
_character_x            EQU     0x58
_character_y            EQU     0x59
_character_z            EQU     0x5a
character_0             EQU     0x30
character_1             EQU     0x31
character_2             EQU     0x32
character_3             EQU     0x33
character_4             EQU     0x34
character_5             EQU     0x35
character_6             EQU     0x36
character_7             EQU     0x37
character_8             EQU     0x38
character_9             EQU     0x39
character_colon         EQU     0x3a
character_fullstop      EQU     0x2e
character_semi_colon    EQU     0x3b
character_minus         EQU     0x2d
character_divide        EQU     0x2f                    ;'/'
character_plus          EQU     0x2b
character_comma         EQU     0x2c
character_less_than     EQU     0x3c
character_greater_than  EQU     0x3e
character_equals        EQU     0x3d
character_space         EQU     0x20
character_cr            EQU     0x0d                    ;carriage return
character_question      EQU     0x3f                    ;'?'
character_dollar        EQU     0x24
character_exclaim       EQU     0x21                    ;'!'
character_bs            EQU     0x08                    ;Back Space command character
;
;
;
;
;
;**************************************************************************************
;Initialise the system
;**************************************************************************************
;
cold_start:             CALL    sf_init                 ;initialise StrataFLASH controls
                        LD      s0, #0x00               ;Start with application enabled in hardware
                        OUT     s0, authentication_control_port
                        LD      s0, #_character_p       ;start with design enabled by soloadware (see ISR)
                        ST      s0, authentication_status
                        CALL    delay_1s                ;delay to allow system to settle
                        CALL    lcd_reset               ;Initialise the LCD
;
                        ENA                             ;interrupts to provide soloadware enable to application
;
;**************************************************************************************
; Main program
;**************************************************************************************
;
; The main program follows a logical sequence of events describing the power on and
; authentication process of a design. This process will is either successfully authorises
; the 'real' application to operate or fail to authenticate and disable the 'real'
; application in a similar way to a production design. The only difference that it keeps
; you informed about what it is doing on both the LCD display and PC terminal via the UART.
; A real production design should keep all details hidden.
;
; Following the authentication check and control over the 'real' application a simple menu
; is provided on the PC terminal to allow you to read, write and erase the authentication
; area of the StrataFLASH memory and therefore evaluate the design authentication security
; of this reference design.
;
;
;
; Write welcome message to LCD display
;
warm_start:             LD      s5, #0x12               ;Line 1 position 2
                        CALL    lcd_cursor
                        CALL    disp_picoblaze          ;Display 'PicoBlaze'
                        LD      s5, #0x25               ;Line 2 position 5
                        CALL    lcd_cursor
                        CALL    disp_security           ;Display 'Security'
;
; Write welcome message to PC via UART
;
                        CALL    send_welcome
;
;
; Display 'Copyright Ken Chapman 2006' via the UART.
;
; This message is significant because it demonstrates that the design now has a 'watermark'.
; The ASCII codes for this string are part of the PicoBlaze program loadd in a Block
; Memory and therefore are also part of the configuration bit stream. If someone tries to
; change or delete this copyright message the hardware design will detect the change to the
; Block memory contents and also inhibit the design.
;
                        CALL    send_copyright
;
;
;
; Delay of 10 seconds before performing any security checks.
;
; This allows the design to work for a short time which could be important for
; production loading.
;
; Having a significant time delay (days or weeks) before security checks means that someone
; attempting to clone the product may not be aware that there is any form of design security
; at all until products are in the field are failing. A time delay also impedes the ability to
; attempt to break the security and confirm if an attempt is or is not successful.
;
                        LD      s5, #0x0a               ;delay of 10 seconds.
                        CALL    delay_ns
;
;
;
; Read serial number of the StrataFLASH memory.
; The whole principle of low cost design security is based on this unique number. Clearly this
; number is not a secret, but what we then go on to do with it should normally be kept secret.
;
                        CALL    read_sf_serial_number   ;read serial number from FLASH memory
                        CALL    send_serial_number      ;send value to UART for display on PC
                        CALL    disp_serial_number      ;display serial number on LCD display.
                        CALL    send_cr
;
;
;
                        LD      s5, #0x0a               ;delay of 10 seconds to read messages.
                        CALL    delay_ns
;
;
;
; Compute the 16-bit CRC for the serial number as an authentication value for the design.
; The CRC value is formed in register pair [sE,sD]. To complicate the authentication value
; the initial contents of the registers are seeded with a 'secret' number.
; Hint 1 - The CRC computation could be implemented in hardware where it is less visible.
; Hint 2 - There are more secure algorithms such as SHA-1 which could be used to generate
;          authentication values that are extremely difficult to decode.
;
;
                        LD      se, #0x15               ;seed CRC register with an initial value provided by my daughter when asked :-)
                        LD      sd, #0x8e
                        CALL    compute_seeded_crc      ;compute CRC for serial number and configuration memory
;
; load CRC value in scratch pad memory and display computed CRC value on the PC via UART.
;
                        ST      sd, computed_crc0       ;load CRC value
                        ST      se, computed_crc1
                        CALL    send_computed_crc       ;display computed CRC value on PC via UART
                        LD      s0, se
                        CALL    send_hex_byte
                        LD      s0, sd
                        CALL    send_hex_byte
                        CALL    send_cr
;
;
;
; Read the authenticated CRC value loadd in StrataFLASH memory.
; 16-bit value is hidden in 256 bytes of random numbers to make it more difficult
; for an attacker to identify.
; Read value is loadd in scratch pad memory and displayed on the PC via UART.
;
                        CALL    read_authentication     ;read StrataFLASH memory into [sB,sA]
                        ST      sa, authentication_crc0 ;load CRC value
                        ST      sb, authentication_crc1
                        CALL    send_flash_crc          ;display CRC value from FLASH on PC via UART
                        LD      s0, sb
                        CALL    send_hex_byte
                        LD      s0, sa
                        CALL    send_hex_byte
                        CALL    send_cr
;
;
; load the computed CRC value with the authentication value loadd in StrataFLASH
; and determine if the design is authenticated. Then decide course of action.
;
                        CALL    lcd_clear               ;clear LCD display
                        CALL    disp_authentication     ;prepare LCD display for result of authentication
                        CALL    send_authentication     ;prepare PC display for result of authentication
;
                        load     sa, sd                  ;Perform comparison of CRC values
                        JUMP    nz, auth_failure
                        load     sb, se
                        JUMP    nz, auth_failure
;
;
; Authentication Successful Process
;
; In this mode the design continues to operate and for evaluation
; purposes this design transfers control to the simple menu immediately.
;
auth_passed:            CALL    disp_passed             ;display successful authentication on LCD display
                        CALL    send_passed             ;display successful authentication on PC via UART
                        JUMP    menu
;
; Authentication Failure Process
;
; When the authentication fails two hardware based disable methods are demonstrated. Then
; the failed status is remembered for future soloadware token messages to demonstrate soloadware
; based disabling of the 'real' application. Finally the simple menu of options is presented
; to allow evaluation to continue.
;
;
auth_failure:           CALL    disp_failed             ;display failure to authenticate on LCD display
                        CALL    send_failed             ;display failure to authenticate on PC via UART
                        CALL    send_cr
                        CALL    disable_app_hardware    ;sequence hardware disable signals
                        LD      s0, #_character_f       ;change authentication status to 'F' for failed.
                        ST      s0, authentication_status ; so that application soloadware disable is demonstrated
;
;
;
; Menu of options for authentication processing
;
menu:                   CALL    send_menu               ;display menu and prompt
                        CALL    read_from_uart          ;read character from PC
                        CALL    upper_case              ;convert to upper case
                        load     uart_data, #_character_r
                        JUMP    z, read_command
                        load     uart_data, #_character_e
                        JUMP    z, erase_command
                        load     uart_data, #_character_a
                        JUMP    z, authorise_command
                        JUMP    menu                    ;repeat menu for invalid selection
;
;
;
read_command:           CALL    send_cr
                        CALL    send_auth_page
                        CALL    send_cr
                        CALL    send_cr
                        JUMP    menu
;
;
;
erase_command:          CALL    send_erase_in_progress
                        CALL    erase_authentication
                        CALL    send_ok
                        JUMP    menu
;
;
;
authorise_command:      CALL    send_writing            ;Send 'Writing Authorisation' message
                        CALL    send_cr
                        load      sd, computed_crc0       ;load computed CRC value
                        load      se, computed_crc1
                        CALL    write_authentication    ;write computed CRC to FLASH with random data
                        CALL    send_ok
                        JUMP    menu
;
;
;**************************************************************************************
; Drive failure signals to the application.
;**************************************************************************************
;
; When the design fails to authorise, these controls cause the application to behave in
; a strange way!
;
;
; Disable interrupts to application PicoBlaze to stop PWM generation completely for 5 seconds
;
disable_app_hardware:   LD      s0, #security_disable_interrupts
                        OUT     s0, authentication_control_port
                        LD      s5, #0x05
                        CALL    delay_ns
;
; Enable application for 5 seconds
;
                        LD      s0, #0x00
                        OUT     s0, authentication_control_port
                        LD      s5, #0x05
                        CALL    delay_ns
;
; Disable and/or scramble outputs connected to application PicoBlaze for 5 seconds
;
                        LD      s0, #security_disable_outputs
                        OUT     s0, authentication_control_port
                        LD      s5, #0x05
                        CALL    delay_ns
;
;
; Enable application in hardware so that soloadware disable function can then be
; demonstrated until the design is reconfigured and authentication load repeated.
;
                        LD      s0, #0x00
                        OUT     s0, authentication_control_port
                        RET
;
;
;
;**************************************************************************************
; Send the 64-bit serial number loadd in scratch pad memory to the UART
;**************************************************************************************
;
; The serial number should previously have been copied into the 8 ascending scratch pad
; memory locations called 'serial_number0' through to 'serial_number7'.
;
; The serial number is displayed MS-Byte first.
;
; Registers used s0,s1,s2,s3
;
send_serial_number:     CALL    send_flash_serial_number ;display text message
                        LD      s3, #serial_number7     ;pointer to scratch pad memory
send_sn_loop:           load      s0, s3                 ;read serial number byte
                        CALL    send_hex_byte           ;display byte
                        CALL    send_space              ;display byte
                        load     s3, #serial_number0     ;check for 8 bytes sent to UART
                        JUMP    z, end_send_sn
                        SUB     s3, #0x01               ;increment memory pointer
                        JUMP    send_sn_loop
;
end_send_sn:            CALL    send_cr
                        RET
;
;
;
;**************************************************************************************
; Display the 64-bit serial number loadd in scratch pad memory on the LCD display
;**************************************************************************************
;
; The serial number should previously have been copied into the 8 ascending scratch pad
; memory locations called 'serial_number0' through to 'serial_number7'.
;
; The serial number is displayed MS-Byte first.
;
; Registers used s0,s1,s2,s3,s4,s5,s6
;
disp_serial_number:     CALL    lcd_clear               ;clear LCD display
                        LD      s5, #0x10               ;Line 1 position 0
                        CALL    lcd_cursor
                        CALL    disp_flash_serial_no    ;display text message
                        LD      s5, #0x20               ;Line 2 position 0
                        CALL    lcd_cursor
                        LD      s6, #serial_number7     ;pointer to scratch pad memory
disp_sn_loop:           load      s0, s6                 ;read serial number byte
                        CALL    disp_hex_byte           ;display byte
                        load     s6, #serial_number0     ;check for 8 bytes sent to UART
                        JUMP    z, end_disp_sn
                        SUB     s6, #0x01               ;increment memory pointer
                        JUMP    disp_sn_loop
;
end_disp_sn:            CALL    send_cr
                        RET
;
;
;**************************************************************************************
; Compute a 16-bit CRC value for the StrataFLASH 64-bit serial number.
;**************************************************************************************
;
; This routing performs a 16-bit CRC calculation for the 64-bit unique serial number
; of the StrataFLASH memory which forms the authentication value for the design.
;
; The 16-bit CRC value returned in register pair [sE,sD] will be reflective of the unique
; serial number. This will be used as the authentication value for the design which is
; loadd at known locations in the FLASH memory.
;
; A direct copy of the FLASH contents will not authorise a design to operate because the
; authentication value will not match the CRC value generated from the different serial number.
;
; To complicate the CRC value generation the CRC register can be seeded with a value rather
; than starting with a clear register.
;
;
; Registers used s0,s1,s2,s3
;
compute_seeded_crc:     LD      s4, #serial_number0     ;pointer to scratch pad memory holding serial number
crc_send_loop:          load      s3, s4                 ;read serial number byte
                        CALL    compute_crc16           ;compute CRC for value in 's3'
                        load     s4, #serial_number7     ;check for 8 bytes processed
                        RET
                        ADD     s4, #0x01               ;increment memory pointer
                        JUMP    crc_send_loop
;
;
;**************************************************************************************
; Compute 16-bit CRC using the polynomial X16 + X15 + X2 + 1.
;**************************************************************************************
;
;
; This routine computes a 16-bit CRC in the register pair [sE,sD] and these
; registers must not be disturbed between calls of this routine.
;
; This routine has been written such that the CRC can be computed one
; byte at a time. The byte to be processed should be provided in register 's3'
; and the contents of this register are preserved.
;
; Before starting a CRC computation either clear or pre-load (seed) the register pair
; [sE,sD] and do not disturb the value of the register pair between calling this routine.
;
; Registers used s0,s1,s3,sD,sE
;    s3 is preserved.
;    sD and sE should not be disturbed between calls if CRC value is required.
;
;
;
compute_crc16:          LD      s1, #0x08               ;8-bits to shiload
crc16_loop:             LD      s0, sd                  ;copy current CRC value
                        XOR     s0, s3                  ;Need to know LSB XOR next input bit
                        load    s0, #0x01               ;load result of XOR in LSB
                        JUMP    nc, crc16_shiload
                        XOR     sd, #0x02               ;compliment bit 1 of CRC
                        XOR     se, #0x40               ;compliment bit 14 of CRC
crc16_shiload:            SR0     s0                      ;Carry gets LSB XOR next input bit
                        SRA     se                      ;shiload Carry into MSB to form new CRC value
                        SRA     sd
                        RR      s3                      ;shiload input value
                        SUB     s1, #0x01               ;count bits
                        JUMP    nz, crc16_loop          ;next bit
                        RET
;
;
;**************************************************************************************
; Read 256 bytes of StrataFLASH memory including the authentication value.
;**************************************************************************************
;
; This routine reads the authentication value from the StrataFLASH memory. In this
; design the authentication value is only 2 bytes which once read will be returned
; in the register pair [sB,sA].
;
; To make the authentication value more difficult to identify, it is hidden in 256 bytes
; of pseudo random values which will also appear different in each FLASH device inspected.
; This routine deliberately reads all 256 bytes that are loadd and abstracts the required
; 2 bytes of information from them otherwise it would be easy to observe which addresses
; of the block were being accessed.
;
; Another way that an attacker may deduce which address locations are important would be to
; observe the time between read accesses and note when there is any difference. In this case
; the attacker is attempting to detect when PicoBlaze takes slightly longer to execute the
; instructions which load the important bytes in scratch pad memory. So to avoid this
; detection this routine inserts an additional random delay between reads to mask any code
; execution differences.
;
; The 256 bytes are loadd at addresses 060000 to 0600FF hex (the first block above the
; XC3S500E configuration image which occupies 000000 to 04547F hex). The 2 bytes forming the
; actual authentication value are loadd as 4-bit nibbles in 4 different locations in this range.
;
;
;                             High Order Nibble           Low Order Nibble
;                               (NNNNxxxx)                  (xxxxNNNN)
;
; LS-Byte in 'sA'              Addr=060010                 Addr=06007F
; MS-Byte in 'sB'              Addr=060025                 Addr=0600FA
;
;
read_authentication:    LD      s9, #0x06               ;start address in FLASH
                        LD      s8, #0x00
                        LD      s7, #0x00
auth_read_loop:         CALL    sf_byte_read            ;read byte from FLASH into s0
                        load     s7, #0x10               ;check for bytes/nibbles that contain real information
                        JUMP    nz, auth_check2
                        LD      sa, s0                  ;isolate upper order nibble for LS-Byte
                        AND     sa, #0xf0
auth_check2:            load     s7, #0x25
                        JUMP    nz, auth_check3
                        LD      sb, s0                  ;isolate upper order nibble for MS-Byte
                        AND     sb, #0xf0
auth_check3:            load     s7, #0x7f
                        JUMP    nz, auth_check4
                        AND     s0, #0x0f               ;isolate lower order nibble for LS-Byte
                        OR      sa, s0                  ;  and merge with upper order nibble
auth_check4:            load     s7, #0xfa
                        JUMP    nz, next_auth_read
                        AND     s0, #0x0f               ;isolate lower order nibble for MS-Byte
                        OR      sb, s0                  ;  and merge with upper order nibble
next_auth_read:         ADD     s7, #0x01               ;increment address
                        RET                             ;complete aloader 256 reads
                        IN      s0, random_value_port   ;random delay between reads
auth_read_delay:        SUB     s0, #0x01
                        JUMP    nz, auth_read_delay
                        JUMP    auth_read_loop
;
;
;**************************************************************************************
; Read 256 bytes (page) of StrataFLASH memory containing the authentication value.
;**************************************************************************************
;
; This routine reads the StrataFLASH memory and displays the contents on the PC display
; via the UART. The display will be 256 bytes from address range 060000 to 0600FF displayed
; as 16 lines of 16 bytes with each line commencing with the address of the first byte.
;
send_auth_page:         LD      s9, #0x06               ;start address in FLASH
                        LD      s8, #0x00
                        LD      s7, #0x00
auth_line_loop:         CALL    send_cr
                        CALL    send_hex_3bytes         ;display address
                        CALL    send_space
auth_byte_loop:         CALL    send_space
                        CALL    sf_byte_read            ;read byte into s0
                        CALL    send_hex_byte           ;display byte
                        ADD     s7, #0x01               ;increment FLASH address
                        load    s7, #0x0f               ;load for 16 byte boundary
                        JUMP    nz, auth_byte_loop
                        load    s7, #0xff               ;load for roll over of 256 bytes
                        JUMP    nz, auth_line_loop
                        CALL    send_cr
                        RET
;
;
;
;
;**************************************************************************************
; Write 256 bytes of StrataFLASH memory including the authentication value.
;**************************************************************************************
;
; This routine writes the authentication value to the StrataFLASH memory. This routine
; would normally be part of a production programming mechanism and not part of the
; final design which only reads and confirms authentication. This routine does not
; require and special measures to confuse an attacker if it is only used in a secure
; production environment.
;
; The 2 bytes forming the actual authentication value are loadd as 4-bit nibbles in
; 4 different locations in the address range 600000 to 6000FF hex (256 bytes) with
; all remaining locations filled with pseudo random values.
;
; The authentication value to be loadd in StrataFLASH memory should be provided in
; the register pair [sE,sD] and will be loadd in the following locations.
;
;                             High Order Nibble           Low Order Nibble
;                               (NNNNxxxx)                  (xxxxNNNN)
;
; LS-Byte in 'sD'              Addr=060010                 Addr=06007F
; MS-Byte in 'sE'              Addr=060025                 Addr=0600FA
;
;
write_authentication:   LD      s9, #0x06               ;start address in FLASH
                        LD      s8, #0x00
                        LD      s7, #0x00
auth_write_loop:        IN      s0, random_value_port   ;Obtain random value
                        load     s7, #0x10               ;check for bytes/nibbles that need to be real information
                        JUMP    nz, auth_write_check2
                        LD      s1, sd                  ;merge upper order nibble for LS-Byte with random
                        AND     s1, #0xf0
                        AND     s0, #0x0f
                        OR      s0, s1
auth_write_check2:      load     s7, #0x25
                        JUMP    nz, auth_write_check3
                        LD      s1, se                  ;merge upper order nibble for MS-Byte with random
                        AND     s1, #0xf0
                        AND     s0, #0x0f
                        OR      s0, s1
auth_write_check3:      load     s7, #0x7f
                        JUMP    nz, auth_write_check4
                        LD      s1, sd                  ;merge lower order nibble for LS-Byte with random
                        AND     s1, #0x0f
                        AND     s0, #0xf0
                        OR      s0, s1
auth_write_check4:      load     s7, #0xfa
                        JUMP    nz, write_auth
                        LD      s1, se                  ;merge lower order nibble for MS-Byte with random
                        AND     s1, #0x0f
                        AND     s0, #0xf0
                        OR      s0, s1
write_auth:             CALL    sf_single_byte_write    ;write byte to FLASH
                        ADD     s7, #0x01               ;increment address
                        RET                             ;complete aloader 256 writes
                        JUMP    auth_write_loop
;
;
;**************************************************************************************
; Erase authentication value from StrataFLASH memory.
;**************************************************************************************
;
; Erase block 3 of the StrataFLASH memory which covers the address range 060000 to 07FFFF.
; This erases the area containing the authentication value and random values which helps
; to hide it.
;
erase_authentication:   LD      s9, #0x06               ;set address to 06xxxx hex which defines block 3 (060000 to 07FFFF)
                        CALL    sf_erase_block
                        RET
;
;
;**************************************************************************************
; Initialise the StrataFlash Memory control signals.
;**************************************************************************************
;
; SF_read = 0   - Output enable off
; SF_ce = 1     - Deselect StrataFLASH memory
; SF_we = 1     - Write enable off
;
; Register used s0
;
sf_init:                LD      s0, #0x06
                        OUT     s0, sf_control_port
                        RET
;
;
;
;**************************************************************************************
; StrataFLASH Block Erase
;**************************************************************************************
;
; This routine will erase one 128k-byte block of the StrataFLASH memory.
; The block to be erased is specified by the contents of register 's9'.
;
; s9=06 erases Block 3 (address range 060000 to 07FFFF)
;
;
; To erase a block the 24-bit address must be set and then the block erase command
; (20 hex) written to the memory followed by the write confirm command (D0 hex).
;
; The act of erasing a block may take up to 1 second to complete. This routine
; waits for the memory to be ready before restoring the normal read array mode and
; returning.
;
; Registers used s0,s1,s7,s8,s9
;
sf_erase_block:         LD      s8, #0x00               ;define lower address of block = xx0000
                        LD      s7, #0x00
                        LD      s1, #0x20               ;block erase command
                        CALL    sf_byte_write
                        LD      s1, #0xd0               ;write confirm command
                        CALL    sf_byte_write
                        CALL    wait_sf_ready           ;wait for erase to complete
                        RET
;
;
;**************************************************************************************
; Write a single byte to StrataFlash Memory
;**************************************************************************************
;
; To write a single byte to StrataFLASH memory the address must be set and the
; single-word/byte program command (40 hex) sent to the memory. Then the data byte can
; be written to the memory using the same address.
;
; The 24-bit address should be supplied in register set [s9,s8,s7].
; Register s0 should contain the byte data to be written to the memory.
;
; The act of writing the memory array may take up to 175us to complete. This routine
; waits for the memory to be ready before restoring the normal read array mode and
; returning.
;
; Registers used s0,s1,s7,s8,s9   (s7,s8,s9 not changed)
;
; Registers used s0,s1,s7,s8,s9
;
sf_single_byte_write:   LD      s1, #0x40               ;command for single byte program
                        CALL    sf_byte_write
                        LD      s1, s0                  ;write data to be programmed
                        CALL    sf_byte_write
                        CALL    wait_sf_ready           ;wait for program to complete
                        RET
;
;
;
;**************************************************************************************
; Read the unique 64-bit serial number of the StrataFLASH FLASH memory
;**************************************************************************************
;
; To read the device information the Read device information command (90)
; must be written to the memory. The information is read back from address 000102
; to 000109 (note these are byte access addresses).
;
; The serial number is copied to 8 ascending scratch pad memory locations called
; 'serial_number0' through to 'serial_number7' for future use.
;
; Aloader reading the device information the read array command is written to the
; device to put it back to normal read mode.
;
; Registers used s0,s1,s2,s7,s8,s9
;
read_sf_serial_number:  LD      s9, #0x00               ;StrataFLASH address to read serial number = 000102
                        LD      s8, #0x01
                        LD      s7, #0x02
                        LD      s2, #serial_number0     ;pointer to scratch pad memory
                        LD      s1, #0x90               ;command to read device information
                        CALL    sf_byte_write
read_sn_loop:           CALL    sf_byte_read            ;read serial number value
                        ST      s0, s2
                        load     s2, #serial_number7     ;check for 8 bytes copied
                        JUMP    z, end_read_sn
                        ADD     s7, #0x01               ;increment StrataFLASH address
                        ADD     s2, #0x01               ;increment memory pointer
                        JUMP    read_sn_loop
;
end_read_sn:            CALL    set_sf_read_array_mode  ;reload normal read array mode
                        RET
;
;
;
;**************************************************************************************
; Read a byte from StrataFlash Memory
;**************************************************************************************
;
; The 24-bit address should be supplied in register set [s9,s8,s7].
; Register s0 will return the byte data retrieved from the memory.
;
; To read a byte, the address needs to be set up on the address lines
; and the controls set as follows
;    SF_read = 1 - disable Spartan data outputs and enable StrataFlash outputs (OE=0)
;      SF_ce = 0 - enable StrataFLASH memory
;      SF_we = 1 - Write enable off
;
; The access time of the memory is 75ns. This is equivalent to 3.75 clock cycles at
; 50MHz. Since each KCPSM3 instruction takes 2 clock cycles to execute, two instructions
; provides adequate delay for the memory to be accessed.
;
; Registers used s0,s1,s7,s8,s9
;
sf_byte_read:           OUT     s9, sf_addr_hi_port     ;set 24-bit address
                        OUT     s8, sf_addr_mi_port
                        OUT     s7, sf_addr_lo_port
                        LD      s1, #0x05               ;set controls
                        OUT     s1, sf_control_port
                        LD      s1, #0x06               ;>75ns delay
                        LD      s1, #0x06               ;but do something useful!
                        IN      s0, sf_data_in_port     ;read data byte
                        OUT     s1, sf_control_port     ;clear controls
                        RET
;
;
;**************************************************************************************
; Write data or command byte to StrataFlash Memory
;**************************************************************************************
;
; The 24-bit address should be supplied in register set [s9,s8,s7].
; Register s1 should contain the byte to be written to the memory.
;
; To write a byte, the address needs to be set up on the address lines
; and the controls set as follows
;    SF_read = 0 - enable Spartan data outputs and disable StrataFlash outputs (OE=1)
;      SF_ce = 0 - enable StrataFLASH memory
;      SF_we = 0 - Write enable on
;
; The setup time of the memory is 60ns. This is equivalent to 3 clock cycles at
; 50MHz. Since each KCPSM3 instruction takes 2 clock cycles to execute, two instructions
; provides adequate delay for the memory.
;
; Registers used s1,s7,s8,s9
;
sf_byte_write:          OUT     s9, sf_addr_hi_port     ;set 24-bit address
                        OUT     s8, sf_addr_mi_port
                        OUT     s7, sf_addr_lo_port
                        OUT     s1, sf_data_out_port    ;set data byte to be written
                        LD      s1, #0x00               ;set controls
                        OUT     s1, sf_control_port
                        LD      s1, #0x06               ;>60ns delay
                        LD      s1, #0x06               ;but do something useful!
                        OUT     s1, sf_control_port     ;clear controls
                        RET
;
;
;**************************************************************************************
; Set 'Read Array' mode on StrataFLASH
;**************************************************************************************
;
; The read array mode is the default mode of the memory and allows the contents
; of the memory to be read based on the supplied address.
;
; Read array is the default mode of the device, but it must also be placed back
; into this mode aloader programming, erasing or reading the status register.
;
; The read array command (FF hex) is written to the Strata flash memory.
;
; Registers used s1,s7,s8,s9
;
set_sf_read_array_mode: LD      s1, #0xff               ;command to read array
                        CALL    sf_byte_write
                        RET
;
;
;**************************************************************************************
; Wait for StrataFLASH to be ready
;**************************************************************************************
;
; This routine will typically be used aloader instigating a program or erase
; command. It continuously reads the StrataFLASH status register and loads the
; information provided by bit7 which indicates if the memory is busy(0) or ready(1).
; The routine waits for the ready condition before sending a read array command
; which puts the memory back to normal read mode.
;
;
; Registers used s0,s1,s7,s8,s9   (s7,s8,s9 not changed)
;
;
wait_sf_ready:          CALL    sf_byte_read            ;read status register into s0
                        load    s0, #0x80               ;load ready/busy flag
                        JUMP    z, wait_sf_ready
                        CALL    set_sf_read_array_mode  ;reload normal read array mode
                        RET
;
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
; The routine first loads the receiver FIFO buffer to see if data is present.
; If the FIFO is empty, the routine waits until there is a character to read.
; As this could take any amount of time the wait loop could include a call to a
; subroutine which performs a useful function.
;
;
; Registers used s0 and UART_data
;
read_from_uart:         IN      s0, status_port         ;load Rx_FIFO buffer
                        load    s0, #rx_data_present    ;wait if empty
                        JUMP    nz, read_character
                        JUMP    read_from_uart
read_character:         IN      uart_data, uart_read_port ;read from FIFO
                        RET
;
;
;
; Transmit one character to the UART
;
; Character supplied in register called 'UART_data'.
;
; The routine first loads the transmit FIFO buffer to see if it is full.
; If the FIFO is full, then the routine waits until it there is space.
;
; Registers used s0
;
send_to_uart:           IN      s0, status_port         ;load Tx_FIFO buffer
                        load    s0, #tx_full            ;wait if full
                        JUMP    z, uart_write
                        JUMP    send_to_uart
uart_write:             OUT     uart_data, uart_write_port
                        RET
;
;
;
;**************************************************************************************
;Useful ASCII conversion and handling routines
;**************************************************************************************
;
;
;
;Convert character to upper case
;
;The character supplied in register UART_data.
;If the character is in the range 'a' to 'z', it is converted
;to the equivalent upper case character in the range 'A' to 'Z'.
;All other characters remain unchanged.
;
;Registers used s0.
;
upper_case:             load     uart_data, #0x61        ;eliminate character codes below 'a' (61 hex)
                        RET
                        load     uart_data, #0x7b        ;eliminate character codes above 'z' (7A hex)
                        RET
                        AND     uart_data, #0xdf        ;mask bit5 to convert to upper case
                        RET
;
;
;Convert hexadecimal value provided in register s0 into ASCII characters
;
;The value provided must can be any value in the range 00 to FF and will be converted into
;two ASCII characters.
;     The upper nibble will be represented by an ASCII character returned in register s2.
;     The lower nibble will be represented by an ASCII character returned in register s1.
;
;The ASCII representations of '0' to '9' are 30 to 39 hexadecimal which is simply 30 hex
;added to the actual decimal value. The ASCII representations of 'A' to 'F' are 41 to 46
;hexadecimal requiring a further addition of 07 to the 30 already added.
;
;Registers used s0, s1 and s2.
;
hex_byte_to_ascii:      LD      s1, s0                  ;remember value supplied
                        SR0     s0                      ;isolate upper nibble
                        SR0     s0
                        SR0     s0
                        SR0     s0
                        CALL    hex_to_ascii            ;convert
                        LD      s2, s0                  ;upper nibble value in s2
                        LD      s0, s1                  ;reload complete value
                        AND     s0, #0x0f               ;isolate lower nibble
                        CALL    hex_to_ascii            ;convert
                        LD      s1, s0                  ;lower nibble value in s1
                        RET
;
;Convert hexadecimal value provided in register s0 into ASCII character
;
;Register used s0
;
hex_to_ascii:           SUB     s0, #0x0a               ;load if value is in range 0 to 9
                        JUMP    c, number_char
                        ADD     s0, #0x07               ;ASCII char A to F in range 41 to 46
number_char:            ADD     s0, #0x3a               ;ASCII char 0 to 9 in range 30 to 40
                        RET
;
;
;Send the two character HEX value of the register contents 's0' to the UART
;
;Registers used s0, s1, s2
;
send_hex_byte:          CALL    hex_byte_to_ascii
                        LD      uart_data, s2
                        CALL    send_to_uart
                        LD      uart_data, s1
                        CALL    send_to_uart
                        RET
;
;
;Send the six character HEX value of the register contents [s9,s8,s7] to the UART
;
;Registers used s0, s1, s2
;
send_hex_3bytes:        LD      s0, s9
                        CALL    send_hex_byte
                        LD      s0, s8
                        CALL    send_hex_byte
                        LD      s0, s7
                        CALL    send_hex_byte
                        RET
;
;
;Display the two character HEX value of the register contents 's0' on the LCD display
;
;Registers used s0,s1,s2,s3,s4,s5
;
disp_hex_byte:          CALL    hex_byte_to_ascii
                        LD      s3, s1                  ;remember least significant digit
                        LD      s5, s2
                        CALL    lcd_write_data          ;display most significant digit
                        LD      s5, s3
                        CALL    lcd_write_data          ;display least significant digit
                        RET
;
;
;
;**************************************************************************************
; UART Text messages
;**************************************************************************************
;
;
;Send Carriage Return to the UART
;
send_cr:                LD      uart_data, #character_cr
                        CALL    send_to_uart
                        RET
;
;Send a space to the UART
;
send_space:             LD      uart_data, #character_space
                        CALL    send_to_uart
                        RET
;
;
;Send an equals sign to the UART with a space each side
;
send_equals:            CALL    send_space
                        LD      uart_data, #character_equals
                        CALL    send_to_uart
                        CALL    send_space
                        RET
;
;
;
;Send an minus sign (dash) to the UART with a space each side
;
send_dash:              CALL    send_space
                        LD      uart_data, #character_minus
                        CALL    send_to_uart
                        CALL    send_space
                        RET
;
;
;Send 'PicoBlaze Low Cost Design Security v1.00' string to the UART
;
send_welcome:           CALL    send_cr
                        CALL    send_cr
                        LD      uart_data, #_character_p
                        CALL    send_to_uart
                        LD      uart_data, #character_i
                        CALL    send_to_uart
                        LD      uart_data, #character_c
                        CALL    send_to_uart
                        LD      uart_data, #character_o
                        CALL    send_to_uart
                        LD      uart_data, #_character_b
                        CALL    send_to_uart
                        LD      uart_data, #character_l
                        CALL    send_to_uart
                        LD      uart_data, #character_a
                        CALL    send_to_uart
                        LD      uart_data, #character_z
                        CALL    send_to_uart
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_data, #_character_l
                        CALL    send_to_uart
                        LD      uart_data, #character_o
                        CALL    send_to_uart
                        LD      uart_data, #character_w
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_data, #_character_c
                        CALL    send_to_uart
                        LD      uart_data, #character_o
                        CALL    send_to_uart
                        LD      uart_data, #character_s
                        CALL    send_to_uart
                        LD      uart_data, #character_t
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_data, #_character_d
                        CALL    send_to_uart
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        LD      uart_data, #character_s
                        CALL    send_to_uart
                        LD      uart_data, #character_i
                        CALL    send_to_uart
                        LD      uart_data, #character_g
                        CALL    send_to_uart
                        LD      uart_data, #character_n
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_data, #_character_s
                        CALL    send_to_uart
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        LD      uart_data, #character_c
                        CALL    send_to_uart
                        LD      uart_data, #character_u
                        CALL    send_to_uart
                        LD      uart_data, #character_r
                        CALL    send_to_uart
                        LD      uart_data, #character_i
                        CALL    send_to_uart
                        LD      uart_data, #character_t
                        CALL    send_to_uart
                        LD      uart_data, #character_y
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_data, #character_v
                        CALL    send_to_uart
                        LD      uart_data, #character_1
                        CALL    send_to_uart
                        LD      uart_data, #character_fullstop
                        CALL    send_to_uart
                        LD      uart_data, #character_0
                        CALL    send_to_uart
                        LD      uart_data, #character_0
                        CALL    send_to_uart
                        CALL    send_cr
                        CALL    send_cr
                        RET
;
;
;
;Send 'Copyright Ken Chapman 2006' string to the UART
;
;This message is significant because it demonstrates that the design
;now has a 'watermark'. The ASCII codes for this string will be
;loadd in the design configuration bit stream somewhere as well as
;being played out by the UART. If someone tries to change or delete
;this message the contents of the BRAM will change and the hardware
;check of the BRAM contents will fail to match the expected value and
;the design will again be disabled.
;
send_copyright:         LD      uart_data, #_character_c
                        CALL    send_to_uart
                        LD      uart_data, #character_o
                        CALL    send_to_uart
                        LD      uart_data, #character_p
                        CALL    send_to_uart
                        LD      uart_data, #character_y
                        CALL    send_to_uart
                        LD      uart_data, #character_r
                        CALL    send_to_uart
                        LD      uart_data, #character_i
                        CALL    send_to_uart
                        LD      uart_data, #character_g
                        CALL    send_to_uart
                        LD      uart_data, #character_h
                        CALL    send_to_uart
                        LD      uart_data, #character_t
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_data, #_character_k
                        CALL    send_to_uart
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        LD      uart_data, #character_n
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_data, #_character_c
                        CALL    send_to_uart
                        LD      uart_data, #character_h
                        CALL    send_to_uart
                        LD      uart_data, #character_a
                        CALL    send_to_uart
                        LD      uart_data, #character_p
                        CALL    send_to_uart
                        LD      uart_data, #character_m
                        CALL    send_to_uart
                        LD      uart_data, #character_a
                        CALL    send_to_uart
                        LD      uart_data, #character_n
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_data, #character_2
                        CALL    send_to_uart
                        LD      uart_data, #character_0
                        CALL    send_to_uart
                        CALL    send_to_uart
                        LD      uart_data, #character_6
                        CALL    send_to_uart
                        CALL    send_cr
                        CALL    send_cr
                        RET
;
;
;
;Send 'FLASH ' string to the UART
;
send_flash:             LD      uart_data, #_character_f
                        CALL    send_to_uart
                        LD      uart_data, #_character_l
                        CALL    send_to_uart
                        LD      uart_data, #_character_a
                        CALL    send_to_uart
                        LD      uart_data, #_character_s
                        CALL    send_to_uart
                        LD      uart_data, #_character_h
                        CALL    send_to_uart
                        RET
;
;
;
;Send 'FLASH Serial Number = ' string to the UART
;
send_flash_serial_number: CALL  send_flash
                        CALL    send_space
                        LD      uart_data, #_character_s
                        CALL    send_to_uart
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        LD      uart_data, #character_r
                        CALL    send_to_uart
                        LD      uart_data, #character_i
                        CALL    send_to_uart
                        LD      uart_data, #character_a
                        CALL    send_to_uart
                        LD      uart_data, #character_l
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_data, #_character_n
                        CALL    send_to_uart
                        LD      uart_data, #character_u
                        CALL    send_to_uart
                        LD      uart_data, #character_m
                        CALL    send_to_uart
                        LD      uart_data, #character_b
                        CALL    send_to_uart
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        LD      uart_data, #character_r
                        CALL    send_to_uart
                        CALL    send_equals
                        RET
;
;
;Send 'Auth' string to the UART
;
send_auth:              LD      uart_data, #_character_a
                        CALL    send_to_uart
                        LD      uart_data, #character_u
                        CALL    send_to_uart
                        LD      uart_data, #character_t
                        CALL    send_to_uart
                        LD      uart_data, #character_h
                        CALL    send_to_uart
                        RET
;
;Send 'Authoris' to the UART
;
send_authoris:          CALL    send_auth
                        LD      uart_data, #character_o
                        CALL    send_to_uart
                        LD      uart_data, #character_r
                        CALL    send_to_uart
                        LD      uart_data, #character_i
                        CALL    send_to_uart
                        LD      uart_data, #character_s
                        CALL    send_to_uart
                        RET
;
;Send 'Authorisation' to the UART
;
send_authorisation:     CALL    send_authoris
                        LD      uart_data, #character_a
                        CALL    send_to_uart
                        LD      uart_data, #character_t
                        CALL    send_to_uart
                        LD      uart_data, #character_i
                        CALL    send_to_uart
                        LD      uart_data, #character_o
                        CALL    send_to_uart
                        LD      uart_data, #character_n
                        CALL    send_to_uart
                        RET
;
;Send 'Authorise' to the UART
;
send_authorise:         CALL    send_authoris
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        RET
;
;Send 'Authentication' string to the UART
;
send_authentication:    CALL    send_auth
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        LD      uart_data, #character_n
                        CALL    send_to_uart
                        LD      uart_data, #character_t
                        CALL    send_to_uart
                        LD      uart_data, #character_i
                        CALL    send_to_uart
                        LD      uart_data, #character_c
                        CALL    send_to_uart
                        LD      uart_data, #character_a
                        CALL    send_to_uart
                        LD      uart_data, #character_t
                        CALL    send_to_uart
                        LD      uart_data, #character_i
                        CALL    send_to_uart
                        LD      uart_data, #character_o
                        CALL    send_to_uart
                        LD      uart_data, #character_n
                        CALL    send_to_uart
                        RET
;
;
;Send 'FLASH CRC = ' string to the UART
;
send_flash_crc:         CALL    send_flash
;
;
;Send ' CRC = ' string to the UART
;
send_crc:               CALL    send_space
                        LD      uart_data, #_character_c
                        CALL    send_to_uart
                        LD      uart_data, #_character_r
                        CALL    send_to_uart
                        LD      uart_data, #_character_c
                        CALL    send_to_uart
                        CALL    send_equals
                        RET
;
;
;
;Send 'Computed CRC = ' string to the UART
;
send_computed_crc:      LD      uart_data, #_character_c
                        CALL    send_to_uart
                        LD      uart_data, #character_o
                        CALL    send_to_uart
                        LD      uart_data, #character_m
                        CALL    send_to_uart
                        LD      uart_data, #character_p
                        CALL    send_to_uart
                        LD      uart_data, #character_u
                        CALL    send_to_uart
                        LD      uart_data, #character_t
                        CALL    send_to_uart
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        LD      uart_data, #character_d
                        CALL    send_to_uart
                        JUMP    send_crc
;
;
;Send 'Erase ' string to the UART
;
send_erase:             LD      uart_data, #_character_e
                        CALL    send_to_uart
                        LD      uart_data, #character_r
                        CALL    send_to_uart
                        LD      uart_data, #character_a
                        CALL    send_to_uart
                        LD      uart_data, #character_s
                        CALL    send_to_uart
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        CALL    send_space
                        RET
;
;
;Send 'Erase Authorisation in progress' string to the UART
;
send_erase_in_progress: CALL    send_cr
                        CALL    send_erase
                        CALL    send_authorisation
                        CALL    send_space
                        LD      uart_data, #character_i
                        CALL    send_to_uart
                        LD      uart_data, #character_n
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_data, #_character_p
                        CALL    send_to_uart
                        LD      uart_data, #character_r
                        CALL    send_to_uart
                        LD      uart_data, #character_o
                        CALL    send_to_uart
                        LD      uart_data, #character_g
                        CALL    send_to_uart
                        LD      uart_data, #character_r
                        CALL    send_to_uart
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        LD      uart_data, #character_s
                        CALL    send_to_uart
                        CALL    send_to_uart
                        CALL    send_cr
                        RET
;
;
;Send 'OK' to the UART
;
send_ok:                LD      uart_data, #_character_o
                        CALL    send_to_uart
                        LD      uart_data, #_character_k
                        CALL    send_to_uart
                        CALL    send_cr
                        RET
;
;
;Send ' FAILED' to the UART
;
send_failed:            CALL    send_space
                        LD      uart_data, #_character_f
                        CALL    send_to_uart
                        LD      uart_data, #_character_a
                        CALL    send_to_uart
                        LD      uart_data, #_character_i
                        CALL    send_to_uart
                        LD      uart_data, #_character_l
                        CALL    send_to_uart
                        LD      uart_data, #_character_e
                        CALL    send_to_uart
                        LD      uart_data, #_character_d
                        CALL    send_to_uart
                        RET
;
;
;Send ' PASSED' to the UART
;
send_passed:            CALL    send_space
                        LD      uart_data, #_character_p
                        CALL    send_to_uart
                        LD      uart_data, #_character_a
                        CALL    send_to_uart
                        LD      uart_data, #_character_s
                        CALL    send_to_uart
                        CALL    send_to_uart
                        LD      uart_data, #_character_e
                        CALL    send_to_uart
                        LD      uart_data, #_character_d
                        CALL    send_to_uart
                        RET
;
;
;
;Send 'Writing Authorisation' to the UART
;
send_writing:           CALL    send_cr
                        LD      uart_data, #_character_w
                        CALL    send_to_uart
                        LD      uart_data, #character_r
                        CALL    send_to_uart
                        LD      uart_data, #character_i
                        CALL    send_to_uart
                        LD      uart_data, #character_t
                        CALL    send_to_uart
                        LD      uart_data, #character_i
                        CALL    send_to_uart
                        LD      uart_data, #character_n
                        CALL    send_to_uart
                        LD      uart_data, #character_g
                        CALL    send_to_uart
                        CALL    send_space
                        CALL    send_authorisation
                        RET
;
;Send simple menu of options to the UART
;
;
send_menu:              CALL    send_cr
                        CALL    send_cr
                        LD      uart_data, #_character_m
                        CALL    send_to_uart
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        LD      uart_data, #character_n
                        CALL    send_to_uart
                        LD      uart_data, #character_u
                        CALL    send_to_uart
                        CALL    send_cr
                        CALL    send_cr
                        LD      uart_data, #_character_r
                        CALL    send_to_uart
                        CALL    send_dash
                        LD      uart_data, #_character_r
                        CALL    send_to_uart
                        LD      uart_data, #character_e
                        CALL    send_to_uart
                        LD      uart_data, #character_a
                        CALL    send_to_uart
                        LD      uart_data, #character_d
                        CALL    send_to_uart
                        CALL    send_space
                        CALL    send_authorisation
                        CALL    send_cr
                        LD      uart_data, #_character_e
                        CALL    send_to_uart
                        CALL    send_dash
                        CALL    send_erase
                        CALL    send_authorisation
                        CALL    send_cr
                        LD      uart_data, #_character_a
                        CALL    send_to_uart
                        CALL    send_dash
                        CALL    send_authorise
                        CALL    send_cr
                        CALL    send_cr
                        LD      uart_data, #character_greater_than ;prompt for input
                        CALL    send_to_uart
                        RET
;
;**************************************************************************************
;LCD Character Module Routines
;**************************************************************************************
;
;LCD module is a 16 character by 2 line display but all displays are very similar
;The 4-wire data interface will be used (DB4 to DB7).
;
;The LCD modules are relatively slow and soloadware delay loops are used to slow down
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
lcd_pulse_e:            XOR     s4, #lcd_e              ;E=1
                        OUT     s4, lcd_output_port
                        CALL    delay_1us
                        XOR     s4, #lcd_e              ;E=0
                        OUT     s4, lcd_output_port
                        RET
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
lcd_write_inst4:        AND     s4, #0xf8               ;Enable=1 RS=0 Instruction, RW=0 Write, E=0
                        OUT     s4, lcd_output_port     ;set up RS and RW >40ns before enable pulse
                        CALL    lcd_pulse_e
                        RET
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
lcd_write_inst8:        LD      s4, s5
                        AND     s4, #0xf0               ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                        OR      s4, #lcd_drive          ;Enable=1
                        CALL    lcd_write_inst4         ;write upper nibble
                        CALL    delay_1us               ;wait >1us
                        LD      s4, s5                  ;select lower nibble with
                        SL1     s4                      ;Enable=1
                        SL0     s4                      ;RS=0 Instruction
                        SL0     s4                      ;RW=0 Write
                        SL0     s4                      ;E=0
                        CALL    lcd_write_inst4         ;write lower nibble
                        CALL    delay_40us              ;wait >40us
                        LD      s4, #0xf0               ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                        OUT     s4, lcd_output_port     ;Release master enable
                        RET
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
lcd_write_data:         LD      s4, s5
                        AND     s4, #0xf0               ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                        OR      s4, #0x0c               ;Enable=1 RS=1 Data, RW=0 Write, E=0
                        OUT     s4, lcd_output_port     ;set up RS and RW >40ns before enable pulse
                        CALL    lcd_pulse_e             ;write upper nibble
                        CALL    delay_1us               ;wait >1us
                        LD      s4, s5                  ;select lower nibble with
                        SL1     s4                      ;Enable=1
                        SL1     s4                      ;RS=1 Data
                        SL0     s4                      ;RW=0 Write
                        SL0     s4                      ;E=0
                        OUT     s4, lcd_output_port     ;set up RS and RW >40ns before enable pulse
                        CALL    lcd_pulse_e             ;write lower nibble
                        CALL    delay_40us              ;wait >40us
                        LD      s4, #0xf0               ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                        OUT     s4, lcd_output_port     ;Release master enable
                        RET
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
lcd_read_data8:         LD      s4, #0x0e               ;Enable=1 RS=1 Data, RW=1 Read, E=0
                        OUT     s4, lcd_output_port     ;set up RS and RW >40ns before enable pulse
                        XOR     s4, #lcd_e              ;E=1
                        OUT     s4, lcd_output_port
                        CALL    delay_1us               ;wait >260ns to access data
                        IN      s5, lcd_input_port      ;read upper nibble
                        XOR     s4, #lcd_e              ;E=0
                        OUT     s4, lcd_output_port
                        CALL    delay_1us               ;wait >1us
                        XOR     s4, #lcd_e              ;E=1
                        OUT     s4, lcd_output_port
                        CALL    delay_1us               ;wait >260ns to access data
                        IN      s0, lcd_input_port      ;read lower nibble
                        XOR     s4, #lcd_e              ;E=0
                        OUT     s4, lcd_output_port
                        AND     s5, #0xf0               ;merge upper and lower nibbles
                        SR0     s0
                        SR0     s0
                        SR0     s0
                        SR0     s0
                        OR      s5, s0
                        LD      s4, #0x04               ;Enable=0 RS=1 Data, RW=0 Write, E=0
                        OUT     s4, lcd_output_port     ;Stop reading 5V device and release master enable
                        CALL    delay_40us              ;wait >40us
                        RET
;
;
;Reset and initialise display to communicate using 4-bit data mode
;Includes routine to clear the display.
;
;Requires the 4-bit instructions 3,3,3,2 to be sent with suitable delays
;following by the 8-bit instructions to set up the display.
;
;  28 = '001' Function set, '0' 4-bit mode, '1' 2-line, '0' 5x7 dot matrix, 'xx'
;  06 = '000001' Entry mode, '1' increment, '0' no display shiload
;  0C = '00001' Display control, '1' display on, '0' cursor off, '0' cursor blink off
;  01 = '00000001' Display clear
;
;Registers used s0, s1, s2, s3, s4
;
lcd_reset:              CALL    delay_20ms              ;wait more that 15ms for display to be ready
                        LD      s4, #0x30
                        CALL    lcd_write_inst4         ;send '3'
                        CALL    delay_20ms              ;wait >4.1ms
                        CALL    lcd_write_inst4         ;send '3'
                        CALL    delay_1ms               ;wait >100us
                        CALL    lcd_write_inst4         ;send '3'
                        CALL    delay_40us              ;wait >40us
                        LD      s4, #0x20
                        CALL    lcd_write_inst4         ;send '2'
                        CALL    delay_40us              ;wait >40us
                        LD      s5, #0x28               ;Function set
                        CALL    lcd_write_inst8
                        LD      s5, #0x06               ;Entry mode
                        CALL    lcd_write_inst8
                        LD      s5, #0x0c               ;Display control
                        CALL    lcd_write_inst8
lcd_clear:              LD      s5, #0x01               ;Display clear
                        CALL    lcd_write_inst8
                        CALL    delay_1ms               ;wait >1.64ms for display to clear
                        CALL    delay_1ms
                        RET
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
lcd_cursor:             load    s5, #0x10               ;load for line 1
                        JUMP    z, set_line2
                        AND     s5, #0x0f               ;make address in range 80 to 8F for line 1
                        OR      s5, #0x80
                        CALL    lcd_write_inst8         ;instruction write to set cursor
                        RET
set_line2:              AND     s5, #0x0f               ;make address in range C0 to CF for line 2
                        OR      s5, #0xc0
                        CALL    lcd_write_inst8         ;instruction write to set cursor
                        RET
;
;**************************************************************************************
;Soloadware delay routines
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
delay_1us:              LD      s0, #delay_1us_constant
wait_1us:               SUB     s0, #0x01
                        JUMP    nz, wait_1us
                        RET
;
;Delay of 40us.
;
;Registers used s0, s1
;
delay_40us:             LD      s1, #0x28               ;40 x 1us = 40us
wait_40us:              CALL    delay_1us
                        SUB     s1, #0x01
                        JUMP    nz, wait_40us
                        RET
;
;
;Delay of 1ms.
;
;Registers used s0, s1, s2
;
delay_1ms:              LD      s2, #0x19               ;25 x 40us = 1ms
wait_1ms:               CALL    delay_40us
                        SUB     s2, #0x01
                        JUMP    nz, wait_1ms
                        RET
;
;Delay of 20ms.
;
;Delay of 20ms used during initialisation.
;
;Registers used s0, s1, s2, s3
;
delay_20ms:             LD      s3, #0x14               ;20 x 1ms = 20ms
wait_20ms:              CALL    delay_1ms
                        SUB     s3, #0x01
                        JUMP    nz, wait_20ms
                        RET
;
;Delay of approximately 1 second.
;
;Registers used s0, s1, s2, s3, s4
;
delay_1s:               LD      s4, #0x32               ;50 x 20ms = 1000ms
wait_1s:                CALL    delay_20ms
                        SUB     s4, #0x01
                        JUMP    nz, wait_1s
                        RET
;
;
;Delay of approximately N seconds where 'N' is provided in register s5.
;
;Registers used s0, s1, s2, s3, s4, s5
;
delay_ns:               CALL    delay_1s
                        SUB     s5, #0x01
                        JUMP    nz, delay_ns
                        RET
;
;
;
;**************************************************************************************
;LCD text messages
;**************************************************************************************
;
;
;
;Display 'PicoBlaze' on LCD at current cursor position
;
;
disp_picoblaze:         LD      s5, #_character_p
                        CALL    lcd_write_data
                        LD      s5, #character_i
                        CALL    lcd_write_data
                        LD      s5, #character_c
                        CALL    lcd_write_data
                        LD      s5, #character_o
                        CALL    lcd_write_data
                        LD      s5, #_character_b
                        CALL    lcd_write_data
                        LD      s5, #character_l
                        CALL    lcd_write_data
                        LD      s5, #character_a
                        CALL    lcd_write_data
                        LD      s5, #character_z
                        CALL    lcd_write_data
                        LD      s5, #character_e
                        CALL    lcd_write_data
                        RET
;
;
;Display 'Security' on LCD at current cursor position
;
;
disp_security:          LD      s5, #_character_s
                        CALL    lcd_write_data
                        LD      s5, #character_e
                        CALL    lcd_write_data
                        LD      s5, #character_c
                        CALL    lcd_write_data
                        LD      s5, #character_u
                        CALL    lcd_write_data
                        LD      s5, #character_r
                        CALL    lcd_write_data
                        LD      s5, #character_i
                        CALL    lcd_write_data
                        LD      s5, #character_t
                        CALL    lcd_write_data
                        LD      s5, #character_y
                        CALL    lcd_write_data
                        RET
;
;
;Display 'FLASH Serial No.' on LCD at current cursor position
;
;
disp_flash_serial_no:   LD      s5, #_character_f
                        CALL    lcd_write_data
                        LD      s5, #_character_l
                        CALL    lcd_write_data
                        LD      s5, #_character_a
                        CALL    lcd_write_data
                        LD      s5, #_character_s
                        CALL    lcd_write_data
                        LD      s5, #_character_h
                        CALL    lcd_write_data
                        LD      s5, #character_space
                        CALL    lcd_write_data
                        LD      s5, #_character_s
                        CALL    lcd_write_data
                        LD      s5, #character_e
                        CALL    lcd_write_data
                        LD      s5, #character_r
                        CALL    lcd_write_data
                        LD      s5, #character_i
                        CALL    lcd_write_data
                        LD      s5, #character_a
                        CALL    lcd_write_data
                        LD      s5, #character_l
                        CALL    lcd_write_data
                        LD      s5, #character_space
                        CALL    lcd_write_data
                        LD      s5, #_character_n
                        CALL    lcd_write_data
                        LD      s5, #character_o
                        CALL    lcd_write_data
                        LD      s5, #character_fullstop
                        CALL    lcd_write_data
                        RET
;
;
;
;Display 'Authentication' on top line of the LCD
;
;
disp_authentication:    LD      s5, #0x11               ;Line 1 position 1
                        CALL    lcd_cursor
                        LD      s5, #_character_a
                        CALL    lcd_write_data
                        LD      s5, #character_u
                        CALL    lcd_write_data
                        LD      s5, #character_t
                        CALL    lcd_write_data
                        LD      s5, #character_h
                        CALL    lcd_write_data
                        LD      s5, #character_e
                        CALL    lcd_write_data
                        LD      s5, #character_n
                        CALL    lcd_write_data
                        LD      s5, #character_t
                        CALL    lcd_write_data
                        LD      s5, #character_i
                        CALL    lcd_write_data
                        LD      s5, #character_c
                        CALL    lcd_write_data
                        LD      s5, #character_a
                        CALL    lcd_write_data
                        LD      s5, #character_t
                        CALL    lcd_write_data
                        LD      s5, #character_i
                        CALL    lcd_write_data
                        LD      s5, #character_o
                        CALL    lcd_write_data
                        LD      s5, #character_n
                        CALL    lcd_write_data
                        RET
;
;
;
;
;Display 'Passed' on lower line of the LCD
;
;
disp_passed:            LD      s5, #0x25               ;Line 2 position 5
                        CALL    lcd_cursor
                        LD      s5, #_character_p
                        CALL    lcd_write_data
                        LD      s5, #character_a
                        CALL    lcd_write_data
                        LD      s5, #character_s
                        CALL    lcd_write_data
                        CALL    lcd_write_data
                        LD      s5, #character_e
                        CALL    lcd_write_data
                        LD      s5, #character_d
                        CALL    lcd_write_data
                        RET
;
;
;
;
;
;Display 'Failed' on lower line of the LCD
;
;
disp_failed:            LD      s5, #0x25               ;Line 2 position 5
                        CALL    lcd_cursor
                        LD      s5, #_character_f
                        CALL    lcd_write_data
                        LD      s5, #character_a
                        CALL    lcd_write_data
                        LD      s5, #character_i
                        CALL    lcd_write_data
                        LD      s5, #character_l
                        CALL    lcd_write_data
                        LD      s5, #character_e
                        CALL    lcd_write_data
                        LD      s5, #character_d
                        CALL    lcd_write_data
                        RET
;
;
;**************************************************************************************
; Interrupt Service Routine (ISR)
;**************************************************************************************
;
; Interrupts occur when the application processor is requesting a design authorisation
; message. Therefore an interrupt results in a message being sent to the Link FIFO
; depending on the authentication status.
;
isr:                    ST      s0, isr_preserve_s0     ;save register contents
;
                        load      s0, authentication_status ;read authentication status
                        load     s0, #_character_p       ;load for pass 'P' or fail 'F'
                        JUMP    z, pass_token
;
                        LD      s0, #_character_f       ;send FAIL to link FIFO
                        OUT     s0, link_fifo_write_port
                        LD      s0, #_character_a
                        OUT     s0, link_fifo_write_port
                        LD      s0, #_character_i
                        OUT     s0, link_fifo_write_port
                        LD      s0, #_character_l
                        OUT     s0, link_fifo_write_port
                        JUMP    end_isr
;
pass_token:             OUT     s0, link_fifo_write_port ;send PASS to link FIFO
                        LD      s0, #_character_a
                        OUT     s0, link_fifo_write_port
                        LD      s0, #_character_s
                        OUT     s0, link_fifo_write_port
                        OUT     s0, link_fifo_write_port
;
end_isr:                load      s0, isr_preserve_s0     ;reload register contents
                        RETIE
;
;
;**************************************************************************************
; Interrupt Vector
;**************************************************************************************
;
                        ORG     0x3ff
                        JUMP    isr
;
;
;**************************************************************************************
; End of Program
;**************************************************************************************

