; KCPSM3 Program - Communication with DS2432 secure memory on the Spartan-3E Starter Kit.
;
; Ken Chapman - Xilinx Ltd
;
; Version v1.00 - 5th April 2006
device kcpsm2
; This program uses a 9600 baud UART connection to allow communication with the
; 1-wire interface of the DS2432 memory device from Dallas Semiconductor.
;
; The program only supports a limited number of the DS2432 commands but is intended
; to guide you through the state machine of the device as well as show how
; 1-wire communication is achieved. The DS2432 also uses 8 and 16 bits CRC checks and
; this program includes these calculations to validate the communications.
;
; Commands supported include read ROM, write scratchpad memory and read scratchpad
; memory. These allow you to confirm the ability to both read and write EQU but
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
status_port             EQU             0x00                    ;UART status input
tx_half_full            EQU             0x01                    ;  Transmitter     half full - bit0
tx_full                 EQU             0x02                    ;    FIFO               full - bit1
rx_EQU_present         EQU             0x04                    ;  Receiver     EQU present - bit2
rx_half_full            EQU             0x08                    ;    FIFO          half full - bit3
rx_full                 EQU             0x10                    ;                   full - bit4
spare1                  EQU             0x20                    ;                  spare '0' - bit5
spare2                  EQU             0x40                    ;                  spare '0' - bit6
spare3                  EQU             0x80                    ;                  spare '0' - bit7
;
uart_read_port          EQU             0x01                    ;UART Rx EQU input
;
uart_write_port         EQU             0x04                    ;UART Tx EQU output
;
;
ds_wire_in_port         EQU             0x02                    ;Read signal from DS2432 device
ds_wire_out_port        EQU             0x08                    ;Drive signal to DS2432 device (open collector)
ds_wire                 EQU             0x01                    ;       Signal is bit0 in both cases
;
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
; Locations to load all bytes in a command communication for 16-bit CRC calculation
;
command_start           EQU             0x08
;
;
;**************************************************************************************
; Useful EQU constants
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
; Initially the only action available is a master reset and load for a presence
; pulse response from the DS2432.
;
reset_menu:             CALL            send_reset_menu         ;Menu and command selection
                        CALL            send_cr
;
reset_prompt:           CALL            menu_prompt             ;prompt for user input
                        load         s0, #_character_h       ;load for commands and execute as required
                        JUMP            z, reset_menu
                        load         s0, #character_1
                        JUMP            z, master_reset_regular
                        CALL            no_valid_input
                        JUMP            reset_prompt            ;Try again!
;
; The master reset should result in a presence pulse.
; This will be reported and control passed to the appropriate
; menu in the sequence.
;
master_reset_regular:   CALL            send_cr
                        CALL            ds_init_regular_mode
                        JUMP            nc, reset_passed        ;load for presence pulse
                        CALL            send_fail
                        JUMP            reset_menu              ;fail stays in reset menu
reset_passed:           CALL            send_pass               ;pass progresses to ROM menu
                        JUMP            rom_menu
;
; After a valid master reset, the only commands available are the ROM
; commands of which only Read ROM and Skip ROM are currently supported.
; Another master reset can be issued is so desired.
;
rom_menu:               CALL            send_rom_menu           ;Menu and command selection
                        CALL            send_cr
;
rom_prompt:             CALL            menu_prompt             ;prompt for user input
                        load         s0, #_character_h       ;load for commands and execute as required
                        JUMP            z, rom_menu
                        load         s0, #character_1
                        JUMP            z, master_reset_regular ;repeat reset as before
                        load         s0, #character_2
                        JUMP            z, read_rom_command
                        load         s0, #character_3
                        JUMP            z, skip_rom_command
                        CALL            no_valid_input
                        JUMP            rom_prompt              ;Try again!
