; KCPSM3 Program - Program to test line stores using the Spartan-3E Starter Kit.
;
; Ken Chapman - Xilinx Ltd
;
; Version v1.00 - 26th June 2006
;
; PicoBlaze communicates via the UART to control the value applied to the inputs of
; line stores and enable them for a specified number of clock cycles. The outputs of
; all the line stores are then displayed.
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
;UART ports
;
status_port             EQU             0x00                    ;UART status input
tx_half_full            EQU             0x01                    ;  Transmitter     half full - bit0
tx_full                 EQU             0x02                    ;    FIFO               full - bit1
rx_data_present         EQU             0x04                    ;  Receiver     data present - bit2
rx_half_full            EQU             0x08                    ;    FIFO          half full - bit3
rx_full                 EQU             0x10                    ;                   full - bit4
;
uart_read_port          EQU             0x01                    ;UART Rx data input
;
uart_write_port         EQU             0x40                    ;UART Tx data output
;
;
;The first line store input is 18-bits and requires 3 bytes
;to be written to a holding register. Then when required,
;the whole 18-bit value can be stored in the line store
;using a dummy write (data not used) to a forth port.
;
;
line_store_input_l      EQU             0x01                    ;Line Store input bits [7:0]
line_store_input_m      EQU             0x02                    ;Line Store input bits [15:8]
line_store_input_h      EQU             0x04                    ;Line Store input bits [17:16]
line_store_write_en     EQU             0x08                    ;Line Store clock enable (dummy write)
;
;
;
;The first line store 768x24 and requires 3 bytes to be read.
;
line_store1_output_l    EQU             0x02                    ;Line Store output bits [7:0]
line_store1_output_m    EQU             0x03                    ;Line Store output bits [15:8]
line_store1_output_h    EQU             0x04                    ;Line Store output bits [23:16]
;
;
;The second line store 1024x18 and requires 3 bytes to be read.
;
line_store2_output_l    EQU             0x05                    ;Line Store output bits [7:0]
line_store2_output_m    EQU             0x06                    ;Line Store output bits [15:8]
line_store2_output_h    EQU             0x07                    ;Line Store output bits [17:16]
;
;
;The third line store 1280x13 and requires 2 bytes to be read.
;
line_store3_output_l    EQU             0x08                    ;Line Store output bits [7:0]
line_store3_output_h    EQU             0x09                    ;Line Store output bits [12:8]
;
;
;The forth line store is 1280x72 so to make it more manageable it has been
;folded to make it 3 times longer and only 24-bits wide. This requires 3 bytes
;to be read at each 1280 delay tapping point.
;
line_store4a_output_l   EQU             0x0a                    ;Line Store output bits [7:0]   First tap
line_store4a_output_m   EQU             0x0b                    ;Line Store output bits [15:8]
line_store4a_output_h   EQU             0x0c                    ;Line Store output bits [23:16]
;
line_store4b_output_l   EQU             0x0d                    ;Line Store output bits [31:24] Second tap
line_store4b_output_m   EQU             0x0e                    ;Line Store output bits [39:32]
line_store4b_output_h   EQU             0x0f                    ;Line Store output bits [47:40]
;
line_store4c_output_l   EQU             0x10                    ;Line Store output bits [55:48] Third tap
line_store4c_output_m   EQU             0x11                    ;Line Store output bits [63:56]
line_store4c_output_h   EQU             0x12                    ;Line Store output bits [71:64]
;
;
;The fifth line store 1536x12 and requires 2 bytes to be read.
;
line_store5_output_l    EQU             0x13                    ;Line Store output bits [7:0]
line_store5_output_h    EQU             0x14                    ;Line Store output bits [11:8]
;
;
;The sixth line store 1920x9 and requires 2 bytes to be read.
;
line_store6_output_l    EQU             0x15                    ;Line Store output bits [7:0]
line_store6_output_h    EQU             0x16                    ;Line Store output bit [8]
;
;
;The seventh line store is 1920x48 so to make it more manageable it has been
;folded to make it 2 times longer and only 24-bits wide. This requires 3 bytes
;to be read at each 1920 delay tapping point.
;
line_store7a_output_l   EQU             0x17                    ;Line Store output bits [7:0]   First tap
line_store7a_output_m   EQU             0x18                    ;Line Store output bits [15:8]
line_store7a_output_h   EQU             0x19                    ;Line Store output bits [23:16]
;
line_store7b_output_l   EQU             0x1a                    ;Line Store output bits [31:24] Second tap
line_store7b_output_m   EQU             0x1b                    ;Line Store output bits [39:32]
line_store7b_output_h   EQU             0x1c                    ;Line Store output bits [47:40]
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
step_counter0           EQU             0x00                    ;decimal count of line store write operations
step_counter1           EQU             0x01
step_counter2           EQU             0x02
step_counter3           EQU             0x03
step_counter4           EQU             0x04
;
test_data_in0           EQU             0x05                    ;24-bit data applied to line store input
test_data_in1           EQU             0x06
test_data_in2           EQU             0x07
;
n_count0                EQU             0x08                    ;decimal count cycles to count in command
n_count1                EQU             0x09
n_count2                EQU             0x0a
n_count3                EQU             0x0b
;
;
auto_inc                EQU             0x0c                    ;Determines if auto increment is active
;
fast_mode               EQU             0x0d                    ;Determines if fast mode is active
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
;
cold_start:             LOAD            s0, #led0
                        OUTPUT          s0, led_port
