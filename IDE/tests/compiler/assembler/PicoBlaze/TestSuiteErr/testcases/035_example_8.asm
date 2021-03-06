                          ; KCPSM3 Program - Communication with DS2432 secure memory on the Spartan-3E Starter Kit.
                          ;
                          ; Ken Chapman - Xilinx Ltd
                          ;
                          ; Version v1.00 - 5th April 2006
                          ;
                          ; This program uses a 9600 baud UART connection to allow communication with the
                          ; 1-wire interface of the DS2432 memory device from Dallas Semiconductor.
                          ;
                          ; The program only supports a limited number of the DS2432 commands but is intended
                          ; to guide you through the state machine of the device as well as show how
                          ; 1-wire communication is achieved. The DS2432 also uses 8 and 16 bits CRC checks and
                          ; this program includes these calculations to validate the communications.
                          ;
                          ; Commands supported include read ROM, write scratchpad memory and read scratchpad
                          ; memory. These allow you to confirm the ability to both read and write data but
                          ; at no time modify the actual EEPROM secure memory or secret.
                          ;
                          ; There are then two manual commands which allow you to write any byte value that
                          ; you enter and to read a byte from the DS2432 device. At this point you are free
                          ; to experiment and this may result in the device contents being altered. You also
                          ; become responsible for tracking the states but remember that a master reset will
                          ; always return you to a known starting point again.
                          ;
                          ;
                          ;**************************************************************************************
                          ; Port definitions
                          ;**************************************************************************************
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
                          CONSTANT UART_write_port, 04           ;UART Tx data output
                          ;
                          ;
                          CONSTANT DS_wire_in_port, 02           ;Read signal from DS2432 device
                          CONSTANT DS_wire_out_port, 08          ;Drive signal to DS2432 device (open collector)
                          CONSTANT DS_wire, 01                   ;       Signal is bit0 in both cases
                          ;
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
                          ;**************************************************************************************
                          ; Scratch Pad Memory Locations
                          ;**************************************************************************************
                          ;
                          ; Scratch pad memory provides 64 bytes in the address range 00 to 3F hex.
                          ;
                          ;
                          ; Locations for device family code, serial number and 8-bit CRC value
                          ;
                          CONSTANT family_code, 00
                          CONSTANT serial_number0, 01            ;48-bit serial number LS-Byte first
                          CONSTANT serial_number1, 02
                          CONSTANT serial_number2, 03
                          CONSTANT serial_number3, 04
                          CONSTANT serial_number4, 05
                          CONSTANT serial_number5, 06
                          CONSTANT read_ROM_CRC, 07              ;8-bit CRC
                          ;
                          ;
                          ; Locations to store all bytes in a command communication for 16-bit CRC calculation
                          ;
                          CONSTANT command_start, 08
                          ;
                          ;
                          ;**************************************************************************************
                          ; Useful data constants
                          ;**************************************************************************************
                          ;
                          ; Constant to define a software delay of 1us. This must be adjusted to reflect the
                          ; clock applied to KCPSM3. Every instruction executes in 2 clock cycles making the
                          ; calculation highly predictable. The '6' in the following equation even allows for
                          ; 'CALL delay_1us' instruction in the initiating code.
                          ;
                          ; delay_1us_constant =  (clock_rate - 6)/4       Where 'clock_rate' is in MHz
                          ;
                          ; Example: For a 50MHz clock the constant value is (10-6)/4 = 11  (0B Hex).
                          ; For clock rates below 10MHz the value of 1 must be used and the operation will
                          ; become lower than intended.
                          ;
                          CONSTANT delay_1us_constant, 0B
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
                          CONSTANT character_plus, 2B
                          CONSTANT character_comma, 2C
                          CONSTANT character_less_than, 3C       ;'<'
                          CONSTANT character_greater_than, 3E    ;'>'
                          CONSTANT character_open, 28            ;'('
                          CONSTANT character_close, 29           ;')'
                          CONSTANT character_divide, 2F          ;'/'
                          CONSTANT character_equals, 3D
                          CONSTANT character_space, 20
                          CONSTANT character_CR, 0D              ;carriage return
                          CONSTANT character_LF, 0A              ;line feed
                          CONSTANT character_question, 3F        ;'?'
                          CONSTANT character_dollar, 24
                          CONSTANT character_exclaim, 21         ;'!'
                          CONSTANT character_BS, 08              ;Back Space command character
                          CONSTANT character_XON, 11             ;Flow control ON
                          CONSTANT character_XOFF, 13            ;Flow control OFF
                          ;
                          ;
                          ;**************************************************************************************
                          ; Initialise the system and welcome message
                          ;**************************************************************************************
                          ;
              cold_start: CALL DS_wire_init                      ;Ensure DS_wire is not driven (pulled High)
                          CALL delay_1s                          ;Allow everything to settle!
           welcome_start: CALL send_welcome                      ;start up message and version number
                          ;
                          ;
                          ;
                          ; The main program guides the user to follow the command state machine of the
                          ; DS2432 device by only offering those choices that are valid at each stage.
                          ; This program only offers a limited choice and it is left as an exercise to
                          ; the user to modify or expand this choice as required.
                          ;
                          ;**************************************************************************************
                          ; Reset Main menu and command selection
                          ;**************************************************************************************
                          ;
                          ; Initially the only action available is a master reset and test for a presence
                          ; pulse response from the DS2432.
                          ;
              reset_menu: CALL send_reset_menu                   ;Menu and command selection
                          CALL send_CR
                          ;
            reset_prompt: CALL menu_prompt                       ;prompt for user input
                          COMPARE s0, character_H                ;test for commands and execute as required
                          JUMP Z, reset_menu
                          COMPARE s0, character_1
                          JUMP Z, master_reset_regular
                          CALL no_valid_input
                          JUMP reset_prompt                      ;Try again!
                          ;
                          ; The master reset should result in a presence pulse.
                          ; This will be reported and control passed to the appropriate
                          ; menu in the sequence.
                          ;
    master_reset_regular: CALL send_CR
                          CALL DS_init_regular_mode
                          JUMP NC, reset_passed                  ;test for presence pulse
                          CALL send_Fail
                          JUMP reset_menu                        ;fail stays in reset menu
            reset_passed: CALL send_Pass                         ;pass progresses to ROM menu
                          JUMP ROM_menu
                          ;
                          ; After a valid master reset, the only commands available are the ROM
                          ; commands of which only Read ROM and Skip ROM are currently supported.
                          ; Another master reset can be issued is so desired.
                          ;
                ROM_menu: CALL send_ROM_menu                     ;Menu and command selection
                          CALL send_CR
                          ;
              ROM_prompt: CALL menu_prompt                       ;prompt for user input
                          COMPARE s0, character_H                ;test for commands and execute as required
                          JUMP Z, ROM_menu
                          COMPARE s0, character_1
                          JUMP Z, master_reset_regular           ;repeat reset as before
                          COMPARE s0, character_2
                          JUMP Z, read_ROM_command
                          COMPARE s0, character_3
                          JUMP Z, skip_ROM_command
                          CALL no_valid_input
                          JUMP ROM_prompt                        ;Try again!
                          ;
                          ;
                          ;
                          ; After a valid ROM command the DS2432 specific memory commands and SHA-1
                          ; functions become accessible. Each of these end with the DS2432 returned
                          ; back to the waiting for ROM command state completing the menu sequence.
                          ;
             DS2432_menu: CALL send_DS2432_menu                  ;Menu and command selection
                          CALL send_CR
                          ;
           DS2432_prompt: CALL menu_prompt                       ;prompt for user input
                          COMPARE s0, character_H                ;test for commands and execute as required
                          JUMP Z, DS2432_menu
                          COMPARE s0, character_1
                          JUMP Z, master_reset_regular           ;repeat reset as before
                          COMPARE s0, character_2
                          JUMP Z, read_memory_command
                          COMPARE s0, character_3
                          JUMP Z, write_scratchpad_command
                          COMPARE s0, character_4
                          JUMP Z, read_scratchpad_command
                          COMPARE s0, character_5
                          JUMP Z, write_byte_command
                          COMPARE s0, character_6
                          JUMP Z, read_byte_command
                          CALL no_valid_input
                          JUMP DS2432_prompt                     ;Try again!
                          ;
                          ;
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; Prompt and selection tasks used in menus.
                          ;**************************************************************************************
                          ;
                          ; Prompts for input with > character and waits for key stroke which is
                          ; returned in upper case in register s0
                          ;
             menu_prompt: CALL send_CR
                          CALL send_CR
                          LOAD UART_data, character_greater_than ;prompt for input
                          CALL send_to_UART
                          CALL read_upper_case
                          RETURN
                          ;
          no_valid_input: CALL send_CR                           ;no valid command input
                          LOAD UART_data, character_question     ;display ???
                          CALL send_to_UART
                          CALL send_to_UART
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ;**************************************************************************************
                          ; DS2432 Read ROM Command.
                          ;**************************************************************************************
                          ;
                          ; The read ROM command (33 hex) allows the 8-bit family code, 48-bit unique serial
                          ; number and 8-bit CRC to be read from the DS2432 device.
                          ;
                          ; This routine reads the values and places them in KCPSM3 scratch pad memory
                          ; locations for future reference. These locations should be defined with constants
                          ; as follows and MUST be in consecutive ascending locations.
                          ;
                          ;  family_code
                          ;     Location to store family code which should be 33 hex
                          ;  serial_number0 to serial_number5
                          ;     6 bytes to hold 48-bit serial number (LS-byte first).
                          ;  read_ROM_CRC
                          ;     8-bit CRC value for the above data.
                          ;
                          ;
                          ; The routine also displays the values read and performs a verification of the
                          ; 8-bit CRC displaying a 'pass' or 'fail' message as appropriate.
                          ;
        read_ROM_command: LOAD s3, 33                            ;Read ROM Command
                          CALL write_byte_slow                   ;transmit command
                          LOAD s5, family_code                   ;memory pointer
           read_ROM_loop: CALL read_byte_slow                    ;read response into s3
                          STORE s3, (s5)                         ;store value
                          COMPARE s5, read_ROM_CRC               ;8-bytes to read
                          JUMP Z, display_ROM
                          ADD s5, 01
                          JUMP read_ROM_loop
             display_ROM: CALL send_CR
                          CALL send_code                         ;'code=' to display family code
                          FETCH s0, family_code
                          CALL send_hex_byte
                          CALL send_CR
                          CALL send_serial                       ;'serial=' to display family code
                          LOAD s5, serial_number5                ;memory pointer starting MS-byte first
        disp_serial_loop: FETCH s0, (s5)
                          CALL send_hex_byte
                          COMPARE s5, serial_number0
                          JUMP Z, end_serial
                          SUB s5, 01
                          JUMP disp_serial_loop
              end_serial: CALL send_CR
                          CALL send_CRC                          ;'CRC=' to display CRC value
                          FETCH s0, read_ROM_CRC
                          CALL send_hex_byte
                          CALL send_CR
                          CALL compute_CRC8                      ;compute CRC value in s0
                          FETCH s1, read_ROM_CRC                 ;compare with received value
                          COMPARE s0, s1
                          JUMP NZ, crc8_fail
                          CALL send_Pass
                          JUMP DS2432_menu                       ;now in memory and SHA-1 functions level
               crc8_fail: CALL send_Fail
                          JUMP ROM_menu                          ;can not proceed to next level
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; DS2432 Skip ROM Command.
                          ;**************************************************************************************
                          ;
                          ; The skip ROM command (CC hex) is a short cut to the memory commands and SHA-1
                          ; functions which means that the 64 bit ROM contents do not have to be read.
                          ;
                          ;
        skip_ROM_command: LOAD s3, CC                            ;Skip ROM Command
                          CALL write_byte_slow                   ;transmit command
                          CALL send_OK
                          JUMP DS2432_menu
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; DS2432 Read Memory Command.
                          ;**************************************************************************************
                          ;
                          ; The read memory command (F0 hex) allows the entire memory contents to be read
                          ; except for the secret. This routine displays the address followed by 8 bytes
                          ; of data on each line until the address 0097 is reached.
                          ;
                          ; The initial 'F0' command must be followed by the 16-bit start address transmitted
                          ; LS-byte first. Then reads must continue until address 0097 has been read for the
                          ; command to complete naturally (otherwise a master reset is required).
                          ;
                          ;
                          ;
     read_memory_command: LOAD s3, F0                            ;read memory Command
                          CALL write_byte_slow                   ;transmit command
                          LOAD s5, 00                            ;initial address in [s5,s4]=0000
                          LOAD s4, 00
                          LOAD s3, s4                            ;transmit address
                          CALL write_byte_slow
                          LOAD s3, s5
                          CALL write_byte_slow
           rmc_line_loop: CALL send_CR
                          LOAD s0, s5                            ;display 16-bit address
                          CALL send_hex_byte
                          LOAD s0, s4
                          CALL send_hex_byte
                          CALL send_space
                          CALL send_space
           rmc_data_loop: CALL send_space
                          CALL read_byte_slow                    ;read data into s3
                          LOAD s0, s3                            ;display byte
                          CALL send_hex_byte
                          ADD s4, 01                             ;increment address
                          ADDCY s5, 00
                          TEST s4, 07                            ;test for 8-byte boundary
                          JUMP NZ, rmc_data_loop
                          COMPARE s4, 98                         ;test for last address
                          JUMP NZ, rmc_line_loop
                          CALL send_OK
                          JUMP reset_menu                        ;needs master reset next
                          ;
                          ;
                          ;**************************************************************************************
                          ; DS2432 Write Scratchpad Memory Command.
                          ;**************************************************************************************
                          ;
                          ; The write scratchpad memory command (0F hex) allows 8-bytes of data to be written
                          ; together with a target address for final storage in the main memory map.
                          ;
                          ; The DS2432 provides an initial confirmation of the write by returning a 16-bit CRC
                          ; value which KCPSM3 tests. The CRC is computed based on the command, address and
                          ; data transmitted (11 bytes). To achieve this, all bytes transmitted to the DS2432
                          ; are recorded in KCPSM3 scratch pad memory in ascending locations starting at
                          ; the location defined by constant 'command_start'.
                          ;
                          ; This routine prompts the user to enter the 16-bit target address is to be loaded
                          ; into the target address registers TA2 and TA1 in the DS2432 device. Note that only
                          ; address values below 0090 hex are valid. If the address is too high, then the
                          ; DS2432 aborts the command and this routine will too.
                          ;
                          ; Also note that the address will be forced internally to the DS2432 to match an
                          ; 8-byte boundary address in which the least significant 3-bits are reset to '000'
                          ; regardless of the address provided. The CRC still reflects the transmitted address.
                          ;
                          ; After providing a valid address, the routine then prompts the user to enter
                          ; 8 bytes of data which are written to the DS2432.
                          ;
                          ;
                          ;
