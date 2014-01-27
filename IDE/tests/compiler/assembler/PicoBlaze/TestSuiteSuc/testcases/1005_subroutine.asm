; Compiler test case for Assembler
; instruction opcodes


device kcpsm1

Start:        
       
    ; Others
                CALl      Podprog1

Podprog1:
                CALl      Podprog2
                RETURN
Podprog2:       CALl      Podprog3
                RETURN
Podprog3:       CALl      Podprog4
                RETURN
Podprog4:       CALl      Podprog5
                RETURN
Podprog5:       CALl      Podprog6
                RETURN
Podprog6:       CALl      Podprog7
                RETURN
Podprog7:       
                RETURN
                
ADD16:

NAMEREG         s0, a_lsb
; rename register s0 as “a_lsb”
NAMEREG         s1, a_msb
; rename register s1 as “a_msb”
NAMEREG         s2, b_lsb
; rename register s2 as “b_lsb”
NAMEREG         s3, b_msb
; rename register s3 as “b_lsb”
ADD             a_lsb, b_lsb
; add LSBs, keep result in a_lsb
ADDCY           a_msb, b_msb
; add MSBs, keep result in a_msb
RETURN

sX              equ             4


inc_16:
                 lo_byte     REG   s8
                hi_byte     REG   s9
        ; increment low byte
        ADD             lo_byte,01
        ; increment high byte only if CARRY bit set when incrementing low byte
        ADDCY           hi_byte,00


Negate:
        ; invert all bits in the register performing a one’s complement
        XOR             sX,#FFh
        ; add one to sX
        ADD             sX,#01
        RETURN




NAMEREG         sF, value
NAMEREG         sE, complement
; Clear ‘complement’ to zero
LOAD            complement, 00
; subtract value from 0 to create two’s complement
SUB             complement, value
RETURN


mult_8x8:
NAMEREG         s0, multiplicand
;
;preserved
NAMEREG         s1, multiplier
; preserved
NAMEREG         s2, bit_mask
; modified
NAMEREG         s3, result_msb
;
;most-significant byte (MSB) of result,
; modified
NAMEREG         s4, result_lsb
; least-significant byte (LSB) of result,
; modified
;
LOAD            bit_mask, 01
; start with least-significant bit (lsb)
LOAD            result_msb, 00
; clear product MSB
LOAD            result_lsb, 00
; clear product LSB (not required)
;
; loop through all bits in multiplier
mult_loop:
TEST            multiplier, bit_mask
; check if bit is set
JUMP            Z, no_add
; if bit is not set, skip addition
;
ADD             result_msb, multiplicand
; addition only occurs in MSB
;
no_add:
SRA             result_msb
; shift MSB right, CARRY into bit 7,
; lsb into CARRY
SRA             result_lsb
; shift LSB right,
; lsb from result_msb into bit 7
;
SL0             bit_mask
; shift bit_mask left to examine
; next bit in multiplier
;
JUMP            NZ, mult_loop
; if all bit examined, then bit_mask = 0,


; Multiplier Routine (8-bit x 8-bit = 16-bit product)
; ===================================================
; Connects to embedded 18x18 Hardware Multiplier via ports
;

;
;least-significant byte (LSB) of result, modified
;
; Define the port ID numbers as constants for better clarity
CONSTANT         multiplier_lsb, 00
CONSTANT         multiplier_msb, 01
;
;
;Output multiplicand and multiplier to FPGA registers connected to the
;inputs of
; the embedded multiplier.
OUTPUT           multiplicand, multiplier_lsb
OUTPUT           multiplier, multiplier_msb
;
; Input the resulting product from the embedded multiplier.
INPUT            result_lsb, multiplier_lsb
INPUT            result_msb, multiplier_msb


; Divide Routine (8-bit / 8-bit = 8-bit result, remainder)
; ==================================================
; Shift and subtract algorithm
;
div_8by8:
NAMEREG          s0, dividend
; preserved
NAMEREG          s1, divisor
; preserved
NAMEREG          s2, quotient
; preserved
NAMEREG          s3, remainder
; modified

; used to test bits in dividend,
; one-hot encoded, modified
;
LOAD             remainder, 00
; clear remainder
LOAD             bit_mask, 80
; start with most-significant bit (msb)
div_loop:
TEST             dividend, bit_mask
; test bit, set CARRY if bit is '1'
SLA              remainder
; shift CARRY into lsb of remainder
SL0              quotient
; shift quotient left (multiply by 2)
;
COMPARE          remainder, divisor
; is remainder > divisor?
JUMP             C, no_sub
; if divisor is greater, continue to next bit
SUB              remainder, divisor
; if remainder > divisor, then subtract
ADD              quotient, 01
; add one to quotient
no_sub:
SR0              bit_mask
; shift to examine next bit position
JUMP             NZ, div_loop
;
;if bit_mask=0, then all bits examined


set_carry:
LOAD             sX, 00
COMPARE          sX, 01
; set CARRY flag and reset ZERO flag


LOAD             s0, 05
; s0 = 00000101
TEST             s0, 04
; mask = 00000100
; CARRY = 1, ZERO = 0