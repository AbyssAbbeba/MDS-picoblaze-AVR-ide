    ; testing of rotate instructions


    org 00h

    start:          load        s0,#1
                    load        s1,#1
                    load        s2,#1
                    load        s3,#1
                    ;; step     4
                    ;; reg[0] == 1
                    ;; reg[1] == 1
                    ;; reg[2] == 1
                    ;; reg[3] == 1
                    sl0         s0
                    ;; step
                    ;; reg[0] == 2
                    sl1         s1
                    ;; step
                    ;; reg[1] == 3
                    slx         s2
                    ;; step
                    ;; reg[2] == 1
                    sla         s3
                    ;; step
                    ;; reg[3] == 2
                    rl          s0
                    ;; step
                    ;; reg[0] == 4
                    load        s4,#3
                    ;; step
                    ;; reg[4] == 3
                    slx         s4
                    ;; step
                    ;; reg[4] == 5
                    slx         s4
                    ;; step
                    ;; reg[4] == 9
                    