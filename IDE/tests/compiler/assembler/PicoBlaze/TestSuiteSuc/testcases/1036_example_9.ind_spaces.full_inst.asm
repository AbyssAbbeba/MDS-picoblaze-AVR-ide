; KCPSM3 Program - Implementation of the SHA-1 algorithm for use with the
;                  DS2432 secure memory on the Spartan-3E Starter Kit.
device kcpsm1
; Ken Chapman - Xilinx Ltd
;

; Version v1.00 - 19th April 2006
;
;
; IMPORTANT - This design builds on the reference design called "PicoBlaze
;             DS2432 communicator". It is highly recommend that you look at that
;             design before proceeding with this one.
;
;
; This program uses a 9600 baud UART connection to allow communication with the
; 1-wire interface of the DS2432 memory device from Dallas Semiconductor.
;
; The program only supports a limited number of the DS2432 commands to focus on
; those aspects which use the SHA-1 algorithm.
;
; Note that the code performing the SHA-1 algorithm interacts with the hardware of
; this complete reference design. The hardware provides a 16 word (32-bit) buffer
; combined used in the initialisation of the algorithm and subsequent computation
; of the Wt words.
;
;
; The DS2432 should be programmed with a 64-bit secret. The following constants
; define the secret which will be used. Obviously this would be be changed in a
; real application and further measures taken to prevent it easily being found.
; The secret is 64-bits formed of 8 bytes. 'secret0' would be stored at address
; 0080 of the DS2432 and 'secret7' at address 0087. The write buffer and load
; first secret commands allow you to set any secret into the DS2432 device but
; this program always uses the secret defined in these constants such that you can
; experiment with secrets which do and do not match.
;
;
secret0                 EQU             0x01
secret1                 EQU             0x23
secret2                 EQU             0x45
secret3                 EQU             0x67
secret4                 EQU             0x89
secret5                 EQU             0xab
secret6                 EQU             0xcd
secret7                 EQU             0xef
;
;
; Bytes 4, 5 and 6 of the DS2432 scratch pad memory are used in the SHA-1 algorithm.
; These should be set using the write scratchpad memory command before using the
; read authenticated page command. HOWEVER, it is also important that you also use
; the read scratchpad command BEFORE using the read authenticated page command. This
; is because this program only copies the bytes 4, 5 and 6 during a read such that
; they are can be used by the PicoBlaze SHA-1 algorithm. This limitation is deliberate
; so that you can experiment and prove that the SHA-1 results will not match if
; the same 'challenge' bytes are not used.
;
;
;**************************************************************************************
; Port definitions
;**************************************************************************************
;
;
status_port             EQU             0x40                    ;UART status input
tx_half_full            EQU             0x01                    ;  Transmitter     half full - bit0
tx_full                 EQU             0x02                    ;    FIFO               full - bit1
rx_data_present         EQU             0x04                    ;  Receiver     data present - bit2
rx_half_full            EQU             0x08                    ;    FIFO          half full - bit3
rx_full                 EQU             0x10                    ;                   full - bit4
spare1                  EQU             0x20                    ;                  spare '0' - bit5
spare2                  EQU             0x40                    ;                  spare '0' - bit6
spare3                  EQU             0x80                    ;                  spare '0' - bit7
;
uart_read_port          EQU             0x80                    ;UART Rx data input
;
uart_write_port         EQU             0x04                    ;UART Tx data output
;
;
ds_wire_in_port         EQU             0xc0                    ;Read signal from DS2432 device
ds_wire_out_port        EQU             0x08                    ;Drive signal to DS2432 device (open collector)
ds_wire                 EQU             0x01                    ;       Signal is bit0 in both cases
;
;
;
; The following ports access the 'Wt' word buffer. This buffer holds 16 words
; of 32-bits organised as a 64-byte shift register. Hence each word is stored
; by writing 4 bytes. As each byte is written, all bytes shift along such that
; older Wt values can be read from consistent port addresses.
;
w_word_write_port       EQU             0x10                    ;Write byte to Wt buffer
;
wt_minus3_byte0_read_port EQU           0x08                    ;Read of Wt-3
wt_minus3_byte1_read_port EQU           0x09
wt_minus3_byte2_read_port EQU           0x0a
wt_minus3_byte3_read_port EQU           0x0b
;
wt_minus8_byte0_read_port EQU           0x1c                    ;Read of Wt-8
wt_minus8_byte1_read_port EQU           0x1d
wt_minus8_byte2_read_port EQU           0x1e
wt_minus8_byte3_read_port EQU           0x1f
;
wt_minus14_byte0_read_port EQU          0x34                    ;Read of Wt-14
wt_minus14_byte1_read_port EQU          0x35
wt_minus14_byte2_read_port EQU          0x36
wt_minus14_byte3_read_port EQU          0x37
;
wt_minus16_byte0_read_port EQU          0x3c                    ;Read of Wt-16
wt_minus16_byte1_read_port EQU          0x3d
wt_minus16_byte2_read_port EQU          0x3e
wt_minus16_byte3_read_port EQU          0x3f
;
;
;**************************************************************************************
; Special Register usage
;**************************************************************************************
;
uart_data               REG             sf                      ;used to pass data to and from the UART
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
family_code             EQU             0x00
serial_number0          EQU             0x01                    ;48-bit serial number LS-Byte first
serial_number1          EQU             0x02
serial_number2          EQU             0x03
serial_number3          EQU             0x04
serial_number4          EQU             0x05
serial_number5          EQU             0x06
read_rom_crc            EQU             0x07                    ;8-bit CRC
;
;
; Locations for variables used in SHA-1 algorithm.
; Each variable is 32-bits and requires 4 bytes to store.
; '0' indicates the least significant byte and '3' the most significant byte.
;
; Note that the concatenation of 'A', 'B', 'C', 'D' and 'E' will be the 160-bit MAC.
;
var_a0                  EQU             0x08                    ;Variable 'A'
var_a1                  EQU             0x09
var_a2                  EQU             0x0a
var_a3                  EQU             0x0b
;
var_b0                  EQU             0x0c                    ;Variable 'B'
var_b1                  EQU             0x0d
var_b2                  EQU             0x0e
var_b3                  EQU             0x0f
;
var_c0                  EQU             0x10                    ;Variable 'C'
var_c1                  EQU             0x11
var_c2                  EQU             0x12
var_c3                  EQU             0x13
;
var_d0                  EQU             0x14                    ;Variable 'D'
var_d1                  EQU             0x15
var_d2                  EQU             0x16
var_d3                  EQU             0x17
;
var_e0                  EQU             0x18                    ;Variable 'E'
var_e1                  EQU             0x19
var_e2                  EQU             0x1a
var_e3                  EQU             0x1b
;
;
; Copy of data in the scratchpad memory of the DS2432.
; This is only updated by the read scratchpad memory command.
; '0' indicates the data in the least significant location.
;
scratchpad0             EQU             0x1c
scratchpad1             EQU             0x1d
scratchpad2             EQU             0x1e
scratchpad3             EQU             0x1f
scratchpad4             EQU             0x20
scratchpad5             EQU             0x21
scratchpad6             EQU             0x22
scratchpad7             EQU             0x23
;
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
; Initialise the system and welcome message
;**************************************************************************************
;
cold_start:             CALL            ds_wire_init            ;Ensure DS_wire is not driven (pulled High)
                        CALL            delay_1s                ;Allow everything to settle!
welcome_start:          CALL            send_welcome            ;start up message and version number
;
;
;**************************************************************************************
; Reset Main menu and command selection
;**************************************************************************************
;
; The main program allows you to use four of the DS2432 memory and SHA function
; commands. A simple menu is displayed and you are guided to enter more information
; when required. All the communication and protocol required to get the DS2432 ready
; to receive memory and SHA function commands has been automated although information
; is displayed to indicate the procedures being executed.
;
; Before any memory and function commands are available a master reset and read ROM
; command must be issued.
;
warm_start:             CALL            send_cr
                        CALL            send_cr
                        CALL            ds_init_regular_mode    ;master reset
                        JUMP            c, warm_start           ;repeat reset if no presence pulse detected
                        CALL            read_rom_command        ;read ROM command and display results
;
; After a valid ROM command the DS2432 specific memory commands and SHA-1
; functions become accessible. This program assumes that the ROM command did
; 'Pass' so you will need to check yourself. If this program automatically
; reset the DS2432 and tried again and there was a fault it would just cause
; the display to roll continuously and not be very informative!
;
; Each of the DS2432 commands selected from the menu will require the master reset
; and read ROM command to be repeated before being able to proceed with the next
; memory or SHA-1 function. This is automated by the program.
;
;
ds2432_menu:            CALL            send_ds2432_menu        ;Menu and command selection
                        CALL            send_cr
