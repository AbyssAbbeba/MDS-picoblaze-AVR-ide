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
                     CONSTANT LED_port, 80                  ;8 simple LEDs
                     CONSTANT LED0, 01                      ;     LED 0 - bit0
                     CONSTANT LED1, 02                      ;         1 - bit1
                     CONSTANT LED2, 04                      ;         2 - bit2
                     CONSTANT LED3, 08                      ;         3 - bit3
                     CONSTANT LED4, 10                      ;         4 - bit4
                     CONSTANT LED5, 20                      ;         5 - bit5
                     CONSTANT LED6, 40                      ;         6 - bit6
                     CONSTANT LED7, 80                      ;         7 - bit7
                     ;
                     ;
                     ;UART ports
                     ;
                     CONSTANT status_port, 00               ;UART status input
                     CONSTANT tx_half_full, 01              ;  Transmitter     half full - bit0
                     CONSTANT tx_full, 02                   ;    FIFO               full - bit1
                     CONSTANT rx_data_present, 04           ;  Receiver     data present - bit2
                     CONSTANT rx_half_full, 08              ;    FIFO          half full - bit3
                     CONSTANT rx_full, 10                   ;                   full - bit4
                     ;
                     CONSTANT UART_read_port, 01            ;UART Rx data input
                     ;
                     CONSTANT UART_write_port, 40           ;UART Tx data output
                     ;
                     ;
                     ;The first line store input is 18-bits and requires 3 bytes
                     ;to be written to a holding register. Then when required,
                     ;the whole 18-bit value can be stored in the line store
                     ;using a dummy write (data not used) to a forth port.
                     ;
                     ;
                     CONSTANT line_store_input_L, 01        ;Line Store input bits [7:0]
                     CONSTANT line_store_input_M, 02        ;Line Store input bits [15:8]
                     CONSTANT line_store_input_H, 04        ;Line Store input bits [17:16]
                     CONSTANT line_store_write_en, 08       ;Line Store clock enable (dummy write)
                     ;
                     ;
                     ;
                     ;The first line store 768x24 and requires 3 bytes to be read.
                     ;
                     CONSTANT line_store1_output_L, 02      ;Line Store output bits [7:0]
                     CONSTANT line_store1_output_M, 03      ;Line Store output bits [15:8]
                     CONSTANT line_store1_output_H, 04      ;Line Store output bits [23:16]
                     ;
                     ;
                     ;The second line store 1024x18 and requires 3 bytes to be read.
                     ;
                     CONSTANT line_store2_output_L, 05      ;Line Store output bits [7:0]
                     CONSTANT line_store2_output_M, 06      ;Line Store output bits [15:8]
                     CONSTANT line_store2_output_H, 07      ;Line Store output bits [17:16]
                     ;
                     ;
                     ;The third line store 1280x13 and requires 2 bytes to be read.
                     ;
                     CONSTANT line_store3_output_L, 08      ;Line Store output bits [7:0]
                     CONSTANT line_store3_output_H, 09      ;Line Store output bits [12:8]
                     ;
                     ;
                     ;The forth line store is 1280x72 so to make it more manageable it has been
                     ;folded to make it 3 times longer and only 24-bits wide. This requires 3 bytes
                     ;to be read at each 1280 delay tapping point.
                     ;
                     CONSTANT line_store4a_output_L, 0A     ;Line Store output bits [7:0]   First tap
                     CONSTANT line_store4a_output_M, 0B     ;Line Store output bits [15:8]
                     CONSTANT line_store4a_output_H, 0C     ;Line Store output bits [23:16]
                     ;
                     CONSTANT line_store4b_output_L, 0D     ;Line Store output bits [31:24] Second tap
                     CONSTANT line_store4b_output_M, 0E     ;Line Store output bits [39:32]
                     CONSTANT line_store4b_output_H, 0F     ;Line Store output bits [47:40]
                     ;
                     CONSTANT line_store4c_output_L, 10     ;Line Store output bits [55:48] Third tap
                     CONSTANT line_store4c_output_M, 11     ;Line Store output bits [63:56]
                     CONSTANT line_store4c_output_H, 12     ;Line Store output bits [71:64]
                     ;
                     ;
                     ;The fifth line store 1536x12 and requires 2 bytes to be read.
                     ;
                     CONSTANT line_store5_output_L, 13      ;Line Store output bits [7:0]
                     CONSTANT line_store5_output_H, 14      ;Line Store output bits [11:8]
                     ;
                     ;
                     ;The sixth line store 1920x9 and requires 2 bytes to be read.
                     ;
                     CONSTANT line_store6_output_L, 15      ;Line Store output bits [7:0]
                     CONSTANT line_store6_output_H, 16      ;Line Store output bit [8]
                     ;
                     ;
                     ;The seventh line store is 1920x48 so to make it more manageable it has been
                     ;folded to make it 2 times longer and only 24-bits wide. This requires 3 bytes
                     ;to be read at each 1920 delay tapping point.
                     ;
                     CONSTANT line_store7a_output_L, 17     ;Line Store output bits [7:0]   First tap
                     CONSTANT line_store7a_output_M, 18     ;Line Store output bits [15:8]
                     CONSTANT line_store7a_output_H, 19     ;Line Store output bits [23:16]
                     ;
                     CONSTANT line_store7b_output_L, 1A     ;Line Store output bits [31:24] Second tap
                     CONSTANT line_store7b_output_M, 1B     ;Line Store output bits [39:32]
                     CONSTANT line_store7b_output_H, 1C     ;Line Store output bits [47:40]
                     ;
                     ;
                     ;**************************************************************************************
                     ; Special Register usage
                     ;**************************************************************************************
                     ;
                     NAMEREG sF, UART_data                  ;used to pass data to and from the UART
                     ;
                     ;
                     ;
                     ;**************************************************************************************
                     ;Scratch Pad Memory Locations
                     ;**************************************************************************************
                     ;
                     CONSTANT step_counter0, 00             ;decimal count of line store write operations
                     CONSTANT step_counter1, 01
                     CONSTANT step_counter2, 02
                     CONSTANT step_counter3, 03
                     CONSTANT step_counter4, 04
                     ;
                     CONSTANT test_data_in0, 05             ;24-bit data applied to line store input
                     CONSTANT test_data_in1, 06
                     CONSTANT test_data_in2, 07
                     ;
                     CONSTANT n_count0, 08                  ;decimal count cycles to count in command
                     CONSTANT n_count1, 09
                     CONSTANT n_count2, 0A
                     CONSTANT n_count3, 0B
                     ;
                     ;
                     CONSTANT auto_inc, 0C                  ;Determines if auto increment is active
                     ;
                     CONSTANT fast_mode, 0D                 ;Determines if fast mode is active
                     ;
                     ;UART character strings will be stored in scratch pad memory ending in carriage return.
                     ;A string can be up to 16 characters with the start location defined by this constant.
                     ;
                     CONSTANT string_start, 30
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
                     CONSTANT character_stop, 2E
                     CONSTANT character_semi_colon, 3B
                     CONSTANT character_minus, 2D
                     CONSTANT character_divide, 2F          ;'/'
                     CONSTANT character_plus, 2B
                     CONSTANT character_comma, 2C
                     CONSTANT character_less_than, 3C
                     CONSTANT character_greater_than, 3E
                     CONSTANT character_equals, 3D
                     CONSTANT character_space, 20
                     CONSTANT character_CR, 0D              ;carriage return
                     CONSTANT character_question, 3F        ;'?'
                     CONSTANT character_dollar, 24
                     CONSTANT character_exclaim, 21         ;'!'
                     CONSTANT character_BS, 08              ;Back Space command character
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
         cold_start: LOAD s0, LED0
                     OUTPUT s0, LED_port
                     ;
                     CALL send_welcome                      ;Write welcome message to UART
                     ;
                     LOAD s0, 00                            ;clear counter
                     STORE s0, step_counter4
                     STORE s0, step_counter3
                     STORE s0, step_counter2
                     STORE s0, step_counter1
                     STORE s0, step_counter0
                     ;
                     OUTPUT s0, line_store_input_L          ;Clear input to line store
                     OUTPUT s0, line_store_input_M
                     OUTPUT s0, line_store_input_H
                     STORE s0, test_data_in0
                     STORE s0, test_data_in1
                     STORE s0, test_data_in2
                     ;
                     ;
                     LOAD s2, 0F                            ;purge line stores with 4000 writes of zero
                     LOAD s1, A0
         purge_loop: OUTPUT s0, line_store_write_en         ;dummy write to enable line store
                     SUB s1, 01
                     JUMP NC, purge_loop
                     SUB s2, 01
                     JUMP NC, purge_loop
                     ;
                     ;
                     STORE s0, fast_mode                    ;turn off fast mode by default
                     ;
                     LOAD s0, 01                            ;default first value is 000001 hex
                     OUTPUT s0, line_store_input_L
                     STORE s0, test_data_in0
                     ;
                     STORE s0, auto_inc                     ;turn auto increment on by default
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
         warm_start: FETCH s0, auto_inc                     ;use LED0 to display state of auto increment
                     FETCH s1, fast_mode                    ;use LED1 to display state of fast mode
                     COMPARE s1, 00
                     JUMP Z, set_LEDs
                     OR s0, LED1
           set_LEDs: OUTPUT s0, LED_port
                     ;
                     CALL send_prompt                       ;Prompt 'KCPSM3>'
                     CALL receive_string                    ;obtain input string of up to 16 characters
                     CALL upper_case_string                 ;convert string to upper case
                     ;
                     LOAD sE, string_start                  ;sE is memory pointer
                     FETCH s0, (sE)                         ;test for carriage return
                     COMPARE s0, character_CR
                     JUMP Z, warm_start
                     COMPARE s0, character_S                ;test for 'S' of 'SET' command
                     JUMP Z, SET_command
                     COMPARE s0, character_C                ;test for 'C' of 'CYCLE' command
                     JUMP Z, CYCLE_command
                     COMPARE s0, character_R                ;test for 'R' of 'RESET' command
                     JUMP Z, RESET_command
                     COMPARE s0, character_A                ;test for 'A' of 'AUTO' command
                     JUMP Z, AUTO_command
                     COMPARE s0, character_F                ;test for 'F' of 'FAST' command
                     JUMP Z, FAST_command
        bad_command: CALL send_CR                           ;no valid command entered
                     CALL send_Error
                     JUMP warm_start
                     ;
                     ;Processing potential 'RESET' command
                     ;
      RESET_command: CALL read_next_char                    ;test for 'E' of 'RESET' command
                     COMPARE s0, character_E
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;test for 'S' of 'RESET' command
                     COMPARE s0, character_S
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;test for 'E' of 'RESET' command
                     COMPARE s0, character_E
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;test for 'T' of 'RESET' command
                     COMPARE s0, character_T
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;test for a carriage return
                     COMPARE s0, character_CR
                     JUMP NZ, bad_command
                     JUMP cold_start
                     ;
                     ;Processing potential 'SET' command
                     ;
        SET_command: CALL read_next_char                    ;test for 'E' of 'SET' command
                     COMPARE s0, character_E
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;test for 'T' of 'SET' command
                     COMPARE s0, character_T
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;test for a space
                     COMPARE s0, character_space
                     JUMP NZ, bad_command
                     ;read value into register set [sC,sB,sA]
                     CALL read_next_char                    ;read two character hex value
                     LOAD s3, s0
                     CALL read_next_char
                     LOAD s2, s0
                     CALL ASCII_byte_to_hex                 ;convert to value in s0
                     JUMP C, bad_command
                     LOAD sC, s0                            ;remember value
                     CALL read_next_char                    ;read two character hex value
                     LOAD s3, s0
                     CALL read_next_char
                     LOAD s2, s0
                     CALL ASCII_byte_to_hex                 ;convert to value in s0
                     JUMP C, bad_command
                     LOAD sB, s0                            ;remember value
                     CALL read_next_char                    ;read two character hex value
                     LOAD s3, s0
                     CALL read_next_char
                     LOAD s2, s0
                     CALL ASCII_byte_to_hex                 ;convert to value in s0
                     JUMP C, bad_command
                     LOAD sA, s0                            ;remember value
                     CALL read_next_char                    ;test for carriage return to end command
                     COMPARE s0, character_CR
                     JUMP NZ, bad_command
                     STORE sA, test_data_in0                ;store new line store input value
                     STORE sB, test_data_in1
                     STORE sC, test_data_in2
                     OUTPUT sA, line_store_input_L          ;Write data to register driving line store
                     OUTPUT sB, line_store_input_M
                     OUTPUT sC, line_store_input_H
                     CALL send_OK
                     JUMP warm_start
                     ;
                     ;Processing potential 'AUTO' command
                     ;
       AUTO_command: CALL read_next_char
                     COMPARE s0, character_U                ;test for 'U' of 'AUTO' command
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_T                ;test for 'T' of 'AUTO' command
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_O                ;test for 'O' of 'AUTO' command
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;test for a space
                     COMPARE s0, character_space
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_O                ;test for 'O' of 'ON' or 'OFF'
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_N                ;test for 'N' of 'ON'
                     JUMP Z, test_auto_ON
                     COMPARE s0, character_F                ;test for 'F' of 'OFF'
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_F                ;test for 'F' of 'OFF'
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_CR               ;test for carriage return
                     JUMP NZ, bad_command
                     LOAD s0, 00                            ;turn off auto increment
                     JUMP update_auto
       test_auto_ON: CALL read_next_char
                     COMPARE s0, character_CR               ;test for carriage return
                     JUMP NZ, bad_command
                     LOAD s0, 01                            ;turn on auto increment
        update_auto: STORE s0, auto_inc
                     CALL send_OK
                     JUMP warm_start
                     ;
                     ;Processing potential 'FAST' command
                     ;
       FAST_command: CALL read_next_char
                     COMPARE s0, character_A                ;test for 'A' of 'FAST' command
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_S                ;test for 'S' of 'FAST' command
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_T                ;test for 'T' of 'FAST' command
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;test for a space
                     COMPARE s0, character_space
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_O                ;test for 'O' of 'ON' or 'OFF'
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_N                ;test for 'N' of 'ON'
                     JUMP Z, test_fast_ON
                     COMPARE s0, character_F                ;test for 'F' of 'OFF'
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_F                ;test for 'F' of 'OFF'
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_CR               ;test for carriage return
                     JUMP NZ, bad_command
                     LOAD s0, 00                            ;turn off fast mode
                     JUMP update_fast
       test_fast_ON: CALL read_next_char
                     COMPARE s0, character_CR               ;test for carriage return
                     JUMP NZ, bad_command
                     LOAD s0, 01                            ;turn on fast mode
        update_fast: STORE s0, fast_mode
                     CALL send_OK
                     JUMP warm_start
                     ;
                     ;Processing potential 'CYCLE' command
                     ;
      CYCLE_command: CALL read_next_char
                     COMPARE s0, character_Y                ;test for 'Y' of 'CYCLE' command
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_C                ;test for 'C' of 'CYCLE' command
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_L                ;test for 'L' of 'CYCLE' command
                     JUMP NZ, bad_command
                     CALL read_next_char
                     COMPARE s0, character_E                ;test for 'E' of 'CYCLE' command
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;test for a space
                     COMPARE s0, character_space
                     JUMP NZ, bad_command
                     CALL read_next_char                    ;determine decimal value of 'n' in [s9,s8,s7,s6]
                     COMPARE s0, character_CR
                     JUMP Z, bad_command                    ;need at least 1 digit
                     CALL ASCII_to_decimal                  ;convert to decimal and test
                     JUMP C, bad_command
                     LOAD s6, s0
                     LOAD s7, 00
                     LOAD s8, 00
                     LOAD s9, 00
                     CALL read_next_char
                     COMPARE s0, character_CR
                     JUMP Z, store_n
                     CALL ASCII_to_decimal
                     JUMP C, bad_command
                     LOAD s7, s6
                     LOAD s6, s0
                     CALL read_next_char
                     COMPARE s0, character_CR
                     JUMP Z, store_n
                     CALL ASCII_to_decimal
                     JUMP C, bad_command
                     LOAD s8, s7
                     LOAD s7, s6
                     LOAD s6, s0
                     CALL read_next_char
                     COMPARE s0, character_CR
                     JUMP Z, store_n
                     CALL ASCII_to_decimal
                     JUMP C, bad_command
                     LOAD s9, s8
                     LOAD s8, s7
                     LOAD s7, s6
                     LOAD s6, s0
                     CALL read_next_char
                     COMPARE s0, character_CR
                     JUMP NZ, bad_command                   ;only 4 digits supported so must be a CR next
            store_n: STORE s6, n_count0                     ;store value of 'n'
                     STORE s7, n_count1
                     STORE s8, n_count2
                     STORE s9, n_count3
                     CALL send_CR
             n_loop: FETCH s6, n_count0                     ;Execute cycle command 'n' times
                     FETCH s7, n_count1
                     FETCH s8, n_count2
                     FETCH s9, n_count3
                     SUB s6, 01                             ;decrement counter
                     JUMP NC, update_n
                     LOAD s6, 09
                     SUB s7, 01
                     JUMP NC, update_n
                     LOAD s7, 09
                     SUB s8, 01
                     JUMP NC, update_n
                     LOAD s8, 09
                     SUB s9, 01
                     JUMP NC, update_n
                     JUMP end_cycle                         ;roll under to 9999 signifies end of command
           update_n: STORE s6, n_count0                     ;updated stored value of 'n'
                     STORE s7, n_count1
                     STORE s8, n_count2
                     STORE s9, n_count3
                     FETCH sE, fast_mode                    ;determine display mode
                     COMPARE sE, 00                         ;display active if fast mode is off
                     JUMP Z, step_test
                     LOAD sE, 01                            ;turn display off for fast mode on
                     COMPARE s6, 00                         ;but display last line of cycle command
                     JUMP NZ, step_test
                     COMPARE s7, 00
                     JUMP NZ, step_test
                     COMPARE s8, 00
                     JUMP NZ, step_test
                     COMPARE s9, 00
                     JUMP NZ, step_test
                     LOAD sE, 00
          step_test: CALL step_line_store                   ;execute one test step of the line store
                     JUMP n_loop
          end_cycle: CALL send_index                        ;display index card for data
                     CALL send_OK
                     JUMP warm_start
                     ;
                     ;
                     ;Read next character from scratch pad memory
                     ;
     read_next_char: ADD sE, 01
                     FETCH s0, (sE)                         ;test for space
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
    step_line_store: FETCH s9, step_counter4                ;increment step counter
                     FETCH s8, step_counter3
                     FETCH s7, step_counter2
                     FETCH s6, step_counter1
                     FETCH s5, step_counter0
                     ADD s5, 01
                     COMPARE s5, 0A
                     JUMP NZ, store_step_count
                     LOAD s5, 00
                     ADD s6, 01
                     COMPARE s6, 0A
                     JUMP NZ, store_step_count
                     LOAD s6, 00
                     ADD s7, 01
                     COMPARE s7, 0A
                     JUMP NZ, store_step_count
                     LOAD s7, 00
                     ADD s8, 01
                     COMPARE s8, 0A
                     JUMP NZ, store_step_count
                     LOAD s8, 00
                     ADD s9, 01
                     COMPARE s9, 0A
                     JUMP NZ, store_step_count
                     LOAD s9, 00
   store_step_count: STORE s9, step_counter4
                     STORE s8, step_counter3
                     STORE s7, step_counter2
                     STORE s6, step_counter1
                     STORE s5, step_counter0
                     COMPARE sE, 00                         ;suppress display
                     JUMP NZ, skip_display
                     LOAD UART_data, s9                     ;display step counter
                     ADD UART_data, 30
                     CALL send_to_UART
                     LOAD UART_data, s8
                     ADD UART_data, 30
                     CALL send_to_UART
                     LOAD UART_data, s7
                     ADD UART_data, 30
                     CALL send_to_UART
                     LOAD UART_data, s6
                     ADD UART_data, 30
                     CALL send_to_UART
                     LOAD UART_data, s5
                     ADD UART_data, 30
                     CALL send_to_UART
                     CALL send_space
                     CALL send_space
                     CALL send_space
                     FETCH sA, test_data_in0                ;Read set value into [sC,sB,sA]
                     FETCH sB, test_data_in1
                     FETCH sC, test_data_in2
                     FETCH s0, auto_inc
                     LOAD s0, sC                            ;display value being input to line store
                     CALL send_hex_byte
                     LOAD s0, sB
                     CALL send_hex_byte
                     LOAD s0, sA
                     CALL send_hex_byte
                     CALL send_space
                     CALL send_space
                     INPUT s0, line_store1_output_H         ;read 24-bit line store 1 output and display
                     CALL send_hex_byte
                     INPUT s0, line_store1_output_M
                     CALL send_hex_byte
                     INPUT s0, line_store1_output_L
                     CALL send_hex_byte
                     CALL send_space
                     CALL send_space
                     INPUT s0, line_store2_output_H         ;read 18-bit line store 2 output and display
                     CALL send_nibble
                     INPUT s0, line_store2_output_M
                     CALL send_hex_byte
                     INPUT s0, line_store2_output_L
                     CALL send_hex_byte
                     CALL send_space
                     CALL send_space
                     INPUT s0, line_store3_output_H         ;read 13-bit line store 3 output and display
                     CALL send_hex_byte
                     INPUT s0, line_store3_output_L
                     CALL send_hex_byte
                     CALL send_space
                     CALL send_space
                     INPUT s0, line_store4a_output_H        ;read 24-bit line store 4 output 'A' and display
                     CALL send_hex_byte
                     INPUT s0, line_store4a_output_M
                     CALL send_hex_byte
                     INPUT s0, line_store4a_output_L
                     CALL send_hex_byte
                     CALL send_space
                     INPUT s0, line_store4b_output_H        ;read 24-bit line store 4 output 'B' and display
                     CALL send_hex_byte
                     INPUT s0, line_store4b_output_M
                     CALL send_hex_byte
                     INPUT s0, line_store4b_output_L
                     CALL send_hex_byte
                     CALL send_space
                     INPUT s0, line_store4c_output_H        ;read 24-bit line store 4 output 'C' and display
                     CALL send_hex_byte
                     INPUT s0, line_store4c_output_M
                     CALL send_hex_byte
                     INPUT s0, line_store4c_output_L
                     CALL send_hex_byte
                     CALL send_space
                     CALL send_space
                     INPUT s0, line_store5_output_H         ;read 12-bit line store 5 output and display
                     CALL send_nibble
                     INPUT s0, line_store5_output_L
                     CALL send_hex_byte
                     CALL send_space
                     CALL send_space
                     INPUT s0, line_store6_output_H         ;read 9-bit line store 6 output and display
                     CALL send_nibble
                     INPUT s0, line_store6_output_L
                     CALL send_hex_byte
                     CALL send_space
                     CALL send_space
                     INPUT s0, line_store7a_output_H        ;read 24-bit line store 7 output 'A' and display
                     CALL send_hex_byte
                     INPUT s0, line_store7a_output_M
                     CALL send_hex_byte
                     INPUT s0, line_store7a_output_L
                     CALL send_hex_byte
                     CALL send_space
                     INPUT s0, line_store7b_output_H        ;read 24-bit line store 7 output 'B' and display
                     CALL send_hex_byte
                     INPUT s0, line_store7b_output_M
                     CALL send_hex_byte
                     INPUT s0, line_store7b_output_L
                     CALL send_hex_byte
                     CALL send_CR
                     ;
                     ;
       skip_display: OUTPUT s0, line_store_write_en         ;dummy write to enable line stores
                     FETCH s0, auto_inc                     ;increment input value if auto is 'on'
                     ADD sA, s0
                     ADDCY sB, 00
                     ADDCY sC, 00
                     STORE sA, test_data_in0                ;store new line store input value
                     STORE sB, test_data_in1
                     STORE sC, test_data_in2
                     OUTPUT sA, line_store_input_L          ;Write data to register driving line store
                     OUTPUT sB, line_store_input_M
                     OUTPUT sC, line_store_input_H
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
     receive_string: LOAD s1, string_start                  ;locate start of string
                     LOAD s2, s1                            ;compute 16 character address
                     ADD s2, 10
  receive_full_test: INPUT s0, status_port                  ;test Rx_FIFO buffer for full
                     TEST s0, rx_full
                     JUMP NZ, read_error
                     CALL read_from_UART                    ;obtain and echo character
                     CALL send_to_UART
                     STORE UART_data, (s1)                  ;write to memory
                     COMPARE UART_data, character_CR        ;test for end of string
                     RETURN Z
                     COMPARE UART_data, character_BS        ;test for back space
                     JUMP Z, BS_edit
                     ADD s1, 01                             ;increment memory pointer
                     COMPARE s1, s2                         ;test for pointer exceeding 16 characters
                     JUMP NZ, receive_full_test             ;next character
                     CALL send_backspace                    ;hold end of string position on terminal display
            BS_edit: SUB s1, 01                             ;memory pointer back one
                     COMPARE s1, string_start               ;test for under flow
                     JUMP C, string_start_again
                     CALL send_space                        ;clear character at current position
                     CALL send_backspace                    ;position cursor
                     JUMP receive_full_test                 ;next character
 string_start_again: CALL send_greater_than                 ;restore '>' at prompt
                     JUMP receive_string                    ;begin again
                     ;Receiver buffer overflow condition
         read_error: CALL send_CR                           ;Transmit error message
                     STORE UART_data, string_start          ;empty string in memory (start with CR)
                     CALL send_Overflow_Error
                     CALL send_CR
 clear_UART_Rx_loop: INPUT s0, status_port                  ;test Rx_FIFO buffer for data
                     TEST s0, rx_data_present
                     RETURN Z                               ;finish when buffer is empty
                     INPUT UART_data, UART_read_port        ;read from FIFO and ignore
                     JUMP clear_UART_Rx_loop
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
         upper_case: COMPARE s0, 61                         ;eliminate character codes below 'a' (61 hex)
                     RETURN C
                     COMPARE s0, 7B                         ;eliminate character codes above 'z' (7A hex)
                     RETURN NC
                     AND s0, DF                             ;mask bit5 to convert to upper case
                     RETURN
                     ;
                     ;
                     ;
                     ; Convert string held in scratch pad memory to upper case.
                     ;
                     ; Registers used s0, s1
                     ;
  upper_case_string: LOAD s1, string_start
           ucs_loop: FETCH s0, (s1)
                     COMPARE s0, character_CR
                     RETURN Z
                     CALL upper_case
                     STORE s0, (s1)
                     ADD s1, 01
                     JUMP ucs_loop
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
                     ;
                     ;
                     ; Routine to convert ASCII data in 's0' to an equivalent DECIMAL value.
                     ;
                     ; If character is not valid for decimal, then CARRY is set on return.
                     ;
                     ; Register used s0
                     ;
   ASCII_to_decimal: ADD s0, C6                             ;test for above ASCII code 39 ('9')
                     RETURN C
                     SUB s0, F6                             ;normalise to 0 to 9
                     RETURN                                 ;carry set for ASCII code below 30 ('0')
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
                     ; Send the single HEX value representing the lower 4-bits of the register 's0'
                     ; to the UART
                     ;
                     ; Registers used s0, s1, s2
                     ;
        send_nibble: CALL hex_to_ASCII
                     LOAD UART_data, s0
                     CALL send_to_UART
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
                     ;
                     ;Send a back space to the UART
                     ;
     send_backspace: LOAD UART_data, character_BS
                     CALL send_to_UART
                     RETURN
                     ;
                     ;
                     ; Send 'PicoBlaze Servo Control' string to the UART
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
                     LOAD UART_data, character_i
                     CALL send_to_UART
                     LOAD UART_data, character_n
                     CALL send_to_UART
                     LOAD UART_data, character_e
                     CALL send_to_UART
                     CALL send_space
                     LOAD UART_data, character_S
                     CALL send_to_UART
                     LOAD UART_data, character_t
                     CALL send_to_UART
                     LOAD UART_data, character_o
                     CALL send_to_UART
                     LOAD UART_data, character_r
                     CALL send_to_UART
                     LOAD UART_data, character_e
                     CALL send_to_UART
                     CALL send_space
                     LOAD UART_data, character_T
                     CALL send_to_UART
                     LOAD UART_data, character_e
                     CALL send_to_UART
                     LOAD UART_data, character_s
                     CALL send_to_UART
                     LOAD UART_data, character_t
                     CALL send_to_UART
                     LOAD UART_data, character_e
                     CALL send_to_UART
                     LOAD UART_data, character_r
                     CALL send_to_UART
                     CALL send_CR
                     CALL send_CR
                     RETURN
                     ;
                     ;
                     ;Send 'KCPSM3>' prompt to the UART
                     ;
        send_prompt: CALL send_CR                           ;start new line
                     LOAD UART_data, character_K
                     CALL send_to_UART
                     LOAD UART_data, character_C
                     CALL send_to_UART
                     LOAD UART_data, character_P
                     CALL send_to_UART
                     LOAD UART_data, character_S
                     CALL send_to_UART
                     LOAD UART_data, character_M
                     CALL send_to_UART
                     LOAD UART_data, character_3
                     CALL send_to_UART
                     ;
                     ;Send '>' character to the UART
                     ;
  send_greater_than: LOAD UART_data, character_greater_than
                     CALL send_to_UART
                     RETURN
                     ;
                     ;
                     ;Send 'Overflow Error' to the UART
                     ;
