;KCPSM3 Program - FC_CTRL
                      ;
                      ;Frequency Generator Control
                      ;Calculates and displays results on the 16x2 LCD display.
                      ;
                      ;
                      ; Version : 1.12
                      ; Date : 28th February 2006
                      ;
                      ; Ken Chapman
                      ; Xilinx Ltd
                      ;
                      ; chapman@xilinx.com
                      ;
                      ;
                      ;
                      ;**************************************************************************************
                      ;Port definitions
                      ;**************************************************************************************
                      ;
                      CONSTANT A_count0_port, 00          ;32-bit A-counter (LSByte first)
                      CONSTANT A_count1_port, 10
                      CONSTANT A_count2_port, 20
                      CONSTANT A_count3_port, 30
                      ;
                      CONSTANT B_count0_port, 40          ;32-bit B-counter (LSByte first)
                      CONSTANT B_count1_port, 50
                      CONSTANT B_count2_port, 60
                      CONSTANT B_count3_port, 70
                      ;
                      CONSTANT status_port, 80            ;4 switches and counter status
                      CONSTANT switch0, 01                ;  Switches      SW0 - bit0
                      CONSTANT switch1, 02                ; active High    SW1 - bit1
                      CONSTANT switch2, 04                ;                SW2 - bit2
                      CONSTANT switch3, 08                ;                SW3 - bit3
                      CONSTANT AB_switch, 10              ;  0=A-count enabled 1=B-count enabled
                      ;
                      ;
                      CONSTANT count_resetport, 02        ;Reset frequency counter controls
                      CONSTANT a_count_reset, 01          ;  A-count = bit0
                      CONSTANT b_count_reset, 02          ;  B-count = bit1
                      ;
                      CONSTANT LED_port, 01               ;8 simple LEDs - active high
                      ;
                      ;
                      CONSTANT source_control_port, 08    ;Select and control test sources
                      CONSTANT source_sel0, 01            ;  00 = SMA clock  01=50MHz
                      CONSTANT source_sel1, 02            ;  10 = DCM Osc    11=Ring Osc
                      CONSTANT ring_reset, 40             ; active High rest of ring osc - bit6
                      CONSTANT dcm_kick, 80               ; DCM kick start signal - bit7
                      ;
                      ;
                      ;LCD interface ports
                      ;
                      ;The master enable signal is not used by the LCD display itself
                      ;but may be required to confirm that LCD communication is active.
                      ;This is required on the Spartan-3E Starter Kit if the StrataFLASH
                      ;is used because it shares the same data pins and conflicts must be avoided.
                      ;
                      CONSTANT LCD_output_port, 04        ;LCD character module output data and control
                      CONSTANT LCD_E, 01                  ;   active High Enable        E - bit0
                      CONSTANT LCD_RW, 02                 ;   Read=1 Write=0           RW - bit1
                      CONSTANT LCD_RS, 04                 ;   Instruction=0 Data=1     RS - bit2
                      CONSTANT LCD_drive, 08              ;   Master enable (active High) - bit3
                      CONSTANT LCD_DB4, 10                ;   4-bit              Data DB4 - bit4
                      CONSTANT LCD_DB5, 20                ;   interface          Data DB5 - bit5
                      CONSTANT LCD_DB6, 40                ;                      Data DB6 - bit6
                      CONSTANT LCD_DB7, 80                ;                      Data DB7 - bit7
                      ;
                      ;
                      CONSTANT LCD_input_port, 09         ;LCD character module input data
                      CONSTANT LCD_read_spare0, 01        ;    Spare bits               - bit0
                      CONSTANT LCD_read_spare1, 02        ;    are zero                 - bit1
                      CONSTANT LCD_read_spare2, 04        ;                             - bit2
                      CONSTANT LCD_read_spare3, 08        ;                             - bit3
                      CONSTANT LCD_read_DB4, 10           ;    4-bit           Data DB4 - bit4
                      CONSTANT LCD_read_DB5, 20           ;    interface       Data DB5 - bit5
                      CONSTANT LCD_read_DB6, 40           ;                    Data DB6 - bit6
                      CONSTANT LCD_read_DB7, 80           ;                    Data DB7 - bit7
                      ;
                      ;
                      ;
                      ;Special Register usage
                      ;
                      ;
                      ;
                      ;**************************************************************************************
                      ;Scratch Pad Memory Locations
                      ;**************************************************************************************
                      ;
                      ;
                      CONSTANT count0, 00                 ;last 32-bit counter value (LSByte first)
                      CONSTANT count1, 01
                      CONSTANT count2, 02
                      CONSTANT count3, 03
                      ;
                      CONSTANT ISR_count, 04              ;count number of interrupts for a clean start
                      ;
                      CONSTANT decimal0, 11               ;10 digit decimal value up to 4,294,967,295
                      CONSTANT decimal1, 12
                      CONSTANT decimal2, 13
                      CONSTANT decimal3, 14
                      CONSTANT decimal4, 15
                      CONSTANT decimal5, 16
                      CONSTANT decimal6, 17
                      CONSTANT decimal7, 18
                      CONSTANT decimal8, 19
                      CONSTANT decimal9, 1A
                      ;
                      ;
                      ;
                      CONSTANT preserve_s0, 30            ;place to save register contents
                      CONSTANT preserve_s1, 31
                      CONSTANT preserve_s2, 32
                      CONSTANT preserve_s3, 33
                      CONSTANT preserve_s4, 34
                      CONSTANT preserve_s5, 35
                      CONSTANT preserve_s6, 36
                      CONSTANT preserve_s7, 37
                      CONSTANT preserve_s8, 38
                      CONSTANT preserve_s9, 39
                      CONSTANT preserve_sA, 3A
                      CONSTANT preserve_sB, 3B
                      CONSTANT preserve_sC, 3C
                      CONSTANT preserve_sD, 3D
                      CONSTANT preserve_sE, 3E
                      CONSTANT preserve_sF, 3F
                      ;
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
                      CONSTANT character_divide, 2F       ;'/'
                      CONSTANT character_plus, 2B
                      CONSTANT character_comma, 2C
                      CONSTANT character_less_than, 3C
                      CONSTANT character_greater_than, 3E
                      CONSTANT character_equals, 3D
                      CONSTANT character_space, 20
                      CONSTANT character_CR, 0D           ;carriage return
                      CONSTANT character_question, 3F     ;'?'
                      CONSTANT character_dollar, 24
                      CONSTANT character_exclaim, 21      ;'!'
                      CONSTANT character_BS, 08           ;Back Space command character
                      ;
                      ;
                      ;
                      ;
                      ;
                      ;**************************************************************************************
                      ;Initialise the system
                      ;**************************************************************************************
                      ;
          cold_start: CALL LCD_reset                      ;initialise LCD display
                      LOAD s0, 00                         ;Turn off LEDs
                      OUTPUT s0, LED_port
                      ;
                      ;
                      ;Write welcome message to LCD display
                      ;
                      LOAD s5, 10                         ;Line 1 position 0
                      CALL LCD_cursor
                      CALL disp_PicoBlaze                 ;Display 'PicoBlaze Inside'
                      CALL delay_1s                       ;wait 3 seconds
                      CALL delay_1s
                      CALL delay_1s
                      LOAD s5, 10                         ;Line 1 position 0
                      CALL LCD_cursor
                      CALL disp_Frequency                 ;Display 'Frequency Counter V1.00'
                      LOAD s5, 21                         ;Line 2 position 1
                      CALL LCD_cursor
                      CALL disp_Counter
                      LOAD s5, 2B                         ;Line 2 position 11
                      CALL LCD_cursor
                      CALL disp_version
                      CALL delay_1s                       ;wait 5 seconds
                      CALL delay_1s
                      CALL delay_1s
                      CALL delay_1s
                      CALL delay_1s
                      CALL LCD_clear                      ;Clear display
                      ;
                      ;Kick start the DCM oscillator.
                      ; Just requires a few cyles of activity
                      ;
                      LOAD s0, FF
                      LOAD s1, 00
           kick_loop: OUTPUT s1, source_control_port
                      XOR s1, dcm_kick                    ;toggle kick start signal
                      SUB s0, 01
                      JUMP NZ, kick_loop
                      ;
                      ;clear all scratch pad memory locations
                      ;
                      LOAD s1, 3F
                      LOAD s0, 00
           clear_spm: STORE s0, (s1)
                      SUB s1, 01
                      JUMP NC, clear_spm
                      ;
                      ENABLE INTERRUPT
                      ;
                      ;**************************************************************************************
                      ;Main Program
                      ;**************************************************************************************
                      ;
                      ;The task of the main program is just to read the most recent values from
                      ;scratch pad memory and display them as fast as it can.
                      ;
                      ;It also reads the slide switches controls the selection of the source frequency to
                      ;be measured.
                      ;
          warm_start: LOAD s5, 21                         ;Line 2 position 1
                      CALL LCD_cursor
                      INPUT sF, status_port               ;select source based on switches
                      COMPARE sF, 00                      ;test for no switches active
                      AND sF, 0F                          ;isolate switches
                      JUMP NZ, test_SMA
                      CALL disp_menu
                      JUMP warm_start
            test_SMA: COMPARE sF, switch0
                      JUMP NZ, test_50M
                      CALL disp_SMA_input
                      LOAD sF, 00
                      JUMP select_source
            test_50M: COMPARE sF, switch1
                      JUMP NZ, test_DCM
                      CALL disp_50MHz_Crystal
                      LOAD sF, 01
                      JUMP select_source
            test_DCM: COMPARE sF, switch2
                      JUMP NZ, test_Ring
                      CALL disp_DCM_Oscillator
                      LOAD sF, 02
                      JUMP select_source
           test_Ring: COMPARE sF, switch3
                      JUMP Z, Ring_select
                      CALL disp_menu                      ;more than one switch is set
                      JUMP warm_start
         Ring_select: CALL disp_Ring_Oscillator
                      LOAD sF, 03
       select_source: OUTPUT sF, source_control_port      ;select source control
                      ;
                      ;Read the most recent values from display on LCD.
                      ;
                      ;Interrupts will be disabled during the reading of values to ensure a clean
                      ;value is obtained when reading multi-byte values.
                      ;
                      ;
                      ;Display the count value in the top right of the LCD display
                      ;Up to 999,999,999
                      ;
                      DISABLE INTERRUPT                   ;copy cycle count to register set [s5,s4,s3,s2]
                      FETCH s2, count0
                      FETCH s3, count1
                      FETCH s4, count2
                      FETCH s5, count3
                      ENABLE INTERRUPT
                      CALL integer_to_BCD                 ;convert last 32-bit value to BCD digits
                      LOAD s5, 10                         ;Line 1 position 0
                      CALL LCD_cursor
                      LOAD s6, decimal8                   ;up to 999,999,999 Hz
                      CALL disp_digits
                      JUMP warm_start
                      ;
                      ;
                      ;
                      ;**************************************************************************************
                      ; Display frequency value on LCD display
                      ;**************************************************************************************
                      ;
                      ;
                      ;Display value on the LCD display at current position.
                      ;The values to be displayed must be stored in scratch pad memory
                      ;locations 'decimal0' to 'decimal9' which must be in ascending locations.
                      ;
                      ;The routing performs leading zero suppression and scales to Hz, KHz or MHz ranges.
                      ;
                      ;Registers used s0,s1,s4,s5,sE,sF
                      ;
         disp_digits: LOAD sF, FF                         ;set blanking flag
                      LOAD sE, character_space            ;scaling character for MHz, KHz or Hz
                      FETCH s5, decimal8                  ;100MHz digit
                      CALL zero_test
                      CALL disp_digit
                      FETCH s5, decimal7                  ;10MHz digit
                      CALL zero_test
                      CALL disp_digit
                      FETCH s5, decimal6                  ;1MHz digit
                      CALL zero_test
                      CALL disp_digit
                      COMPARE sF, FF                      ;check if any MHz were active
                      JUMP Z, khz_space
                      LOAD s5, character_stop
                      CALL LCD_write_data
                      LOAD sE, character_M
                      JUMP khz_digits
           khz_space: LOAD s5, character_space
                      CALL LCD_write_data
          khz_digits: FETCH s5, decimal5                  ;100KHz digit
                      CALL zero_test
                      CALL disp_digit
                      FETCH s5, decimal4                  ;10KHz digit
                      CALL zero_test
                      CALL disp_digit
                      FETCH s5, decimal3                  ;1KHz digit
                      CALL zero_test
                      CALL disp_digit
                      COMPARE sE, character_M             ;check if any MHz were active
                      JUMP Z, hz_space
                      COMPARE sF, FF                      ;check if any KHz were active
                      JUMP Z, hz_space
                      LOAD s5, character_stop
                      CALL LCD_write_data
                      LOAD sE, character_K
                      JUMP hz_digits
            hz_space: LOAD s5, character_space
                      CALL LCD_write_data
           hz_digits: FETCH s5, decimal2                  ;100KHz digit
                      CALL zero_test
                      CALL disp_digit
                      FETCH s5, decimal1                  ;10KHz digit
                      CALL zero_test
                      CALL disp_digit
                      FETCH s5, decimal0                  ;1KHz digit (always displayed)
                      ADD s5, character_0                 ;convert number to ASCII
                      CALL LCD_write_data
                      LOAD s5, character_space
                      CALL LCD_write_data
                      LOAD s5, sE
                      CALL LCD_write_data
                      LOAD s5, character_H
                      CALL LCD_write_data
                      LOAD s5, character_z
                      CALL LCD_write_data
                      LOAD s5, character_space            ;ensure end of line is clear
                      CALL LCD_write_data
                      RETURN
                      ;
                      ;Check digit for zero. If not zero then clear
                      ;blanking flag (sF=00)
           zero_test: COMPARE s5, 00
                      RETURN Z
                      LOAD sF, 00
                      RETURN
                      ;
                      ;Display single digit at current position
                      ;or space if blanking (sF=FF) is active
                      ;
          disp_digit: COMPARE sF, FF
                      JUMP Z, blank_digit
                      ADD s5, character_0                 ;convert number to ASCII
                      CALL LCD_write_data
                      RETURN
         blank_digit: LOAD s5, character_space
                      CALL LCD_write_data
                      RETURN
                      ;
                      ;
                      ;
                      ;**************************************************************************************
                      ; 32-bit integer to BCD conversion
                      ;**************************************************************************************
                      ;
                      ;Convert the 32 bit value in register set [s5,s4,s3,s2]
                      ;into the BCD decimal equivalent located in the scratch pad memory
                      ;locations 'decimal0' to 'decimal9' which must be in ascending locations.
                      ;
                      ;Each digit is formed in turn starting with the least significant.
                      ;
                      ;Registers used s0,s2,s3,s4,s5,s6,s7,s8,s9,sA,sB,sC,sD,sE,sF
                      ;
      integer_to_BCD: LOAD sE, 0A                         ;10 digits to be formed from value upto 4294967295
                      LOAD sF, decimal0                   ;pointer for LS-Digit
     int_to_BCD_loop: CALL divide_32bit_by_10
                      STORE s1, (sF)                      ;remainder becomes digit value
                      ADD sF, 01                          ;move to next most significant digit
                      SUB sE, 01                          ;one less digit to compute
                      JUMP NZ, int_to_BCD_loop
                      RETURN
                      ;
                      ;Divide 32-bit binary integer by 10
                      ;
                      ;The value to be divided is held in register set [s5,s4,s3,s2]
                      ;and this is where the result is returned to.
                      ;
                      ;At then end of the integer division the remainder in the range 0 to 9
                      ;will be in register s1.
                      ;
                      ;Registers used s0, s2,s3,s4,s5,s6,s7,s8,s9,sA,sB,sC,sD
                      ;
  divide_32bit_by_10: LOAD sA, s2                         ;copy input value to set [sD,sC,sB,sA]
                      LOAD sB, s3
                      LOAD sC, s4
                      LOAD sD, s5
                      LOAD s2, 00                         ;clear result
                      LOAD s3, 00
                      LOAD s4, 00
                      LOAD s5, 00
                      LOAD s9, A0                         ;initialise '10' value into msb's of set [s9,s8,s7,s6]
                      LOAD s8, 00
                      LOAD s7, 00
                      LOAD s6, 00
                      LOAD s0, 1D                         ;29 subtract and shift iterations to be performed
          div10_loop: SUB sA, s6                          ;perform 32-bit subtract [sD,sC,sB,sA]-[s9,s8,s7,s6]
                      SUBCY sB, s7
                      SUBCY sC, s8
                      SUBCY sD, s9
                      JUMP C, div10_restore
                      SL1 s2                              ;shift '1' into result
                      JUMP div10_shifts
       div10_restore: ADD sA, s6                          ;perform 32-bit addition [sD,sC,sB,sA]+[s9,s8,s7,s6]
                      ADDCY sB, s7
                      ADDCY sC, s8
                      ADDCY sD, s9
                      SL0 s2                              ;shift '0' into result
        div10_shifts: SLA s3                              ;complete 32-bit shift left
                      SLA s4
                      SLA s5
                      SR0 s9                              ;divide '10' value by 2 (shift right 1 place)
                      SRA s8
                      SRA s7
                      SRA s6
                      SUB s0, 01                          ;count iterations
                      JUMP NZ, div10_loop
                      LOAD s1, sA                         ;remainder of division
                      RETURN
                      ;
                      ;
                      ;
                      ;
                      ;**************************************************************************************
                      ;LCD text messages
                      ;**************************************************************************************
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
                      ;Display 'Frequency' on LCD at current cursor position
                      ;
                      ;
      disp_Frequency: LOAD s5, character_F
                      CALL LCD_write_data
                      LOAD s5, character_r
                      CALL LCD_write_data
                      LOAD s5, character_e
                      CALL LCD_write_data
                      LOAD s5, character_q
                      CALL LCD_write_data
                      LOAD s5, character_u
                      CALL LCD_write_data
                      LOAD s5, character_e
                      CALL LCD_write_data
                      LOAD s5, character_n
                      CALL LCD_write_data
                      LOAD s5, character_c
                      CALL LCD_write_data
                      LOAD s5, character_y
                      CALL LCD_write_data
                      RETURN
                      ;
                      ;
                      ;Display 'Counter' on LCD at current cursor position
                      ;
                      ;
        disp_Counter: LOAD s5, character_C
                      CALL LCD_write_data
                      LOAD s5, character_o
                      CALL LCD_write_data
                      LOAD s5, character_u
                      CALL LCD_write_data
                      LOAD s5, character_n
                      CALL LCD_write_data
                      LOAD s5, character_t
                      CALL LCD_write_data
                      LOAD s5, character_e
                      CALL LCD_write_data
                      LOAD s5, character_r
                      CALL LCD_write_data
                      RETURN
                      ;
                      ;Display version number on LCD at current cursor position
                      ;
                      ;
        disp_version: LOAD s5, character_v
                      CALL LCD_write_data
                      LOAD s5, character_1
                      CALL LCD_write_data
                      LOAD s5, character_stop
                      CALL LCD_write_data
                      LOAD s5, character_0
                      CALL LCD_write_data
                      LOAD s5, character_0
                      CALL LCD_write_data
                      RETURN
                      ;
                      ;
                      ;Display 'SMA input' at current cursor position
                      ;
                      ;
      disp_SMA_input: LOAD s5, character_S
                      CALL LCD_write_data
                      LOAD s5, character_M
                      CALL LCD_write_data
                      LOAD s5, character_A
                      CALL LCD_write_data
                      LOAD s5, character_space
                      CALL LCD_write_data
                      LOAD s5, character_i
                      CALL LCD_write_data
                      LOAD s5, character_n
                      CALL LCD_write_data
                      LOAD s5, character_p
                      CALL LCD_write_data
                      LOAD s5, character_u
                      CALL LCD_write_data
                      LOAD s5, character_t
                      CALL LCD_write_data
                      LOAD sF, 06
                      JUMP disp_spaces
                      ;
                      ;
                      ;
                      ;Display '50MHz Crystal' at current cursor position
                      ;
                      ;
  disp_50MHz_Crystal: LOAD s5, character_5
                      CALL LCD_write_data
                      LOAD s5, character_0
                      CALL LCD_write_data
                      LOAD s5, character_M
                      CALL LCD_write_data
                      LOAD s5, character_H
                      CALL LCD_write_data
                      LOAD s5, character_z
                      CALL LCD_write_data
                      LOAD s5, character_space
                      CALL LCD_write_data
                      LOAD s5, character_C
                      CALL LCD_write_data
                      LOAD s5, character_r
                      CALL LCD_write_data
                      LOAD s5, character_y
                      CALL LCD_write_data
                      LOAD s5, character_s
                      CALL LCD_write_data
                      LOAD s5, character_t
                      CALL LCD_write_data
                      LOAD s5, character_a
                      CALL LCD_write_data
                      LOAD s5, character_l
                      CALL LCD_write_data
                      LOAD sF, 02
                      JUMP disp_spaces
                      ;
                      ;
                      ;
                      ;Display 'DCM oscillator' at current cursor position
                      ;
                      ;
 disp_DCM_Oscillator: LOAD s5, character_D
                      CALL LCD_write_data
                      LOAD s5, character_C
                      CALL LCD_write_data
                      LOAD s5, character_M
                      CALL LCD_write_data
     disp_Oscillator: LOAD s5, character_space
                      CALL LCD_write_data
                      LOAD s5, character_O
                      CALL LCD_write_data
                      LOAD s5, character_s
                      CALL LCD_write_data
                      LOAD s5, character_c
                      CALL LCD_write_data
                      LOAD s5, character_i
                      CALL LCD_write_data
                      LOAD s5, character_l
                      CALL LCD_write_data
                      CALL LCD_write_data
                      LOAD s5, character_a
                      CALL LCD_write_data
                      LOAD s5, character_t
                      CALL LCD_write_data
                      LOAD s5, character_o
                      CALL LCD_write_data
                      LOAD s5, character_r
                      CALL LCD_write_data
                      LOAD s5, character_space
                      CALL LCD_write_data
                      RETURN
                      ;
                      ;
                      ;
                      ;Display 'Ring oscillator' at current cursor position
                      ;
                      ;