;
ds2432_prompt:          CALL            send_cr                 ;prompt for user input
                        CALL            send_cr
                        LOAD            uart_data, #character_greater_than ;prompt for input
                        CALL            send_to_uart
                        CALL            read_upper_case
                        COMPARE         s0, #character_1        ;test for commands and execute as required
                        JUMP            z, write_scratchpad_command
                        COMPARE         s0, #character_2
                        JUMP            z, read_scratchpad_command
                        COMPARE         s0, #character_3
                        JUMP            z, load_first_secret_command
                        COMPARE         s0, #character_4
                        JUMP            z, read_auth_page_command
                        CALL            send_cr                 ;no valid command input
                        LOAD            uart_data, #character_question ;display ???
                        CALL            send_to_uart
                        CALL            send_to_uart
                        CALL            send_to_uart
                        JUMP            ds2432_prompt           ;Try again!
;
;
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
; 8-bit CRC displaying a 'Pass' or 'Fail' message as appropriate.
;
read_rom_command:       LOAD            s3, #0x33               ;Read ROM Command
                        CALL            write_byte_slow         ;transmit command
                        LOAD            s5, #family_code        ;memory pointer
read_rom_loop:          CALL            read_byte_slow          ;read response into s3
                        STORE           s3, @s5                 ;store value
                        COMPARE         s5, #read_rom_crc       ;8-bytes to read
                        JUMP            z, display_rom
                        ADD             s5, #0x01
                        JUMP            read_rom_loop
display_rom:            CALL            send_cr
                        CALL            send_code               ;'code=' to display family code
                        FETCH           s0, family_code
                        CALL            send_hex_byte
                        CALL            send_cr
                        CALL            send_sn                 ;'s/n=' to display family code
                        LOAD            s5, #serial_number5     ;memory pointer starting MS-byte first
disp_serial_loop:       FETCH           s0, @s5
                        CALL            send_hex_byte
                        COMPARE         s5, #serial_number0
                        JUMP            z, end_serial
                        SUB             s5, #0x01
                        JUMP            disp_serial_loop
end_serial:             CALL            send_cr
                        CALL            send_crc                ;'CRC=' to display CRC value
                        FETCH           s0, read_rom_crc
                        CALL            send_hex_byte
                        CALL            send_cr
                        CALL            compute_crc8            ;compute CRC value in s0
                        FETCH           s1, read_rom_crc        ;compare with received value
                        COMPARE         s0, s1
                        JUMP            nz, crc8_fail
                        CALL            send_pass
                        RETURN
crc8_fail:              CALL            send_fail
                        RETURN
;
;
;
;**************************************************************************************
; DS2432 Load First Secret Command.
;**************************************************************************************
;
; This command will only be valid if the write scratchpad memory command has previously
; been used to define the new secret to be stored at address 0080.
;
; The Load First Secret Command (5A hex) will only copy the scratchpad contents into                           ;
; the EEPROM array of the DS2432 if the address was correctly specified in the
; write scratchpad command. This routine will assume that the address specified
; was 0080. If everything is OK with the programming of the secret, the DS2432 responds
; with 'AA' hex after the command and this routine will report 'Pass'. You can further
; check using a read scratchpad command and look to see if E/S has changed from '5F'
; to 'DF' which indicates the successful write.
;
; Note that this program defines the secret to be used by the PicoBlaze SHA-1 algorithm
; in the constants 'secret0' through to 'secret7'. Only if you program the DS2432
; with a matching secret will the read authenticated message command result in a
; 'Pass' being reported for the MAC. This Load First Secret Command routine deliberately
; does not update the secret used by the PicoBlaze SHA-1 algorithm so that you can
; prove that only a DS2432 with the matching secret will generate matching MAC
; responses.
;
;
;
load_first_secret_command: LOAD         s3, #0x5a               ;Load First Secret Command
                        CALL            write_byte_slow         ;transmit command
                        LOAD            s3, #0x80               ;TA1 value for secret = 80 hex
                        CALL            write_byte_slow
                        LOAD            s3, #0x00               ;TA2 value for secret = 00 hex
                        CALL            write_byte_slow
                        LOAD            s3, #0x5f               ;E/S value before writing = 5F hex
                        CALL            write_byte_slow
                        CALL            delay_20ms              ;write takes place in 10ms
                        CALL            send_cr
                        CALL            send_secret
                        CALL            send_space
                        CALL            read_byte_slow          ;read data into s3
                        COMPARE         s3, #0xaa               ;test response
                        JUMP            z, secret_pass
                        CALL            send_fail
                        JUMP            warm_start
secret_pass:            CALL            send_pass
                        JUMP            warm_start
;
;
;**************************************************************************************
; DS2432 Write Scratchpad Memory Command.
;**************************************************************************************
;
; The write scratchpad memory command (0F hex) allows 8-bytes of data to be written
; together with a target address for final storage in the main memory map. The
; DS2432 scratch pad is also used to define a 3 byte 'challenge' used in the
; SHA-1 algorithm.
;
; The DS2432 provides an initial confirmation of the write by returning a 16-bit CRC
; value which KCPSM3 tests. The CRC is computed based on the command, address and
; data transmitted (11 bytes). PicoBlaze also computes the CRC and and tests this
; against the value received from the DS2432.
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
write_scratchpad_command: CALL          clear_crc16             ;prepare CRC registers [sE,sD]
                        LOAD            s3, #0x0f               ;write scratchpad memory Command
                        CALL            write_byte_slow         ;transmit command
                        CALL            compute_crc16           ;compute CRC for value in 's3'
wsc_addr_loop:          CALL            send_address            ;obtain 16-bit address 0000 to FFFF in [s5,s4]
                        CALL            obtain_8bits
                        JUMP            c, wsc_addr_loop        ;bad input address
                        LOAD            s5, s0
                        CALL            obtain_8bits
                        JUMP            c, wsc_addr_loop        ;bad input address
                        LOAD            s4, s0
                        LOAD            s3, s4                  ;transmit target address TA1 (LS-Byte)
                        CALL            write_byte_slow
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        LOAD            s3, s5                  ;transmit target address TA2 (MS-Byte)
                        CALL            write_byte_slow
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        COMPARE         s5, #0x00               ;check address less than 0090 hex
                        JUMP            nz, warm_start          ;DS2432 aborts command and so do we!
                        COMPARE         s4, #0x90               ;no need to read data bytes.
                        JUMP            nc, warm_start
                        LOAD            s4, #0x00               ;initialise byte counter
wsc_data_loop:          CALL            send_data               ;obtain a byte of data
                        LOAD            uart_data, s4           ;display which byte requested
                        ADD             uart_data, #character_0 ;convert to ASCII
                        CALL            send_to_uart
                        CALL            send_equals
                        CALL            obtain_8bits
                        JUMP            c, wsc_data_loop        ;bad input data
                        LOAD            s3, s0                  ;transmit byte
                        CALL            write_byte_slow
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        ADD             s4, #0x01               ;count bytes
                        COMPARE         s4, #0x08
                        JUMP            nz, wsc_data_loop
                        CALL            send_cr
                        CALL            read_send_test_crc16    ;read, display and test CRC value
                        JUMP            warm_start
;
;
;
;**************************************************************************************
; DS2432 Read Scratchpad Memory Command.
;**************************************************************************************
;
; The read scratchpad memory command (AA hex) allows the 8-bytes of data previously
; to be written into the scratchpad memory to be read back for verification together with
; the target address, a transfer status register (E/S) and a 16-bit CRC value.
;
; The 16-bit CRC is formed of the command byte, address TA1 and TA2, E/S byte and 8 data
; bytes as transmitted (12 bytes). These may not be the same as the values provided
; during a previous write to scratchpad memory. PicoBlaze also computes the CRC and
; and tests this against the value received from the DS2432.
;
; The 8 bytes of data are also copied to PicoBlaze memory at locations defined by the
; constants 'scratchpad0' to 'scratchpad7'. Three bytes are used as a 'challenge'
; by the SHA-1 algorithm.
;
;
;
read_scratchpad_command: CALL           clear_crc16             ;prepare CRC registers [sE,sD]
                        LOAD            s3, #0xaa               ;read scratchpad memory Command
                        CALL            write_byte_slow         ;transmit command
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        CALL            send_address            ;display 'Address='
                        CALL            read_byte_slow          ;read address into [s5,s4]
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        LOAD            s4, s3
                        CALL            read_byte_slow
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        LOAD            s5, s3
                        LOAD            s0, s5                  ;display address
                        CALL            send_hex_byte
                        LOAD            s0, s4
                        CALL            send_hex_byte
                        CALL            send_es                 ;display 'E/S='
                        CALL            read_byte_slow          ;read E/S register
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        LOAD            s0, s3                  ;display value
                        CALL            send_hex_byte
                        CALL            send_data               ;display 'Data='
                        CALL            send_equals
                        LOAD            s4, #scratchpad0        ;pointer to memory and byte counter
