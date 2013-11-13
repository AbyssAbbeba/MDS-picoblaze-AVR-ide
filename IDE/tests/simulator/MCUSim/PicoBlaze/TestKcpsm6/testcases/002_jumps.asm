

   device kcpsm6

    ORG         0x00
        
Start:
       
        ;; PC == 0
      
        LOAD    0x00,#1
        ;; STEP
        ;; PC == 1
        ;; REG[0x00] == 1  
        LOAD    0x00,#55
        ;; STEP
        ;; PC == 2
        ;; REG[0x00] == 55        
        JUMP    HOP1
        ;; STEP
        ;; PC == 3
HOP1:   JUMP    HOP2
        ;; STEP
        ;; PC == 4
HOP2:   JUMP    HOP3
        ;; STEP
        ;; PC == 5
HOP3:   JUMP    HOP4


        LOAD    s0,#1
        ;; STEP
        ;; PC == 7
HOP4:   JUMP    HOP5
        ;; STEP
        ;; PC == 8
HOP5:   JUMP    HOP6
        ;; STEP
        ;; PC == 0x0C

        LOAD    s0,#1
        ;;STEP
        ;;PC == 13
        LOAD    s0,#1
        LOAD    s0,#1
HOP6:   JUMP    HOP7
HOP7:   JUMP    HOP8
        LOAD    s0,#1
        LOAD    s0,#12
HOP8:   
        ;; STEP
        ;; PC == 16
END