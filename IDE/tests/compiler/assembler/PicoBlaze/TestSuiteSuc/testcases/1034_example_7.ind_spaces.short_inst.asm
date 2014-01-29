;KCPSM3 Program - SPI Control of D/A converter on Spartan-3E Starter Kit.
device kcpsm1
;
;Ken Chapman - Xilinx Ltd
;
;Version v1.00 - 24th November 2005
;
;This program uses an 8KHz interrupt to generate load waveforms on the
;4 analogue outputs provided by the Linear Technology LTC2624 device.
;
;As well as the port connections vital to communication with the UART and the SPI
;FLASH memory, there are additional port connections used to disable the other
;devices sharing the SPI bus on the Starter Kit board. Although these could have been
;controlled at the hardware level, they are included in this code to aid
;future investigations of communication with the other SPI devices using PicoBlaze.
;
;Connections to the LEDs, switches and press buttons are provided to aid
;development and enable further experiments. Otherwise know as having fun!
;
;Port definitions
;
;
spi_control_port        EQU     0x08                    ;SPI clock and chip selects
spi_sck                 EQU     0x01                    ;                  SCK - bit0
spi_rom_cs              EQU     0x02                    ;    serial rom select - bit1
spi_spare_control       EQU     0x04                    ;                spare - bit2
spi_amp_cs              EQU     0x08                    ;     amplifier select - bit3
spi_adc_conv            EQU     0x10                    ;          A/D convert - bit4
spi_dac_cs              EQU     0x20                    ;           D/A select - bit5
spi_amp_shdn            EQU     0x40                    ;       amplifier SHDN - bit6
spi_dac_clr             EQU     0x80                    ;            D/A clear - bit7
;
spi_output_port         EQU     0x04                    ;SPI EQU output
spi_sdo                 EQU     0x80                    ;   SDO - bit7
;
spi_input_port          EQU     0x01                    ;SPI EQU input
spi_sdi                 EQU     0x80                    ;             SDI - bit7
spi_amp_sdi             EQU     0x40                    ;   amplifier SDI - bit6
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
switch_port             EQU     0x00                    ;Read switches and press buttons
btn_north               EQU     0x01                    ;  Buttons     North - bit0
btn_east                EQU     0x02                    ;               East - bit1
btn_south               EQU     0x04                    ;              South - bit2
btn_west                EQU     0x08                    ;               West - bit3
switch0                 EQU     0x10                    ;  Switches        0 - bit4
switch1                 EQU     0x20                    ;                  1 - bit5
switch2                 EQU     0x40                    ;                  2 - bit6
switch3                 EQU     0x80                    ;                  3 - bit7
;
;
;
;
;Special Register usage
;
;
;Useful EQU constants
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
delay_1us_constant      EQU     0x0b
;
;
;
;
;
;
;Scratch Pad Memory Locations
;
;Values to be written to the D/A converter
;
;
chan_a_lsb              EQU     0x00                    ;Channel A value LS-Byte
chan_a_msb              EQU     0x01                    ;                MS-Byte
;
chan_b_lsb              EQU     0x02                    ;Channel B value LS-Byte
chan_b_msb              EQU     0x03                    ;                MS-Byte
;
chan_c_lsb              EQU     0x04                    ;Channel C value LS-Byte
chan_c_msb              EQU     0x05                    ;                MS-Byte
;
chan_d_lsb              EQU     0x06                    ;Channel D value LS-Byte
chan_d_msb              EQU     0x07                    ;                MS-Byte
;
;
;Value used to synthesise a triangle wave
;
triangle_up_down        EQU     0x08                    ;Determines up or down slope
;
;Value used to synthesise a square wave
;
square_count            EQU     0x09                    ;Counts samples in square wave
;
;
;Values used to synthesise a sine wave
;
sine_y_lsb              EQU     0x10                    ;Sine wave value LS-Byte
sine_y_msb              EQU     0x11                    ;                 MS-Byte
sine_y1_lsb             EQU     0x12                    ;Sine wave delayed LS-Byte
sine_y1_msb             EQU     0x13                    ;                  MS-Byte
sine_k_lsb              EQU     0x14                    ;Sine constant LS-Byte
sine_k_msb              EQU     0x15                    ;              MS-Byte
;
;
;Sample counter used to give activity indication on LEDs
;
sample_count_lsb        EQU     0x20                    ;16-bit counter LS-Byte
sample_count_msb        EQU     0x21                    ;               MS-Byte
;
;Initialise the system
;
;
cold_start:             CALL    spi_init                ;initialise SPI bus ports
                        CALL    init_sine_wave          ;initialise sine wave synthesis values
                        CALL    delay_1s                ;bus settling delay
                        LD      s0, #0x00               ;clear all internal D/A values
                        ST      s0, chan_a_lsb
                        ST      s0, chan_a_msb
                        ST      s0, chan_b_lsb
                        ST      s0, chan_b_msb
                        ST      s0, chan_c_lsb
                        ST      s0, chan_c_msb
                        ST      s0, chan_d_lsb
                        ST      s0, chan_d_msb
                        ST      s0, triangle_up_down    ;initial slope is up
                        CALL    dac_reset               ;reset D/A converter on all channels
                        ENA                             ;Interrupts define 8KHz sample rate
