device "001_test.procdef"

S0      REG     0x0
S1      REG     0x1
S2      REG     0x2
S3      REG     0x3
S4      REG     0x4
S5      REG     0x5
S6      REG     0x6
S7      REG     0x7
S8      REG     0x8
S9      REG     0x9
SA      REG     0xA
SB      REG     0xB
SC      REG     0xC
SD      REG     0xD
SE      REG     0xE
SF      REG     0xF

  XXX     define          1 * {0} + {1}/{2}+5
  YYY     define          1 + {0}
  Abyss   EQU             XXX(1,2,YYY(0x01)) + 2


        LOAD        0x5, #Abyss
        ;; step
        ;; reg[5] == 9

