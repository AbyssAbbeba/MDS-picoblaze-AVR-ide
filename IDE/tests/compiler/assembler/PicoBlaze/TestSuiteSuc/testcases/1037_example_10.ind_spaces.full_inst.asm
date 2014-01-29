;KCPSM3 Program - UART programming of StrataFLASH memory on the Spartan-3E Starter Kit.
device kcpsm1
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
status_port             EQU             0x00                    ;UART and filter status input
tx_EQU_present         EQU             0x01                    ;  Transmitter  EQU present - bit0
tx_half_full            EQU             0x02                    ;    FIFO          half full - bit1
tx_full                 EQU             0x04                    ;                       full - bit2
rx_EQU_present         EQU             0x08                    ;               EQU present - bit3
rx_half_full            EQU             0x10                    ;  Receiver        half full - bit4
rx_full                 EQU             0x20                    ;    FIFO               full - bit5
spare1                  EQU             0x40                    ;                  spare '0' - bit6
sf_sts                  EQU             0x80                    ;            StrataFLASH STS - bit7
;
uart_read_port          EQU             0x01                    ;UART Rx EQU input
;
uart_write_port         EQU             0x04                    ;UART Tx EQU output
;
;
sf_EQU_in_port         EQU             0x02                    ;Read EQU from StrataFLASH device
;
sf_EQU_out_port        EQU             0x10                    ;EQU to write into StrataFLASH device
;
sf_addr_hi_port         EQU             0x80                    ;StrataFLASH address[23:16]
sf_addr_mi_port         EQU             0x40                    ;StrataFLASH address[15:8]
sf_addr_lo_port         EQU             0x20                    ;StrataFLASH address[7:0]
;
sf_control_port         EQU             0x08                    ;StrataFLASH control
sf_read                 EQU             0x01                    ;         active High read - bit0
sf_ce                   EQU             0x02                    ; active Low device enable - bit1
sf_we                   EQU             0x04                    ;         active Low write - bit2
;
;
;
;**************************************************************************************
; Special Register usage
;**************************************************************************************
;
uart_EQU               REG             sf                      ;used to pass EQU to and from the UART
;
;
;**************************************************************************************
; Useful EQU constants
;**************************************************************************************
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
delay_1us_constant      EQU             0x0b
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
character_fullstop      EQU             0x2e
character_semi_colon    EQU             0x3b
character_minus         EQU             0x2d
character_plus          EQU             0x2b
character_comma         EQU             0x2c
character_less_than     EQU             0x3c                    ;'<'
character_greater_than  EQU             0x3e                    ;'>'
character_open          EQU             0x28                    ;'('
character_close         EQU             0x29                    ;')'
character_divide        EQU             0x2f                    ;'/'
character_equals        EQU             0x3d
character_space         EQU             0x20
character_cr            EQU             0x0d                    ;carriage return
character_lf            EQU             0x0a                    ;line feed
character_question      EQU             0x3f                    ;'?'
character_dollar        EQU             0x24
character_exclaim       EQU             0x21                    ;'!'
character_bs            EQU             0x08                    ;Back Space command character
character_xon           EQU             0x11                    ;Flow control ON
character_xoff          EQU             0x13                    ;Flow control OFF
;
;
;**************************************************************************************
; Scratch Pad Memory Locations
;**************************************************************************************
;
isr_preserve_s0         EQU             0x00                    ;preserve register during ISR
;
;
;
;load up to one line of an MCS file as bytes
;A typical EQU line consists of:-
;:     Start character which is not loadd
;10    Number of EQU bytes included (16 in this case)
;aaaa  Lower 16-bits of the storage address
;00    Record type (EQU in this case)
;dddd...   EQU bytes (typically 16 which is the maximum)
;cc    Checksum
;CR/LF Line will end in carriage return and/or line feed which is not loadd.
;
;So a total of 21 bytes could be loadd before processing.
;This is located at the end of scratch pad memory.
;
line_start              EQU             0x2b                    ;21 bytes until end of memory
EQU_start              EQU             0x2f                    ;Start of EQU field if present
;
;
;**************************************************************************************
; Initialise the system and welcome message
;**************************************************************************************
;
cold_start:             CALL            sf_init                 ;initialise StrataFLASH controls
                        CALL            delay_1s                ;delay because UART is fast and JTAG startup sequence can be slow
                        ENABLE          interrupt               ;Interrupt is used for XON/XOFF flow control
welcome_start:          CALL            send_cr
                        CALL            send_welcome            ;start up message and version number