;
;
;The program is interrupt driven to maintain an 8KHz sample rate. The main body
;of the program waits for an interrupt to occur. The interrupt updates all four
;analogue outputs with values loadd in scratch pad memory. This takes approximately
;58us of the 125us available between interrupts. The main program then prepares
;new values for the analogue outputs (in less than 67us) before waiting for the
;next interrupt.
;
;
warm_start:             LD      sf, #0xff               ;flag set and wait for interrupt to be serviced
wait_int:               load     sf, #0xff
                        JUMP    z, wait_int             ;interrupt clears the flag
;
;
;Channel A is a square wave of 2KHz.
;
;This is formed from the 2KHz square wave on channel C and demonstrates that the
;D/A converter echoes the previously sent 32-bit command word.
;
;Following the interrupt service routine (ISR), the register set [s9,s8,s7,s6]
;will contain the command which was last sent for the setting of channel C. The
;12-bit sample value is extracted from this word and loadd in the location for
;channel A. This should mean that channel A is one sample behind channel C. In this
;version that does not mean a lag of 90 degrees because each output is updated
;sequentially and that takes approximatly 14.5us per channel.
;
;This will also demonstrate that the reference voltage on channels A and B is 3.3v
;loadd with 2.5v on channels C and D. So whilst the square wave on channel C is
;set for 0.50v to 2.00v, it should be 0.66v to 2.64v on channel A.
;
                        SR0     s7                      ; shiload 12-bit value right 4 places
                        SRA     s6
                        SR0     s7
                        SRA     s6
                        SR0     s7
                        SRA     s6
                        SR0     s7
                        SRA     s6
                        ST      s7, chan_a_msb          ;load value for D/A output
                        ST      s6, chan_a_lsb
;
;
;
;
;Channel B is a triangle waveform of 200Hz.
;
;Given the sample rate of 8KHz, there are 40 samples per waveform period.
;To achieve close to full scale deflection, the waveform needs to increase or
;decrease by 204 each sample so that over the first 20 samples it rises from
;0 to 4080 and then over the next 20 samples it reduces back to zero.
;
                        load      s0, chan_b_lsb          ;load current value into [s1,s0]
                        load      s1, chan_b_msb
                        load      s2, triangle_up_down    ;read current slope direction
                        load     s2, #0x00               ;determine current direction
                        JUMP    nz, slope_down
                        ADD     s0, #0xcc               ;add 204 (00CC hex) to current value
                        ADDCY   s1, #0x00
                        load     s1, #0x0f               ;load for peak value of 4080 (0FF0 hex)
                        JUMP    nz, load_channel_b
                        load     s0, #0xf0
                        JUMP    nz, load_channel_b
                        LD      s2, #0x01               ;change to slope down next time
                        ST      s2, triangle_up_down
                        JUMP    load_channel_b
slope_down:             SUB     s0, #0xcc               ;subtract 204 (00CC hex) from current value
                        SUBCY   s1, #0x00
                        load     s1, #0x00               ;load for zero (0000 hex)
                        JUMP    nz, load_channel_b
                        load     s0, #0x00
                        JUMP    nz, load_channel_b
                        LD      s2, #0x00               ;change to slope up next time
                        ST      s2, triangle_up_down
