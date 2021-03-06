device kcpsm3

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
rotary_port             EQU             0x01                    ;Read status of rotary encoder
rotary_left             EQU             0x01                    ; Direction of last move Left=1 Right=0  - bit0
rotary_press            EQU             0x02                    ;     Centre press contact (active High) - bit1
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
lcd_input_port          EQU             0x02                    ;LCD character module input data
lcd_read_spare0         EQU             0x01                    ;    Spare bits               - bit0
lcd_read_spare1         EQU             0x02                    ;    are zero                 - bit1
lcd_read_spare2         EQU             0x04                    ;                             - bit2
lcd_read_spare3         EQU             0x08                    ;                             - bit3
lcd_read_db4            EQU             0x10                    ;    4-bit           Data DB4 - bit4
lcd_read_db5            EQU             0x20                    ;    interface       Data DB5 - bit5
lcd_read_db6            EQU             0x40                    ;                    Data DB6 - bit6
lcd_read_db7            EQU             0x80                    ;                    Data DB7 - bit7
bit7                    EQU             0x80                    ;
bit6                    EQU             0x40                    ;
bit5                    EQU             0x20                    ;
bit4                    EQU             0x10                    ;
bit3                    EQU             0x08                    ;
bit2                    EQU             0x04                    ;
bit1                    EQU             0x02                    ;
bit0                    EQU             0x01                    ;
;
;**************************************************************************************
;Scratch Pad Memory Locations
;**************************************************************************************
;
rotary_status           EQU             0x00                    ;Status of rotary encoder
isr_s0                  EQU             0x01                    ;Preserve s0 contents during ISR
led_pattern             EQU             0x02                    ;LED pattern used in rotation mode
;
mode                    EQU             0x03                    ;control mode 00=switch and button FF=rotary
ms1                     EQU             0x04
secl                    EQU             0x05
sech                    EQU             0x06
minl                    EQU             0x07
minh                    EQU             0x08
hourl                   EQU             0x09
hourh                   EQU             0x0a
week                    EQU             0x0b
year1                   EQU             0x0c
year2                   EQU             0x0d
year3                   EQU             0x0e
year4                   EQU             0x0f
ms2                     EQU             0x10
ms500                   EQU             0x11
isr_s1                  EQU             0x12
ms100                   EQU             0x13
rota_cnt                EQU             0x14
isr_s2                  EQU             0x15
temp                    EQU             0x16
pressed                 EQU             0x17
disp_flag               EQU             0x18
blink_flag              EQU             0x19
disp_flag2              EQU             0x1a                    ;;;;;no use
monh                    EQU             0x1b
monl                    EQU             0x1c
dayh                    EQU             0x1d
dayl                    EQU             0x1e
day_up                  EQU             0x1f
year100                 EQU             0x20
hourb                   EQU             0x21
minb                    EQU             0x22
secb                    EQU             0x23
yearb                   EQU             0x24
monb                    EQU             0x25
dayb                    EQU             0x26
dmax                    EQU             0x27                    ;
jan                     EQU             0x28                    ;
feb                     EQU             0x29                    ;
mar                     EQU             0x2a                    ;
apr                     EQU             0x2b                    ;
may                     EQU             0x2c                    ;
jun                     EQU             0x2d                    ;
jul                     EQU             0x2e                    ;
aug                     EQU             0x2f                    ;
sep                     EQU             0x30                    ;
oct                     EQU             0x31                    ;
nov                     EQU             0x32                    ;
dec1                     EQU             0x33                    ;
jan2                    EQU             0x34                    ;
feb2                    EQU             0x35                    ;
mar2                    EQU             0x36                    ;
apr2                    EQU             0x37                    ;
may2                    EQU             0x38                    ;
jun2                    EQU             0x39                    ;
jul2                    EQU             0x3a                    ;
aug2                    EQU             0x3b                    ;
sep2                    EQU             0x3c                    ;
oct2                    EQU             0x3d                    ;
nov2                    EQU             0x3e                    ;
dec2                    EQU             0x3f                    ;
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
; delay_1us_constant =  (clock_rate - 6)/4       Where 'clock_rate' is in MHz
;
;Example: For a 50MHz clock the constant value is (10-6)/4 = 11  (0B Hex).
;For clock rates below 10MHz the value of 1 must be used and the operation will
;become lower than intended.
;
delay_1us_constant      EQU             0x0b
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
reset:                  CALL            lcd_reset
                        ENABLE          interrupt
                        LOAD            sd, #0xff
                        LOAD            s0, #0x00               ;
                        STORE           s0, hourh
                        STORE           s0, minl
                        STORE           s0, minh
                        STORE           s0, sech
                        STORE           s0, secl
                        STORE           s0, year2
                        STORE           s0, year3
                        STORE           s0, monh
                        STORE           s0, minb                ;;;;;;;;;;
                        STORE           s0, secb                ;;;;;;;;;;
                        LOAD            s0, #0x09               ;
                        STORE           s0, hourl
                        STORE           s0, hourb               ;;;;;;;;;
                        LOAD            s0, #0x02               ;
                        STORE           s0, year1
                        STORE           s0, dayh
                        LOAD            s0, #0x06               ;
                        STORE           s0, dayl
                        STORE           s0, year4
                        STORE           s0, yearb               ;;;;;;;;;
                        LOAD            s0, #0x05               ;
                        STORE           s0, monl
                        STORE           s0, week
                        STORE           s0, monb                ;;;;;;;;;
                        LOAD            s0, #0x1a
                        STORE           s0, dayb                ;;;;;;;;;
                        LOAD            s0, #0x1f
                        STORE           s0, jan                 ;1
                        STORE           s0, mar                 ;3
                        STORE           s0, may                 ;5
                        STORE           s0, jul                 ;7
                        STORE           s0, aug                 ;8
                        STORE           s0, oct                 ;10
                        STORE           s0, 0xdec               ;12
                        LOAD            s0, #0x1e
                        STORE           s0, apr                 ;4
                        STORE           s0, jun                 ;6
                        STORE           s0, sep                 ;9
                        STORE           s0, nov                 ;11
                        LOAD            s0, #0x1c
                        STORE           s0, 0xfeb               ;2
                        LOAD            s0, #0x03
                        STORE           s0, jan2
                        STORE           s0, oct2
                        LOAD            s0, #0x06
                        STORE           s0, 0xfeb2
                        STORE           s0, mar2
                        STORE           s0, nov2
                        LOAD            s0, #0x02
                        STORE           s0, apr2
                        STORE           s0, jul2
                        LOAD            s0, #0x04
                        STORE           s0, may2
                        LOAD            s0, #0x00
                        STORE           s0, jun2
                        LOAD            s0, #0x05
                        STORE           s0, aug2
                        LOAD            s0, #0x01
                        STORE           s0, sep2
                        STORE           s0, 0xdec2
                        LOAD            sf, #0x00               ;