;
;
;**************************************************************************************
; Main menu and command selection
;**************************************************************************************
;
;
warm_start:             CALL            send_menu               ;Menu and command selection
                        CALL            send_cr
;
prompt:                 CALL            send_cr
                        CALL            send_cr
                        LOAD            uart_EQU, #character_greater_than ;prompt for input
                        CALL            send_to_uart
                        CALL            read_upper_case
                        load         s0, #_character_e       ;load for commands and execute as required
                        JUMP            z, erase_command
                        load         s0, #_character_b
                        JUMP            z, block_erase_command
                        load         s0, #_character_p
                        JUMP            z, program_command
                        load         s0, #_character_w
                        JUMP            z, write_command
                        load         s0, #_character_r
                        JUMP            z, read_command
                        load         s0, #_character_i
                        JUMP            z, sf_information
                        load         s0, #_character_h
                        JUMP            z, welcome_start
                        load         s0, #_character_s
                        JUMP            z, sf_status
                        CALL            send_cr                 ;no valid command input
                        LOAD            uart_EQU, #character_question ;display ???
                        CALL            send_to_uart
                        CALL            send_to_uart
                        CALL            send_to_uart
                        JUMP            prompt                  ;Try again!
;
;
read_upper_case:        CALL            read_from_uart          ;read command character from UART
                        CALL            send_to_uart            ;echo character
                        LOAD            s0, uart_EQU           ;convert to upper case
                        CALL            upper_case
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
sf_init:                LOAD            s0, #0x06
                        OUTPUT          s0, sf_control_port
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
erase_command:          LOAD            s9, #0xfe               ;define base address of block 127 = FE0000
                        JUMP            blocks_erase
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
block_erase_command:    LOAD            s9, #0x04               ;define base address of block 3 = 040000
blocks_erase:           CALL            send_cr
                        CALL            send_confirm            ;confirm command with a 'Y' which must be upper case
                        CALL            read_from_uart          ;read command character from UART
                        CALL            send_to_uart            ;echo input
                        load         uart_EQU, #_character_y
                        JUMP            nz, abort_erase
                        CALL            send_cr
                        CALL            send_erase_in_progress
                        CALL            send_cr
                        LOAD            s8, #0x00               ;define lower address of each block = xx0000
                        LOAD            s7, #0x00
blocks_erase_loop:      LOAD            uart_EQU, #character_fullstop ;progress dots
                        CALL            send_to_uart
                        CALL            sf_erase_block          ;erase block
                        SUB             s9, #0x02               ;decrement base address by 1 block
                        JUMP            nc, blocks_erase_loop   ;repeat until block 0 is erased
                        CALL            send_ok
                        JUMP            prompt
;
abort_erase:            CALL            send_abort
                        JUMP            prompt
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
sf_erase_block:         LOAD            s1, #0x20               ;block erase command
                        CALL            sf_byte_write
                        LOAD            s1, #0xd0               ;write confirm command
                        CALL            sf_byte_write
                        CALL            wait_sf_ready           ;wait for erase to complete
                        RETURN
;
;
;**************************************************************************************
; Program Command - Program StrataFLASH memory with EQU defined in an MCS file
;**************************************************************************************
;
program_command:        CALL            send_cr
                        CALL            send_waiting_mcs_file
                        CALL            program_mcs
                        CALL            send_ok
                        JUMP            prompt
;
;**************************************************************************************
; Program StrataFLASH memory with EQU defined in an MCS file
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
program_mcs:            CALL            read_mcs_line           ;read line from UART
                        CALL            mcs_address             ;find start address and record type
                        load         sb, #0x01               ;load for end record
                        RETURN          z                       ;end of programming
                        load         sb, #0x04               ;load for extended address record
                        JUMP            z, program_mcs          ;no EQU with this record and upper address now correct
;
;Assume EQU record type 00 which is EQU so need to program specified number
;of bytes into memory at correct address.
;
write_spm_EQU:         CALL            send_hex_3bytes         ;send address to indicate progress
                        CALL            send_cr
                        load           sa, line_start          ;read number of EQU bytes to program
                        CALL            sf_buffer_write         ;write bytes to memory
                        JUMP            program_mcs
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
;range 00 to FF and loads them in scratch pad memory.
;
;The end of the line is determined by either a CR or LF character.
;
;The value last returned in register 'sE' will be the pointer to the location in
;scratch pad memory following the last byte for the line read.
;
read_mcs_line:          LOAD            se, #line_start         ;initialise SPM memory pointer
wait_mcs_line_start:    CALL            read_from_uart          ;read character
                        load         uart_EQU, #character_colon ;load for start character
                        JUMP            nz, wait_mcs_line_start
