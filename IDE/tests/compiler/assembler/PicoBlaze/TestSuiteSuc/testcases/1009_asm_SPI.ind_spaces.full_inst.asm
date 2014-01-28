device kcpsm1

BSDA_Z  EQU     5
BSCL_Z  EQU     5
BSDA_0  EQU     5
BSCL_0  EQU     5
BSDA_IN EQU     5
; ---\              /--------\                                            /--
;     \  SDA       /          \                                          /
;      \----------/ \--------/ \----------------------------------------/
; -------\            /----\                                         /-------
;         \  SCL     /      \                                       /
;          \--------/        \-------------------------------------/
; | Start    |      Data bit     |                             |  Stop     |

init_i2c:               OUTPUT          sf, bsda_z              ; SDA = Z
                        OUTPUT          sf, bscl_z              ; SCL = Z
                        JUMP            delay

delay:
; -------------------------------------------------------------------------------
; Routine to set up for and read four bytes from I2C device
; -------------------------------------------------------------------------------

readwrite4_i2c:
                        CALL            i2c_start               ; Send Start, control byte and ack
                        INPUT           sc, s0                 ; Load device code for TX
                        SL0             sc                      ; add write flag
                        CALL            i2c_xmit                ; Send address and ack
                        CALL            i2c_sack

                        INPUT           sc, s1                 ; Load data for TX
                        CALL            i2c_xmit                ; Send data and ack
                        CALL            i2c_sack

                        CALL            i2c_repstart            ; Send repeated start, control byte and ack
                        INPUT           sc, s0                 ; Load device code for TX
                        SL1             sc                      ; add read flag
                        CALL            i2c_xmit                ; Send address and ack
                        CALL            i2c_sack

                        CALL            i2c_recv                ; Read 8 bits of data and send ack
                        CALL            i2c_mack
                        INPUT           s2, sc

                        CALL            i2c_recv                ; Read next 8 bits of data and send ack
                        CALL            i2c_mack
                        INPUT           s3, sc

                        CALL            i2c_recv                ; Read 8 bits of data and send ack
                        CALL            i2c_mack
                        INPUT           s4, sc

                        CALL            i2c_recv                ; Read next 8 bits of data and send ack
                        CALL            i2c_mnack
                        INPUT           s5, sc

                        JUMP            i2c_stop                ; Send Stop

; -------------------------------------------------------------------------------
; Routine to set up for and write two bytes to I2C device
; -------------------------------------------------------------------------------
write3_i2c:
                        CALL            i2c_start               ; Send Start, control byte and ack
                        INPUT           sc, s0                 ; Load device code for TX
                        SL0             sc                      ; add write flag
                        CALL            i2c_xmit                ; Send address and ack
                        CALL            i2c_sack

                        INPUT           sc, s1                 ; Load data for TX
                        CALL            i2c_xmit                ; Send data and ack
                        CALL            i2c_sack

                        INPUT           sc, s2                 ; Load data for TX
                        CALL            i2c_xmit                ; Send data and ack
                        CALL            i2c_sack

                        INPUT           sc, s3                 ; Load data for TX
                        CALL            i2c_xmit                ; Send data and ack
                        CALL            i2c_sack

                        JUMP            i2c_stop                ; Send stop

; -------------------------------------------------------------------------------
; Routine to set up for and read two bytes from I2C device
; -------------------------------------------------------------------------------

read2_i2c:
                        CALL            i2c_start               ; Send start, control byte and ack
                        INPUT           sc, s0                 ; Load device code for TX
                        SL1             sc                      ; add read flag
                        CALL            i2c_xmit                ; Send address and ack
                        CALL            i2c_sack

                        CALL            i2c_recv                ; Read 8 bits of data and send ack
                        CALL            i2c_mack
                        INPUT           s2, sc

                        CALL            i2c_recv                ; Read next 8 bits of data and send ack
                        CALL            i2c_mnack
                        INPUT           s3, sc

                        JUMP            i2c_stop                ; Send Stop

; -------------------------------------------------------------------------------
; Routine to set up for and write two bytes to I2C device
; -------------------------------------------------------------------------------
write2_i2c:
                        CALL            i2c_start               ; Send Start, control byte and ack
                        INPUT           sc, s0                 ; Load device code for TX
                        SL0             sc                      ; add write flag
                        CALL            i2c_xmit                ; Send address and ack
                        CALL            i2c_sack

                        INPUT           sc, s1                 ; Load data for TX
                        CALL            i2c_xmit                ; Send data and ack
                        CALL            i2c_sack

                        INPUT           sc, s2                 ; Load data for TX
                        CALL            i2c_xmit                ; Send data and ack
                        CALL            i2c_sack

                        JUMP            i2c_stop                ; Send stop

; -------------------------------------------------------------------------------
; Routine to set up for and read one byte of I2C device
; -------------------------------------------------------------------------------
read1_i2c:

                        CALL            i2c_start               ; Send start, control byte and ack
                        INPUT           sc, s0                 ; Load address for TX
                        SL1             sc                      ; add read flag
                        CALL            i2c_xmit                ; Send address and ack
                        CALL            i2c_sack

                        CALL            i2c_start               ; Send start, control byte and ack

                        CALL            i2c_recv                ; Read 8 bits of data and send ack
                        CALL            i2c_mnack
                        INPUT           s2, sc

                        JUMP            i2c_stop                ; Send Stop

