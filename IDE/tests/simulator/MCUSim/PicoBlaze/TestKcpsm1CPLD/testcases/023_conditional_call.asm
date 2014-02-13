; TEST CASE
; testing conditional call and ret
device kcpsm1cpld
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
            call        Z,zero_test
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
            call        NZ,nzero_test
                        ;; step                 # sub
                        ;; step                 # call nz
                        ;; step                 #
                        ;; flag[z] == false
                        ;; pc == 12

            load        s2,#255
nzero_test:
            load        s2,#255
            add         s2,#10
            call        C,c_test

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
            call        NC,nc_test

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




           