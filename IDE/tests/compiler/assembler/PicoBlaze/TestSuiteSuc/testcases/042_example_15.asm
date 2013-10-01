                  ;KCPSM3 Program - Control of LEDs and LCD display using switches, buttons and rotary
                  ;controls on the Spartan-3E Starter Kit.
                  ;
                  ;
                  ;Ken Chapman - Xilinx Ltd
                  ;
                  ;Version v1.00 - 4th January 2006
                  ;
                  ;**************************************************************************************
                  ;Port definitions
                  ;**************************************************************************************
                  ;
                  ;
                  ;
                  CONSTANT LED_port, 80               ;8 simple LEDs
                  CONSTANT LED0, 01                   ;     LED 0 - bit0
                  CONSTANT LED1, 02                   ;         1 - bit1
                  CONSTANT LED2, 04                   ;         2 - bit2
                  CONSTANT LED3, 08                   ;         3 - bit3
                  CONSTANT LED4, 10                   ;         4 - bit4
                  CONSTANT LED5, 20                   ;         5 - bit5
                  CONSTANT LED6, 40                   ;         6 - bit6
                  CONSTANT LED7, 80                   ;         7 - bit7
                  ;
                  ;
                  CONSTANT switch_port, 00            ;Read switches and press buttons
                  CONSTANT switch0, 01                ;  Switches      SW0 - bit0
                  CONSTANT switch1, 02                ;                SW1 - bit1
                  CONSTANT switch2, 04                ;                SW2 - bit2
                  CONSTANT switch3, 08                ;                SW3 - bit3
                  CONSTANT BTN_east, 10               ;  Buttons      East - bit4
                  CONSTANT BTN_south, 20              ;              South - bit5
                  CONSTANT BTN_north, 40              ;              North - bit6
                  CONSTANT BTN_west, 80               ;               West - bit7
                  ;
                  ;
                  CONSTANT rotary_port, 01            ;Read status of rotary encoder
                  CONSTANT rotary_left, 01            ; Direction of last move Left=1 Right=0  - bit0
                  CONSTANT rotary_press, 02           ;     Centre press contact (active High) - bit1
                  ;
                  ;
                  ;LCD interface ports
                  ;
                  ;The master enable signal is not used by the LCD display itself
                  ;but may be required to confirm that LCD communication is active.
                  ;This is required on the Spartan-3E Starter Kit if the StrataFLASH
                  ;is used because it shares the same data pins and conflicts must be avoided.
                  ;
                  CONSTANT LCD_output_port, 40        ;LCD character module output data and control
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
                  CONSTANT LCD_input_port, 02         ;LCD character module input data
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
                  ;**************************************************************************************
                  ;Special Register usage
                  ;**************************************************************************************
                  ;
                  ;**************************************************************************************
                  ;Scratch Pad Memory Locations
                  ;**************************************************************************************
                  ;
                  CONSTANT rotary_status, 00          ;Status of rotary encoder
                  CONSTANT ISR_preserve_s0, 01        ;Preserve s0 contents during ISR
                  ;
                  CONSTANT LED_pattern, 02            ;LED pattern used in rotation mode
                  ;
                  CONSTANT mode, 03                   ;control mode 00=switch and button FF=rotary
                  ;
                  ;
                  ;
                  ;**************************************************************************************
                  ;Useful data constants
                  ;**************************************************************************************
                  ;
                  ;
                  ;The main operation of the program uses 1ms delays to set the shift rate
                  ;of the LCD display. A 16-bit value determines how many milliseconds
                  ;there are between shifts
                  ;
                  ;Tests indicate that the fastest shift rate that the LCD display supports is
                  ;500ms. Faster than this and the display becomes less clear to read.
                  ;
                  CONSTANT shift_delay_msb, 01        ;delay is 500ms (01F4 hex)
                  CONSTANT shift_delay_lsb, F4
                  ;
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
                  ENABLE INTERRUPT
                  ;
                  ;Write welcome message to LCD display
                  ;
                  LOAD s5, 10                         ;Line 1 position 0
                  CALL LCD_cursor
                  CALL disp_SPARTAN                   ;Display 'SPARTAN-3E STARTER KIT'
                  LOAD s5, 2E                         ;Line 2 position 14
                  CALL LCD_cursor
                  CALL disp_www                       ;Display 'www.xilinx.com/s3estarter'
                  ;
                  LOAD s0, 08                         ;initialise walking '1' pattern
                  STORE s0, LED_pattern
                  ;
                  LOAD sD, FF                         ;initial mode is rotary control
                  STORE sD, mode
                  ;
                  ;**************************************************************************************
                  ;Main program
                  ;**************************************************************************************
                  ;
                  ;The main program is responsible for continuously shifting the LCD display
                  ;at 0.5 second intervals. It also polls the centre press button of the rotary
                  ;encoder to determine which mode of LED control should be active.
                  ;
                  ;
 LCD_shift_delay: LOAD sF, shift_delay_msb            ; [sF,sE]=loop delay in ms
                  LOAD sE, shift_delay_lsb
  LCD_delay_loop: INPUT s0, rotary_port               ;test for press of rotary
                  TEST s0, rotary_press
                  JUMP NZ, mode_switch
                  FETCH sD, mode                      ;perform control task required
                  COMPARE sD, 00
                  CALL Z, normal_mode
                  COMPARE sD, FF
                  CALL Z, rotate_mode
                  CALL delay_1ms                      ;1ms delay
                  SUB sE, 01                          ;decrement delay counter
                  SUBCY sF, 00
                  JUMP NC, LCD_delay_loop
                  CALL LCD_shift_left                 ;shift LCD display
                  JUMP LCD_shift_delay
                  ;
     mode_switch: XOR sD, FF                          ;toggle mode
                  STORE sD, mode