; -------------------------------------------------------------------------------
; Routine to set up for and write one byte to I2C device
; -------------------------------------------------------------------------------
write1_i2c:

                        CALL            i2c_start               ; Send Start, control byte and ack
                        INPUT           sc, s0                 ; Load device code for TX
                        SL0             sc                      ; add write flag
                        CALL            i2c_xmit                ; Send address and ack
                        CALL            i2c_sack

                        INPUT           sc, s1                 ; Load data for TX
                        CALL            i2c_xmit                ; Send data and ack
                        CALL            i2c_sack

                        JUMP            i2c_stop                ; Send stop

; -------------------------------------------------------------------------------
; Transmit 8 bits of I2C data
; -------------------------------------------------------------------------------
i2c_xmit:
                        INPUT           sd, 8                   ; Load I2C bit counter
i2c_tx:
                        CALL            delay
                        SL0             sc                      ; Move data bit -> carry
                        JUMP            c, i2c_tx1              ; Jump if bit high
i2c_tx0:
                        OUTPUT          sf, bsda_0              ; SDA = 0
                        CALL            delay
                        JUMP            i2c_tx2                 ; Jump over next instructions
i2c_tx1:
                        OUTPUT          sf, bsda_z              ; SDA = 1 due to pull-up
                        CALL            delay
i2c_tx2:

                        OUTPUT          sf, bscl_z              ; SCL = 1 due to pull-up
                        CALL            delay
                        CALL            delay
                        CALL            delay
                        OUTPUT          sf, bscl_0              ; SCL = 0

                        SUB             sd, #1                  ; Decrement I2C bit counter
                        JUMP            nz, i2c_tx              ; Loop until 8 bits are sent
                        JUMP            delay

; -------------------------------------------------------------------------------
; Read 8 bits of I2C data
; -------------------------------------------------------------------------------
i2c_recv:
                        INPUT           sd, 8                   ; Load I2C bit counter
                        CALL            delay
                        OUTPUT          sf, bsda_z              ; SDA = 1 due to pull-up

i2c_rx:
                        CALL            delay
                        OUTPUT          sf, bscl_z              ; SCL = 1 due to pull-up
                        CALL            delay
                        CALL            delay
                        INPUT           sf, bsda_in
                        CALL            delay
                        SR0             sf                      ; LSBit -> carry bit
                        SLA             sc                      ; Carry shiloaded into LSBit
                        OUTPUT          sf, bscl_0              ; SCL = 0
                        CALL            delay
                        SUB             sd, #1                  ; Decrement I2C bit counter
                        JUMP            nz, i2c_rx              ; Loop until 8 bits are read
                        RETURN

; -------------------------------------------------------------------------------
; Ack by master, keep data 0
; -------------------------------------------------------------------------------
i2c_mack:
                        CALL            delay
                        OUTPUT          sf, bsda_0              ; SDA = 0
                        CALL            delay
                        OUTPUT          sf, bscl_z              ; SCL = 1 due to pull-up
                        CALL            delay
                        CALL            delay
                        CALL            delay
                        OUTPUT          sf, bscl_0              ; SCL = 0
                        CALL            delay
                        OUTPUT          sf, bsda_0              ; SDA = 0
                        RETURN

; -------------------------------------------------------------------------------
; NAck by master, release data
; -------------------------------------------------------------------------------
i2c_mnack:
                        CALL            delay
                        OUTPUT          sf, bsda_z              ; SDA = 1 due to pull-up
                        CALL            delay
                        OUTPUT          sf, bscl_z              ; SCL = 1 due to pull-up
                        CALL            delay
                        CALL            delay
                        CALL            delay
                        OUTPUT          sf, bscl_0              ; SCL = 0
                        CALL            delay
                        OUTPUT          sf, bsda_0              ; SDA = 0
                        RETURN

; -------------------------------------------------------------------------------
; Ack by slave, release data
; -------------------------------------------------------------------------------
i2c_sack:
                        CALL            delay
                        OUTPUT          sf, bsda_z              ; SDA = 1 due to pull-up
                        CALL            delay
                        OUTPUT          sf, bscl_z              ; SCL = 1 due to pull-up
                        CALL            delay
                        CALL            delay
                        CALL            delay
                        OUTPUT          sf, bscl_0              ; SCL = 0
                        CALL            delay
                        RETURN

; -------------------------------------------------------------------------------
; Set up start condition for I2C
; -------------------------------------------------------------------------------

; SCL and SDA set to inputs, signals go high due to pull-up resistors

i2c_repstart:
                        CALL            delay
                        OUTPUT          sf, bscl_z              ; SCL = Z
i2c_start:
                        CALL            delay
                        OUTPUT          sf, bsda_0              ; SDA = 0
                        CALL            delay
                        OUTPUT          sf, bscl_0              ; SCL = 0
                        CALL            delay
                        RETURN

; -------------------------------------------------------------------------------
; Send I2C stop command
; -------------------------------------------------------------------------------
i2c_stop:
                        CALL            delay
                        OUTPUT          sf, bscl_z              ; SCL = 1 due to pull-up
                        CALL            delay
                        OUTPUT          sf, bsda_z              ; SDA = 1 due to pull-up
                        CALL            delay
                        RETURN

; -----------I2C Subroutines end-------------------------------------------------

