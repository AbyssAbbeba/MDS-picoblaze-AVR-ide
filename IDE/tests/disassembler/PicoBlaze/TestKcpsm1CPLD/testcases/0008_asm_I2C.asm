
device kcpsm1cpld
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
                IN      s1, s1                 ; switch on the appropriate CE
                XOR     s1, #FFh
                AND     s1, s1
                OUT     s1, s1

                LOAD    s0, s1
                CALL    SPI8
                LOAD    s0, s2
                CALL    SPI8
                LOAD    s0, s3
                CALL    SPI8

                IN      s1, s1
                XOR     s1, #FFh
                OR      s1, s1
                OUT     s1, s1                  ; switch off the CE

                RET

ESPI16:
      ; address of the SPI register, contains MOSI, MISO, SCLK signals
SPI16:
                IN      s1, s1                  ; switch on the appropriate CE
                XOR     s1, #FFh
                AND     s1, s1
                OUT     s1, s1

                LOAD    s0, s1
                CALL    SPI8
                LOAD    s0, s2
                CALL    SPI8

                IN      s1, s1
                XOR     s1, #FFh
                OR      s1, s1
                OUT     s1, s1                  ; switch off the CE

                RET

ESPI8:
    ; address of the SPI register, contains MOSI, MISO, SCLK signals
SPI8:
                IN      s1, s1                  ; switch on the appropriate CE
                XOR     s1, #FFh
                AND     s1, s1
                OUT     s1, s1

                LOAD    s0, s1
                CALL    SPI8

                IN      s1, s1
                XOR     s1, #FFh
                OR      s1, s1
                OUT     s1, s1                  ; switch off the CE

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
                IN      s1, s1
                AND     s1, ~ bMOSI             ; clear MOSI bit

                SL0     s0                      ; check if upper bit set
                OR      s1, bMOSI               ; set MOSI bit

                OUT     s1, s1                  ; update MOSI signal
                OR      s1, bSCLK               ; set SCLK bit
                OUT     s1, s1                  ; update SCLK signal
                AND     s1, ~ bSCLK             ; clear SCLK bit
                OUT     s1, s1                  ; update SCLK bit

                IN      s1, s1
                RET     Z
                OR      s0, #1                   ; reflect it in s0
                RET

; -----------SPI Subroutines end-------------------------------------------------