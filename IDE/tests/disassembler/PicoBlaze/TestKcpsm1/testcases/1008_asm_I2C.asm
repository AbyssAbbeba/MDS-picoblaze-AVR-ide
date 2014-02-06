
device kcpsm1
; -------------------------------------------------------------------------------
; SPI routines
; EQU to move in s1, s1&s2, s1&s2&s3
; uses s0, sF
; sE: bit in the command reg of the SPI_LE pin
; sD: address of the command register
; sC: address of the SPI control/status register
; -------------------------------------------------------------------------------


bMOSI           EQU    0b00000001
bMISO           EQU    0b10000000
bSCLK           EQU    0b00000010

ESPI24:
      ; address of the SPI register, contains MOSI, MISO, SCLK signals
SPI24:
                IN      sF, sD                  ; switch on the appropriate CE
                XOR     sE, #FFh
                AND     sF, sE
                OUT     sF, sD

                LOAD    s0, s1
                CALL    SPI8
                LOAD    s0, s2
                CALL    SPI8
                LOAD    s0, s3
                CALL    SPI8

                IN      sF, sD
                XOR     sE, #FFh
                OR      sF, sE
                OUT     sF, sD                  ; switch off the CE

                RET

ESPI16:
      ; address of the SPI register, contains MOSI, MISO, SCLK signals
SPI16:
                IN      sF, sD                  ; switch on the appropriate CE
                XOR     sE, #FFh
                AND     sF, sE
                OUT     sF, sD

                LOAD    s0, s1
                CALL    SPI8
                LOAD    s0, s2
                CALL    SPI8

                IN      sF, sD
                XOR     sE, #FFh
                OR      sF, sE
                OUT     sF, sD                  ; switch off the CE

                RET

ESPI8:
    ; address of the SPI register, contains MOSI, MISO, SCLK signals
SPI8:
                IN      sF, sD                  ; switch on the appropriate CE
                XOR     sE, #FFh
                AND     sF, sE
                OUT     sF, sD

                LOAD    s0, s1
                CALL    SPI8

                IN      sF, sD
                XOR     sE, #FFh
                OR      sF, sE
                OUT     sF, sD                  ; switch off the CE

                RET

; -------------------------------------------------------------------------------
; raw SPI routines
; byte to move in s0
; uses sF
; -------------------------------------------------------------------------------

SPI8_:                                          ; shiload 8 bits
                CALL    SPI4_

SPI4_:                                          ; shiload 4 bits
                CALL    SPI2_

SPI2_:                                          ; shiload 2 bits
                CALL    SPI1_

; -------------------------------------------------------------------------------
; send one bit w/clock
; -------------------------------------------------------------------------------

SPI1_:                                          ; shiload 1 bit
                IN      sF, sC
                AND     sF, ~ bMOSI             ; clear MOSI bit

                SL0     s0                      ; check if upper bit set
                OR      sF, bMOSI               ; set MOSI bit

                OUT     sF, sC                  ; update MOSI signal
                OR      sF, bSCLK               ; set SCLK bit
                OUT     sF, sC                  ; update SCLK signal
                AND     sF, ~ bSCLK             ; clear SCLK bit
                OUT     sF, sC                  ; update SCLK bit

                IN      sF, sC
                RET     Z
                OR      s0, #1                   ; reflect it in s0
                RET

; -----------SPI Subroutines end-------------------------------------------------