restart:
                        LOAD            s5, #0x10
                        CALL            lcd_cursor
                        CALL            disp_msg1
                        LOAD            s5, #0x20
                        CALL            lcd_cursor
                        CALL            disp_msg2
                        LOAD            s3, #0x35
                        STORE           s3, temp
blink10:                LOAD            s3, #0x00
                        STORE           s3, ms100
blink1:                 FETCH           s3, ms100
                        COMPARE         s3, #0x05
                        JUMP            c, blink1
                        LOAD            s5, #0x2f
                        CALL            lcd_cursor
                        FETCH           s5, temp
                        CALL            lcd_write_data
                        LOAD            s3, #0x00
                        STORE           s3, ms100
wait1:                  FETCH           s3, ms100
                        COMPARE         s3, #0x05
                        JUMP            c, wait1
                        LOAD            s5, #0x2f
                        CALL            lcd_cursor
                        LOAD            s5, #0x20
                        CALL            lcd_write_data
                        FETCH           s5, temp
                        ADD             s5, #0xff
                        STORE           s5, temp
                        COMPARE         s5, #0x2f
                        JUMP            nz, blink10
rrestart:               LOAD            s5, #0x10
                        CALL            lcd_cursor
                        CALL            disp_msg3
                        LOAD            s5, #0x20
                        CALL            lcd_cursor
                        CALL            disp_msg4
                        LOAD            s3, #0x35
                        STORE           s3, temp
blink20:                LOAD            s3, #0x00
                        STORE           s3, ms100
blink2:                 FETCH           s3, ms100
                        COMPARE         s3, #0x05
                        JUMP            c, blink2
                        LOAD            s5, #0x2f
                        CALL            lcd_cursor
                        FETCH           s5, temp
                        CALL            lcd_write_data
                        LOAD            s3, #0x00
                        STORE           s3, ms100
wait2:                  FETCH           s3, ms100
                        COMPARE         s3, #0x05
                        JUMP            c, wait2
                        LOAD            s5, #0x2f
                        CALL            lcd_cursor
                        LOAD            s5, #0x20
                        CALL            lcd_write_data
                        FETCH           s5, temp
                        ADD             s5, #0xff
                        STORE           s5, temp
                        COMPARE         s5, #0x2f
                        JUMP            nz, blink20
                        LOAD            s0, #0x00
                        STORE           s0, led_pattern
                        STORE           s0, mode
                        STORE           s0, blink_flag
                        STORE           s0, ms100
                        LOAD            s5, #0x10
                        CALL            lcd_cursor
                        CALL            disp_msg5
                        LOAD            s5, #0x20
                        CALL            lcd_cursor
                        CALL            disp_msg6
pause:                  FETCH           s0, ms100
                        COMPARE         s0, #0x32
                        JUMP            c, pause
                        CALL            lcd_clear
                        INPUT           s0, 0x00
                        STORE           s0, disp_flag2
                        STORE           s0, ms100
loop:
                        FETCH           s0, hourb
                        CALL            b2bcd
                        STORE           s0, hourl
                        STORE           s1, hourh
                        FETCH           s0, minb
                        CALL            b2bcd
                        STORE           s0, minl
                        STORE           s1, minh
                        FETCH           s0, secb
                        CALL            b2bcd
                        STORE           s0, secl
                        STORE           s1, sech
                        FETCH           s0, dayb
                        CALL            b2bcd
                        STORE           s0, dayl
                        STORE           s1, dayh
                        FETCH           s0, monb
                        CALL            b2bcd
                        STORE           s0, monl
                        STORE           s1, monh
                        FETCH           s0, yearb
                        CALL            b2bcd
                        STORE           s0, year4
                        STORE           s1, year3
                        AND             sf, #0xff
                        JUMP            nz, skip1