;
                        CALL            send_welcome            ;Write welcome message to UART
;
                        LOAD            s0, #0x00               ;clear counter
                        STORE           s0, step_counter4
                        STORE           s0, step_counter3
                        STORE           s0, step_counter2
                        STORE           s0, step_counter1
                        STORE           s0, step_counter0
;
                        OUTPUT          s0, line_store_input_l  ;Clear input to line store
                        OUTPUT          s0, line_store_input_m
                        OUTPUT          s0, line_store_input_h
                        STORE           s0, test_data_in0
                        STORE           s0, test_data_in1
                        STORE           s0, test_data_in2
;
;
                        LOAD            s2, #0x0f               ;purge line stores with 4000 writes of zero
                        LOAD            s1, #0xa0
purge_loop:             OUTPUT          s0, line_store_write_en ;dummy write to enable line store
                        SUB             s1, #0x01
                        JUMP            nc, purge_loop
                        SUB             s2, #0x01
                        JUMP            nc, purge_loop
;
;
                        STORE           s0, fast_mode           ;turn off fast mode by default
;
                        LOAD            s0, #0x01               ;default first value is 000001 hex
                        OUTPUT          s0, line_store_input_l
                        STORE           s0, test_data_in0
;
                        STORE           s0, auto_inc            ;turn auto increment on by default
;
;**************************************************************************************
; Main program
;**************************************************************************************
;
; Provides a prompt to which an input with one of the following formats is expected...
;
;     set hhhhhh     - Set value to be written into line store.
;                      'hhhhhh' is a 6 digit hex value.
;
;     cycle n        - drive line store with 'n' cycles and display results.
;                      'n' is a decimal number up to 9999.
;
;     reset          - Clears the counter and also purges all line store of all values.
;
;     auto on        - Turns on the auto increment of the set value so that each
;                      seccessive write of data will be different and useful for
;                      determining the delay length.
;
;     auto off       - Turns off the auto increment function so that all successive writes
;                      to the line stores will be identical.
;
;
;     fast on        - Turns on the fast execute mode in which the output to the
;                      display via the UART is turned off during multi-cycle operations.
;                      This results in much greater speed.
;
;     fast off       - Turns off fast execution mode allowing all data to be displayed.
;
; The input allows a degree of editing to be performed and upper and lower case letters
; to be used.
;
warm_start:             FETCH           s0, auto_inc            ;use LED0 to display state of auto increment
                        FETCH           s1, fast_mode           ;use LED1 to display state of fast mode
                        COMPARE         s1, #0x00
                        JUMP            z, set_leds
                        OR              s0, #led1
set_leds:               OUTPUT          s0, led_port
;
                        CALL            send_prompt             ;Prompt 'KCPSM3>'
                        CALL            receive_string          ;obtain input string of up to 16 characters
                        CALL            upper_case_string       ;convert string to upper case
