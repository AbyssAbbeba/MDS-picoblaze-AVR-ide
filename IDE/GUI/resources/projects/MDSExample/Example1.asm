; MDS - Demonstration code
; Very simple code

; Press F2 and F6 to run the program (start simulator and animate)

        org     0h
        load    s0,#1
main:   
        store   s0,@s1
        inc     s1
        
        
        if      s1 == #255
            load     s1, #0
            inc      s0
            jump     main
        else
            jump     main   
        endif

        
        end
; end of demonstration code
; you can start developing your aplication by creating a new project