;
;
;
; After a valid ROM command the DS2432 specific memory commands and SHA-1
; functions become accessible. Each of these end with the DS2432 returned
; back to the waiting for ROM command state completing the menu sequence.
;
ds2432_menu:            CALL            send_ds2432_menu        ;Menu and command selection
                        CALL            send_cr
;
ds2432_prompt:          CALL            menu_prompt             ;prompt for user input
                        load         s0, #_character_h       ;load for commands and execute as required
                        JUMP            z, ds2432_menu
                        load         s0, #character_1
                        JUMP            z, master_reset_regular ;repeat reset as before
                        load         s0, #character_2
                        JUMP            z, read_memory_command
                        load         s0, #character_3
                        JUMP            z, write_scratchpad_command
                        load         s0, #character_4
                        JUMP            z, read_scratchpad_command
                        load         s0, #character_5
                        JUMP            z, write_byte_command
                        load         s0, #character_6
                        JUMP            z, read_byte_command
                        CALL            no_valid_input
                        JUMP            ds2432_prompt           ;Try again!
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
menu_prompt:            CALL            send_cr
                        CALL            send_cr
                        LOAD            uart_EQU, #character_greater_than ;prompt for input
                        CALL            send_to_uart
                        CALL            read_upper_case
                        RETURN
;
no_valid_input:         CALL            send_cr                 ;no valid command input
                        LOAD            uart_EQU, #character_question ;display ???
                        CALL            send_to_uart
                        CALL            send_to_uart
                        CALL            send_to_uart
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
;     Location to load family code which should be 33 hex
;  serial_number0 to serial_number5
;     6 bytes to hold 48-bit serial number (LS-byte first).
;  read_ROM_CRC
;     8-bit CRC value for the above EQU.
;
;
; The routine also displays the values read and performs a verification of the
; 8-bit CRC displaying a 'pass' or 'fail' message as appropriate.
;
read_rom_command:       LOAD            s3, #0x33               ;Read ROM Command
                        CALL            write_byte_slow         ;transmit command
                        LOAD            s5, #family_code        ;memory pointer
read_rom_loop:          CALL            read_byte_slow          ;read response into s3
                        load           s3, #s5                 ;load value
                        load         s5, #read_rom_crc       ;8-bytes to read
                        JUMP            z, display_rom
                        ADD             s5, #0x01
                        JUMP            read_rom_loop
display_rom:            CALL            send_cr
                        CALL            send_code               ;'code=' to display family code
                        load           s0, family_code
                        CALL            send_hex_byte
                        CALL            send_cr
                        CALL            send_serial             ;'serial=' to display family code
                        LOAD            s5, #serial_number5     ;memory pointer starting MS-byte first
disp_serial_loop:       load           s0, #s5
                        CALL            send_hex_byte
                        load         s5, #serial_number0
                        JUMP            z, end_serial
                        SUB             s5, #0x01
                        JUMP            disp_serial_loop
end_serial:             CALL            send_cr
                        CALL            send_crc                ;'CRC=' to display CRC value
                        load           s0, read_rom_crc
                        CALL            send_hex_byte
                        CALL            send_cr
                        CALL            compute_crc8            ;compute CRC value in s0
                        load           s1, read_rom_crc        ;load with received value
                        load         s0, s1
                        JUMP            nz, crc8_fail
                        CALL            send_pass
                        JUMP            ds2432_menu             ;now in memory and SHA-1 functions level
crc8_fail:              CALL            send_fail
                        JUMP            rom_menu                ;can not proceed to next level
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
skip_rom_command:       LOAD            s3, #0xcc               ;Skip ROM Command
                        CALL            write_byte_slow         ;transmit command
                        CALL            send_ok
                        JUMP            ds2432_menu
