; MDS PicoBlaze IDE - Demonstration code

; Example of sytax errors and list of most used directives
; ------------------------------------------------------
; See manual for more info
;

; Code with syntax errors
        nolist                        ; Disable code listing
if 0
        LOAD       A, #55d, B         ; too many operands
        ADD        0FFh,, 04x4h       ; invalid operands
        ORG        (4 *** 5)          ; invalid expression
label?: SR0        B                  ; invalid label
endif
        list    ; Enable code listing

; <-- Bookmark   (ctrl + shift + M)
; <-- Breakpoint (ctrl + shift + B)
; Pseudo instructions
                        SETB                            s1,5
                        CLRB                            s1,5
; Set bit and clear bit
                        NOTB                            s1,4
; Negation of selected bit
                        SETR                            s8
                        CLRR                            s8
; Set register and clear register
                        CPL2                            s8
; Twos complement
                        CPL                                     s8
; Ones complement
                        DJNZ                            s1,start
; Decrement until zero
                        IJNZ                            s1,loop

; Here you can see syntax of some important directives
; Those directives may significantly improve quality and readability of your code

                DEVICE          KCPSM3
; Tell compiler type of procesor (KCPSM2, KCPSM3, KCPSM6 available)
                INCLUDE      hello.asm        ;  "< file name >"
;Compiler basicaly copies content of included file to line, where the directive is used.
;Include files can include other files.
<symbol>        EQU     <expresion>
;The directive EQU stands for "equals". It allows you to give a numerical value to a symbol.
;This symbol cannot be redefined.
CONSTANT        <symbol>, <expresion>
;The directive CONSTANT allows you to give a numerical value to a symbol. It is same like EQU,
;but with diferent syntax. Cannot be redefined.
 <symbol>       SET     <expresion>
; Directive SET is quite similar to EQU, but symbols defined with the SET(and Variable)
; directive can be redefined with another value in your source code, but those defined with EQU cannot.
VARIABLE        <symbol>,<expresion>
; Directive VARIABLE is similar to SET, but with diferent syntax. Symbols defined with
; VARIABLE can be redefined with another value in your source code, but those defined with EQU cannot.
<symbol>        REG     <expresion>
; Symbols defined with the REG directive are by compiler considered as work registers.
NAMEREG         <symbol>,<expresion>
; Directive NAMEREG is similar to REG, but with diferent syntax.
; Symbols defined with NAMEREG can be redefined with another value in your source code.
<symbol>        DATA    <expresion>
; Symbols defined with the DATA directive are by compiler considered as data memory.
; It must be used only with instructions FETCH and STORE.
<symbol>        CODE    <expresion>
; Symbols defined with the CODE directive are by compiler considered as program memory.
<symbol>        PORT    <expresion>
; Symbol defined with this directive is considered to be PORT_ID identificator.
<symbol>        AUTOREG [AT E]  ; Optional attribute can define counter starting adress
; This directive saves time. You can use it, when you dont care which work register will be used.
; It will automatically assign a register at adress 0x00, which is incremented with every other
; AUTOREG directive. Optionaly, you can change starting address counter by adding a
; parameter after AUTOREG directive. You can check assigned registers in codelisting (file .lst)
; and symbol table (file .sym).
<symbol>        AUTOSPR [AT E]  ; Optional attribute can define counter starting adress

<symbol>        DEFINE  <expresion>
; You can define expresion using previously defined symbols.
; Value of this expresion is calculated every time the symbol is used.

    ORG         <expresion>
ADDRESS         <expresion>
; When the ORG or ADDRESS statement is encountered, the assembler calculates the value of the
; expression and changes the address counter. The MDS assembler maintains a location counter for each
; segment. The location counter contains the offset of the instruction or data being assembled and is
; incremented after each line by the number of bytes of data or code in that line.

REPT <expresion> ; expresion determines number of repetion
<repeated code>
ENDR ; end of repetion

; Directive REPT is able to repeatedly execute a block of code.

WHILE <expresion>
<repeated code>
ENDW ; end of while
; The while construct consists of a block of code and an expresion.
; Following code is executed until expresion equals to zero.

SKIP <expresion> ; expresion determines number of skipped lines of code

MACRO     [<parameter1>] [,<parameter2>..]
<macro source code>
ENDM    ; end of macro declaration
; Macro with mandatory parameters







