                         ; KCPSM3 Program - Security control and design authentication.
                          ;
                          ; This program is provided for use with the reference design
                          ; 'low_cost_design_authentication_for_spartan_3e.vhd' implemented on the Spartan-3E Starter
                          ; Kit. The program provides design authentication control over the 'real' application as well
                          ; as providing features to enable evaluation of the design authentication method.
                          ;
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
                          CONSTANT status_port, 00                 ;UART and memory status
                          CONSTANT tx_half_full, 01                ;  Transmitter     half full - bit0
                          CONSTANT tx_full, 02                     ;    FIFO            tx_full - bit1
                          CONSTANT rx_data_present, 04             ;  Receiver     data present - bit2
                          CONSTANT rx_half_full, 08                ;    FIFO          half full - bit3
                          CONSTANT rx_full, 10                     ;                    rx_full - bit4
                          CONSTANT spare1, 20                      ;                  spare '0' - bit5
                          CONSTANT spare2, 40                      ;                  spare '0' - bit6
                          CONSTANT strataflash_sts, 80             ;  StrataFLASH           STS - bit7
                          ;
                          CONSTANT UART_read_port, 01              ;UART Rx data input
                          ;
                          CONSTANT UART_write_port, 08             ;UART Tx data output
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
                          CONSTANT LCD_output_port, 20             ;LCD character module output data and control
                          CONSTANT LCD_E, 01                       ;   active High Enable        E - bit0
                          CONSTANT LCD_RW, 02                      ;   Read=1 Write=0           RW - bit1
                          CONSTANT LCD_RS, 04                      ;   Instruction=0 Data=1     RS - bit2
                          CONSTANT LCD_drive, 08                   ;   Master enable (active High) - bit3
                          CONSTANT LCD_DB4, 10                     ;   4-bit              Data DB4 - bit4
                          CONSTANT LCD_DB5, 20                     ;   interface          Data DB5 - bit5
                          CONSTANT LCD_DB6, 40                     ;                      Data DB6 - bit6
                          CONSTANT LCD_DB7, 80                     ;                      Data DB7 - bit7
                          ;
                          ;
                          CONSTANT LCD_input_port, 03              ;LCD character module input data
                          CONSTANT LCD_read_spare0, 01             ;    Spare bits               - bit0
                          CONSTANT LCD_read_spare1, 02             ;    are zero                 - bit1
                          CONSTANT LCD_read_spare2, 04             ;                             - bit2
                          CONSTANT LCD_read_spare3, 08             ;                             - bit3
                          CONSTANT LCD_read_DB4, 10                ;    4-bit           Data DB4 - bit4
                          CONSTANT LCD_read_DB5, 20                ;    interface       Data DB5 - bit5
                          CONSTANT LCD_read_DB6, 40                ;                    Data DB6 - bit6
                          CONSTANT LCD_read_DB7, 80                ;                    Data DB7 - bit7
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
                          CONSTANT SF_data_in_port, 02             ;Read data from StrataFLASH device
                          ;
                          CONSTANT SF_data_out_port, 80            ;Data to write into StrataFLASH device
                          ;
                          CONSTANT SF_addr_hi_port, 83             ;StrataFLASH address[21:16] (6 LSB's)
                          CONSTANT SF_addr_mi_port, 82             ;StrataFLASH address[15:8]
                          CONSTANT SF_addr_lo_port, 81             ;StrataFLASH address[7:0]
                          ;
                          CONSTANT SF_control_port, 40             ;StrataFLASH control
                          CONSTANT SF_read, 01                     ;         active High read - bit0
                          CONSTANT SF_ce, 02                       ; active Low device enable - bit1
                          CONSTANT SF_we, 04                       ;         active Low write - bit2
                          ;
                          ;
                          ; Design Authentication enable/disable signals.
                          ;
                          ; Hardware controls over the 'real' application.
                          ;
                          CONSTANT authentication_control_port, 10 ;Design disable control port
                          CONSTANT security_disable_interrupts, 01 ;   active High disable of interrupt generation - bit0
                          CONSTANT security_disable_outputs, 02    ;            active High disable of output pins - bit1
                          ;
                          ; Pseudo Random number generator
                          ;
                          CONSTANT random_value_port, 04           ;read LFSR counter value
                          ;
                          ;
                          ; Link FIFO buffer
                          ;
                          ; Provides a connection to the 'real' application such that 'soft tokens' in the
                          ; form of short messages to be passed to the 'real' application to enable or disable
                          ; it depending on the authentication status.
                          ;
                          CONSTANT link_FIFO_write_port, 04        ;write data to FIFO
                          ;
                          ;
                          ;**************************************************************************************
                          ; Special Register usage
                          ;**************************************************************************************
                          ;
                          NAMEREG sF, UART_data                    ;used to pass data to and from the UART
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ;Scratch Pad Memory Locations
                          ;**************************************************************************************
                          ;
                          CONSTANT ISR_preserve_s0, 00             ;preserve register contents during Interrupt Service Routine
                          ;
                          ;
                          CONSTANT serial_number0, 10              ;64-bit serial number of StrataFlash
                          CONSTANT serial_number1, 11              ;LS-Byte first
                          CONSTANT serial_number2, 12
                          CONSTANT serial_number3, 13
                          CONSTANT serial_number4, 14
                          CONSTANT serial_number5, 15
                          CONSTANT serial_number6, 16
                          CONSTANT serial_number7, 17
                          ;
                          ;
                          CONSTANT computed_CRC0, 18               ;computed 16-bit CRC based on the
                          CONSTANT computed_CRC1, 19               ;  StrataFlash unique serial number (LS-Byte first)
                          ;
                          ;
                          CONSTANT authentication_CRC0, 1A         ;16-bit CRC value read from authentication
                          CONSTANT authentication_CRC1, 1B         ;  area of StrataFLASH memory (LS-Byte first)
                          ;
                          ;
                          CONSTANT authentication_status, 1C       ;Status of design authentication
                          ;
                          ;
                          ;**************************************************************************************
                          ;Useful data constants
                          ;**************************************************************************************
                          ;
                          ;
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
                          CONSTANT delay_1us_constant, 0B
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
                          CONSTANT character_fullstop, 2E
                          CONSTANT character_semi_colon, 3B
                          CONSTANT character_minus, 2D
                          CONSTANT character_divide, 2F            ;'/'
                          CONSTANT character_plus, 2B
                          CONSTANT character_comma, 2C
                          CONSTANT character_less_than, 3C
                          CONSTANT character_greater_than, 3E
                          CONSTANT character_equals, 3D
                          CONSTANT character_space, 20
                          CONSTANT character_CR, 0D                ;carriage return
                          CONSTANT character_question, 3F          ;'?'
                          CONSTANT character_dollar, 24
                          CONSTANT character_exclaim, 21           ;'!'
                          CONSTANT character_BS, 08                ;Back Space command character
                          ;
                          ;
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ;Initialise the system
                          ;**************************************************************************************
                          ;
              cold_start: CALL SF_init                             ;initialise StrataFLASH controls
                          LOAD s0, 00                              ;Start with application enabled in hardware
                          OUTPUT s0, authentication_control_port
                          LOAD s0, character_P                     ;start with design enabled by software (see ISR)
                          STORE s0, authentication_status
                          CALL delay_1s                            ;delay to allow system to settle
                          CALL LCD_reset                           ;Initialise the LCD
                          ;
                          ENABLE INTERRUPT                         ;interrupts to provide software enable to application
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
              warm_start: LOAD s5, 12                              ;Line 1 position 2
                          CALL LCD_cursor
                          CALL disp_PicoBlaze                      ;Display 'PicoBlaze'
                          LOAD s5, 25                              ;Line 2 position 5
                          CALL LCD_cursor
                          CALL disp_Security                       ;Display 'Security'
                          ;
                          ; Write welcome message to PC via UART
                          ;
                          CALL send_welcome
                          ;
                          ;
                          ; Display 'Copyright Ken Chapman 2006' via the UART.
                          ;
                          ; This message is significant because it demonstrates that the design now has a 'watermark'.
                          ; The ASCII codes for this string are part of the PicoBlaze program stored in a Block
                          ; Memory and therefore are also part of the configuration bit stream. If someone tries to
                          ; change or delete this copyright message the hardware design will detect the change to the
                          ; Block memory contents and also inhibit the design.
                          ;
                          CALL send_Copyright
                          ;
                          ;
                          ;
                          ; Delay of 10 seconds before performing any security checks.
                          ;
                          ; This allows the design to work for a short time which could be important for
                          ; production testing.
                          ;
                          ; Having a significant time delay (days or weeks) before security checks means that someone
                          ; attempting to clone the product may not be aware that there is any form of design security
                          ; at all until products are in the field are failing. A time delay also impedes the ability to
                          ; attempt to break the security and confirm if an attempt is or is not successful.
                          ;
                          LOAD s5, 0A                              ;delay of 10 seconds.
                          CALL delay_Ns
                          ;
                          ;
                          ;
                          ; Read serial number of the StrataFLASH memory.
                          ; The whole principle of low cost design security is based on this unique number. Clearly this
                          ; number is not a secret, but what we then go on to do with it should normally be kept secret.
                          ;
                          CALL read_SF_serial_number               ;read serial number from FLASH memory
                          CALL send_serial_number                  ;send value to UART for display on PC
                          CALL disp_serial_number                  ;display serial number on LCD display.
                          CALL send_CR
                          ;
                          ;
                          ;
                          LOAD s5, 0A                              ;delay of 10 seconds to read messages.
                          CALL delay_Ns
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
                          LOAD sE, 15                              ;seed CRC register with an initial value provided by my daughter when asked :-)
                          LOAD sD, 8E
                          CALL compute_seeded_CRC                  ;compute CRC for serial number and configuration memory
                          ;
                          ; Store CRC value in scratch pad memory and display computed CRC value on the PC via UART.
                          ;
                          STORE sD, computed_CRC0                  ;store CRC value
                          STORE sE, computed_CRC1
                          CALL send_Computed_CRC                   ;display computed CRC value on PC via UART
                          LOAD s0, sE
                          CALL send_hex_byte
                          LOAD s0, sD
                          CALL send_hex_byte
                          CALL send_CR
                          ;
                          ;
                          ;
                          ; Read the authenticated CRC value stored in StrataFLASH memory.
                          ; 16-bit value is hidden in 256 bytes of random numbers to make it more difficult
                          ; for an attacker to identify.
                          ; Read value is stored in scratch pad memory and displayed on the PC via UART.
                          ;
                          CALL read_authentication                 ;read StrataFLASH memory into [sB,sA]
                          STORE sA, authentication_CRC0            ;store CRC value
                          STORE sB, authentication_CRC1
                          CALL send_FLASH_CRC                      ;display CRC value from FLASH on PC via UART
                          LOAD s0, sB
                          CALL send_hex_byte
                          LOAD s0, sA
                          CALL send_hex_byte
                          CALL send_CR
                          ;
                          ;
                          ; Compare the computed CRC value with the authentication value stored in StrataFLASH
                          ; and determine if the design is authenticated. Then decide course of action.
                          ;
                          CALL LCD_clear                           ;clear LCD display
                          CALL disp_Authentication                 ;prepare LCD display for result of authentication
                          CALL send_Authentication                 ;prepare PC display for result of authentication
                          ;
                          COMPARE sA, sD                           ;Perform comparison of CRC values
                          JUMP NZ, auth_failure
                          COMPARE sB, sE
                          JUMP NZ, auth_failure
                          ;
                          ;
                          ; Authentication Successful Process
                          ;
                          ; In this mode the design continues to operate and for evaluation
                          ; purposes this design transfers control to the simple menu immediately.
                          ;
             auth_passed: CALL disp_Passed                         ;display successful authentication on LCD display
                          CALL send_PASSED                         ;display successful authentication on PC via UART
                          JUMP Menu
                          ;
                          ; Authentication Failure Process
                          ;
                          ; When the authentication fails two hardware based disable methods are demonstrated. Then
                          ; the failed status is remembered for future software token messages to demonstrate software
                          ; based disabling of the 'real' application. Finally the simple menu of options is presented
                          ; to allow evaluation to continue.
                          ;
                          ;
            auth_failure: CALL disp_Failed                         ;display failure to authenticate on LCD display
                          CALL send_FAILED                         ;display failure to authenticate on PC via UART
                          CALL send_CR
                          CALL disable_app_hardware                ;sequence hardware disable signals
                          LOAD s0, character_F                     ;change authentication status to 'F' for failed.
                          STORE s0, authentication_status          ; so that application software disable is demonstrated
                          ;
                          ;
                          ;
                          ; Menu of options for authentication processing
                          ;
                    Menu: CALL send_Menu                           ;display menu and prompt
                          CALL read_from_UART                      ;read character from PC
                          CALL upper_case                          ;convert to upper case
                          COMPARE UART_data, character_R
                          JUMP Z, read_command
                          COMPARE UART_data, character_E
                          JUMP Z, erase_command
                          COMPARE UART_data, character_A
                          JUMP Z, authorise_command
                          JUMP Menu                                ;repeat menu for invalid selection
                          ;
                          ;
                          ;
            read_command: CALL send_CR
                          CALL send_auth_page
                          CALL send_CR
                          CALL send_CR
                          JUMP Menu
                          ;
                          ;
                          ;
           erase_command: CALL send_Erase_in_progress
                          CALL erase_authentication
                          CALL send_OK
                          JUMP Menu
                          ;
                          ;
                          ;
       authorise_command: CALL send_Writing                        ;Send 'Writing Authorisation' message
                          CALL send_CR
                          FETCH sD, computed_CRC0                  ;fetch computed CRC value
                          FETCH sE, computed_CRC1
                          CALL write_authentication                ;write computed CRC to FLASH with random data
                          CALL send_OK
                          JUMP Menu
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
    disable_app_hardware: LOAD s0, security_disable_interrupts
                          OUTPUT s0, authentication_control_port
                          LOAD s5, 05
                          CALL delay_Ns
                          ;
                          ; Enable application for 5 seconds
                          ;
                          LOAD s0, 00
                          OUTPUT s0, authentication_control_port
                          LOAD s5, 05
                          CALL delay_Ns
                          ;
                          ; Disable and/or scramble outputs connected to application PicoBlaze for 5 seconds
                          ;
                          LOAD s0, security_disable_outputs
                          OUTPUT s0, authentication_control_port
                          LOAD s5, 05
                          CALL delay_Ns
                          ;
                          ;
                          ; Enable application in hardware so that software disable function can then be
                          ; demonstrated until the design is reconfigured and authentication test repeated.
                          ;
                          LOAD s0, 00
                          OUTPUT s0, authentication_control_port
                          RETURN
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; Send the 64-bit serial number stored in scratch pad memory to the UART
                          ;**************************************************************************************
                          ;
                          ; The serial number should previously have been copied into the 8 ascending scratch pad
                          ; memory locations called 'serial_number0' through to 'serial_number7'.
                          ;
                          ; The serial number is displayed MS-Byte first.
                          ;
                          ; Registers used s0,s1,s2,s3
                          ;
      send_serial_number: CALL send_FLASH_Serial_Number            ;display text message
                          LOAD s3, serial_number7                  ;pointer to scratch pad memory
            send_SN_loop: FETCH s0, (s3)                           ;read serial number byte
                          CALL send_hex_byte                       ;display byte
                          CALL send_space                          ;display byte
                          COMPARE s3, serial_number0               ;check for 8 bytes sent to UART
                          JUMP Z, end_send_SN
                          SUB s3, 01                               ;increment memory pointer
                          JUMP send_SN_loop
                          ;
             end_send_SN: CALL send_CR
                          RETURN
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; Display the 64-bit serial number stored in scratch pad memory on the LCD display
                          ;**************************************************************************************
                          ;
                          ; The serial number should previously have been copied into the 8 ascending scratch pad
                          ; memory locations called 'serial_number0' through to 'serial_number7'.
                          ;
                          ; The serial number is displayed MS-Byte first.
                          ;
                          ; Registers used s0,s1,s2,s3,s4,s5,s6
                          ;
      disp_serial_number: CALL LCD_clear                           ;clear LCD display
                          LOAD s5, 10                              ;Line 1 position 0
                          CALL LCD_cursor
                          CALL disp_FLASH_Serial_No                ;display text message
                          LOAD s5, 20                              ;Line 2 position 0
                          CALL LCD_cursor
                          LOAD s6, serial_number7                  ;pointer to scratch pad memory
            disp_SN_loop: FETCH s0, (s6)                           ;read serial number byte
                          CALL disp_hex_byte                       ;display byte
                          COMPARE s6, serial_number0               ;check for 8 bytes sent to UART
                          JUMP Z, end_disp_SN
                          SUB s6, 01                               ;increment memory pointer
                          JUMP disp_SN_loop
                          ;
             end_disp_SN: CALL send_CR
                          RETURN
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
                          ; stored at known locations in the FLASH memory.
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
      compute_seeded_CRC: LOAD s4, serial_number0                  ;pointer to scratch pad memory holding serial number
           CRC_send_loop: FETCH s3, (s4)                           ;read serial number byte
                          CALL compute_CRC16                       ;compute CRC for value in 's3'
                          COMPARE s4, serial_number7               ;check for 8 bytes processed
                          RETURN Z
                          ADD s4, 01                               ;increment memory pointer
                          JUMP CRC_send_loop
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
           compute_CRC16: LOAD s1, 08                              ;8-bits to shift
              crc16_loop: LOAD s0, sD                              ;copy current CRC value
                          XOR s0, s3                               ;Need to know LSB XOR next input bit
                          TEST s0, 01                              ;test result of XOR in LSB
                          JUMP NC, crc16_shift
                          XOR sD, 02                               ;compliment bit 1 of CRC
                          XOR sE, 40                               ;compliment bit 14 of CRC
             crc16_shift: SR0 s0                                   ;Carry gets LSB XOR next input bit
                          SRA sE                                   ;shift Carry into MSB to form new CRC value
                          SRA sD
                          RR s3                                    ;shift input value
                          SUB s1, 01                               ;count bits
                          JUMP NZ, crc16_loop                      ;next bit
                          RETURN
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
                          ; This routine deliberately reads all 256 bytes that are stored and abstracts the required
                          ; 2 bytes of information from them otherwise it would be easy to observe which addresses
                          ; of the block were being accessed.
                          ;
                          ; Another way that an attacker may deduce which address locations are important would be to
                          ; observe the time between read accesses and note when there is any difference. In this case
                          ; the attacker is attempting to detect when PicoBlaze takes slightly longer to execute the
                          ; instructions which store the important bytes in scratch pad memory. So to avoid this
                          ; detection this routine inserts an additional random delay between reads to mask any code
                          ; execution differences.
                          ;
                          ; The 256 bytes are stored at addresses 060000 to 0600FF hex (the first block above the
                          ; XC3S500E configuration image which occupies 000000 to 04547F hex). The 2 bytes forming the
                          ; actual authentication value are stored as 4-bit nibbles in 4 different locations in this range.
                          ;
                          ;
                          ;                             High Order Nibble           Low Order Nibble
                          ;                               (NNNNxxxx)                  (xxxxNNNN)
                          ;
                          ; LS-Byte in 'sA'              Addr=060010                 Addr=06007F
                          ; MS-Byte in 'sB'              Addr=060025                 Addr=0600FA
                          ;
                          ;
     read_authentication: LOAD s9, 06                              ;start address in FLASH
                          LOAD s8, 00
                          LOAD s7, 00
          auth_read_loop: CALL SF_byte_read                        ;read byte from FLASH into s0
                          COMPARE s7, 10                           ;check for bytes/nibbles that contain real information
                          JUMP NZ, auth_check2
                          LOAD sA, s0                              ;isolate upper order nibble for LS-Byte
                          AND sA, F0
             auth_check2: COMPARE s7, 25
                          JUMP NZ, auth_check3
                          LOAD sB, s0                              ;isolate upper order nibble for MS-Byte
                          AND sB, F0
             auth_check3: COMPARE s7, 7F
                          JUMP NZ, auth_check4
                          AND s0, 0F                               ;isolate lower order nibble for LS-Byte
                          OR sA, s0                                ;  and merge with upper order nibble
             auth_check4: COMPARE s7, FA
                          JUMP NZ, next_auth_read
                          AND s0, 0F                               ;isolate lower order nibble for MS-Byte
                          OR sB, s0                                ;  and merge with upper order nibble
          next_auth_read: ADD s7, 01                               ;increment address
                          RETURN Z                                 ;complete after 256 reads
                          INPUT s0, random_value_port              ;random delay between reads
         auth_read_delay: SUB s0, 01
                          JUMP NZ, auth_read_delay
                          JUMP auth_read_loop
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
          send_auth_page: LOAD s9, 06                              ;start address in FLASH
                          LOAD s8, 00
                          LOAD s7, 00
          auth_line_loop: CALL send_CR
                          CALL send_hex_3bytes                     ;display address
                          CALL send_space
          auth_byte_loop: CALL send_space
                          CALL SF_byte_read                        ;read byte into s0
                          CALL send_hex_byte                       ;display byte
                          ADD s7, 01                               ;increment FLASH address
                          TEST s7, 0F                              ;test for 16 byte boundary
                          JUMP NZ, auth_byte_loop
                          TEST s7, FF                              ;test for roll over of 256 bytes
                          JUMP NZ, auth_line_loop
                          CALL send_CR
                          RETURN
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
                          ; The 2 bytes forming the actual authentication value are stored as 4-bit nibbles in
                          ; 4 different locations in the address range 600000 to 6000FF hex (256 bytes) with
                          ; all remaining locations filled with pseudo random values.
                          ;
                          ; The authentication value to be stored in StrataFLASH memory should be provided in
                          ; the register pair [sE,sD] and will be stored in the following locations.
                          ;
                          ;                             High Order Nibble           Low Order Nibble
                          ;                               (NNNNxxxx)                  (xxxxNNNN)
                          ;
                          ; LS-Byte in 'sD'              Addr=060010                 Addr=06007F
                          ; MS-Byte in 'sE'              Addr=060025                 Addr=0600FA
                          ;
                          ;
    write_authentication: LOAD s9, 06                              ;start address in FLASH
                          LOAD s8, 00
                          LOAD s7, 00
         auth_write_loop: INPUT s0, random_value_port              ;Obtain random value
                          COMPARE s7, 10                           ;check for bytes/nibbles that need to be real information
                          JUMP NZ, auth_write_check2
                          LOAD s1, sD                              ;merge upper order nibble for LS-Byte with random
                          AND s1, F0
                          AND s0, 0F
                          OR s0, s1
       auth_write_check2: COMPARE s7, 25
                          JUMP NZ, auth_write_check3
                          LOAD s1, sE                              ;merge upper order nibble for MS-Byte with random
                          AND s1, F0
                          AND s0, 0F
                          OR s0, s1
       auth_write_check3: COMPARE s7, 7F
                          JUMP NZ, auth_write_check4
                          LOAD s1, sD                              ;merge lower order nibble for LS-Byte with random
                          AND s1, 0F
                          AND s0, F0
                          OR s0, s1
       auth_write_check4: COMPARE s7, FA
                          JUMP NZ, write_auth
                          LOAD s1, sE                              ;merge lower order nibble for MS-Byte with random
                          AND s1, 0F
                          AND s0, F0
                          OR s0, s1
              write_auth: CALL SF_single_byte_write                ;write byte to FLASH
                          ADD s7, 01                               ;increment address
                          RETURN Z                                 ;complete after 256 writes
                          JUMP auth_write_loop
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
    erase_authentication: LOAD s9, 06                              ;set address to 06xxxx hex which defines block 3 (060000 to 07FFFF)
                          CALL SF_erase_block
                          RETURN
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
                 SF_init: LOAD s0, 06
                          OUTPUT s0, SF_control_port
                          RETURN
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
          SF_erase_block: LOAD s8, 00                              ;define lower address of block = xx0000
                          LOAD s7, 00
                          LOAD s1, 20                              ;block erase command
                          CALL SF_byte_write
                          LOAD s1, D0                              ;write confirm command
                          CALL SF_byte_write
                          CALL wait_SF_ready                       ;wait for erase to complete
                          RETURN
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
    SF_single_byte_write: LOAD s1, 40                              ;command for single byte program
                          CALL SF_byte_write
                          LOAD s1, s0                              ;write data to be programmed
                          CALL SF_byte_write
                          CALL wait_SF_ready                       ;wait for program to complete
                          RETURN
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
                          ; After reading the device information the read array command is written to the
                          ; device to put it back to normal read mode.
                          ;
                          ; Registers used s0,s1,s2,s7,s8,s9
                          ;
   read_SF_serial_number: LOAD s9, 00                              ;StrataFLASH address to read serial number = 000102
                          LOAD s8, 01
                          LOAD s7, 02
                          LOAD s2, serial_number0                  ;pointer to scratch pad memory
                          LOAD s1, 90                              ;command to read device information
                          CALL SF_byte_write
            read_SN_loop: CALL SF_byte_read                        ;read serial number value
                          STORE s0, (s2)
                          COMPARE s2, serial_number7               ;check for 8 bytes copied
                          JUMP Z, end_read_SN
                          ADD s7, 01                               ;increment StrataFLASH address
                          ADD s2, 01                               ;increment memory pointer
                          JUMP read_SN_loop
                          ;
             end_read_SN: CALL set_SF_read_array_mode              ;restore normal read array mode
                          RETURN
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
            SF_byte_read: OUTPUT s9, SF_addr_hi_port               ;set 24-bit address
                          OUTPUT s8, SF_addr_mi_port
                          OUTPUT s7, SF_addr_lo_port
                          LOAD s1, 05                              ;set controls
                          OUTPUT s1, SF_control_port
                          LOAD s1, 06                              ;>75ns delay
                          LOAD s1, 06                              ;but do something useful!
                          INPUT s0, SF_data_in_port                ;read data byte
                          OUTPUT s1, SF_control_port               ;clear controls
                          RETURN
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
           SF_byte_write: OUTPUT s9, SF_addr_hi_port               ;set 24-bit address
                          OUTPUT s8, SF_addr_mi_port
                          OUTPUT s7, SF_addr_lo_port
                          OUTPUT s1, SF_data_out_port              ;set data byte to be written
                          LOAD s1, 00                              ;set controls
                          OUTPUT s1, SF_control_port
                          LOAD s1, 06                              ;>60ns delay
                          LOAD s1, 06                              ;but do something useful!
                          OUTPUT s1, SF_control_port               ;clear controls
                          RETURN
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
                          ; into this mode after programming, erasing or reading the status register.
                          ;
                          ; The read array command (FF hex) is written to the Strata flash memory.
                          ;
                          ; Registers used s1,s7,s8,s9
                          ;
  set_SF_read_array_mode: LOAD s1, FF                              ;command to read array
                          CALL SF_byte_write
                          RETURN
                          ;
                          ;
                          ;**************************************************************************************
                          ; Wait for StrataFLASH to be ready
                          ;**************************************************************************************
                          ;
                          ; This routine will typically be used after instigating a program or erase
                          ; command. It continuously reads the StrataFLASH status register and tests the
                          ; information provided by bit7 which indicates if the memory is busy(0) or ready(1).
                          ; The routine waits for the ready condition before sending a read array command
                          ; which puts the memory back to normal read mode.
                          ;
                          ;
                          ; Registers used s0,s1,s7,s8,s9   (s7,s8,s9 not changed)
                          ;
                          ;
           wait_SF_ready: CALL SF_byte_read                        ;read status register into s0
                          TEST s0, 80                              ;test ready/busy flag
                          JUMP Z, wait_SF_ready
                          CALL set_SF_read_array_mode              ;restore normal read array mode
                          RETURN
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
                          ; The routine first tests the receiver FIFO buffer to see if data is present.
                          ; If the FIFO is empty, the routine waits until there is a character to read.
                          ; As this could take any amount of time the wait loop could include a call to a
                          ; subroutine which performs a useful function.
                          ;
                          ;
                          ; Registers used s0 and UART_data
                          ;
          read_from_UART: INPUT s0, status_port                    ;test Rx_FIFO buffer
                          TEST s0, rx_data_present                 ;wait if empty
                          JUMP NZ, read_character
                          JUMP read_from_UART
          read_character: INPUT UART_data, UART_read_port          ;read from FIFO
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
            send_to_UART: INPUT s0, status_port                    ;test Tx_FIFO buffer
                          TEST s0, tx_full                         ;wait if full
                          JUMP Z, UART_write
                          JUMP send_to_UART
              UART_write: OUTPUT UART_data, UART_write_port
                          RETURN
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
              upper_case: COMPARE UART_data, 61                    ;eliminate character codes below 'a' (61 hex)
                          RETURN C
                          COMPARE UART_data, 7B                    ;eliminate character codes above 'z' (7A hex)
                          RETURN NC
                          AND UART_data, DF                        ;mask bit5 to convert to upper case
                          RETURN
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
       hex_byte_to_ASCII: LOAD s1, s0                              ;remember value supplied
                          SR0 s0                                   ;isolate upper nibble
                          SR0 s0
                          SR0 s0
                          SR0 s0
                          CALL hex_to_ASCII                        ;convert
                          LOAD s2, s0                              ;upper nibble value in s2
                          LOAD s0, s1                              ;restore complete value
                          AND s0, 0F                               ;isolate lower nibble
                          CALL hex_to_ASCII                        ;convert
                          LOAD s1, s0                              ;lower nibble value in s1
                          RETURN
                          ;
                          ;Convert hexadecimal value provided in register s0 into ASCII character
                          ;
                          ;Register used s0
                          ;
            hex_to_ASCII: SUB s0, 0A                               ;test if value is in range 0 to 9
                          JUMP C, number_char
                          ADD s0, 07                               ;ASCII char A to F in range 41 to 46
             number_char: ADD s0, 3A                               ;ASCII char 0 to 9 in range 30 to 40
                          RETURN
                          ;
                          ;
                          ;Send the two character HEX value of the register contents 's0' to the UART
                          ;
                          ;Registers used s0, s1, s2
                          ;
           send_hex_byte: CALL hex_byte_to_ASCII
                          LOAD UART_data, s2
                          CALL send_to_UART
                          LOAD UART_data, s1
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ;Send the six character HEX value of the register contents [s9,s8,s7] to the UART
                          ;
                          ;Registers used s0, s1, s2
                          ;
         send_hex_3bytes: LOAD s0, s9
                          CALL send_hex_byte
                          LOAD s0, s8
                          CALL send_hex_byte
                          LOAD s0, s7
                          CALL send_hex_byte
                          RETURN
                          ;
                          ;
                          ;Display the two character HEX value of the register contents 's0' on the LCD display
                          ;
                          ;Registers used s0,s1,s2,s3,s4,s5
                          ;
           disp_hex_byte: CALL hex_byte_to_ASCII
                          LOAD s3, s1                              ;remember least significant digit
                          LOAD s5, s2
                          CALL LCD_write_data                      ;display most significant digit
                          LOAD s5, s3
                          CALL LCD_write_data                      ;display least significant digit
                          RETURN
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
                 send_CR: LOAD UART_data, character_CR
                          CALL send_to_UART
                          RETURN
                          ;
                          ;Send a space to the UART
                          ;
              send_space: LOAD UART_data, character_space
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ;Send an equals sign to the UART with a space each side
                          ;
             send_equals: CALL send_space
                          LOAD UART_data, character_equals
                          CALL send_to_UART
                          CALL send_space
                          RETURN
                          ;
                          ;
                          ;
                          ;Send an minus sign (dash) to the UART with a space each side
                          ;
               send_dash: CALL send_space
                          LOAD UART_data, character_minus
                          CALL send_to_UART
                          CALL send_space
                          RETURN
                          ;
                          ;
                          ;Send 'PicoBlaze Low Cost Design Security v1.00' string to the UART
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
                          LOAD UART_data, character_L
                          CALL send_to_UART
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_w
                          CALL send_to_UART
                          CALL send_space
                          LOAD UART_data, character_C
                          CALL send_to_UART
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_s
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          CALL send_space
                          LOAD UART_data, character_D
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_s
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_g
                          CALL send_to_UART
                          LOAD UART_data, character_n
                          CALL send_to_UART
                          CALL send_space
                          LOAD UART_data, character_S
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_c
                          CALL send_to_UART
                          LOAD UART_data, character_u
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_y
                          CALL send_to_UART
                          CALL send_space
                          LOAD UART_data, character_v
                          CALL send_to_UART
                          LOAD UART_data, character_1
                          CALL send_to_UART
                          LOAD UART_data, character_fullstop
                          CALL send_to_UART
                          LOAD UART_data, character_0
                          CALL send_to_UART
                          LOAD UART_data, character_0
                          CALL send_to_UART
                          CALL send_CR
                          CALL send_CR
                          RETURN
                          ;
                          ;
                          ;
                          ;Send 'Copyright Ken Chapman 2006' string to the UART
                          ;
                          ;This message is significant because it demonstrates that the design
                          ;now has a 'watermark'. The ASCII codes for this string will be
                          ;stored in the design configuration bit stream somewhere as well as
                          ;being played out by the UART. If someone tries to change or delete
                          ;this message the contents of the BRAM will change and the hardware
                          ;check of the BRAM contents will fail to match the expected value and
                          ;the design will again be disabled.
                          ;
          send_Copyright: LOAD UART_data, character_C
                          CALL send_to_UART
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_p
                          CALL send_to_UART
                          LOAD UART_data, character_y
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_g
                          CALL send_to_UART
                          LOAD UART_data, character_h
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          CALL send_space
                          LOAD UART_data, character_K
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_n
                          CALL send_to_UART
                          CALL send_space
                          LOAD UART_data, character_C
                          CALL send_to_UART
                          LOAD UART_data, character_h
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_p
                          CALL send_to_UART
                          LOAD UART_data, character_m
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_n
                          CALL send_to_UART
                          CALL send_space
                          LOAD UART_data, character_2
                          CALL send_to_UART
                          LOAD UART_data, character_0
                          CALL send_to_UART
                          CALL send_to_UART
                          LOAD UART_data, character_6
                          CALL send_to_UART
                          CALL send_CR
                          CALL send_CR
                          RETURN
                          ;
                          ;
                          ;
                          ;Send 'FLASH ' string to the UART
                          ;
              send_FLASH: LOAD UART_data, character_F
                          CALL send_to_UART
                          LOAD UART_data, character_L
                          CALL send_to_UART
                          LOAD UART_data, character_A
                          CALL send_to_UART
                          LOAD UART_data, character_S
                          CALL send_to_UART
                          LOAD UART_data, character_H
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ;
                          ;Send 'FLASH Serial Number = ' string to the UART
                          ;