;;;DISP HH
                        LOAD            s5, #0x11
                        CALL            lcd_cursor
                        FETCH           s3, mode
                        COMPARE         s3, #0x01
                        JUMP            nz, dsp_hh
                        FETCH           s3, ms500
                        TEST            s3, #0x01
                        JUMP            nz, space7
dsp_hh:                 FETCH           s5, hourh
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        FETCH           s5, hourl
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        JUMP            mode_2
space7:                 CALL            disp_space
                        CALL            disp_space
mode_2:
;;;DISP MM
                        LOAD            s5, #0x14
                        CALL            lcd_cursor
                        FETCH           s3, mode
                        COMPARE         s3, #0x02
                        JUMP            nz, dsp_mm
                        FETCH           s3, ms500
                        TEST            s3, #0x01
                        JUMP            nz, space6
dsp_mm:                 FETCH           s5, minh
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        FETCH           s5, minl
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        JUMP            mode_3
space6:                 CALL            disp_space
                        CALL            disp_space
mode_3:
;;;DISP SS
                        LOAD            s5, #0x17
                        CALL            lcd_cursor
                        FETCH           s3, mode
                        COMPARE         s3, #0x03
                        JUMP            nz, dsp_ss
                        FETCH           s3, ms500
                        TEST            s3, #0x01
                        JUMP            nz, space5
dsp_ss:                 FETCH           s5, sech
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        FETCH           s5, secl
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        JUMP            mode_4
space5:                 CALL            disp_space
                        CALL            disp_space
mode_4:
;;;DISP week
                        LOAD            s5, #0x1c
                        CALL            lcd_cursor
                        FETCH           s3, week
                        COMPARE         s3, #0x00
                        JUMP            nz, mon
                        LOAD            s5, #_character_s
                        CALL            lcd_write_data
                        LOAD            s5, #character_u
                        CALL            lcd_write_data
                        LOAD            s5, #character_n
                        CALL            lcd_write_data
                        JUMP            test_bit0
mon:                    FETCH           s3, week
                        COMPARE         s3, #0x01
                        JUMP            nz, tue
                        LOAD            s5, #_character_m
                        CALL            lcd_write_data
                        LOAD            s5, #character_o
                        CALL            lcd_write_data
                        LOAD            s5, #character_n
                        CALL            lcd_write_data
                        JUMP            test_bit0
tue:                    FETCH           s3, week
                        COMPARE         s3, #0x02
                        JUMP            nz, wed
                        LOAD            s5, #_character_t
                        CALL            lcd_write_data
                        LOAD            s5, #character_u
                        CALL            lcd_write_data
                        LOAD            s5, #character_e
                        CALL            lcd_write_data
                        JUMP            test_bit0
wed:                    FETCH           s3, week
                        COMPARE         s3, #0x03
                        JUMP            nz, thu
                        LOAD            s5, #_character_w
                        CALL            lcd_write_data
                        LOAD            s5, #character_e
                        CALL            lcd_write_data
                        LOAD            s5, #character_d
                        CALL            lcd_write_data
                        JUMP            test_bit0
thu:                    FETCH           s3, week
                        COMPARE         s3, #0x04
                        JUMP            nz, fri
                        LOAD            s5, #_character_t
                        CALL            lcd_write_data
                        LOAD            s5, #character_h
                        CALL            lcd_write_data
                        LOAD            s5, #character_u
                        CALL            lcd_write_data
                        JUMP            test_bit0
fri:                    FETCH           s3, week
                        COMPARE         s3, #0x05
                        JUMP            nz, sat
                        LOAD            s5, #_character_f
                        CALL            lcd_write_data
                        LOAD            s5, #character_r
                        CALL            lcd_write_data
                        LOAD            s5, #character_i
                        CALL            lcd_write_data
                        JUMP            test_bit0
sat:                    LOAD            s5, #_character_s
                        CALL            lcd_write_data
                        LOAD            s5, #character_a
                        CALL            lcd_write_data
                        LOAD            s5, #character_t
                        CALL            lcd_write_data
                        JUMP            test_bit0
test_bit0:
rotary_press1:
rotary_press2:
skip1:
;;;DISP month
                        LOAD            s5, #0x26
                        CALL            lcd_cursor
                        FETCH           s3, mode
                        COMPARE         s3, #0x04
                        JUMP            nz, dsp_mon
                        FETCH           s3, ms500
                        TEST            s3, #0x01
                        JUMP            nz, space4
dsp_mon:                FETCH           s5, monh
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        FETCH           s5, monl
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        JUMP            mode_5
space4:                 CALL            disp_space
                        CALL            disp_space
mode_5:
;;;DISP day
                        LOAD            s5, #0x29
                        CALL            lcd_cursor
                        FETCH           s3, mode
                        COMPARE         s3, #0x05
                        JUMP            nz, dsp_dd
                        FETCH           s3, ms500
                        TEST            s3, #0x01
                        JUMP            nz, space3
dsp_dd:                 FETCH           s5, dayh
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        FETCH           s5, dayl
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        JUMP            mode_6
space3:                 CALL            disp_space
                        CALL            disp_space
mode_6:
;;;DISP year
                        LOAD            s5, #0x2c
                        CALL            lcd_cursor
                        FETCH           s3, mode
                        COMPARE         s3, #0x06
                        JUMP            nz, dsp_yy
                        FETCH           s3, ms500
                        TEST            s3, #0x01
                        JUMP            nz, space2