write_scratchpad_command: LOAD sE, command_start                 ;pointer to memory
                          LOAD s3, 0F                            ;write scratchpad memory Command
                          STORE s3, (sE)                         ;record command sequence
                          ADD sE, 01                             ;increment pointer
                          CALL write_byte_slow                   ;transmit command
           wsc_addr_loop: CALL send_address                      ;obtain 16-bit address 0000 to FFFF in [s5,s4]
                          CALL obtain_8bits
                          JUMP C, wsc_addr_loop                  ;bad input address
                          LOAD s5, s0
                          CALL obtain_8bits
                          JUMP C, wsc_addr_loop                  ;bad input address
                          LOAD s4, s0
                          LOAD s3, s4                            ;transmit target address TA1 (LS-Byte)
                          STORE s3, (sE)                         ;record command sequence
                          ADD sE, 01                             ;increment pointer
                          CALL write_byte_slow
                          LOAD s3, s5                            ;transmit target address TA2 (MS-Byte)
                          STORE s3, (sE)                         ;record command sequence
                          ADD sE, 01                             ;increment pointer
                          CALL write_byte_slow
                          COMPARE s5, 00                         ;check address less than 0090 hex
                          JUMP NZ, end_write_scratchpad          ;DS2432 aborts command and so
                          COMPARE s4, 90                         ;no need to read data bytes.
                          JUMP NC, end_write_scratchpad
                          LOAD s4, 00                            ;initialise byte counter
           wsc_data_loop: CALL send_data                         ;obtain a byte of data
                          LOAD UART_data, s4                     ;display which byte requested
                          ADD UART_data, character_0             ;convert to ASCII
                          CALL send_to_UART
                          CALL send_equals
                          CALL obtain_8bits
                          JUMP C, wsc_data_loop                  ;bad input data
                          LOAD s3, s0                            ;transmit byte
                          STORE s3, (sE)                         ;record command sequence
                          ADD sE, 01                             ;increment pointer
                          CALL write_byte_slow
                          ADD s4, 01                             ;count bytes
                          COMPARE s4, 08
                          JUMP NZ, wsc_data_loop
                          CALL read_byte_slow                    ;read back the 16-bit CRC into [s5,s4]
                          LOAD s4, s3
                          CALL read_byte_slow
                          LOAD s5, s3
                          STORE s4, (sE)                         ;record command sequence
                          ADD sE, 01                             ;increment pointer
                          STORE s5, (sE)                         ;record command sequence
                          CALL send_CR
                          CALL send_CRC                          ;'CRC=' to display CRC value
                          LOAD s0, s5
                          CALL send_hex_byte
                          LOAD s0, s4
                          CALL send_hex_byte
                          CALL send_CR
                          LOAD s2, 0B                            ;11 (0B hex) bytes transmitted in this command
                          CALL compute_CRC16                     ;compute CRC value in [s1,s0]
                          FETCH s5, (sE)                         ;compare with received value
                          SUB sE, 01
                          FETCH s4, (sE)                         ;compare with received value
                          COMPARE s5, s1
                          JUMP NZ, wsc_crc16_fail
                          COMPARE s4, s0
                          JUMP NZ, wsc_crc16_fail
                          CALL send_Pass
                          JUMP reset_menu                        ;needs master reset next
          wsc_crc16_fail: CALL send_Fail
    end_write_scratchpad: JUMP reset_menu                        ;needs master reset next
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; Write Byte Command.
                          ;**************************************************************************************
                          ;
                          ; This routine simply allows you to specify any byte and write it to the DS2432 device.
                          ; Apart from checking that the value is in the range 00 to FF hex, there is no checking
                          ; of the meaning of the value to the DS2432 device and all attempts to track the
                          ; state machine will be lost.
                          ;
                          ;
      write_byte_command: CALL send_CR
                          CALL send_Byte                         ;obtain a byte of data
                          CALL send_equals
                          CALL obtain_8bits
                          JUMP C, write_byte_command             ;bad input data
                          LOAD s3, s0                            ;transmit byte
                          CALL write_byte_slow                   ;transmit byte
                          CALL send_OK
                          JUMP DS2432_menu                       ;remain in memory and function menu
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; Read Byte Command.
                          ;**************************************************************************************
                          ;
                          ; This routine will attempt to read a single byte from the DS2432 device.
                          ; The results will be displayed on the terminal. There is no checking of the meaning
                          ; of the value received from the DS2432 device and all attempts to track the
                          ; state machine will be lost.
                          ;
                          ;
       read_byte_command: CALL send_CR
                          CALL send_Byte                         ;obtain a byte of data
                          CALL send_equals
                          CALL read_byte_slow                    ;receive byte
                          LOAD s0, s3                            ;display value
                          CALL send_hex_byte
                          CALL send_OK
                          JUMP DS2432_menu                       ;remain in memory and function menu
                          ;
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; DS2432 Read Scratchpad Memory Command.
                          ;**************************************************************************************
                          ;
                          ; The read scratchpad memory command (AA hex) allows the 8-bytes of data previously
                          ; to be written into the scratchpad memory to be read back for verification together with
                          ; the target address, a transfer status register and a 16-bit CRC value.
                          ;
                          ; The 16-bit CRC is formed of the command byte, address TA1 and TA2, E/S byte and 8 data
                          ; bytes as transmitted (12 bytes). These may not be the same as the values provided
                          ; during a previous write to scratchpad memory. All these bytes are recorded in KCPSM3
                          ; scratch pad memory in ascending locations starting at the location defined by
                          ; constant 'command_start'.
                          ;
                          ;
 read_scratchpad_command: LOAD sE, command_start                 ;pointer to memory
                          LOAD s3, AA                            ;read scratchpad memory Command
                          STORE s3, (sE)                         ;record command sequence
                          ADD sE, 01                             ;increment pointer
                          CALL write_byte_slow                   ;transmit command
                          CALL send_address                      ;display 'Address='
                          CALL read_byte_slow                    ;read address into [s5,s4]
                          LOAD s4, s3
                          CALL read_byte_slow
                          LOAD s5, s3
                          STORE s4, (sE)                         ;record sequence
                          ADD sE, 01                             ;increment pointer
                          STORE s5, (sE)                         ;record sequence
                          ADD sE, 01                             ;increment pointer
                          LOAD s0, s5                            ;display address
                          CALL send_hex_byte
                          LOAD s0, s4
                          CALL send_hex_byte
                          CALL send_ES                           ;display 'E/S='
                          CALL read_byte_slow                    ;read E/S register
                          STORE s3, (sE)                         ;record sequence
                          ADD sE, 01                             ;increment pointer
                          LOAD s0, s3                            ;display value
                          CALL send_hex_byte
                          CALL send_data                         ;display 'Data='
                          CALL send_equals
                          LOAD s4, 08                            ;8 bytes to read
                rsc_loop: CALL send_space
                          CALL read_byte_slow                    ;read data byte
                          STORE s3, (sE)                         ;record sequence
                          ADD sE, 01                             ;increment pointer
                          LOAD s0, s3                            ;display value
                          CALL send_hex_byte
                          SUB s4, 01                             ;count bytes
                          JUMP NZ, rsc_loop
                          CALL read_byte_slow                    ;read 16-bit CRC into [s5,s4]
                          LOAD s4, s3
                          CALL read_byte_slow
                          LOAD s5, s3
                          STORE s4, (sE)                         ;record command sequence
                          ADD sE, 01                             ;increment pointer
                          STORE s5, (sE)                         ;record command sequence
                          CALL send_CR
                          CALL send_CRC                          ;'CRC=' to display CRC value
                          LOAD s0, s5
                          CALL send_hex_byte
                          LOAD s0, s4
                          CALL send_hex_byte
                          CALL send_CR
                          LOAD s2, 0C                            ;12 (0C hex) bytes in this command
                          CALL compute_CRC16                     ;compute CRC value in [s1,s0]
                          FETCH s5, (sE)                         ;compare with received value
                          SUB sE, 01
                          FETCH s4, (sE)                         ;compare with received value
                          COMPARE s5, s1
                          JUMP NZ, rsc_crc16_fail
                          COMPARE s4, s0
                          JUMP NZ, rsc_crc16_fail
                          CALL send_Pass
                          JUMP reset_menu                        ;needs master reset next
          rsc_crc16_fail: CALL send_Fail
                          JUMP reset_menu                        ;needs master reset next
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; Compute 8-bit CRC used by DS2432.
                          ;**************************************************************************************
                          ;
                          ; The DS2432 computes an 8-bit CRC using the polynomial X8 + X5 + X4 + 1.
                          ; See the DS2432 data sheet for full details.
                          ;
                          ; Test input value of value 00 00 00 01 B8 1C 02
                          ; should produce CRC=A2.
                          ;
                          ; This routine computes the same CRC based on the values stored in the KCPSM3
                          ; scratch pad memory by the read ROM command. The result is returned in register s0.
                          ;
                          ; Registers used s0,s1,s2,s3,s4,s5,s6,s7,s8,s9
                          ;
                          ;
                          ;Start by loading family code and serial number (56-bits) into
                          ;register set [s9,s8,s7,s6,s5,s4,s3] so they can be shifted out
                          ;LSB first.
                          ;
            compute_CRC8: FETCH s3, family_code
                          FETCH s4, serial_number0
                          FETCH s5, serial_number1
                          FETCH s6, serial_number2
                          FETCH s7, serial_number3
                          FETCH s8, serial_number4
                          FETCH s9, serial_number5
                          LOAD s2, 38                            ;56 bits to shift (38 hex)
                          LOAD s0, 00                            ;clear CRC value
               crc8_loop: LOAD s1, s0                            ;copy current CRC value
                          XOR s1, s3                             ;Need to know LSB XOR next input bit
                          TEST s1, 01                            ;test result of XOR in LSB
                          JUMP NC, crc8_shift
                          XOR s0, 18                             ;compliment bits 3 and 4 of CRC
              crc8_shift: SR0 s1                                 ;Carry gets LSB XOR next input bit
                          SRA s0                                 ;shift Carry into MSB to form new CRC value
                          SR0 s9                                 ;shift input value
                          SRA s8
                          SRA s7
                          SRA s6
                          SRA s5
                          SRA s4
                          SRA s3
                          SUB s2, 01                             ;count iterations
                          JUMP NZ, crc8_loop
                          RETURN
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; Compute 16-bit CRC used by DS2432.
                          ;**************************************************************************************
                          ;
                          ; The DS2432 computes a 16-bit CRC using the polynomial X16 + X15 + X2 + 1.
                          ; See the DS2432 data sheet for full details.
                          ;
                          ; Note that the value formed in the CRC shift register is inverted to give the
                          ; same value as that sent from the DS2432 during scratchpad write, scratchpad read
                          ; and read auth page commands.
                          ;
                          ; This routine computes the CRC based on the values stored in the KCPSM3
                          ; scratch pad memory starting at address defined by constant 'command_start'.
                          ; register 's2' must specify how many bytes are to be used in the calculation
                          ; and the CRC is returned in register pair [s1,s0] once it has been inverted.
                          ;
                          ; Registers used s0,s1,s2,s3,s4,s5,s6
                          ;
                          ;
                          ;Start by loading family code and serial number (56-bits) into
                          ;register set [s9,s8,s7,s6,s5,s4,s3] so they can be shifted out
                          ;LSB first.
                          ;
           compute_CRC16: LOAD s5, command_start                 ;memory pointer
                          LOAD s0, 00                            ;clear CRC value
                          LOAD s1, 00
         crc16_byte_loop: FETCH s4, (s5)                         ;read input byte
                          LOAD s3, 08                            ;8-bits to shift
          crc16_bit_loop: LOAD s6, s0                            ;copy current CRC value
                          XOR s6, s4                             ;Need to know LSB XOR next input bit
                          TEST s6, 01                            ;test result of XOR in LSB
                          JUMP NC, crc16_shift
                          XOR s0, 02                             ;compliment bit 1 of CRC
                          XOR s1, 40                             ;compliment bit 14 of CRC
             crc16_shift: SR0 s6                                 ;Carry gets LSB XOR next input bit
                          SRA s1                                 ;shift Carry into MSB to form new CRC value
                          SRA s0
                          SR0 s4                                 ;shift input value
                          SUB s3, 01                             ;count bits
                          JUMP NZ, crc16_bit_loop                ;next bit
                          ADD s5, 01                             ;increment memory pointer
                          SUB s2, 01                             ;count bytes
                          JUMP NZ, crc16_byte_loop               ;next byte
                          XOR s0, FF                             ;1's complement of CRC value
                          XOR s1, FF
                          RETURN
                          ;
                          ;
                          ;**************************************************************************************
                          ; Initialise the DS2432 1-wire interface.
                          ;**************************************************************************************
                          ;
                          ; The 1-wire interface is an open-collector communication scheme employing an external
                          ; pull-up resistor of 680 Ohms.
                          ;
                          ; The hardware section of this translates the one bit signal from PicoBlaze such that
                          ; when this signal is Low the output is driven Low, but when it is High, it turns off
                          ; the output buffer and the signal is pulled High externally.
                          ;
                          ; This initialisation routine simply ensures that the line is High after configuration.
                          ; It is vital that DS_wire is generally in the High state because it is the only way in
                          ; which the DS2432 device derives power to operate.
                          ;
                          ; Registers used s0
                          ;
            DS_wire_init: LOAD s0, DS_wire
                          OUTPUT s0, DS_wire_out_port
                          RETURN
                          ;
                          ;
                          ;**************************************************************************************
                          ; DS2432 initialisation - Regular Speed.
                          ;**************************************************************************************
                          ;
                          ; The initialisation sequence must be performed before any communication can be
                          ; made with the DS2432 device. This involves the application of an active Low master
                          ; reset pulse.
                          ;
                          ; The regular (slow) speed communication is established by transmitting an active
                          ; Low reset pulse for a duration of at least 480us. This design generates a 500us pulse.
                          ;
                          ; The DS2432 acknowledges the reset and the setting of regular mode by generating an
                          ; active Low 'Rx Presence Pulse'. This presence pulse can start 15 to 60us after the
                          ; reset pulse and will end between 120 and 300us after the reset pulse.
                          ;
                          ; To confirm that regular mode has been set, this routine confirms that the presence pulse
                          ; is active only after 60us have elapsed since the reset pulse. This ensures that the
                          ; faster presence pulse of overdrive mode can not be detected.
                          ;
                          ; The carry flag will be set if no valid presence pulse was received (wire remained High) and
                          ; can be used to indicate an initialisation failure or success.
                          ;
                          ; The routine only completes 300us after the presence pulse to ensure the DS2432 has
                          ; completed the presence pulse and is ready for the first operation.
                          ;
                          ; Registers used s0,s1,s2
                          ;
    DS_init_regular_mode: LOAD s0, 00                            ;transmit reset pulse
                          OUTPUT s0, DS_wire_out_port
                          ;Delay of 500us is equivalent to 12500 instructions at 50MHz.
                          ;This delay loop is formed of 28 instructions requiring 446 repetitions.
                          LOAD s2, 01                            ;[s3,s2]=445 decimal (01BD hex)
                          LOAD s1, BD
           rm_wait_500us: CALL delay_1us                         ;25 instructions including CALL
                          SUB s1, 01                             ;decrement delay counter
                          SUBCY s2, 00
                          JUMP NC, rm_wait_500us                 ;repeat until -1
                          LOAD s0, 01                            ;end of regular reset pulse
                          OUTPUT s0, DS_wire_out_port
                          ;Delay of 60us is equivalent to 1500 instructions at 50MHz.
                          ;This delay and is formed of 27 instructions requiring 56 repetitions.
                          LOAD s1, 38                            ;56 (38 hex)
            rm_wait_60us: CALL delay_1us                         ;25 instructions including CALL
                          SUB s1, 01                             ;decrement delay counter
                          JUMP NZ, rm_wait_60us                  ;repeat until zero
                          ;The DS_wire is now checked at approximately 1us intervals for the next 240us looking
                          ;to detect an active Low presence pulse. The 240us is equivalent to 6000 instructions
                          ;at 50MHz and this polling loop is formed of 33 instructions requiring 182 repetitions.
                          LOAD s2, 01                            ;set bit which will be reset by a presence pulse
                          LOAD s1, B6                            ;182 (B6 hex)
           rm_poll_240us: CALL delay_1us                         ;25 instructions including CALL
                          CALL read_DS_wire                      ;read wire - 5 instructions including CALL
                          AND s2, s0                             ;clear flag if DS_wire was Low
                          SUB s1, 01                             ;decrement delay counter
                          JUMP NZ, rm_poll_240us                 ;repeat until zero
                          TEST s2, 01                            ;set carry flag if no pulse detected
                          RETURN
                          ;
                          ;
                          ;**************************************************************************************
                          ; Read the DS_wire
                          ;**************************************************************************************
                          ;
                          ; The DS_wire signal is read and returned in bit0 of register 's0'.
                          ; Additionally the carry flag is set if the signal is High and reset if Low
                          ;
                          ; Registers used s0
                          ;
            read_DS_wire: INPUT s0, DS_wire_in_port
                          AND s0, DS_wire                        ;ensure only bit0 is active
                          TEST s0, DS_wire                       ;set carry flag if DS_wire is High
                          RETURN
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; Write a byte to DS2432 in regular speed mode.
                          ;**************************************************************************************
                          ;
                          ; Bytes are written to the DS2432 with LSB first.
                          ;
                          ; The byte to be written should be provided in register 's3' and this will be preserved.
                          ;
                          ; Registers used s0,s1,s2,s3
                          ;
         write_byte_slow: LOAD s2, 08                            ;8 bits to transmit
                wbs_loop: RR s3                                  ;test next bit LSB first
                          JUMP C, wbs1                           ;transmit '0' or '1'
                          CALL write_Low_slow
                          JUMP next_slow_bit
                    wbs1: CALL write_High_slow
           next_slow_bit: SUB s2, 01                             ;count bits
                          JUMP NZ, wbs_loop                      ;repeat until 8-bits transmitted
                          RETURN
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; Write a '0' to DS_wire in regular speed mode.
                          ;**************************************************************************************
                          ;
                          ; To write a '0' to the DS_wire the signal must be Low for 60 to 120us. This design
                          ; generates a 78us active Low pulse.
                          ;
                          ; The DS2432 then requires at least 1us of recovery time for which this routine
                          ; provides a 2us delay such that the entire write Low process (slot time) is 80us.
                          ; A recovery time of 1us was also found to be marginal in practice probably due
                          ; to the rise time of the DS_wire via the external pull up resistor.
                          ;
                          ; Registers used s0,s1
                          ;
          write_Low_slow: LOAD s0, 00                            ;transmit Low pulse
                          OUTPUT s0, DS_wire_out_port
                          ;Delay of 78us is equivalent to 1950 instructions at 50MHz.
                          ;This delay loop is formed of 27 instructions requiring 72 repetitions.
                          LOAD s1, 48                            ;72 (48 hex)
           wls_wait_78us: CALL delay_1us                         ;25 instructions including CALL
                          SUB s1, 01                             ;decrement delay counter
                          JUMP NZ, wls_wait_78us                 ;repeat until zero
                          LOAD s0, 01                            ;end of Low pulse
                          OUTPUT s0, DS_wire_out_port
                          CALL delay_1us                         ;2us recovery time
                          CALL delay_1us
                          RETURN
                          ;
                          ;
                          ;**************************************************************************************
                          ; Write a '1' to DS_wire in regular speed mode.
                          ;**************************************************************************************
                          ;
                          ; To write a '1' to the DS_wire the signal must be Low for 1 to 15us to instigate the
                          ; write of the data. This design generates an 8us active Low pulse for this purpose.
                          ;
                          ; Then the output must be High for 53 to 114us to provide the '1' for the DS2432 to
                          ; read and then provide recovery time. This design implements a 72us delay such that
                          ; the entire write High process (slot time) is 80us
                          ;
                          ; Registers used s0,s1
                          ;
         write_High_slow: LOAD s0, 00                            ;transmit Low pulse
                          OUTPUT s0, DS_wire_out_port
                          ;Delay of 8us is equivalent to 200 instructions at 50MHz.
                          ;This delay loop is formed of 27 instructions requiring 8 repetitions.
                          LOAD s1, 08                            ;8 (08 hex)
            whs_wait_8us: CALL delay_1us                         ;25 instructions including CALL
                          SUB s1, 01                             ;decrement delay counter
                          JUMP NZ, whs_wait_8us                  ;repeat until zero
                          LOAD s0, 01                            ;end of Low pulse
                          OUTPUT s0, DS_wire_out_port
                          ;Delay of 72us is equivalent to 1800 instructions at 50MHz.
                          ;This delay loop is formed of 27 instructions requiring 67 repetitions.
                          LOAD s1, 43                            ;67 (43 hex)
           whs_wait_72us: CALL delay_1us                         ;25 instructions including CALL
                          SUB s1, 01                             ;decrement delay counter
                          JUMP NZ, whs_wait_72us                 ;repeat until zero
                          RETURN
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; Read a byte from DS2432 in regular speed mode.
                          ;**************************************************************************************
                          ;
                          ; Bytes are read from the DS2432 with LSB first.
                          ;
                          ; The byte read will be returned in register 's3'.
                          ;
                          ; Registers used s0,s1,s2,s3
                          ;
          read_byte_slow: LOAD s2, 08                            ;8 bits to receive
                rbs_loop: CALL read_bit_slow                     ;read next bit LSB first
                          SUB s2, 01                             ;count bits
                          JUMP NZ, rbs_loop                      ;repeat until 8-bits received
                          RETURN
                          ;
                          ;
                          ;
                          ;
                          ;**************************************************************************************
                          ; Read a data bit sent from the DS2432 in regular speed mode.
                          ;**************************************************************************************
                          ;
                          ; To read a bit, PicoBlaze must initiate the processed with an active Low pulse of
                          ; 1 to 15us. This design generates a 4us active Low pulse for this purpose.
                          ;
                          ; Then DS2432 responds to the Low pulse by diving DS_wire in two differet ways
                          ; depending on the logic level it is trying to send back.
                          ;
                          ; For a logic '0' the DS2432 will drive the DS-wire Low for up to 15us after
                          ; the start of the instigating pulse. Therefore PicoBlaze must read the DS-wire
                          ; before this time has elapsed but only after it has itself released the wire.
                          ;
                          ; For a logic '1' the DS2432 will do nothing and hence the DS-wire will be pulled
                          ; High by the external resistor after PicoBlaze has released the wire. PicoBlaze
                          ; will sample the wire and detect the High level.
                          ;
                          ; In this design, PicoBlaze needs to detect the logic state of the wire after
                          ; releasing the wire at 4us. Sampling the wire too quickly would not provide
                          ; adequate time for a High signal to be formed by the pull up resistor. However, it
                          ; must sample the wire before 15us have elapsed and any potential Low is removed.
                          ; This design samples the wire at 12us which is 8us after the initiation pulse ends.
                          ;
                          ; A further delay of 68us is then allowed for the DS2432 to stop transmitting and
                          ; to recover. This also mean that the entire read process (slot time) is 80us.
                          ;
                          ; The received data bit is SHIFTED into the MSB of register 's3'. In this way
                          ; the reception of 8-bits will shift the first bit into the LSB position of 's3'.
                          ;
                          ; Registers used s0,s1,s3
                          ;
           read_bit_slow: LOAD s0, 00                            ;transmit Low pulse
                          OUTPUT s0, DS_wire_out_port
                          ;Delay of 4us is equivalent to 100 instructions at 50MHz.
                          ;This delay loop is formed of 27 instructions requiring 4 repetitions.
                          LOAD s1, 04                            ;4 (04 hex)
            rbs_wait_4us: CALL delay_1us                         ;25 instructions including CALL
                          SUB s1, 01                             ;decrement delay counter
                          JUMP NZ, rbs_wait_4us                  ;repeat until zero
                          LOAD s0, 01                            ;end of Low pulse
                          OUTPUT s0, DS_wire_out_port
                          ;Delay of 8us is equivalent to 200 instructions at 50MHz.
                          ;This delay loop is formed of 27 instructions requiring 8 repetitions.
                          LOAD s1, 08                            ;8 (08 hex)
            rbs_wait_8us: CALL delay_1us                         ;25 instructions including CALL
                          SUB s1, 01                             ;decrement delay counter
                          JUMP NZ, rbs_wait_8us                  ;repeat until zero
                          CALL read_DS_wire                      ;sample wire (carry = state)
                          SRA s3                                 ;shift received bit into MSB of s3
                          ;Delay of 68us is equivalent to 1700 instructions at 50MHz.
                          ;This delay loop is formed of 27 instructions requiring 63 repetitions.
                          LOAD s1, 3F                            ;63 (3F hex)
           rbs_wait_68us: CALL delay_1us                         ;25 instructions including CALL
                          SUB s1, 01                             ;decrement delay counter
                          JUMP NZ, rbs_wait_68us                 ;repeat until zero
                          RETURN
                          ;
                          ;
                          ;**************************************************************************************
                          ; Software delay routines
                          ;**************************************************************************************
                          ;
                          ; Delay of 1us.
                          ;
                          ; Constant value defines reflects the clock applied to KCPSM3. Every instruction
                          ; executes in 2 clock cycles making the calculation highly predictable. The '6' in
                          ; the following equation even allows for 'CALL delay_1us' instruction in the initiating code.
                          ;
                          ; delay_1us_constant =  (clock_rate - 6)/4       Where 'clock_rate' is in MHz
                          ;
                          ; Register used s0
                          ;
               delay_1us: LOAD s0, delay_1us_constant
                wait_1us: SUB s0, 01
                          JUMP NZ, wait_1us
                          RETURN
                          ;
                          ; Delay of 40us.
                          ;
                          ; Registers used s0, s1
                          ;
              delay_40us: LOAD s1, 28                            ;40 x 1us = 40us
               wait_40us: CALL delay_1us
                          SUB s1, 01
                          JUMP NZ, wait_40us
                          RETURN
                          ;
                          ;
                          ; Delay of 1ms.
                          ;
                          ; Registers used s0, s1, s2
                          ;
               delay_1ms: LOAD s2, 19                            ;25 x 40us = 1ms
                wait_1ms: CALL delay_40us
                          SUB s2, 01
                          JUMP NZ, wait_1ms
                          RETURN
                          ;
                          ; Delay of 20ms.
                          ;
                          ; Registers used s0, s1, s2, s3
                          ;
              delay_20ms: LOAD s3, 14                            ;20 x 1ms = 20ms
               wait_20ms: CALL delay_1ms
                          SUB s3, 01
                          JUMP NZ, wait_20ms
                          RETURN
                          ;
                          ; Delay of approximately 1 second.
                          ;
                          ; Registers used s0, s1, s2, s3, s4
                          ;
                delay_1s: LOAD s4, 14                            ;50 x 20ms = 1000ms
                 wait_1s: CALL delay_20ms
                          SUB s4, 01
                          JUMP NZ, wait_1s
                          RETURN
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
                          ;**************************************************************************************
                          ; Useful ASCII conversion and handling routines
                          ;**************************************************************************************
                          ;
                          ; Convert value provided in register s0 into ASCII characters
                          ;
                          ; The value provided must in the range 0 to 99 and will be converted into
                          ; two ASCII characters.
                          ;     The number of 'tens' will be represented by an ASCII character returned in register s1.
                          ;     The number of 'units' will be represented by an ASCII character returned in register s0.
                          ;
                          ; The ASCII representations of '0' to '9' are 30 to 39 hexadecimal which is simply 30 hex added to
                          ; the actual decimal value.
                          ;
                          ; Registers used s0 and s1.
                          ;
        decimal_to_ASCII: LOAD s1, 30                            ;load 'tens' counter with ASCII for '0'
            test_for_ten: ADD s1, 01                             ;increment 'tens' value
                          SUB s0, 0A                             ;try to subtract 10 from the supplied value
                          JUMP NC, test_for_ten                  ;repeat if subtraction was possible without underflow.
                          SUB s1, 01                             ;'tens' value one less ten due to underflow
                          ADD s0, 3A                             ;restore units value (the remainder) and convert to ASCII
                          RETURN
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
                          ; Determine the numerical value of a two character decimal string held in
                          ; scratch pad memory such the result is in the range 0 to 99 (00 to 63 hex).
                          ;
                          ; The string must be stored in two consecutive memory locations and the
                          ; location of the first (tens) character supplied in the s1 register.
                          ; The result is provided in register s2. Strings not using characters in the
                          ; range '0' to '9' are signified by the return with the CARRY flag set.
                          ;
                          ; Registers used s0, s1 and s2.
                          ;
          2char_to_value: FETCH s0, (s1)                         ;read 'tens' character
                          CALL 1char_to_value                    ;convert to numerical value
                          RETURN C                               ;bad character - CARRY set
                          LOAD s2, s0
                          SL0 s2                                 ;multiply 'tens' value by 10 (0A hex)
                          SL0 s2
                          ADD s2, s0
                          SL0 s2
                          ADD s1, 01                             ;read 'units' character
                          FETCH s0, (s1)
                          CALL 1char_to_value                    ;convert to numerical value
                          RETURN C                               ;bad character - CARRY set
                          ADD s2, s0                             ;add units to result and clear CARRY flag
                          RETURN
                          ;
                          ;
                          ; Convert hexadecimal value provided in register s0 into ASCII characters
                          ;
                          ; The value provided must can be any value in the range 00 to FF and will be converted into
                          ; two ASCII characters.
                          ;     The upper nibble will be represented by an ASCII character returned in register s2.
                          ;     The lower nibble will be represented by an ASCII character returned in register s1.
                          ;
                          ; The ASCII representations of '0' to '9' are 30 to 39 hexadecimal which is simply 30 hex
                          ; added to the actual decimal value. The ASCII representations of 'A' to 'F' are 41 to 46
                          ; hexadecimal requiring a further addition of 07 to the 30 already added.
                          ;
                          ; Registers used s0, s1 and s2.
                          ;
       hex_byte_to_ASCII: LOAD s1, s0                            ;remember value supplied
                          SR0 s0                                 ;isolate upper nibble
                          SR0 s0
                          SR0 s0
                          SR0 s0
                          CALL hex_to_ASCII                      ;convert
                          LOAD s2, s0                            ;upper nibble value in s2
                          LOAD s0, s1                            ;restore complete value
                          AND s0, 0F                             ;isolate lower nibble
                          CALL hex_to_ASCII                      ;convert
                          LOAD s1, s0                            ;lower nibble value in s1
                          RETURN
                          ;
                          ; Convert hexadecimal value provided in register s0 into ASCII character
                          ;
                          ;Register used s0
                          ;
            hex_to_ASCII: SUB s0, 0A                             ;test if value is in range 0 to 9
                          JUMP C, number_char
                          ADD s0, 07                             ;ASCII char A to F in range 41 to 46
             number_char: ADD s0, 3A                             ;ASCII char 0 to 9 in range 30 to 40
                          RETURN
                          ;
                          ;
                          ; Send the two character HEX value of the register contents 's0' to the UART
                          ;
                          ; Registers used s0, s1, s2
                          ;
           send_hex_byte: CALL hex_byte_to_ASCII
                          LOAD UART_data, s2
                          CALL send_to_UART
                          LOAD UART_data, s1
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ;
                          ; Send the six character HEX value of the register contents [s9,s8,s7] to the UART
                          ;
                          ; Registers used s0, s1, s2
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
                          ; Read one character from UART and echo.
                          ; Convert to upper case and return.
                          ;
                          ;
         read_upper_case: CALL read_from_UART                    ;read command character from UART
                          CALL send_to_UART                      ;echo character
                          LOAD s0, UART_data                     ;convert to upper case
                          CALL upper_case
                          RETURN
                          ;
                          ;
                          ; Read two hex characters from UART and convert to single byte data
                          ;
            obtain_8bits: CALL read_upper_case                   ;obtain one byte from UART
                          LOAD s3, s0
                          CALL read_upper_case
                          LOAD s2, s0
                          CALL ASCII_byte_to_hex
                          RETURN
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
                          ; Send a minus sign to the UART
                          ;
              send_minus: LOAD UART_data, character_minus
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'PicoBlaze DS2432 Communicator v1.00' string to the UART
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
                          LOAD UART_data, character_D
                          CALL send_to_UART
                          LOAD UART_data, character_S
                          CALL send_to_UART
                          LOAD UART_data, character_2
                          CALL send_to_UART
                          LOAD UART_data, character_4
                          CALL send_to_UART
                          LOAD UART_data, character_3
                          CALL send_to_UART
                          LOAD UART_data, character_2
                          CALL send_to_UART
                          CALL send_space
                          LOAD UART_data, character_C
                          CALL send_to_UART
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_m
                          CALL send_to_UART
                          CALL send_to_UART
                          LOAD UART_data, character_u
                          CALL send_to_UART
                          LOAD UART_data, character_n
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_c
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_r
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
                          ; Send reset menu to the UART
                          ;
         send_reset_menu: CALL send_CR
                          CALL send_CR
                          CALL send_HHelp
                          LOAD UART_data, character_1
                          CALL send_to_UART
                          CALL send_minus
                          CALL send_Master_Reset
                          CALL send_CR
                          RETURN
                          ;
                          ;
                          ;
                          ;
                          ;
                          ;
                          ; Send ROM menu to the UART
                          ;
           send_ROM_menu: CALL send_CR
                          CALL send_CR
                          CALL send_HHelp
                          LOAD UART_data, character_1
                          CALL send_to_UART
                          CALL send_minus
                          CALL send_Master_Reset
                          CALL send_CR
                          LOAD UART_data, character_2
                          CALL send_to_UART
                          CALL send_minus
                          CALL send_Read
                          CALL send_space
                          CALL send_ROM
                          CALL send_space
                          CALL send_Command
                          CALL send_CR
                          LOAD UART_data, character_3
                          CALL send_to_UART
                          CALL send_minus
                          CALL send_Skip
                          CALL send_space
                          CALL send_ROM
                          CALL send_space
                          CALL send_Command
                          CALL send_CR
                          RETURN
                          ;
                          ;
                          ;
                          ; Send DS2432 menu to the UART
                          ;
        send_DS2432_menu: CALL send_CR
                          CALL send_CR
                          CALL send_HHelp
                          LOAD UART_data, character_1
                          CALL send_to_UART
                          CALL send_minus
                          CALL send_Master_Reset
                          CALL send_CR
                          LOAD UART_data, character_2
                          CALL send_to_UART
                          CALL send_minus
                          CALL send_Read
                          CALL send_space
                          CALL send_Memory
                          CALL send_space
                          CALL send_Command
                          CALL send_CR
                          LOAD UART_data, character_3
                          CALL send_to_UART
                          CALL send_minus
                          CALL send_Write
                          CALL send_space
                          CALL send_Scratchpad
                          CALL send_space
                          CALL send_Memory
                          CALL send_space
                          CALL send_Command
                          CALL send_CR
                          LOAD UART_data, character_4
                          CALL send_to_UART
                          CALL send_minus
                          CALL send_Read
                          CALL send_space
                          CALL send_Scratchpad
                          CALL send_space
                          CALL send_Memory
                          CALL send_space
                          CALL send_Command
                          CALL send_CR
                          LOAD UART_data, character_5
                          CALL send_to_UART
                          CALL send_minus
                          CALL send_Write
                          CALL send_space
                          CALL send_Byte
                          CALL send_CR
                          LOAD UART_data, character_6
                          CALL send_to_UART
                          CALL send_minus
                          CALL send_Read
                          CALL send_space
                          CALL send_Byte
                          CALL send_CR
                          RETURN
                          ;
                          ;
                          ;
                          ; Send carriage return, 'OK' and carriage return to the UART
                          ;
                 send_OK: CALL send_CR
                          LOAD UART_data, character_O
                          CALL send_to_UART
                          LOAD UART_data, character_K
                          CALL send_to_UART
                          CALL send_CR
                          RETURN
                          ;
                          ;
                          ; Send 'H-Help'menu to the UART
                          ;
              send_HHelp: CALL send_CR
                          LOAD UART_data, character_H
                          CALL send_to_UART
                          CALL send_minus
                          LOAD UART_data, character_H
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_l
                          CALL send_to_UART
                          LOAD UART_data, character_p
                          CALL send_to_UART
                          CALL send_CR
                          RETURN
                          ;
                          ;
                          ; Send 'ROM' to the UART
                          ;
                send_ROM: LOAD UART_data, character_R
                          CALL send_to_UART
                          LOAD UART_data, character_O
                          CALL send_to_UART
                          LOAD UART_data, character_M
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'Memory' to the UART
                          ;
             send_Memory: LOAD UART_data, character_M
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_m
                          CALL send_to_UART
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          LOAD UART_data, character_y
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'Scratchpad' to the UART
                          ;
         send_Scratchpad: LOAD UART_data, character_S
                          CALL send_to_UART
                          LOAD UART_data, character_c
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_c
                          CALL send_to_UART
                          LOAD UART_data, character_h
                          CALL send_to_UART
                          LOAD UART_data, character_p
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_d
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'Master Reset' to the UART
                          ;
       send_Master_Reset: LOAD UART_data, character_M
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_s
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          CALL send_space
                          LOAD UART_data, character_R
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_s
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'Byte' to the UART
                          ;
               send_Byte: LOAD UART_data, character_B
                          CALL send_to_UART
                          LOAD UART_data, character_y
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'Skip' to the UART
                          ;
               send_Skip: LOAD UART_data, character_S
                          CALL send_to_UART
                          LOAD UART_data, character_k
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_p
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'Read' to the UART
                          ;
               send_Read: LOAD UART_data, character_R
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_d
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'Write' to the UART
                          ;
              send_Write: LOAD UART_data, character_W
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'Pass' to the UART
                          ;
               send_Pass: LOAD UART_data, character_P
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_s
                          CALL send_to_UART
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'Fail' to the UART
                          ;
               send_Fail: LOAD UART_data, character_F
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_i
                          CALL send_to_UART
                          LOAD UART_data, character_l
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'Command' to the UART
                          ;
            send_Command: LOAD UART_data, character_C
                          CALL send_to_UART
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_m
                          CALL send_to_UART
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_n
                          CALL send_to_UART
                          LOAD UART_data, character_d
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'address=' to the UART
                          ;
            send_address: CALL send_CR
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_d
                          CALL send_to_UART
                          CALL send_to_UART
                          LOAD UART_data, character_r
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          LOAD UART_data, character_s
                          CALL send_to_UART
                          CALL send_to_UART
             send_equals: LOAD UART_data, character_equals
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'data' to the UART
                          ;
               send_data: CALL send_CR
                          LOAD UART_data, character_d
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          LOAD UART_data, character_t
                          CALL send_to_UART
                          LOAD UART_data, character_a
                          CALL send_to_UART
                          RETURN
                          ;
                          ;
                          ; Send 'E/S=' to the UART
                          ;
                 send_ES: CALL send_CR
                          LOAD UART_data, character_E
                          CALL send_to_UART
                          LOAD UART_data, character_divide
                          CALL send_to_UART
                          LOAD UART_data, character_S
                          CALL send_to_UART
                          JUMP send_equals
                          ;
                          ;
                          ; Send 'code=' to the UART
                          ;
               send_code: LOAD UART_data, character_c
                          CALL send_to_UART
                          LOAD UART_data, character_o
                          CALL send_to_UART
                          LOAD UART_data, character_d
                          CALL send_to_UART
                          LOAD UART_data, character_e
                          CALL send_to_UART
                          JUMP send_equals
                          ;
                          ;
                          ; Send 'serial=' to the UART
                          ;
             send_serial: LOAD UART_data, character_s
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
                          JUMP send_equals
                          ;
                          ;
                          ; Send 'CRC=' to the UART
                          ;
                send_CRC: LOAD UART_data, character_C
                          CALL send_to_UART
                          LOAD UART_data, character_R
                          CALL send_to_UART
                          LOAD UART_data, character_C
                          JUMP send_equals
                          ;
                          ;
                          ;**************************************************************************************
                          ; Interrupt Service Routine (ISR)
                          ;**************************************************************************************
                          ;
                          ; Interrupts are not used in this design. This is a place keeper only.
                          ;
                          ADDRESS 3FE
                     ISR: RETURNI ENABLE
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
