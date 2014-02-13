; TEST CASE
; testing conditional et
device kcpsm6
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
                        ;; step 2               #
                        ;; flag[z] == false
                        ;; pc == 11

            load        s2,#255
nzero_test:
            load        s2,#255
            add         s2,#10
            call        _3

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
            call        _4

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



_1:
            return      Z    
_2:
            return      NZ   
_3:
            return      C
_4:
            return      NC
           