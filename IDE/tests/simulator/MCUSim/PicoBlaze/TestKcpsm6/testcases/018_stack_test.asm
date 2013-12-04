device kcpsm6
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