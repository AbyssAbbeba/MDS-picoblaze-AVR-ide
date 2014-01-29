;KCPSM3 Program - FC_CTRL
;Frequency Generator Control
;Calculates and displays results on the 16x2 LCD display.
device kcpsm1
;
; Version : 1.12
; Date : 28th February 2006
;
; Ken Chapman
; Xilinx Ltd
;
; chapmanxilinx.com
;
;
;
;**************************************************************************************
;Port definitions
;**************************************************************************************
;
a_count0_port           EQU             0x00                    ;32-bit A-counter (LSByte first)
a_count1_port           EQU             0x10
a_count2_port           EQU             0x20
a_count3_port           EQU             0x30
;
b_count0_port           EQU             0x40                    ;32-bit B-counter (LSByte first)
b_count1_port           EQU             0x50
b_count2_port           EQU             0x60
b_count3_port           EQU             0x70
;
status_port             EQU             0x80                    ;4 switches and counter status
switch0                 EQU             0x01                    ;  Switches      SW0 - bit0
switch1                 EQU             0x02                    ; active High    SW1 - bit1
switch2                 EQU             0x04                    ;                SW2 - bit2
switch3                 EQU             0x08                    ;                SW3 - bit3
ab_switch               EQU             0x10                    ;  0=A-count enabled 1=B-count enabled
;
;
count_resetport         EQU             0x02                    ;Reset frequency counter controls
a_count_reset           EQU             0x01                    ;  A-count = bit0
b_count_reset           EQU             0x02                    ;  B-count = bit1
;
led_port                EQU             0x01                    ;8 simple LEDs - active high
;
;
source_control_port     EQU             0x08                    ;Select and control load sources
source_sel0             EQU             0x01                    ;  00 = SMA clock  01=50MHz
source_sel1             EQU             0x02                    ;  10 = DCM Osc    11=Ring Osc
ring_reset              EQU             0x40                    ; active High rest of ring osc - bit6
dcm_kick                EQU             0x80                    ; DCM kick start signal - bit7
;
;
;LCD interface ports
;
;The master enable signal is not used by the LCD display itself
;but may be required to confirm that LCD communication is active.
;This is required on the Spartan-3E Starter Kit if the StrataFLASH
;is used because it shares the same EQU pins and conflicts must be avoided.
;
lcd_output_port         EQU             0x04                    ;LCD character module output EQU and control
lcd_e                   EQU             0x01                    ;   active High Enable        E - bit0
lcd_rw                  EQU             0x02                    ;   Read=1 Write=0           RW - bit1
lcd_rs                  EQU             0x04                    ;   Instruction=0 EQU=1     RS - bit2
lcd_drive               EQU             0x08                    ;   Master enable (active High) - bit3
lcd_db4                 EQU             0x10                    ;   4-bit              EQU DB4 - bit4
lcd_db5                 EQU             0x20                    ;   interface          EQU DB5 - bit5
lcd_db6                 EQU             0x40                    ;                      EQU DB6 - bit6
lcd_db7                 EQU             0x80                    ;                      EQU DB7 - bit7
;
;
lcd_input_port          EQU             0x09                    ;LCD character module input EQU
lcd_read_spare0         EQU             0x01                    ;    Spare bits               - bit0
lcd_read_spare1         EQU             0x02                    ;    are zero                 - bit1
lcd_read_spare2         EQU             0x04                    ;                             - bit2
lcd_read_spare3         EQU             0x08                    ;                             - bit3
lcd_read_db4            EQU             0x10                    ;    4-bit           EQU DB4 - bit4
lcd_read_db5            EQU             0x20                    ;    interface       EQU DB5 - bit5
lcd_read_db6            EQU             0x40                    ;                    EQU DB6 - bit6
lcd_read_db7            EQU             0x80                    ;                    EQU DB7 - bit7
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
count0                  EQU             0x00                    ;last 32-bit counter value (LSByte first)
count1                  EQU             0x01
count2                  EQU             0x02
count3                  EQU             0x03
;
isr_count               EQU             0x04                    ;count number of interrupts for a clean start
;
decimal0                EQU             0x11                    ;10 digit decimal value up to 4,294,967,295
decimal1                EQU             0x12
decimal2                EQU             0x13
decimal3                EQU             0x14
decimal4                EQU             0x15
decimal5                EQU             0x16
decimal6                EQU             0x17
decimal7                EQU             0x18
decimal8                EQU             0x19
decimal9                EQU             0x1a
;
;
;
preserve_s0             EQU             0x30                    ;place to save register contents
preserve_s1             EQU             0x31
preserve_s2             EQU             0x32
preserve_s3             EQU             0x33
preserve_s4             EQU             0x34
preserve_s5             EQU             0x35
preserve_s6             EQU             0x36
preserve_s7             EQU             0x37
preserve_s8             EQU             0x38
preserve_s9             EQU             0x39
preserve_sa             EQU             0x3a
preserve_sb             EQU             0x3b
preserve_sc             EQU             0x3c
preserve_sd             EQU             0x3d
preserve_se             EQU             0x3e
preserve_sf             EQU             0x3f
;
;
;
;**************************************************************************************
;Useful EQU constants
;**************************************************************************************
;
;
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
                        LOAD            s0, #0x00               ;Turn off LEDs
                        OUTPUT          s0, led_port
