        device  "001_BasicTest_I.procdef"

        org     0x0
        ;; PC == 0

lbl:    load    0, #5
        ;; step
        ;; PC == 1
        ;; reg[0] == 5

        load    1, #6
        ;; step
        ;; PC == 2
        ;; reg[1] == 6

        load    2, #7
        ;; step
        ;; PC == 3
        ;; reg[2] == 7

        jump    lbl
        ;; step
        ;; PC == 0

        ;; step 4
        ;; PC == 0

        end
