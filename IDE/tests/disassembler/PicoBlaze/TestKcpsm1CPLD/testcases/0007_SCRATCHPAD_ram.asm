; Compiler load case for Assembler
; instruction opcodes

device kcpsm1cpld


NAMEREG         s0, ram_EQU
NAMEREG         s1, ram_address

CONSTANT        ram_locations, 40h               ; there are 64 locations
CONSTANT        initial_value, 00h               ; initialize to zero


Start:
        LOAD            ram_EQU, initial_value         ; load initial value
        LOAD            ram_address, ram_locations
        ; fill from top to bottom
ram_fill:
        SUB             ram_address, 01h
        ; decrement address

        
        
        
        ; initialize location
        JUMP            NZ, ram_fill
        ; if not address 0, goto
        ; ram_fill

CONSTANT                switches, 00h
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
; load LED output EQU in register s1
; Load 7-segment LED patterns into scratchpad RAM
        LOAD            LED_output, LED_0

        ; load in RAM[0]
        LOAD            LED_output, LED_1
        ; grab LED pattern for switches = 0001

        ;
        LOAD            LED_output, LED_F
        ; grab LED pattern for switches = 1111

        ; Read switch values and display value on 7-segment LED
loop:
        INPUT           switch_value, switches
        ; read value on switches
        AND             switch_value, 0Fh

        ; look up LED pattern in RAM
        OUTPUT          LED_output, LEDs
        ; display switch value on 7-segment LED
        JUMP            loop




NAMEREG         s7, stack_ptr
; reserve register sF for the stack pointer
; Initialize stack pointer to location 32 in the scratchpad RAM
        LOAD            s7, 20h
my_subroutine:
        ; preserve register s0
        CALL            push_s0
        ; *** remainder of subroutine algorithm ***
        ; reload register s0
        CALL            pop_s0
        RETURN
        
push_s0:
        ; preserve register s0 onto “stack”
        ADD             stack_ptr, 01
        ; increment stack pointer
        RETURN
        
pop_s0:
        SUB             stack_ptr, 01h
        ; reload register s0 from “stack”
        RETURN