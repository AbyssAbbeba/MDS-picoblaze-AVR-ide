
        ORG     0h                      ; Define code segment

; --------------------
start:
        ;; step 8
        ;; PC == 0
        ;; stack[0] == 0
       call ahoj
       call sss
       jump start

ahoj: call ahoj2
      return

ahoj2: call ahoj3
       return

ahoj3: return

sss: return