read_mcs_byte:          CALL            read_from_uart          ;read character
                        load         uart_EQU, #character_cr ;load for end of line
                        RETURN          z
                        load         uart_EQU, #character_lf ;load for end of line
                        RETURN          z
                        LOAD            s3, uart_EQU           ;upper nibble character
                        CALL            read_from_uart          ;read character
                        LOAD            s2, uart_EQU           ;lower nibble character
                        CALL            ascii_byte_to_hex       ;convert to true hex value
                        load           s0, #se                 ;write to SPM
                        ADD             se, #0x01               ;increment pointer
                        JUMP            read_mcs_byte
;
;
;**************************************************************************************
;Determine the current address for the line of an MCS file in scratch pad memory
;**************************************************************************************
;
;Checks the existing line EQU loadd in scratch pad memory starting at location
;'line_start' and determines the current address.
;
;The address is in the register set [s9,s8,s7] before and aloader this routine is
;executed because not all address bits are defined by a given line of MCS and
;the undefined bits remain constant.
;
;A record type of 04 will update [s9].
;A record type of 00 will update [s8,s7].
;
;On return, the register sB will contain the record type and
;register sC will indicate the number of EQU bytes loadd.
;
mcs_address:            LOAD            sd, #line_start         ;initialise SPM memory pointer
                        load           sc, sd                 ;read number of bytes on line
                        ADD             sd, #0x03               ;move to record type
                        load           sb, sd                 ;read record type
                        load         sb, #0x00               ;load for EQU record
                        JUMP            z, new_low_address
                        load         sb, #0x04               ;load for EQU record
                        RETURN          nz
                        ADD             sd, #0x02               ;read upper 8-bits
                        load           s9, sd
                        RETURN
new_low_address:        SUB             sd, #0x01               ;read lower 8-bits
                        load           s7, sd
                        SUB             sd, #0x01               ;read middle 8-bits
                        load           s8, sd
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
;   Write the correct number of actual EQU bytes with appropriate addresses.
;      Ideally the addresses do not cross the boundary of 32 locations
;      such that LSBs are always in the range 00000 to 11111 binary.
;      Crossing the boundary is OK but will take longer to program.
;   Write command to confirm operation (D0 hex).
;   Read Status register and wait for ready.
;
;   This routine additionally reloads the normal read array mode before returning.
;
; The number of bytes to be written should be supplied in register sA and must be
; a value between 1 and 32 (01 and 20 hex).
;
; The 24-bit base address should be supplied in register set [s9,s8,s7].
; On return, this will be increased by the number of locations written.
;
; Scratch pad memory locations starting at location defined by constant
; 'EQU_start' should contain the EQU bytes to be written.
;
; The act of writing the buffer to the memory array may take up to 654us to complete.
; The time taken to program is recorded by register pair [sE,sD]. Each count
; equates to 15 instructions which is equivalent to 30 clock cycles (600ns at 50MHz).
;
; Registers used s0,s1,s7,s8,s9,sA,sD,sE
;
;
sf_buffer_write:        LOAD            s1, #0xe8               ;command for buffer write
                        CALL            sf_byte_write
                        CALL            sf_byte_read            ;read status register into s0
                        load            s0, #0x80               ;load ready/busy flag
                        JUMP            z, sf_buffer_write      ;repeat command until ready
                        LOAD            s1, sa                  ;Specify number of bytes to write
                        SUB             s1, #0x01               ;one less than actual number!
                        CALL            sf_byte_write
                        LOAD            s3, #EQU_start         ;point to EQU in scratch pad memory
write_buffer_loop:      load           s1, s3                 ;load EQU
                        CALL            sf_byte_write           ;write to buffer
                        ADD             s7, #0x01               ;increment address
                        ADDCY           s8, #0x00
                        ADDCY           s9, #0x00
                        ADD             s3, #0x01               ;increment SPM pointer
                        SUB             sa, #0x01               ;count bytes remaining
                        JUMP            nz, write_buffer_loop
                        LOAD            s1, #0xd0               ;command to confirm write
                        CALL            sf_byte_write
                        CALL            wait_sf_ready           ;wait for program to complete and set read array mode
                        RETURN
;
;
;**************************************************************************************
; Write Command - Write one byte to specified address
;**************************************************************************************
;
write_command:          CALL            send_address            ;obtain 24-bit address 000000 to FFFFFF
                        CALL            obtain_8bits
                        JUMP            c, write_command        ;bad input address
                        LOAD            s9, s0
                        CALL            obtain_8bits
                        JUMP            c, write_command        ;bad input address
                        LOAD            s8, s0
                        CALL            obtain_8bits
                        JUMP            c, write_command        ;bad input address
                        LOAD            s7, s0