rsc_loop:               CALL            send_space
                        CALL            read_byte_slow          ;read data byte
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        STORE           s3, @s4                 ;store value in memory
                        LOAD            s0, s3                  ;display value
                        CALL            send_hex_byte
                        COMPARE         s4, #scratchpad7        ;count bytes
                        JUMP            z, end_rsc_data_loop
                        ADD             s4, #0x01
                        JUMP            rsc_loop
end_rsc_data_loop:      CALL            send_cr
                        CALL            read_send_test_crc16    ;read, display and test CRC value
                        JUMP            warm_start
;
;
;
;
;
;**************************************************************************************
; DS2432 Read Authenticated Page Command.
;**************************************************************************************
;
; The read authenticated page command (A5 hex) allows the 8-byte secret to be tested
; without it actually being read (which would obviously give away the secret!).
;
; This routine has been written to work with page 0 but could easily be changed and
; is documented below. During the first part of the command, the 32 bytes
; contained in the page are read back from the DS2432 and these are used in
; the preparation of the table required for the for SHA-1 algorithm. Other values
; stored in the table are the secret, serial number of the DS2432, family code, some
; constants, 4-bits of the page address and a 3 byte 'challenge' currently set into
; the DS2432 scratchpad memory.
;
; NOTE - The read scratchpad command must be executed before this routine in order
; that the 3 byte 'challenge' of scratchpad memory is known to PicoBlaze.
;
; During this command, two 16-bit CRC values are generated which PicoBlaze also
; computes and tests. The first is formed of the command byte, address TA1 and TA2,
; all the bytes of the page read and an 'FF' byte. The second is formed of the 20
; bytes of the 160-but message authentication code (MAC).
;
;
; Preparing the table.
;
; The table is stored in the external 'Wt' buffer and must first be initialised with the
; 16 'M' words (32-bit words each requiring 4 bytes). This is achieved by shifting in
; each word in sequence. Storing each word most significant byte first is a natural
; fit with the reading of the page data from the DS2432 and the way each 'M' word
; is organised. Notice how this causes least significant bytes to be swapped with most
; significant bytes!
;
;          [31:24]      [23:16]      [15:8]       [7:0]
;
;   M0 = [secret0    , secret1    , secret2    , secret3    ]
;   M1 = [page_data0 , page_data1 , page_data2 , page_data3 ]
;   M2 = [page_data4 , page_data5 , page_data6 , page_data7 ]
;   M3 = [page_data8 , page_data9 , page_data10, page_data11]
;   M4 = [page_data12, page_data13, page_data14, page_data15]
;   M5 = [page_data16, page_data17, page_data18, page_data19]
;   M6 = [page_data20, page_data21, page_data22, page_data23]
;   M7 = [page_data24, page_data25, page_data26, page_data27]
;   M8 = [page_data28, page_data29, page_data30, page_data31]
;   M9 = [   FF      ,    FF      ,    FF      ,    FF      ]
;  M10 = [   40      ,    33      , serial_num0, serial_num1]
;  M11 = [serial_num2, serial_num3, serial_num4, serial_num5]
;  M12 = [secret4    , secret5    , secret6    , secret7    ]
;  M13 = [scratchpad4, scratchpad5, scratchpad6,    80      ]
;  M14 = [   00      ,    00      ,    00      ,    00      ]
;  M15 = [   00      ,    00      ,    01      ,    B8      ]
;
; In M10, the '33' is the family code and the '40' is made up of a constant bit
; pattern '0100' and then bits [8:5] of the page address. This gives 4 possible values
; for this byte during a Read Authenticated Page Command, but this routine is currently
; fixed to work with page 0 only.
;        40 - page 0
;        41 - page 1
;        42 - page 2
;        43 - page 3
;
; M13 contains the 3 byte challenge from the scratch pad memory. This assumes that a
; read scratchpad command has previously been used and the bytes held in the DS2432
; scratchpad match those held in the PicoBlaze memory.
;
;
; The 160-bit Message Authentication Code (MAC) is computed from the table using the SHA-1
; algorithm. This algorithm actually results in 5 variables 'A', 'B', 'C', 'D' and 'E'
; which are 32-bit values each formed of 4 bytes. The MAC is the concatenation of
; the variables. To match the same order in which the Read Authenticated Page Command
; sends the MAC, the variables must be read in the order 'E', 'D', 'C', 'B' and 'A' and
; with the least significant byte of each variable first.
;
;
;
;
;
read_auth_page_command: LOAD            s0, #secret0            ;store M0 (secret 0, 1, 2 and 3) in Wt buffer.
                        OUTPUT          s0, w_word_write_port
                        LOAD            s0, #secret1
                        OUTPUT          s0, w_word_write_port
                        LOAD            s0, #secret2
                        OUTPUT          s0, w_word_write_port
                        LOAD            s0, #secret3
                        OUTPUT          s0, w_word_write_port
;
;Start of DS2432 command
;
                        CALL            clear_crc16             ;prepare CRC registers [sE,sD]
                        LOAD            s3, #0xa5               ;read authenticated page command
                        CALL            write_byte_slow         ;transmit command
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        LOAD            s5, #0x00               ;set address for page 0
                        LOAD            s4, #0x00               ;  [TA2,TA1]=0000 hex
                        LOAD            s3, s4                  ;transmit TA1
                        CALL            write_byte_slow
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        LOAD            s3, s5                  ;transmit TA2
                        CALL            write_byte_slow
                        CALL            compute_crc16           ;compute CRC for value in 's3'
;
;Read 32-bytes of data associated with page 0
;Store these as M1 through to M8
;
rapc_line_loop:         CALL            send_cr
                        LOAD            s0, s5                  ;display 16-bit address
                        CALL            send_hex_byte
                        LOAD            s0, s4
                        CALL            send_hex_byte
                        CALL            send_space
                        CALL            send_space
rapc_data_loop:         CALL            send_space
                        CALL            read_byte_slow          ;read data into s3
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        OUTPUT          s3, w_word_write_port   ;store as 'M' word
                        LOAD            s0, s3                  ;display byte
                        CALL            send_hex_byte
                        ADD             s4, #0x01               ;increment address
                        ADDCY           s5, #0x00
                        TEST            s4, #0x07               ;test for 8-byte boundary
                        JUMP            nz, rapc_data_loop
                        COMPARE         s4, #0x20               ;test for last address
                        JUMP            nz, rapc_line_loop
                        CALL            send_cr
;
;Read one byte that should be value FF hex
;
                        CALL            read_byte_slow          ;read data into s3
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        LOAD            s0, s3                  ;display byte
                        CALL            send_hex_byte
                        CALL            send_cr
                        CALL            read_send_test_crc16    ;read, display and test CRC value
;
;Complete table by stroring M9 through to M15
;
                        LOAD            s0, #0xff               ;W9 = FF FF FF FF
                        LOAD            s1, #0x04
store_w9:               OUTPUT          s0, w_word_write_port
                        SUB             s1, #0x01
                        JUMP            nz, store_w9
;
                        LOAD            s0, #0x40               ;W10 begins with 40 for page 0
                        OUTPUT          s0, w_word_write_port
;
;W10 ends with family code and serial number 0 and 1.
;W11 is formed of serial number 2, 3, 4 and 5.
;All of this information is in PicoBlaze memory having been read by the
;read ROM command.
;
                        LOAD            s1, #family_code        ;pointer to memory
                        LOAD            s2, #0x07               ;7 bytes to read and store
next_m10_m11:           FETCH           s0, @s1
                        OUTPUT          s0, w_word_write_port
                        ADD             s1, #0x01               ;increment pointer
                        SUB             s2, #0x01
                        JUMP            nz, next_m10_m11
