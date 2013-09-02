

ORG 0


DATA    AUTOREG         AT 5
ADRESA  AUTOREG
CMD1    AUTOREG
CMD2    AUTOREG
CMD3    AUTOREG
CMD4    AUTOREG
CMD5    AUTOREG

START:
        LOAD    DATA,#0xFF
        LOAD    ADRESA,1
        LOAD    DATA,CMD1
        LOAD    DATA,CMD2
        LOAD    DATA,CMD3
        LOAD    DATA,CMD4
        LOAD    DATA,CMD5

        JUMP    MULTIPLY

MULTIPLY:

X       SET              1
Y       SET              5
C       DEFINE           X * Y

        LOAD    ADRESA,#C
        END