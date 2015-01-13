; TEST CASE
; testing conditional et
device "024_conditional_return.procdef"

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

; jump instruction
            jump        aaa
            load        s1,#1
aaa:
            load        s2,#255
            load        s1,#1

            sub         s1,#1
            load        s3,#25
            call        _1
                        ;; 
                        ;; step
                        ;; pc == 2
                        ;; step 4
                        ;; flag[z] == true
                        ;; step 2               # call z
                        ;; pc == 7

            load        s2,#255
zero_test:
            load        s1,#1
            sub         s2,#12
            call        _2
                        ;; step                 # sub
                        ;; step                 # call nz
                        ;; step 3               #
                        ;; flag[z] == false
                        ;; pc == 11

            load        s2,#255
nzero_test:
            load        s2,#255
            add         s2,#10
            call        _3

                        ;; step                 # add
                        ;; pc == 12
                        ;; step                 # call c
                        ;; pc == 13
                        ;; step                 # after call load2
                        ;; flag[c] == true
                        ;; pc == 14
                        ;; step 2                 # call c
                        ;; pc == 15

            load        s2,#255
c_test:
            load        s2,#255
            add         s1,#10
            call        _4

                        ;; step                 #
                        ;; pc == 16
                        ;; step                 # call
                        ;; pc == 17
                        ;; step
                        ;; flag[c] == false
                        ;; step 2
                        ;; pc == 19
            load        s1,#1
nc_test:
            load        s1,#1



_1:
            returnZ    
_2:
            returnNZ
_3:
            returnC
_4:
            returnNC
           