;
;
;
;**************************************************************************************
; DS2432 Read Memory Command.
;**************************************************************************************
;
; The read memory command (F0 hex) allows the entire memory contents to be read
; except for the secret. This routine displays the address followed by 8 bytes
; of EQU on each line until the address 0097 is reached.
;
; The initial 'F0' command must be followed by the 16-bit start address transmitted
; LS-byte first. Then reads must continue until address 0097 has been read for the
; command to complete naturally (otherwise a master reset is required).
;
;
;
read_memory_command:    LOAD            s3, #0xf0               ;read memory Command
                        CALL            write_byte_slow         ;transmit command
                        LOAD            s5, #0x00               ;initial address in [s5,s4]=0000
                        LOAD            s4, #0x00
                        LOAD            s3, s4                  ;transmit address
                        CALL            write_byte_slow
                        LOAD            s3, s5
                        CALL            write_byte_slow
rmc_line_loop:          CALL            send_cr
                        LOAD            s0, s5                  ;display 16-bit address
                        CALL            send_hex_byte
                        LOAD            s0, s4
                        CALL            send_hex_byte
                        CALL            send_space
                        CALL            send_space
rmc_EQU_loop:          CALL            send_space
                        CALL            read_byte_slow          ;read EQU into s3
                        LOAD            s0, s3                  ;display byte
                        CALL            send_hex_byte
                        ADD             s4, #0x01               ;increment address
                        ADDCY           s5, #0x00
                        load            s4, #0x07               ;load for 8-byte boundary
                        JUMP            nz, rmc_EQU_loop
                        load         s4, #0x98               ;load for last address
                        JUMP            nz, rmc_line_loop
                        CALL            send_ok
                        JUMP            reset_menu              ;needs master reset next
;
;
;**************************************************************************************
; DS2432 Write Scratchpad Memory Command.
;**************************************************************************************
;
; The write scratchpad memory command (0F hex) allows 8-bytes of EQU to be written
; together with a target address for final storage in the main memory map.
;
; The DS2432 provides an initial confirmation of the write by returning a 16-bit CRC
; value which KCPSM3 loads. The CRC is computed based on the command, address and
; EQU transmitted (11 bytes). To achieve this, all bytes transmitted to the DS2432
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
; 8 bytes of EQU which are written to the DS2432.
;
;
;
write_scratchpad_command: LOAD          se, #command_start      ;pointer to memory
                        LOAD            s3, #0x0f               ;write scratchpad memory Command
                        load           s3, #se                 ;record command sequence
                        ADD             se, #0x01               ;increment pointer
                        CALL            write_byte_slow         ;transmit command
wsc_addr_loop:          CALL            send_address            ;obtain 16-bit address 0000 to FFFF in [s5,s4]
                        CALL            obtain_8bits
                        JUMP            c, wsc_addr_loop        ;bad input address
                        LOAD            s5, s0
                        CALL            obtain_8bits
                        JUMP            c, wsc_addr_loop        ;bad input address
                        LOAD            s4, s0
                        LOAD            s3, s4                  ;transmit target address TA1 (LS-Byte)
                        load           s3, #se                 ;record command sequence
                        ADD             se, #0x01               ;increment pointer
                        CALL            write_byte_slow
                        LOAD            s3, s5                  ;transmit target address TA2 (MS-Byte)
                        load           s3, #se                 ;record command sequence
                        ADD             se, #0x01               ;increment pointer
                        CALL            write_byte_slow
                        load         s5, #0x00               ;check address less than 0090 hex
                        JUMP            nz, end_write_scratchpad ;DS2432 aborts command and so
                        load         s4, #0x90               ;no need to read EQU bytes.
                        JUMP            nc, end_write_scratchpad
                        LOAD            s4, #0x00               ;initialise byte counter