dsp_yy:                 FETCH           s5, year1
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        FETCH           s5, year2
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        FETCH           s5, year3
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        FETCH           s5, year4
                        ADD             s5, #0x30
                        CALL            lcd_write_data
                        JUMP            mode_end
space2:                 CALL            disp_space
                        CALL            disp_space
                        CALL            disp_space
                        CALL            disp_space
mode_end:
                        FETCH           s0, monb
                        LOAD            s1, #dmax
                        ADD             s1, s0
                        FETCH           s2, @s1                 ;;28
                        COMPARE         s0, #0x02               ;;Y
                        JUMP            nz, no_feb
                        FETCH           s1, yearb
                        AND             s1, #0x03
                        JUMP            nz, no_feb
                        ADD             s2, #0x01
no_runnian:
no_feb:                 STORE           s2, dmax
                        FETCH           s1, dayb
                        COMPARE         s2, s1
                        JUMP            nc, under
                        STORE           s2, dayb
under:
                        FETCH           s1, day_up
                        AND             s1, s1
                        JUMP            z, no_day_up
                        FETCH           s1, dayb
                        ADD             s1, #0x01
                        STORE           s1, dayb
                        FETCH           s2, dmax
                        ADD             s2, #0x01
                        COMPARE         s1, s2
                        JUMP            nz, no_mon_up
                        LOAD            s1, #0x01
                        STORE           s1, dayb
                        FETCH           s1, monb
                        ADD             s1, #0x01
                        STORE           s1, monb
                        COMPARE         s1, #0x0d
                        JUMP            nz, no_year_up
                        LOAD            s1, #0x01
                        STORE           s1, monb
                        FETCH           s1, yearb
                        ADD             s1, #0x01
                        STORE           s1, yearb
                        COMPARE         s1, #0x64
                        JUMP            nz, no_year_up
                        LOAD            s1, #0x00
                        STORE           s1, yearb
                        JUMP            no_year_up
no_mon_up:
no_day_up:
no_year_up:
                        LOAD            s0, #0x00
                        STORE           s0, day_up
                        LOAD            s0, #0xdec
                        FETCH           s2, monb
                        ADD             s0, s2
                        FETCH           s1, @s0                 ;;;K
                        FETCH           s0, yearb
                        ADD             s1, s0                  ;;;;;;;K+Y
                        COMPARE         s2, #0x03
                        JUMP            c, rn
                        ADD             s0, #0x01               ;;;;100+Y  >2(3..12)
rn:                     ADD             s0, #0x63               ;;;;;99+Y   1,2
                        SR0             s0
                        SR0             s0                      ;;;/4
                        ADD             s0, s1
                        ADD             s0, #0x06
                        FETCH           s2, dayb
                        ADD             s0, s2
mod7:                   ADD             s0, #0xf9
                        JUMP            nc, m7_end
                        JUMP            mod7
m7_end:                 ADD             s0, #0x07
                        STORE           s0, week
                        FETCH           s0, mode
                        AND             s0, #0xff
                        JUMP            nz, setup
;;;RUN
                        LOAD            s5, #0x20
                        CALL            lcd_cursor
                        FETCH           s5, disp_flag
                        CALL            lcd_write_data
                        CALL            lcd_write_data
                        CALL            lcd_write_data
                        JUMP            modeupdate
setup:
;;;SET
                        LOAD            s5, #0x20
                        CALL            lcd_cursor
                        LOAD            s5, #0x53               ;;;;;;;;;;;;;;;;;;;;;;;    S
                        CALL            lcd_write_data
                        LOAD            s5, #0x45               ;;;;;;;;;;;;;;;;;;;;;;;    E
                        CALL            lcd_write_data
                        LOAD            s5, #0x54               ;;;;;;;;;;;;;;;;;;;;;;;    T
                        CALL            lcd_write_data
modeupdate:             FETCH           s0, mode
                        STORE           s0, blink_flag
nochange:
                        INPUT           s0, 0x00
                        FETCH           s1, disp_flag2
                        XOR             s1, s0
                        JUMP            nz, restart
                        STORE           s0, disp_flag2
                        FETCH           s0, monb
                        COMPARE         s0, #0x05
                        JUMP            nz, chk_sf
                        FETCH           s0, dayb
                        COMPARE         s0, #0x11
                        JUMP            nz, chk_sf
                        FETCH           s0, minb
                        COMPARE         s0, #0x00
                        JUMP            z, msg7
chk_sf:                 COMPARE         sf, #0xff
                        JUMP            nz, no_bd
                        CALL            lcd_reset
                        JUMP            no_bd
msg7:                   LOAD            s5, #0x10
                        CALL            lcd_cursor
                        CALL            disp_msg7               ;
                        FETCH           s0, ms100
                        COMPARE         s0, #0x32
                        JUMP            c, wait
                        LOAD            sf, #0x00
                        JUMP            restart
wait:                   LOAD            sf, #0xff
                        JUMP            loop
no_bd:                  LOAD            sf, #0x00
                        STORE           sf, ms100
                        JUMP            loop
