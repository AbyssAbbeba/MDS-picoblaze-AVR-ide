; TEST CASE
; testing conditional call and ret
device "023_conditional_call.procdef"

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

org     0

start:

; call instruction
            call        aaa
            load        s1,#1
aaa:
            load        s2,#255
            load        s1,#1

            sub         s1,#1
            load        s3,#25
            callZ       zero_test
                        ;; step
                        ;; pc == 2
                        ;; step 4
                        ;; flag[z] == true
                        ;; step                 # call z
                        ;; pc == 8

            load        s2,#255
zero_test:
            load        s1,#1
            sub         s2,#12
            callNZ      nzero_test
                        ;; step                 # sub
                        ;; step                 # call nz
                        ;; step                 #
                        ;; flag[z] == false
                        ;; pc == 12

            load        s2,#255
nzero_test:
            load        s2,#255
            add         s2,#10
            callC       c_test

                        ;; step                 # add
                        ;; pc == 13
                        ;; step                 # call c
                        ;; pc == 14
                        ;; flag[c] == true
                        ;; pc == 14
                        ;; step                 # after call load2
                        ;; pc == 16

            load        s2,#255
c_test:
            load        s2,#255
            add         s1,#10
            callNC      nc_test

                        ;; step                 #
                        ;; pc == 17
                        ;; step                 # call
                        ;; pc == 18
                        ;; step
                        ;; flag[c] == false
                        ;; pc == 20
            load        s1,#1
nc_test:
            load        s1,#1




           