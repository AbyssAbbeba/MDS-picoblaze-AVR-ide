; KCPSM3 Program - Automatic Pulse Width Modulation (PWM) Control on the Spartan-3E Starter Kit.
;
; Ken Chapman - Xilinx Ltd
;
; Version v1.00 - 24th May 2006
device kcpsm2
; Automatically sequences the LEDs on the board using PWM to change intensity.
;
;**************************************************************************************
; Port definitions
;**************************************************************************************
;
;
;
led_port                EQU     0x80                    ;8 simple LEDs
led0                    EQU     0x01                    ;     LED 0 - bit0
led1                    EQU     0x02                    ;         1 - bit1
led2                    EQU     0x04                    ;         2 - bit2
led3                    EQU     0x08                    ;         3 - bit3
led4                    EQU     0x10                    ;         4 - bit4
led5                    EQU     0x20                    ;         5 - bit5
led6                    EQU     0x40                    ;         6 - bit6
led7                    EQU     0x80                    ;         7 - bit7
;
;
simple_port             EQU     0x40                    ;4 simple outputs
simple_io9              EQU     0x01                    ;  Header  IO9  - bit0
simple_io10             EQU     0x02                    ;          IO10 - bit1
simple_io11             EQU     0x04                    ;          IO11 - bit2
simple_io12             EQU     0x08                    ;          IO12 - bit3
;
;
;
status_port             EQU     0x00                    ;UART status input
tx_half_full            EQU     0x01                    ;  Transmitter     half full - bit0
tx_full                 EQU     0x02                    ;    FIFO               full - bit1
rx_EQU_present         EQU     0x04                    ;  Receiver     EQU present - bit2
rx_half_full            EQU     0x08                    ;    FIFO          half full - bit3
rx_full                 EQU     0x10                    ;                   full - bit4
spare1                  EQU     0x20                    ;                  spare '0' - bit5
spare2                  EQU     0x40                    ;                  spare '0' - bit6
spare3                  EQU     0x80                    ;                  spare '0' - bit7
;
uart_read_port          EQU     0x01                    ;UART Rx EQU input
;
uart_write_port         EQU     0x20                    ;UART Tx EQU output
;
;
;
;**************************************************************************************
; Special Register usage
;**************************************************************************************
;
uart_EQU               REG     sf                      ;used to pass EQU to and from the UART
;
;
;
;**************************************************************************************
;Scratch Pad Memory Locations
;**************************************************************************************
;
pwm_duty_counter        EQU     0x00                    ;Duty Counter 0 to 255 within 1KHz period (1ms)
pwm_channel0            EQU     0x01                    ;PWM settings for each channel
pwm_channel1            EQU     0x02                    ; Channels 0 to 7 = LEDs 0 to 7
pwm_channel2            EQU     0x03                    ; Channels 8 to 11 = IO9 to IO12
pwm_channel3            EQU     0x04
pwm_channel4            EQU     0x05
pwm_channel5            EQU     0x06
pwm_channel6            EQU     0x07
pwm_channel7            EQU     0x08
pwm_channel8            EQU     0x09
pwm_channel9            EQU     0x0a
pwm_channel10           EQU     0x0b
pwm_channel11           EQU     0x0c
isr_preserve_s0         EQU     0x0d                    ;preserve register contents during Interrupt Service Routine
isr_preserve_s1         EQU     0x0e
isr_preserve_s2         EQU     0x0f
;
;
led0_sequence           EQU     0x10                    ;LED sequence values
led1_sequence           EQU     0x11
led2_sequence           EQU     0x12
led3_sequence           EQU     0x13
led4_sequence           EQU     0x14
led5_sequence           EQU     0x15
led6_sequence           EQU     0x16
led7_sequence           EQU     0x17
;
;
;
;**************************************************************************************
;Useful EQU constants
;**************************************************************************************
;
;
;
;
;ASCII table
;
character_a             EQU     0x61
character_b             EQU     0x62
character_c             EQU     0x63
character_d             EQU     0x64
character_e             EQU     0x65
character_f             EQU     0x66
character_g             EQU     0x67
character_h             EQU     0x68
character_i             EQU     0x69
character_j             EQU     0x6a
character_k             EQU     0x6b
character_l             EQU     0x6c
character_m             EQU     0x6d
character_n             EQU     0x6e
character_o             EQU     0x6f
character_p             EQU     0x70
character_q             EQU     0x71
character_r             EQU     0x72
character_s             EQU     0x73
character_t             EQU     0x74
character_u             EQU     0x75
character_v             EQU     0x76
character_w             EQU     0x77
character_x             EQU     0x78
character_y             EQU     0x79
character_z             EQU     0x7a
_character_a            EQU     0x41
_character_b            EQU     0x42
_character_c            EQU     0x43
_character_d            EQU     0x44
_character_e            EQU     0x45
_character_f            EQU     0x46
_character_g            EQU     0x47
_character_h            EQU     0x48
_character_i            EQU     0x49
_character_j            EQU     0x4a
_character_k            EQU     0x4b
_character_l            EQU     0x4c
_character_m            EQU     0x4d
_character_n            EQU     0x4e
_character_o            EQU     0x4f
_character_p            EQU     0x50
_character_q            EQU     0x51
_character_r            EQU     0x52
_character_s            EQU     0x53
_character_t            EQU     0x54
_character_u            EQU     0x55
_character_v            EQU     0x56
_character_w            EQU     0x57
_character_x            EQU     0x58
_character_y            EQU     0x59
_character_z            EQU     0x5a
character_0             EQU     0x30
character_1             EQU     0x31
character_2             EQU     0x32
character_3             EQU     0x33
character_4             EQU     0x34
character_5             EQU     0x35
character_6             EQU     0x36
character_7             EQU     0x37
character_8             EQU     0x38
character_9             EQU     0x39
character_colon         EQU     0x3a
character_stop          EQU     0x2e
character_semi_colon    EQU     0x3b
character_minus         EQU     0x2d
character_divide        EQU     0x2f                    ;'/'
character_plus          EQU     0x2b
character_comma         EQU     0x2c
character_less_than     EQU     0x3c
character_greater_than  EQU     0x3e
character_equals        EQU     0x3d
character_space         EQU     0x20
character_cr            EQU     0x0d                    ;carriage return
character_question      EQU     0x3f                    ;'?'
character_dollar        EQU     0x24
character_exclaim       EQU     0x21                    ;'!'
character_bs            EQU     0x08                    ;Back Space command character
;
;
;
;
;
;**************************************************************************************
;Initialise the system
;**************************************************************************************
;
; All PWM channels initialise to off (zero).
; Simple I/O outputs will remain off at all times.
;
cold_start:             LD      s0, #0x00
                        LD      s1, #pwm_channel0