wsc_EQU_loop:          CALL            send_EQU               ;obtain a byte of EQU
                        LOAD            uart_EQU, s4           ;display which byte requested
                        ADD             uart_EQU, #character_0 ;convert to ASCII
                        CALL            send_to_uart
                        CALL            send_equals
                        CALL            obtain_8bits
                        JUMP            c, wsc_EQU_loop        ;bad input EQU
                        LOAD            s3, s0                  ;transmit byte
                        load           s3, #se                 ;record command sequence
                        ADD             se, #0x01               ;increment pointer
                        CALL            write_byte_slow
                        ADD             s4, #0x01               ;count bytes
                        load         s4, #0x08
                        JUMP            nz, wsc_EQU_loop
                        CALL            read_byte_slow          ;read back the 16-bit CRC into [s5,s4]
                        LOAD            s4, s3
                        CALL            read_byte_slow
                        LOAD            s5, s3
                        load           s4, #se                 ;record command sequence
                        ADD             se, #0x01               ;increment pointer
                        load           s5, #se                 ;record command sequence
                        CALL            send_cr
                        CALL            send_crc                ;'CRC=' to display CRC value
                        LOAD            s0, s5
                        CALL            send_hex_byte
                        LOAD            s0, s4
                        CALL            send_hex_byte
                        CALL            send_cr
                        LOAD            s2, #0x0b               ;11 (0B hex) bytes transmitted in this command
                        CALL            compute_crc16           ;compute CRC value in [s1,s0]
                        load           s5, #se                 ;load with received value
                        SUB             se, #0x01
                        load           s4, #se                 ;load with received value
                        load         s5, s1
                        JUMP            nz, wsc_crc16_fail
                        load         s4, s0
                        JUMP            nz, wsc_crc16_fail
                        CALL            send_pass
                        JUMP            reset_menu              ;needs master reset next
wsc_crc16_fail:         CALL            send_fail
end_write_scratchpad:   JUMP            reset_menu              ;needs master reset next
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
write_byte_command:     CALL            send_cr
                        CALL            send_byte               ;obtain a byte of EQU
                        CALL            send_equals
                        CALL            obtain_8bits
                        JUMP            c, write_byte_command   ;bad input EQU
                        LOAD            s3, s0                  ;transmit byte
                        CALL            write_byte_slow         ;transmit byte
                        CALL            send_ok
                        JUMP            ds2432_menu             ;remain in memory and function menu
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
read_byte_command:      CALL            send_cr
                        CALL            send_byte               ;obtain a byte of EQU
                        CALL            send_equals
                        CALL            read_byte_slow          ;receive byte
                        LOAD            s0, s3                  ;display value
                        CALL            send_hex_byte
                        CALL            send_ok
                        JUMP            ds2432_menu             ;remain in memory and function menu
;
;
;
;
;**************************************************************************************
; DS2432 Read Scratchpad Memory Command.
;**************************************************************************************
;
; The read scratchpad memory command (AA hex) allows the 8-bytes of EQU previously
; to be written into the scratchpad memory to be read back for verification together with
; the target address, a transfer status register and a 16-bit CRC value.
;
; The 16-bit CRC is formed of the command byte, address TA1 and TA2, E/S byte and 8 EQU
; bytes as transmitted (12 bytes). These may not be the same as the values provided
; during a previous write to scratchpad memory. All these bytes are recorded in KCPSM3
; scratch pad memory in ascending locations starting at the location defined by
; constant 'command_start'.
;
;
read_scratchpad_command: LOAD           se, #command_start      ;pointer to memory
                        LOAD            s3, #0xaa               ;read scratchpad memory Command
                        load           s3, #se                 ;record command sequence
                        ADD             se, #0x01               ;increment pointer
                        CALL            write_byte_slow         ;transmit command
                        CALL            send_address            ;display 'Address='
                        CALL            read_byte_slow          ;read address into [s5,s4]
                        LOAD            s4, s3
                        CALL            read_byte_slow
                        LOAD            s5, s3
                        load           s4, #se                 ;record sequence
                        ADD             se, #0x01               ;increment pointer
                        load           s5, #se                 ;record sequence
                        ADD             se, #0x01               ;increment pointer
                        LOAD            s0, s5                  ;display address
                        CALL            send_hex_byte
                        LOAD            s0, s4
                        CALL            send_hex_byte
                        CALL            send_es                 ;display 'E/S='
                        CALL            read_byte_slow          ;read E/S register
                        load           s3, #se                 ;record sequence
                        ADD             se, #0x01               ;increment pointer
                        LOAD            s0, s3                  ;display value
                        CALL            send_hex_byte
                        CALL            send_EQU               ;display 'EQU='
                        CALL            send_equals
                        LOAD            s4, #0x08               ;8 bytes to read