;
                        LOAD            s0, #secret4            ;store M12 (secret 4, 5, 6 and 7) in Wt buffer
                        OUTPUT          s0, w_word_write_port
                        LOAD            s0, #secret5
                        OUTPUT          s0, w_word_write_port
                        LOAD            s0, #secret6
                        OUTPUT          s0, w_word_write_port
                        LOAD            s0, #secret7
                        OUTPUT          s0, w_word_write_port
;
                        FETCH           s0, scratchpad4         ;M13 uses scratchpad 4, 5, and 6 and '80' hex
                        OUTPUT          s0, w_word_write_port
                        FETCH           s0, scratchpad5
                        OUTPUT          s0, w_word_write_port
                        FETCH           s0, scratchpad6
                        OUTPUT          s0, w_word_write_port
                        LOAD            s0, #0x80
                        OUTPUT          s0, w_word_write_port
;
                        LOAD            s0, #0x00               ;W14 = 00 00 00 00   W15 = 00 00 01 B8
                        LOAD            s1, #0x06
store_w14_w15:          OUTPUT          s0, w_word_write_port
                        SUB             s1, #0x01
                        JUMP            nz, store_w14_w15
                        LOAD            s0, #0x01
                        OUTPUT          s0, w_word_write_port
                        LOAD            s0, #0xb8
                        OUTPUT          s0, w_word_write_port
;
;Compute the SHA-1 algorithm at the same time that the DS2432 is also computing (2ms).
;
                        CALL            compute_sha1
;
;The 160-bit Message Authentication Code is read from the DS2432 as 20 bytes
;and compared with the concatenation of variables E, D, C, B and A in that order
;with each variable received from the DS2432 least significant byte first.
;Each received byte is also used to form a 16-bit CRC value which is tested to
;reveal any communication errors.
;
;
                        CALL            send_mac                ;display 'mac='
                        CALL            clear_crc16             ;prepare CRC registers [sE,sD]
                        LOAD            sc, #0x00               ;Clear byte match counter
                        LOAD            sb, #var_e0             ;start match with LS-Byte of variable 'E'
mac_match_var:          LOAD            sa, #0x04               ;4 bytes to match in each variable
mac_match_byte:         FETCH           s9, @sb                 ;read variable byte from local SHA-1
                        CALL            read_byte_slow          ;read DS2432 byte into s3
                        CALL            compute_crc16           ;compute CRC for value in 's3'
                        COMPARE         s3, s9                  ;compare MAC values
                        JUMP            nz, display_mac_byte    ;count matching bytes
                        ADD             sc, #0x01               ;decrement match counter
display_mac_byte:       LOAD            s0, s3                  ;display byte
                        CALL            send_hex_byte
                        CALL            send_space
                        SUB             sa, #0x01               ;counts bytes per variable
                        JUMP            z, next_mac_var
                        ADD             sb, #0x01
                        JUMP            mac_match_byte
next_mac_var:           COMPARE         sb, #var_a3             ;test for last byte
                        JUMP            z, report_mac
                        SUB             sb, #0x07               ;point to next variable
                        JUMP            mac_match_var
;
;MAC has passed if all 20 bytes matched
;
report_mac:             CALL            send_cr
                        COMPARE         sc, #0x14               ;20 bytes should have matched
                        JUMP            nz, mac_fail
                        CALL            send_pass
                        JUMP            read_mac_crc
mac_fail:               CALL            send_fail
;
;Next two bytes received are the 16-bit CRC
;Read 16-bit CRC into [s5,s4] and send value to UART
;
read_mac_crc:           CALL            read_send_test_crc16    ;read, display and test CRC value
;
;Read one byte that should be value AA hex.
;  Would actually read AA hex continuously until master reset
;
                        CALL            read_byte_slow          ;read data into s3
                        LOAD            s0, s3                  ;display byte
                        CALL            send_hex_byte
                        CALL            send_cr
;
                        JUMP            warm_start
;
;
;**************************************************************************************
; Compute SHA-1 Algorithm.
;**************************************************************************************
;
; Computes the SHA-1 algorithm based on the initial table of values (M0 through to M15)
; which are stored in the external Wt buffer.
;
; The SHA-1 algorithms uses 5 variables called 'A', 'B', 'C', 'D' and 'E'. Each variable
; is 32-bits and stored as 4 bytes in PicoBlaze scratch pad memory. The locations must
; be defined using constants 'var_A0' thought to 'var_E3' in ascending locations.
;
; Constants must also be used to define access to the external Wt buffer.
;
; During this process, register 'sE' is used to count iterations from 0 to 79 (4F hex).
; Other registers are consistently grouped as follows to support 32-bit operations.
;
; Register set [s5,s4,s3,s2] is used as a temporary 32-bit word
; Register set [s9,s8,s7,s6] is used as a temporary 32-bit word
; Register set [sD,sC,sB,sA] is used as a temporary 32-bit word
;
;
; Initialise the 32-bit variables
;
;
compute_sha1:           LOAD            s0, #0x01               ;A=67452301
                        STORE           s0, var_a0
                        LOAD            s0, #0x23
                        STORE           s0, var_a1
                        LOAD            s0, #0x45
                        STORE           s0, var_a2
                        LOAD            s0, #0x67
                        STORE           s0, var_a3
                        LOAD            s0, #0x89               ;B=EFCDAB89
                        STORE           s0, var_b0
                        LOAD            s0, #0xab
                        STORE           s0, var_b1
                        LOAD            s0, #0xcd
                        STORE           s0, var_b2
                        LOAD            s0, #0xef
                        STORE           s0, var_b3
                        LOAD            s0, #0xfe               ;C=98BADCFE
                        STORE           s0, var_c0
                        LOAD            s0, #0xdc
                        STORE           s0, var_c1
                        LOAD            s0, #0xba
                        STORE           s0, var_c2
                        LOAD            s0, #0x98
                        STORE           s0, var_c3
                        LOAD            s0, #0x76               ;D=10325476
                        STORE           s0, var_d0
                        LOAD            s0, #0x54
                        STORE           s0, var_d1
                        LOAD            s0, #0x32
                        STORE           s0, var_d2
                        LOAD            s0, #0x10
                        STORE           s0, var_d3
                        LOAD            s0, #0xf0               ;E=C3D2E1F0
                        STORE           s0, var_e0
                        LOAD            s0, #0xe1
                        STORE           s0, var_e1
                        LOAD            s0, #0xd2
                        STORE           s0, var_e2
                        LOAD            s0, #0xc3
                        STORE           s0, var_e3
;
;
                        LOAD            se, #0x00               ;reset iteration counter
;
;
;Compute ft(B,C,D) in register set [s5,s4,s3,s2] and then add constant Kt.
;
;Iterations 0 to 19 - process type 1
;   ft = (B and C) or ((not B) and D)
;  Then the constant Kt=5A827999 will be added
;
;Iterations 20 to 39  and iterations 60 to 79  - process type 2
;   ft = B xor C xor D
;  Then the constant Kt=6ED9EBA1 will be added for iterations 20 to 39
;  Then the constant Kt=CA62C1D6 will be added for iterations 60 to 79
;
;Iterations 40 to 59  - process type 3
;   ft = (B and C) or (B and D) or (C and D)
;  Then the constant Kt=8F1BBCDC will be added
;
next_sha1_iteration:    FETCH           s5, var_b3              ;B in [s5,s4,s3,s2]
                        FETCH           s4, var_b2
                        FETCH           s3, var_b1
                        FETCH           s2, var_b0
                        CALL            fetch_c                 ;C in [s9,s8,s7,s6]
                        FETCH           sd, var_d3              ;D in [sD,sC,sB,sA]
                        FETCH           sc, var_d2
                        FETCH           sb, var_d1
                        FETCH           sa, var_d0
;
;Determine process type
;
                        COMPARE         se, #0x14               ;set carry flag for iterations <20
                        JUMP            c, ft_type1
                        COMPARE         se, #0x28               ;set carry flag for iterations <40
                        JUMP            c, ft_type2
                        COMPARE         se, #0x3c               ;set carry flag for iterations <60
                        JUMP            c, ft_type3