clear_loop:             ST      s0, #s1
                        load     s1, #pwm_channel11
                        JUMP    z, enable_int
                        ADD     s1, #0x01
                        JUMP    clear_loop
;
enable_int:             ENA                             ;interrupts used to drive servo
;
                        CALL    send_welcome            ;Write welcome message to UART
                        CALL    send_ok
;
;
; Initialise LED pattern sequence
;
                        LD      s0, #0x01               ;trigger to start wave pattern
                        ST      s0, led0_sequence
                        LD      s0, #0x00
                        ST      s0, led1_sequence
                        ST      s0, led2_sequence
                        ST      s0, led3_sequence
                        ST      s0, led4_sequence
                        ST      s0, led5_sequence
                        ST      s0, led6_sequence
                        ST      s0, led7_sequence
;
;**************************************************************************************
; Main program
;**************************************************************************************
;
; Provides a pattern of interest on the LEDs :-)
;
; Each LED increases intensity in 8 steps and then decreases intensity in 8 steps until it is off.
; The middle LEDs (LD2 to LD5) each start to turn on when either neighbour is turned half on and increasing
; to provide the effect of a passing a 'wave' of light passing from side to side. The pair of LEDs at each
; (LD0, Ld1 and LD6, LD7) are required to reflect the 'wave' so that the pattern continues.
;
; I'm sure this code cold be written in more elegant way, but I leave that as an exercise to you :-)
;
warm_start:             LD      s2, #0x03               ;simple delay loop (time will be increased by ISR processing)
delay_s2_loop:          LD      s1, #0xff
delay_s1_loop:          LD      s0, #0xff
delay_s0_loop:          SUB     s0, #0x01
                        JUMP    nc, delay_s0_loop
                        SUB     s1, #0x01
                        JUMP    nc, delay_s1_loop
                        SUB     s2, #0x01
                        JUMP    nc, delay_s2_loop