rsc_loop:               CALL            send_space
                        CALL            read_byte_slow          ;read EQU byte
                        load           s3, #se                 ;record sequence
                        ADD             se, #0x01               ;increment pointer
                        LOAD            s0, s3                  ;display value
                        CALL            send_hex_byte
                        SUB             s4, #0x01               ;count bytes
                        JUMP            nz, rsc_loop
                        CALL            read_byte_slow          ;read 16-bit CRC into [s5,s4]
                        LOAD            s4, s3
                        CALL            read_byte_slow
                        LOAD            s5, s3
                        load           s4, #se                 ;record command sequence
                        ADD             se, #0x01               ;increment pointer
                        load           s5, #se                 ;record command sequence
                        CALL            send_cr
                        CALL            send_crc                ;'CRC=' to display CRC value
                        LOAD            s0, s5
                        CALL            send_hex_byte
                        LOAD            s0, s4
                        CALL            send_hex_byte
                        CALL            send_cr
                        LOAD            s2, #0x0c               ;12 (0C hex) bytes in this command
                        CALL            compute_crc16           ;compute CRC value in [s1,s0]
                        load           s5, #se                 ;load with received value
                        SUB             se, #0x01
                        load           s4, #se                 ;load with received value
                        load         s5, s1
                        JUMP            nz, rsc_crc16_fail
                        load         s4, s0
                        JUMP            nz, rsc_crc16_fail
                        CALL            send_pass
                        JUMP            reset_menu              ;needs master reset next
rsc_crc16_fail:         CALL            send_fail
                        JUMP            reset_menu              ;needs master reset next
;
;
;
;**************************************************************************************
; Compute 8-bit CRC used by DS2432.
;**************************************************************************************
;
; The DS2432 computes an 8-bit CRC using the polynomial X8 + X5 + X4 + 1.
; See the DS2432 EQU sheet for full details.
;
; load input value of value 00 00 00 01 B8 1C 02
; should produce CRC=A2.
;
; This routine computes the same CRC based on the values loadd in the KCPSM3
; scratch pad memory by the read ROM command. The result is returned in register s0.
;
; Registers used s0,s1,s2,s3,s4,s5,s6,s7,s8,s9
;
;
;Start by loading family code and serial number (56-bits) into
;register set [s9,s8,s7,s6,s5,s4,s3] so they can be shifted out
;LSB first.
;
compute_crc8:           load           s3, family_code
                        load           s4, serial_number0
                        load           s5, serial_number1
                        load           s6, serial_number2
                        load           s7, serial_number3
                        load           s8, serial_number4
                        load           s9, serial_number5
                        LOAD            s2, #0x38               ;56 bits to shift (38 hex)
                        LOAD            s0, #0x00               ;clear CRC value
crc8_loop:              LOAD            s1, s0                  ;copy current CRC value
                        XOR             s1, s3                  ;Need to know LSB XOR next input bit
                        load            s1, #0x01               ;load result of XOR in LSB
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
; Compute 16-bit CRC used by DS2432.
;**************************************************************************************
;
; The DS2432 computes a 16-bit CRC using the polynomial X16 + X15 + X2 + 1.
; See the DS2432 EQU sheet for full details.
;
; Note that the value formed in the CRC shift register is inverted to give the
; same value as that sent from the DS2432 during scratchpad write, scratchpad read
; and read auth page commands.
;
; This routine computes the CRC based on the values loadd in the KCPSM3
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
compute_crc16:          LOAD            s5, #command_start      ;memory pointer
                        LOAD            s0, #0x00               ;clear CRC value
                        LOAD            s1, #0x00