;
                        LOAD            se, #string_start       ;sE is memory pointer
                        FETCH           s0, @se                 ;test for carriage return
                        COMPARE         s0, #character_cr
                        JUMP            z, warm_start
                        COMPARE         s0, #_character_s       ;test for 'S' of 'SET' command
                        JUMP            z, set_command
                        COMPARE         s0, #_character_c       ;test for 'C' of 'CYCLE' command
                        JUMP            z, cycle_command
                        COMPARE         s0, #_character_r       ;test for 'R' of 'RESET' command
                        JUMP            z, reset_command
                        COMPARE         s0, #_character_a       ;test for 'A' of 'AUTO' command
                        JUMP            z, auto_command
                        COMPARE         s0, #_character_f       ;test for 'F' of 'FAST' command
                        JUMP            z, fast_command
bad_command:            CALL            send_cr                 ;no valid command entered
                        CALL            send_error
                        JUMP            warm_start
;
;Processing potential 'RESET' command
;
reset_command:          CALL            read_next_char          ;test for 'E' of 'RESET' command
                        COMPARE         s0, #_character_e
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;test for 'S' of 'RESET' command
                        COMPARE         s0, #_character_s
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;test for 'E' of 'RESET' command
                        COMPARE         s0, #_character_e
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;test for 'T' of 'RESET' command
                        COMPARE         s0, #_character_t
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;test for a carriage return
                        COMPARE         s0, #character_cr
                        JUMP            nz, bad_command
                        JUMP            cold_start
;
;Processing potential 'SET' command
;
set_command:            CALL            read_next_char          ;test for 'E' of 'SET' command
                        COMPARE         s0, #_character_e
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;test for 'T' of 'SET' command
                        COMPARE         s0, #_character_t
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;test for a space
                        COMPARE         s0, #character_space
                        JUMP            nz, bad_command
;read value into register set [sC,sB,sA]
                        CALL            read_next_char          ;read two character hex value
                        LOAD            s3, s0
                        CALL            read_next_char
                        LOAD            s2, s0
                        CALL            ascii_byte_to_hex       ;convert to value in s0
                        JUMP            c, bad_command
                        LOAD            sc, s0                  ;remember value
                        CALL            read_next_char          ;read two character hex value
                        LOAD            s3, s0
                        CALL            read_next_char
                        LOAD            s2, s0
                        CALL            ascii_byte_to_hex       ;convert to value in s0
                        JUMP            c, bad_command
                        LOAD            sb, s0                  ;remember value
                        CALL            read_next_char          ;read two character hex value
                        LOAD            s3, s0
                        CALL            read_next_char
                        LOAD            s2, s0
                        CALL            ascii_byte_to_hex       ;convert to value in s0
                        JUMP            c, bad_command
                        LOAD            sa, s0                  ;remember value
                        CALL            read_next_char          ;test for carriage return to end command
                        COMPARE         s0, #character_cr
                        JUMP            nz, bad_command
                        STORE           sa, test_data_in0       ;store new line store input value
                        STORE           sb, test_data_in1
                        STORE           sc, test_data_in2
                        OUTPUT          sa, line_store_input_l  ;Write data to register driving line store
                        OUTPUT          sb, line_store_input_m
                        OUTPUT          sc, line_store_input_h
                        CALL            send_ok
                        JUMP            warm_start
;
;Processing potential 'AUTO' command
;
auto_command:           CALL            read_next_char
                        COMPARE         s0, #_character_u       ;test for 'U' of 'AUTO' command
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_t       ;test for 'T' of 'AUTO' command
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_o       ;test for 'O' of 'AUTO' command
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;test for a space
                        COMPARE         s0, #character_space
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_o       ;test for 'O' of 'ON' or 'OFF'
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_n       ;test for 'N' of 'ON'
                        JUMP            z, test_auto_on
                        COMPARE         s0, #_character_f       ;test for 'F' of 'OFF'
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_f       ;test for 'F' of 'OFF'
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #character_cr       ;test for carriage return
                        JUMP            nz, bad_command
                        LOAD            s0, #0x00               ;turn off auto increment
                        JUMP            update_auto
test_auto_on:           CALL            read_next_char
                        COMPARE         s0, #character_cr       ;test for carriage return
                        JUMP            nz, bad_command
                        LOAD            s0, #0x01               ;turn on auto increment
update_auto:            STORE           s0, auto_inc
                        CALL            send_ok
                        JUMP            warm_start
