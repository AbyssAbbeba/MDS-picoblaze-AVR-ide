; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device kcpsm1
org     0


            load                s0,#5
            load                s1,#250
            ;; step                     # load
            ;; step                     # load
loop:
            DJNZ                s0,loop
            ;; step                     # sub
            ;;
            ;; reg[0] == 4
            ;; flag[z] == false
            ;; flag[c] == false
            ;; step                     #jump
            ;; step 8
            ;; flag[z] == true
            ;; flag[c] == false
            load                 s3,#45
            ;; step                     # load
            ;; reg[3] == 45


loop2:
            IJNZ                s1,loop2
            ;; step                     # add
            ;;
            ;; reg[1] == 251
            ;; flag[z] == false
            ;; flag[c] == false
            ;; step                     #jump
            ;; step 10
            ;; flag[z] == true
            ;; flag[c] == true
            load                 s1,#55
            ;; step                     # load
            ;; reg[1] == 55















            
            