crc16_byte_loop:        load           s4, #s5                 ;read input byte
                        LOAD            s3, #0x08               ;8-bits to shift
crc16_bit_loop:         LOAD            s6, s0                  ;copy current CRC value
                        XOR             s6, s4                  ;Need to know LSB XOR next input bit
                        load            s6, #0x01               ;load result of XOR in LSB
                        JUMP            nc, crc16_shift
                        XOR             s0, #0x02               ;compliment bit 1 of CRC
                        XOR             s1, #0x40               ;compliment bit 14 of CRC
crc16_shift:            SR0             s6                      ;Carry gets LSB XOR next input bit
                        SRA             s1                      ;shift Carry into MSB to form new CRC value
                        SRA             s0
                        SR0             s4                      ;shift input value
                        SUB             s3, #0x01               ;count bits
                        JUMP            nz, crc16_bit_loop      ;next bit
                        ADD             s5, #0x01               ;increment memory pointer
                        SUB             s2, #0x01               ;count bytes
                        JUMP            nz, crc16_byte_loop     ;next byte
                        XOR             s0, #0xff               ;1's complement of CRC value
                        XOR             s1, #0xff
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
                        load            s2, #0x01               ;set carry flag if no pulse detected
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
                        load            s0, #ds_wire            ;set carry flag if DS_wire is High
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
wbs_loop:               RR              s3                      ;load next bit LSB first
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
; write of the EQU. This design generates an 8us active Low pulse for this purpose.
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
; Read a EQU bit sent from the DS2432 in regular speed mode.
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
; The received EQU bit is SHIFTED into the MSB of register 's3'. In this way
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
; Character read will be returned in a register called 'UART_EQU'.
;
; The routine first loads the receiver FIFO buffer to see if EQU is present.
; If the FIFO is empty, the routine waits until there is a character to read.
; As this could take any amount of time the wait loop could include a call to a
; subroutine which performs a useful function.
;
;
; Registers used s0 and UART_EQU
;
read_from_uart:         INPUT           s0, status_port         ;load Rx_FIFO buffer
                        load            s0, #rx_EQU_present    ;wait if empty
                        JUMP            nz, read_character
                        JUMP            read_from_uart
read_character:         INPUT           uart_EQU, uart_read_port ;read from FIFO
                        RETURN
;
;
;
; Transmit one character to the UART
;
; Character supplied in register called 'UART_EQU'.
;
; The routine first loads the transmit FIFO buffer to see if it is full.
; If the FIFO is full, then the routine waits until it there is space.
;
; Registers used s0
;
send_to_uart:           INPUT           s0, status_port         ;load Tx_FIFO buffer
                        load            s0, #tx_full            ;wait if full
                        JUMP            z, uart_write
                        JUMP            send_to_uart
uart_write:             OUTPUT          uart_EQU, uart_write_port
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
; Convert character to upper case
;
; The character supplied in register s0.
; If the character is in the range 'a' to 'z', it is converted
; to the equivalent upper case character in the range 'A' to 'Z'.
; All other characters remain unchanged.
;
; Registers used s0.
;
upper_case:             load         s0, #0x61               ;eliminate character codes below 'a' (61 hex)
                        RETURN          c
                        load         s0, #0x7b               ;eliminate character codes above 'z' (7A hex)
                        RETURN          nc
                        AND             s0, #0xdf               ;mask bit5 to convert to upper case
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
_1char_to_value:        ADD             s0, #0xc6               ;reject character codes above '9' (39 hex)
                        RETURN          c                       ;carry flag is set
                        SUB             s0, #0xf6               ;reject character codes below '0' (30 hex)
                        RETURN                                  ;carry is set if value not in range