;
;Processing potential 'FAST' command
;
fast_command:           CALL            read_next_char
                        COMPARE         s0, #_character_a       ;test for 'A' of 'FAST' command
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_s       ;test for 'S' of 'FAST' command
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_t       ;test for 'T' of 'FAST' command
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;test for a space
                        COMPARE         s0, #character_space
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_o       ;test for 'O' of 'ON' or 'OFF'
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_n       ;test for 'N' of 'ON'
                        JUMP            z, test_fast_on
                        COMPARE         s0, #_character_f       ;test for 'F' of 'OFF'
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_f       ;test for 'F' of 'OFF'
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #character_cr       ;test for carriage return
                        JUMP            nz, bad_command
                        LOAD            s0, #0x00               ;turn off fast mode
                        JUMP            update_fast
test_fast_on:           CALL            read_next_char
                        COMPARE         s0, #character_cr       ;test for carriage return
                        JUMP            nz, bad_command
                        LOAD            s0, #0x01               ;turn on fast mode
update_fast:            STORE           s0, fast_mode
                        CALL            send_ok
                        JUMP            warm_start
;
;Processing potential 'CYCLE' command
;
cycle_command:          CALL            read_next_char
                        COMPARE         s0, #_character_y       ;test for 'Y' of 'CYCLE' command
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_c       ;test for 'C' of 'CYCLE' command
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_l       ;test for 'L' of 'CYCLE' command
                        JUMP            nz, bad_command
                        CALL            read_next_char
                        COMPARE         s0, #_character_e       ;test for 'E' of 'CYCLE' command
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;test for a space
                        COMPARE         s0, #character_space
                        JUMP            nz, bad_command
                        CALL            read_next_char          ;determine decimal value of 'n' in [s9,s8,s7,s6]
                        COMPARE         s0, #character_cr
                        JUMP            z, bad_command          ;need at least 1 digit
                        CALL            ascii_to_decimal        ;convert to decimal and test
                        JUMP            c, bad_command
                        LOAD            s6, s0
                        LOAD            s7, #0x00
                        LOAD            s8, #0x00
                        LOAD            s9, #0x00
                        CALL            read_next_char
                        COMPARE         s0, #character_cr
                        JUMP            z, store_n
                        CALL            ascii_to_decimal
                        JUMP            c, bad_command
                        LOAD            s7, s6
                        LOAD            s6, s0
                        CALL            read_next_char
                        COMPARE         s0, #character_cr
                        JUMP            z, store_n
                        CALL            ascii_to_decimal
                        JUMP            c, bad_command
                        LOAD            s8, s7
                        LOAD            s7, s6
                        LOAD            s6, s0
                        CALL            read_next_char
                        COMPARE         s0, #character_cr
                        JUMP            z, store_n
                        CALL            ascii_to_decimal
                        JUMP            c, bad_command
                        LOAD            s9, s8
                        LOAD            s8, s7
                        LOAD            s7, s6
                        LOAD            s6, s0
                        CALL            read_next_char
                        COMPARE         s0, #character_cr
                        JUMP            nz, bad_command         ;only 4 digits supported so must be a CR next
store_n:                STORE           s6, n_count0            ;store value of 'n'
                        STORE           s7, n_count1
                        STORE           s8, n_count2
                        STORE           s9, n_count3
                        CALL            send_cr
n_loop:                 FETCH           s6, n_count0            ;Execute cycle command 'n' times
                        FETCH           s7, n_count1
                        FETCH           s8, n_count2
                        FETCH           s9, n_count3
                        SUB             s6, #0x01               ;decrement counter
                        JUMP            nc, update_n
                        LOAD            s6, #0x09
                        SUB             s7, #0x01
                        JUMP            nc, update_n
                        LOAD            s7, #0x09
                        SUB             s8, #0x01
                        JUMP            nc, update_n
                        LOAD            s8, #0x09
                        SUB             s9, #0x01
                        JUMP            nc, update_n
                        JUMP            end_cycle               ;roll under to 9999 signifies end of command