;;;;;;;;;s0---BINARY, s1---BCD_H, s0---BCD_L
b2bcd:                  LOAD            s1, #0x00               ;
bcd_lp:                 ADD             s0, #0xf6
                        JUMP            nc, h_end
                        ADD             s1, #0x01               ;
                        JUMP            bcd_lp
h_end:                  ADD             s0, #0x0a
                        RETURN
;
;     normal_mode: INPUT s0, switch_port               ;Read switches and buttons and directly drive LEDs
;                  OUTPUT s0, LED_port
;                  RETURN
;
;
;
;This procedure will poll the status of the rotary encoder.
;If rotation occurs, then the LED pattern will adjust appropriately.
;
;    rotate_mode: FETCH sA, LED_pattern               ;last known position
;                 FETCH s0, rotary_status             ;check status of rotation
;                 TEST s0, 80                         ;test flag
;                 JUMP Z, update_pattern
;                 AND s0, 7F                          ;clear flag
;                 STORE s0, rotary_status
;                 TEST s0, rotary_left                ;event occurred so update pattern
;                 JUMP Z, move_right
;                 RL sA
;                 JUMP update_pattern
;     move_right: RR sA
; update_pattern: STORE sA, LED_pattern
;                 OUTPUT sA, LED_port                 ;drive LEDs with current pattern
;                 RETURN
;                 ;
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
disp_msg1:
                        CALL            disp_space
                        LOAD            s5, #_character_a
                        CALL            lcd_write_data
                        CALL            disp_space
                        LOAD            s5, #_character_c
                        CALL            lcd_write_data
                        LOAD            s5, #character_l
                        CALL            lcd_write_data
                        LOAD            s5, #character_o
                        CALL            lcd_write_data
                        LOAD            s5, #character_c
                        CALL            lcd_write_data
                        LOAD            s5, #character_k
                        CALL            lcd_write_data
                        CALL            disp_space
                        LOAD            s5, #_character_b
                        CALL            lcd_write_data
                        LOAD            s5, #character_a
                        CALL            lcd_write_data
                        LOAD            s5, #character_s
                        CALL            lcd_write_data
                        LOAD            s5, #character_e
                        CALL            lcd_write_data
                        LOAD            s5, #character_d
                        CALL            lcd_write_data
                        CALL            disp_space
                        RETURN
;
;
;
disp_msg2:              LOAD            s5, #_character_s
                        CALL            lcd_write_data
                        LOAD            s5, #character_3
                        CALL            lcd_write_data
                        LOAD            s5, #_character_e
                        CALL            lcd_write_data
                        CALL            disp_space
                        LOAD            s5, #_character_s
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
                        CALL            disp_space
                        LOAD            s5, #_character_k
                        CALL            lcd_write_data
                        LOAD            s5, #character_i
                        CALL            lcd_write_data
                        LOAD            s5, #character_t
                        CALL            lcd_write_data
                        LOAD            s5, #character_3
                        CALL            lcd_write_data
                        RETURN
disp_msg3:              CALL            disp_space
                        CALL            disp_space
                        CALL            disp_space
                        CALL            disp_space
                        CALL            disp_space
                        LOAD            s5, #_character_x
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
                        CALL            disp_space
                        CALL            disp_space
                        CALL            disp_space
                        CALL            disp_space
                        CALL            disp_space
                        RETURN
disp_msg4:              LOAD            s5, #_character_p
                        CALL            lcd_write_data
                        LOAD            s5, #character_i
                        CALL            lcd_write_data
                        LOAD            s5, #character_c
                        CALL            lcd_write_data
                        LOAD            s5, #character_o
                        CALL            lcd_write_data
                        LOAD            s5, #_character_b
                        CALL            lcd_write_data
                        LOAD            s5, #character_l
                        CALL            lcd_write_data
                        LOAD            s5, #character_a
                        CALL            lcd_write_data
                        LOAD            s5, #character_z
                        CALL            lcd_write_data
                        LOAD            s5, #character_e
                        CALL            lcd_write_data
                        CALL            disp_space
                        LOAD            s5, #_character_t
                        CALL            lcd_write_data
                        LOAD            s5, #character_e
                        CALL            lcd_write_data
                        LOAD            s5, #character_s
                        CALL            lcd_write_data
                        LOAD            s5, #character_t
                        CALL            lcd_write_data
                        CALL            disp_space
                        RETURN
disp_msg5:              LOAD            s5, #_character_p
                        CALL            lcd_write_data
                        LOAD            s5, #character_r
                        CALL            lcd_write_data
                        LOAD            s5, #character_e
                        CALL            lcd_write_data
                        LOAD            s5, #character_s
                        CALL            lcd_write_data
                        LOAD            s5, #character_s
                        CALL            lcd_write_data
                        CALL            disp_space
                        LOAD            s5, #0x26
                        CALL            lcd_write_data
                        CALL            disp_space
                        LOAD            s5, #_character_t
                        CALL            lcd_write_data
                        LOAD            s5, #character_u
                        CALL            lcd_write_data
                        LOAD            s5, #character_r
                        CALL            lcd_write_data
                        LOAD            s5, #character_n
                        CALL            lcd_write_data
                        CALL            disp_space
                        LOAD            s5, #character_t
                        CALL            lcd_write_data
                        LOAD            s5, #character_h
                        CALL            lcd_write_data
                        LOAD            s5, #character_e
                        CALL            lcd_write_data
                        RETURN
