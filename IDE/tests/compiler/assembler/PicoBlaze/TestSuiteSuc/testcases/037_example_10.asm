                        ;KCPSM3 Program - UART programming of StrataFLASH memory on the Spartan-3E Starter Kit.
                        ;
                        ;Ken Chapman - Xilinx Ltd
                        ;
                        ;Version v1.00 - 28th March 2006
                        ;
                        ;This program uses a 115200 baud UART connection with XON/XOFF flow control
                        ;to allow a standard MCS file for the configuration of a Spartan-3E device to
                        ;be programmed into the Intel StrataFLASH device on the board.
                        ;
                        ;
                        ;
                        ;
                        ;**************************************************************************************
                        ; Port definitions
                        ;**************************************************************************************
                        ;
                        ;
                        CONSTANT status_port, 00               ;UART and filter status input
                        CONSTANT tx_data_present, 01           ;  Transmitter  data present - bit0
                        CONSTANT tx_half_full, 02              ;    FIFO          half full - bit1
                        CONSTANT tx_full, 04                   ;                       full - bit2
                        CONSTANT rx_data_present, 08           ;               data present - bit3
                        CONSTANT rx_half_full, 10              ;  Receiver        half full - bit4
                        CONSTANT rx_full, 20                   ;    FIFO               full - bit5
                        CONSTANT spare1, 40                    ;                  spare '0' - bit6
                        CONSTANT SF_STS, 80                    ;            StrataFLASH STS - bit7
                        ;
                        CONSTANT UART_read_port, 01            ;UART Rx data input
                        ;
                        CONSTANT UART_write_port, 04           ;UART Tx data output
                        ;
                        ;
                        CONSTANT SF_data_in_port, 02           ;Read data from StrataFLASH device
                        ;
                        CONSTANT SF_data_out_port, 10          ;Data to write into StrataFLASH device
                        ;
                        CONSTANT SF_addr_hi_port, 80           ;StrataFLASH address[23:16]
                        CONSTANT SF_addr_mi_port, 40           ;StrataFLASH address[15:8]
                        CONSTANT SF_addr_lo_port, 20           ;StrataFLASH address[7:0]
                        ;
                        CONSTANT SF_control_port, 08           ;StrataFLASH control
                        CONSTANT SF_read, 01                   ;         active High read - bit0
                        CONSTANT SF_ce, 02                     ; active Low device enable - bit1
                        CONSTANT SF_we, 04                     ;         active Low write - bit2
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
                        ; Useful data constants
                        ;**************************************************************************************
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
                        ; Scratch Pad Memory Locations
                        ;**************************************************************************************
                        ;
                        CONSTANT ISR_preserve_s0, 00           ;preserve register during ISR
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
                        ;So a total of 21 bytes could be stored before processing.
                        ;This is located at the end of scratch pad memory.
                        ;
                        CONSTANT line_start, 2B                ;21 bytes until end of memory
                        CONSTANT data_start, 2F                ;Start of data field if present
                        ;
                        ;
                        ;**************************************************************************************
                        ; Initialise the system and welcome message
                        ;**************************************************************************************
                        ;
            cold_start: CALL SF_init                           ;initialise StrataFLASH controls
                        CALL delay_1s                          ;delay because UART is fast and JTAG startup sequence can be slow
                        ENABLE INTERRUPT                       ;Interrupt is used for XON/XOFF flow control
         welcome_start: CALL send_CR
                        CALL send_welcome                      ;start up message and version number
                        ;
                        ;
                        ;**************************************************************************************
                        ; Main menu and command selection
                        ;**************************************************************************************
                        ;
                        ;
            warm_start: CALL send_Menu                         ;Menu and command selection
                        CALL send_CR
                        ;
                prompt: CALL send_CR
                        CALL send_CR
                        LOAD UART_data, character_greater_than ;prompt for input
                        CALL send_to_UART
                        CALL read_upper_case
                        COMPARE s0, character_E                ;test for commands and execute as required
                        JUMP Z, erase_command
                        COMPARE s0, character_B
                        JUMP Z, block_erase_command
                        COMPARE s0, character_P
                        JUMP Z, program_command
                        COMPARE s0, character_W
                        JUMP Z, write_command
                        COMPARE s0, character_R
                        JUMP Z, read_command
                        COMPARE s0, character_I
                        JUMP Z, SF_information
                        COMPARE s0, character_H
                        JUMP Z, welcome_start
                        COMPARE s0, character_S
                        JUMP Z, SF_status
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
                        ;**************************************************************************************
                        ; Erase Command - Perform bulk erase of the StrataFLASH memory
                        ;**************************************************************************************
                        ;
                        ; This routine executes the block erase command 128 times with a different base
                        ; address in each case.
                        ;
                        ; Note that this could take as long as 8 minutes 30 seconds
                        ; and even typical times will be approximately 2 minutes.
                        ;
                        ; Registers used s1,s7,s8,s9
                        ;
         erase_command: LOAD s9, FE                            ;define base address of block 127 = FE0000
                        JUMP blocks_erase
                        ;
                        ;
                        ;**************************************************************************************
                        ; Block Erase Command - Performs erase of lowest 3 blocks of StrataFLASH memory which
                        ; covers the address range 000000 to 05FFFF in which the configuration for an XC3S500E
                        ; would be able to fit (000000 to 045470).
                        ;**************************************************************************************
                        ;
                        ; This routine executes the block erase command 3 times with a different base
                        ; address in each case.
                        ;
                        ; Each block is 128K bytes and therefore has an address range of 000000 to 01FFFF.
                        ; So each block is separated by 020000 hex.
                        ;
                        ; Registers used s0,s1,s7,s8,s9
                        ;
   block_erase_command: LOAD s9, 04                            ;define base address of block 3 = 040000
          blocks_erase: CALL send_CR
                        CALL send_Confirm                      ;confirm command with a 'Y' which must be upper case
                        CALL read_from_UART                    ;read command character from UART
                        CALL send_to_UART                      ;echo input
                        COMPARE UART_data, character_Y
                        JUMP NZ, abort_erase
                        CALL send_CR
                        CALL send_Erase_in_progress
                        CALL send_CR
                        LOAD s8, 00                            ;define lower address of each block = xx0000
                        LOAD s7, 00
     blocks_erase_loop: LOAD UART_data, character_fullstop     ;progress dots
                        CALL send_to_UART
                        CALL SF_erase_block                    ;erase block
                        SUB s9, 02                             ;decrement base address by 1 block
                        JUMP NC, blocks_erase_loop             ;repeat until block 0 is erased
                        CALL send_OK
                        JUMP prompt
                        ;
           abort_erase: CALL send_Abort
                        JUMP prompt
                        ;
                        ;
                        ;**************************************************************************************
                        ; Erase a single 128K Byte block of the StrataFlash Memory
                        ;**************************************************************************************
                        ;
                        ; The 24-bit address of the block should be supplied in register set [s9,s8,s7].
                        ;
                        ; To erase a block the address must be set and then the block erase command (20 hex)
                        ; written to the memory followed by the write confirm command (D0 hex).
                        ;
                        ; The act of erasing a block may take up to 1 second to complete. This routine
                        ; waits for the memory to be ready before restoring the normal read array mode and
                        ; returning.
                        ;
                        ; Registers used s1,s7,s8,s9
                        ;
        SF_erase_block: LOAD s1, 20                            ;block erase command
                        CALL SF_byte_write
                        LOAD s1, D0                            ;write confirm command
                        CALL SF_byte_write
                        CALL wait_SF_ready                     ;wait for erase to complete
                        RETURN
                        ;
                        ;
                        ;**************************************************************************************
                        ; Program Command - Program StrataFLASH memory with data defined in an MCS file
                        ;**************************************************************************************
                        ;
       program_command: CALL send_CR
                        CALL send_Waiting_MCS_file
                        CALL program_MCS
                        CALL send_OK
                        JUMP prompt
                        ;
                        ;**************************************************************************************
                        ; Program StrataFLASH memory with data defined in an MCS file
                        ;**************************************************************************************
                        ;
                        ;Reads the MCS file from the UART and programs the Strata FLASH device at the locations.
                        ;specified by the file contents.
                        ;
                        ;This routine will continue until an end of file record is detected.
                        ;For each line of MCS received, the current address will be output so that
                        ;progress can be monitored.
                        ;
                        ;
           program_MCS: CALL read_MCS_line                     ;read line from UART
                        CALL MCS_address                       ;find start address and record type
                        COMPARE sB, 01                         ;test for end record
                        RETURN Z                               ;end of programming
                        COMPARE sB, 04                         ;test for extended address record
                        JUMP Z, program_MCS                    ;no data with this record and upper address now correct
                        ;
                        ;Assume data record type 00 which is data so need to program specified number
                        ;of bytes into memory at correct address.
                        ;
        write_spm_data: CALL send_hex_3bytes                   ;send address to indicate progress
                        CALL send_CR
                        FETCH sA, line_start                   ;read number of data bytes to program
                        CALL SF_buffer_write                   ;write bytes to memory
                        JUMP program_MCS
                        ;
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
                        ;
                        ;**************************************************************************************
                        ; Write to Buffer of StrataFlash Memory and program
                        ;**************************************************************************************
                        ;
                        ; Writing to the buffer allows faster operation than writing individual bytes.
                        ; The buffer size is limited to 32 locations. To perform a buffer write the process
                        ; is as follows:-
                        ;   Write command for buffer write to StrataFLASH memory (E8 hex).
                        ;      Base address for writing should also be set.
                        ;   Read Status register and if not ready repeat command until it is.
                        ;   Write a value specifying the number of bytes to be written LESS ONE.
                        ;      In this program the number of bytes will be specified in register sA
                        ;      and this value needs to be decremented before writing to the memory.
                        ;   Write the correct number of actual data bytes with appropriate addresses.
                        ;      Ideally the addresses do not cross the boundary of 32 locations
                        ;      such that LSBs are always in the range 00000 to 11111 binary.
                        ;      Crossing the boundary is OK but will take longer to program.
                        ;   Write command to confirm operation (D0 hex).
                        ;   Read Status register and wait for ready.
                        ;
                        ;   This routine additionally restores the normal read array mode before returning.
                        ;
                        ; The number of bytes to be written should be supplied in register sA and must be
                        ; a value between 1 and 32 (01 and 20 hex).
                        ;
                        ; The 24-bit base address should be supplied in register set [s9,s8,s7].
                        ; On return, this will be increased by the number of locations written.
                        ;
                        ; Scratch pad memory locations starting at location defined by constant
                        ; 'data_start' should contain the data bytes to be written.
                        ;
                        ; The act of writing the buffer to the memory array may take up to 654us to complete.
                        ; The time taken to program is recorded by register pair [sE,sD]. Each count
                        ; equates to 15 instructions which is equivalent to 30 clock cycles (600ns at 50MHz).
                        ;
                        ; Registers used s0,s1,s7,s8,s9,sA,sD,sE
                        ;
                        ;
       SF_buffer_write: LOAD s1, E8                            ;command for buffer write
                        CALL SF_byte_write
                        CALL SF_byte_read                      ;read status register into s0
                        TEST s0, 80                            ;test ready/busy flag
                        JUMP Z, SF_buffer_write                ;repeat command until ready
                        LOAD s1, sA                            ;Specify number of bytes to write
                        SUB s1, 01                             ;one less than actual number!
                        CALL SF_byte_write
                        LOAD s3, data_start                    ;point to data in scratch pad memory
     write_buffer_loop: FETCH s1, (s3)                         ;fetch data
                        CALL SF_byte_write                     ;write to buffer
                        ADD s7, 01                             ;increment address
                        ADDCY s8, 00
                        ADDCY s9, 00
                        ADD s3, 01                             ;increment SPM pointer
                        SUB sA, 01                             ;count bytes remaining
                        JUMP NZ, write_buffer_loop
                        LOAD s1, D0                            ;command to confirm write
                        CALL SF_byte_write
                        CALL wait_SF_ready                     ;wait for program to complete and set read array mode
                        RETURN
                        ;
                        ;
                        ;**************************************************************************************
                        ; Write Command - Write one byte to specified address
                        ;**************************************************************************************
                        ;
         write_command: CALL send_address                      ;obtain 24-bit address 000000 to FFFFFF
                        CALL obtain_8bits
                        JUMP C, write_command                  ;bad input address
                        LOAD s9, s0
                        CALL obtain_8bits
                        JUMP C, write_command                  ;bad input address
                        LOAD s8, s0
                        CALL obtain_8bits
                        JUMP C, write_command                  ;bad input address
                        LOAD s7, s0
              get_data: CALL send_data                         ;obtain 8-bit data 00 to FF into s0
                        CALL obtain_8bits
                        JUMP C, get_data                       ;bad input data
                        CALL SF_single_byte_write
                        CALL send_CR
                        CALL send_OK
                        JUMP prompt
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
                        ; returning. The time taken to program is recorded by register pair [sE,sD]. Each count
                        ; equates to 15 instructions which is equivalent to 30 clock cycles (600ns at 50MHz).
                        ;
                        ; Registers used s0,s1,s7,s8,s9,sD,sE   (s7,s8,s9 not changed)
                        ;
                        ; Registers used s0,s1,s7,s8,s9
                        ;
  SF_single_byte_write: LOAD s1, 40                            ;command for single byte program
                        CALL SF_byte_write
                        LOAD s1, s0                            ;write data to be programmed
                        CALL SF_byte_write
                        CALL wait_SF_ready                     ;wait for program to complete
                        RETURN
                        ;
                        ;
                        ;**************************************************************************************
                        ;Read Command - Read one page of memory at specified address
                        ;**************************************************************************************
                        ;
          read_command: CALL send_address                      ;obtain 24-bit address 000000 to FFFFFF
                        CALL obtain_8bits                      ;read value from UART
                        JUMP C, read_command                   ;bad input address
                        LOAD s9, s0
                        CALL obtain_8bits
                        JUMP C, read_command                   ;bad input address
                        LOAD s8, s0
                        CALL obtain_8bits
                        JUMP C, read_command                   ;bad input address
                        LOAD s7, s0
                        CALL send_CR
                        CALL send_SF_page
                        CALL send_OK
                        JUMP prompt
                        ;
                        ;The start address should be provided in register set [s9,s8,s7].
                        ;The display will be next 256 bytes displayed as 16 lines of 16 bytes
                        ;with each line commencing with the address of the first byte.
                        ;
          send_SF_page: LOAD s6, 10                            ;16 lines to display
          send_SF_line: CALL send_CR
                        CALL send_hex_3bytes                   ;display address
                        CALL send_space
                        LOAD s5, 10                            ;16 bytes to display on a line
          send_SF_byte: CALL send_space
                        CALL SF_byte_read                      ;read byte into s0
                        ADD s7, 01                             ;increment StrataFLASH address
                        ADDCY s8, 00
                        ADDCY s9, 00
                        CALL send_hex_byte                     ;display byte
                        SUB s5, 01                             ;count bytes per line
                        JUMP NZ, send_SF_byte
                        SUB s6, 01                             ;count lines
                        JUMP NZ, send_SF_line
                        CALL send_CR
                        RETURN
                        ;
                        ;
                        ;**************************************************************************************
                        ; ID Command - Read and display the device information for the StrataFLASH FLASH memory
                        ;**************************************************************************************
                        ;
                        ; Normal response should be
                        ;   Device Manufacturer Code (Intel) = 89 hex
                        ;   Memory ID code for 128Mbit = 18 hex
                        ;
                        ; To read the device information the Read device information command (90)
                        ; must be written to the memory. The information is read back but assumes
                        ; that 16-bit words are being used and hence address bit0 is not really used.
                        ; hence addresses 000000 and 0000001 both return the Device Manufacturer Code and
                        ; addresses 000002 and 0000003 both return the Memory ID code.
                        ;
                        ; After reading the device information the read array command is written to the
                        ; device to put it back to normal read mode.
                        ;
                        ; Registers used s0,s7,s8,s9
                        ;
        SF_information: CALL send_CR                           ;send 'ID=' to terminal
                        CALL send_ID
                        LOAD UART_data, character_equals
                        CALL send_to_UART
                        CALL send_space
                        LOAD s9, 00                            ;define base address 000000
                        LOAD s8, 00
                        LOAD s7, 00
                        LOAD s1, 90                            ;command to read device information
                        CALL SF_byte_write
                        CALL SF_byte_read                      ;read Device Manufacturer Code into s0
                        CALL send_hex_byte                     ;display byte
                        CALL send_space
                        LOAD s7, 02                            ;change address
                        CALL SF_byte_read                      ;read Memory ID code into s0
                        CALL send_hex_byte                     ;display byte
                        CALL send_CR
                        CALL set_SF_read_array_mode            ;restore normal read array mode
                        JUMP prompt
                        ;
                        ;
                        ;**************************************************************************************
                        ; Read StrataFLASH status register
                        ;**************************************************************************************
                        ;
                        ; The main reason for reading the status register is to determine when the memory
                        ; is ready or busy. This information is provided by bit7 (0=busy and 1=ready).
                        ;
                        ; The lower bits all indicate errors of some kind and therefore the only desirable
                        ; response is 00 hex or 80 hex. In this program, no error checking or clearing
                        ; is performed and the way this routine is executed from the menu only 80 hex is
                        ; expected.
                        ;
                        ; To read the status register the read status register command must be written to
                        ; the device. All subsequent reads are then result in the return of the status
                        ; register. A different read command must be written to the device to stop this
                        ; mode.
                        ;
                        ; This mode is also entered automatically when performing program and erase operations.
                        ;
             SF_status: LOAD s9, 00                            ;define base address 000000
                        LOAD s8, 00
                        LOAD s7, 00
                        LOAD s1, 70                            ;command to read status register
                        CALL SF_byte_write
                        CALL send_CR
                        CALL SF_byte_read                      ;read status register into s0
                        CALL send_hex_byte                     ;display byte
                        CALL send_CR
                        CALL set_SF_read_array_mode
                        JUMP prompt
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
          SF_byte_read: OUTPUT s9, SF_addr_hi_port             ;set 24-bit address
                        OUTPUT s8, SF_addr_mi_port
                        OUTPUT s7, SF_addr_lo_port
                        LOAD s1, 05                            ;set controls
                        OUTPUT s1, SF_control_port
                        LOAD s1, 06                            ;>75ns delay
                        LOAD s1, 06                            ;but do something useful!
                        INPUT s0, SF_data_in_port              ;read data byte
                        OUTPUT s1, SF_control_port             ;clear controls
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
         SF_byte_write: OUTPUT s9, SF_addr_hi_port             ;set 24-bit address
                        OUTPUT s8, SF_addr_mi_port
                        OUTPUT s7, SF_addr_lo_port
                        OUTPUT s1, SF_data_out_port            ;set data byte to be written
                        LOAD s1, 00                            ;set controls
                        OUTPUT s1, SF_control_port
                        LOAD s1, 06                            ;>60ns delay
                        LOAD s1, 06                            ;but do something useful!
                        OUTPUT s1, SF_control_port             ;clear controls
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
set_SF_read_array_mode: LOAD s1, FF                            ;command to read array
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
                        ; During the polling process, a counter formed by register pair [sE,sD] records
                        ; approximately how long the memory is busy. This can be used to evaluate programming
                        ; and erase times if required. The timing loop is 15 instructions which is equivalent
                        ; to 30 clock cycles (600ns at 50MHz)
                        ;
                        ; Registers used s0,s1,s7,s8,s9,sD,sE   (s7,s8,s9 not changed)
                        ;
                        ;
         wait_SF_ready: LOAD sE, 00                            ;clear 16-bit counter timer
                        LOAD sD, 00
          wait_SF_loop: ADD sD, 01                             ;increment counter timer
                        ADDCY sE, 00
                        CALL SF_byte_read                      ;read status register into s0
                        TEST s0, 80                            ;test ready/busy flag
                        JUMP Z, wait_SF_loop
                        CALL set_SF_read_array_mode            ;restore normal read array mode
                        RETURN
                        ;
                        ;
                        ;**************************************************************************************
                        ; Send 16-bit value in register pair [sE,sD] to UART
                        ;**************************************************************************************
                        ;
                        ; In this program the register pair [sE,sD] indicates the programming time of the
                        ; StrataFLASH memory in 600ns increments. This routine can be used to display that
                        ; value if required.
                        ;
    send_counter_timer: CALL send_CR
                        LOAD s0, sE
                        CALL send_hex_byte
                        LOAD s0, sD
                        CALL send_hex_byte
                        CALL send_CR
                        RETURN
                        ;
                        ;
                        ;**************************************************************************************
                        ;Software delay routines
                        ;**************************************************************************************
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
                        ;
                        ;**************************************************************************************
                        ;Useful ASCII conversion and handling routines
                        ;**************************************************************************************
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
                        ;Read two hex characters from UART and convert to single byte data
                        ;
          obtain_8bits: CALL read_upper_case                   ;obtain one byte from UART
                        LOAD s3, s0
                        CALL read_upper_case
                        LOAD s2, s0
                        CALL ASCII_byte_to_hex
                        RETURN
                        ;
                        ;**************************************************************************************
                        ;Text messages
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
                        ;Send 'PicoBlaze NOR FLASH Programmer' string to the UART
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
                        LOAD UART_data, character_N
                        CALL send_to_UART
                        LOAD UART_data, character_O
                        CALL send_to_UART
                        LOAD UART_data, character_R
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
                        LOAD UART_data, character_B
                        CALL send_to_UART
                        LOAD UART_data, character_minus
                        CALL send_to_UART
                        CALL send_Erase
                        LOAD UART_data, character_b
                        CALL send_to_UART
                        LOAD UART_data, character_l
                        CALL send_to_UART
                        LOAD UART_data, character_o
                        CALL send_to_UART
                        LOAD UART_data, character_c
                        CALL send_to_UART
                        LOAD UART_data, character_k
                        CALL send_to_UART
                        LOAD UART_data, character_s
                        CALL send_to_UART
                        CALL send_space
                        LOAD UART_data, character_1
                        CALL send_to_UART
                        LOAD UART_data, character_minus
                        CALL send_to_UART
                        LOAD UART_data, character_3
                        CALL send_to_UART
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
                        LOAD UART_data, character_W
                        CALL send_to_UART
                        LOAD UART_data, character_minus
                        CALL send_to_UART
                        LOAD UART_data, character_W
                        CALL send_to_UART
                        LOAD UART_data, character_r
                        CALL send_to_UART
                        LOAD UART_data, character_i
                        CALL send_to_UART
                        LOAD UART_data, character_t
                        CALL send_to_UART
                        LOAD UART_data, character_e
                        CALL send_to_UART
                        CALL send_space
                        CALL send_byte
                        CALL send_CR
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
                        LOAD UART_data, character_2
                        CALL send_to_UART
                        LOAD UART_data, character_5
                        CALL send_to_UART
                        LOAD UART_data, character_6
                        CALL send_to_UART
                        CALL send_space
                        CALL send_byte
                        LOAD UART_data, character_s
                        CALL send_to_UART
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
                        LOAD UART_data, character_S
                        CALL send_to_UART
                        LOAD UART_data, character_minus
                        CALL send_to_UART
                        LOAD UART_data, character_S
                        CALL send_to_UART
                        LOAD UART_data, character_t
                        CALL send_to_UART
                        LOAD UART_data, character_a
                        CALL send_to_UART
                        LOAD UART_data, character_t
                        CALL send_to_UART
                        LOAD UART_data, character_u
                        CALL send_to_UART
                        LOAD UART_data, character_s
                        CALL send_to_UART
                        CALL send_CR
                        RETURN
                        ;
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
                        ;Send 'byte' to the UART
                        ;
             send_byte: LOAD UART_data, character_b
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
                        ;Send 'address=' to the UART
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
                        ;Send 'data=' to the UART
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
                        JUMP send_equals
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
