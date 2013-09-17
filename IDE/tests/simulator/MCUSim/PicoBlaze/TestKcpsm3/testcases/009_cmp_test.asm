; testcase for instructions compare and TEST


    ORG         0x000


Start:

            LOAD        s0,#20
            LOAD        s1,#1
            LOAD        s2,#50
            LOAD        s3,#50
            LOAD        s4,#20
            LOAD        s5,#1
            ;; step 6
            ;; reg[0]==20
            ;; reg[1]==1
            ;; reg[2]==50
            ;; reg[3]==50
            ;; reg[4]==20
            ;; reg[5]==1
            COMPARE     s0,#20
            
        