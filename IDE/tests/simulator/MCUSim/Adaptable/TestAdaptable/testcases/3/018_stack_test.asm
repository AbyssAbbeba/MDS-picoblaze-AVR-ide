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

        ORG     0h                      ; Define code segment

; --------------------
start:
        ;; step 
        ;; PC == 3
        ;; stack[0] == 1
        ;; step
        ;; pc == 5
        ;; stack[1] == 4
        ;; step
        ;; pc == 7
        ;; stack[2] == 6
        ;; step
        ;; pc == 6
        ;; step
        ;; pc == 4
        ;; step
        ;; pc == 1
        ;; step
        ;; pc == 8
        ;; step
        ;; pc == 2
        
       call ahoj
       call sss
       jump start

ahoj: call ahoj2
      return

ahoj2: call ahoj3
       return

ahoj3: return

sss: return