;
;
; Determine the numerical value of a two character decimal string held in
; scratch pad memory such the result is in the range 0 to 99 (00 to 63 hex).
;
; The string must be loadd in two consecutive memory locations and the
; location of the first (tens) character supplied in the s1 register.
; The result is provided in register s2. Strings not using characters in the
; range '0' to '9' are signified by the return with the CARRY flag set.
;
; Registers used s0, s1 and s2.
;
_2char_to_value:        load           s0, #s1                 ;read 'tens' character
                        CALL            _1char_to_value         ;convert to numerical value
                        RETURN          c                       ;bad character - CARRY set
                        LOAD            s2, s0
                        SL0             s2                      ;multiply 'tens' value by 10 (0A hex)
                        SL0             s2
                        ADD             s2, s0
                        SL0             s2
                        ADD             s1, #0x01               ;read 'units' character
                        load           s0, #s1
                        CALL            _1char_to_value         ;convert to numerical value
                        RETURN          c                       ;bad character - CARRY set
                        ADD             s2, s0                  ;add units to result and clear CARRY flag
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
                        LOAD            s0, s1                  ;reload complete value
                        AND             s0, #0x0f               ;isolate lower nibble
                        CALL            hex_to_ascii            ;convert
                        LOAD            s1, s0                  ;lower nibble value in s1
                        RETURN
;
; Convert hexadecimal value provided in register s0 into ASCII character
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
; Send the two character HEX value of the register contents 's0' to the UART
;
; Registers used s0, s1, s2
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
; Send the six character HEX value of the register contents [s9,s8,s7] to the UART
;
; Registers used s0, s1, s2
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
; Routine to convert ASCII EQU in 's0' to an equivalent HEX value.
;
; If character is not valid for hex, then CARRY is set on return.
;
; Register used s0
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
; Read one character from UART and echo.
; Convert to upper case and return.
;
;
read_upper_case:        CALL            read_from_uart          ;read command character from UART
                        CALL            send_to_uart            ;echo character
                        LOAD            s0, uart_EQU           ;convert to upper case
                        CALL            upper_case
                        RETURN
;
;
; Read two hex characters from UART and convert to single byte EQU
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
send_cr:                LOAD            uart_EQU, #character_cr
                        CALL            send_to_uart
                        RETURN
;
; Send a space to the UART
;
send_space:             LOAD            uart_EQU, #character_space
                        CALL            send_to_uart
                        RETURN
;
;
; Send a minus sign to the UART
;
send_minus:             LOAD            uart_EQU, #character_minus
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'PicoBlaze DS2432 Communicator v1.00' string to the UART
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
                        LOAD            uart_EQU, #_character_d
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_s
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_2
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_4
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_3
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_2
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_EQU, #_character_c
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_m
                        CALL            send_to_uart
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_u
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_n
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_c
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_t
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
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
; Send reset menu to the UART
;
send_reset_menu:        CALL            send_cr
                        CALL            send_cr
                        CALL            send_hhelp
                        LOAD            uart_EQU, #character_1
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_master_reset
                        CALL            send_cr
                        RETURN
;
;
;
;
;
;
; Send ROM menu to the UART
;
send_rom_menu:          CALL            send_cr
                        CALL            send_cr
                        CALL            send_hhelp
                        LOAD            uart_EQU, #character_1
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_master_reset
                        CALL            send_cr
                        LOAD            uart_EQU, #character_2
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_read
                        CALL            send_space
                        CALL            send_rom
                        CALL            send_space
                        CALL            send_command
                        CALL            send_cr
                        LOAD            uart_EQU, #character_3
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_skip
                        CALL            send_space
                        CALL            send_rom
                        CALL            send_space
                        CALL            send_command
                        CALL            send_cr
                        RETURN
