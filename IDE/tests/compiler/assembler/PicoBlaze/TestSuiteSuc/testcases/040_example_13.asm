                    device kcpsm6
                    ; warning > added only for testing purposes


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
                 CONSTANT rotary_port, 01            ;Read status of rotary encoder
                 CONSTANT rotary_left, 01            ; Direction of last move Left=1 Right=0  - bit0
                 CONSTANT rotary_press, 02           ;     Centre press contact (active High) - bit1
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
                 CONSTANT LCD_input_port, 02         ;LCD character module input data
                 CONSTANT LCD_read_spare0, 01        ;    Spare bits               - bit0
                 CONSTANT LCD_read_spare1, 02        ;    are zero                 - bit1
                 CONSTANT LCD_read_spare2, 04        ;                             - bit2
                 CONSTANT LCD_read_spare3, 08        ;                             - bit3
                 CONSTANT LCD_read_DB4, 10           ;    4-bit           Data DB4 - bit4
                 CONSTANT LCD_read_DB5, 20           ;    interface       Data DB5 - bit5
                 CONSTANT LCD_read_DB6, 40           ;                    Data DB6 - bit6
                 CONSTANT LCD_read_DB7, 80           ;                    Data DB7 - bit7
                 CONSTANT BIT7, 80                   ;
                 CONSTANT BIT6, 40                   ;
                 CONSTANT BIT5, 20                   ;
                 CONSTANT BIT4, 10                   ;
                 CONSTANT BIT3, 08                   ;
                 CONSTANT BIT2, 04                   ;
                 CONSTANT BIT1, 02                   ;
                 CONSTANT BIT0, 01                   ;
                 ;
                 ;**************************************************************************************
                 ;Scratch Pad Memory Locations
                 ;**************************************************************************************
                 ;
                 CONSTANT rotary_status, 00          ;Status of rotary encoder
                 CONSTANT ISR_s0, 01                 ;Preserve s0 contents during ISR
                 CONSTANT LED_pattern, 02            ;LED pattern used in rotation mode
                 ;
                 CONSTANT mode, 03                   ;control mode 00=switch and button FF=rotary
                 CONSTANT ms1, 04
                 CONSTANT secL, 05
                 CONSTANT secH, 06
                 CONSTANT minL, 07
                 CONSTANT minH, 08
                 CONSTANT hourL, 09
                 CONSTANT hourH, 0A
                 CONSTANT week, 0B
                 CONSTANT year1, 0C
                 CONSTANT year2, 0D
                 CONSTANT year3, 0E
                 CONSTANT year4, 0F
                 CONSTANT ms2, 10
                 CONSTANT ms500, 11
                 CONSTANT ISR_s1, 12
                 CONSTANT ms100, 13
                 CONSTANT ROTA_CNT, 14
                 CONSTANT ISR_s2, 15
                 CONSTANT temp, 16
                 CONSTANT pressed, 17
                 CONSTANT disp_flag, 18
                 CONSTANT blink_flag, 19
                 CONSTANT disp_flag2, 1A             ;;;;;no use
                 CONSTANT monH, 1B
                 CONSTANT monL, 1C
                 CONSTANT dayH, 1D
                 CONSTANT dayL, 1E
                 CONSTANT DAY_UP, 1F
                 CONSTANT year100, 20
                 CONSTANT hourB, 21
                 CONSTANT minB, 22
                 CONSTANT secB, 23
                 CONSTANT yearB, 24
                 CONSTANT monB, 25
                 CONSTANT dayB, 26
                 CONSTANT DMAX, 27                   ;
                 CONSTANT JAN, 28                    ;
                 CONSTANT FEB, 29                    ;
                 CONSTANT MAR, 2A                    ;
                 CONSTANT APR, 2B                    ;
                 CONSTANT MAY, 2C                    ;
                 CONSTANT JUN, 2D                    ;
                 CONSTANT JUL, 2E                    ;
                 CONSTANT AUG, 2F                    ;
                 CONSTANT SEP, 30                    ;
                 CONSTANT OCT, 31                    ;
                 CONSTANT NOV, 32                    ;
                 CONSTANT DEC, 33                    ;
                 CONSTANT JAN2, 34                   ;
                 CONSTANT FEB2, 35                   ;
                 CONSTANT MAR2, 36                   ;
                 CONSTANT APR2, 37                   ;
                 CONSTANT MAY2, 38                   ;
                 CONSTANT JUN2, 39                   ;
                 CONSTANT JUL2, 3A                   ;
                 CONSTANT AUG2, 3B                   ;
                 CONSTANT SEP2, 3C                   ;
                 CONSTANT OCT2, 3D                   ;
                 CONSTANT NOV2, 3E                   ;
                 CONSTANT DEC2, 3F                   ;
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
                 ; delay_1us_constant =  (clock_rate - 6)/4       Where 'clock_rate' is in MHz
                 ;
                 ;Example: For a 50MHz clock the constant value is (10-6)/4 = 11  (0B Hex).
                 ;For clock rates below 10MHz the value of 1 must be used and the operation will
                 ;become lower than intended.
                 ;
                 CONSTANT delay_1us_constant, 0B
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
          reset: CALL LCD_reset
                 ENABLE INTERRUPT
                 LOAD sD, FF
                 LOAD s0, 00                         ;
                 STORE s0, hourH
                 STORE s0, minL
                 STORE s0, minH
                 STORE s0, secH
                 STORE s0, secL
                 STORE s0, year2
                 STORE s0, year3
                 STORE s0, monH
                 STORE s0, minB                      ;;;;;;;;;;
                 STORE s0, secB                      ;;;;;;;;;;
                 LOAD s0, 09                         ;
                 STORE s0, hourL
                 STORE s0, hourB                     ;;;;;;;;;
                 LOAD s0, 02                         ;
                 STORE s0, year1
                 STORE s0, dayH
                 LOAD s0, 06                         ;
                 STORE s0, dayL
                 STORE s0, year4
                 STORE s0, yearB                     ;;;;;;;;;
                 LOAD s0, 05                         ;
                 STORE s0, monL
                 STORE s0, week
                 STORE s0, monB                      ;;;;;;;;;
                 LOAD s0, 1A
                 STORE s0, dayB                      ;;;;;;;;;
                 LOAD s0, 1F
                 STORE s0, JAN                       ;1
                 STORE s0, MAR                       ;3
                 STORE s0, MAY                       ;5
                 STORE s0, JUL                       ;7
                 STORE s0, AUG                       ;8
                 STORE s0, OCT                       ;10
                 STORE s0, DEC                       ;12
                 LOAD s0, 1E
                 STORE s0, APR                       ;4
                 STORE s0, JUN                       ;6
                 STORE s0, SEP                       ;9
                 STORE s0, NOV                       ;11
                 LOAD s0, 1C
                 STORE s0, FEB                       ;2
                 LOAD s0, 03
                 STORE s0, JAN2
                 STORE s0, OCT2
                 LOAD s0, 06
                 STORE s0, FEB2
                 STORE s0, MAR2
                 STORE s0, NOV2
                 LOAD s0, 02
                 STORE s0, APR2
                 STORE s0, JUL2
                 LOAD s0, 04
                 STORE s0, MAY2
                 LOAD s0, 00
                 STORE s0, JUN2
                 LOAD s0, 05
                 STORE s0, AUG2
                 LOAD s0, 01
                 STORE s0, SEP2
                 STORE s0, DEC2
                 LOAD sF, 00                         ;
        restart:
                 LOAD s5, 10
                 CALL LCD_cursor
                 CALL disp_msg1
                 LOAD s5, 20
                 CALL LCD_cursor
                 CALL disp_msg2
                 LOAD s3, 35
                 STORE s3, temp
        BLINK10: LOAD s3, 00
                 STORE s3, ms100
         BLINK1: FETCH s3, ms100
                 COMPARE s3, 05
                 JUMP C, BLINK1
                 LOAD s5, 2F
                 CALL LCD_cursor
                 FETCH s5, temp
                 CALL LCD_write_data
                 LOAD s3, 00
                 STORE s3, ms100
          wait1: FETCH s3, ms100
                 COMPARE s3, 05
                 JUMP C, wait1
                 LOAD s5, 2F
                 CALL LCD_cursor
                 LOAD s5, 20
                 CALL LCD_write_data
                 FETCH s5, temp
                 ADD s5, FF
                 STORE s5, temp
                 COMPARE s5, 2F
                 JUMP NZ, BLINK10
       RRESTART: LOAD s5, 10
                 CALL LCD_cursor
                 CALL disp_msg3
                 LOAD s5, 20
                 CALL LCD_cursor
                 CALL disp_msg4
                 LOAD s3, 35
                 STORE s3, temp
        BLINK20: LOAD s3, 00
                 STORE s3, ms100
         BLINK2: FETCH s3, ms100
                 COMPARE s3, 05
                 JUMP C, BLINK2
                 LOAD s5, 2F
                 CALL LCD_cursor
                 FETCH s5, temp
                 CALL LCD_write_data
                 LOAD s3, 00
                 STORE s3, ms100
          wait2: FETCH s3, ms100
                 COMPARE s3, 05
                 JUMP C, wait2
                 LOAD s5, 2F
                 CALL LCD_cursor
                 LOAD s5, 20
                 CALL LCD_write_data
                 FETCH s5, temp
                 ADD s5, FF
                 STORE s5, temp
                 COMPARE s5, 2F
                 JUMP NZ, BLINK20
                 LOAD s0, 00
                 STORE s0, LED_pattern
                 STORE s0, mode
                 STORE s0, blink_flag
                 STORE s0, ms100
                 LOAD s5, 10
                 CALL LCD_cursor
                 CALL disp_msg5
                 LOAD s5, 20
                 CALL LCD_cursor
                 CALL disp_msg6
          PAUSE: FETCH s0, ms100
                 COMPARE s0, 32
                 JUMP C, PAUSE
                 CALL LCD_clear
                 INPUT s0, 00
                 STORE s0, disp_flag2
                 STORE s0, ms100
           loop:
                 FETCH s0, hourB
                 CALL B2BCD
                 STORE s0, hourL
                 STORE s1, hourH
                 FETCH s0, minB
                 CALL B2BCD
                 STORE s0, minL
                 STORE s1, minH
                 FETCH s0, secB
                 CALL B2BCD
                 STORE s0, secL
                 STORE s1, secH
                 FETCH s0, dayB
                 CALL B2BCD
                 STORE s0, dayL
                 STORE s1, dayH
                 FETCH s0, monB
                 CALL B2BCD
                 STORE s0, monL
                 STORE s1, monH
                 FETCH s0, yearB
                 CALL B2BCD
                 STORE s0, year4
                 STORE s1, year3
                 AND sF, FF
                 JUMP NZ, SKIP1
                 ;;;DISP HH
                 LOAD s5, 11
                 CALL LCD_cursor
                 FETCH s3, mode
                 COMPARE s3, 01
                 JUMP NZ, DSP_HH
                 FETCH s3, ms500
                 TEST s3, 01
                 JUMP NZ, SPACE7
         DSP_HH: FETCH s5, hourH
                 ADD s5, 30
                 CALL LCD_write_data
                 FETCH s5, hourL
                 ADD s5, 30
                 CALL LCD_write_data
                 JUMP MODE_2
         SPACE7: CALL disp_space
                 CALL disp_space
         MODE_2:
                 ;;;DISP MM
                 LOAD s5, 14
                 CALL LCD_cursor
                 FETCH s3, mode
                 COMPARE s3, 02
                 JUMP NZ, DSP_MM
                 FETCH s3, ms500
                 TEST s3, 01
                 JUMP NZ, SPACE6
         DSP_MM: FETCH s5, minH
                 ADD s5, 30
                 CALL LCD_write_data
                 FETCH s5, minL
                 ADD s5, 30
                 CALL LCD_write_data
                 JUMP MODE_3
         SPACE6: CALL disp_space
                 CALL disp_space
         MODE_3:
                 ;;;DISP SS
                 LOAD s5, 17
                 CALL LCD_cursor
                 FETCH s3, mode
                 COMPARE s3, 03
                 JUMP NZ, DSP_SS
                 FETCH s3, ms500
                 TEST s3, 01
                 JUMP NZ, SPACE5
         DSP_SS: FETCH s5, secH
                 ADD s5, 30
                 CALL LCD_write_data
                 FETCH s5, secL
                 ADD s5, 30
                 CALL LCD_write_data
                 JUMP MODE_4
         SPACE5: CALL disp_space
                 CALL disp_space
         MODE_4:
                 ;;;DISP week
                 LOAD s5, 1C
                 CALL LCD_cursor
                 FETCH s3, week
                 COMPARE s3, 00
                 JUMP NZ, MON
                 LOAD s5, character_S
                 CALL LCD_write_data
                 LOAD s5, character_u
                 CALL LCD_write_data
                 LOAD s5, character_n
                 CALL LCD_write_data
                 JUMP TEST_BIT0
            MON: FETCH s3, week
                 COMPARE s3, 01
                 JUMP NZ, TUE
                 LOAD s5, character_M
                 CALL LCD_write_data
                 LOAD s5, character_o
                 CALL LCD_write_data
                 LOAD s5, character_n
                 CALL LCD_write_data
                 JUMP TEST_BIT0
            TUE: FETCH s3, week
                 COMPARE s3, 02
                 JUMP NZ, WED
                 LOAD s5, character_T
                 CALL LCD_write_data
                 LOAD s5, character_u
                 CALL LCD_write_data
                 LOAD s5, character_e
                 CALL LCD_write_data
                 JUMP TEST_BIT0
            WED: FETCH s3, week
                 COMPARE s3, 03
                 JUMP NZ, THU
                 LOAD s5, character_W
                 CALL LCD_write_data
                 LOAD s5, character_e
                 CALL LCD_write_data
                 LOAD s5, character_d
                 CALL LCD_write_data
                 JUMP TEST_BIT0
            THU: FETCH s3, week
                 COMPARE s3, 04
                 JUMP NZ, FRI
                 LOAD s5, character_T
                 CALL LCD_write_data
                 LOAD s5, character_h
                 CALL LCD_write_data
                 LOAD s5, character_u
                 CALL LCD_write_data
                 JUMP TEST_BIT0
            FRI: FETCH s3, week
                 COMPARE s3, 05
                 JUMP NZ, SAT
                 LOAD s5, character_F
                 CALL LCD_write_data
                 LOAD s5, character_r
                 CALL LCD_write_data
                 LOAD s5, character_i
                 CALL LCD_write_data
                 JUMP TEST_BIT0
            SAT: LOAD s5, character_S
                 CALL LCD_write_data
                 LOAD s5, character_a
                 CALL LCD_write_data
                 LOAD s5, character_t
                 CALL LCD_write_data
                 JUMP TEST_BIT0
      TEST_BIT0:
  ROTARY_PRESS1:
  ROTARY_PRESS2:
          SKIP1:
                 ;;;DISP month
                 LOAD s5, 26
                 CALL LCD_cursor
                 FETCH s3, mode
                 COMPARE s3, 04
                 JUMP NZ, DSP_MON
                 FETCH s3, ms500
                 TEST s3, 01
                 JUMP NZ, SPACE4
        DSP_MON: FETCH s5, monH
                 ADD s5, 30
                 CALL LCD_write_data
                 FETCH s5, monL
                 ADD s5, 30
                 CALL LCD_write_data
                 JUMP MODE_5
         SPACE4: CALL disp_space
                 CALL disp_space
         MODE_5:
                 ;;;DISP day
                 LOAD s5, 29
                 CALL LCD_cursor
                 FETCH s3, mode
                 COMPARE s3, 05
                 JUMP NZ, DSP_DD
                 FETCH s3, ms500
                 TEST s3, 01
                 JUMP NZ, SPACE3
         DSP_DD: FETCH s5, dayH
                 ADD s5, 30
                 CALL LCD_write_data
                 FETCH s5, dayL
                 ADD s5, 30
                 CALL LCD_write_data
                 JUMP MODE_6
         SPACE3: CALL disp_space
                 CALL disp_space
         MODE_6:
                 ;;;DISP year
                 LOAD s5, 2C
                 CALL LCD_cursor
                 FETCH s3, mode
                 COMPARE s3, 06
                 JUMP NZ, DSP_YY
                 FETCH s3, ms500
                 TEST s3, 01
                 JUMP NZ, SPACE2
         DSP_YY: FETCH s5, year1
                 ADD s5, 30
                 CALL LCD_write_data
                 FETCH s5, year2
                 ADD s5, 30
                 CALL LCD_write_data
                 FETCH s5, year3
                 ADD s5, 30
                 CALL LCD_write_data
                 FETCH s5, year4
                 ADD s5, 30
                 CALL LCD_write_data
                 JUMP MODE_END
         SPACE2: CALL disp_space
                 CALL disp_space
                 CALL disp_space
                 CALL disp_space
       MODE_END:
                 FETCH s0, monB
                 LOAD s1, DMAX
                 ADD s1, s0
                 FETCH s2, (s1)                      ;;28
                 COMPARE s0, 02                      ;;Y
                 JUMP NZ, NO_FEB
                 FETCH s1, yearB
                 AND s1, 03
                 JUMP NZ, NO_FEB
                 ADD s2, 01
     NO_RUNNIAN:
         NO_FEB: STORE s2, DMAX
                 FETCH s1, dayB
                 COMPARE s2, s1
                 JUMP NC, UNDER
                 STORE s2, dayB
          UNDER:
                 FETCH s1, DAY_UP
                 AND s1, s1
                 JUMP Z, NO_DAY_UP
                 FETCH s1, dayB
                 ADD s1, 01
                 STORE s1, dayB
                 FETCH s2, DMAX
                 ADD s2, 01
                 COMPARE s1, s2
                 JUMP NZ, NO_MON_UP
                 LOAD s1, 01
                 STORE s1, dayB
                 FETCH s1, monB
                 ADD s1, 01
                 STORE s1, monB
                 COMPARE s1, 0D
                 JUMP NZ, NO_YEAR_UP
                 LOAD s1, 01
                 STORE s1, monB
                 FETCH s1, yearB
                 ADD s1, 01
                 STORE s1, yearB
                 COMPARE s1, 64
                 JUMP NZ, NO_YEAR_UP
                 LOAD s1, 00
                 STORE s1, yearB
                 JUMP NO_YEAR_UP
      NO_MON_UP:
      NO_DAY_UP:
     NO_YEAR_UP:
                 LOAD s0, 00
                 STORE s0, DAY_UP
                 LOAD s0, DEC
                 FETCH s2, monB
                 ADD s0, s2
                 FETCH s1, (s0)                      ;;;K
                 FETCH s0, yearB
                 ADD s1, s0                          ;;;;;;;K+Y
                 COMPARE s2, 03
                 JUMP C, RN
                 ADD s0, 01                          ;;;;100+Y  >2(3..12)
             RN: ADD s0, 63                          ;;;;;99+Y   1,2
                 SR0 s0
                 SR0 s0                              ;;;/4
                 ADD s0, s1
                 ADD s0, 06
                 FETCH s2, dayB
                 ADD s0, s2
           MOD7: ADD s0, F9
                 JUMP NC, M7_END
                 JUMP MOD7
         M7_END: ADD s0, 07
                 STORE s0, week
                 FETCH s0, mode
                 AND s0, FF
                 JUMP NZ, SETUP
                 ;;;RUN
                 LOAD s5, 20
                 CALL LCD_cursor
                 FETCH s5, disp_flag
                 CALL LCD_write_data
                 CALL LCD_write_data
                 CALL LCD_write_data
                 JUMP MODEUPDATE
          SETUP:
                 ;;;SET
                 LOAD s5, 20
                 CALL LCD_cursor
                 LOAD s5, 53                         ;;;;;;;;;;;;;;;;;;;;;;;    S
                 CALL LCD_write_data
                 LOAD s5, 45                         ;;;;;;;;;;;;;;;;;;;;;;;    E
                 CALL LCD_write_data
                 LOAD s5, 54                         ;;;;;;;;;;;;;;;;;;;;;;;    T
                 CALL LCD_write_data
     MODEUPDATE: FETCH s0, mode
                 STORE s0, blink_flag
       NOCHANGE:
                 INPUT s0, 00
                 FETCH s1, disp_flag2
                 XOR s1, s0
                 JUMP NZ, restart
                 STORE s0, disp_flag2
                 FETCH s0, monB
                 COMPARE s0, 05
                 JUMP NZ, chk_sf
                 FETCH s0, dayB
                 COMPARE s0, 11
                 JUMP NZ, chk_sf
                 FETCH s0, minB
                 COMPARE s0, 00
                 JUMP Z, msg7
         chk_sf: COMPARE sF, FF
                 JUMP NZ, NO_BD
                 CALL LCD_reset
                 JUMP NO_BD
           msg7: LOAD s5, 10
                 CALL LCD_cursor
                 CALL disp_msg7                      ;
                 FETCH s0, ms100
                 COMPARE s0, 32
                 JUMP C, WAIT
                 LOAD sF, 00
                 JUMP restart
           WAIT: LOAD sF, FF
                 JUMP loop
          NO_BD: LOAD sF, 00
                 STORE sF, ms100
                 JUMP loop
                 ;;;;;;;;;s0---BINARY, s1---BCD_H, s0---BCD_L
          B2BCD: LOAD s1, 00                         ;
         BCD_LP: ADD s0, F6
                 JUMP NC, H_END
                 ADD s1, 01                          ;
                 JUMP BCD_LP
          H_END: ADD s0, 0A
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
                 CALL disp_space
                 LOAD s5, character_A
                 CALL LCD_write_data
                 CALL disp_space
                 LOAD s5, character_C
                 CALL LCD_write_data
                 LOAD s5, character_l
                 CALL LCD_write_data
                 LOAD s5, character_o
                 CALL LCD_write_data
                 LOAD s5, character_c
                 CALL LCD_write_data
                 LOAD s5, character_k
                 CALL LCD_write_data
                 CALL disp_space
                 LOAD s5, character_B
                 CALL LCD_write_data
                 LOAD s5, character_a
                 CALL LCD_write_data
                 LOAD s5, character_s
                 CALL LCD_write_data
                 LOAD s5, character_e
                 CALL LCD_write_data
                 LOAD s5, character_d
                 CALL LCD_write_data
                 CALL disp_space
                 RETURN
                 ;
                 ;
                 ;
      disp_msg2: LOAD s5, character_S
                 CALL LCD_write_data
                 LOAD s5, character_3
                 CALL LCD_write_data
                 LOAD s5, character_E
                 CALL LCD_write_data
                 CALL disp_space
                 LOAD s5, character_S
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
                 CALL disp_space
                 LOAD s5, character_K
                 CALL LCD_write_data
                 LOAD s5, character_i
                 CALL LCD_write_data
                 LOAD s5, character_t
                 CALL LCD_write_data
                 LOAD s5, character_3
                 CALL LCD_write_data
                 RETURN
      disp_msg3: CALL disp_space
                 CALL disp_space
                 CALL disp_space
                 CALL disp_space
                 CALL disp_space
                 LOAD s5, character_X
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
                 CALL disp_space
                 CALL disp_space
                 CALL disp_space
                 CALL disp_space
                 CALL disp_space
                 RETURN
      disp_msg4: LOAD s5, character_P
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
                 CALL disp_space
                 LOAD s5, character_T
                 CALL LCD_write_data
                 LOAD s5, character_e
                 CALL LCD_write_data
                 LOAD s5, character_s
                 CALL LCD_write_data
                 LOAD s5, character_t
                 CALL LCD_write_data
                 CALL disp_space
                 RETURN
      disp_msg5: LOAD s5, character_P
                 CALL LCD_write_data
                 LOAD s5, character_r
                 CALL LCD_write_data
                 LOAD s5, character_e
                 CALL LCD_write_data
                 LOAD s5, character_s
                 CALL LCD_write_data
                 LOAD s5, character_s
                 CALL LCD_write_data
                 CALL disp_space
                 LOAD s5, 26
                 CALL LCD_write_data
                 CALL disp_space
                 LOAD s5, character_T
                 CALL LCD_write_data
                 LOAD s5, character_u
                 CALL LCD_write_data
                 LOAD s5, character_r
                 CALL LCD_write_data
                 LOAD s5, character_n
                 CALL LCD_write_data
                 CALL disp_space
                 LOAD s5, character_t
                 CALL LCD_write_data
                 LOAD s5, character_h
                 CALL LCD_write_data
                 LOAD s5, character_e
                 CALL LCD_write_data
                 RETURN
      disp_msg6: CALL disp_space
                 LOAD s5, character_E
                 CALL LCD_write_data
                 LOAD s5, character_n
                 CALL LCD_write_data
                 LOAD s5, character_c
                 CALL LCD_write_data
                 LOAD s5, character_o
                 CALL LCD_write_data
                 LOAD s5, character_d
                 CALL LCD_write_data
                 LOAD s5, character_e
                 CALL LCD_write_data
                 LOAD s5, character_r
                 CALL LCD_write_data
                 CALL disp_space
                 LOAD s5, character_t
                 CALL LCD_write_data
                 LOAD s5, character_o
                 CALL LCD_write_data
                 CALL disp_space
                 LOAD s5, character_S
                 CALL LCD_write_data
                 LOAD s5, character_e
                 CALL LCD_write_data
                 LOAD s5, character_t
                 CALL LCD_write_data
                 CALL disp_space
                 RETURN
      disp_msg7: LOAD s5, character_H
                 CALL LCD_write_data
                 LOAD s5, character_a
                 CALL LCD_write_data
                 LOAD s5, character_p
                 CALL LCD_write_data
                 LOAD s5, character_p
                 CALL LCD_write_data
                 LOAD s5, character_y
                 CALL LCD_write_data
                 CALL disp_space
                 LOAD s5, character_B
                 CALL LCD_write_data
                 LOAD s5, character_i
                 CALL LCD_write_data
                 LOAD s5, character_r
                 CALL LCD_write_data
                 LOAD s5, character_t
                 CALL LCD_write_data
                 LOAD s5, character_h
                 CALL LCD_write_data
                 LOAD s5, character_d
                 CALL LCD_write_data
                 LOAD s5, character_a
                 CALL LCD_write_data
                 LOAD s5, character_y
                 CALL LCD_write_data
                 LOAD s5, 21
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
                 LOAD s5, 13
                 CALL LCD_cursor
                 LOAD s5, 3A                         ;;;;;;;;;;;;;;;;;;;;;;;    :
                 CALL LCD_write_data
                 LOAD s5, 16
                 CALL LCD_cursor
                 LOAD s5, 2D                         ;;;;;;;;;;;;;;;;;;;;;;     -
                 CALL LCD_write_data
                 LOAD s5, 20
                 CALL LCD_cursor
                 LOAD s5, 2D                         ;;;;;;;;;;;;;;;;;;;;;;     -
                 CALL LCD_write_data
                 LOAD s5, 2D                         ;;;;;;;;;;;;;;;;;;;;;;     -
                 CALL LCD_write_data
                 LOAD s5, 2D                         ;;;;;;;;;;;;;;;;;;;;;;     -
                 CALL LCD_write_data
                 LOAD s5, 28
                 CALL LCD_cursor
                 LOAD s5, 2F                         ;;;;;;;;;;;;;;;;;;;;;;     /
                 CALL LCD_write_data
                 LOAD s5, 2B
                 CALL LCD_cursor
                 LOAD s5, 2F                         ;;;;;;;;;;;;;;;;;;;;;;     -
                 CALL LCD_write_data
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
                 ;LCD_shift_left: LOAD s5, 18                         ;shift display left
                 ;                CALL LCD_write_inst8
                 ;                RETURN
                 ;
                 ;**************************************************************************************
                 ;Interrupt Service Routine (ISR)
                 ;**************************************************************************************
            ISR: STORE s0, ISR_s0                    ;preserve s0
                 STORE s1, ISR_s1
                 STORE s2, ISR_s2
                 INPUT s1, rotary_port
                 LOAD s0, 00
                 TEST s1, rotary_press
                 JUMP Z, no_press
                 FETCH s0, pressed
                 COMPARE s0, 00
                 JUMP NZ, no_press
                 FETCH s2, LED_pattern
                 ADD s2, 01
                 STORE s2, LED_pattern
                 FETCH s2, mode
                 ADD s2, 01
                 COMPARE s2, 07
                 JUMP NZ, mode_save
                 AND s2, 00
      mode_save: STORE s2, mode                      ;;;;;0-normal;1-hour;2-min;3-sec;4-day,5-mon,6-year
                 LOAD s0, FF
       no_press: STORE s0, pressed
                 TEST s1, 40                         ;;;rotary INT ?
                 JUMP Z, T2_5m_INT                   ;;NOT rotary INT,JUMP away
       CLR_INT2: LOAD s0, 40                         ;;;;;CLR_INT1
                 OUTPUT s0, 20
                 FETCH s0, LED_pattern
                 TEST s1, rotary_left
                 JUMP Z, ITS_RIGHT
                 ADD s0, FF
                 STORE s0, LED_pattern
                 ;;;turn left,--
                 FETCH s0, mode
                 COMPARE s0, 00
                 JUMP Z, CLR_INT1                    ;
                 COMPARE s0, 01
                 JUMP NZ, MODE2N
                 FETCH s1, hourB
                 ADD s1, FF
                 COMPARE s1, FF                      ;;;24
                 JUMP NZ, SAVE_HHN
                 LOAD s1, 17
       SAVE_HHN: STORE s1, hourB
                 JUMP real_time_end
         MODE2N: COMPARE s0, 02
                 JUMP NZ, MODE3N
                 FETCH s1, minB
                 ADD s1, FF
                 COMPARE s1, FF                      ;;;3C;;60
                 JUMP NZ, SAVE_MMN
                 LOAD s1, 3B
       SAVE_MMN: STORE s1, minB
                 JUMP real_time_end
         MODE3N: COMPARE s0, 03
                 JUMP NZ, MODE4N
                 FETCH s1, secB
                 ADD s1, FF
                 COMPARE s1, FF                      ;;;60
                 JUMP NZ, SAVE_SSN
                 LOAD s1, 3B
       SAVE_SSN: STORE s1, secB
                 JUMP real_time_end
         MODE4N: COMPARE s0, 04
                 JUMP NZ, MODE5N
                 FETCH s1, monB
                 ADD s1, FF
                 COMPARE s1, 00                      ;;;12
                 JUMP NZ, SAVE_MONN
                 LOAD s1, 0C
      SAVE_MONN: STORE s1, monB
                 JUMP real_time_end
         MODE5N: COMPARE s0, 05
                 JUMP NZ, MODE6N
                 FETCH s1, dayB
                 ADD s1, FF
                 COMPARE s1, 00                      ;;;12
                 JUMP NZ, SAVE_DAYN
                 FETCH s1, DMAX
      SAVE_DAYN: STORE s1, dayB
                 JUMP real_time_end
         MODE6N: COMPARE s0, 06
                 JUMP NZ, MODE7N
                 FETCH s1, yearB
                 ADD s1, FF
                 COMPARE s1, FF                      ;;;12
                 JUMP NZ, SAVE_YEARN
                 LOAD s1, 63
     SAVE_YEARN: STORE s1, yearB
         MODE7N:
                 JUMP real_time_end
      ITS_RIGHT: ADD s0, 01
                 STORE s0, LED_pattern
                 ;;;turn right++
                 FETCH s0, mode
                 COMPARE s0, 00
                 JUMP Z, CLR_INT1                    ;
                 COMPARE s0, 01
                 JUMP NZ, MODE2P
                 FETCH s1, hourB
                 ADD s1, 01
                 COMPARE s1, 18                      ;;;24
                 JUMP NZ, SAVE_HHP
                 LOAD s1, 00
       SAVE_HHP: STORE s1, hourB
                 JUMP real_time_end
         MODE2P: COMPARE s0, 02
                 JUMP NZ, MODE3P
                 FETCH s1, minB
                 ADD s1, 01
                 COMPARE s1, 3C                      ;;;3C;;60
                 JUMP NZ, SAVE_MMP
                 LOAD s1, 00
       SAVE_MMP: STORE s1, minB
                 JUMP real_time_end
         MODE3P: COMPARE s0, 03
                 JUMP NZ, MODE4P
                 FETCH s1, secB
                 ADD s1, 01
                 COMPARE s1, 3C                      ;;;60
                 JUMP NZ, SAVE_SSP
                 LOAD s1, 00
       SAVE_SSP: STORE s1, secB
                 JUMP real_time_end
         MODE4P: COMPARE s0, 04
                 JUMP NZ, MODE5P
                 FETCH s1, monB
                 ADD s1, 01
                 COMPARE s1, 0D                      ;;;12
                 JUMP NZ, SAVE_MONP
                 LOAD s1, 01
      SAVE_MONP: STORE s1, monB
                 JUMP real_time_end
         MODE5P: COMPARE s0, 05
                 JUMP NZ, MODE6P
                 FETCH s1, dayB
                 ADD s1, 01
                 FETCH s2, DMAX
                 ADD s2, 01
                 COMPARE s1, s2
                 JUMP NZ, SAVE_DAYP
                 LOAD s1, 01
      SAVE_DAYP: STORE s1, dayB
                 JUMP real_time_end
         MODE6P: COMPARE s0, 06
                 JUMP NZ, MODE7P
                 FETCH s1, yearB
                 ADD s1, 01
                 COMPARE s1, 64                      ;;;12
                 JUMP NZ, SAVE_YEARP
                 LOAD s1, 00
     SAVE_YEARP: STORE s1, yearB
         MODE7P: JUMP real_time_end
       CLR_INT1: LOAD s0, 40                         ;;;;;CLR_INT1
                 OUTPUT s0, 20
                 ;;;TIMER interrupt process;;;;mode:::0-normal;1-hour;2-min;3-sec;4-day,5-mon,6-year
      T2_5m_INT: TEST s1, 80
                 JUMP Z, end_ISR
                 LOAD s0, 80                         ;;;;;;;;;;clr_int2
                 OUTPUT s0, 20
                 FETCH s0, ms1
                 ADD s0, 01
                 STORE s0, ms1
                 COMPARE s0, 64
                 JUMP NZ, real_time_end
                 AND s0, 00                          ;;;;;;;;;;;;;CLR ms1
                 STORE s0, ms1
                 FETCH s0, LED_pattern               ;
                 ADD s0, 10
                 STORE s0, LED_pattern
                 FETCH s1, ms500
                 ADD s1, 01
                 STORE s1, ms500
                 COMPARE s1, 01                      ;
                 JUMP NZ, CMP_2
                 LOAD s0, 2D                         ;;;------
                 JUMP CMP_4
          CMP_2: COMPARE s1, 02                      ;
                 JUMP NZ, CMP_3
                 LOAD s0, 60                         ;;;\\\\\\
                 JUMP CMP_4
          CMP_3: COMPARE s1, 03                      ;
                 JUMP NZ, CMP_4
                 LOAD s0, 7C                         ;;;||||
          CMP_4: STORE s0, disp_flag
                 COMPARE s1, 04
                 JUMP NZ, real_time_end
                 LOAD s0, 2F                         ;;;////
                 ;;FETCH s0,disp_flag
                 ;;ADD s0,01
                 STORE s0, disp_flag
                 AND s1, 00                          ;;;;;;;;;;;;;CLR ms500
                 STORE s1, ms500
                 FETCH s0, secB                      ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;CLOCK PROCESS START
                 ADD s0, 01
                 STORE s0, secB
                 COMPARE s0, 3C                      ;;;60
                 JUMP NZ, real_time_end
                 AND s0, 00
                 STORE s0, secB
                 FETCH s0, minB
                 ADD s0, 01
                 STORE s0, minB
                 COMPARE s0, 3C
                 JUMP NZ, real_time_end
                 AND s0, 00
                 STORE s0, minB
                 ;;;24 Hours
                 FETCH s0, hourB
                 ADD s0, 01
                 STORE s0, hourB
                 COMPARE s0, 18
                 JUMP NZ, real_time_end
                 AND s0, 00
                 STORE s0, hourB
          DAY_P: FETCH s0, DAY_UP
                 LOAD s0, 01
                 STORE s0, DAY_UP
  real_time_end: FETCH s0, ms2                       ;;;;;;;;2.5ms counter
                 ADD s0, 01
                 COMPARE s0, 28                      ;;;;;;counter to 100ms
                 JUMP NZ, store_ms2
                 FETCH s1, ms100
                 ADD s1, 01
      SAV_ms100: STORE s1, ms100
                 LOAD s0, 00
      store_ms2: STORE s0, ms2
                 FETCH s0, LED_pattern
                 OUTPUT s0, LED_port
        end_ISR: FETCH s0, ISR_s0                    ;restore s0
                 FETCH s1, ISR_s1                    ;restore s1
                 FETCH s2, ISR_s2                    ;restore s2
                 RETURNI ENABLE
                 ;**************************************************************************************
                 ;Interrupt Vector
                 ;**************************************************************************************
                 ;
                 ADDRESS 3FF
                 JUMP ISR
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
