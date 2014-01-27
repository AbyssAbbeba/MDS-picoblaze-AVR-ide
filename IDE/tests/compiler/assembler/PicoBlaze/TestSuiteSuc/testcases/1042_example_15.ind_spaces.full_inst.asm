;KCPSM3 Program - Control of LEDs and LCD display using switches, buttons and rotary
;controls on the Spartan-3E Starter Kit.
device kcpsm1
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
switch_port             EQU             0x00                    ;Read switches and press buttons
switch0                 EQU             0x01                    ;  Switches      SW0 - bit0
switch1                 EQU             0x02                    ;                SW1 - bit1
switch2                 EQU             0x04                    ;                SW2 - bit2
switch3                 EQU             0x08                    ;                SW3 - bit3
btn_east                EQU             0x10                    ;  Buttons      East - bit4
btn_south               EQU             0x20                    ;              South - bit5
btn_north               EQU             0x40                    ;              North - bit6
btn_west                EQU             0x80                    ;               West - bit7
;
;
rotary_port             EQU             0x01                    ;Read status of rotary encoder
rotary_left             EQU             0x01                    ; Direction of last move Left=1 Right=0  - bit0
rotary_press            EQU             0x02                    ;     Centre press contact (active High) - bit1
;
;
;LCD interface ports
;
;The master enable signal is not used by the LCD display itself
;but may be required to confirm that LCD communication is active.
;This is required on the Spartan-3E Starter Kit if the StrataFLASH
;is used because it shares the same data pins and conflicts must be avoided.
;
lcd_output_port         EQU             0x40                    ;LCD character module output data and control
lcd_e                   EQU             0x01                    ;   active High Enable        E - bit0
lcd_rw                  EQU             0x02                    ;   Read=1 Write=0           RW - bit1
lcd_rs                  EQU             0x04                    ;   Instruction=0 Data=1     RS - bit2
lcd_drive               EQU             0x08                    ;   Master enable (active High) - bit3
lcd_db4                 EQU             0x10                    ;   4-bit              Data DB4 - bit4
lcd_db5                 EQU             0x20                    ;   interface          Data DB5 - bit5
lcd_db6                 EQU             0x40                    ;                      Data DB6 - bit6
lcd_db7                 EQU             0x80                    ;                      Data DB7 - bit7
;
;
lcd_input_port          EQU             0x02                    ;LCD character module input data
lcd_read_spare0         EQU             0x01                    ;    Spare bits               - bit0
lcd_read_spare1         EQU             0x02                    ;    are zero                 - bit1
lcd_read_spare2         EQU             0x04                    ;                             - bit2
lcd_read_spare3         EQU             0x08                    ;                             - bit3
lcd_read_db4            EQU             0x10                    ;    4-bit           Data DB4 - bit4
lcd_read_db5            EQU             0x20                    ;    interface       Data DB5 - bit5
lcd_read_db6            EQU             0x40                    ;                    Data DB6 - bit6
lcd_read_db7            EQU             0x80                    ;                    Data DB7 - bit7
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
rotary_status           EQU             0x00                    ;Status of rotary encoder
isr_preserve_s0         EQU             0x01                    ;Preserve s0 contents during ISR
;
led_pattern             EQU             0x02                    ;LED pattern used in rotation mode
;
mode                    EQU             0x03                    ;control mode 00=switch and button FF=rotary
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
shift_delay_msb         EQU             0x01                    ;delay is 500ms (01F4 hex)
shift_delay_lsb         EQU             0xf4
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
cold_start:             CALL            lcd_reset               ;initialise LCD display
                        ENABLE          interrupt
;
;Write welcome message to LCD display
;
                        LOAD            s5, #0x10               ;Line 1 position 0
                        CALL            lcd_cursor
                        CALL            disp_spartan            ;Display 'SPARTAN-3E STARTER KIT'
                        LOAD            s5, #0x2e               ;Line 2 position 14
                        CALL            lcd_cursor
                        CALL            disp_www                ;Display 'www.xilinx.com/s3estarter'
;
                        LOAD            s0, #0x08               ;initialise walking '1' pattern
                        STORE           s0, led_pattern