load_channel_b:        ST      s0, chan_b_lsb          ;load value for D/A output
                        ST      s1, chan_b_msb
;
;
;Channel C is a square wave of 2KHz.
;
;Since the sample rate is 8KHz, this square wave is formed of two samples at a
;low level and two samples at a high level. This is used to demonstrate when the
;D/A converter output actually changes and how to determine the voltage levels.
;It is also used indirectly to form the signal for channel A.
;
;The low level voltage is 0.50v.
;   The 12-bit value is therefore 4096 x 0.5 / 2.5 = 819 (333 hex)
;
;The high level voltage is 2.00v.
;   The 12-bit value is therefore 4096 x 2.0 / 2.5 = 3277 (CCD hex)
;
;
                        load      s2, square_count        ;read sample counter
                        load    s2, #0x02               ;bit 1 has correct frequency
                        JUMP    nz, square_high
                        LD      s1, #0x03               ;Set low level
                        LD      s0, #0x33
                        JUMP    load_channel_c
square_high:            LD      s1, #0x0c               ;Set high level
                        LD      s0, #0xcd
load_channel_c:        ST      s0, chan_c_lsb          ;load value for D/A output
                        ST      s1, chan_c_msb
                        ADD     s2, #0x01               ;increment sampel count
                        ST      s2, square_count        ;load new sample count
;
;Sine wave for channel D
;
;A synthesis algorithm is used to generate a stable 770Hz sine wave
;which is one of the 8 tines used in DTMF telephone dialing.
;
                        CALL    calc_next_sine
                        SR0     s9                      ;reduce value to 12-bits
                        SRA     s8
                        SR0     s9
                        SRA     s8
                        SR0     s9
                        SRA     s8
                        ADD     s9, #0x08               ;Scale signed number to mid-rail of unsigned output
                        ST      s9, chan_d_msb          ;load value for D/A output
                        ST      s8, chan_d_lsb
;
;
;Drive LEDs with simple binary count of the samples to indicate
;that the design is active.
;
                        load      s0, sample_count_lsb    ;read sample counter
                        load      s1, sample_count_msb
                        ADD     s0, #0x01               ;increment counter
                        ADDCY   s1, #0x00
                        ST      s0, sample_count_lsb    ;load new value
                        ST      s1, sample_count_msb
                        OUT     s1, led_port            ;upper bits are 31.25Hz and lower
;
                        JUMP    warm_start              ;wait for next interrupt
;
;**************************************************************************************
;Sine wave synthesis algorithm
;**************************************************************************************
;
;This example is set to generate 770Hz at a sample rate of 8KHz. 770Hz is one of
;the eight DTMF frequences. Please see design documentation for more details.
;
init_sine_wave:         LD      s0, #0x24               ;initial value 9216 (2400 hex)
                        ST      s0, sine_y_msb
                        LD      s0, #0x00
                        ST      s0, sine_y_lsb
                        LD      s0, #0x00               ;initial delayed value 0 (0000 hex)
                        ST      s0, sine_y1_msb
                        ST      s0, sine_y1_lsb
                        LD      s0, #0xd2               ;Coefficient for 770Hz is UFIX_16_15 value 53913/32768 = 1.64529
                        ST      s0, sine_k_msb
                        LD      s0, #0x99
                        ST      s0, sine_k_lsb
                        RET
;
;
;Calculate a new output sample for a single tone.
;
;The tone sample is generated as a 16-bit signed integer.
;The waveform is virtually full scale deflection for a 15-bit integer
;such that the addition of two tones for DTMF will not exceed the 16-bits
;provided by two registers.
;
;Obtain current values from wscratch pad memory
;
calc_next_sine:         load      sf, sine_y_msb          ;[sF,sE] is Y
                        load      se, sine_y_lsb
                        load      sd, sine_y1_msb         ;[sD,sC] is Y1
                        load      sc, sine_y1_lsb
                        load      sb, sine_k_msb          ;[sB,sA] is K
                        load      sa, sine_k_lsb
