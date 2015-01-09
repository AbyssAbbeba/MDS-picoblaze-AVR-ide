; TEST CASE
; testing conditional jump and ret
device "003_picoblazecopy.procdef"
org     0

numero          equ             1

start:

; jump instruction
            load        s0,#numero
            load        s1,#numero + 1
            load        s2,#numero + 2
            load        s3,#numero + 3 
            load        s4,#numero + 4
            load        s5,#numero + 5
            load        s6,#numero + 6
            load        s7,#numero + 7
            ;; step 8
aaa:

            store       s0,1
            ;; step
            ;; data[1] == 1
            store       s0,5
            ;; step
            ;; data[5] == 1
            store       s0,15
            ;; step
            ;; data[15] == 1
            store       s0,13
            ;; step
            ;; data[13] == 1
            store       s0,18
            ;; step
            ;; data[18] == 1
            store       s0,24
            ;; step
            ;; data[24] == 1
            store       s0,30
            ;; step
            ;; data[30] == 1
            store       s0,35
            ;; step
            ;; data[35] == 1
            store       s0,63
            ;; step
            ;; data[1] == 1

            fetch       s8,1
            ;; step
            ;; reg[8] == 1
            fetch       s9,5
            ;; step
            ;; reg[9] == 1
            fetch       sa,15
            ;; step
            ;; reg[10] == 1
            fetch       sb,13
            ;; step
            ;; reg[11] == 1
            fetch       sc,18
            ;; step
            ;; reg[12] == 1
            fetch       sd,24
            ;; step
            ;; reg[13] == 1
            fetch       se,30
            ;; step
            ;; reg[14] == 1
            fetch       sf,35
            ;; step
            ;; reg[15] == 1

            store       s8,@1
            ;; step
            ;; data[2] == 1
            store       s9,@2
            ;; step
            ;; data[3] == 1
            store       sa,@3
            ;; step
            ;; data[4] == 1
            store       sb,@4
            ;; step
            ;; data[5] == 1
            store       sc,@5
            ;; step
            ;; data[6] == 1
            store       sd,@7
            ;; step
            ;; data[8] == 1
            store       se,@s0
            ;; step
            ;; data[1] == 1
            store       sf,@s7
            ;; step
            ;; data[15] == 1


            fetch       s8,@1
            ;; step
            ;; reg[8] == 1
            fetch       s9,@2
            ;; step
            ;; reg[9] == 1
            fetch       sa,@s1
            ;; step
            ;; reg[10] == 1
            fetch       sb,@s3
            ;; step
            ;; reg[11] == 1
            fetch       sc,@s5
            ;; step
            ;; reg[12] == 1
            fetch       sd,@s7
            ;; step
            ;; reg[13] == 1
            fetch       se,@s0
            ;; step
            ;; reg[14] == 1
            fetch       sf,@s2
            ;; step
            ;; reg[15] == 1


           