send_FLASH_Serial_Number: CALL send_FLASH
                          CALL send_space
                          LOAD UART_data, character_S
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_l
                          CALL send_to_UART
                          CALL send_space
                          LOAD UART_data, character_N
                          CALL send_to_UART
                          LOAD UART_data, character_u
                          CALL send_to_UART
                          LOAD UART_data, character_m
                          CALL send_to_UART
                          LOAD UART_data, character_b
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          CALL send_equals
                          RETURN
                          ;
                          ;
                          ;Send 'Auth' string to the UART
                          ;
               send_Auth: LOAD UART_data, character_A
                          CALL send_to_UART
                          LOAD UART_data, character_u
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_h
                          CALL send_to_UART
                          RETURN
                          ;
                          ;Send 'Authoris' to the UART
                          ;
           send_Authoris: CALL send_Auth
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_s
                          CALL send_to_UART
                          RETURN
                          ;
                          ;Send 'Authorisation' to the UART
                          ;
      send_Authorisation: CALL send_Authoris
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_n
                          CALL send_to_UART
                          RETURN
                          ;
                          ;Send 'Authorise' to the UART
                          ;
          send_Authorise: CALL send_Authoris
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          RETURN
                          ;
                          ;Send 'Authentication' string to the UART
                          ;
     send_Authentication: CALL send_Auth
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_n
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_c
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_n
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ;Send 'FLASH CRC = ' string to the UART
                          ;
          send_FLASH_CRC: CALL send_FLASH
                          ;
                          ;
                          ;Send ' CRC = ' string to the UART
                          ;
                send_CRC: CALL send_space
                          LOAD UART_data, character_C
                          CALL send_to_UART
                          LOAD UART_data, character_R
                          CALL send_to_UART
                          LOAD UART_data, character_C
                          CALL send_to_UART
                          CALL send_equals
                          RETURN
                          ;
                          ;
                          ;
                          ;Send 'Computed CRC = ' string to the UART
                          ;
       send_Computed_CRC: LOAD UART_data, character_C
                          CALL send_to_UART
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_m
                          CALL send_to_UART
                          LOAD UART_data, character_p
                          CALL send_to_UART
                          LOAD UART_data, character_u
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_d
                          CALL send_to_UART
                          JUMP send_CRC
                          ;
                          ;
                          ;Send 'Erase ' string to the UART
                          ;
              send_Erase: LOAD UART_data, character_E
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_s
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          CALL send_space
                          RETURN
                          ;
                          ;
                          ;Send 'Erase Authorisation in progress' string to the UART
                          ;
  send_Erase_in_progress: CALL send_CR
                          CALL send_Erase
                          CALL send_Authorisation
                          CALL send_space
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_n
                          CALL send_to_UART
                          CALL send_space
                          LOAD UART_data, character_P
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_g
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_s
                          CALL send_to_UART
                          CALL send_to_UART
                          CALL send_CR
                          RETURN
                          ;
                          ;
                          ;Send 'OK' to the UART
                          ;
                 send_OK: LOAD UART_data, character_O
                          CALL send_to_UART
                          LOAD UART_data, character_K
                          CALL send_to_UART
                          CALL send_CR
                          RETURN
                          ;
                          ;
                          ;Send ' FAILED' to the UART
                          ;
             send_FAILED: CALL send_space
                          LOAD UART_data, character_F
                          CALL send_to_UART
                          LOAD UART_data, character_A
                          CALL send_to_UART
                          LOAD UART_data, character_I
                          CALL send_to_UART
                          LOAD UART_data, character_L
                          CALL send_to_UART
                          LOAD UART_data, character_E
                          CALL send_to_UART
                          LOAD UART_data, character_D
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ;Send ' PASSED' to the UART
                          ;
             send_PASSED: CALL send_space
                          LOAD UART_data, character_P
                          CALL send_to_UART
                          LOAD UART_data, character_A
                          CALL send_to_UART
                          LOAD UART_data, character_S
                          CALL send_to_UART
                          CALL send_to_UART
                          LOAD UART_data, character_E
                          CALL send_to_UART
                          LOAD UART_data, character_D
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ;
                          ;Send 'Writing Authorisation' to the UART
                          ;
            send_Writing: CALL send_CR
                          LOAD UART_data, character_W
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_n
                          CALL send_to_UART
                          LOAD UART_data, character_g
                          CALL send_to_UART
                          CALL send_space
                          CALL send_Authorisation
                          RETURN
                          ;
                          ;Send simple menu of options to the UART
                          ;
                          ;
               send_Menu: CALL send_CR
                          CALL send_CR
                          LOAD UART_data, character_M
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_n
                          CALL send_to_UART
                          LOAD UART_data, character_u
                          CALL send_to_UART
                          CALL send_CR
                          CALL send_CR
                          LOAD UART_data, character_R
                          CALL send_to_UART
                          CALL send_dash
                          LOAD UART_data, character_R
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_d
                          CALL send_to_UART
                          CALL send_space
                          CALL send_Authorisation
                          CALL send_CR
                          LOAD UART_data, character_E
                          CALL send_to_UART
                          CALL send_dash
                          CALL send_Erase
                          CALL send_Authorisation
                          CALL send_CR
                          LOAD UART_data, character_A
                          CALL send_to_UART
                          CALL send_dash
                          CALL send_Authorise
                          CALL send_CR
                          CALL send_CR
                          LOAD UART_data, character_greater_than   ;prompt for input
                          CALL send_to_UART
                          RETURN
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
             LCD_pulse_E: XOR s4, LCD_E                            ;E=1
                          OUTPUT s4, LCD_output_port
                          CALL delay_1us
                          XOR s4, LCD_E                            ;E=0
                          OUTPUT s4, LCD_output_port
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
         LCD_write_inst4: AND s4, F8                               ;Enable=1 RS=0 Instruction, RW=0 Write, E=0
                          OUTPUT s4, LCD_output_port               ;set up RS and RW >40ns before enable pulse
                          CALL LCD_pulse_E
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
         LCD_write_inst8: LOAD s4, s5
                          AND s4, F0                               ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                          OR s4, LCD_drive                         ;Enable=1
                          CALL LCD_write_inst4                     ;write upper nibble
                          CALL delay_1us                           ;wait >1us
                          LOAD s4, s5                              ;select lower nibble with
                          SL1 s4                                   ;Enable=1
                          SL0 s4                                   ;RS=0 Instruction
                          SL0 s4                                   ;RW=0 Write
                          SL0 s4                                   ;E=0
                          CALL LCD_write_inst4                     ;write lower nibble
                          CALL delay_40us                          ;wait >40us
                          LOAD s4, F0                              ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                          OUTPUT s4, LCD_output_port               ;Release master enable
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
          LCD_write_data: LOAD s4, s5
                          AND s4, F0                               ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                          OR s4, 0C                                ;Enable=1 RS=1 Data, RW=0 Write, E=0
                          OUTPUT s4, LCD_output_port               ;set up RS and RW >40ns before enable pulse
                          CALL LCD_pulse_E                         ;write upper nibble
                          CALL delay_1us                           ;wait >1us
                          LOAD s4, s5                              ;select lower nibble with
                          SL1 s4                                   ;Enable=1
                          SL1 s4                                   ;RS=1 Data
                          SL0 s4                                   ;RW=0 Write
                          SL0 s4                                   ;E=0
                          OUTPUT s4, LCD_output_port               ;set up RS and RW >40ns before enable pulse
                          CALL LCD_pulse_E                         ;write lower nibble
                          CALL delay_40us                          ;wait >40us
                          LOAD s4, F0                              ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                          OUTPUT s4, LCD_output_port               ;Release master enable
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
          LCD_read_data8: LOAD s4, 0E                              ;Enable=1 RS=1 Data, RW=1 Read, E=0
                          OUTPUT s4, LCD_output_port               ;set up RS and RW >40ns before enable pulse
                          XOR s4, LCD_E                            ;E=1
                          OUTPUT s4, LCD_output_port
                          CALL delay_1us                           ;wait >260ns to access data
                          INPUT s5, LCD_input_port                 ;read upper nibble
                          XOR s4, LCD_E                            ;E=0
                          OUTPUT s4, LCD_output_port
                          CALL delay_1us                           ;wait >1us
                          XOR s4, LCD_E                            ;E=1
                          OUTPUT s4, LCD_output_port
                          CALL delay_1us                           ;wait >260ns to access data
                          INPUT s0, LCD_input_port                 ;read lower nibble
                          XOR s4, LCD_E                            ;E=0
                          OUTPUT s4, LCD_output_port
                          AND s5, F0                               ;merge upper and lower nibbles
                          SR0 s0
                          SR0 s0
                          SR0 s0
                          SR0 s0
                          OR s5, s0
                          LOAD s4, 04                              ;Enable=0 RS=1 Data, RW=0 Write, E=0
                          OUTPUT s4, LCD_output_port               ;Stop reading 5V device and release master enable
                          CALL delay_40us                          ;wait >40us
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
                          ;  0C = '00001' Display control, '1' display on, '0' cursor off, '0' cursor blink off
                          ;  01 = '00000001' Display clear
                          ;
                          ;Registers used s0, s1, s2, s3, s4
                          ;
               LCD_reset: CALL delay_20ms                          ;wait more that 15ms for display to be ready
                          LOAD s4, 30
                          CALL LCD_write_inst4                     ;send '3'
                          CALL delay_20ms                          ;wait >4.1ms
                          CALL LCD_write_inst4                     ;send '3'
                          CALL delay_1ms                           ;wait >100us
                          CALL LCD_write_inst4                     ;send '3'
                          CALL delay_40us                          ;wait >40us
                          LOAD s4, 20
                          CALL LCD_write_inst4                     ;send '2'
                          CALL delay_40us                          ;wait >40us
                          LOAD s5, 28                              ;Function set
                          CALL LCD_write_inst8
                          LOAD s5, 06                              ;Entry mode
                          CALL LCD_write_inst8
                          LOAD s5, 0C                              ;Display control
                          CALL LCD_write_inst8
               LCD_clear: LOAD s5, 01                              ;Display clear
                          CALL LCD_write_inst8
                          CALL delay_1ms                           ;wait >1.64ms for display to clear
                          CALL delay_1ms
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
              LCD_cursor: TEST s5, 10                              ;test for line 1
                          JUMP Z, set_line2
                          AND s5, 0F                               ;make address in range 80 to 8F for line 1
                          OR s5, 80
                          CALL LCD_write_inst8                     ;instruction write to set cursor
                          RETURN
               set_line2: AND s5, 0F                               ;make address in range C0 to CF for line 2
                          OR s5, C0
                          CALL LCD_write_inst8                     ;instruction write to set cursor
                          RETURN
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
               delay_1us: LOAD s0, delay_1us_constant
                wait_1us: SUB s0, 01
                          JUMP NZ, wait_1us
                          RETURN
                          ;
                          ;Delay of 40us.
                          ;
                          ;Registers used s0, s1
                          ;
              delay_40us: LOAD s1, 28                              ;40 x 1us = 40us
               wait_40us: CALL delay_1us
                          SUB s1, 01
                          JUMP NZ, wait_40us
                          RETURN
                          ;
                          ;
                          ;Delay of 1ms.
                          ;
                          ;Registers used s0, s1, s2
                          ;
               delay_1ms: LOAD s2, 19                              ;25 x 40us = 1ms
                wait_1ms: CALL delay_40us
                          SUB s2, 01
                          JUMP NZ, wait_1ms
                          RETURN
                          ;
                          ;Delay of 20ms.
                          ;
                          ;Delay of 20ms used during initialisation.
                          ;
                          ;Registers used s0, s1, s2, s3
                          ;
              delay_20ms: LOAD s3, 14                              ;20 x 1ms = 20ms
               wait_20ms: CALL delay_1ms
                          SUB s3, 01
                          JUMP NZ, wait_20ms
                          RETURN
                          ;
                          ;Delay of approximately 1 second.
                          ;
                          ;Registers used s0, s1, s2, s3, s4
                          ;
                delay_1s: LOAD s4, 32                              ;50 x 20ms = 1000ms
                 wait_1s: CALL delay_20ms
                          SUB s4, 01
                          JUMP NZ, wait_1s
                          RETURN
                          ;
                          ;
                          ;Delay of approximately N seconds where 'N' is provided in register s5.
                          ;
                          ;Registers used s0, s1, s2, s3, s4, s5
                          ;
                delay_Ns: CALL delay_1s
                          SUB s5, 01
                          JUMP NZ, delay_Ns
                          RETURN
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
          disp_PicoBlaze: LOAD s5, character_P
                          CALL LCD_write_data
                          LOAD s5, character_i
                          CALL LCD_write_data
                          LOAD s5, character_c
                          CALL LCD_write_data
                          LOAD s5, character_o
                          CALL LCD_write_data
                          LOAD s5, character_B
                          CALL LCD_write_data
                          LOAD s5, character_l
                          CALL LCD_write_data
                          LOAD s5, character_a
                          CALL LCD_write_data
                          LOAD s5, character_z
                          CALL LCD_write_data
                          LOAD s5, character_e
                          CALL LCD_write_data
                          RETURN
                          ;
                          ;
                          ;Display 'Security' on LCD at current cursor position
                          ;
                          ;
           disp_Security: LOAD s5, character_S
                          CALL LCD_write_data
                          LOAD s5, character_e
                          CALL LCD_write_data
                          LOAD s5, character_c
                          CALL LCD_write_data
                          LOAD s5, character_u
                          CALL LCD_write_data
                          LOAD s5, character_r
                          CALL LCD_write_data
                          LOAD s5, character_i
                          CALL LCD_write_data
                          LOAD s5, character_t
                          CALL LCD_write_data
                          LOAD s5, character_y
                          CALL LCD_write_data
                          RETURN
                          ;
                          ;
                          ;Display 'FLASH Serial No.' on LCD at current cursor position
                          ;
                          ;
    disp_FLASH_Serial_No: LOAD s5, character_F
                          CALL LCD_write_data
                          LOAD s5, character_L
                          CALL LCD_write_data
                          LOAD s5, character_A
                          CALL LCD_write_data
                          LOAD s5, character_S
                          CALL LCD_write_data
                          LOAD s5, character_H
                          CALL LCD_write_data
                          LOAD s5, character_space
                          CALL LCD_write_data
                          LOAD s5, character_S
                          CALL LCD_write_data
                          LOAD s5, character_e
                          CALL LCD_write_data
                          LOAD s5, character_r
                          CALL LCD_write_data
                          LOAD s5, character_i
                          CALL LCD_write_data
                          LOAD s5, character_a
                          CALL LCD_write_data
                          LOAD s5, character_l
                          CALL LCD_write_data
                          LOAD s5, character_space
                          CALL LCD_write_data
                          LOAD s5, character_N
                          CALL LCD_write_data
                          LOAD s5, character_o
                          CALL LCD_write_data
                          LOAD s5, character_fullstop
                          CALL LCD_write_data
                          RETURN
                          ;
                          ;
                          ;
                          ;Display 'Authentication' on top line of the LCD
                          ;
                          ;
     disp_Authentication: LOAD s5, 11                              ;Line 1 position 1
                          CALL LCD_cursor
                          LOAD s5, character_A
                          CALL LCD_write_data
                          LOAD s5, character_u
                          CALL LCD_write_data
                          LOAD s5, character_t
                          CALL LCD_write_data
                          LOAD s5, character_h
                          CALL LCD_write_data
                          LOAD s5, character_e
                          CALL LCD_write_data
                          LOAD s5, character_n
                          CALL LCD_write_data
                          LOAD s5, character_t
                          CALL LCD_write_data
                          LOAD s5, character_i
                          CALL LCD_write_data
                          LOAD s5, character_c
                          CALL LCD_write_data
                          LOAD s5, character_a
                          CALL LCD_write_data
                          LOAD s5, character_t
                          CALL LCD_write_data
                          LOAD s5, character_i
                          CALL LCD_write_data
                          LOAD s5, character_o
                          CALL LCD_write_data
                          LOAD s5, character_n
                          CALL LCD_write_data
                          RETURN
                          ;
                          ;
                          ;
                          ;
                          ;Display 'Passed' on lower line of the LCD
                          ;
                          ;
             disp_Passed: LOAD s5, 25                              ;Line 2 position 5
                          CALL LCD_cursor
                          LOAD s5, character_P
                          CALL LCD_write_data
                          LOAD s5, character_a
                          CALL LCD_write_data
                          LOAD s5, character_s
                          CALL LCD_write_data
                          CALL LCD_write_data
                          LOAD s5, character_e
                          CALL LCD_write_data
                          LOAD s5, character_d
                          CALL LCD_write_data
                          RETURN
                          ;
                          ;
                          ;
                          ;
                          ;
                          ;Display 'Failed' on lower line of the LCD
                          ;
                          ;
             disp_Failed: LOAD s5, 25                              ;Line 2 position 5
                          CALL LCD_cursor
                          LOAD s5, character_F
                          CALL LCD_write_data
                          LOAD s5, character_a
                          CALL LCD_write_data
                          LOAD s5, character_i
                          CALL LCD_write_data
                          LOAD s5, character_l
                          CALL LCD_write_data
                          LOAD s5, character_e
                          CALL LCD_write_data
                          LOAD s5, character_d
                          CALL LCD_write_data
                          RETURN
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
                     ISR: STORE s0, ISR_preserve_s0                ;save register contents
                          ;
                          FETCH s0, authentication_status          ;read authentication status
                          COMPARE s0, character_P                  ;test for pass 'P' or fail 'F'
                          JUMP Z, pass_token
                          ;
                          LOAD s0, character_F                     ;send FAIL to link FIFO
                          OUTPUT s0, link_FIFO_write_port
                          LOAD s0, character_A
                          OUTPUT s0, link_FIFO_write_port
                          LOAD s0, character_I
                          OUTPUT s0, link_FIFO_write_port
                          LOAD s0, character_L
                          OUTPUT s0, link_FIFO_write_port
                          JUMP end_ISR
                          ;
              pass_token: OUTPUT s0, link_FIFO_write_port          ;send PASS to link FIFO
                          LOAD s0, character_A
                          OUTPUT s0, link_FIFO_write_port
                          LOAD s0, character_S
                          OUTPUT s0, link_FIFO_write_port
                          OUTPUT s0, link_FIFO_write_port
                          ;
                 end_ISR: FETCH s0, ISR_preserve_s0                ;restore register contents
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
                          ;**************************************************************************************
                          ; End of Program
                          ;**************************************************************************************