;
;16-bit signed by 16-bit unsigned multiplication. [s9,s8]=[sB,sA]x[sF,sE]
;
;The unsigned number is of format UFIX_16_15 resulting
;in a FIX_32_15 product. Since only the integer part of the
;product is to be retained as a 16-bit value, their is no
;shiload of the result on the last cycle of the multiplication.
;Execution requires a maximum of 145 instructions.
;
                        LD      s9, #0x00               ;clear temporary result registers [s9,s8]
                        LD      s8, #0x00
                        LD      s0, #0x10               ;16 bit multiply
mult_loop:              SRX     s9                      ;signed divide result by 2
                        SRA     s8
                        SR0     sb                      ;shiload coefficient
                        SRA     sa
                        JUMP    nc, no_mult_add         ;load for active bit
                        ADD     s8, se                  ;16-bit signed addition
                        ADDCY   s9, sf
no_mult_add:            SUB     s0, #0x01               ;load for 16 cycles
                        JUMP    nz, mult_loop
;
;Subtract of delayed sample
;
                        SUB     s8, sc                  ;16-bit signed subtract
                        SUBCY   s9, sd
;
;Update scratch pad memory with new sample values
;
                        ST      sf, sine_y1_msb         ;delayed sample gets previous output
                        ST      se, sine_y1_lsb
                        ST      s9, sine_y_msb          ;new current sample
                        ST      s8, sine_y_lsb
                        RET
;
;
;**************************************************************************************
;SPI communication routines for D/A Converter
;**************************************************************************************
;
;These routines will work with two output ports and one input port which should be
;defined as follows using CONSTANT directives.
;   (replace 'pp' with appropriate port address in each case)
;In the list of CONSTANT directives, only the ones marked with a * are really required
;for the D/A Converter system. The other directives are to control (disable) or
;communicate with the other SPI components on the same SPI bus of the Spartan-3E Starter Kit.
;
;
;
;CONSTANT SPI_control_port, pp       ;SPI clock and chip selects     *
;CONSTANT SPI_sck, 01                ;                  SCK - bit0   *
;CONSTANT SPI_rom_cs, 02             ;    serial rom select - bit1
;CONSTANT SPI_spare_control, 04      ;                spare - bit2
;CONSTANT SPI_amp_cs, 08             ;     amplifier select - bit3
;CONSTANT SPI_adc_conv, 10           ;          A/D convert - bit4
;CONSTANT SPI_dac_cs, 20             ;           D/A select - bit5   *
;CONSTANT SPI_amp_shdn, 40           ;       amplifier SHDN - bit6
;CONSTANT SPI_dac_clr, 80            ;            D/A clear - bit7   *
;
;CONSTANT SPI_output_port, pp        ;SPI EQU output                *
;CONSTANT SPI_sdo, 80                ;   SDO - bit7                  *
;
;CONSTANT SPI_input_port, pp         ;SPI EQU input                 *
;CONSTANT SPI_sdi, 80                ;             SDI - bit7        *
;CONSTANT SPI_amp_sdi, 40            ;   amplifier SDI - bit6
;
;
;
;
;Initialise SPI bus
;
;This routine should be used to initialise the SPI bus.
;The SCK clock is made low.
;Device selections are made inactive as follows
;   SPI_sck      = 0      Clock is Low (required)
;   SPI_rom_cs   = 1      Deselect ROM
;   spare        = 1      spare control bit
;   SPI_amp_cs   = 1      Deselect amplifier
;   SPI_adc_conv = 0      A/D convert ready to apply positive pulse
;   SPI_dac_cs   = 1      Deselect D/A
;   SPI_amp_shdn = 0      Amplifier active and available
;   SPI_dac_clr  = 1      D/A clear off
;
spi_init:               LD      s0, #0xae               ;normally AE
                        OUT     s0, spi_control_port
                        RET