disp_msg6:              CALL            disp_space
                        LOAD            s5, #_character_e
                        CALL            lcd_write_data
                        LOAD            s5, #character_n
                        CALL            lcd_write_data
                        LOAD            s5, #character_c
                        CALL            lcd_write_data
                        LOAD            s5, #character_o
                        CALL            lcd_write_data
                        LOAD            s5, #character_d
                        CALL            lcd_write_data
                        LOAD            s5, #character_e
                        CALL            lcd_write_data
                        LOAD            s5, #character_r
                        CALL            lcd_write_data
                        CALL            disp_space
                        LOAD            s5, #character_t
                        CALL            lcd_write_data
                        LOAD            s5, #character_o
                        CALL            lcd_write_data
                        CALL            disp_space
                        LOAD            s5, #_character_s
                        CALL            lcd_write_data
                        LOAD            s5, #character_e
                        CALL            lcd_write_data
                        LOAD            s5, #character_t
                        CALL            lcd_write_data
                        CALL            disp_space
                        RETURN
disp_msg7:              LOAD            s5, #_character_h
                        CALL            lcd_write_data
                        LOAD            s5, #character_a
                        CALL            lcd_write_data
                        LOAD            s5, #character_p
                        CALL            lcd_write_data
                        LOAD            s5, #character_p
                        CALL            lcd_write_data
                        LOAD            s5, #character_y
                        CALL            lcd_write_data
                        CALL            disp_space
                        LOAD            s5, #_character_b
                        CALL            lcd_write_data
                        LOAD            s5, #character_i
                        CALL            lcd_write_data
                        LOAD            s5, #character_r
                        CALL            lcd_write_data
                        LOAD            s5, #character_t
                        CALL            lcd_write_data
                        LOAD            s5, #character_h
                        CALL            lcd_write_data
                        LOAD            s5, #character_d
                        CALL            lcd_write_data
                        LOAD            s5, #character_a
                        CALL            lcd_write_data
                        LOAD            s5, #character_y
                        CALL            lcd_write_data
                        LOAD            s5, #0x21
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
;LCD_read_data8: LOAD s4, 0E                         ;Enable=1 RS=1 Data, RW=1 Read, E=0
;                OUTPUT s4, LCD_output_port          ;set up RS and RW >40ns before enable pulse
;                XOR s4, LCD_E                       ;E=1
;                OUTPUT s4, LCD_output_port
;                CALL delay_1us                      ;wait >260ns to access data
;                INPUT s5, LCD_input_port            ;read upper nibble
;                XOR s4, LCD_E                       ;E=0
;                OUTPUT s4, LCD_output_port
;                CALL delay_1us                      ;wait >1us
;                XOR s4, LCD_E                       ;E=1
;                OUTPUT s4, LCD_output_port
;                CALL delay_1us                      ;wait >260ns to access data
;                INPUT s0, LCD_input_port            ;read lower nibble
;                XOR s4, LCD_E                       ;E=0
;                OUTPUT s4, LCD_output_port
;                AND s5, F0                          ;merge upper and lower nibbles
;                SR0 s0
;                SR0 s0
;                SR0 s0
;                SR0 s0
;                OR s5, s0
;                LOAD s4, 04                         ;Enable=0 RS=1 Data, RW=0 Write, E=0
;                OUTPUT s4, LCD_output_port          ;Stop reading 5V device and release master enable
;                CALL delay_40us                     ;wait >40us
;                RETURN
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
                        LOAD            s5, #0x13
                        CALL            lcd_cursor
                        LOAD            s5, #0x3a               ;;;;;;;;;;;;;;;;;;;;;;;    :
                        CALL            lcd_write_data
                        LOAD            s5, #0x16
                        CALL            lcd_cursor
                        LOAD            s5, #0x2d               ;;;;;;;;;;;;;;;;;;;;;;     -
                        CALL            lcd_write_data
                        LOAD            s5, #0x20
                        CALL            lcd_cursor
                        LOAD            s5, #0x2d               ;;;;;;;;;;;;;;;;;;;;;;     -
                        CALL            lcd_write_data
                        LOAD            s5, #0x2d               ;;;;;;;;;;;;;;;;;;;;;;     -
                        CALL            lcd_write_data
                        LOAD            s5, #0x2d               ;;;;;;;;;;;;;;;;;;;;;;     -
                        CALL            lcd_write_data
                        LOAD            s5, #0x28
                        CALL            lcd_cursor
                        LOAD            s5, #0x2f               ;;;;;;;;;;;;;;;;;;;;;;     /
                        CALL            lcd_write_data
                        LOAD            s5, #0x2b
                        CALL            lcd_cursor
                        LOAD            s5, #0x2f               ;;;;;;;;;;;;;;;;;;;;;;     -
                        CALL            lcd_write_data
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
;LCD_shift_left: LOAD s5, 18                         ;shift display left
;                CALL LCD_write_inst8
;                RETURN
;
;**************************************************************************************
;Interrupt Service Routine (ISR)
;**************************************************************************************
isr:                    STORE           s0, isr_s0              ;preserve s0
                        STORE           s1, isr_s1
                        STORE           s2, isr_s2
                        INPUT           s1, rotary_port
                        LOAD            s0, #0x00
                        TEST            s1, #rotary_press
                        JUMP            z, no_press
                        FETCH           s0, pressed
                        COMPARE         s0, #0x00
                        JUMP            nz, no_press
                        FETCH           s2, led_pattern
                        ADD             s2, #0x01
                        STORE           s2, led_pattern
                        FETCH           s2, mode
                        ADD             s2, #0x01
                        COMPARE         s2, #0x07
                        JUMP            nz, mode_save
                        AND             s2, #0x00
