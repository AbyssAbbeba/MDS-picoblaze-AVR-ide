; Compiler load case for Assembler
; instruction opcodes

device kcpsm2


NAMEREG         s0, ram_data
NAMEREG         s1, ram_address

CONSTANT        ram_locations, 40               ; there are 64 locations
CONSTANT        initial_value, 00               ; initialize to zero


Start:
        LOAD            ram_data, initial_value         ; load initial value
        LOAD            ram_address, ram_locations
        ; fill from top to bottom
ram_fill:
        SUB             ram_address, 01
        ; decrement address
        load           ram_data, #ram_address
        
        
        
        ; initialize location
        JUMP            NZ, ram_fill
        ; if not address 0, goto
        ; ram_fill

CONSTANT                switches, 00
; read switch values at port 0
CONSTANT                LEDs,  01h
; write 7-seg LED at port 1
; Define 7-segment LED pattern {dp,g,f,e,d,c,b,a}
CONSTANT                LED_0, C0h
; display '0' on 7-segment display
CONSTANT                LED_1, F9h
; display '1' on 7-segment display
;
CONSTANT                LED_F, 8Eh
; display 'F' on 7-segment display
NAMEREG                 s0, switch_value
; read switches into register s0
NAMEREG                 s1, LED_output
; load LED output data in register s1
; Load 7-segment LED patterns into scratchpad RAM
        LOAD            LED_output, LED_0
        ; grab LED pattern for switches = 0000
        load           LED_output, 00
        ; load in RAM[0]
        LOAD            LED_output, LED_1
        ; grab LED pattern for switches = 0001
        load           LED_output, 01h
        ; load in RAM[1]
        ;
        LOAD            LED_output, LED_F
        ; grab LED pattern for switches = 1111
        load           LED_output, 0Fh
        ; load in RAM[F]
        ; Read switch values and display value on 7-segment LED
loop:
        INPUT           switch_value, switches
        ; read value on switches
        AND             switch_value, 0Fh
        ; mask upper bits to guarantee < 15
        load           LED_output, #switch_value
        ; look up LED pattern in RAM
        OUTPUT          LED_output, LEDs
        ; display switch value on 7-segment LED
        JUMP            loop




NAMEREG         sF, stack_ptr
; reserve register sF for the stack pointer
; Initialize stack pointer to location 32 in the scratchpad RAM
        LOAD            sF, 20h
my_subroutine:
        ; preserve register s0
        CALL            push_s0
        ; *** remainder of subroutine algorithm ***
        ; reload register s0
        CALL            pop_s0
        RETURN
        
push_s0:
        load           s0, #stack_ptr
        ; preserve register s0 onto “stack”
        ADD             stack_ptr, 01
        ; increment stack pointer
        RETURN
        
pop_s0:
        SUB             stack_ptr, 01h
        ; decrement stack pointer
        load           s0, #stack_ptr
        ; reload register s0 from “stack”
        RETURN