
                    DEVICE KCPSM3

ADDRESS_SPR               AUTOREG
STORE_DATA                AUTOREG


         ORG       0x000

     Start:         LOAD     ADDRESS_SPR,#00h
                    LOAD     STORE_DATA,#FFh
                    ;; step 2
                    ;; reg[0]== 0
                    ;; reg[1]== 0xff

SAVE_DATA:          STORE    STORE_DATA,@ADDRESS_SPR
                    ADD      ADDRESS_SPR,#10
                    STORE    STORE_DATA,@ADDRESS_SPR
                    ADD      ADDRESS_SPR,#15

                    STORE    STORE_DATA,@ADDRESS_SPR
                    ADD      ADDRESS_SPR,#10
                    ;; step     2
                    ;; data[25]== 0xff
                    ;; reg[0] == 35                    
                    STORE    STORE_DATA,5
                    ;; step     
                    ;; data[5]== 0xff
                    
                    STORE    STORE_DATA,63
                    ;; step     
                    ;; data[63]== 0xff
                    ;; 
                    ;;
                    STORE    STORE_DATA,54
                    ;; step     
                    ;; data[54]== 0xff
                   
                    ;;              
FETCH_DATA:         FETCH    S2,0
                    ;; step
                    ;; reg[0] == 35
                    FETCH    S3,10
                    ;; step
                    ;; reg[3] == 0xff
                    FETCH    S4,25
                    ;; step
                    ;; reg[4] == 0xff
                    FETCH    S5,35
                    ;; step
                    ;; reg[5] == 0
                    FETCH    S6,5
                    ;; step
                    ;; reg[6] == 0xff
                    FETCH    S7,64
                    ;; step
                    ;; reg[7] == 0xff
                    FETCH    S8,54
                    ;; step
                    ;; reg[8] == 0xff