;
                        LOAD            sd, #0xff               ;initial mode is rotary control
                        STORE           sd, mode
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
lcd_shift_delay:        LOAD            sf, #shift_delay_msb    ; [sF,sE]=loop delay in ms
                        LOAD            se, #shift_delay_lsb
lcd_delay_loop:         INPUT           s0, rotary_port         ;test for press of rotary
                        TEST            s0, #rotary_press
                        JUMP            nz, mode_switch
                        FETCH           sd, mode                ;perform control task required
                        COMPARE         sd, #0x00
                        CALL            z, normal_mode
                        COMPARE         sd, #0xff
                        CALL            z, rotate_mode
                        CALL            delay_1ms               ;1ms delay
                        SUB             se, #0x01               ;decrement delay counter
                        SUBCY           sf, #0x00
                        JUMP            nc, lcd_delay_loop
                        CALL            lcd_shift_left          ;shift LCD display
                        JUMP            lcd_shift_delay
;
mode_switch:            XOR             sd, #0xff               ;toggle mode
                        STORE           sd, mode
wait_mode_switch:       CALL            delay_1ms               ;keep shifting LCD display #WHILE waiting
                        SUB             se, #0x01
                        SUBCY           sf, #0x00
                        JUMP            nc, wait_no_shift
                        CALL            lcd_shift_left          ;shift LCD display
                        LOAD            sf, #shift_delay_msb    ; [sF,sE]=loop delay in ms
                        LOAD            se, #shift_delay_lsb
wait_no_shift:          INPUT           s0, rotary_port         ;wait for release of press button
                        TEST            s0, #rotary_press
                        JUMP            nz, wait_mode_switch
                        JUMP            lcd_delay_loop
;
;This procedure will copy the condition of the slide switches and press
;buttons to the LEDs.
;
;
normal_mode:            INPUT           s0, switch_port         ;Read switches and buttons and directly drive LEDs
                        OUTPUT          s0, led_port
                        RETURN
;
;
;
;This procedure will poll the status of the rotary encoder.
;If rotation occurs, then the LED pattern will adjust appropriately.
;
rotate_mode:            FETCH           sa, led_pattern         ;last known position
                        FETCH           s0, rotary_status       ;check status of rotation
                        TEST            s0, #0x80               ;test flag
                        JUMP            z, update_pattern
                        AND             s0, #0x7f               ;clear flag
                        STORE           s0, rotary_status
                        TEST            s0, #rotary_left        ;event occurred so update pattern
                        JUMP            z, move_right
                        RL              sa
                        JUMP            update_pattern
move_right:             RR              sa
update_pattern:         STORE           sa, led_pattern
                        OUTPUT          sa, led_port            ;drive LEDs with current pattern
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
disp_spartan:           LOAD            s5, #_character_s
                        CALL            lcd_write_data
                        LOAD            s5, #_character_p
                        CALL            lcd_write_data
                        LOAD            s5, #_character_a
                        CALL            lcd_write_data
                        LOAD            s5, #_character_r
                        CALL            lcd_write_data
                        LOAD            s5, #_character_t
                        CALL            lcd_write_data
                        LOAD            s5, #_character_a
                        CALL            lcd_write_data
                        LOAD            s5, #_character_n
                        CALL            lcd_write_data
                        LOAD            s5, #character_minus
                        CALL            lcd_write_data
                        LOAD            s5, #character_3
                        CALL            lcd_write_data
                        LOAD            s5, #_character_e
                        CALL            lcd_write_data
                        CALL            disp_space
                        LOAD            s5, #_character_s
                        CALL            lcd_write_data
                        LOAD            s5, #_character_t
                        CALL            lcd_write_data
                        LOAD            s5, #_character_a
                        CALL            lcd_write_data
                        LOAD            s5, #_character_r
                        CALL            lcd_write_data
                        LOAD            s5, #_character_t
                        CALL            lcd_write_data
                        LOAD            s5, #_character_e
                        CALL            lcd_write_data
                        LOAD            s5, #_character_r
                        CALL            lcd_write_data
                        CALL            disp_space
                        LOAD            s5, #_character_k
                        CALL            lcd_write_data
                        LOAD            s5, #_character_i
                        CALL            lcd_write_data
                        LOAD            s5, #_character_t
                        CALL            lcd_write_data
                        RETURN