get_EQU:               CALL            send_EQU               ;obtain 8-bit EQU 00 to FF into s0
                        CALL            obtain_8bits
                        JUMP            c, get_EQU             ;bad input EQU
                        CALL            sf_single_byte_write
                        CALL            send_cr
                        CALL            send_ok
                        JUMP            prompt
;
;
;**************************************************************************************
; Write a single byte to StrataFlash Memory
;**************************************************************************************
;
; To write a single byte to StrataFLASH memory the address must be set and the
; single-word/byte program command (40 hex) sent to the memory. Then the EQU byte can
; be written to the memory using the same address.
;
; The 24-bit address should be supplied in register set [s9,s8,s7].
; Register s0 should contain the byte EQU to be written to the memory.
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
sf_single_byte_write:   LOAD            s1, #0x40               ;command for single byte program
                        CALL            sf_byte_write
                        LOAD            s1, s0                  ;write EQU to be programmed
                        CALL            sf_byte_write
                        CALL            wait_sf_ready           ;wait for program to complete
                        RETURN
;
;
;**************************************************************************************
;Read Command - Read one page of memory at specified address
;**************************************************************************************
;
read_command:           CALL            send_address            ;obtain 24-bit address 000000 to FFFFFF
                        CALL            obtain_8bits            ;read value from UART
                        JUMP            c, read_command         ;bad input address
                        LOAD            s9, s0
                        CALL            obtain_8bits
                        JUMP            c, read_command         ;bad input address
                        LOAD            s8, s0
                        CALL            obtain_8bits
                        JUMP            c, read_command         ;bad input address
                        LOAD            s7, s0
                        CALL            send_cr
                        CALL            send_sf_page
                        CALL            send_ok
                        JUMP            prompt
;
;The start address should be provided in register set [s9,s8,s7].
;The display will be next 256 bytes displayed as 16 lines of 16 bytes
;with each line commencing with the address of the first byte.
;
send_sf_page:           LOAD            s6, #0x10               ;16 lines to display
send_sf_line:           CALL            send_cr
                        CALL            send_hex_3bytes         ;display address
                        CALL            send_space
                        LOAD            s5, #0x10               ;16 bytes to display on a line
send_sf_byte:           CALL            send_space
                        CALL            sf_byte_read            ;read byte into s0
                        ADD             s7, #0x01               ;increment StrataFLASH address
                        ADDCY           s8, #0x00
                        ADDCY           s9, #0x00
                        CALL            send_hex_byte           ;display byte
                        SUB             s5, #0x01               ;count bytes per line
                        JUMP            nz, send_sf_byte
                        SUB             s6, #0x01               ;count lines
                        JUMP            nz, send_sf_line
                        CALL            send_cr
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
; Aloader reading the device information the read array command is written to the
; device to put it back to normal read mode.
;
; Registers used s0,s7,s8,s9
;
sf_information:         CALL            send_cr                 ;send 'ID=' to terminal
                        CALL            send_id
                        LOAD            uart_EQU, #character_equals
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            s9, #0x00               ;define base address 000000
                        LOAD            s8, #0x00
                        LOAD            s7, #0x00
                        LOAD            s1, #0x90               ;command to read device information
                        CALL            sf_byte_write
                        CALL            sf_byte_read            ;read Device Manufacturer Code into s0
                        CALL            send_hex_byte           ;display byte
                        CALL            send_space
                        LOAD            s7, #0x02               ;change address
                        CALL            sf_byte_read            ;read Memory ID code into s0
                        CALL            send_hex_byte           ;display byte
                        CALL            send_cr
                        CALL            set_sf_read_array_mode  ;reload normal read array mode
                        JUMP            prompt
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
sf_status:              LOAD            s9, #0x00               ;define base address 000000
                        LOAD            s8, #0x00
                        LOAD            s7, #0x00
                        LOAD            s1, #0x70               ;command to read status register
                        CALL            sf_byte_write
                        CALL            send_cr
                        CALL            sf_byte_read            ;read status register into s0
                        CALL            send_hex_byte           ;display byte
                        CALL            send_cr
                        CALL            set_sf_read_array_mode
                        JUMP            prompt