;
;Pattern generation
;
                        FT      s0, led0_sequence       ;read sequence for LED0
                        load     s0, #0x00
                        JUMP    z, load_led0_start
                        SUB     s0, #0x20               ;Count longer to ensure end stops then reset count if maximum
                        JUMP    z, update_led0
                        ADD     s0, #0x20
inc_led0:               ADD     s0, #0x01               ;increment counter
                        JUMP    update_led0
load_led0_start:        FT      s1, led1_sequence       ;start LED0 if LED1 = 4
                        load     s1, #0x04
                        JUMP    z, inc_led0
update_led0:            ST      s0, led0_sequence
                        CALL    led_to_duty
                        ST      s1, pwm_channel0
;
                        FT      s1, led0_sequence       ; refresh LED1 if LED0 = 11 (0B hex) to reflect wave
                        load     s1, #0x0b
                        JUMP    nz, normal_led1
                        LD      s0, #0x04
                        JUMP    update_led1
normal_led1:            FT      s0, led1_sequence       ;read sequence for LED1
                        load     s0, #0x00
                        JUMP    z, load_led1_start
                        SUB     s0, #0x10               ;reset count if maximum
                        JUMP    z, update_led1
                        ADD     s0, #0x10
inc_led1:               ADD     s0, #0x01               ;increment counter
                        JUMP    update_led1
load_led1_start:        FT      s1, led0_sequence       ;start LED1 if LED0 = 11 (0B hex) to reflect wave
                        load     s1, #0x0b
                        JUMP    z, inc_led1
                        FT      s1, led2_sequence       ;start LED1 if LED2 = 4
                        load     s1, #0x04
                        JUMP    z, inc_led1
update_led1:            ST      s0, led1_sequence
                        CALL    led_to_duty
                        ST      s1, pwm_channel1
;
                        FT      s0, led2_sequence       ;read sequence for LED2
                        load     s0, #0x00
                        JUMP    z, load_led2_start
                        SUB     s0, #0x10               ;reset count if maximum
                        JUMP    z, update_led2
                        ADD     s0, #0x10
inc_led2:               ADD     s0, #0x01               ;increment counter
                        JUMP    update_led2
load_led2_start:        FT      s1, led1_sequence       ;start LED2 if LED1 = 4
                        load     s1, #0x04
                        JUMP    z, inc_led2
                        FT      s1, led3_sequence       ;start LED2 if LED3 = 4
                        load     s1, #0x04
                        JUMP    z, inc_led2
update_led2:            ST      s0, led2_sequence
                        CALL    led_to_duty
                        ST      s1, pwm_channel2
;
;
                        FT      s0, led3_sequence       ;read sequence for LED3
                        load     s0, #0x00
                        JUMP    z, load_led3_start
                        SUB     s0, #0x10               ;reset count if maximum
                        JUMP    z, update_led3
                        ADD     s0, #0x10