mode_save:              STORE           s2, mode                ;;;;;0-normal;1-hour;2-min;3-sec;4-day,5-mon,6-year
                        LOAD            s0, #0xff
no_press:               STORE           s0, pressed
                        TEST            s1, #0x40               ;;;rotary INT ?
                        JUMP            z, t2_5m_int            ;;NOT rotary INT,JUMP away
clr_int2:               LOAD            s0, #0x40               ;;;;;CLR_INT1
                        OUTPUT          s0, 0x20
                        FETCH           s0, led_pattern
                        TEST            s1, #rotary_left
                        JUMP            z, its_right
                        ADD             s0, #0xff
                        STORE           s0, led_pattern
;;;turn left,--
                        FETCH           s0, mode
                        COMPARE         s0, #0x00
                        JUMP            z, clr_int1             ;
                        COMPARE         s0, #0x01
                        JUMP            nz, mode2n
                        FETCH           s1, hourb
                        ADD             s1, #0xff
                        COMPARE         s1, #0xff               ;;;24
                        JUMP            nz, save_hhn
                        LOAD            s1, #0x17
save_hhn:               STORE           s1, hourb
                        JUMP            real_time_end
mode2n:                 COMPARE         s0, #0x02
                        JUMP            nz, mode3n
                        FETCH           s1, minb
                        ADD             s1, #0xff
                        COMPARE         s1, #0xff               ;;;3C;;60
                        JUMP            nz, save_mmn
                        LOAD            s1, #0x3b
save_mmn:               STORE           s1, minb
                        JUMP            real_time_end
mode3n:                 COMPARE         s0, #0x03
                        JUMP            nz, mode4n
                        FETCH           s1, secb
                        ADD             s1, #0xff
                        COMPARE         s1, #0xff               ;;;60
                        JUMP            nz, save_ssn
                        LOAD            s1, #0x3b
save_ssn:               STORE           s1, secb
                        JUMP            real_time_end
mode4n:                 COMPARE         s0, #0x04
                        JUMP            nz, mode5n
                        FETCH           s1, monb
                        ADD             s1, #0xff
                        COMPARE         s1, #0x00               ;;;12
                        JUMP            nz, save_monn
                        LOAD            s1, #0x0c
save_monn:              STORE           s1, monb
                        JUMP            real_time_end
mode5n:                 COMPARE         s0, #0x05
                        JUMP            nz, mode6n
                        FETCH           s1, dayb
                        ADD             s1, #0xff
                        COMPARE         s1, #0x00               ;;;12
                        JUMP            nz, save_dayn
                        FETCH           s1, dmax
save_dayn:              STORE           s1, dayb
                        JUMP            real_time_end
mode6n:                 COMPARE         s0, #0x06
                        JUMP            nz, mode7n
                        FETCH           s1, yearb
                        ADD             s1, #0xff
                        COMPARE         s1, #0xff               ;;;12
                        JUMP            nz, save_yearn
                        LOAD            s1, #0x63
save_yearn:             STORE           s1, yearb
mode7n:
                        JUMP            real_time_end
its_right:              ADD             s0, #0x01
                        STORE           s0, led_pattern
;;;turn right++
                        FETCH           s0, mode
                        COMPARE         s0, #0x00
                        JUMP            z, clr_int1             ;
                        COMPARE         s0, #0x01
                        JUMP            nz, mode2p
                        FETCH           s1, hourb
                        ADD             s1, #0x01
                        COMPARE         s1, #0x18               ;;;24
                        JUMP            nz, save_hhp
                        LOAD            s1, #0x00
save_hhp:               STORE           s1, hourb
                        JUMP            real_time_end
mode2p:                 COMPARE         s0, #0x02
                        JUMP            nz, mode3p
                        FETCH           s1, minb
                        ADD             s1, #0x01
                        COMPARE         s1, #0x3c               ;;;3C;;60
                        JUMP            nz, save_mmp
                        LOAD            s1, #0x00
save_mmp:               STORE           s1, minb
                        JUMP            real_time_end
mode3p:                 COMPARE         s0, #0x03
                        JUMP            nz, mode4p
                        FETCH           s1, secb
                        ADD             s1, #0x01
                        COMPARE         s1, #0x3c               ;;;60
                        JUMP            nz, save_ssp
                        LOAD            s1, #0x00
save_ssp:               STORE           s1, secb
                        JUMP            real_time_end
mode4p:                 COMPARE         s0, #0x04
                        JUMP            nz, mode5p
                        FETCH           s1, monb
                        ADD             s1, #0x01
                        COMPARE         s1, #0x0d               ;;;12
                        JUMP            nz, save_monp
                        LOAD            s1, #0x01
save_monp:              STORE           s1, monb
                        JUMP            real_time_end
mode5p:                 COMPARE         s0, #0x05
                        JUMP            nz, mode6p
                        FETCH           s1, dayb
                        ADD             s1, #0x01
                        FETCH           s2, dmax
                        ADD             s2, #0x01
                        COMPARE         s1, s2
                        JUMP            nz, save_dayp
                        LOAD            s1, #0x01
