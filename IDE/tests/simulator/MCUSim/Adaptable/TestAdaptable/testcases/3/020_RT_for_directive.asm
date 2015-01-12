; TEST CASE
; testing run time directives, rtif rtfor, rt#WHILE
device "003_picoblazecopy.procdef"

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
        LOAD    S0, #10
        ;; step 
        ;; reg[0] == 10
        
        FOR          S0,2
        add              S6,#1
        ENDF
        ;; step         # load
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # add
        ;; reg[6] == 1
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # add
        ;; reg[6] == 2
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == true
        
      
        FOR          S0,10..12
        add              S7,#1
        ENDF
        ;; step         # load
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # add
        ;; reg[7] == 1
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # add
        ;; reg[7] == 2
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == true
        
        FOR          S0,10..20,5
        add              S8,#1
        ENDF

        jump            next1
        ;; step         # load
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # add
        ;; reg[8] == 1
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == false
        ;; step         # add
        ;; step         # add
        ;; reg[8] == 2
        ;; step         # jump to compare
        ;; step         # compare
        ;; step         # jump z
        ;; flag[z] == true
        ;; step         # jump to next for

        ;; # test of nested for
next1:
        FOR     s0,2
            FOR         s0,1
                add         s9,#1
            ENDF
            add     sA,#1
        ENDF
        ;; step         # load  1
        ;; step         # compare 1
        ;; step         # jump z 1
            ;; flag[z] == false
        ;; step         # load  2
        ;; step         # compare 2
        ;; step         # jump z 2
            ;; flag[z] == false
        ;; step         # add 2
            ;; reg[9] == 1
        ;; step         # add 2
        ;; step         # jump to compare 2
        ;; step         # compare       2
        ;; step         # jump z 2
            ;; flag[z] == true
        ;; step         # add 1
        ;; step         # add 1
            ;; reg[10] == 1
        ;; step         # jump to compare 1
        ;; step         # compare 1
        ;; step         # jump z 1
            ;; flag[z] == true
        ;; step         # load  2
        ;; step         # compare 2
        ;; step         # jump z 2
            ;; flag[z] == true
        ;; step         # add 2
            ;; reg[9] == 1
        ;; step         # add 2
        ;; step         # jump to compare 2
        ;; step         # compare       2
        ;; step         # jump z 2
            ;; flag[z] == true
        ;; step         # add 1
        ;; step         # add 1
            ;; reg[10] == 1
        ;; step         # jump to compare 1
        ;; step         # compare 1
        ;; step         # jump z 1
            ;; flag[z] == true
        
 

        