;
;
;Write welcome message to LCD display
;
                        LOAD            s5, #0x10               ;Line 1 position 0
                        CALL            lcd_cursor
                        CALL            disp_picoblaze          ;Display 'PicoBlaze Inside'
                        CALL            delay_1s                ;wait 3 seconds
                        CALL            delay_1s
                        CALL            delay_1s
                        LOAD            s5, #0x10               ;Line 1 position 0
                        CALL            lcd_cursor
                        CALL            disp_frequency          ;Display 'Frequency Counter V1.00'
                        LOAD            s5, #0x21               ;Line 2 position 1
                        CALL            lcd_cursor
                        CALL            disp_counter
                        LOAD            s5, #0x2b               ;Line 2 position 11
                        CALL            lcd_cursor
                        CALL            disp_version
                        CALL            delay_1s                ;wait 5 seconds
                        CALL            delay_1s
                        CALL            delay_1s
                        CALL            delay_1s
                        CALL            delay_1s
                        CALL            lcd_clear               ;Clear display
;
;Kick start the DCM oscillator.
; Just requires a few cyles of activity
;
                        LOAD            s0, #0xff
                        LOAD            s1, #0x00
kick_loop:              OUTPUT          s1, source_control_port
                        XOR             s1, #dcm_kick           ;toggle kick start signal
                        SUB             s0, #0x01
                        JUMP            nz, kick_loop
;
;clear all scratch pad memory locations
;
                        LOAD            s1, #0x3f
                        LOAD            s0, #0x00
clear_spm:              load           s0, #s1
                        SUB             s1, #0x01
                        JUMP            nc, clear_spm
;
                        ENABLE          interrupt
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
warm_start:             LOAD            s5, #0x21               ;Line 2 position 1
                        CALL            lcd_cursor
                        INPUT           sf, status_port         ;select source based on switches
                        load         sf, #0x00               ;load for no switches active
                        AND             sf, #0x0f               ;isolate switches
                        JUMP            nz, load_sma
                        CALL            disp_menu
                        JUMP            warm_start
load_sma:               load         sf, #switch0
                        JUMP            nz, load_50m
                        CALL            disp_sma_input
                        LOAD            sf, #0x00
                        JUMP            select_source
load_50m:               load         sf, #switch1
                        JUMP            nz, load_dcm
                        CALL            disp_50mhz_crystal
                        LOAD            sf, #0x01
                        JUMP            select_source
load_dcm:               load         sf, #switch2
                        JUMP            nz, load_ring
                        CALL            disp_dcm_oscillator
                        LOAD            sf, #0x02
                        JUMP            select_source
load_ring:              load         sf, #switch3
                        JUMP            z, ring_select
                        CALL            disp_menu               ;more than one switch is set
                        JUMP            warm_start
ring_select:            CALL            disp_ring_oscillator
                        LOAD            sf, #0x03
