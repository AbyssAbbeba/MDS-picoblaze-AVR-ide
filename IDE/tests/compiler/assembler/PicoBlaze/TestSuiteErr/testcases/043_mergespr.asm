mergespr        0x1A9


device kcpsm6

org 0

load s0,#2

load s0,#2
iijnz   s0,loop
load s0,#2

ljnz            s1,loop

sešc
dec 80
dec4

inc init
clrr                    s1
setrr                   s2
setbit                          s3,1
clrbit                          s3,1