inc_led3:               ADD     s0, #0x01               ;increment counter
                        JUMP    update_led3
load_led3_start:        FT      s1, led2_sequence       ;start LED3 if LED2 = 4
                        load     s1, #0x04
                        JUMP    z, inc_led3
                        FT      s1, led4_sequence       ;start LED3 if LED4 = 4
                        load     s1, #0x04
                        JUMP    z, inc_led3
update_led3:            ST      s0, led3_sequence
                        CALL    led_to_duty
                        ST      s1, pwm_channel3
;
                        FT      s0, led4_sequence       ;read sequence for LED4
                        load     s0, #0x00
                        JUMP    z, load_led4_start
                        SUB     s0, #0x10               ;reset count if maximum
                        JUMP    z, update_led4
                        ADD     s0, #0x10
inc_led4:               ADD     s0, #0x01               ;increment counter
                        JUMP    update_led4
load_led4_start:        FT      s1, led3_sequence       ;start LED4 if LED3 = 4
                        load     s1, #0x04
                        JUMP    z, inc_led4
                        FT      s1, led5_sequence       ;start LED4 if LED5 = 4
                        load     s1, #0x04
                        JUMP    z, inc_led4
update_led4:            ST      s0, led4_sequence
                        CALL    led_to_duty
                        ST      s1, pwm_channel4
;
                        FT      s0, led5_sequence       ;read sequence for LED5
                        load     s0, #0x00
                        JUMP    z, load_led5_start
                        SUB     s0, #0x10               ;reset count if maximum
                        JUMP    z, update_led5
                        ADD     s0, #0x10
inc_led5:               ADD     s0, #0x01               ;increment counter
                        JUMP    update_led5
load_led5_start:        FT      s1, led4_sequence       ;start LED5 if LED4 = 4
                        load     s1, #0x04
                        JUMP    z, inc_led5
                        FT      s1, led6_sequence       ;start LED5 if LED6 = 4
                        load     s1, #0x04
                        JUMP    z, inc_led5
update_led5:            ST      s0, led5_sequence
                        CALL    led_to_duty
                        ST      s1, pwm_channel5
;
                        FT      s1, led7_sequence       ; refresh LED6 if LED7 = 11 (0B hex) to reflect wave
                        load     s1, #0x0b
                        JUMP    nz, normal_led6
                        LD      s0, #0x04
                        JUMP    update_led6
normal_led6:            FT      s0, led6_sequence       ;read sequence for LED6
                        load     s0, #0x00
                        JUMP    z, load_led6_start
                        SUB     s0, #0x10               ;reset count if maximum
                        JUMP    z, update_led6
                        ADD     s0, #0x10
inc_led6:               ADD     s0, #0x01               ;increment counter
                        JUMP    update_led6
load_led6_start:        FT      s1, led5_sequence       ;start LED6 if LED5 = 4
                        load     s1, #0x04
                        JUMP    z, inc_led6
update_led6:            ST      s0, led6_sequence
                        CALL    led_to_duty
                        ST      s1, pwm_channel6
;
                        FT      s0, led7_sequence       ;read sequence for LED7
                        load     s0, #0x00
                        JUMP    z, load_led7_start
                        SUB     s0, #0x20               ;Count longer to ensure end stops then reset count if maximum
                        JUMP    z, update_led7
                        ADD     s0, #0x20
inc_led7:               ADD     s0, #0x01               ;increment counter
                        JUMP    update_led7
load_led7_start:        FT      s1, led6_sequence       ;start LED7 if LED6 = 4
                        load     s1, #0x04
                        JUMP    z, inc_led7
update_led7:            ST      s0, led7_sequence
                        CALL    led_to_duty
                        ST      s1, pwm_channel7
                        JUMP    warm_start