wait_mode_switch: CALL delay_1ms                      ;keep shifting LCD display while waiting
                  SUB sE, 01
                  SUBCY sF, 00
                  JUMP NC, wait_no_shift
                  CALL LCD_shift_left                 ;shift LCD display
                  LOAD sF, shift_delay_msb            ; [sF,sE]=loop delay in ms
                  LOAD sE, shift_delay_lsb
   wait_no_shift: INPUT s0, rotary_port               ;wait for release of press button
                  TEST s0, rotary_press
                  JUMP NZ, wait_mode_switch
                  JUMP LCD_delay_loop
                  ;
                  ;This procedure will copy the condition of the slide switches and press
                  ;buttons to the LEDs.
                  ;
                  ;
     normal_mode: INPUT s0, switch_port               ;Read switches and buttons and directly drive LEDs
                  OUTPUT s0, LED_port
                  RETURN
                  ;
                  ;
                  ;
                  ;This procedure will poll the status of the rotary encoder.
                  ;If rotation occurs, then the LED pattern will adjust appropriately.
                  ;
     rotate_mode: FETCH sA, LED_pattern               ;last known position
                  FETCH s0, rotary_status             ;check status of rotation
                  TEST s0, 80                         ;test flag
                  JUMP Z, update_pattern
                  AND s0, 7F                          ;clear flag
                  STORE s0, rotary_status
                  TEST s0, rotary_left                ;event occurred so update pattern
                  JUMP Z, move_right
                  RL sA
                  JUMP update_pattern
      move_right: RR sA
  update_pattern: STORE sA, LED_pattern
                  OUTPUT sA, LED_port                 ;drive LEDs with current pattern
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
                  ;Display 'SPARTAN-3E STARTER KIT' on LCD at current cursor position
                  ;
                  ;
    disp_SPARTAN: LOAD s5, character_S
                  CALL LCD_write_data
                  LOAD s5, character_P
                  CALL LCD_write_data
                  LOAD s5, character_A
                  CALL LCD_write_data
                  LOAD s5, character_R
                  CALL LCD_write_data
                  LOAD s5, character_T
                  CALL LCD_write_data
                  LOAD s5, character_A
                  CALL LCD_write_data
                  LOAD s5, character_N
                  CALL LCD_write_data
                  LOAD s5, character_minus
                  CALL LCD_write_data
                  LOAD s5, character_3
                  CALL LCD_write_data
                  LOAD s5, character_E
                  CALL LCD_write_data
                  CALL disp_space
                  LOAD s5, character_S
                  CALL LCD_write_data
                  LOAD s5, character_T
                  CALL LCD_write_data
                  LOAD s5, character_A
                  CALL LCD_write_data
                  LOAD s5, character_R
                  CALL LCD_write_data
                  LOAD s5, character_T
                  CALL LCD_write_data
                  LOAD s5, character_E
                  CALL LCD_write_data
                  LOAD s5, character_R
                  CALL LCD_write_data
                  CALL disp_space
                  LOAD s5, character_K
                  CALL LCD_write_data
                  LOAD s5, character_I
                  CALL LCD_write_data
                  LOAD s5, character_T
                  CALL LCD_write_data
                  RETURN
                  ;
                  ;
                  ;Display 'www.xilinx.com/s3estarter' on LCD at current cursor position
                  ;
                  ;
        disp_www: LOAD s5, character_w
                  CALL LCD_write_data
                  LOAD s5, character_w
                  CALL LCD_write_data
                  LOAD s5, character_w
                  CALL LCD_write_data
                  LOAD s5, character_stop
                  CALL LCD_write_data
                  LOAD s5, character_x
                  CALL LCD_write_data
                  LOAD s5, character_i
                  CALL LCD_write_data
                  LOAD s5, character_l
                  CALL LCD_write_data
                  LOAD s5, character_i
                  CALL LCD_write_data
                  LOAD s5, character_n
                  CALL LCD_write_data
                  LOAD s5, character_x
                  CALL LCD_write_data
                  LOAD s5, character_stop
                  CALL LCD_write_data
                  LOAD s5, character_c
                  CALL LCD_write_data
                  LOAD s5, character_o
                  CALL LCD_write_data
                  LOAD s5, character_m
                  CALL LCD_write_data
                  LOAD s5, character_divide
                  CALL LCD_write_data
                  LOAD s5, character_s
                  CALL LCD_write_data
                  LOAD s5, character_3
                  CALL LCD_write_data
                  LOAD s5, character_e
                  CALL LCD_write_data
                  LOAD s5, character_s
                  CALL LCD_write_data
                  LOAD s5, character_t
                  CALL LCD_write_data
                  LOAD s5, character_a
                  CALL LCD_write_data
                  LOAD s5, character_r
                  CALL LCD_write_data
                  LOAD s5, character_t
                  CALL LCD_write_data
                  LOAD s5, character_e
                  CALL LCD_write_data
                  LOAD s5, character_r
                  CALL LCD_write_data
                  RETURN
                  ;
                  ;Display a space on LCD at current cursor position
                  ;
                  ;
      disp_space: LOAD s5, character_space
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
                  ;This routine will shift the complete display one position to the left.
                  ;The cursor position and LCD memory contents will not change.
                  ;
                  ;
                  ;Registers used s0, s1, s2, s3, s4, s5
                  ;
  LCD_shift_left: LOAD s5, 18                         ;shift display left
                  CALL LCD_write_inst8
                  RETURN
                  ;
                  ;**************************************************************************************
                  ;Interrupt Service Routine (ISR)
                  ;**************************************************************************************
                  ;
                  ;Interrupts occur when the rotary control has been moved.
                  ;
                  ;The ISR captures the state of the direction which it writes to scratch pad memory (SPM).
                  ;The most significant bit is also set at this location to provide a 'flag' to the
                  ;main body of the program.
                  ;
                  ;If the main program is not in rotation mode then the ISR effectively ignores the
                  ;event although the hardware interface will be cleared by the interrupt_ack signal.
                  ;This prevents a rotation event which has been made during normal switch and button
                  ;mode from having any effect.
                  ;
             ISR: STORE s0, ISR_preserve_s0           ;preserve s0
                  FETCH s0, mode                      ;test operation mode
                  COMPARE s0, 00                      ;ignore events under normal mode
                  JUMP Z, end_ISR
                  INPUT s0, rotary_port               ;read rotary encoder
                  OR s0, 80                           ;set flag
                  STORE s0, rotary_status             ;put result in SCM
         end_ISR: FETCH s0, ISR_preserve_s0           ;restore s0
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
