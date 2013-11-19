
device kcpsm1
ORG 0


DATA1    AUTOREG         AT 5
ADRESA  AUTOREG
CMD1    AUTOREG
CMD2    AUTOREG
CMD3    AUTOREG
CMD4    AUTOREG
CMD5    AUTOREG

START:
        LOAD    DATA1,#0xFF
        LOAD    ADRESA,1
        LOAD    DATA1,CMD1
        LOAD    DATA1,CMD2
        LOAD    DATA1,CMD3
        LOAD    DATA1,CMD4
        LOAD    DATA1,CMD5

        JUMP    MULTIPLY

MULTIPLY:

X       SET              1
Y       SET              5
C       DEFINE           X * Y

        LOAD    ADRESA,#C
        END