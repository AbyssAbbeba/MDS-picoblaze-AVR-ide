    device kcpsm1cpld

  XXX     define          1 * {0} + {1}/{2}+5
  YYY     define          1 + {0}
  Abyss   EQU             XXX(1,2,YYY(0x01)) + 2


        LOAD        0x5, #Abyss
        ;; step
        ;; reg[5] == 9