select_source:          OUTPUT          sf, source_control_port ;select source control
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
                        DISABLE         interrupt               ;copy cycle count to register set [s5,s4,s3,s2]
                        load           s2, count0
                        load           s3, count1
                        load           s4, count2
                        load           s5, count3
                        ENABLE          interrupt
                        CALL            integer_to_bcd          ;convert last 32-bit value to BCD digits
                        LOAD            s5, #0x10               ;Line 1 position 0
                        CALL            lcd_cursor
                        LOAD            s6, #decimal8           ;up to 999,999,999 Hz
                        CALL            disp_digits
                        JUMP            warm_start
;
;
;
;**************************************************************************************
; Display frequency value on LCD display
;**************************************************************************************
;
;
;Display value on the LCD display at current position.
;The values to be displayed must be loadd in scratch pad memory
;locations 'decimal0' to 'decimal9' which must be in ascending locations.
;
;The routing performs leading zero suppression and scales to Hz, KHz or MHz ranges.
;
;Registers used s0,s1,s4,s5,sE,sF
;
disp_digits:            LOAD            sf, #0xff               ;set blanking flag
                        LOAD            se, #character_space    ;scaling character for MHz, KHz or Hz
                        load           s5, decimal8            ;100MHz digit
                        CALL            zero_load
                        CALL            disp_digit
                        load           s5, decimal7            ;10MHz digit
                        CALL            zero_load
                        CALL            disp_digit
                        load           s5, decimal6            ;1MHz digit
                        CALL            zero_load
                        CALL            disp_digit
                        load         sf, #0xff               ;check if any MHz were active
                        JUMP            z, khz_space
                        LOAD            s5, #character_stop
                        CALL            lcd_write_EQU
                        LOAD            se, #_character_m
                        JUMP            khz_digits
khz_space:              LOAD            s5, #character_space
                        CALL            lcd_write_EQU
khz_digits:             load           s5, decimal5            ;100KHz digit
                        CALL            zero_load
                        CALL            disp_digit
                        load           s5, decimal4            ;10KHz digit
                        CALL            zero_load
                        CALL            disp_digit
                        load           s5, decimal3            ;1KHz digit
                        CALL            zero_load
                        CALL            disp_digit
                        load         se, #_character_m       ;check if any MHz were active
                        JUMP            z, hz_space
                        load         sf, #0xff               ;check if any KHz were active
                        JUMP            z, hz_space
                        LOAD            s5, #character_stop
                        CALL            lcd_write_EQU
                        LOAD            se, #_character_k
                        JUMP            hz_digits
hz_space:               LOAD            s5, #character_space
                        CALL            lcd_write_EQU
hz_digits:              load           s5, decimal2            ;100KHz digit
                        CALL            zero_load
                        CALL            disp_digit
                        load           s5, decimal1            ;10KHz digit
                        CALL            zero_load
                        CALL            disp_digit
                        load           s5, decimal0            ;1KHz digit (always displayed)
                        ADD             s5, #character_0        ;convert number to ASCII
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        LOAD            s5, se
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_h
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_z
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_space    ;ensure end of line is clear
                        CALL            lcd_write_EQU
                        RETURN
;
;Check digit for zero. If not zero then clear
;blanking flag (sF=00)
zero_load:              load         s5, #0x00
                        RETURN          z
                        LOAD            sf, #0x00
                        RETURN
;
;Display single digit at current position
;or space if blanking (sF=FF) is active
;
disp_digit:             load         sf, #0xff
                        JUMP            z, blank_digit
                        ADD             s5, #character_0        ;convert number to ASCII
                        CALL            lcd_write_EQU
                        RETURN
blank_digit:            LOAD            s5, #character_space
                        CALL            lcd_write_EQU
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
integer_to_bcd:         LOAD            se, #0x0a               ;10 digits to be formed from value upto 4294967295
                        LOAD            sf, #decimal0           ;pointer for LS-Digit
