                        ;KCPSM3 Program - UART programming of SPI Flash memory on Spartan-3E Starter Kit.
                        ;
                                            device kcpsm6
                    ; warning > added only for testing purposes
                        ;
                        ;Ken Chapman - Xilinx Ltd
                        ;
                        ;Version v1.00 - 11th November 2005
                        ;
                        ;This program uses a 115200 baud UART connection with XON/XOFF flow control
                        ;to allow a standard MCS file for the configuration of a Spartan-3E device to
                        ;be programmed into the ST Microelectronics M25P16 device on the board.
                        ;
                        ;
                        ;
                        ;As well as the port connections vital to communication with the UART and the SPI
                        ;FLASH memory, there are additional port connections used to disable the other
                        ;devices sharing the SPI bus on the Starter Kit board. Although these could have been
                        ;controlled at the hardware level, they are included in this code to aid
                        ;future investigations of communication with the other SPI devices using PicoBlaze.
                        ;
                        ;
                        ;
                        ;
                        ;Port definitions
                        ;
                        CONSTANT status_port, 00               ;UART and filter status input
                        CONSTANT tx_data_present, 01           ;  Transmitter  data present - bit0
                        CONSTANT tx_half_full, 02              ;    FIFO          half full - bit1
                        CONSTANT tx_full, 04                   ;                       full - bit2
                        CONSTANT rx_data_present, 08           ;               data present - bit3
                        CONSTANT rx_half_full, 10              ;  Receiver        half full - bit4
                        CONSTANT rx_full, 20                   ;    FIFO               full - bit5
                        ;
                        CONSTANT UART_read_port, 01            ;UART Rx data input
                        ;
                        CONSTANT UART_write_port, 10           ;UART Tx data output
                        ;
                        ;
                        CONSTANT SPI_control_port, 08          ;SPI clock and chip selects
                        CONSTANT SPI_sck, 01                   ;                  SCK - bit0
                        CONSTANT SPI_rom_cs, 02                ;    serial rom select - bit1
                        CONSTANT SPI_spare_control, 04         ;                spare - bit2
                        CONSTANT SPI_amp_cs, 08                ;     amplifier select - bit3
                        CONSTANT SPI_adc_conv, 10              ;          A/D convert - bit4
                        CONSTANT SPI_dac_cs, 20                ;           D/A select - bit5
                        CONSTANT SPI_amp_shdn, 40              ;       amplifier SHDN - bit6
                        CONSTANT SPI_dac_clr, 80               ;            D/A clear - bit7
                        ;
                        CONSTANT SPI_output_port, 04           ;SPI data output
                        CONSTANT SPI_sdo, 80                   ;   SDO - bit7
                        ;
                        CONSTANT SPI_input_port, 02            ;SPI data input
                        CONSTANT SPI_sdi, 80                   ;             SDI - bit7
                        CONSTANT SPI_amp_sdi, 40               ;   amplifier SDI - bit6
                        ;
                        ;
                        ;
                        ;
                        ;Special Register usage
                        ;
                        NAMEREG sF, UART_data                  ;used to pass data to and from the UART
                        ;
                        ;
                        ;Useful data constants
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
                        ;Useful constants
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
                        ;Scratch Pad Memory Locations
                        ;
                        ;
                        CONSTANT ISR_preserve_s0, 00           ;preserve register during ISR
                        ;
                        CONSTANT page_address_h, 01            ;Remember page address for SPI boundary checking.
                        CONSTANT page_address_m, 02            ;high, middle and low bytes
                        CONSTANT page_address_l, 03
                        ;
                        CONSTANT SPI_control_status, 04        ;SPI status signals
                        ;
                        ;
                        ;
                        ;Store up to one line of an MCS file as bytes
                        ;A typical data line consists of:-
                        ;:     Start character which is not stored
                        ;10    Number of data bytes included (16 in this case)
                        ;aaaa  Lower 16-bits of the storage address
                        ;00    Record type (data in this case)
                        ;dddd...   Data bytes (typically 16 which is the maximum)
                        ;cc    Checksum
                        ;CR/LF Line will end in carriage return and/or line feed which is not stored.
                        ;
                        ;So a total of 21 could be stored before processing.
                        ;
                        CONSTANT line_start, 2B                ;21 bytes until end of memory
                        ;
                        ;
                        ;Initialise the system and welcome message
                        ;
                        ;
            cold_start: CALL SPI_init                          ;initialise SPI bus ports
                        CALL delay_1s                          ;delay because UART is fast and JTAG startup sequence can be slow
                        ENABLE INTERRUPT                       ;Interrupt is used for XON/XOFF flow control
         welcome_start: CALL send_CR
                        CALL send_welcome                      ;start up message and version number
                        ;
                        ;Main menu and command selection
                        ;
                        ;
                        ;
            warm_start: CALL send_Menu                         ;Menu and command selection
                prompt: CALL send_CR
                        CALL send_CR
                        LOAD UART_data, character_greater_than ;prompt for input
                        CALL send_to_UART
                        CALL read_upper_case
                        COMPARE s0, character_E                ;command test
                        JUMP Z, erase_command
                        COMPARE s0, character_S                ;command test
                        JUMP Z, sector_erase_command
                        COMPARE s0, character_P                ;command test
                        JUMP Z, program_command
                        COMPARE s0, character_R                ;command test
                        JUMP Z, read_command
                        COMPARE s0, character_I                ;command test
                        JUMP Z, ID_command
                        COMPARE s0, character_H                ;command test
                        JUMP Z, welcome_start
                        CALL send_CR                           ;no valid command input
                        LOAD UART_data, character_question     ;display ???
                        CALL send_to_UART
                        CALL send_to_UART
                        CALL send_to_UART
                        JUMP prompt                            ;Try again!
                        ;
                        ;
       read_upper_case: CALL read_from_UART                    ;read command character from UART
                        CALL send_to_UART                      ;echo character
                        LOAD s0, UART_data                     ;convert to upper case
                        CALL upper_case
                        RETURN
                        ;
                        ;**************************************************************************************
                        ;ID Command - Read and display the ID for the SPI FLASH memory
                        ;**************************************************************************************
                        ;
                        ;Normal response should be
                        ;   s9 = Manufacturer Identification = 20 hex
                        ;   s8 = Memory Type = 20 hex
                        ;   s7 = Memory Capacity = 15 hex
                        ;
            ID_command: CALL send_CR
                        CALL read_spi_flash_ID
                        CALL send_ID
                        LOAD UART_data, character_equals
                        CALL send_to_UART
                        CALL send_space
                        LOAD s0, s9
                        CALL send_hex_byte
                        CALL send_space
                        LOAD s0, s8
                        CALL send_hex_byte
                        CALL send_space
                        LOAD s0, s7
                        CALL send_hex_byte
                        CALL send_space
                        CALL send_CR
                        JUMP prompt
                        ;
                        ;**************************************************************************************
                        ;Erase Command - Perform bulk erase of the SPI FLASH memory and display messages
                        ;**************************************************************************************
                        ;
         erase_command: CALL send_CR
                        CALL send_Confirm                      ;confirm command with a 'Y' which must be upper case
                        CALL read_from_UART                    ;read command character from UART
                        CALL send_to_UART                      ;echo input
                        COMPARE UART_data, character_Y
                        JUMP NZ, abort_erase
                        CALL send_CR
                        CALL send_Erase_in_progress
                        CALL bulk_erase_spi
                        CALL send_OK
                        JUMP prompt
                        ;
           abort_erase: CALL send_Abort
                        JUMP prompt
                        ;
                        ;
                        ;**************************************************************************************
                        ;Sector Erase Command - Performs erase of lowest 5 sectors SPI FLASH memory which
                        ;covers the address range 000000 to 04FFFF in which the configuration for an XC3S500E
                        ;would be able to fit.
                        ;**************************************************************************************
                        ;
  sector_erase_command: CALL send_CR
                        CALL send_Confirm                      ;confirm command with a 'Y' which must be upper case
                        CALL read_from_UART                    ;read command character from UART
                        CALL send_to_UART                      ;echo input
                        COMPARE UART_data, character_Y
                        JUMP NZ, abort_erase
                        CALL send_CR
                        CALL send_Erase_in_progress
                        LOAD s9, 00                            ;any address inside sector 0
                        CALL erase_spi_sector
                        LOAD s9, 01                            ;any address inside sector 1
                        CALL erase_spi_sector
                        LOAD s9, 02                            ;any address inside sector 2
                        CALL erase_spi_sector
                        LOAD s9, 03                            ;any address inside sector 3
                        CALL erase_spi_sector
                        LOAD s9, 04                            ;any address inside sector 4
                        CALL erase_spi_sector
                        CALL send_OK
                        JUMP prompt
                        ;
                        ;
                        ;**************************************************************************************
                        ;Program Command - Program SPI FLASH memory with MCS file
                        ;**************************************************************************************
                        ;
       program_command: CALL send_CR
                        CALL send_Waiting_MCS_file
                        CALL program_MCS
                        CALL send_OK
                        JUMP prompt
                        ;
                        ;
                        ;**************************************************************************************
                        ;Read Command - Read one page of memory at specified address
                        ;**************************************************************************************
                        ;
          read_command: CALL send_page_address                 ;obtain 24-bit address
                        CALL obtain_8bits
                        JUMP C, read_command                   ;bad input address
                        COMPARE s0, 20                         ;test for address greater than 1FFFFF
                        JUMP NC, read_command                  ;value too big
                        LOAD s9, s0
                        CALL obtain_8bits
                        JUMP C, read_command                   ;bad input address
                        LOAD s8, s0
                        CALL obtain_8bits
                        JUMP C, read_command                   ;bad input address
                        LOAD s7, s0
                        CALL send_CR
                        CALL send_spi_page
                        CALL send_OK
                        JUMP prompt
          obtain_8bits: CALL read_upper_case                   ;obtain one byte from UART
                        LOAD s3, s0
                        CALL read_upper_case
                        LOAD s2, s0
                        CALL ASCII_byte_to_hex
                        RETURN
                        ;
                        ;
                        ;
                        ;**************************************************************************************
                        ;Program SPI FLASH with MCS file
                        ;**************************************************************************************
                        ;
                        ;Reads the MCS file from the UART and programs the SPI FLASH device at the locations.
                        ;specified by the file contents.
                        ;
                        ;One important factor of programming the SPI FLASH for use as configuration
                        ;memory is that the bits within each byte must be in reverse order. This
                        ;is because an SPI device outputs data MSB first compared with a Xilinx
                        ;serial PROM which outputs LSB first. Therefore this routine will swap
                        ;the bits of each byte provided by the MCS file before programming.
                        ;
                        ;This routine will continue until an end of file record is detected.
                        ;For each line of MCS received, the current address will be output so that
                        ;progress can be monitored.
                        ;
                        ;Register sA is used to remember if a page is currently open (01) or closed (00)
                        ;for writing on the SPI memory.
                        ;
           program_MCS: LOAD sA, 00                            ;page is closed
        next_prog_line: CALL read_MCS_line                     ;read line
                        CALL MCS_address                       ;find start address and record type
                        COMPARE sB, 01                         ;test for end record
                        JUMP Z, end_program_MCS
                        CALL send_hex_3bytes                   ;send address for other lines
                        CALL send_CR
                        COMPARE sB, 04                         ;test for extended address record
                        JUMP Z, MCS_address_boundary
                        ;
                        ;Assume data record type 00 now and program SPI page
                        ;
                        SUB sE, 01                             ;location of checksum just after last stored data byte
                        LOAD sD, line_start                    ;Point to first data byte
                        ADD sD, 04
                        COMPARE sA, 00                         ;check if page is closed
                        JUMP Z, program_byte                   ;jump if page needs to be opened
                        FETCH s2, page_address_h               ;check new address is sequential
                        COMPARE s2, s9
                        JUMP NZ, addr_out_of_sequence
                        FETCH s2, page_address_m               ;check new address is sequential
                        COMPARE s2, s8
                        JUMP NZ, addr_out_of_sequence
                        FETCH s2, page_address_l               ;check new address is sequential
                        COMPARE s2, s7
                        JUMP Z, program_byte                   ;continue with open page
  addr_out_of_sequence: CALL close_prog_page_spi               ;close page because address out of sequence
                        LOAD sA, 00                            ;page is now closed
          program_byte: COMPARE sA, 00                         ;check if page is closed
                        JUMP NZ, page_is_open                  ;jump is page already open
                        CALL open_prog_page_spi                ;open page with address [s9,s8,s7]
                        LOAD sA, 01                            ;page is open
          page_is_open: FETCH s1, (sD)                         ;fetch data byte
                        LOAD s0, 08                            ;reverse order of bits
             swap_bits: SR0 s1
                        SLA s2
                        SUB s0, 01
                        JUMP NZ, swap_bits                     ;swapped bits now in s2
                        CALL SPI_FLASH_tx_rx                   ;program byte into SPI memory
                        ADD s7, 01                             ;increment address to keep track
                        ADDCY s8, 00
                        ADDCY s9, 00
                        COMPARE s7, 00                         ;test if crossing page boundary FF to 00
                        JUMP NZ, byte_programmed
                        CALL close_prog_page_spi
                        LOAD sA, 00                            ;page is now closed
       byte_programmed: ADD sD, 01                             ;move to next byte
                        COMPARE sD, sE                         ;check for last on line
                        JUMP NZ, program_byte                  ;fetch next byte to program
                        STORE s9, page_address_h               ;remember next address in sequence
                        STORE s8, page_address_m
                        STORE s7, page_address_l
                        JUMP next_prog_line                    ;read next line for programming
  MCS_address_boundary: COMPARE sA, 00                         ;check if page needs to be closed
                        JUMP Z, next_prog_line
                        CALL close_prog_page_spi
                        JUMP program_MCS
       end_program_MCS: COMPARE sA, 00                         ;check if page needs to be closed
                        RETURN Z
                        CALL close_prog_page_spi
                        RETURN
                        ;
                        ;**************************************************************************************
                        ;Read one line of an MCS file into scratch pad memory
                        ;**************************************************************************************
                        ;
                        ;Reads one line of MCS file format into scratch pad memory starting at location 'line_start'.
                        ;
                        ;The routine detects the line start character ':' ignoring any preceding characters. This
                        ;will remove any additional CR or LF characters.
                        ;
                        ;It then reads each subsequent pair of ASCII characters, converts them to true hex in the
                        ;range 00 to FF and stores them in scratch pad memory.
                        ;
                        ;The end of the line is determined by either a CR or LF character.
                        ;
                        ;The value last returned in register 'sE' will be the pointer to the location in
                        ;scratch pad memory following the last byte for the line read.
                        ;
         read_MCS_line: LOAD sE, line_start                    ;initialise SPM memory pointer
   wait_MCS_line_Start: CALL read_from_UART                    ;read character
                        COMPARE UART_data, character_colon     ;test for start character
                        JUMP NZ, wait_MCS_line_Start
         read_MCS_byte: CALL read_from_UART                    ;read character
                        COMPARE UART_data, character_CR        ;test for end of line
                        RETURN Z
                        COMPARE UART_data, character_LF        ;test for end of line
                        RETURN Z
                        LOAD s3, UART_data                     ;upper nibble character
                        CALL read_from_UART                    ;read character
                        LOAD s2, UART_data                     ;lower nibble character
                        CALL ASCII_byte_to_hex                 ;convert to true hex value
                        STORE s0, (sE)                         ;write to SPM
                        ADD sE, 01                             ;increment pointer
                        JUMP read_MCS_byte
                        ;
                        ;
                        ;**************************************************************************************
                        ;Determine the current address for the line of an MCS file in scratch pad memory
                        ;**************************************************************************************
                        ;
                        ;Checks the existing line data stored in scratch pad memory starting at location
                        ;'line_start' and determines the current address.
                        ;
                        ;The address is in the register set [s9,s8,s7] before and after this routine is
                        ;executed because not all address bits are defined by a given line of MCS and
                        ;the undefined bits remain constant.
                        ;
                        ;A record type of 04 will update [s9].
                        ;A record type of 00 will update [s8,s7].
                        ;
                        ;On return, the register sB will contain the record type and
                        ;register sC will indicate the number of data bytes stored.
                        ;
           MCS_address: LOAD sD, line_start                    ;initialise SPM memory pointer
                        FETCH sC, (sD)                         ;read number of bytes on line
                        ADD sD, 03                             ;move to record type
                        FETCH sB, (sD)                         ;read record type
                        COMPARE sB, 00                         ;test for data record
                        JUMP Z, new_low_address
                        COMPARE sB, 04                         ;test for data record
                        RETURN NZ
                        ADD sD, 02                             ;read upper 8-bits
                        FETCH s9, (sD)
                        RETURN
       new_low_address: SUB sD, 01                             ;read lower 8-bits
                        FETCH s7, (sD)
                        SUB sD, 01                             ;read middle 8-bits
                        FETCH s8, (sD)
                        RETURN
                        ;
                        ;**************************************************************************************
                        ;Read a page from SPI FLASH memory and display
                        ;**************************************************************************************
                        ;
                        ;The start address should be provided in register set [s9,s8,s7].
                        ;The display will be next 256 bytes displayed as 16 lines of 16 bytes
                        ;with each line commencing with the address of the first byte.
                        ;
                        ;
         send_spi_page: LOAD s6, 10                            ;16 lines to display
         send_spi_line: CALL send_CR
                        CALL send_hex_3bytes                   ;display address
                        CALL send_space
                        LOAD s5, 10                            ;16 lines to display
         send_spi_byte: CALL send_space
                        CALL read_spi_byte                     ;read byte into s2
                        ADD s7, 01                             ;increment SPI FLASH address
                        ADDCY s8, 00
                        ADDCY s9, 00
                        LOAD s0, s2                            ;display byte
                        CALL send_hex_byte
                        SUB s5, 01                             ;count bytes per line
                        JUMP NZ, send_spi_byte
                        SUB s6, 01                             ;count lines
                        JUMP NZ, send_spi_line
                        CALL send_CR
                        RETURN
                        ;
                        ;**************************************************************************************
                        ;Test of SPI FLASH memory operations
                        ;**************************************************************************************
                        ;
                        ;Sector 18 (120000 to 12FFFF) is used.
                        ;A page (123400 to 1234FF) is programmed with a test pattern 00 to FF.
                        ;The pattern is verified and then the sector erased and a blank check performed.
                        ;
                        ;Note that the page used is already blank (all locations contain FF hex)
                        ;as with any device supplied (initial delivery state).
                        ;
                        ;Program page with test pattern
                        ;
           memory_test: LOAD s9, 12                            ;select page address 123400
                        LOAD s8, 34
                        LOAD s7, 00
                        CALL open_prog_page_spi                ; program test pattern 00 to FF
          pattern_loop: LOAD s2, s7
                        CALL SPI_FLASH_tx_rx
                        ADD s7, 01
                        JUMP NC, pattern_loop
                        CALL close_prog_page_spi               ; program test pattern 00 to FF
                        ;
                        ;Verify test pattern by reading back page
                        ;
                        LOAD UART_data, character_p            ;p for pass
                        LOAD s9, 12                            ;select page address 123400
                        LOAD s8, 34
                        LOAD s7, 00
      verify_test_page: CALL read_spi_byte                     ;read byte into s2
                        COMPARE s2, s7                         ;check test pattern data value
                        JUMP NZ, memory_verify_fail
                        ADD s7, 01                             ;next location
                        JUMP NZ, verify_test_page              ;loop until roll over page
                        JUMP memory_verify_result
    memory_verify_fail: LOAD UART_data, character_f            ;f for fail
  memory_verify_result: CALL send_to_UART
                        RETURN
                        ;
                        ;Erase sector with test pattern and verify blank
                        ;
     erase_test_sector: LOAD s9, 12                            ;sector 18 start address 120000
                        LOAD s8, 00
                        LOAD s7, 00
                        CALL erase_spi_sector
                        LOAD UART_data, character_p            ;p for pass
                        LOAD s9, 12                            ;select page address 123400
                        LOAD s8, 34
                        LOAD s7, 00
     verify_blank_page: CALL read_spi_byte                     ;read byte into s2
                        COMPARE s2, FF                         ;check blank 'FF'
                        JUMP NZ, memory_blank_fail
                        ADD s7, 01                             ;next location
                        JUMP NZ, verify_blank_page             ;loop until roll over page
                        JUMP memory_blank_result
     memory_blank_fail: LOAD UART_data, character_f            ;f for fail
   memory_blank_result: CALL send_to_UART
                        RETURN
                        ;
                        ;
                        ;
                        ;**************************************************************************************
                        ;Test of SPI FLASH memory communications
                        ;**************************************************************************************
                        ;Link must be installed in J11 to link ROM-CS to CSO_B
                        ;
                        ;Read the identification ID from SPI FLASH memory (ST type M25P16)
                        ;and compare with expected response.
                        ;   s9 = Manufacturer Identification = 20 hex
                        ;   s8 = Memory Type = 20 hex
                        ;   s7 = Memory Capacity = 15 hex
                        ;
     memory_comms_test: CALL read_spi_flash_ID
                        LOAD UART_data, character_p            ;p for pass
                        COMPARE s9, 20
                        JUMP NZ, SPI_FLASH_ID_fail
                        COMPARE s8, 20
                        JUMP NZ, SPI_FLASH_ID_fail
                        COMPARE s7, 15
                        JUMP NZ, SPI_FLASH_ID_fail
                        JUMP SPI_FLASH_ID_result
     SPI_FLASH_ID_fail: LOAD UART_data, character_f            ;f for fail
   SPI_FLASH_ID_result: CALL send_to_UART
                        RETURN
                        ;
                        ;
                        ;
                        ;
                        ;
                        ;
                        ;**************************************************************************************
                        ;SPI FLASH memory routines
                        ;**************************************************************************************
                        ;
                        ;These routines will work with two output ports and one input port which should be
                        ;defined as follows using CONSTANT directives.
                        ;   (replace 'pp' with appropriate port address in each case)
                        ;In the list of CONSTANT directives, only the ones marked with a * are really required
                        ;in an SPI FLASH memory system. The other directives are to control (disable) or
                        ;communicate with the other SPI components on the same SPI bus of the Spartan-3E Starter Kit.
                        ;
                        ;
                        ;
                        ;CONSTANT SPI_control_port, pp       ;SPI clock and chip selects     *
                        ;CONSTANT SPI_sck, 01                ;                  SCK - bit0   *
                        ;CONSTANT SPI_rom_cs, 02             ;    serial rom select - bit1   *
                        ;CONSTANT SPI_spare_control, 04      ;                spare - bit2
                        ;CONSTANT SPI_amp_cs, 08             ;     amplifier select - bit3
                        ;CONSTANT SPI_adc_conv, 10           ;          A/D convert - bit4
                        ;CONSTANT SPI_dac_cs, 20             ;           D/A select - bit5
                        ;CONSTANT SPI_amp_shdn, 40           ;       amplifier SHDN - bit6
                        ;CONSTANT SPI_dac_clr, 80            ;            D/A clear - bit7
                        ;
                        ;CONSTANT SPI_output_port, pp        ;SPI data output                *
                        ;CONSTANT SPI_sdo, 80                ;   SDO - bit7                  *
                        ;
                        ;CONSTANT SPI_input_port, pp         ;SPI data input                 *
                        ;CONSTANT SPI_sdi, 80                ;             SDI - bit7        *
                        ;CONSTANT SPI_amp_sdi, 40            ;   amplifier SDI - bit6
                        ;
                        ;
                        ;A single scratch pad memory location is also employed to remember the status of
                        ;the SPI_control_port. This memory location must be defined as follows.
                        ;   (replace 'ss' with appropriate memory location)
                        ;
                        ;CONSTANT SPI_control_status, ss       ;SPI status signals
                        ;
                        ;Not all the SPI routines will use this memory location because although they
                        ;will change the bits on the control port, they will leave them in the same state
                        ;as they were in when they started.
                        ;
                        ;
                        ;
                        ;
                        ;Initialise SPI bus
                        ;
                        ;This routine should be used to initialise the SPI bus.
                        ;The SCK clock is made low.
                        ;Device selections are made inactive as follows
                        ;   SPI_sck      = 0      Clock is Low (required)
                        ;   SPI_rom_cs   = 1      Deselect ROM
                        ;   spare        = 1      spare control bit
                        ;   SPI_amp_cs   = 1      Deselect amplifier
                        ;   SPI_adc_conv = 0      A/D convert ready to apply positive pulse
                        ;   SPI_dac_cs   = 1      Deselect D/A
                        ;   SPI_amp_shdn = 0      Amplifier active and available
                        ;   SPI_dac_clr  = 1      D/A clear off
                        ;
              SPI_init: LOAD s0, AE                            ;normally AE
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        RETURN
                        ;
                        ;
                        ;Send and receive one byte to or from the SPI FLASH memory.
                        ;
                        ;The data supplied in register 's2' is transmitted to the SPI bus and
                        ;at the same time any received byte is used to replace the value in 's2'.
                        ;The SCK clock is generated by software and results in a communication rate of
                        ;2.5Mbit/s with a 50MHz clock.
                        ;
                        ;Note that you must have previously selected the required device on the bus
                        ;before attempting communication and you must subsequently deselect the device
                        ;when appropriate.
                        ;
                        ;Entry to this routine assumes that SCK is already Low and the clock will be Low
                        ;at the end of execution (provided in scratch pad memory location SPI_control_status).
                        ;
                        ;As a 'master' the signal sequence is as follows..
                        ;   Receive data bit from SDI line (Flash transmits on previous falling edge)
                        ;   Transmit data bit on SDO line
                        ;   Drive SCK transition from low to high
                        ;   Drive SCK transition from high to low.
                        ;
       SPI_FLASH_tx_rx: LOAD s1, 08                            ;8-bits to transmit and receive
                        FETCH s0, SPI_control_status           ;read control status bits
    next_SPI_FLASH_bit: OUTPUT s2, SPI_output_port             ;output data bit ready to be used on rising edge
                        INPUT s3, SPI_input_port               ;read input bit
                        TEST s3, SPI_sdi                       ;detect state of received bit
                        SLA s2                                 ;shift new data into result and move to next transmit bit
                        XOR s0, SPI_sck                        ;clock High (bit0)
                        OUTPUT s0, SPI_control_port            ;drive clock High
                        XOR s0, SPI_sck                        ;clock Low (bit0)
                        OUTPUT s0, SPI_control_port            ;drive clock Low
                        SUB s1, 01                             ;count bits
                        JUMP NZ, next_SPI_FLASH_bit            ;repeat until finished
                        RETURN
                        ;
                        ;
                        ;Read status register from SPI FLASH memory (ST type M25P16)
                        ;
                        ;Transmits instruction 05hex and then receives one byte in response
                        ;which is returned in register s2.
                        ;
                        ; bit    meaning
                        ;  7    SRWD Status Register Write Protect
                        ;  6    '0'
                        ;  5    '0'
                        ;  4    BP2 Block protect bit
                        ;  3    BP1 Block protect bit
                        ;  2    BP0 Block protect bit
                        ;  1    WEL Write Enable Latch Bit
                        ;  0    WIP Write In Progress
                        ;
                        ;
 read_spi_flash_status: CALL SPI_init                          ;ensure known state of bus and s0 register
                        XOR s0, SPI_rom_cs                     ;select (Low) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        LOAD s2, 05                            ;Read Status register instruction
                        CALL SPI_FLASH_tx_rx                   ;transmit instruction
                        CALL SPI_FLASH_tx_rx                   ;Receive status register information
                        XOR s0, SPI_rom_cs                     ;deselect (High) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        RETURN
                        ;
                        ;Set write enable mode in SPI FLASH memory (ST type M25P16)
                        ;
                        ;Transmits instruction 06hex.
                        ;
    set_spi_flash_WREN: CALL SPI_init                          ;ensure known state of bus and s0 register
                        XOR s0, SPI_rom_cs                     ;select (Low) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        LOAD s2, 06                            ;Set write enable mode instruction
                        CALL SPI_FLASH_tx_rx                   ;transmit instruction
                        XOR s0, SPI_rom_cs                     ;deselect (High) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        RETURN
                        ;
                        ;Reset the write enable mode in SPI FLASH memory (ST type M25P16)
                        ;
                        ;Transmits instruction 04hex.
                        ;
  reset_spi_flash_WREN: CALL SPI_init                          ;ensure known state of bus and s0 register
                        XOR s0, SPI_rom_cs                     ;select (Low) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        LOAD s2, 04                            ;Reset write enable mode instruction
                        CALL SPI_FLASH_tx_rx                   ;transmit instruction
                        XOR s0, SPI_rom_cs                     ;deselect (High) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        RETURN
                        ;
                        ;Read the identification ID from SPI FLASH memory (ST type M25P16)
                        ;
                        ;Transmits instruction 9Fhex and then reads the 3 byte response into [s9,s8,s7]
                        ;
                        ;response should be
                        ;   s9 = Manufacturer Identification = 20 hex
                        ;   s8 = Memory Type = 20 hex
                        ;   s7 = Memory Capacity = 15 hex
                        ;
     read_spi_flash_ID: CALL SPI_init                          ;ensure known state of bus and s0 register
                        XOR s0, SPI_rom_cs                     ;select (Low) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        LOAD s2, 9F                            ;Read ID instruction
                        CALL SPI_FLASH_tx_rx                   ;transmit instruction
                        CALL SPI_FLASH_tx_rx                   ;receive Manufacturer ID
                        LOAD s9, s2
                        CALL SPI_FLASH_tx_rx                   ;receive Memory Type
                        LOAD s8, s2
                        CALL SPI_FLASH_tx_rx                   ;receive Memory Capacity
                        LOAD s7, s2
                        XOR s0, SPI_rom_cs                     ;deselect (High) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        RETURN
                        ;
                        ;Read a single byte from the SPI FLASH memory (ST type M25P16)
                        ;
                        ;Transmits instruction 03hex followed by a 24-bit address which must be supplied in the
                        ;register set [s9,s8,s7]. It then transmits a dummy byte to retrieve the memory data
                        ;which is returned in register s2.
                        ;
         read_spi_byte: CALL SPI_init                          ;ensure known state of bus and s0 register
                        XOR s0, SPI_rom_cs                     ;select (Low) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        LOAD s2, 03                            ;Read Data Bytes instruction
                        CALL SPI_FLASH_tx_rx                   ;transmit instruction
                        LOAD s2, s9                            ;Transmit 24-bit address
                        CALL SPI_FLASH_tx_rx
                        LOAD s2, s8
                        CALL SPI_FLASH_tx_rx
                        LOAD s2, s7
                        CALL SPI_FLASH_tx_rx
                        CALL SPI_FLASH_tx_rx                   ;read data byte
                        XOR s0, SPI_rom_cs                     ;deselect (High) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        RETURN
                        ;
                        ;
                        ;Erase a single sector from the SPI FLASH memory (ST type M25P16)
                        ;
                        ;Sets the WREN instruction and then transmits instruction D8 hex followed by a 24-bit
                        ;address which must be supplied in the register set [s9,s8,s7]. The address must be
                        ;at some location within the sector to be erased. A sector erase can take up to
                        ;3 seconds to complete. The routine therefore reads the FLASH status and tests
                        ;the write in progress (WIP) bit to test for completion
                        ;
      erase_spi_sector: CALL set_spi_flash_WREN                ;set write enable mode
                        CALL SPI_init                          ;ensure known state of bus and s0 register
                        XOR s0, SPI_rom_cs                     ;select (Low) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        LOAD s2, D8                            ;Sector erase mode
                        CALL SPI_FLASH_tx_rx                   ;transmit instruction
                        LOAD s2, s9                            ;Transmit 24-bit address [s9,s8,s7].
                        CALL SPI_FLASH_tx_rx
                        LOAD s2, s8
                        CALL SPI_FLASH_tx_rx
                        LOAD s2, s7
                        CALL SPI_FLASH_tx_rx
                        XOR s0, SPI_rom_cs                     ;deselect (High) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
     sector_erase_wait: CALL read_spi_flash_status             ;test WIP bit until finished
                        TEST s2, 01
                        JUMP NZ, sector_erase_wait
                        RETURN
                        ;
                        ;
                        ;
                        ;Bulk erase the whole SPI FLASH memory (ST type M25P16)
                        ;
                        ;Sets the WREN instruction and then transmits instruction C7 hex.
                        ;A bulk erase can take up to 40 seconds to complete. The routine therefore reads the
                        ;FLASH status and tests the write in progress (WIP) bit to test for completion
                        ;
        bulk_erase_spi: CALL set_spi_flash_WREN                ;set write enable mode
                        CALL SPI_init                          ;ensure known state of bus and s0 register
                        XOR s0, SPI_rom_cs                     ;select (Low) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        LOAD s2, C7                            ;Sector erase mode
                        CALL SPI_FLASH_tx_rx                   ;transmit instruction
                        XOR s0, SPI_rom_cs                     ;deselect (High) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
       bulk_erase_wait: CALL read_spi_flash_status             ;test WIP bit until finished
                        TEST s2, 01
                        JUMP NZ, bulk_erase_wait
                        RETURN
                        ;
                        ;
                        ;
                        ;Open a page for programming.
                        ;The 24-bit start address to be supplied in the register set [s9,s8,s7].
                        ;Note that s7=00 hex for normal page boundaries but you could start at any address.
                        ;Caution : Exceeding s7=FF hex will result in the roll over to 00 hex but without
                        ;incrementing to the next page.
                        ;
                        ;Transmits instruction 02hex followed by the 24-bit start address.
                        ;It is then ready to transmit data bytes using the s2 register and the SPI_FLASH_tx_rx
                        ;subroutine. After transmitting bytes, close the page with the close_prog_page_spi
                        ;routine.
                        ;
    open_prog_page_spi: CALL set_spi_flash_WREN                ;set write enable mode
                        CALL SPI_init                          ;ensure known state of bus and s0 register
                        XOR s0, SPI_rom_cs                     ;select (Low) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
                        LOAD s2, 02                            ;Page program mode
                        CALL SPI_FLASH_tx_rx                   ;transmit instruction
                        LOAD s2, s9                            ;Transmit 24-bit address [s9,s8,s7].
                        CALL SPI_FLASH_tx_rx
                        LOAD s2, s8
                        CALL SPI_FLASH_tx_rx
                        LOAD s2, s7
                        CALL SPI_FLASH_tx_rx
                        RETURN
                        ;
                        ;
                        ;This routine completes a page program operation started with
                        ;open_prog_page_spi and data bytes sent with SPI_FLASH_tx_rx.
                        ;
                        ;A page program can take up to 5ms to complete. The routine therefore reads the
                        ;FLASH status and tests the write in progress (WIP) bit to test for completion
                        ;
                        ;
   close_prog_page_spi: FETCH s0, SPI_control_status           ;read control status bits
                        XOR s0, SPI_rom_cs                     ;deselect (High) FLASH
                        OUTPUT s0, SPI_control_port
                        STORE s0, SPI_control_status           ;preserve status
        page_prog_wait: CALL read_spi_flash_status             ;test WIP bit until finished
                        TEST s2, 01
                        JUMP NZ, page_prog_wait
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
            delay_40us: LOAD s1, 28                            ;40 x 1us = 40us
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
             delay_1ms: LOAD s2, 19                            ;25 x 40us = 1ms
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
            delay_20ms: LOAD s3, 14                            ;20 x 1ms = 20ms
             wait_20ms: CALL delay_1ms
                        SUB s3, 01
                        JUMP NZ, wait_20ms
                        RETURN
                        ;
                        ;Delay of approximately 1 second.
                        ;
                        ;Registers used s0, s1, s2, s3, s4
                        ;
              delay_1s: LOAD s4, 14                            ;50 x 20ms = 1000ms
               wait_1s: CALL delay_20ms
                        SUB s4, 01
                        JUMP NZ, wait_1s
                        RETURN
                        ;
                        ;**************************************************************************************
                        ;UART communication routines
                        ;**************************************************************************************
                        ;
                        ;Read one character from the UART
                        ;
                        ;Character read will be returned in a register called 'UART_data'.
                        ;
                        ;The routine first tests the receiver FIFO buffer to see if data is present.
                        ;If the FIFO is empty, the routine waits until there is a character to read.
                        ;As this could take any amount of time the wait loop could include a call to a
                        ;subroutine which performs a useful function.
                        ;
                        ;If the received character is an XOFF, then the routine will then wait
                        ;for an XON to be received. This means that the rest of the program is held
                        ;in suspense and therefore it can not transmit. Once an XON is received, it will
                        ;again wait for a normal character before returning.
                        ;
                        ;NOTE: Characters between the XOFF and XON will be ignored in this version of the
                        ;program!!!
                        ;
                        ;Interrupt is disabled during this routine to prevent a false situation. If the
                        ;receiver half-full flag went High it should result in an interrupt transmitting
                        ;an XOFF character. However, if this routine were able to read the receiver buffer
                        ;at just about the same as the hardware detects the half-full flag, then it could
                        ;think that an XON needs to be transmitted.
                        ;
                        ;
                        ;Registers used s0 and UART_data
                        ;
        read_from_UART: DISABLE INTERRUPT
     wait_Rx_character: INPUT s0, status_port                  ;test Rx_FIFO buffer
                        TEST s0, rx_data_present
                        JUMP NZ, read_character
                        JUMP wait_Rx_character
        read_character: INPUT UART_data, UART_read_port        ;read from FIFO
                        COMPARE UART_data, character_XOFF      ;test for XOFF
                        JUMP Z, wait_XON
                        ENABLE INTERRUPT                       ;normal finish
                        RETURN
              wait_XON: INPUT s0, status_port                  ;test Rx_FIFO buffer
                        TEST s0, rx_data_present
                        JUMP NZ, read_XON
                        JUMP wait_XON
              read_XON: INPUT UART_data, UART_read_port        ;read from FIFO
                        COMPARE UART_data, character_XON       ;test for XON
                        JUMP Z, wait_Rx_character              ;now wait for normal character
                        JUMP wait_XON                          ;continue to wait for XON
                        ;
                        ;
                        ;
                        ;Transmit one character to the UART
                        ;
                        ;Character supplied in register called 'UART_data'.
                        ;
                        ;The routine first tests the transmit FIFO buffer is empty.
                        ;If the FIFO currently has any data, the routine waits until it is empty.
                        ;Ultimately this means that only one character is sent at a time which
                        ;could be important if the PC at the other end of the link transmits
                        ;an XOFF and needs the flow of data to terminate as soon as possible.
                        ;
                        ;Registers used s0
                        ;
          send_to_UART: INPUT s0, status_port                  ;test Tx_FIFO buffer
                        TEST s0, tx_data_present
                        JUMP Z, UART_write
                        JUMP send_to_UART
            UART_write: OUTPUT UART_data, UART_write_port
                        RETURN
                        ;
                        ;Convert value provided in register s0 into ASCII characters
                        ;
                        ;The value provided must in the range 0 to 99 and will be converted into
                        ;two ASCII characters.
                        ;     The number of 'tens' will be represented by an ASCII character returned in register s1.
                        ;     The number of 'units' will be represented by an ASCII character returned in register s0.
                        ;
                        ;The ASCII representations of '0' to '9' are 30 to 39 hexadecimal which is simply 30 hex added to
                        ;the actual decimal value.
                        ;
                        ;Registers used s0 and s1.
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
                        ;Convert character to upper case
                        ;
                        ;The character supplied in register s0.
                        ;If the character is in the range 'a' to 'z', it is converted
                        ;to the equivalent upper case character in the range 'A' to 'Z'.
                        ;All other characters remain unchanged.
                        ;
                        ;Registers used s0.
                        ;
            upper_case: COMPARE s0, 61                         ;eliminate character codes below 'a' (61 hex)
                        RETURN C
                        COMPARE s0, 7B                         ;eliminate character codes above 'z' (7A hex)
                        RETURN NC
                        AND s0, DF                             ;mask bit5 to convert to upper case
                        RETURN
                        ;
                        ;
                        ;Convert character '0' to '9' to numerical value in range 0 to 9
                        ;
                        ;The character supplied in register s0. If the character is in the
                        ;range '0' to '9', it is converted to the equivalent decimal value.
                        ;Characters not in the range '0' to '9' are signified by the return
                        ;with the CARRY flag set.
                        ;
                        ;Registers used s0.
                        ;
        1char_to_value: ADD s0, C6                             ;reject character codes above '9' (39 hex)
                        RETURN C                               ;carry flag is set
                        SUB s0, F6                             ;reject character codes below '0' (30 hex)
                        RETURN                                 ;carry is set if value not in range
                        ;
                        ;
                        ;Determine the numerical value of a two character decimal string held in
                        ;scratch pad memory such the result is in the range 0 to 99 (00 to 63 hex).
                        ;
                        ;The string must be stored in two consecutive memory locations and the
                        ;location of the first (tens) character supplied in the s1 register.
                        ;The result is provided in register s2. Strings not using characters in the
                        ;range '0' to '9' are signified by the return with the CARRY flag set.
                        ;
                        ;Registers used s0, s1 and s2.
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
                        ;Convert hexadecimal value provided in register s0 into ASCII character
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
                        ;Convert the HEX ASCII characters contained in 's3' and 's2' into
                        ;an equivalent hexadecimal value in register 's0'.
                        ;     The upper nibble is represented by an ASCII character in register s3.
                        ;     The lower nibble is represented by an ASCII character in register s2.
                        ;
                        ;Input characters must be in the range 00 to FF hexadecimal or the CARRY flag
                        ;will be set on return.
                        ;
                        ;Registers used s0, s2 and s3.
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
                        ;Routine to convert ASCII data in 's0' to an equivalent HEX value.
                        ;
                        ;If character is not valid for hex, then CARRY is set on return.
                        ;
                        ;Register used s0
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
                        ;**************************************************************************************
                        ;Text messages
                        ;**************************************************************************************
                        ;
                        ;
                        ;Send 'PicoBlaze SPI FLASH Programmer' string to the UART
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
                        LOAD UART_data, character_S
                        CALL send_to_UART
                        LOAD UART_data, character_P
                        CALL send_to_UART
                        LOAD UART_data, character_I
                        CALL send_to_UART
                        CALL send_space
                        LOAD UART_data, character_F
                        CALL send_to_UART
                        LOAD UART_data, character_L
                        CALL send_to_UART
                        LOAD UART_data, character_A
                        CALL send_to_UART
                        LOAD UART_data, character_S
                        CALL send_to_UART
                        LOAD UART_data, character_H
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
                        LOAD UART_data, character_a
                        CALL send_to_UART
                        LOAD UART_data, character_m
                        CALL send_to_UART
                        LOAD UART_data, character_m
                        CALL send_to_UART
                        LOAD UART_data, character_e
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
                        ;Send 'Waiting_MCS_file' string to the UART
                        ;
 send_Waiting_MCS_file: LOAD UART_data, character_W
                        CALL send_to_UART
                        LOAD UART_data, character_a
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
                        LOAD UART_data, character_f
                        CALL send_to_UART
                        LOAD UART_data, character_o
                        CALL send_to_UART
                        LOAD UART_data, character_r
                        CALL send_to_UART
         send_MCS_file: CALL send_space
                        LOAD UART_data, character_M
                        CALL send_to_UART
                        LOAD UART_data, character_C
                        CALL send_to_UART
                        LOAD UART_data, character_S
                        CALL send_to_UART
                        CALL send_space
                        LOAD UART_data, character_F
                        CALL send_to_UART
                        LOAD UART_data, character_i
                        CALL send_to_UART
                        LOAD UART_data, character_l
                        CALL send_to_UART
                        LOAD UART_data, character_e
                        CALL send_to_UART
                        CALL send_CR
                        RETURN
                        ;
                        ;
                        ;Send 'Erase in progress' string to the UART
                        ;