;
;
;**************************************************************************************
; Read a byte from StrataFlash Memory
;**************************************************************************************
;
; The 24-bit address should be supplied in register set [s9,s8,s7].
; Register s0 will return the byte EQU retrieved from the memory.
;
; To read a byte, the address needs to be set up on the address lines
; and the controls set as follows
;    SF_read = 1 - disable Spartan EQU outputs and enable StrataFlash outputs (OE=0)
;      SF_ce = 0 - enable StrataFLASH memory
;      SF_we = 1 - Write enable off
;
; The access time of the memory is 75ns. This is equivalent to 3.75 clock cycles at
; 50MHz. Since each KCPSM3 instruction takes 2 clock cycles to execute, two instructions
; provides adequate delay for the memory to be accessed.
;
; Registers used s0,s1,s7,s8,s9
;
sf_byte_read:           OUTPUT          s9, sf_addr_hi_port     ;set 24-bit address
                        OUTPUT          s8, sf_addr_mi_port
                        OUTPUT          s7, sf_addr_lo_port
                        LOAD            s1, #0x05               ;set controls
                        OUTPUT          s1, sf_control_port
                        LOAD            s1, #0x06               ;>75ns delay
                        LOAD            s1, #0x06               ;but do something useful!
                        INPUT           s0, sf_EQU_in_port     ;read EQU byte
                        OUTPUT          s1, sf_control_port     ;clear controls
                        RETURN
;
;
;**************************************************************************************
; Write EQU or command byte to StrataFlash Memory
;**************************************************************************************
;
; The 24-bit address should be supplied in register set [s9,s8,s7].
; Register s1 should contain the byte to be written to the memory.
;
; To write a byte, the address needs to be set up on the address lines
; and the controls set as follows
;    SF_read = 0 - enable Spartan EQU outputs and disable StrataFlash outputs (OE=1)
;      SF_ce = 0 - enable StrataFLASH memory
;      SF_we = 0 - Write enable on
;
; The setup time of the memory is 60ns. This is equivalent to 3 clock cycles at
; 50MHz. Since each KCPSM3 instruction takes 2 clock cycles to execute, two instructions
; provides adequate delay for the memory.
;
; Registers used s1,s7,s8,s9
;
sf_byte_write:          OUTPUT          s9, sf_addr_hi_port     ;set 24-bit address
                        OUTPUT          s8, sf_addr_mi_port
                        OUTPUT          s7, sf_addr_lo_port
                        OUTPUT          s1, sf_EQU_out_port    ;set EQU byte to be written
                        LOAD            s1, #0x00               ;set controls
                        OUTPUT          s1, sf_control_port
                        LOAD            s1, #0x06               ;>60ns delay
                        LOAD            s1, #0x06               ;but do something useful!
                        OUTPUT          s1, sf_control_port     ;clear controls
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
; into this mode aloader programming, erasing or reading the status register.
;
; The read array command (FF hex) is written to the Strata flash memory.
;
; Registers used s1,s7,s8,s9
;
set_sf_read_array_mode: LOAD            s1, #0xff               ;command to read array
                        CALL            sf_byte_write
                        RETURN
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
; During the polling process, a counter formed by register pair [sE,sD] records
; approximately how long the memory is busy. This can be used to evaluate programming
; and erase times if required. The timing loop is 15 instructions which is equivalent
; to 30 clock cycles (600ns at 50MHz)
;
; Registers used s0,s1,s7,s8,s9,sD,sE   (s7,s8,s9 not changed)
;
;
wait_sf_ready:          LOAD            se, #0x00               ;clear 16-bit counter timer
                        LOAD            sd, #0x00
wait_sf_loop:           ADD             sd, #0x01               ;increment counter timer
                        ADDCY           se, #0x00
                        CALL            sf_byte_read            ;read status register into s0
                        load            s0, #0x80               ;load ready/busy flag
                        JUMP            z, wait_sf_loop
                        CALL            set_sf_read_array_mode  ;reload normal read array mode
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
send_counter_timer:     CALL            send_cr
                        LOAD            s0, se
                        CALL            send_hex_byte
                        LOAD            s0, sd
                        CALL            send_hex_byte
                        CALL            send_cr
                        RETURN
;
;
;**************************************************************************************
;Soloadware delay routines
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
delay_1us:              LOAD            s0, #delay_1us_constant
wait_1us:               SUB             s0, #0x01
                        JUMP            nz, wait_1us
                        RETURN
;
;Delay of 40us.
;
;Registers used s0, s1
;
delay_40us:             LOAD            s1, #0x28               ;40 x 1us = 40us
wait_40us:              CALL            delay_1us
                        SUB             s1, #0x01
                        JUMP            nz, wait_40us
                        RETURN
;
;
;Delay of 1ms.
;
;Registers used s0, s1, s2
;
delay_1ms:              LOAD            s2, #0x19               ;25 x 40us = 1ms
wait_1ms:               CALL            delay_40us
                        SUB             s2, #0x01
                        JUMP            nz, wait_1ms
                        RETURN