;
;   ft = B xor C xor D
;
;       B xor C     =        B       xor       C
;   [s5,s4,s3,s2]   =  [s5,s4,s3,s2] xor [s9,s8,s7,s6]
;
;   B xor C xor D   =    (B xor C)   xor       D
;   [s5,s4,s3,s2]   =  [s5,s4,s3,s2] xor [sD,sC,sB,sA]
;
;
ft_type2:               XOR             s5, s9                  ;B xor C in [s5,s4,s3,s2]
                        XOR             s4, s8
                        XOR             s3, s7
                        XOR             s2, s6
                        XOR             s5, sd                  ;(B xor C) xor D in [s5,s4,s3,s2]
                        XOR             s4, sc
                        XOR             s3, sb
                        XOR             s2, sa
                        COMPARE         se, #0x3c               ;set carry flag for iterations <60
                        JUMP            nc, kt_ca62c1d6
                        ADD             s2, #0xa1               ;add Kt=6ED9EBA1
                        ADDCY           s3, #0xeb
                        ADDCY           s4, #0xd9
                        ADDCY           s5, #0x6e
                        JUMP            compute_tmp
kt_ca62c1d6:            ADD             s2, #0xd6               ;add Kt=CA62C1D6
                        ADDCY           s3, #0xc1
                        ADDCY           s4, #0x62
                        ADDCY           s5, #0xca
                        JUMP            compute_tmp
;
;   ft = (B and C) or ((not B) and D)
;
;       B and C     =        C       and       B
;   [s9,s8,s7,s6]   =  [s9,s8,s7,s6] and [s5,s4,s3,s2]
;
;       not B       =        B       xor   FFFFFFFF
;   [s5,s4,s3,s2]   =  [s5,s4,s3,s2] xor [FF,FF,FF,FF]
;
;   (not B) and D   =    (not B)     and       D
;   [s5,s4,s3,s2]   =  [s5,s4,s3,s2] and [sD,sC,sB,sA]
;
;   ;(B and C) or ((not B) and D)  =  ((not B) and D)  or   (B and C)
;            [s5,s4,s3,s2]         =   [s5,s4,s3,s2]   or  [s9,s8,s7,s6]
;
ft_type1:               AND             s9, s5                  ;B and C in [s9,s8,s7,s6]
                        AND             s8, s4
                        AND             s7, s3
                        AND             s6, s2
                        XOR             s5, #0xff               ;(not B) in [s5,s4,s3,s2]
                        XOR             s4, #0xff
                        XOR             s3, #0xff
                        XOR             s2, #0xff
                        AND             s5, sd                  ;((not B) and D) in [s5,s4,s3,s2]
                        AND             s4, sc
                        AND             s3, sb
                        AND             s2, sa
                        OR              s5, s9                  ;(B and C) or ((not B) and D) in [s5,s4,s3,s2]
                        OR              s4, s8
                        OR              s3, s7
                        OR              s2, s6
                        ADD             s2, #0x99               ;add Kt=5A827999
                        ADDCY           s3, #0x79
                        ADDCY           s4, #0x82
                        ADDCY           s5, #0x5a
                        JUMP            compute_tmp
;
;Routine to fetch variable 'C' into register set [s9,s8,s7,s6]
;
fetch_c:                FETCH           s9, var_c3
                        FETCH           s8, var_c2
                        FETCH           s7, var_c1
                        FETCH           s6, var_c0
                        RETURN
;
;   ft = (B and C) or (B and D) or (C and D)
;
;       B and C     =        C       and       B
;   [s9,s8,s7,s6]   =  [s9,s8,s7,s6] and [s5,s4,s3,s2]
;
;       B and D     =        B       and       D
;   [s5,s4,s3,s2]   =  [s5,s4,s3,s2] and [sD,sC,sB,sA]
;
;  (B and C) or (B and D)   =    (B and D)    or    (B and C)
;      [s5,s4,s3,s2]        =  [s5,s4,s3,s2]  or  [s9,s8,s7,s6]
;
;     read C again into [s9,s8,s7,s6]
;
;       C and D     =        C       and       D
;   [s9,s8,s7,s6]   =  [s9,s8,s7,s6] and [sD,sC,sB,sA]
;
;  ((B and C) or (B and D)) or (C and D)   =    ((B and C) or (B and D)) or   (C and D)
;               [s5,s4,s3,s2]              =           [s5,s4,s3,s2]     or  [s9,s8,s7,s6]
;
ft_type3:               AND             s9, s5                  ;(B and C) in [s9,s8,s7,s6]
                        AND             s8, s4
                        AND             s7, s3
                        AND             s6, s2
                        AND             s5, sd                  ;(B and D) in [s5,s4,s3,s2]
                        AND             s4, sc
                        AND             s3, sb
                        AND             s2, sa
                        OR              s5, s9                  ;(B and C) or (B and D) in [s5,s4,s3,s2]
                        OR              s4, s8
                        OR              s3, s7
                        OR              s2, s6
                        CALL            fetch_c                 ;C in [s9,s8,s7,s6]
                        AND             s9, sd                  ;(C and D) in [s9,s8,s7,s6]
                        AND             s8, sc
                        AND             s7, sb
                        AND             s6, sa
                        OR              s5, s9                  ;(B and C) or (B and D) or (C and D) in [s5,s4,s3,s2]
                        OR              s4, s8
                        OR              s3, s7
                        OR              s2, s6
                        ADD             s2, #0xdc               ;add Kt=8F1BBCDC
                        ADDCY           s3, #0xbc
                        ADDCY           s4, #0x1b
                        ADDCY           s5, #0x8f
;
;Add variable 'E' to [s5,s4,s3,s2]
;
compute_tmp:            FETCH           s0, var_e0
                        ADD             s2, s0
                        FETCH           s0, var_e1
                        ADDCY           s3, s0
                        FETCH           s0, var_e2
                        ADDCY           s4, s0
                        FETCH           s0, var_e3
                        ADDCY           s5, s0
;
;Add variable 'A' rotated left 5 places
;
                        FETCH           s9, var_a3              ;A in [s9,s8,s7,s6]
                        FETCH           s8, var_a2
                        FETCH           s7, var_a1
                        FETCH           s6, var_a0
                        LOAD            s0, #0x05               ;rotate left 5 places
                        CALL            rotate_word_left_n_places
                        ADD             s2, s6                  ;add to TMP
                        ADDCY           s3, s7
                        ADDCY           s4, s8
                        ADDCY           s5, s9
;
;
;Compute Wt in register set [s9,s8,s7,s6]
;  Value computed is also stored back in the external buffer for
;  use in later iterations as well as being added to TMP.
;
;Iterations 0 to 15
;  Wt = Mt
; This only requires Wt-16 to be read and then shifted back into the buffer again.
;
;Iterations 0 to 15
;  Wt = rotate_left_1_place(Wt-3 xor Wt-8 xor Wt-14 xor Wt-16)
; This requires all data values to be read first. Then XORed and rotated before
; shifting the new Wt word into the buffer.
;
;
                        INPUT           s9, wt_minus16_byte3_read_port ;Read Wt-16 value
                        INPUT           s8, wt_minus16_byte2_read_port
                        INPUT           s7, wt_minus16_byte1_read_port
                        INPUT           s6, wt_minus16_byte0_read_port
                        COMPARE         se, #0x10               ;set carry flag for iterations 0 to 15
                        JUMP            c, store_wt
;
;Read other Wt words and perform XOR
;
                        INPUT           s0, wt_minus14_byte3_read_port ;XOR with Wt-14 value
                        XOR             s9, s0
                        INPUT           s0, wt_minus14_byte2_read_port
                        XOR             s8, s0
                        INPUT           s0, wt_minus14_byte1_read_port
                        XOR             s7, s0
                        INPUT           s0, wt_minus14_byte0_read_port
                        XOR             s6, s0
                        INPUT           s0, wt_minus8_byte3_read_port ;XOR with Wt-8 value
                        XOR             s9, s0
                        INPUT           s0, wt_minus8_byte2_read_port
                        XOR             s8, s0
                        INPUT           s0, wt_minus8_byte1_read_port
                        XOR             s7, s0
                        INPUT           s0, wt_minus8_byte0_read_port
                        XOR             s6, s0
                        INPUT           s0, wt_minus3_byte3_read_port ;XOR with Wt-3 value
                        XOR             s9, s0
                        INPUT           s0, wt_minus3_byte2_read_port
                        XOR             s8, s0
                        INPUT           s0, wt_minus3_byte1_read_port
                        XOR             s7, s0
                        INPUT           s0, wt_minus3_byte0_read_port
                        XOR             s6, s0
                        CALL            rotate_word_left        ;rotate XORed word left by one place
;
;Store new Wt value in external buffer
;
store_wt:               OUTPUT          s9, w_word_write_port
                        OUTPUT          s8, w_word_write_port
                        OUTPUT          s7, w_word_write_port
                        OUTPUT          s6, w_word_write_port
