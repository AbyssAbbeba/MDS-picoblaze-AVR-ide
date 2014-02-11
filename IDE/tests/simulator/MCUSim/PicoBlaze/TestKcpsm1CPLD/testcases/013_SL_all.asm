    ; testing of rotate instructions

device kcpsm1CPLD
    org 00h

start:      load        s0,#1
            load        s1,#1
            load        s2,#1
            load        s3,#0
            ;; step     4
            ;; reg[0] == 1
            ;; reg[1] == 1
            ;; reg[2] == 1
            ;; reg[3] == 0
; sl0 testing
            sl0         s0
            ;; step
            ;; reg[0] == 2
            ;; flag[c] == false
            ;; flag[z] == false
            sl0         s0
            ;; step
            ;; reg[0] == 4
            ;; flag[c] == false
            ;; flag[z] == false
           
            sl0         s0
            ;; step
            ;; reg[0] == 8
            ;; flag[c] == false
            ;; flag[z] == false
            sl0         s0
            ;; step
            ;; reg[0] == 16
            ;; flag[c] == false
            ;; flag[z] == false
           
            sl0         s0
            ;; step
            ;; reg[0] == 32
            ;; flag[c] == false
            ;; flag[z] == false
            sl0         s0
            ;; step
            ;; reg[0] == 64
            ;; flag[c] == false
            ;; flag[z] == false
            sl0         s0
            ;; step
            ;; reg[0] == 128
            ;; flag[c] == false
            ;; flag[z] == false
            sl0         s0
            ;; step
            ;; reg[0] == 0
            ;; flag[c] == true
            ;; flag[z] == true

; sl1 testing
            sl1         s0
            ;; step
            ;; reg[0] == 1
            sl1         s0
            ;; step
            ;; reg[0] == 3
            sl1         s0
            ;; step
            ;; reg[0] == 7
            sl1         s0
            ;; step
            ;; reg[0] == 15
            sl1         s0
            ;; step
            ;; reg[0] == 31
            sl1         s0
            ;; step
            ;; reg[0] == 63
            sl1         s0
            ;; step
            ;; reg[0] == 127
            ;; flag[c] == false
            ;; flag[z] == false
            sl1         s0
            ;; step
            ;; reg[0] == 255
            ;; flag[c] == false
            ;; flag[z] == false
            sl1         s0
            ;; step
            ;; reg[0] == 255
            ;; flag[c] == true
            ;; flag[z] == false
; slx testing
            load        s2,#1                
            slx         s2
            ;; step 2
            ;; reg[2] == 3          
            slx         s2
            ;; step
            ;; reg[2] == 7
            slx         s2
            ;; step
            ;; reg[2] == 15
            slx         s2
            ;; step
            ;; reg[2] == 31
            slx         s2
            ;; step
            ;; reg[2] == 63
            slx         s2
            ;; step
            ;; reg[2] == 127
            ;; flag[c] == false
            ;; flag[z] == false
            slx         s2
            ;; step
            ;; reg[0] == 255
            ;; flag[c] == false
            ;; flag[z] == false
            slx         s2
            ;; step
            ;; reg[2] == 255
            ;; flag[c] == true
            ;; flag[z] == false
            load        s2,#128
            slx         s2
            ;; step 2
            ;; flag[c] == true
            ;; flag[z] == true  
; sla testing
            sla         s3
            ;; step     
            ;; reg[3] == 1
            sla         s3
            ;; step
            ;; reg[3] == 2
            sla         s3
            ;; step
            ;; reg[3] == 4
            sla         s3
            ;; step
            ;; reg[3] == 8
            sla         s3
            ;; step
            ;; reg[3] == 16
            sla         s3
            ;; step
            ;; reg[3] == 32
            sla         s3
            ;; step
            ;; reg[3] == 64
            sla         s3
            ;; step
            ;; reg[3] == 128
            sla         s3
            ;; step
            ;; reg[3] == 0
            ;; flag[z] == true
            ;; flag[c] == true
            sla         s3
            ;; step
            ;; reg[3] == 1
            ;; flag[z] == false
            ;; flag[c] == false
; testing of rl
            rl         s3
            ;; step
            ;; reg[3] == 2
            rl         s3
            ;; step
            ;; reg[3] == 4
            rl         s3
            ;; step
            ;; reg[3] == 8
            rl         s3
            ;; step
            ;; reg[3] == 16
            rl         s3
            ;; step
            ;; reg[3] == 32
            rl         s3
            ;; step
            ;; reg[3] == 64
            rl         s3
            ;; step
            ;; reg[3] == 128
            rl         s3
            ;; step
            ;; reg[3] == 1
            ;; flag[c] == true
            ;; flag[z] == false
            load        s3,#0
            rl          s3
            ;; step 2
            ;; flag[z] == true
            ;; flag[c] == false
