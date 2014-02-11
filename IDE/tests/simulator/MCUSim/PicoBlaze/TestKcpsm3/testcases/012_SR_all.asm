    ; testing of rotate instructions

device kcpsm3
    org 00h

start:      load        s0,#128
            load        s1,#128
            load        s2,#128
            load        s3,#0
            ;; step     4
            ;; reg[0] == 128
            ;; reg[1] == 128
            ;; reg[2] == 128
            ;; reg[3] == 0
; sr0 testing
            sr0         s0
            ;; step
            ;; reg[0] == 64
            ;; flag[c] == false
            ;; flag[z] == false
            sr0         s0
            ;; step
            ;; reg[0] == 32
            ;; flag[c] == false
            ;; flag[z] == false

            sr0         s0
            ;; step
            ;; reg[0] == 16
            ;; flag[c] == false
            ;; flag[z] == false
            sr0         s0
            ;; step
            ;; reg[0] == 8
            ;; flag[c] == false
            ;; flag[z] == false

            sr0         s0
            ;; step
            ;; reg[0] == 4
            ;; flag[c] == false
            ;; flag[z] == false
            sr0         s0
            ;; step
            ;; reg[0] == 2
            ;; flag[c] == false
            ;; flag[z] == false
            sr0         s0
            ;; step
            ;; reg[0] == 1
            ;; flag[c] == false
            ;; flag[z] == false
            sr0         s0
            ;; step
            ;; reg[0] == 0
            ;; flag[c] == true
            ;; flag[z] == true

; sr1 testing
            sr1         s0
            ;; step
            ;; reg[0] == 128
            sr1         s0
            ;; step
            ;; reg[0] == 192
            sr1         s0
            ;; step
            ;; reg[0] == 224
            sr1         s0
            ;; step
            ;; reg[0] == 240
            sr1         s0
            ;; step
            ;; reg[0] == 248
            sr1         s0
            ;; step
            ;; reg[0] == 252
            sr1         s0
            ;; step
            ;; reg[0] == 254
            ;; flag[c] == false
            ;; flag[z] == false
            sr1         s0
            ;; step
            ;; reg[0] == 255
            ;; flag[c] == false
            ;; flag[z] == false
            sr1         s0
            ;; step
            ;; reg[0] == 255
            ;; flag[c] == true
            ;; flag[z] == false
; srx testing
            load        s2,#128
            srx         s2
            ;; step 2
            ;; reg[2] == 192
            srx         s2
            ;; step
            ;; reg[2] == 224
            srx         s2
            ;; step
            ;; reg[2] == 240
            srx         s2
            ;; step
            ;; reg[2] == 248
            srx         s2
            ;; step
            ;; reg[2] == 252
            srx         s2
            ;; step
            ;; reg[2] == 254
            ;; flag[c] == false
            ;; flag[z] == false
            srx        s2
            ;; step
            ;; reg[0] == 255
            ;; flag[c] == false
            ;; flag[z] == false
            srx         s2
            ;; step
            ;; reg[2] == 255
            ;; flag[c] == true
            ;; flag[z] == false
            load        s2,#1
            srx         s2
            ;; step 2
            ;; flag[c] == true
            ;; flag[z] == true
; sra testing
            sra         s3
            ;; step
            ;; reg[3] == 128
            sra         s3
            ;; step
            ;; reg[3] == 64
            sra         s3
            ;; step
            ;; reg[3] == 32
            sra         s3
            ;; step
            ;; reg[3] == 16
            sra         s3
            ;; step
            ;; reg[3] == 8
            sra         s3
            ;; step
            ;; reg[3] == 4
            sra         s3
            ;; step
            ;; reg[3] == 2
            sra         s3
            ;; step
            ;; reg[3] == 1
            sra         s3
            ;; step
            ;; reg[3] == 0
            ;; flag[z] == true
            ;; flag[c] == true
            sra         s3
            ;; step
            ;; reg[3] == 128
            ;; flag[z] == false
            ;; flag[c] == false
; testing of rr
            rr         s3
            ;; step
            ;; reg[3] == 64
            rr         s3
            ;; step
            ;; reg[3] == 32
            rr         s3
            ;; step
            ;; reg[3] == 16
            rr         s3
            ;; step
            ;; reg[3] == 8
            rr         s3
            ;; step
            ;; reg[3] == 4
            rr         s3
            ;; step
            ;; reg[3] == 2
            rr         s3
            ;; step
            ;; reg[3] == 1
            rr         s3
            ;; step
            ;; reg[3] == 128
            ;; flag[c] == true
            ;; flag[z] == false
            load        s3,#0
            rr          s3
            ;; step 2
            ;; flag[z] == true
            ;; flag[c] == false