;
;Add new computed Wt value to TMP in [s5,s4,s3,s2]
;
                        ADD             s2, s6
                        ADDCY           s3, s7
                        ADDCY           s4, s8
                        ADDCY           s5, s9
;
;TMP is now complete in [s5,s4,s3,s2]
;
;
;copy values
;  E <= D
;  D <= C
;  C <= B (this will need to be rotated 30 places afterwards)
;  B <= A
;
                        LOAD            sd, #0x04               ;4 bytes per word to copy
copy_var_loop:          LOAD            sc, #var_e3
                        LOAD            sb, #var_e2
move_var_loop:          FETCH           sa, @sb
                        STORE           sa, @sc
                        SUB             sc, #0x01
                        SUB             sb, #0x01
                        COMPARE         sc, #var_a0
                        JUMP            nz, move_var_loop
                        SUB             sd, #0x01
                        JUMP            nz, copy_var_loop
;
;rotate 'C' (the previous 'B') left 30 places
;
                        CALL            fetch_c                 ;C in [s9,s8,s7,s6]
                        LOAD            s0, #0x1e               ;rotate left 30 places
                        CALL            rotate_word_left_n_places
                        STORE           s9, var_c3
                        STORE           s8, var_c2
                        STORE           s7, var_c1
                        STORE           s6, var_c0
;
;  A <= TMP
;
                        STORE           s5, var_a3
                        STORE           s4, var_a2
                        STORE           s3, var_a1
                        STORE           s2, var_a0
;
;count iterations
;
                        COMPARE         se, #0x4f               ;test for last iteration = 79 decimal (4F hex)
                        RETURN          z
                        ADD             se, #0x01
                        JUMP            next_sha1_iteration
;
; Routine to rotate left the contents of Register set [s9,s8,s7,s6]
; by the number of places specified in register 's0'.
;
rotate_word_left_n_places: CALL         rotate_word_left
                        SUB             s0, #0x01
                        JUMP            nz, rotate_word_left_n_places
                        RETURN
;
; Routine to rotate left the contents of Register set [s9,s8,s7,s6]
; by one place.
;
rotate_word_left:       TEST            s9, #0x80               ;test MSB of word
                        SLA             s6
                        SLA             s7
                        SLA             s8
                        SLA             s9
                        RETURN
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
; Start by loading family code and serial number (56-bits) into register set
; [s9,s8,s7,s6,s5,s4,s3] so that it can be shifted out LSB first.
;
;
compute_crc8:           FETCH           s3, family_code
                        FETCH           s4, serial_number0
                        FETCH           s5, serial_number1
                        FETCH           s6, serial_number2
                        FETCH           s7, serial_number3
                        FETCH           s8, serial_number4
                        FETCH           s9, serial_number5
                        LOAD            s2, #0x38               ;56 bits to shift (38 hex)
                        LOAD            s0, #0x00               ;clear CRC value
crc8_loop:              LOAD            s1, s0                  ;copy current CRC value
                        XOR             s1, s3                  ;Need to know LSB XOR next input bit
                        TEST            s1, #0x01               ;test result of XOR in LSB
                        JUMP            nc, crc8_shift
                        XOR             s0, #0x18               ;compliment bits 3 and 4 of CRC
crc8_shift:             SR0             s1                      ;Carry gets LSB XOR next input bit
                        SRA             s0                      ;shift Carry into MSB to form new CRC value
                        SR0             s9                      ;shift input value
                        SRA             s8
                        SRA             s7
                        SRA             s6
                        SRA             s5
                        SRA             s4
                        SRA             s3
                        SUB             s2, #0x01               ;count iterations
                        JUMP            nz, crc8_loop
                        RETURN
;
;
;
;**************************************************************************************
; Clear or Compute 16-bit CRC used by DS2432.
;**************************************************************************************
;
; The DS2432 computes a 16-bit CRC using the polynomial X16 + X15 + X2 + 1.
; See the DS2432 data sheet for full details.
;
; Note that the value formed in the CRC shift register MUST BE INVERTED to give the
; same value as that sent from the DS2432 during scratchpad write, scratchpad read
; and read auth page commands.
;
; The 16-bit CRC is computed using a different number of bytes depending on the
; command. This routine has been written such that the CRC can be computed one
; byte at a time. The byte to be processed should be provided in register 's3'
; and the contents of this register are preserved.
;
; This routine computes the 16-bit CRC in the register pair [sE,sD] and these
; registers must not be disturbed between calls of this routine.
;
; Before starting a CRC computation the 'clear_CRC16' should be used.
;
; Registers used s0,s1,s3,sD,sE
;    s3 is preserved.
;    sD and sE should not be disturbed between calls if CRC value is required.
;
;
clear_crc16:            LOAD            se, #0x00               ;[sE,sD]=0000
                        LOAD            sd, #0x00
                        RETURN
;
compute_crc16:          LOAD            s1, #0x08               ;8-bits to shift
crc16_loop:             LOAD            s0, sd                  ;copy current CRC value
                        XOR             s0, s3                  ;Need to know LSB XOR next input bit
                        TEST            s0, #0x01               ;test result of XOR in LSB
                        JUMP            nc, crc16_shift
                        XOR             sd, #0x02               ;compliment bit 1 of CRC
                        XOR             se, #0x40               ;compliment bit 14 of CRC
crc16_shift:            SR0             s0                      ;Carry gets LSB XOR next input bit
                        SRA             se                      ;shift Carry into MSB to form new CRC value
                        SRA             sd
                        RR              s3                      ;shift input value
                        SUB             s1, #0x01               ;count bits
                        JUMP            nz, crc16_loop          ;next bit
                        RETURN
;
;
;**************************************************************************************
; Read 16-bit CRC from DS2432, send value received to UART and test result.
;**************************************************************************************
;
; The computed CRC value for comparison must be in register pair [sE,sD]
;
read_send_test_crc16:   CALL            read_byte_slow          ;read 16-bit CRC into [s5,s4]
                        LOAD            s4, s3
                        CALL            read_byte_slow
                        LOAD            s5, s3
                        CALL            send_crc                ;'crc=' to display CRC value
                        LOAD            s0, s5
                        CALL            send_hex_byte
                        LOAD            s0, s4
                        CALL            send_hex_byte
                        CALL            send_cr
                        XOR             sd, #0xff               ;1's complement the computed CRC value
                        XOR             se, #0xff
                        COMPARE         s4, sd                  ;test received value with computed value
                        JUMP            nz, crc16_fail
                        COMPARE         s5, se
                        JUMP            nz, crc16_fail
                        CALL            send_pass               ;display 'Pass' with carriage return
                        RETURN
crc16_fail:             CALL            send_fail               ;display 'Fail' with carriage return
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
ds_wire_init:           LOAD            s0, #ds_wire
                        OUTPUT          s0, ds_wire_out_port
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
ds_init_regular_mode:   LOAD            s0, #0x00               ;transmit reset pulse
                        OUTPUT          s0, ds_wire_out_port
;Delay of 500us is equivalent to 12500 instructions at 50MHz.
;This delay loop is formed of 28 instructions requiring 446 repetitions.
                        LOAD            s2, #0x01               ;[s3,s2]=445 decimal (01BD hex)
                        LOAD            s1, #0xbd
rm_wait_500us:          CALL            delay_1us               ;25 instructions including CALL
                        SUB             s1, #0x01               ;decrement delay counter
                        SUBCY           s2, #0x00
                        JUMP            nc, rm_wait_500us       ;repeat until -1
                        LOAD            s0, #0x01               ;end of regular reset pulse
                        OUTPUT          s0, ds_wire_out_port
;Delay of 60us is equivalent to 1500 instructions at 50MHz.
;This delay and is formed of 27 instructions requiring 56 repetitions.
                        LOAD            s1, #0x38               ;56 (38 hex)
rm_wait_60us:           CALL            delay_1us               ;25 instructions including CALL
                        SUB             s1, #0x01               ;decrement delay counter
                        JUMP            nz, rm_wait_60us        ;repeat until zero
;The DS_wire is now checked at approximately 1us intervals for the next 240us looking
;to detect an active Low presence pulse. The 240us is equivalent to 6000 instructions
;at 50MHz and this polling loop is formed of 33 instructions requiring 182 repetitions.
                        LOAD            s2, #0x01               ;set bit which will be reset by a presence pulse
                        LOAD            s1, #0xb6               ;182 (B6 hex)
