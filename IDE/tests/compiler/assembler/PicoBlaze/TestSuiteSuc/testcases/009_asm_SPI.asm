DEVICE KCPSM3

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

Init_I2C:       OUTPUT     sF, bSDA_Z              ; SDA = Z
                OUTPUT     sF, bSCL_Z              ; SCL = Z
                JUMP    Delay

DElay:
; -------------------------------------------------------------------------------
; Routine to set up for and read four bytes from I2C device
; -------------------------------------------------------------------------------

ReadWrite4_I2C:
                CALL    I2C_Start               ; Send Start, control byte and ack
                INPUT    sC, @s0                  ; Load device code for TX
                SL0     sC                      ; add write flag
                CALL    I2C_Xmit                ; Send address and ack
                CALL    I2C_SAck

                INPUT    sC, @s1                  ; Load data for TX
                CALL    I2C_Xmit                ; Send data and ack
                CALL    I2C_SAck

                CALL    I2C_RepStart            ; Send repeated start, control byte and ack
                INPUT    sC, @s0                  ; Load device code for TX
                SL1     sC                      ; add read flag
                CALL    I2C_Xmit                ; Send address and ack
                CALL    I2C_SAck

                CALL    I2C_Recv                ; Read 8 bits of data and send ack
                CALL    I2C_MAck
                INPUT    s2, @sC

                CALL    I2C_Recv                ; Read next 8 bits of data and send ack
                CALL    I2C_MAck
                INPUT    s3, @sC

                CALL    I2C_Recv                ; Read 8 bits of data and send ack
                CALL    I2C_MAck
                INPUT    s4, @sC

                CALL    I2C_Recv                ; Read next 8 bits of data and send ack
                CALL    I2C_MNAck
                INPUT    s5, @sC

                JUMP    I2C_Stop                ; Send Stop

; -------------------------------------------------------------------------------
; Routine to set up for and write two bytes to I2C device
; -------------------------------------------------------------------------------
Write3_I2C:
                CALL    I2C_Start               ; Send Start, control byte and ack
                INPUT    sC, @s0                  ; Load device code for TX
                SL0     sC                      ; add write flag
                CALL    I2C_Xmit                ; Send address and ack
                CALL    I2C_SAck

                INPUT    sC, @s1                  ; Load data for TX
                CALL    I2C_Xmit                ; Send data and ack
                CALL    I2C_SAck

                INPUT    sC, @s2                  ; Load data for TX
                CALL    I2C_Xmit                ; Send data and ack
                CALL    I2C_SAck

                INPUT    sC, @s3                  ; Load data for TX
                CALL    I2C_Xmit                ; Send data and ack
                CALL    I2C_SAck

                JUMP    I2C_Stop                ; Send stop

; -------------------------------------------------------------------------------
; Routine to set up for and read two bytes from I2C device
; -------------------------------------------------------------------------------

Read2_I2C:
                CALL    I2C_Start               ; Send start, control byte and ack
                INPUT    sC, @s0                  ; Load device code for TX
                SL1     sC                      ; add read flag
                CALL    I2C_Xmit                ; Send address and ack
                CALL    I2C_SAck

                CALL    I2C_Recv                ; Read 8 bits of data and send ack
                CALL    I2C_MAck
                INPUT    s2, @sC

                CALL    I2C_Recv                ; Read next 8 bits of data and send ack
                CALL    I2C_MNAck
                INPUT    s3, @sC

                JUMP    I2C_Stop                ; Send Stop

; -------------------------------------------------------------------------------
; Routine to set up for and write two bytes to I2C device
; -------------------------------------------------------------------------------
Write2_I2C:
                CALL    I2C_Start               ; Send Start, control byte and ack
                INPUT    sC, @s0                  ; Load device code for TX
                SL0     sC                      ; add write flag
                CALL    I2C_Xmit                ; Send address and ack
                CALL    I2C_SAck

                INPUT    sC, @s1                  ; Load data for TX
                CALL    I2C_Xmit                ; Send data and ack
                CALL    I2C_SAck

                INPUT    sC, @s2                  ; Load data for TX
                CALL    I2C_Xmit                ; Send data and ack
                CALL    I2C_SAck

                JUMP    I2C_Stop                ; Send stop

; -------------------------------------------------------------------------------
; Routine to set up for and read one byte of I2C device
; -------------------------------------------------------------------------------
Read1_I2C:

                CALL    I2C_Start               ; Send start, control byte and ack
                INPUT    sC, @s0                  ; Load address for TX
                SL1     sC                      ; add read flag
                CALL    I2C_Xmit                ; Send address and ack
                CALL    I2C_SAck

                CALL    I2C_Start               ; Send start, control byte and ack

                CALL    I2C_Recv                ; Read 8 bits of data and send ack
                CALL    I2C_MNAck
                INPUT    s2, @sC

                JUMP    I2C_Stop                ; Send Stop