send_Overflow_Error: LOAD UART_data, character_O
                     CALL send_to_UART
                     LOAD UART_data, character_v
                     CALL send_to_UART
                     LOAD UART_data, character_e
                     CALL send_to_UART
                     LOAD UART_data, character_r
                     CALL send_to_UART
                     LOAD UART_data, character_f
                     CALL send_to_UART
                     LOAD UART_data, character_l
                     CALL send_to_UART
                     LOAD UART_data, character_o
                     CALL send_to_UART
                     LOAD UART_data, character_w
                     CALL send_to_UART
   send_space_Error: CALL send_space
                     ;
                     ;Send 'Error' to the UART
                     ;
         send_Error: LOAD UART_data, character_E
                     CALL send_to_UART
                     LOAD UART_data, character_r
                     CALL send_to_UART
                     CALL send_to_UART
                     LOAD UART_data, character_o
                     CALL send_to_UART
                     LOAD UART_data, character_r
                     CALL send_to_UART
                     JUMP send_CR
                     ;
                     ;
                     ;Send 'OK' to the UART
                     ;
            send_OK: CALL send_CR
                     CALL send_CR
                     LOAD UART_data, character_O
                     CALL send_to_UART
                     LOAD UART_data, character_K
                     CALL send_to_UART
                     JUMP send_CR
                     ;
                     ;
                     ;Send index to data being displayed
                     ;
         send_index: CALL send_CR
                     LOAD UART_data, character_c
                     CALL send_to_UART
                     LOAD UART_data, character_y
                     CALL send_to_UART
                     LOAD UART_data, character_c
                     CALL send_to_UART
                     LOAD UART_data, character_l
                     CALL send_to_UART
                     LOAD UART_data, character_e
                     CALL send_to_UART
                     CALL send_space
                     CALL send_space
                     CALL send_space
                     CALL send_space
                     LOAD UART_data, character_i
                     CALL send_to_UART
                     LOAD UART_data, character_n
                     CALL send_to_UART
                     LOAD UART_data, character_p
                     CALL send_to_UART
                     LOAD UART_data, character_u
                     CALL send_to_UART
                     LOAD UART_data, character_t
                     CALL send_to_UART
                     CALL send_space
                     CALL send_space
                     CALL send_space
                     CALL send_space
                     CALL send_space
                     LOAD UART_data, character_7
                     CALL send_to_UART
                     LOAD UART_data, character_6
                     CALL send_to_UART
                     LOAD UART_data, character_8
                     CALL send_to_UART
                     CALL send_space
                     CALL send_space
                     CALL send_space
                     LOAD UART_data, character_1
                     CALL send_to_UART
                     LOAD UART_data, character_0
                     CALL send_to_UART
                     LOAD UART_data, character_2
                     CALL send_to_UART
                     LOAD UART_data, character_4
                     CALL send_to_UART
                     CALL send_space
                     CALL send_space
                     CALL send_1280
                     CALL send_space
                     CALL send_space
                     CALL send_space
                     CALL send_1280
                     LOAD UART_data, character_a
                     CALL send_to_UART
                     CALL send_space
                     CALL send_space
                     CALL send_1280
                     LOAD UART_data, character_b
                     CALL send_to_UART
                     CALL send_space
                     CALL send_space
                     CALL send_1280
                     LOAD UART_data, character_c
                     CALL send_to_UART
                     CALL send_space
                     LOAD UART_data, character_1
                     CALL send_to_UART
                     LOAD UART_data, character_5
                     CALL send_to_UART
                     LOAD UART_data, character_3
                     CALL send_to_UART
                     LOAD UART_data, character_6
                     CALL send_to_UART
                     CALL send_space
                     CALL send_1920
                     CALL send_space
                     CALL send_space
                     CALL send_space
                     CALL send_1920
                     LOAD UART_data, character_a
                     CALL send_to_UART
                     CALL send_space
                     CALL send_space
                     CALL send_1920
                     LOAD UART_data, character_b
                     CALL send_to_UART
                     CALL send_CR
                     RETURN
                     ;
          send_1280: LOAD UART_data, character_1
                     CALL send_to_UART
                     LOAD UART_data, character_2
                     CALL send_to_UART
                     LOAD UART_data, character_8
                     CALL send_to_UART
                     LOAD UART_data, character_0
                     CALL send_to_UART
                     RETURN
                     ;
          send_1920: LOAD UART_data, character_1
                     CALL send_to_UART
                     LOAD UART_data, character_9
                     CALL send_to_UART
                     LOAD UART_data, character_2
                     CALL send_to_UART
                     LOAD UART_data, character_0
                     CALL send_to_UART
                     RETURN
                     ;
                     ;**************************************************************************************
                     ; Interrupt Service Routine (ISR)
                     ;**************************************************************************************
                     ;
                     ; Interrupts are not used in this program.
                     ;
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