update_n:               STORE           s6, n_count0            ;updated stored value of 'n'
                        STORE           s7, n_count1
                        STORE           s8, n_count2
                        STORE           s9, n_count3
                        FETCH           se, fast_mode           ;determine display mode
                        COMPARE         se, #0x00               ;display active if fast mode is off
                        JUMP            z, step_test
                        LOAD            se, #0x01               ;turn display off for fast mode on
                        COMPARE         s6, #0x00               ;but display last line of cycle command
                        JUMP            nz, step_test
                        COMPARE         s7, #0x00
                        JUMP            nz, step_test
                        COMPARE         s8, #0x00
                        JUMP            nz, step_test
                        COMPARE         s9, #0x00
                        JUMP            nz, step_test
                        LOAD            se, #0x00
step_test:              CALL            step_line_store         ;execute one test step of the line store
                        JUMP            n_loop
end_cycle:              CALL            send_index              ;display index card for data
                        CALL            send_ok
                        JUMP            warm_start
;
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
; Line Store step
;**************************************************************************************
;
; Performs one step of the line stores in which the following sequence is followed.
;
; 1) The cycle counter is incremented and then displayed.
; 2) Display the value about to be stored.
; 3) The current output from each line store is read and displayed.
;       This is representative of the value which would be captured on the next rising clock.
; 4) The clock enable to the line stores is activated storing the value held in the
;       line_store_input register.
; 5) Increment the value to be stored next time if function has been turned on.
;
; The display output is suppressed when register 'sE' is not zero.
;
;
step_line_store:        FETCH           s9, step_counter4       ;increment step counter
                        FETCH           s8, step_counter3
                        FETCH           s7, step_counter2
                        FETCH           s6, step_counter1
                        FETCH           s5, step_counter0
                        ADD             s5, #0x01
                        COMPARE         s5, #0x0a
                        JUMP            nz, store_step_count
                        LOAD            s5, #0x00
                        ADD             s6, #0x01
                        COMPARE         s6, #0x0a
                        JUMP            nz, store_step_count
                        LOAD            s6, #0x00
                        ADD             s7, #0x01
                        COMPARE         s7, #0x0a
                        JUMP            nz, store_step_count
                        LOAD            s7, #0x00
                        ADD             s8, #0x01
                        COMPARE         s8, #0x0a
                        JUMP            nz, store_step_count
                        LOAD            s8, #0x00
                        ADD             s9, #0x01
                        COMPARE         s9, #0x0a
                        JUMP            nz, store_step_count
                        LOAD            s9, #0x00