;
;
; Convert LED sequence number into PWM intensity figure
;
; LEDs duty cycle values are 0,1,2,4,8,16,32 and 64 because they appear to give what
; appears to be a fairly liner change in intensity and provides a simple way to set
; the duty value.
;
; Provide sequence value in register s0 and intensity will be
; returned in register s1.
;
; s0   s1
; 00   00
; 01   01
; 02   02
; 03   04
; 04   08
; 05   10
; 06   20
; 07   40
; 08   80
; 09   40
; 0A   20
; 0B   10
; 0C   08
; 0D   04
; 0E   02
; 0F   01
; 10   00  and zero for all larger values of s0
;
led_to_duty:            LD      s1, #0x00
                        load     s0, #0x00               ;load for zero
                        RET
                        LD      s1, #0x01               ;inject '1'
go_up_loop:             SUB     s0, #0x01
                        RET
                        SL0     s1                      ;multiply by 2
                        JUMP    c, go_down
                        JUMP    go_up_loop
go_down:                LD      s1, #0x40
go_down_loop:           SUB     s0, #0x01
                        RET
                        SR0     s1                      ;divide by 2
                        JUMP    go_down_loop
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
read_from_uart:         IN      s0, status_port         ;load Rx_FIFO buffer
                        load    s0, #rx_EQU_present    ;wait if empty
                        JUMP    nz, read_character
                        JUMP    read_from_uart
read_character:         IN      uart_EQU, uart_read_port ;read from FIFO
                        RET
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
send_to_uart:           IN      s0, status_port         ;load Tx_FIFO buffer
                        load    s0, #tx_full            ;wait if full
                        JUMP    z, uart_write
                        JUMP    send_to_uart
uart_write:             OUT     uart_EQU, uart_write_port
                        RET
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
send_cr:                LD      uart_EQU, #character_cr
                        CALL    send_to_uart
                        RET
;
; Send a space to the UART
;
send_space:             LD      uart_EQU, #character_space
                        CALL    send_to_uart
                        RET
;
;
;
; Send 'PicoBlaze Servo Control' string to the UART
;
send_welcome:           CALL    send_cr
                        CALL    send_cr
                        LD      uart_EQU, #_character_p
                        CALL    send_to_uart
                        LD      uart_EQU, #character_i
                        CALL    send_to_uart
                        LD      uart_EQU, #character_c
                        CALL    send_to_uart
                        LD      uart_EQU, #character_o
                        CALL    send_to_uart
                        LD      uart_EQU, #_character_b
                        CALL    send_to_uart
                        LD      uart_EQU, #character_l
                        CALL    send_to_uart
                        LD      uart_EQU, #character_a
                        CALL    send_to_uart
                        LD      uart_EQU, #character_z
                        CALL    send_to_uart
                        LD      uart_EQU, #character_e
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_EQU, #_character_a
                        CALL    send_to_uart
                        LD      uart_EQU, #character_u
                        CALL    send_to_uart
                        LD      uart_EQU, #character_t
                        CALL    send_to_uart
                        LD      uart_EQU, #character_o
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_EQU, #_character_p
                        CALL    send_to_uart
                        LD      uart_EQU, #_character_w
                        CALL    send_to_uart
                        LD      uart_EQU, #_character_m
                        CALL    send_to_uart
                        CALL    send_space
                        LD      uart_EQU, #_character_a
                        CALL    send_to_uart
                        LD      uart_EQU, #character_c
                        CALL    send_to_uart
                        LD      uart_EQU, #character_t
                        CALL    send_to_uart
                        LD      uart_EQU, #character_i
                        CALL    send_to_uart
                        LD      uart_EQU, #character_v
                        CALL    send_to_uart
                        LD      uart_EQU, #character_e
                        CALL    send_to_uart
                        CALL    send_cr
                        CALL    send_cr
                        RET
;
;
;Send 'OK' to the UART
;
send_ok:                CALL    send_cr
                        LD      uart_EQU, #_character_o
                        CALL    send_to_uart
                        LD      uart_EQU, #_character_k
                        CALL    send_to_uart
                        JUMP    send_cr