;
;
;
;Send and receive one byte to and from the SPI D/A converter.
;
;The EQU supplied in register 's2' is transmitted to the SPI bus and
;at the same time the received byte is used to replace the value in 's2'.
;The SCK clock is generated by soloadware and results in a communication rate of
;2.5Mbit/s with a 50MHz clock.
;
;Note that you must have previously selected the required device on the bus
;before attempting communication and you must subsequently deselect the device
;when appropriate.
;
;Entry to this routine assumes that register s0 defines the state of the SPI
;control signals including SCK which should be Low. The easiest way to achieve this is
;to use the SPI_init routine before calling this one for the first time.
;
;As a 'master' the signal sequence is as follows..
;   Transmit EQU bit on SDO line
;   Drive SCK transition from low to high
;   Receive EQU bit from SDI line (D/A transmits on previous falling edge)
;   Drive SCK transition from high to low.
;
;Important note
;   The received EQU bit must be captured some time before SCK goes low.
;   However the combination of relatively slow clock to output time of the
;   LTC2624 combined with the low drive strength of its SDO output means that
;   the received bit needs maximum time to settle. Therefore this routine
;   schedules the read as late as it can.
;
spi_dac_tx_rx:          LD      s1, #0x08               ;8-bits to transmit and receive
next_spi_dac_bit:       OUT     s2, spi_output_port     ;output EQU bit ready to be used on rising edge
                        XOR     s0, #spi_sck            ;clock High (bit0)
                        OUT     s0, spi_control_port    ;drive clock High
                        XOR     s0, #spi_sck            ;prepare clock Low (bit0)
                        IN      s3, spi_input_port      ;read input bit
                        load    s3, #spi_sdi            ;detect state of received bit
                        SLA     s2                      ;shiload new EQU into result and move to next transmit bit
                        OUT     s0, spi_control_port    ;drive clock Low
                        SUB     s1, #0x01               ;count bits
                        JUMP    nz, next_spi_dac_bit    ;repeat until finished
                        RET
;
;
;
;Set a voltage on one of the LTC2624 D/A converter outputs
;
;The D/A converter has 4 channels. Specify which channel is to be set using
;register sC as follows....
;   sC     Channel                 Nominal Voltage Range
;   00        A                       0 to 3.30v (or VREFAB)
;   01        B                       0 to 3.30v (or VREFAB)
;   02        C                       0 to 2.50v (or VREFCD)
;   03        D                       0 to 2.50v (or VREFCD)
;   0F        All channels            various as above.
;
;The analogue level is a 12-bit value to be supplied in lower 12-bits of register
;pair [sB,sA]. If this value is called 'k' and is in the range 0 to 4095 (000 to FFF)
;then
;      Vout = (k/4096) * VREFx
;Hence it is not possible to reach the absolute level of the reference.
;
;Here are some useful values..
;    Voltage    A or B    C or D
;      0.0       000       000
;      0.5       26D       333
;      0.65      327               A/D reference -1.00v
;      1.0       4D9       666
;      1.5       746       99A
;      1.65      800       A8F     converter reference = 3.3/2 = 1.65v
;      2.0       9B2       CCD
;      2.5       C1F       FFF
;      2.65      CD9               A/D reference +1.00v
;      3.0       E8C       n/a
;      3.3       FFF       n/a
;
;Note that the full scale deflection of FFF will result in different output
;voltages due to different reference voltages for each pair of channels.
;
;SPI communication with the DAC only requires a 24-bit word to be transmitted.
;However, the device internally contains a 32-bit shiload register. When writing
;a command word, the previous contents are shiloaded out and can be observed by
;the master (Spartan-3E in this case). If you do not use a 32-bit format, then
;the read back is confusing. Hence this routine uses a 32-bit format by transmitting
;a dummy byte first.
;
;  Byte 1 = 00   8 dummy bits
;  Byte 2 = 3c   Command nibble (3=write and update) and channel selection
;  Byte 3 = dd   Upper 8-bits of the 12-bit voltage value
;  Byte 4 = d0   lower 4-bits of the 12-bit voltage value and 4 dummy bits.
;
;At the end of this communication, the register set [s9,s8,s7,s6] will contain the
;EQU received back from the D/A converter which should be the previous command.
;
set_dac:                CALL    spi_init                ;ensure known state of bus and s0 register
                        XOR     s0, #spi_dac_cs         ;select low on D/A converter
                        OUT     s0, spi_control_port
                        LD      s2, #0x00               ;Write dummy byte to DAC
                        CALL    spi_dac_tx_rx
                        LD      s9, s2                  ;capture response
                        LD      s2, sc                  ;Select channel for update
                        AND     s2, #0x0f               ;isolate channel bits to be certain of correct command
                        OR      s2, #0x30               ;Use immediate Write and Update command is "0011"
                        CALL    spi_dac_tx_rx
                        LD      s8, s2                  ;capture response
                        SL0     sa                      ;EQU shiload bits into correct position
                        SLA     sb                      ;with 4 dummy bits ('0') in the least significant bits.
                        SL0     sa
                        SLA     sb
                        SL0     sa
                        SLA     sb
                        SL0     sa
                        SLA     sb
                        LD      s2, sb                  ;Write 12 bit value followed by 4 dummy bits
                        CALL    spi_dac_tx_rx
                        LD      s7, s2                  ;capture response
                        LD      s2, sa
                        CALL    spi_dac_tx_rx
                        LD      s6, s2                  ;capture response
                        XOR     s0, #spi_dac_cs         ;deselect the D/A converter to execute
                        OUT     s0, spi_control_port
                        RET