store_step_count:       STORE           s9, step_counter4
                        STORE           s8, step_counter3
                        STORE           s7, step_counter2
                        STORE           s6, step_counter1
                        STORE           s5, step_counter0
                        COMPARE         se, #0x00               ;suppress display
                        JUMP            nz, skip_display
                        LOAD            uart_data, s9           ;display step counter
                        ADD             uart_data, #0x30
                        CALL            send_to_uart
                        LOAD            uart_data, s8
                        ADD             uart_data, #0x30
                        CALL            send_to_uart
                        LOAD            uart_data, s7
                        ADD             uart_data, #0x30
                        CALL            send_to_uart
                        LOAD            uart_data, s6
                        ADD             uart_data, #0x30
                        CALL            send_to_uart
                        LOAD            uart_data, s5
                        ADD             uart_data, #0x30
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_space
                        CALL            send_space
                        FETCH           sa, test_data_in0       ;Read set value into [sC,sB,sA]
                        FETCH           sb, test_data_in1
                        FETCH           sc, test_data_in2
                        FETCH           s0, auto_inc
                        LOAD            s0, sc                  ;display value being input to line store
                        CALL            send_hex_byte
                        LOAD            s0, sb
                        CALL            send_hex_byte
                        LOAD            s0, sa
                        CALL            send_hex_byte
                        CALL            send_space
                        CALL            send_space
                        INPUT           s0, line_store1_output_h ;read 24-bit line store 1 output and display
                        CALL            send_hex_byte
                        INPUT           s0, line_store1_output_m
                        CALL            send_hex_byte
                        INPUT           s0, line_store1_output_l
                        CALL            send_hex_byte
                        CALL            send_space
                        CALL            send_space
                        INPUT           s0, line_store2_output_h ;read 18-bit line store 2 output and display
                        CALL            send_nibble
                        INPUT           s0, line_store2_output_m
                        CALL            send_hex_byte
                        INPUT           s0, line_store2_output_l
                        CALL            send_hex_byte
                        CALL            send_space
                        CALL            send_space
                        INPUT           s0, line_store3_output_h ;read 13-bit line store 3 output and display
                        CALL            send_hex_byte
                        INPUT           s0, line_store3_output_l
                        CALL            send_hex_byte
                        CALL            send_space
                        CALL            send_space
                        INPUT           s0, line_store4a_output_h ;read 24-bit line store 4 output 'A' and display
                        CALL            send_hex_byte
                        INPUT           s0, line_store4a_output_m
                        CALL            send_hex_byte
                        INPUT           s0, line_store4a_output_l
                        CALL            send_hex_byte
                        CALL            send_space
                        INPUT           s0, line_store4b_output_h ;read 24-bit line store 4 output 'B' and display
                        CALL            send_hex_byte
                        INPUT           s0, line_store4b_output_m
                        CALL            send_hex_byte
                        INPUT           s0, line_store4b_output_l
                        CALL            send_hex_byte
                        CALL            send_space
                        INPUT           s0, line_store4c_output_h ;read 24-bit line store 4 output 'C' and display
                        CALL            send_hex_byte
                        INPUT           s0, line_store4c_output_m
                        CALL            send_hex_byte
                        INPUT           s0, line_store4c_output_l
                        CALL            send_hex_byte
                        CALL            send_space
                        CALL            send_space
                        INPUT           s0, line_store5_output_h ;read 12-bit line store 5 output and display
                        CALL            send_nibble
                        INPUT           s0, line_store5_output_l
                        CALL            send_hex_byte
                        CALL            send_space
                        CALL            send_space
                        INPUT           s0, line_store6_output_h ;read 9-bit line store 6 output and display
                        CALL            send_nibble
                        INPUT           s0, line_store6_output_l
                        CALL            send_hex_byte
                        CALL            send_space
                        CALL            send_space
                        INPUT           s0, line_store7a_output_h ;read 24-bit line store 7 output 'A' and display
                        CALL            send_hex_byte
                        INPUT           s0, line_store7a_output_m
                        CALL            send_hex_byte
                        INPUT           s0, line_store7a_output_l
                        CALL            send_hex_byte
                        CALL            send_space
                        INPUT           s0, line_store7b_output_h ;read 24-bit line store 7 output 'B' and display
                        CALL            send_hex_byte
                        INPUT           s0, line_store7b_output_m
                        CALL            send_hex_byte
                        INPUT           s0, line_store7b_output_l
                        CALL            send_hex_byte
                        CALL            send_cr
;
;
skip_display:           OUTPUT          s0, line_store_write_en ;dummy write to enable line stores
                        FETCH           s0, auto_inc            ;increment input value if auto is 'on'
                        ADD             sa, s0
                        ADDCY           sb, #0x00
                        ADDCY           sc, #0x00
                        STORE           sa, test_data_in0       ;store new line store input value
                        STORE           sb, test_data_in1
                        STORE           sc, test_data_in2
                        OUTPUT          sa, line_store_input_l  ;Write data to register driving line store
                        OUTPUT          sb, line_store_input_m
                        OUTPUT          sc, line_store_input_h
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
; Receive ASCII string from UART
;**************************************************************************************
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
;
; Routine to convert ASCII data in 's0' to an equivalent DECIMAL value.
;
; If character is not valid for decimal, then CARRY is set on return.
;
; Register used s0
;
ascii_to_decimal:       ADD             s0, #0xc6               ;test for above ASCII code 39 ('9')
                        RETURN          c
                        SUB             s0, #0xf6               ;normalise to 0 to 9
                        RETURN                                  ;carry set for ASCII code below 30 ('0')