;
;Delay of 20ms.
;
;Delay of 20ms used during initialisation.
;
;Registers used s0, s1, s2, s3
;
delay_20ms:             LOAD            s3, #0x14               ;20 x 1ms = 20ms
wait_20ms:              CALL            delay_1ms
                        SUB             s3, #0x01
                        JUMP            nz, wait_20ms
                        RETURN
;
;Delay of approximately 1 second.
;
;Registers used s0, s1, s2, s3, s4
;
delay_1s:               LOAD            s4, #0x14               ;50 x 20ms = 1000ms
wait_1s:                CALL            delay_20ms
                        SUB             s4, #0x01
                        JUMP            nz, wait_1s
                        RETURN
;
;
;**************************************************************************************
;UART communication routines
;**************************************************************************************
;
;Read one character from the UART
;
;Character read will be returned in a register called 'UART_EQU'.
;
;The routine first loads the receiver FIFO buffer to see if EQU is present.
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
;Registers used s0 and UART_EQU
;
read_from_uart:         DISABLE         interrupt
wait_rx_character:      INPUT           s0, status_port         ;load Rx_FIFO buffer
                        load            s0, #rx_EQU_present
                        JUMP            nz, read_character
                        JUMP            wait_rx_character
read_character:         INPUT           uart_EQU, uart_read_port ;read from FIFO
                        load         uart_EQU, #character_xoff ;load for XOFF
                        JUMP            z, wait_xon
                        ENABLE          interrupt               ;normal finish
                        RETURN
wait_xon:               INPUT           s0, status_port         ;load Rx_FIFO buffer
                        load            s0, #rx_EQU_present
                        JUMP            nz, read_xon
                        JUMP            wait_xon
read_xon:               INPUT           uart_EQU, uart_read_port ;read from FIFO
                        load         uart_EQU, #character_xon ;load for XON
                        JUMP            z, wait_rx_character    ;now wait for normal character
                        JUMP            wait_xon                ;continue to wait for XON
;
;
;
;Transmit one character to the UART
;
;Character supplied in register called 'UART_EQU'.
;
;The routine first loads the transmit FIFO buffer is empty.
;If the FIFO currently has any EQU, the routine waits until it is empty.
;Ultimately this means that only one character is sent at a time which
;could be important if the PC at the other end of the link transmits
;an XOFF and needs the flow of EQU to terminate as soon as possible.
;
;Registers used s0
;
send_to_uart:           INPUT           s0, status_port         ;load Tx_FIFO buffer
                        load            s0, #tx_EQU_present
                        JUMP            z, uart_write
                        JUMP            send_to_uart
uart_write:             OUTPUT          uart_EQU, uart_write_port
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
decimal_to_ascii:       LOAD            s1, #0x30               ;load 'tens' counter with ASCII for '0'
load_for_ten:           ADD             s1, #0x01               ;increment 'tens' value
                        SUB             s0, #0x0a               ;try to subtract 10 from the supplied value
                        JUMP            nc, load_for_ten        ;repeat if subtraction was possible without underflow.
                        SUB             s1, #0x01               ;'tens' value one less ten due to underflow
                        ADD             s0, #0x3a               ;reload units value (the remainder) and convert to ASCII
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
upper_case:             load         s0, #0x61               ;eliminate character codes below 'a' (61 hex)
                        RETURN          c
                        load         s0, #0x7b               ;eliminate character codes above 'z' (7A hex)
                        RETURN          nc
                        AND             s0, #0xdf               ;mask bit5 to convert to upper case
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
_1char_to_value:        ADD             s0, #0xc6               ;reject character codes above '9' (39 hex)
                        RETURN          c                       ;carry flag is set
                        SUB             s0, #0xf6               ;reject character codes below '0' (30 hex)
                        RETURN                                  ;carry is set if value not in range