;
;
;Display 'www.xilinx.com/s3estarter' on LCD at current cursor position
;
;
disp_www:               LOAD            s5, #character_w
                        CALL            lcd_write_data
                        LOAD            s5, #character_w
                        CALL            lcd_write_data
                        LOAD            s5, #character_w
                        CALL            lcd_write_data
                        LOAD            s5, #character_stop
                        CALL            lcd_write_data
                        LOAD            s5, #character_x
                        CALL            lcd_write_data
                        LOAD            s5, #character_i
                        CALL            lcd_write_data
                        LOAD            s5, #character_l
                        CALL            lcd_write_data
                        LOAD            s5, #character_i
                        CALL            lcd_write_data
                        LOAD            s5, #character_n
                        CALL            lcd_write_data
                        LOAD            s5, #character_x
                        CALL            lcd_write_data
                        LOAD            s5, #character_stop
                        CALL            lcd_write_data
                        LOAD            s5, #character_c
                        CALL            lcd_write_data
                        LOAD            s5, #character_o
                        CALL            lcd_write_data
                        LOAD            s5, #character_m
                        CALL            lcd_write_data
                        LOAD            s5, #character_divide
                        CALL            lcd_write_data
                        LOAD            s5, #character_s
                        CALL            lcd_write_data
                        LOAD            s5, #character_3
                        CALL            lcd_write_data
                        LOAD            s5, #character_e
                        CALL            lcd_write_data
                        LOAD            s5, #character_s
                        CALL            lcd_write_data
                        LOAD            s5, #character_t
                        CALL            lcd_write_data
                        LOAD            s5, #character_a
                        CALL            lcd_write_data
                        LOAD            s5, #character_r
                        CALL            lcd_write_data
                        LOAD            s5, #character_t
                        CALL            lcd_write_data
                        LOAD            s5, #character_e
                        CALL            lcd_write_data
                        LOAD            s5, #character_r
                        CALL            lcd_write_data
                        RETURN