rm_poll_240us:          CALL            delay_1us               ;25 instructions including CALL
                        CALL            read_ds_wire            ;read wire - 5 instructions including CALL
                        AND             s2, s0                  ;clear flag if DS_wire was Low
                        SUB             s1, #0x01               ;decrement delay counter
                        JUMP            nz, rm_poll_240us       ;repeat until zero
                        TEST            s2, #0x01               ;set carry flag if no pulse detected
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
read_ds_wire:           INPUT           s0, ds_wire_in_port
                        AND             s0, #ds_wire            ;ensure only bit0 is active
                        TEST            s0, #ds_wire            ;set carry flag if DS_wire is High
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
write_byte_slow:        LOAD            s2, #0x08               ;8 bits to transmit
wbs_loop:               RR              s3                      ;test next bit LSB first
                        JUMP            c, wbs1                 ;transmit '0' or '1'
                        CALL            write_low_slow
                        JUMP            next_slow_bit
wbs1:                   CALL            write_high_slow
next_slow_bit:          SUB             s2, #0x01               ;count bits
                        JUMP            nz, wbs_loop            ;repeat until 8-bits transmitted
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
write_low_slow:         LOAD            s0, #0x00               ;transmit Low pulse
                        OUTPUT          s0, ds_wire_out_port
;Delay of 78us is equivalent to 1950 instructions at 50MHz.
;This delay loop is formed of 27 instructions requiring 72 repetitions.
                        LOAD            s1, #0x48               ;72 (48 hex)
wls_wait_78us:          CALL            delay_1us               ;25 instructions including CALL
                        SUB             s1, #0x01               ;decrement delay counter
                        JUMP            nz, wls_wait_78us       ;repeat until zero
                        LOAD            s0, #0x01               ;end of Low pulse
                        OUTPUT          s0, ds_wire_out_port
                        CALL            delay_1us               ;2us recovery time
                        CALL            delay_1us
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
write_high_slow:        LOAD            s0, #0x00               ;transmit Low pulse
                        OUTPUT          s0, ds_wire_out_port
;Delay of 8us is equivalent to 200 instructions at 50MHz.
;This delay loop is formed of 27 instructions requiring 8 repetitions.
                        LOAD            s1, #0x08               ;8 (08 hex)
whs_wait_8us:           CALL            delay_1us               ;25 instructions including CALL
                        SUB             s1, #0x01               ;decrement delay counter
                        JUMP            nz, whs_wait_8us        ;repeat until zero
                        LOAD            s0, #0x01               ;end of Low pulse
                        OUTPUT          s0, ds_wire_out_port
;Delay of 72us is equivalent to 1800 instructions at 50MHz.
;This delay loop is formed of 27 instructions requiring 67 repetitions.
                        LOAD            s1, #0x43               ;67 (43 hex)
whs_wait_72us:          CALL            delay_1us               ;25 instructions including CALL
                        SUB             s1, #0x01               ;decrement delay counter
                        JUMP            nz, whs_wait_72us       ;repeat until zero
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
read_byte_slow:         LOAD            s2, #0x08               ;8 bits to receive
rbs_loop:               CALL            read_bit_slow           ;read next bit LSB first
                        SUB             s2, #0x01               ;count bits
                        JUMP            nz, rbs_loop            ;repeat until 8-bits received
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
; Then DS2432 responds to the Low pulse by diving DS_wire in two different ways
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
read_bit_slow:          LOAD            s0, #0x00               ;transmit Low pulse
                        OUTPUT          s0, ds_wire_out_port
;Delay of 4us is equivalent to 100 instructions at 50MHz.
;This delay loop is formed of 27 instructions requiring 4 repetitions.
                        LOAD            s1, #0x04               ;4 (04 hex)
rbs_wait_4us:           CALL            delay_1us               ;25 instructions including CALL
                        SUB             s1, #0x01               ;decrement delay counter
                        JUMP            nz, rbs_wait_4us        ;repeat until zero
                        LOAD            s0, #0x01               ;end of Low pulse
                        OUTPUT          s0, ds_wire_out_port
;Delay of 8us is equivalent to 200 instructions at 50MHz.
;This delay loop is formed of 27 instructions requiring 8 repetitions.
                        LOAD            s1, #0x08               ;8 (08 hex)
rbs_wait_8us:           CALL            delay_1us               ;25 instructions including CALL
                        SUB             s1, #0x01               ;decrement delay counter
                        JUMP            nz, rbs_wait_8us        ;repeat until zero
                        CALL            read_ds_wire            ;sample wire (carry = state)
                        SRA             s3                      ;shift received bit into MSB of s3
;Delay of 68us is equivalent to 1700 instructions at 50MHz.
;This delay loop is formed of 27 instructions requiring 63 repetitions.
                        LOAD            s1, #0x3f               ;63 (3F hex)
rbs_wait_68us:          CALL            delay_1us               ;25 instructions including CALL
                        SUB             s1, #0x01               ;decrement delay counter
                        JUMP            nz, rbs_wait_68us       ;repeat until zero
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
delay_1us:              LOAD            s0, #delay_1us_constant
wait_1us:               SUB             s0, #0x01
                        JUMP            nz, wait_1us
                        RETURN
;
; Delay of 40us.
;
; Registers used s0, s1
;
delay_40us:             LOAD            s1, #0x28               ;40 x 1us = 40us
wait_40us:              CALL            delay_1us
                        SUB             s1, #0x01
                        JUMP            nz, wait_40us
                        RETURN
;
;
; Delay of 1ms.
;
; Registers used s0, s1, s2
;
delay_1ms:              LOAD            s2, #0x19               ;25 x 40us = 1ms
wait_1ms:               CALL            delay_40us
                        SUB             s2, #0x01
                        JUMP            nz, wait_1ms
                        RETURN
;
; Delay of 20ms.
;
; Registers used s0, s1, s2, s3
;
delay_20ms:             LOAD            s3, #0x14               ;20 x 1ms = 20ms
wait_20ms:              CALL            delay_1ms
                        SUB             s3, #0x01
                        JUMP            nz, wait_20ms
                        RETURN
;
; Delay of approximately 1 second.
;
; Registers used s0, s1, s2, s3, s4
;
delay_1s:               LOAD            s4, #0x14               ;50 x 20ms = 1000ms
wait_1s:                CALL            delay_20ms
                        SUB             s4, #0x01
                        JUMP            nz, wait_1s
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
;**************************************************************************************
; Useful ASCII conversion and handling routines
;**************************************************************************************
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
hex_byte_to_ascii:      LOAD            s1, s0                  ;remember value supplied
                        SR0             s0                      ;isolate upper nibble
                        SR0             s0
                        SR0             s0
                        SR0             s0
                        CALL            hex_to_ascii            ;convert
                        LOAD            s2, s0                  ;upper nibble value in s2
                        LOAD            s0, s1                  ;restore complete value
                        AND             s0, #0x0f               ;isolate lower nibble
                        CALL            hex_to_ascii            ;convert
                        LOAD            s1, s0                  ;lower nibble value in s1
                        RETURN
;
; Convert hexadecimal value provided in register s0 into ASCII character
;
;Register used s0
;
hex_to_ascii:           SUB             s0, #0x0a               ;test if value is in range 0 to 9
                        JUMP            c, number_char
                        ADD             s0, #0x07               ;ASCII char A to F in range 41 to 46
number_char:            ADD             s0, #0x3a               ;ASCII char 0 to 9 in range 30 to 40
                        RETURN
;
;
; Send the two character HEX value of the register contents 's0' to the UART
;
; Registers used s0, s1, s2
;
send_hex_byte:          CALL            hex_byte_to_ascii
                        LOAD            uart_data, s2
                        CALL            send_to_uart
                        LOAD            uart_data, s1
                        CALL            send_to_uart
                        RETURN
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
; Read one character from UART and echo.
; Convert to upper case and return.
;
;
read_upper_case:        CALL            read_from_uart          ;read command character from UART
                        CALL            send_to_uart            ;echo character
                        LOAD            s0, uart_data           ;convert to upper case
                        CALL            upper_case
                        RETURN
;
;
; Read two hex characters from UART and convert to single byte data
;
obtain_8bits:           CALL            read_upper_case         ;obtain one byte from UART
                        LOAD            s3, s0
                        CALL            read_upper_case
                        LOAD            s2, s0
                        CALL            ascii_byte_to_hex
                        RETURN
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
; Send a minus sign to the UART
;
send_minus:             LOAD            uart_data, #character_minus
                        CALL            send_to_uart
                        RETURN
;
;
; Send the letter 't' to the UART
;
send_t:                 LOAD            uart_data, #character_t
                        CALL            send_to_uart
                        RETURN