;
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
; Send the single HEX value representing the lower 4-bits of the register 's0'
; to the UART
;
; Registers used s0, s1, s2
;
send_nibble:            CALL            hex_to_ascii
                        LOAD            uart_data, s0
                        CALL            send_to_uart
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
                        LOAD            uart_data, #_character_l
                        CALL            send_to_uart
                        LOAD            uart_data, #character_i
                        CALL            send_to_uart
                        LOAD            uart_data, #character_n
                        CALL            send_to_uart
                        LOAD            uart_data, #character_e
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_data, #_character_s
                        CALL            send_to_uart
                        LOAD            uart_data, #character_t
                        CALL            send_to_uart
                        LOAD            uart_data, #character_o
                        CALL            send_to_uart
                        LOAD            uart_data, #character_r
                        CALL            send_to_uart
                        LOAD            uart_data, #character_e
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_data, #_character_t
                        CALL            send_to_uart
                        LOAD            uart_data, #character_e
                        CALL            send_to_uart
                        LOAD            uart_data, #character_s
                        CALL            send_to_uart
                        LOAD            uart_data, #character_t
                        CALL            send_to_uart
                        LOAD            uart_data, #character_e
                        CALL            send_to_uart
                        LOAD            uart_data, #character_r
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
                        CALL            send_cr
                        LOAD            uart_data, #_character_o
                        CALL            send_to_uart
                        LOAD            uart_data, #_character_k
                        CALL            send_to_uart
                        JUMP            send_cr
;
;
;Send index to data being displayed
;
send_index:             CALL            send_cr
                        LOAD            uart_data, #character_c
                        CALL            send_to_uart
                        LOAD            uart_data, #character_y
                        CALL            send_to_uart
                        LOAD            uart_data, #character_c
                        CALL            send_to_uart
                        LOAD            uart_data, #character_l
                        CALL            send_to_uart
                        LOAD            uart_data, #character_e
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_space
                        CALL            send_space
                        CALL            send_space
                        LOAD            uart_data, #character_i
                        CALL            send_to_uart
                        LOAD            uart_data, #character_n
                        CALL            send_to_uart
                        LOAD            uart_data, #character_p
                        CALL            send_to_uart
                        LOAD            uart_data, #character_u
                        CALL            send_to_uart
                        LOAD            uart_data, #character_t
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_space
                        CALL            send_space
                        CALL            send_space
                        CALL            send_space
                        LOAD            uart_data, #character_7
                        CALL            send_to_uart
                        LOAD            uart_data, #character_6
                        CALL            send_to_uart
                        LOAD            uart_data, #character_8
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_space
                        CALL            send_space
                        LOAD            uart_data, #character_1
                        CALL            send_to_uart
                        LOAD            uart_data, #character_0
                        CALL            send_to_uart
                        LOAD            uart_data, #character_2
                        CALL            send_to_uart
                        LOAD            uart_data, #character_4
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_space
                        CALL            send_1280
                        CALL            send_space
                        CALL            send_space
                        CALL            send_space
                        CALL            send_1280
                        LOAD            uart_data, #character_a
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_space
                        CALL            send_1280
                        LOAD            uart_data, #character_b
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_space
                        CALL            send_1280
                        LOAD            uart_data, #character_c
                        CALL            send_to_uart
                        CALL            send_space
                        LOAD            uart_data, #character_1
                        CALL            send_to_uart
                        LOAD            uart_data, #character_5
                        CALL            send_to_uart
                        LOAD            uart_data, #character_3
                        CALL            send_to_uart
                        LOAD            uart_data, #character_6
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_1920
                        CALL            send_space
                        CALL            send_space
                        CALL            send_space
                        CALL            send_1920
                        LOAD            uart_data, #character_a
                        CALL            send_to_uart
                        CALL            send_space
                        CALL            send_space
                        CALL            send_1920
                        LOAD            uart_data, #character_b
                        CALL            send_to_uart
                        CALL            send_cr
                        RETURN
;
send_1280:              LOAD            uart_data, #character_1
                        CALL            send_to_uart
                        LOAD            uart_data, #character_2
                        CALL            send_to_uart
                        LOAD            uart_data, #character_8
                        CALL            send_to_uart
                        LOAD            uart_data, #character_0
                        CALL            send_to_uart
                        RETURN
;
send_1920:              LOAD            uart_data, #character_1
                        CALL            send_to_uart
                        LOAD            uart_data, #character_9
                        CALL            send_to_uart
                        LOAD            uart_data, #character_2
                        CALL            send_to_uart
                        LOAD            uart_data, #character_0
                        CALL            send_to_uart
                        RETURN
;
;**************************************************************************************
; Interrupt Service Routine (ISR)
;**************************************************************************************
;
; Interrupts are not used in this program.
;
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