disp_Ring_Oscillator: LOAD s5, character_R
                      CALL LCD_write_data
                      LOAD s5, character_i
                      CALL LCD_write_data
                      LOAD s5, character_n
                      CALL LCD_write_data
                      LOAD s5, character_g
                      CALL LCD_write_data
                      JUMP disp_Oscillator
                      ;
                      ;
                      ;Display spaces at current cursor position
                      ;Number of spaces to be specified in register sF
                      ;
         disp_spaces: COMPARE sF, 00
                      RETURN Z
                      LOAD s5, character_space
                      CALL LCD_write_data
                      SUB sF, 01
                      JUMP disp_spaces
                      ;
                      ;Display switch setting menu on entire display.
                      ;
           disp_menu: LOAD s5, 10                         ;Line 1 position 0
                      CALL LCD_cursor
                      LOAD s5, character_R
                      CALL LCD_write_data
                      LOAD s5, character_i
                      CALL LCD_write_data
                      LOAD s5, character_n
                      CALL LCD_write_data
                      LOAD s5, character_g
                      CALL LCD_write_data
                      LOAD s5, character_space
                      CALL LCD_write_data
                      LOAD s5, character_D
                      CALL LCD_write_data
                      LOAD s5, character_C
                      CALL LCD_write_data
                      LOAD s5, character_M
                      CALL LCD_write_data
                      LOAD s5, character_space
                      CALL LCD_write_data
                      LOAD s5, character_5
                      CALL LCD_write_data
                      LOAD s5, character_0
                      CALL LCD_write_data
                      LOAD s5, character_M
                      CALL LCD_write_data
                      LOAD s5, character_space
                      CALL LCD_write_data
                      LOAD s5, character_S
                      CALL LCD_write_data
                      LOAD s5, character_M
                      CALL LCD_write_data
                      LOAD s5, character_A
                      CALL LCD_write_data
                      LOAD s5, 20                         ;Line 2 position 0
                      CALL LCD_cursor
                      LOAD s5, character_space
                      CALL LCD_write_data
                      LOAD s5, character_S
                      CALL LCD_write_data
                      LOAD s5, character_W
                      CALL LCD_write_data
                      LOAD s5, character_3
                      CALL LCD_write_data
                      LOAD s5, character_space
                      CALL LCD_write_data
                      LOAD s5, character_S
                      CALL LCD_write_data
                      LOAD s5, character_W
                      CALL LCD_write_data
                      LOAD s5, character_2
                      CALL LCD_write_data
                      LOAD s5, character_space
                      CALL LCD_write_data
                      LOAD s5, character_S
                      CALL LCD_write_data
                      LOAD s5, character_W
                      CALL LCD_write_data
                      LOAD s5, character_1
                      CALL LCD_write_data
                      LOAD s5, character_space
                      CALL LCD_write_data
                      LOAD s5, character_S
                      CALL LCD_write_data
                      LOAD s5, character_W
                      CALL LCD_write_data
                      LOAD s5, character_0
                      CALL LCD_write_data
                      RETURN
                      ;
                      ;
                      ;
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
          delay_40us: LOAD s1, 28                         ;40 x 1us = 40us
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
           delay_1ms: LOAD s2, 19                         ;25 x 40us = 1ms
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
          delay_20ms: LOAD s3, 14                         ;20 x 1ms = 20ms
           wait_20ms: CALL delay_1ms
                      SUB s3, 01
                      JUMP NZ, wait_20ms
                      RETURN
                      ;
                      ;Delay of approximately 1 second.
                      ;
                      ;Registers used s0, s1, s2, s3, s4
                      ;
            delay_1s: LOAD s4, 32                         ;50 x 20ms = 1000ms
             wait_1s: CALL delay_20ms
                      SUB s4, 01
                      JUMP NZ, wait_1s
                      RETURN
                      ;
                      ;
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
         LCD_pulse_E: XOR s4, LCD_E                       ;E=1
                      OUTPUT s4, LCD_output_port
                      CALL delay_1us
                      XOR s4, LCD_E                       ;E=0
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
     LCD_write_inst4: AND s4, F8                          ;Enable=1 RS=0 Instruction, RW=0 Write, E=0
                      OUTPUT s4, LCD_output_port          ;set up RS and RW >40ns before enable pulse
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
                      AND s4, F0                          ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                      OR s4, LCD_drive                    ;Enable=1
                      CALL LCD_write_inst4                ;write upper nibble
                      CALL delay_1us                      ;wait >1us
                      LOAD s4, s5                         ;select lower nibble with
                      SL1 s4                              ;Enable=1
                      SL0 s4                              ;RS=0 Instruction
                      SL0 s4                              ;RW=0 Write
                      SL0 s4                              ;E=0
                      CALL LCD_write_inst4                ;write lower nibble
                      CALL delay_40us                     ;wait >40us
                      LOAD s4, F0                         ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                      OUTPUT s4, LCD_output_port          ;Release master enable
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
                      AND s4, F0                          ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                      OR s4, 0C                           ;Enable=1 RS=1 Data, RW=0 Write, E=0
                      OUTPUT s4, LCD_output_port          ;set up RS and RW >40ns before enable pulse
                      CALL LCD_pulse_E                    ;write upper nibble
                      CALL delay_1us                      ;wait >1us
                      LOAD s4, s5                         ;select lower nibble with
                      SL1 s4                              ;Enable=1
                      SL1 s4                              ;RS=1 Data
                      SL0 s4                              ;RW=0 Write
                      SL0 s4                              ;E=0
                      OUTPUT s4, LCD_output_port          ;set up RS and RW >40ns before enable pulse
                      CALL LCD_pulse_E                    ;write lower nibble
                      CALL delay_40us                     ;wait >40us
                      LOAD s4, F0                         ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                      OUTPUT s4, LCD_output_port          ;Release master enable
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
      LCD_read_data8: LOAD s4, 0E                         ;Enable=1 RS=1 Data, RW=1 Read, E=0
                      OUTPUT s4, LCD_output_port          ;set up RS and RW >40ns before enable pulse
                      XOR s4, LCD_E                       ;E=1
                      OUTPUT s4, LCD_output_port
                      CALL delay_1us                      ;wait >260ns to access data
                      INPUT s5, LCD_input_port            ;read upper nibble
                      XOR s4, LCD_E                       ;E=0
                      OUTPUT s4, LCD_output_port
                      CALL delay_1us                      ;wait >1us
                      XOR s4, LCD_E                       ;E=1
                      OUTPUT s4, LCD_output_port
                      CALL delay_1us                      ;wait >260ns to access data
                      INPUT s0, LCD_input_port            ;read lower nibble
                      XOR s4, LCD_E                       ;E=0
                      OUTPUT s4, LCD_output_port
                      AND s5, F0                          ;merge upper and lower nibbles
                      SR0 s0
                      SR0 s0
                      SR0 s0
                      SR0 s0
                      OR s5, s0
                      LOAD s4, 04                         ;Enable=0 RS=1 Data, RW=0 Write, E=0
                      OUTPUT s4, LCD_output_port          ;Stop reading 5V device and release master enable
                      CALL delay_40us                     ;wait >40us
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
           LCD_reset: CALL delay_20ms                     ;wait more that 15ms for display to be ready
                      LOAD s4, 30
                      CALL LCD_write_inst4                ;send '3'
                      CALL delay_20ms                     ;wait >4.1ms
                      CALL LCD_write_inst4                ;send '3'
                      CALL delay_1ms                      ;wait >100us
                      CALL LCD_write_inst4                ;send '3'
                      CALL delay_40us                     ;wait >40us
                      LOAD s4, 20
                      CALL LCD_write_inst4                ;send '2'
                      CALL delay_40us                     ;wait >40us
                      LOAD s5, 28                         ;Function set
                      CALL LCD_write_inst8
                      LOAD s5, 06                         ;Entry mode
                      CALL LCD_write_inst8
                      LOAD s5, 0C                         ;Display control
                      CALL LCD_write_inst8
           LCD_clear: LOAD s5, 01                         ;Display clear
                      CALL LCD_write_inst8
                      CALL delay_1ms                      ;wait >1.64ms for display to clear
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
          LCD_cursor: TEST s5, 10                         ;test for line 1
                      JUMP Z, set_line2
                      AND s5, 0F                          ;make address in range 80 to 8F for line 1
                      OR s5, 80
                      CALL LCD_write_inst8                ;instruction write to set cursor
                      RETURN
           set_line2: AND s5, 0F                          ;make address in range C0 to CF for line 2
                      OR s5, C0
                      CALL LCD_write_inst8                ;instruction write to set cursor
                      RETURN
                      ;
                      ;
                      ;
                      ;**************************************************************************************
                      ;Interrupt Service Routine
                      ;**************************************************************************************
                      ;
                      ;
                      ;Each interrupt means that there is a new count value to be read.
                      ;However the first 4 interrupts are ignored other than to clear the counter to
                      ;ensure that even the first reading is for one complete period.
                      ;
                      ;After reading the active counter, all calculations are performed and values stored
                      ;in scratch pad memory are updated to reflect the new values.
                      ;
                      ;Registers are preserved and restored by the ISR so main program is unaffected.
                      ;
                 ISR: STORE s0, preserve_s0               ;preserve registers
                      STORE s1, preserve_s1
                      STORE s2, preserve_s2
                      STORE s3, preserve_s3
                      STORE s4, preserve_s4
                      STORE s5, preserve_s5
                      STORE s6, preserve_s6
                      STORE s7, preserve_s7
                      STORE s8, preserve_s8
                      STORE s9, preserve_s9
                      STORE sA, preserve_sA
                      STORE sB, preserve_sB
                      STORE sC, preserve_sC
                      STORE sD, preserve_sD
                      STORE sE, preserve_sE
                      STORE sF, preserve_sF
                      ;
                      ;Ignore the first 4 interrupts except to clear the counter.
                      ;This will ensure a clean start up after reset.
                      ;
                      FETCH s0, ISR_count                 ;test to see if more that 4 interrupts have occurred
                      COMPARE s0, 04
                      JUMP Z, normal_isr
                      ADD s0, 01                          ;increment ISR counter until reaching 4
                      STORE s0, ISR_count
                      INPUT s0, status_port               ;Check which counter to clear
                      TEST s0, AB_switch                  ;if bit0 is Low then A is counting
                      JUMP Z, clear_B_count
       clear_A_count: LOAD s0, a_count_reset              ;clear the active counter
                      JUMP clear_counter
       clear_B_count: LOAD s0, b_count_reset              ;clear the active counter
       clear_counter: OUTPUT s0, count_resetport          ;reset counter with pulse
                      LOAD s0, 00                         ;end reset pulse to either counter
                      OUTPUT s0, count_resetport
                      JUMP restore_reg
                      ;
                      ;Normal ISR Routine
                      ;
                      ;
                      ;Read the new counter value and then clear it ready to start again
                      ;
                      ;
          normal_isr: INPUT s0, status_port               ;test for active counter
                      TEST s0, AB_switch                  ;if bit is low then A is counting
                      JUMP Z, capture_B_count
     capture_A_count: LOAD s0, F0                         ;set LEDs to indicate active counter
                      OUTPUT s0, LED_port
                      INPUT sC, A_count0_port             ;read counter A into [sF,sE,SD,sC]
                      INPUT sD, A_count1_port
                      INPUT sE, A_count2_port
                      INPUT sF, A_count3_port
                      LOAD s0, a_count_reset              ;reset counter A
                      OUTPUT s0, count_resetport
                      JUMP counters_read
                      ;
     capture_B_count: LOAD s0, 0F                         ;set LEDs to indicate active counter
                      OUTPUT s0, LED_port
                      INPUT sC, B_count0_port             ;read counter A into [sF,sE,SD,sC]
                      INPUT sD, B_count1_port
                      INPUT sE, B_count2_port
                      INPUT sF, B_count3_port
                      LOAD s0, b_count_reset              ;reset counter B
                      OUTPUT s0, count_resetport
       counters_read: LOAD s0, 00                         ;end reset pulse to either counter
                      OUTPUT s0, count_resetport
                      ;
                      STORE sC, count0                    ;store new counter value
                      STORE sD, count1
                      STORE sE, count2
                      STORE sF, count3
                      ;
                      ;
                      ;
                      ;Restore registers and end ISR
                      ;
         restore_reg: FETCH sF, preserve_sF               ;restore registers
                      FETCH sE, preserve_sE
                      FETCH sD, preserve_sD
                      FETCH sC, preserve_sC
                      FETCH sB, preserve_sB
                      FETCH sA, preserve_sA
                      FETCH s9, preserve_s9
                      FETCH s8, preserve_s8
                      FETCH s7, preserve_s7
                      FETCH s6, preserve_s6
                      FETCH s5, preserve_s5
                      FETCH s4, preserve_s4
                      FETCH s3, preserve_s3
                      FETCH s2, preserve_s2
                      FETCH s1, preserve_s1
                      FETCH s0, preserve_s0
                      RETURNI ENABLE
                      ;
                      ;
                      ;Interrupt vector
                      ;
                      ADDRESS 3FF
                      JUMP ISR
                      ;
                      ;
                      ;**************************************************************************************
                      ;End of Program
                      ;**************************************************************************************
                      ;
                      ;
