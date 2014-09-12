; MDS - Demonstration code
; Very simple code

; Press F2 and F6 to run the program (start simulator and animate)

        org     0h

main:   inc     s1
        store   s0,@s1
        
        if      s1 == #64
            load     s1, #0
            jump     main
        else
            jump     main   
        endif

        
        end
; end of demonstration code
; you can start developing your aplication by creating a new project