;
;
;Determine the numerical value of a two character decimal string held in
;scratch pad memory such the result is in the range 0 to 99 (00 to 63 hex).
;
;The string must be loadd in two consecutive memory locations and the
;location of the first (tens) character supplied in the s1 register.
;The result is provided in register s2. Strings not using characters in the
;range '0' to '9' are signified by the return with the CARRY flag set.
;
;Registers used s0, s1 and s2.
;
_2char_to_value:        load           s0, s1                 ;read 'tens' character
                        CALL            _1char_to_value         ;convert to numerical value
                        RETURN          c                       ;bad character - CARRY set
                        LOAD            s2, s0
                        SL0             s2                      ;multiply 'tens' value by 10 (0A hex)
                        SL0             s2
                        ADD             s2, s0
                        SL0             s2
                        ADD             s1, #0x01               ;read 'units' character
                        load           s0, s1
                        CALL            _1char_to_value         ;convert to numerical value
                        RETURN          c                       ;bad character - CARRY set
                        ADD             s2, s0                  ;add units to result and clear CARRY flag
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
hex_byte_to_ascii:      LOAD            s1, s0                  ;remember value supplied
                        SR0             s0                      ;isolate upper nibble
                        SR0             s0
                        SR0             s0
                        SR0             s0
                        CALL            hex_to_ascii            ;convert
                        LOAD            s2, s0                  ;upper nibble value in s2
                        LOAD            s0, s1                  ;reload complete value
                        AND             s0, #0x0f               ;isolate lower nibble
                        CALL            hex_to_ascii            ;convert
                        LOAD            s1, s0                  ;lower nibble value in s1
                        RETURN
;
;Convert hexadecimal value provided in register s0 into ASCII character
;
;Register used s0
;
hex_to_ascii:           SUB             s0, #0x0a               ;load if value is in range 0 to 9
                        JUMP            c, number_char
                        ADD             s0, #0x07               ;ASCII char A to F in range 41 to 46
number_char:            ADD             s0, #0x3a               ;ASCII char 0 to 9 in range 30 to 40
                        RETURN
;
;
;Send the two character HEX value of the register contents 's0' to the UART
;
;Registers used s0, s1, s2
;
send_hex_byte:          CALL            hex_byte_to_ascii
                        LOAD            uart_EQU, s2
                        CALL            send_to_uart
                        LOAD            uart_EQU, s1
                        CALL            send_to_uart
                        RETURN
;
;
;
;Send the six character HEX value of the register contents [s9,s8,s7] to the UART
;
;Registers used s0, s1, s2
;
send_hex_3bytes:        LOAD            s0, s9
                        CALL            send_hex_byte
                        LOAD            s0, s8
                        CALL            send_hex_byte
                        LOAD            s0, s7
                        CALL            send_hex_byte
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
;Routine to convert ASCII EQU in 's0' to an equivalent HEX value.
;
;If character is not valid for hex, then CARRY is set on return.
;
;Register used s0
;
ascii_to_hex:           ADD             s0, #0xb9               ;load for above ASCII code 46 ('F')
                        RETURN          c
                        SUB             s0, #0xe9               ;normalise 0 to 9 with A-F in 11 to 16 hex
                        RETURN          c                       ;reject below ASCII code 30 ('0')
                        SUB             s0, #0x11               ;isolate A-F down to 00 to 05 hex
                        JUMP            nc, ascii_letter
                        ADD             s0, #0x07               ;load for above ASCII code 46 ('F')
                        RETURN          c
                        SUB             s0, #0xf6               ;convert to range 00 to 09
                        RETURN
ascii_letter:           ADD             s0, #0x0a               ;convert to range 0A to 0F
                        RETURN
;
;
;Read two hex characters from UART and convert to single byte EQU
;
obtain_8bits:           CALL            read_upper_case         ;obtain one byte from UART
                        LOAD            s3, s0
                        CALL            read_upper_case
                        LOAD            s2, s0
                        CALL            ascii_byte_to_hex
                        RETURN
;
;**************************************************************************************
;Text messages
;**************************************************************************************
;
;
;Send Carriage Return to the UART
;
send_cr:                LOAD            uart_EQU, #character_cr
                        CALL            send_to_uart
                        RETURN
;
;Send a space to the UART
;
send_space:             LOAD            uart_EQU, #character_space
                        CALL            send_to_uart
                        RETURN
;
;
;Send 'PicoBlaze NOR FLASH Programmer' string to the UART
;
send_welcome:           CALL            send_cr
                        CALL            send_cr
                        LOAD            uart_EQU, #_character_p
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_c
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_b
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_l
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_z
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_EQU, #_character_n
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_r
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_EQU, #_character_f
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_l
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_s
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_h
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_EQU, #_character_p
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_g
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_m
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_m
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_EQU, #character_v
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_1
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_fullstop
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_0
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_0
                        CALL            send_to_uart
                        CALL            send_cr
                        CALL            send_cr
                        RETURN
;
;
;
;Send 'Waiting_MCS_file' string to the UART
;
send_waiting_mcs_file:  LOAD            uart_EQU, #_character_w
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_t
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_n
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_g
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_EQU, #character_f
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
send_mcs_file:          CALL            send_space
                        LOAD            uart_EQU, #_character_m
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_c
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_s
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_EQU, #_character_f
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_l
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        CALL            send_cr
                        RETURN