int_to_bcd_loop:        CALL            divide_32bit_by_10
                        load           s1, #sf                 ;remainder becomes digit value
                        ADD             sf, #0x01               ;move to next most significant digit
                        SUB             se, #0x01               ;one less digit to compute
                        JUMP            nz, int_to_bcd_loop
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
divide_32bit_by_10:     LOAD            sa, s2                  ;copy input value to set [sD,sC,sB,sA]
                        LOAD            sb, s3
                        LOAD            sc, s4
                        LOAD            sd, s5
                        LOAD            s2, #0x00               ;clear result
                        LOAD            s3, #0x00
                        LOAD            s4, #0x00
                        LOAD            s5, #0x00
                        LOAD            s9, #0xa0               ;initialise '10' value into msb's of set [s9,s8,s7,s6]
                        LOAD            s8, #0x00
                        LOAD            s7, #0x00
                        LOAD            s6, #0x00
                        LOAD            s0, #0x1d               ;29 subtract and shiload iterations to be performed
div10_loop:             SUB             sa, s6                  ;perform 32-bit subtract [sD,sC,sB,sA]-[s9,s8,s7,s6]
                        SUBCY           sb, s7
                        SUBCY           sc, s8
                        SUBCY           sd, s9
                        JUMP            c, div10_reload
                        SL1             s2                      ;shiload '1' into result
                        JUMP            div10_shiloads
div10_reload:          ADD             sa, s6                  ;perform 32-bit addition [sD,sC,sB,sA]+[s9,s8,s7,s6]
                        ADDCY           sb, s7
                        ADDCY           sc, s8
                        ADDCY           sd, s9
                        SL0             s2                      ;shiload '0' into result
div10_shiloads:           SLA             s3                      ;complete 32-bit shiload leload
                        SLA             s4
                        SLA             s5
                        SR0             s9                      ;divide '10' value by 2 (shiload right 1 place)
                        SRA             s8
                        SRA             s7
                        SRA             s6
                        SUB             s0, #0x01               ;count iterations
                        JUMP            nz, div10_loop
                        LOAD            s1, sa                  ;remainder of division
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
disp_picoblaze:         LOAD            s5, #_character_p
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_i
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_c
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_o
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_b
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_l
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_a
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_z
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_e
                        CALL            lcd_write_EQU
                        RETURN
;
;
;Display 'Frequency' on LCD at current cursor position
;
;
disp_frequency:         LOAD            s5, #_character_f
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_r
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_e
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_q
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_u
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_e
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_n
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_c
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_y
                        CALL            lcd_write_EQU
                        RETURN
;
;
;Display 'Counter' on LCD at current cursor position
;
;
disp_counter:           LOAD            s5, #_character_c
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_o
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_u
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_n
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_t
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_e
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_r
                        CALL            lcd_write_EQU
                        RETURN
;
;Display version number on LCD at current cursor position
;
;
disp_version:           LOAD            s5, #character_v
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_1
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_stop
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_0
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_0
                        CALL            lcd_write_EQU
                        RETURN
;
;
;Display 'SMA input' at current cursor position
;
;
disp_sma_input:         LOAD            s5, #_character_s
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_m
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_a
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_i
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_n
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_p
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_u
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_t
                        CALL            lcd_write_EQU
                        LOAD            sf, #0x06
                        JUMP            disp_spaces
;
;
;
;Display '50MHz Crystal' at current cursor position
;
;
disp_50mhz_crystal:     LOAD            s5, #character_5
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_0
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_m
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_h
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_z
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_c
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_r
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_y
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_s
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_t
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_a
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_l
                        CALL            lcd_write_EQU
                        LOAD            sf, #0x02
                        JUMP            disp_spaces
;
;
;
;Display 'DCM oscillator' at current cursor position
;
;
disp_dcm_oscillator:    LOAD            s5, #_character_d
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_c
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_m
                        CALL            lcd_write_EQU
disp_oscillator:        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_o
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_s
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_c
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_i
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_l
                        CALL            lcd_write_EQU
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_a
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_t
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_o
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_r
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        RETURN
;
;
;
;Display 'Ring oscillator' at current cursor position
;
;
disp_ring_oscillator:   LOAD            s5, #_character_r
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_i
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_n
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_g
                        CALL            lcd_write_EQU
                        JUMP            disp_oscillator
;
;
;Display spaces at current cursor position
;Number of spaces to be specified in register sF
;
disp_spaces:            load         sf, #0x00
                        RETURN          z
                        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        SUB             sf, #0x01
                        JUMP            disp_spaces
