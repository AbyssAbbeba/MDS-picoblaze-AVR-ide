        device  "SimpleArch_I.procdef"

        org     0x0

lbl:    load    0, #5
        load    1, #6
        load    2, #7
        jump    lbl

        end
