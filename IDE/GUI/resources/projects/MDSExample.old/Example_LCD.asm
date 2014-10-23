; MDS PicoBlaze IDE - Demonstration code
; LCD display driver using macros and if-else


;========================================
; Symbol definition
;========================================

Btns            PORT           0x01             ; Ports
Leds            PORT           0x02             ;
LCD_interface   PORT           0x04             ;

speed           REG            s0               ;
led_data        REG            s1               ;
cnt1            REG            s2               ;
cnt2            REG            s3               ;
btn             REG            s6               ; btns

TASK_time       REG            s4               ; task duration
hlp             REG            s5

LCD_port        REG            s7               ; bit 7 dummy

LCD_E           SET            10               ; bit definitions
LCD_RS          SET            20
LCD_RW          SET            40

; ===================================================================
; interface info
; ===================================================================
;# LCD , 4bit data RW, E, RS
;NET "LCD_interface<0>"  LOC = "AF13"  | IOSTANDARD = LVCMOS33 ; # D4 upper nible
;NET "LCD_interface<1>"  LOC = "AF14"  | IOSTANDARD = LVCMOS33 ; #
;NET "LCD_interface<2>"  LOC = "AF15"  | IOSTANDARD = LVCMOS33 ; #
;NET "LCD_interface<3>"  LOC = "AE15"  | IOSTANDARD = LVCMOS33 ; # D7
;NET "LCD_interface<4>"  LOC = "AC14"  | IOSTANDARD = LVCMOS33 ; # E
;NET "LCD_interface<5>"  LOC = "AD14"  | IOSTANDARD = LVCMOS33 ; # RS
;NET "LCD_interface<6>"  LOC = "AD15"  | IOSTANDARD = LVCMOS33 ; # RW

; ==================================================================
; MACROS
; ==================================================================
LCD_send_command   MACRO     cmd                ; Enable=0 RS=0 Instruction, RW=0 Write, E=0
                                                ;D7     D6      D5      D4
                                                ;D3     D2      D1      D0
                    LOAD     LCD_port,#0b00000000
                    AND      LCD_port,#((cmd & 0xF0) >> 4) | 0xF0
                    OUT      LCD_port,LCD_interface
                    LCD_pulse_E
                    LOAD     LCD_port,#0b00000000
                    AND      LCD_port,#(cmd | 0xF0)
                    OUT      LCD_port,LCD_interface
                    LCD_pulse_E
                    CALL     wait_40us
ENDM
                  ; Upper nibble
                  ; wait >1us
                  ; Lower nibble
                  ; wait >40us

; =======================================================================
; input : 8bit data
; ========================================================================
LCD_send_data     MACRO    cmd     ; ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                    AND      LCD_port,#0xF0
                    LOAD     LCD_port,# cmd
                    OUT      LCD_port,LCD_interface
                    LCD_pulse_E
                    LOAD     LCD_port,#(cmd << 4)
                    OUT      LCD_port,LCD_interface
                    LCD_pulse_E
ENDM

; =======================================================================
; input: 8bit string to display like 'A''B' etc.
; =======================================================================
LCD_send_string   MACRO    cmd     ; ;Enable=0 RS=0 Instruction, RW=0 Write, E=0
                    AND      LCD_port,#0xF0
                    LOAD     LCD_port,# cmd
                    OUT      LCD_port,LCD_interface
                    LCD_pulse_E
                    LOAD     LCD_port,#(cmd << 4)
                    OUT      LCD_port,LCD_interface
                    LCD_pulse_E
ENDM

; ========================================================================
; simple E pulse
; =========================================================================
LCD_pulse_E       MACRO
                  XOR        LCD_port, #LCD_E                       ;E=1
                  OUT        LCD_port, LCD_interface
                  CALL       delay_1us
                  XOR        LCD_port, #LCD_E                       ;E=0
                  OUTPUT     LCD_port, LCD_interface
ENDM
; ==========================================================================
; initialization
; =========================================================================
LCD_init          MACRO
                  LCD_send_command     0x11
ENDM
; ==========================================================================

                 ORG         0h                                          ; Define code segment
                 JUMP        start                                       ; Jump to code initialization

Start:
                 CALL        waitfor100ms         ; wait for logic to be initialized
                 LCD_init
                 load        led_data,#0b10101010
                 load        Task_time,#255
                 ENABLE INTERRUPT
                
main_loop:
                 CALL         buttons
                 CALL         leds_
                 CALL         LCD_rut
                 ADD          cnt1,#1
                 ADD          hlp,#1
                 CALL         waitfor100ms
                 JUMP         main_loop

; =============================================================
; subroutines
; =============================================================
leds_:
                CMP         hlp,task_time
                RET         NZ
                load        hlp,#0
                ; speed grade 1-5
                RL          led_data
                OUT         led_data,leds
                RET
;----------------------------------------------------
buttons:
                IF         cnt1 != #255
                    ret
                ENDIF
                IN         btn,btns
                
                IF         btn != #0
                    ADD        task_time,#50
                endif

                RET
;----------------------------------------------------------
LCD_rut:
                ret

; =================================================================
; wait subroutines
; =================================================================

delay_1us:
                load                            sF,#24
wait_1us_i:
                SUB                                     sF,#1
                JUMP                            NZ,wait_1us_i
                RET
;------------------------------------------------
waitfor100ms:
                load                            sF,#0xF
                load                            sE,#0x1
wait_i:
                sub                                     sF,#1
                JUMP                            NZ,wait_i
                sub                                     sE,#1
                JUMP                            NZ,wait_i
                RET
;-----------------------------------------------------
wait_40us:
                load                            sF,#0xF
                load                            sE,#0x1
wait_i_40u:
                sub                                     sF,#1
                JUMP                            NZ,wait_i_40u
                sub                                     sE,#1
                JUMP                            NZ,wait_i_40u
                RET

; --------------------------------------------------
; interrupt service routinne
;---------------------------------------------------
ISR:
                RETURNI ENABLE
;-------------------------------------------------
; interrupt vector
;-------------------------------------------------
            ORG         0x3FF
            JUMP        ISR