;
;Display switch setting menu on entire display.
;
disp_menu:              LOAD            s5, #0x10               ;Line 1 position 0
                        CALL            lcd_cursor
                        LOAD            s5, #_character_r
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_i
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_n
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_g
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_d
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_c
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_m
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_5
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_0
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_m
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_s
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_m
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_a
                        CALL            lcd_write_EQU
                        LOAD            s5, #0x20               ;Line 2 position 0
                        CALL            lcd_cursor
                        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_s
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_w
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_3
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_s
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_w
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_2
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_s
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_w
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_1
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_space
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_s
                        CALL            lcd_write_EQU
                        LOAD            s5, #_character_w
                        CALL            lcd_write_EQU
                        LOAD            s5, #character_0
                        CALL            lcd_write_EQU
                        RETURN
;
;
;
;
;**************************************************************************************
;Soloadware delay routines
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
;The 4-wire EQU interface will be used (DB4 to DB7).
;
;The LCD modules are relatively slow and soloadware delay loops are used to slow down
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
;Write 8-bit EQU to LCD display.
;
;The 8-bit EQU should be provided in register s5.
;EQU bytes are written using the following sequence
; Upper nibble
; wait >1us
; Lower nibble
; wait >40us
;
;Registers used s0, s1, s4, s5
;
lcd_write_EQU:         LOAD            s4, s5
                        AND             s4, #0xf0               ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                        OR              s4, #0x0c               ;Enable=1 RS=1 EQU, RW=0 Write, E=0
                        OUTPUT          s4, lcd_output_port     ;set up RS and RW >40ns before enable pulse
                        CALL            lcd_pulse_e             ;write upper nibble
                        CALL            delay_1us               ;wait >1us
                        LOAD            s4, s5                  ;select lower nibble with
                        SL1             s4                      ;Enable=1
                        SL1             s4                      ;RS=1 EQU
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
;Read 8-bit EQU from LCD display.
;
;The 8-bit EQU will be read from the current LCD memory address
;and will be returned in register s5.
;It is advisable to set the LCD address (cursor position) before
;using the EQU read for the first time otherwise the display may
;generate invalid EQU on the first read.
;
;EQU bytes are read using the following sequence
; Upper nibble
; wait >1us
; Lower nibble
; wait >40us
;
;Registers used s0, s1, s4, s5
;
lcd_read_EQU8:         LOAD            s4, #0x0e               ;Enable=1 RS=1 EQU, RW=1 Read, E=0
                        OUTPUT          s4, lcd_output_port     ;set up RS and RW >40ns before enable pulse
                        XOR             s4, #lcd_e              ;E=1
                        OUTPUT          s4, lcd_output_port
                        CALL            delay_1us               ;wait >260ns to access EQU
                        INPUT           s5, lcd_input_port      ;read upper nibble
                        XOR             s4, #lcd_e              ;E=0
                        OUTPUT          s4, lcd_output_port
                        CALL            delay_1us               ;wait >1us
                        XOR             s4, #lcd_e              ;E=1
                        OUTPUT          s4, lcd_output_port
                        CALL            delay_1us               ;wait >260ns to access EQU
                        INPUT           s0, lcd_input_port      ;read lower nibble
                        XOR             s4, #lcd_e              ;E=0
                        OUTPUT          s4, lcd_output_port
                        AND             s5, #0xf0               ;merge upper and lower nibbles
                        SR0             s0
                        SR0             s0
                        SR0             s0
                        SR0             s0
                        OR              s5, s0
                        LOAD            s4, #0x04               ;Enable=0 RS=1 EQU, RW=0 Write, E=0
                        OUTPUT          s4, lcd_output_port     ;Stop reading 5V device and release master enable
                        CALL            delay_40us              ;wait >40us
                        RETURN