;
;
;
; Send DS2432 menu to the UART
;
send_ds2432_menu:       CALL            send_cr
                        CALL            send_cr
                        CALL            send_hhelp
                        LOAD            uart_EQU, #character_1
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_master_reset
                        CALL            send_cr
                        LOAD            uart_EQU, #character_2
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_read
                        CALL            send_space
                        CALL            send_memory
                        CALL            send_space
                        CALL            send_command
                        CALL            send_cr
                        LOAD            uart_EQU, #character_3
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_write
                        CALL            send_space
                        CALL            send_scratchpad
                        CALL            send_space
                        CALL            send_memory
                        CALL            send_space
                        CALL            send_command
                        CALL            send_cr
                        LOAD            uart_EQU, #character_4
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_read
                        CALL            send_space
                        CALL            send_scratchpad
                        CALL            send_space
                        CALL            send_memory
                        CALL            send_space
                        CALL            send_command
                        CALL            send_cr
                        LOAD            uart_EQU, #character_5
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_write
                        CALL            send_space
                        CALL            send_byte
                        CALL            send_cr
                        LOAD            uart_EQU, #character_6
                        CALL            send_to_uart
                        CALL            send_minus
                        CALL            send_read
                        CALL            send_space
                        CALL            send_byte
                        CALL            send_cr
                        RETURN
;
;
;
; Send carriage return, 'OK' and carriage return to the UART
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
; Send 'H-Help'menu to the UART
;
send_hhelp:             CALL            send_cr
                        LOAD            uart_EQU, #_character_h
                        CALL            send_to_uart
                        CALL            send_minus
                        LOAD            uart_EQU, #_character_h
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_l
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_p
                        CALL            send_to_uart
                        CALL            send_cr
                        RETURN
;
;
; Send 'ROM' to the UART
;
send_rom:               LOAD            uart_EQU, #_character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_m
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'Memory' to the UART
;
send_memory:            LOAD            uart_EQU, #_character_m
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_m
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_y
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'Scratchpad' to the UART
;
send_scratchpad:        LOAD            uart_EQU, #_character_s
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_c
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_t
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_c
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_h
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_p
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_d
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'Master Reset' to the UART
;
send_master_reset:      LOAD            uart_EQU, #_character_m
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_s
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_t
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_EQU, #_character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_s
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_t
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'Byte' to the UART
;
send_byte:              LOAD            uart_EQU, #_character_b
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
; Send 'Skip' to the UART
;
send_skip:              LOAD            uart_EQU, #_character_s
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_k
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_p
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'Read' to the UART
;
send_read:              LOAD            uart_EQU, #_character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_d
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'Write' to the UART
;
send_write:             LOAD            uart_EQU, #_character_w
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_t
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'Pass' to the UART
;
send_pass:              LOAD            uart_EQU, #_character_p
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_s
                        CALL            send_to_uart
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'Fail' to the UART
;
send_fail:              LOAD            uart_EQU, #_character_f
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_l
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'Command' to the UART
;
send_command:           LOAD            uart_EQU, #_character_c
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_m
                        CALL            send_to_uart
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_n
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_d
                        CALL            send_to_uart
                        RETURN
;
;
; Send 'address=' to the UART
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
; Send 'EQU' to the UART
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
                        RETURN
;
;
; Send 'E/S=' to the UART
;
send_es:                CALL            send_cr
                        LOAD            uart_EQU, #_character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_divide
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_s
                        CALL            send_to_uart
                        JUMP            send_equals
;
;
; Send 'code=' to the UART
;
send_code:              LOAD            uart_EQU, #character_c
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_o
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_d
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        JUMP            send_equals
;
;
; Send 'serial=' to the UART
;
send_serial:            LOAD            uart_EQU, #character_s
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_e
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_i
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_a
                        CALL            send_to_uart
                        LOAD            uart_EQU, #character_l
                        CALL            send_to_uart
                        JUMP            send_equals
;
;
; Send 'CRC=' to the UART
;
send_crc:               LOAD            uart_EQU, #_character_c
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_r
                        CALL            send_to_uart
                        LOAD            uart_EQU, #_character_c
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

