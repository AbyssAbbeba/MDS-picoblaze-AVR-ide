; MDS PicoBlaze IDE - Demonstration code

; Macro instructions, conditional compilation, constants and scratch-pad ram handling.
; See manual for more info

; Press Start simulation and Animate to run the program

; Tell compiler type of procesor (KCPSM2, KCPSM3, KCPSM6 available)
        ;DEVICE          KCPSM6

; Program initialization
; --------------------
        ORG     0h                      ; Define code segment
        JUMP    start                   ; Jump to code initialization

; Program start
; --------------------
start:
			load s0,#10

                   ;   if   s0  != #100
            COMPARE 0x0, #0x64
              JUMP    Z, IF_5
              add     s3,#1
                               ;  else
              JUMP    IF_5_END
  IF_5:
                sub     s3,#1
                          ;  endif
  IF_5_END:
         
 			jump    next5

next5:

        if   s0  != #100
            add     s3,#1
            else
            sub     s3,#1
        endif

        jump    $