;
;
;Reset and initialise display to communicate using 4-bit EQU mode
;Includes routine to clear the display.
;
;Requires the 4-bit instructions 3,3,3,2 to be sent with suitable delays
;following by the 8-bit instructions to set up the display.
;
;  28 = '001' Function set, '0' 4-bit mode, '1' 2-line, '0' 5x7 dot matrix, 'xx'
;  06 = '000001' Entry mode, '1' increment, '0' no display shiload
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
lcd_cursor:             load            s5, #0x10               ;load for line 1
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
;Aloader reading the active counter, all calculations are performed and values loadd
;in scratch pad memory are updated to reflect the new values.
;
;Registers are preserved and reloadd by the ISR so main program is unaffected.
;
isr:                    load           s0, preserve_s0         ;preserve registers
                        load           s1, preserve_s1
                        load           s2, preserve_s2
                        load           s3, preserve_s3
                        load           s4, preserve_s4
                        load           s5, preserve_s5
                        load           s6, preserve_s6
                        load           s7, preserve_s7
                        load           s8, preserve_s8
                        load           s9, preserve_s9
                        load           sa, preserve_sa
                        load           sb, preserve_sb
                        load           sc, preserve_sc
                        load           sd, preserve_sd
                        load           se, preserve_se
                        load           sf, preserve_sf
;
;Ignore the first 4 interrupts except to clear the counter.
;This will ensure a clean start up aloader reset.
;
                        load           s0, isr_count           ;load to see if more that 4 interrupts have occurred
                        load         s0, #0x04
                        JUMP            z, normal_isr
                        ADD             s0, #0x01               ;increment ISR counter until reaching 4
                        load           s0, isr_count
                        INPUT           s0, status_port         ;Check which counter to clear
                        load            s0, #ab_switch          ;if bit0 is Low then A is counting
                        JUMP            z, clear_b_count
clear_a_count:          LOAD            s0, #a_count_reset      ;clear the active counter
                        JUMP            clear_counter
clear_b_count:          LOAD            s0, #b_count_reset      ;clear the active counter
clear_counter:          OUTPUT          s0, count_resetport     ;reset counter with pulse
                        LOAD            s0, #0x00               ;end reset pulse to either counter
                        OUTPUT          s0, count_resetport
                        JUMP            reload_reg
;
;Normal ISR Routine
;
;
;Read the new counter value and then clear it ready to start again
;
;
normal_isr:             INPUT           s0, status_port         ;load for active counter
                        load            s0, #ab_switch          ;if bit is low then A is counting
                        JUMP            z, capture_b_count
capture_a_count:        LOAD            s0, #0xf0               ;set LEDs to indicate active counter
                        OUTPUT          s0, led_port
                        INPUT           sc, a_count0_port       ;read counter A into [sF,sE,SD,sC]
                        INPUT           sd, a_count1_port
                        INPUT           se, a_count2_port
                        INPUT           sf, a_count3_port
                        LOAD            s0, #a_count_reset      ;reset counter A
                        OUTPUT          s0, count_resetport
                        JUMP            counters_read
;
capture_b_count:        LOAD            s0, #0x0f               ;set LEDs to indicate active counter
                        OUTPUT          s0, led_port
                        INPUT           sc, b_count0_port       ;read counter A into [sF,sE,SD,sC]
                        INPUT           sd, b_count1_port
                        INPUT           se, b_count2_port
                        INPUT           sf, b_count3_port
                        LOAD            s0, #b_count_reset      ;reset counter B
                        OUTPUT          s0, count_resetport
counters_read:          LOAD            s0, #0x00               ;end reset pulse to either counter
                        OUTPUT          s0, count_resetport
;
                        load           sc, count0              ;load new counter value
                        load           sd, count1
                        load           se, count2
                        load           sf, count3
;
;
;
;Reload registers and end ISR
;
reload_reg:            load           sf, preserve_sf         ;reload registers
                        load           se, preserve_se
                        load           sd, preserve_sd
                        load           sc, preserve_sc
                        load           sb, preserve_sb
                        load           sa, preserve_sa
                        load           s9, preserve_s9
                        load           s8, preserve_s8
                        load           s7, preserve_s7
                        load           s6, preserve_s6
                        load           s5, preserve_s5
                        load           s4, preserve_s4
                        load           s3, preserve_s3
                        load           s2, preserve_s2
                        load           s1, preserve_s1
                        load           s0, preserve_s0
                        RETURNI         enable
;
;
;Interrupt vector
;
                        ORG             0x3ff
                        JUMP            isr
;
;
;**************************************************************************************
;End of Program
;**************************************************************************************
;
;