;
;Perform a hard reset of the D/A converter
;
dac_reset:              CALL    spi_init                ;ensure known state of bus and s0 register
                        XOR     s0, #spi_dac_clr        ;pulse the clear signal.
                        OUT     s0, spi_control_port
                        XOR     s0, #spi_dac_clr
                        OUT     s0, spi_control_port
                        RET
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
delay_1us:              LD      s0, #delay_1us_constant
wait_1us:               SUB     s0, #0x01
                        JUMP    nz, wait_1us
                        RET
;
;Delay of 40us.
;
;Registers used s0, s1
;
delay_40us:             LD      s1, #0x28               ;40 x 1us = 40us
wait_40us:              CALL    delay_1us
                        SUB     s1, #0x01
                        JUMP    nz, wait_40us
                        RET
;
;
;Delay of 1ms.
;
;Registers used s0, s1, s2
;
delay_1ms:              LD      s2, #0x19               ;25 x 40us = 1ms
wait_1ms:               CALL    delay_40us
                        SUB     s2, #0x01
                        JUMP    nz, wait_1ms
                        RET
;
;Delay of 20ms.
;
;Delay of 20ms used during initialisation.
;
;Registers used s0, s1, s2, s3
;
delay_20ms:             LD      s3, #0x14               ;20 x 1ms = 20ms
wait_20ms:              CALL    delay_1ms
                        SUB     s3, #0x01
                        JUMP    nz, wait_20ms
                        RET
;
;Delay of approximately 1 second.
;
;Registers used s0, s1, s2, s3, s4
;
delay_1s:               LD      s4, #0x14               ;50 x 20ms = 1000ms
wait_1s:                CALL    delay_20ms
                        SUB     s4, #0x01
                        JUMP    nz, wait_1s
                        RET
;
;
;
;**************************************************************************************
;Interrupt Service Routine (ISR)
;**************************************************************************************
;
;Interrupts occur at a rate of 8KHz.
;
;Each interrupt is the fundamental timing trigger used to set the sample rate and
;it is therefore use to set the D/A outputs by copying the values loadd in
;scratch pad memory and outputting them to the D/A converter using the SPI bus.
;
;Because the SPI communication is in itself a predictable process, the sample rate
;is preserved without sample jitter. All variable activities are leload to the main
;program.
;
;Each time PicoBlaze transmits a 32-bit command word to the D/A converter, the
;D/A responds with the last command it was sent. So as the end of this service routine
;the register set [s9,s8,s7,s6] will contain the command which has just been sent
;for the setting of channel C.
;
;Set channel A
;
isr:                    LD      sc, #0x00               ;channel A
                        load      sb, chan_a_msb          ;12-bit value
                        load      sa, chan_a_lsb
                        CALL    set_dac
;
;Set channel B
;
                        LD      sc, #0x01               ;channel B
                        load      sb, chan_b_msb          ;12-bit value
                        load      sa, chan_b_lsb
                        CALL    set_dac
;
;Set channel C
;
                        LD      sc, #0x02               ;channel C
                        load      sb, chan_c_msb          ;12-bit value
                        load      sa, chan_c_lsb
                        CALL    set_dac
;
;Set channel A
;
                        LD      sc, #0x03               ;channel D
                        load      sb, chan_d_msb          ;12-bit value
                        load      sa, chan_d_lsb
                        CALL    set_dac
;
                        LD      sf, #0x00               ;clear flag
                        RETIE
;
;
;**************************************************************************************
;Interrupt Vector
;**************************************************************************************
;
                        ORG     0x3ff
                        JUMP    isr
;
;