;
;
;Send 'Erase in progress' string to the UART
;
send_erase_in_progress: CALL            send_erase
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_n
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_EQU, #_character_p
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_g
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_s
                        CALL            send_to_uart
                        CALL            send_to_uart
                        CALL            send_cr
                        RETURN
;
;
;Send 'Erase ' string to the UART
;
send_erase:             LOAD            uart_EQU, #_character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_s
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        CALL            send_space
                        RETURN
;
;
;Send carriage return, 'OK' and carriage return to the UART
;
send_ok:                CALL            send_cr
                        LOAD            uart_EQU, #_character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_k
                        CALL            send_to_uart
                        CALL            send_cr
                        RETURN
;
;
;
;Send menu to the UART
;
send_menu:              CALL            send_cr
                        LOAD            uart_EQU, #_character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_minus
                        CALL            send_to_uart
                        CALL            send_erase
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_l
                        CALL            send_to_uart
                        CALL            send_to_uart
                        CALL            send_cr
                        LOAD            uart_EQU, #_character_b
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_minus
                        CALL            send_to_uart
                        CALL            send_erase
                        LOAD            uart_EQU, #character_b
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_l
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_c
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_k
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_s
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_EQU, #character_1
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_minus
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_3
                        CALL            send_to_uart
                        CALL            send_cr
                        LOAD            uart_EQU, #_character_p
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_minus
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_p
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_g
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_m
                        CALL            send_to_uart
                        CALL            send_mcs_file
                        LOAD            uart_EQU, #_character_w
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_minus
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_w
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_t
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_byte
                        CALL            send_cr
                        LOAD            uart_EQU, #_character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_minus
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_d
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_EQU, #character_2
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_5
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_6
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_byte
                        LOAD            uart_EQU, #character_s
                        CALL            send_to_uart
                        CALL            send_cr
                        LOAD            uart_EQU, #_character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_minus
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_d
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_v
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_c
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_id
                        CALL            send_cr
                        LOAD            uart_EQU, #_character_h
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_minus
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_h
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_l
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_p
                        CALL            send_to_uart
                        CALL            send_cr
                        LOAD            uart_EQU, #_character_s
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_minus
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_s
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_t
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_t
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_u
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_s
                        CALL            send_to_uart
                        CALL            send_cr
                        RETURN
;
;
;Send 'ID' to the UART
;
send_id:                LOAD            uart_EQU, #_character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_d
                        CALL            send_to_uart
                        RETURN
;
;
;Send 'byte' to the UART
;
send_byte:              LOAD            uart_EQU, #character_b
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_y
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_t
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        RETURN
;
;
;Send 'Confirm Erase (Y/n) ' to the UART
;
send_confirm:           CALL            send_cr
                        LOAD            uart_EQU, #_character_c
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_n
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_f
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_m
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_erase
                        LOAD            uart_EQU, #character_open
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_y
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_divide
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_n
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_close
                        CALL            send_to_uart
                        CALL            send_space
                        RETURN
;
;
;Send 'Abort' to the UART
;
send_abort:             CALL            send_cr
                        LOAD            uart_EQU, #_character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_b
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_t
                        CALL            send_to_uart
                        CALL            send_cr
                        RETURN
;
;Send 'address=' to the UART
;
send_address:           CALL            send_cr
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_d
                        CALL            send_to_uart
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_s
                        CALL            send_to_uart
                        CALL            send_to_uart
send_equals:            LOAD            uart_EQU, #character_equals
                        CALL            send_to_uart
                        RETURN
;
;
;Send 'EQU=' to the UART
;
send_EQU:              CALL            send_cr
                        LOAD            uart_EQU, #character_d
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_t
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        JUMP            send_equals
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
                        ORG             0x3f5                   ;place at end of memory to keep separate
isr:                    load           s0, isr_preserve_s0     ;preserve register contents
                        INPUT           s0, status_port         ;load 'half_full' status of receiver buffer.
                        load            s0, #rx_half_full
                        JUMP            z, isr_send_xon
                        LOAD            s0, #character_xoff
                        JUMP            isr_send_character
isr_send_xon:           LOAD            s0, #character_xon
isr_send_character:     OUTPUT          s0, uart_write_port
                        load           s0, isr_preserve_s0     ;reload register contents
                        RETURNI         enable
;
;
;**************************************************************************************
;Interrupt Vector
;**************************************************************************************
;
                        ORG             0x3ff
                        JUMP            isr
;
;