send_Erase_in_progress: CALL send_Erase
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
                        ;Send carriage return, 'OK' and carriage return to the UART
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
                        ;
                        ;Send menu to the UART
                        ;
             send_Menu: CALL send_CR
                        LOAD UART_data, character_E
                        CALL send_to_UART
                        LOAD UART_data, character_minus
                        CALL send_to_UART
                        CALL send_Erase
                        LOAD UART_data, character_a
                        CALL send_to_UART
                        LOAD UART_data, character_l
                        CALL send_to_UART
                        CALL send_to_UART
                        CALL send_CR
                        LOAD UART_data, character_S
                        CALL send_to_UART
                        LOAD UART_data, character_minus
                        CALL send_to_UART
                        LOAD UART_data, character_S
                        CALL send_to_UART
                        LOAD UART_data, character_e
                        CALL send_to_UART
                        LOAD UART_data, character_c
                        CALL send_to_UART
                        LOAD UART_data, character_t
                        CALL send_to_UART
                        LOAD UART_data, character_o
                        CALL send_to_UART
                        LOAD UART_data, character_r
                        CALL send_to_UART
                        CALL send_space
                        CALL send_Erase
                        CALL send_CR
                        LOAD UART_data, character_P
                        CALL send_to_UART
                        LOAD UART_data, character_minus
                        CALL send_to_UART
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
                        LOAD UART_data, character_a
                        CALL send_to_UART
                        LOAD UART_data, character_m
                        CALL send_to_UART
                        CALL send_MCS_file
                        LOAD UART_data, character_R
                        CALL send_to_UART
                        LOAD UART_data, character_minus
                        CALL send_to_UART
                        LOAD UART_data, character_R
                        CALL send_to_UART
                        LOAD UART_data, character_e
                        CALL send_to_UART
                        LOAD UART_data, character_a
                        CALL send_to_UART
                        LOAD UART_data, character_d
                        CALL send_to_UART
                        CALL send_space
                        CALL send_page
                        CALL send_CR
                        LOAD UART_data, character_I
                        CALL send_to_UART
                        LOAD UART_data, character_minus
                        CALL send_to_UART
                        LOAD UART_data, character_D
                        CALL send_to_UART
                        LOAD UART_data, character_e
                        CALL send_to_UART
                        LOAD UART_data, character_v
                        CALL send_to_UART
                        LOAD UART_data, character_i
                        CALL send_to_UART
                        LOAD UART_data, character_c
                        CALL send_to_UART
                        LOAD UART_data, character_e
                        CALL send_to_UART
                        CALL send_space
                        CALL send_ID
                        CALL send_CR
                        LOAD UART_data, character_H
                        CALL send_to_UART
                        LOAD UART_data, character_minus
                        CALL send_to_UART
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
                        ;Send 'page' to the UART
                        ;
             send_page: LOAD UART_data, character_p
                        CALL send_to_UART
                        LOAD UART_data, character_a
                        CALL send_to_UART
                        LOAD UART_data, character_g
                        CALL send_to_UART
                        LOAD UART_data, character_e
                        CALL send_to_UART
                        RETURN
                        ;
                        ;Send 'ID' to the UART
                        ;
               send_ID: LOAD UART_data, character_I
                        CALL send_to_UART
                        LOAD UART_data, character_D
                        CALL send_to_UART
                        RETURN
                        ;
                        ;
                        ;Send 'Confirm Erase (Y/n) ' to the UART
                        ;
          send_Confirm: CALL send_CR
                        LOAD UART_data, character_C
                        CALL send_to_UART
                        LOAD UART_data, character_o
                        CALL send_to_UART
                        LOAD UART_data, character_n
                        CALL send_to_UART
                        LOAD UART_data, character_f
                        CALL send_to_UART
                        LOAD UART_data, character_i
                        CALL send_to_UART
                        LOAD UART_data, character_r
                        CALL send_to_UART
                        LOAD UART_data, character_m
                        CALL send_to_UART
                        CALL send_space
                        CALL send_Erase
                        LOAD UART_data, character_open
                        CALL send_to_UART
                        LOAD UART_data, character_Y
                        CALL send_to_UART
                        LOAD UART_data, character_divide
                        CALL send_to_UART
                        LOAD UART_data, character_n
                        CALL send_to_UART
                        LOAD UART_data, character_close
                        CALL send_to_UART
                        CALL send_space
                        RETURN
                        ;
                        ;
                        ;Send 'Abort' to the UART
                        ;
            send_Abort: CALL send_CR
                        LOAD UART_data, character_A
                        CALL send_to_UART
                        LOAD UART_data, character_b
                        CALL send_to_UART
                        LOAD UART_data, character_o
                        CALL send_to_UART
                        LOAD UART_data, character_r
                        CALL send_to_UART
                        LOAD UART_data, character_t
                        CALL send_to_UART
                        CALL send_CR
                        RETURN
                        ;
                        ;Send 'page address' to the UART
                        ;
     send_page_address: CALL send_CR
                        CALL send_page
                        CALL send_space
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
                        LOAD UART_data, character_equals
                        CALL send_to_UART
                        RETURN
                        ;
                        ;
                        ;**************************************************************************************
                        ;Interrupt Service Routine (ISR)
                        ;**************************************************************************************
                        ;
                        ;An interrupt occurs whenever the status of the UART receiver FIFO 'half_full' flag
                        ;changes.
                        ;
                        ;A change from Low to High means that the buffer is becoming full and therefore
                        ;an XOFF character must be transmitted.
                        ;
                        ;A change from High to Low means that the buffer is starting to empty and therefore
                        ;an XON character can be transmitted to restart the flow of new characters.
                        ;
                        ;
                        ADDRESS 3F5                            ;place at end of memory to keep separate
                   ISR: STORE s0, ISR_preserve_s0              ;preserve register contents
                        INPUT s0, status_port                  ;test 'half_full' status of receiver buffer.
                        TEST s0, rx_half_full
                        JUMP Z, isr_send_xon
                        LOAD s0, character_XOFF
                        JUMP isr_send_character
          isr_send_xon: LOAD s0, character_XON
    isr_send_character: OUTPUT s0, UART_write_port
                        FETCH s0, ISR_preserve_s0              ;restore register contents
                        RETURNI ENABLE
                        ;
                        ;
                        ;**************************************************************************************
                        ;Interrupt Vector
                        ;**************************************************************************************
                        ;
                        ADDRESS 3FF
                        JUMP ISR
                        ;
                        ;