;
; Send the letter 'e' to the UART
;
send_e:                 LOAD            uart_data, #character_e
                        CALL            send_to_uart
                        RETURN
;
; Send the letter 'a' to the UART
;
send_a:                 LOAD            uart_data, #character_a
                        CALL            send_to_uart
                        RETURN
;
;
; Send the letter 'd' to the UART
;
send_d:                 LOAD            uart_data, #character_d
                        CALL            send_to_uart
                        RETURN
;
;
; Send the letter 'r' to the UART
;
send_r:                 LOAD            uart_data, #character_r
                        CALL            send_to_uart
                        RETURN
;
;
; Send the letter 's' to the UART
;
send_s:                 LOAD            uart_data, #character_s
                        CALL            send_to_uart
                        RETURN
;
;
; Send the letter 'c' to the UART
;
send_c:                 LOAD            uart_data, #character_c
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'PicoBlaze SHA-1 Algorithm v1.00' string to the UART
;
send_welcome:           CALL            send_cr
                        CALL            send_cr
                        LOAD            uart_data, #_character_p
                        CALL            send_to_uart
                        LOAD            uart_data, #character_i
                        CALL            send_to_uart
                        CALL            send_c
                        LOAD            uart_data, #character_o
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_b
                        CALL            send_to_uart
                        LOAD            uart_data, #character_l
                        CALL            send_to_uart
                        CALL            send_a
                        LOAD            uart_data, #character_z
                        CALL            send_to_uart
                        CALL            send_e
                        CALL            send_space
                        LOAD            uart_data, #_character_s
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_h
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_a
                        CALL            send_to_uart
                        CALL            send_minus
                        LOAD            uart_data, #character_1
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_data, #_character_a
                        CALL            send_to_uart
                        LOAD            uart_data, #character_l
                        CALL            send_to_uart
                        LOAD            uart_data, #character_g
                        CALL            send_to_uart
                        LOAD            uart_data, #character_o
                        CALL            send_to_uart
                        CALL            send_r
                        LOAD            uart_data, #character_i
                        CALL            send_to_uart
                        CALL            send_t
                        LOAD            uart_data, #character_h
                        CALL            send_to_uart
                        LOAD            uart_data, #character_m
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_data, #character_v
                        CALL            send_to_uart
                        LOAD            uart_data, #character_1
                        CALL            send_to_uart
                        LOAD            uart_data, #character_fullstop
                        CALL            send_to_uart
                        LOAD            uart_data, #character_0
                        CALL            send_to_uart
                        LOAD            uart_data, #character_0
                        CALL            send_to_uart
                        CALL            send_cr
                        CALL            send_cr
                        RETURN
;
;
;
;
;
;
; Send DS2432 menu to the UART
;
send_ds2432_menu:       CALL            send_cr
                        CALL            send_cr
                        LOAD            uart_data, #character_1
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_write
                        CALL            send_space
                        CALL            send_scratchpad
                        CALL            send_cr
                        LOAD            uart_data, #character_2
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_read
                        CALL            send_space
                        CALL            send_scratchpad
                        CALL            send_cr
                        LOAD            uart_data, #character_3
                        CALL            send_to_uart
                        CALL            send_minus
                        LOAD            uart_data, #_character_l
                        CALL            send_to_uart
                        LOAD            uart_data, #character_o
                        CALL            send_to_uart
                        CALL            send_a
                        CALL            send_d
                        CALL            send_space
                        LOAD            uart_data, #character_f
                        CALL            send_to_uart
                        LOAD            uart_data, #character_i
                        CALL            send_to_uart
                        CALL            send_r
                        CALL            send_s
                        CALL            send_t
                        CALL            send_space
                        CALL            send_secret
                        CALL            send_cr
                        LOAD            uart_data, #character_4
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_read
                        CALL            send_space
                        LOAD            uart_data, #character_a
                        CALL            send_to_uart
                        LOAD            uart_data, #character_u
                        CALL            send_to_uart
                        CALL            send_t
                        LOAD            uart_data, #character_h
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_data, #_character_p
                        CALL            send_to_uart
                        CALL            send_a
                        LOAD            uart_data, #character_g
                        CALL            send_to_uart
                        CALL            send_e
                        CALL            send_cr
                        RETURN
;
;
;
; Send carriage return, 'OK' and carriage return to the UART
;
send_ok:                CALL            send_cr
                        LOAD            uart_data, #_character_o
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_k
                        CALL            send_to_uart
                        CALL            send_cr
                        RETURN
;
;
; Send 'scratchpad' to the UART
;
send_scratchpad:        CALL            send_s
                        CALL            send_c
                        CALL            send_r
                        CALL            send_a
                        CALL            send_t
                        CALL            send_c
                        LOAD            uart_data, #character_h
                        CALL            send_to_uart
                        LOAD            uart_data, #character_p
                        CALL            send_to_uart
                        CALL            send_a
                        CALL            send_d
                        RETURN
;
;
; Send 'secret' to the UART
;
send_secret:            CALL            send_s
                        CALL            send_e
                        CALL            send_c
                        CALL            send_r
                        CALL            send_e
                        CALL            send_t
                        RETURN
;
;
; Send 'Byte' to the UART
;
send_byte:              LOAD            uart_data, #_character_b
                        CALL            send_to_uart
                        LOAD            uart_data, #character_y
                        CALL            send_to_uart
                        CALL            send_t
                        CALL            send_e
                        RETURN
;
;
; Send 'Read' to the UART
;
send_read:              LOAD            uart_data, #_character_r
                        CALL            send_to_uart
                        CALL            send_e
                        CALL            send_a
                        CALL            send_d
                        RETURN
;
;
; Send 'Write' to the UART
;
send_write:             LOAD            uart_data, #_character_w
                        CALL            send_to_uart
                        CALL            send_r
                        LOAD            uart_data, #character_i
                        CALL            send_to_uart
                        CALL            send_t
                        CALL            send_e
                        RETURN
;
;
; Send 'Pass' to the UART
;
send_pass:              LOAD            uart_data, #_character_p
                        CALL            send_to_uart
                        CALL            send_a
                        CALL            send_s
                        CALL            send_s
                        CALL            send_cr
                        RETURN
;
;
; Send 'Fail' to the UART
;
send_fail:              LOAD            uart_data, #_character_f
                        CALL            send_to_uart
                        CALL            send_a
                        LOAD            uart_data, #character_i
                        CALL            send_to_uart
                        LOAD            uart_data, #character_l
                        CALL            send_to_uart
                        CALL            send_cr
                        RETURN
;
;
; Send 'address=' to the UART
;
send_address:           CALL            send_cr
                        CALL            send_a
                        CALL            send_d
                        CALL            send_d
                        CALL            send_r
                        CALL            send_e
                        CALL            send_s
                        CALL            send_s
send_equals:            LOAD            uart_data, #character_equals
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'data' to the UART
;
send_data:              CALL            send_cr
                        CALL            send_d
                        CALL            send_a
                        CALL            send_t
                        CALL            send_a
                        RETURN
;
;
; Send 'E/S=' to the UART
;
send_es:                CALL            send_cr
                        LOAD            uart_data, #_character_e
                        CALL            send_to_uart
                        LOAD            uart_data, #character_divide
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_s
                        CALL            send_to_uart
                        JUMP            send_equals
;
;
; Send 'code=' to the UART
;
send_code:              CALL            send_c
                        LOAD            uart_data, #character_o
                        CALL            send_to_uart
                        CALL            send_d
                        CALL            send_e
                        JUMP            send_equals
;
;
; Send 's/n=' to the UART
;
send_sn:                CALL            send_s
                        LOAD            uart_data, #character_divide
                        CALL            send_to_uart
                        LOAD            uart_data, #character_n
                        CALL            send_to_uart
                        JUMP            send_equals
;
;
; Send 'crc=' to the UART
;
send_crc:               CALL            send_c
                        LOAD            uart_data, #character_r
                        CALL            send_to_uart
                        CALL            send_c
                        JUMP            send_equals
;
;
;
; Send 'mac=' to the UART
;
send_mac:               LOAD            uart_data, #character_m
                        CALL            send_to_uart
                        CALL            send_a
                        CALL            send_c
                        JUMP            send_equals
;
;
;**************************************************************************************
; Interrupt Service Routine (ISR)
;**************************************************************************************
;
; Interrupts are not used in this design. This is a place keeper only.
;
                        ORG             0x3fe
isr:                    RETURNI         enable
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