;
;
;**************************************************************************************
; Interrupt Service Routine (ISR)
;**************************************************************************************
;
; Interrupts occur at 3.92us intervals and are used to generate the PWM pulses generated
; at a PRF of 1KHz. The 3.92us interrupt rate corresponds with a resolution of 256 steps
; over the 1ms associated with the 1KHz PRF.
;
; The ISR is self contained and all registers used are preserved. Scratch pad memory
; locations are used to determine the desired duty factor for each of 12 channels.
;
; Note that an interrupt is generated every 196 clock cycles. This means that there is
; only time to execute 98 instructions between each interrupt. This ISR is 48 instructions
; long. A further 3 instructions are also consumed by the interrupt process
; (abandoned instruction, virtual CALL to 3FF and the interrupt vector JUMP) and hence
; PicoBlaze has approximately half of its time available for other tasks in the main program.
;
; Although a loop would normal be employed in software to process each of 12 channels,
; the implementation of a loop would increase the number of instructions which needed to
; be executed to such an extent that this 12 channel implementation would not be possible.
; Consequently the code is written out in a linear fashion which consumes more program
; space but which executes faster.
;
isr:                    ST      s0, isr_preserve_s0     ;preserve registers to be used
                        ST      s1, isr_preserve_s1
                        ST      s2, isr_preserve_s2
;Determine the number of steps currently through the 1ms PWM cycle
                        FT      s1, pwm_duty_counter    ;read 8-bit counter of steps
                        ADD     s1, #0x01               ;increment counter (will roll over to zero)
                        ST      s1, pwm_duty_counter    ;update count value in memory for next interrupt.
;Read duty factor for each channel and load it with the duty counter and set or
;reset a bit in register s2 accordingly.
                        FT      s0, pwm_channel11       ;read desired setting of pulse width
                        load     s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA     s2                      ;shift carry into register s2
                        FT      s0, pwm_channel10       ;read desired setting of pulse width
                        load     s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA     s2                      ;shift carry into register s2
                        FT      s0, pwm_channel9        ;read desired setting of pulse width
                        load     s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA     s2                      ;shift carry into register s2
                        FT      s0, pwm_channel8        ;read desired setting of pulse width
                        load     s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA     s2                      ;shift carry into register s2
                        OUT     s2, simple_port         ;drive pins on connector J4
                        FT      s0, pwm_channel7        ;read desired setting of pulse width
                        load     s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA     s2                      ;shift carry into register s2
                        FT      s0, pwm_channel6        ;read desired setting of pulse width
                        load     s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA     s2                      ;shift carry into register s2
                        FT      s0, pwm_channel5        ;read desired setting of pulse width
                        load     s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA     s2                      ;shift carry into register s2
                        FT      s0, pwm_channel4        ;read desired setting of pulse width
                        load     s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA     s2                      ;shift carry into register s2
                        FT      s0, pwm_channel3        ;read desired setting of pulse width
                        load     s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA     s2                      ;shift carry into register s2
                        FT      s0, pwm_channel2        ;read desired setting of pulse width
                        load     s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA     s2                      ;shift carry into register s2
                        FT      s0, pwm_channel1        ;read desired setting of pulse width
                        load     s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA     s2                      ;shift carry into register s2
                        FT      s0, pwm_channel0        ;read desired setting of pulse width
                        load     s1, s0                  ;set carry flag if duty factor > duty counter
                        SLA     s2                      ;shift carry into register s2
                        OUT     s2, led_port            ;drive LEDs
                        FT      s0, isr_preserve_s0     ;reload register values
                        FT      s1, isr_preserve_s1
                        FT      s2, isr_preserve_s2
                        RETIE
;
;
;**************************************************************************************
; Interrupt Vector
;**************************************************************************************
;
                        ORG     0x3ff
                        JUMP    isr
;
;