save_dayp:              STORE           s1, dayb
                        JUMP            real_time_end
mode6p:                 COMPARE         s0, #0x06
                        JUMP            nz, mode7p
                        FETCH           s1, yearb
                        ADD             s1, #0x01
                        COMPARE         s1, #0x64               ;;;12
                        JUMP            nz, save_yearp
                        LOAD            s1, #0x00
save_yearp:             STORE           s1, yearb
mode7p:                 JUMP            real_time_end
clr_int1:               LOAD            s0, #0x40               ;;;;;CLR_INT1
                        OUTPUT          s0, 0x20
;;;TIMER interrupt process;;;;mode:::0-normal;1-hour;2-min;3-sec;4-day,5-mon,6-year
t2_5m_int:              TEST            s1, #0x80
                        JUMP            z, end_isr
                        LOAD            s0, #0x80               ;;;;;;;;;;clr_int2
                        OUTPUT          s0, 0x20
                        FETCH           s0, ms1
                        ADD             s0, #0x01
                        STORE           s0, ms1
                        COMPARE         s0, #0x64
                        JUMP            nz, real_time_end
                        AND             s0, #0x00               ;;;;;;;;;;;;;CLR ms1
                        STORE           s0, ms1
                        FETCH           s0, led_pattern         ;
                        ADD             s0, #0x10
                        STORE           s0, led_pattern
                        FETCH           s1, ms500
                        ADD             s1, #0x01
                        STORE           s1, ms500
                        COMPARE         s1, #0x01               ;
                        JUMP            nz, cmp_2
                        LOAD            s0, #0x2d               ;;;------
                        JUMP            cmp_4
cmp_2:                  COMPARE         s1, #0x02               ;
                        JUMP            nz, cmp_3
                        LOAD            s0, #0x60               ;;;\\\\\\
                        JUMP            cmp_4
cmp_3:                  COMPARE         s1, #0x03               ;
                        JUMP            nz, cmp_4
                        LOAD            s0, #0x7c               ;;;||||
cmp_4:                  STORE           s0, disp_flag
                        COMPARE         s1, #0x04
                        JUMP            nz, real_time_end
                        LOAD            s0, #0x2f               ;;;////
;;FETCH s0,disp_flag
;;ADD s0,01
                        STORE           s0, disp_flag
                        AND             s1, #0x00               ;;;;;;;;;;;;;CLR ms500
                        STORE           s1, ms500
                        FETCH           s0, secb                ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;CLOCK PROCESS START
                        ADD             s0, #0x01
                        STORE           s0, secb
                        COMPARE         s0, #0x3c               ;;;60
                        JUMP            nz, real_time_end
                        AND             s0, #0x00
                        STORE           s0, secb
                        FETCH           s0, minb
                        ADD             s0, #0x01
                        STORE           s0, minb
                        COMPARE         s0, #0x3c
                        JUMP            nz, real_time_end
                        AND             s0, #0x00
                        STORE           s0, minb
;;;24 Hours
                        FETCH           s0, hourb
                        ADD             s0, #0x01
                        STORE           s0, hourb
                        COMPARE         s0, #0x18
                        JUMP            nz, real_time_end
                        AND             s0, #0x00
                        STORE           s0, hourb
day_p:                  FETCH           s0, day_up
                        LOAD            s0, #0x01
                        STORE           s0, day_up
real_time_end:          FETCH           s0, ms2                 ;;;;;;;;2.5ms counter
                        ADD             s0, #0x01
                        COMPARE         s0, #0x28               ;;;;;;counter to 100ms
                        JUMP            nz, store_ms2
                        FETCH           s1, ms100
                        ADD             s1, #0x01
sav_ms100:              STORE           s1, ms100
                        LOAD            s0, #0x00
store_ms2:              STORE           s0, ms2
                        FETCH           s0, led_pattern
                        OUTPUT          s0, led_port
end_isr:                FETCH           s0, isr_s0              ;restore s0
                        FETCH           s1, isr_s1              ;restore s1
                        FETCH           s2, isr_s2              ;restore s2
                        RETURNI         enable
;**************************************************************************************
;Interrupt Vector
;**************************************************************************************
;
                        ORG             0x3ff
                        JUMP            isr
;First Screen:
;1234567890123456
; Clock Based
;S3E Starter kit
;Second Screen:
;1234567890123456
;   X Xilinx
;PicoBlaze Test
;Third Screen:
;1234567890123456
; Press & Turn
;Encoder to Adjust
;1234567890123456
;09:00:00    Mon
;<->   05/01/2006
;Based on Ken C's
;S3ESK_STARTUP
;Modified By
; George W.
;Dear Mr. Chapman,
;I have just finished a design with PicoBlaze Based the hardware of S3E Starter kit platform.
;It's amazing that the BicoBlaze works perfect,not bad than any other professional MCU I have ever used.I like it very much.
;I have used up all the programme space that MicroBlaze supports and I had to remove some functions from the designs for the programme space reason
;My question is ,wether can I make the programme counter wider in PicoBlaze to support more programme space.
;Another problem,I cannot update the PROM on the starter kit PCB,I'm not sure whether it is caused by the MCS file I generated or other problem.The screen shot and bitstream are attached.Please help me to have a check. Thank you.

