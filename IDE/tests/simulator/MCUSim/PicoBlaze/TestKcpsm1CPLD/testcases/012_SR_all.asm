    ; testing of rotate instructions

device kcpsm1
    org 00h

    start:          load        s0,#128
                    load        s1,#128
                    load        s2,#128
                    load        s3,#128
                    ;; step     4
                    ;; reg[0] == 128
                    ;; reg[1] == 128
                    ;; reg[2] == 128
                    ;; reg[3] == 128
                    sr0         s0
                    ;; step
                    ;; reg[0] == 64
                    sr1         s1
                    ;; step
                    ;; reg[1] == 192
                    srx         s2
                    ;; step
                    ;; reg[2] == 192
                    sra         s3
                    ;; step
                    ;; reg[3] == 64
                    rr          s0
                    ;; step
                    ;; reg[0] == 32
                    load        s4,#192
                    ;; step
                    ;; reg[4] == 192
                    srx         s4
                    ;; step
                    ;; reg[4] == 224
                    srx         s4
                    ;; step
                    ;; reg[4] == 240
                    
                    
                    