;
;Display a space on LCD at current cursor position
;
;
disp_space:             LOAD            s5, #character_space
                        CALL            lcd_write_data
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
delay_1s:               LOAD            s4, #0x32               ;50 x 20ms = 1000ms
wait_1s:                CALL            delay_20ms
                        SUB             s4, #0x01
                        JUMP            nz, wait_1s
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
lcd_pulse_e:            XOR             s4, #lcd_e              ;E=1
                        OUTPUT          s4, lcd_output_port
                        CALL            delay_1us
                        XOR             s4, #lcd_e              ;E=0
                        OUTPUT          s4, lcd_output_port
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
lcd_write_inst4:        AND             s4, #0xf8               ;Enable=1 RS=0 Instruction, RW=0 Write, E=0
                        OUTPUT          s4, lcd_output_port     ;set up RS and RW >40ns before enable pulse
                        CALL            lcd_pulse_e
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
lcd_write_inst8:        LOAD            s4, s5
                        AND             s4, #0xf0               ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                        OR              s4, #lcd_drive          ;Enable=1
                        CALL            lcd_write_inst4         ;write upper nibble
                        CALL            delay_1us               ;wait >1us
                        LOAD            s4, s5                  ;select lower nibble with
                        SL1             s4                      ;Enable=1
                        SL0             s4                      ;RS=0 Instruction
                        SL0             s4                      ;RW=0 Write
                        SL0             s4                      ;E=0
                        CALL            lcd_write_inst4         ;write lower nibble
                        CALL            delay_40us              ;wait >40us
                        LOAD            s4, #0xf0               ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                        OUTPUT          s4, lcd_output_port     ;Release master enable
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
lcd_write_data:         LOAD            s4, s5
                        AND             s4, #0xf0               ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                        OR              s4, #0x0c               ;Enable=1 RS=1 Data, RW=0 Write, E=0
                        OUTPUT          s4, lcd_output_port     ;set up RS and RW >40ns before enable pulse
                        CALL            lcd_pulse_e             ;write upper nibble
                        CALL            delay_1us               ;wait >1us
                        LOAD            s4, s5                  ;select lower nibble with
                        SL1             s4                      ;Enable=1
                        SL1             s4                      ;RS=1 Data
                        SL0             s4                      ;RW=0 Write
                        SL0             s4                      ;E=0
                        OUTPUT          s4, lcd_output_port     ;set up RS and RW >40ns before enable pulse
                        CALL            lcd_pulse_e             ;write lower nibble
                        CALL            delay_40us              ;wait >40us
                        LOAD            s4, #0xf0               ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                        OUTPUT          s4, lcd_output_port     ;Release master enable
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
lcd_read_data8:         LOAD            s4, #0x0e               ;Enable=1 RS=1 Data, RW=1 Read, E=0
                        OUTPUT          s4, lcd_output_port     ;set up RS and RW >40ns before enable pulse
                        XOR             s4, #lcd_e              ;E=1
                        OUTPUT          s4, lcd_output_port
                        CALL            delay_1us               ;wait >260ns to access data
                        INPUT           s5, lcd_input_port      ;read upper nibble
                        XOR             s4, #lcd_e              ;E=0
                        OUTPUT          s4, lcd_output_port
                        CALL            delay_1us               ;wait >1us
                        XOR             s4, #lcd_e              ;E=1
                        OUTPUT          s4, lcd_output_port
                        CALL            delay_1us               ;wait >260ns to access data
                        INPUT           s0, lcd_input_port      ;read lower nibble
                        XOR             s4, #lcd_e              ;E=0
                        OUTPUT          s4, lcd_output_port
                        AND             s5, #0xf0               ;merge upper and lower nibbles
                        SR0             s0
                        SR0             s0
                        SR0             s0
                        SR0             s0
                        OR              s5, s0
                        LOAD            s4, #0x04               ;Enable=0 RS=1 Data, RW=0 Write, E=0
                        OUTPUT          s4, lcd_output_port     ;Stop reading 5V device and release master enable
                        CALL            delay_40us              ;wait >40us
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
lcd_reset:              CALL            delay_20ms              ;wait more that 15ms for display to be ready
                        LOAD            s4, #0x30
                        CALL            lcd_write_inst4         ;send '3'
                        CALL            delay_20ms              ;wait >4.1ms
                        CALL            lcd_write_inst4         ;send '3'
                        CALL            delay_1ms               ;wait >100us
                        CALL            lcd_write_inst4         ;send '3'
                        CALL            delay_40us              ;wait >40us
                        LOAD            s4, #0x20
                        CALL            lcd_write_inst4         ;send '2'
                        CALL            delay_40us              ;wait >40us
                        LOAD            s5, #0x28               ;Function set
                        CALL            lcd_write_inst8
                        LOAD            s5, #0x06               ;Entry mode
                        CALL            lcd_write_inst8
                        LOAD            s5, #0x0c               ;Display control
                        CALL            lcd_write_inst8
lcd_clear:              LOAD            s5, #0x01               ;Display clear
                        CALL            lcd_write_inst8
                        CALL            delay_1ms               ;wait >1.64ms for display to clear
                        CALL            delay_1ms
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
lcd_cursor:             TEST            s5, #0x10               ;test for line 1
                        JUMP            z, set_line2
                        AND             s5, #0x0f               ;make address in range 80 to 8F for line 1
                        OR              s5, #0x80
                        CALL            lcd_write_inst8         ;instruction write to set cursor
                        RETURN
set_line2:              AND             s5, #0x0f               ;make address in range C0 to CF for line 2
                        OR              s5, #0xc0
                        CALL            lcd_write_inst8         ;instruction write to set cursor
                        RETURN
;
;This routine will shift the complete display one position to the left.
;The cursor position and LCD memory contents will not change.
;
;
;Registers used s0, s1, s2, s3, s4, s5
;
lcd_shift_left:         LOAD            s5, #0x18               ;shift display left
                        CALL            lcd_write_inst8
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
isr:                    STORE           s0, isr_preserve_s0     ;preserve s0
                        FETCH           s0, mode                ;test operation mode
                        COMPARE         s0, #0x00               ;ignore events under normal mode
                        JUMP            z, end_isr
                        INPUT           s0, rotary_port         ;read rotary encoder
                        OR              s0, #0x80               ;set flag
                        STORE           s0, rotary_status       ;put result in SCM
end_isr:                FETCH           s0, isr_preserve_s0     ;restore s0
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