; -------------------------------------------------------------------------------
; Routine to set up for and write one byte to I2C device
; -------------------------------------------------------------------------------
Write1_I2C:

                CALL    I2C_Start               ; Send Start, control byte and ack
                INPUT    sC, @s0                  ; Load device code for TX
                SL0     sC                      ; add write flag
                CALL    I2C_Xmit                ; Send address and ack
                CALL    I2C_SAck

                INPUT    sC, @s1                  ; Load data for TX
                CALL    I2C_Xmit                ; Send data and ack
                CALL    I2C_SAck

                JUMP    I2C_Stop                ; Send stop

; -------------------------------------------------------------------------------
; Transmit 8 bits of I2C data
; -------------------------------------------------------------------------------
I2C_Xmit:
                INPUT    sD, 8                   ; Load I2C bit counter
I2C_TX:
                CALL    Delay
                SL0     sC                      ; Move data bit -> carry
                JUMP    C, I2C_TX1              ; Jump if bit high
I2C_TX0:
                OUTPUT     sF, bSDA_0              ; SDA = 0
                CALL    Delay
                JUMP    I2C_TX2                 ; Jump over next instructions
I2C_TX1:
                OUTPUT     sF, bSDA_Z              ; SDA = 1 due to pull-up
                CALL    Delay
I2C_TX2:

                OUTPUT     sF, bSCL_Z              ; SCL = 1 due to pull-up
                CALL    Delay
                CALL    Delay
                CALL    Delay
                OUTPUT     sF, bSCL_0              ; SCL = 0

                SUB     sD, 1                   ; Decrement I2C bit counter
                JUMP    NZ, I2C_TX              ; Loop until 8 bits are sent
                JUMP    Delay

; -------------------------------------------------------------------------------
; Read 8 bits of I2C data
; -------------------------------------------------------------------------------
I2C_Recv:
                INPUT    sD, 8                   ; Load I2C bit counter
                CALL    Delay
                OUTPUT     sF, bSDA_Z              ; SDA = 1 due to pull-up

I2C_RX:
                CALL    Delay
                OUTPUT     sF, bSCL_Z              ; SCL = 1 due to pull-up
                CALL    Delay
                CALL    Delay
                IN      sF, bSDA_In
                CALL    Delay
                SR0     sF                      ; LSBit -> carry bit
                SLA     sC                      ; Carry shifted into LSBit
                OUTPUT     sF, bSCL_0              ; SCL = 0
                CALL    Delay
                SUB     sD, 1                   ; Decrement I2C bit counter
                JUMP    NZ, I2C_RX              ; Loop until 8 bits are read
                RETURN

; -------------------------------------------------------------------------------
; Ack by master, keep data 0
; -------------------------------------------------------------------------------
I2C_MAck:
                CALL    Delay
                OUTPUT     sF, bSDA_0              ; SDA = 0
                CALL    Delay
                OUTPUT     sF, bSCL_Z              ; SCL = 1 due to pull-up
                CALL    Delay
                CALL    Delay
                CALL    Delay
                OUTPUT     sF, bSCL_0              ; SCL = 0
                CALL    Delay
                OUTPUT     sF, bSDA_0              ; SDA = 0
                RETURN

; -------------------------------------------------------------------------------
; NAck by master, release data
; -------------------------------------------------------------------------------
I2C_MNAck:
                CALL    Delay
                OUTPUT     sF, bSDA_Z              ; SDA = 1 due to pull-up
                CALL    Delay
                OUTPUT     sF, bSCL_Z              ; SCL = 1 due to pull-up
                CALL    Delay
                CALL    Delay
                CALL    Delay
                OUTPUT     sF, bSCL_0              ; SCL = 0
                CALL    Delay
                OUTPUT     sF, bSDA_0              ; SDA = 0
                RETURN

; -------------------------------------------------------------------------------
; Ack by slave, release data
; -------------------------------------------------------------------------------
I2C_SAck:
                CALL    Delay
                OUTPUT     sF, bSDA_Z              ; SDA = 1 due to pull-up
                CALL    Delay
                OUTPUT     sF, bSCL_Z              ; SCL = 1 due to pull-up
                CALL    Delay
                CALL    Delay
                CALL    Delay
                OUTPUT     sF, bSCL_0              ; SCL = 0
                CALL    Delay
                RETURN

; -------------------------------------------------------------------------------
; Set up start condition for I2C
; -------------------------------------------------------------------------------

; SCL and SDA set to inputs, signals go high due to pull-up resistors

I2C_RepStart:
                CALL    Delay
                OUTPUT     sF, bSCL_Z              ; SCL = Z
I2C_Start:
                CALL    Delay
                OUTPUT     sF, bSDA_0              ; SDA = 0
                CALL    Delay
                OUTPUT     sF, bSCL_0              ; SCL = 0
                CALL    Delay
                RETURN

; -------------------------------------------------------------------------------
; Send I2C stop command
; -------------------------------------------------------------------------------
I2C_Stop:
                CALL    Delay
                OUTPUT     sF, bSCL_Z              ; SCL = 1 due to pull-up
                CALL    Delay
                OUTPUT     sF, bSDA_Z              ; SDA = 1 due to pull-up
                CALL    Delay
                RETURN

; -----------I2C Subroutines end-------------------------------------------------
