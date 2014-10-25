; ----------------------------------------------------------------------------------------------------------------------
; MDS for PicoBlaze - Demonstration code 1: The First Steps
;
; >>> FOR QUICK RESULTS: press F6, then F8, wait a while and then F6 again (to see program simulation).
; ----------------------------------------------------------------------------------------------------------------------
;
; This is a very simple code meant just to show you the first steps. Please pay attention to this demonstration code,
; it will guide you trough the important basic of using this IDE for PicoBlaze soft-core processors. Be aware that this
; example project is read-only; when you are ready to try your own project, please consult the User Manual how to
; proceed with that, or just click on [Main Menu] -> [Project] -> [New Project] and give it a try. :-)
;
; Please proceed with these steps:
;   1) Press F6 to start the simulator (you may also select [Main Menu] -> [Simulation] -> [Start Simulation]).
;   2) Now your program is loaded in simulator, continue with:
;      a) for step by step simulation press F9 (or [Main Menu] -> [Simulation] -> [Step]),
;      b) for animated simulation press F8 (or [Main Menu] -> [Simulation] -> [Animate]), and again to stop animation,
;      c) for fast simulation ("run") press F7 (or [Main Menu] -> [Simulation] -> [Run]), and again to stop,
;      d) for reset press F10 (or [Main Menu] -> [Simulation] -> [Reset]).
;   3) Now you know how to run simulation in various modes, to stop the simulator entirely press F6 (again).
;   4) Lets try breakpoints:
;      a) To set a breakpoint left click on the vertical panel showing line numbers (on the code editor's left side).
;      b) And do it again at the same position to clear the breakpoint.
;      c) Now set some breakpoint on a line with an instruction and run either `animate' (F8) or `run' (F7), the
;         simulation will always stop at your breakpoint but only in modes animate and run.
;      d) Breakpoints can be all switched off and on again without clearing them, see
;         [Main Menu] -> [Simulation] -> [Disable breakpoints] / [Enable breakpoints]
; ----------------------------------------------------------------------------------------------------------------------

; Start at address 0x000
        org     0x000
        jump    start

; Initialize some registers with some values.
start:  load    S0, #11         ; Load register S0 with value 11 decimal.
        load    S1, #0x0B       ; Load register S1 with value B hexadecimal (11 dec.).
        load    S2, #013        ; Load register S2 with value 13 octal (11 dec.).
        load    S3, #0b1011     ; Load register S3 with value 1001 octal (11 dec.).

; An infinite loop.
loop:   st      S0, @S1         ; Store value of register S0 in SPR at address pointed by register S1.
        out     S2, @S3         ; Set value of register S2 on output port at address pointed by register S3.
        cpl     S0              ; Complement register S0.
        ld      S2, S0          ; Load register S2 with value of register S0.
        inc     S1              ; Increment register S1.
        dec     S3              ; Decrement register S3.
        jump    loop            ; Jump to label `loop'.

; Assembler will stop here, everything after the `END' directive is ignored.
        end

; ----------------------------------------------------------------------------------------------------------------------
; Notes:
;   1) To access the User Manual: [Main Menu] -> [Help] -> [Help].
;   1) Compilation: when you need to run assembler, press F5 (or [Main Menu] -> [Project] -> [Compile]).
;   2) Your hex file, and other files you need: look in the folder where you have located your source file; and is
;      possible, please consult the User Manual on the specifics, MDS built-in assembler can generate a lot of different
;      file formats.
;
; Closing remarks:
; * Confused? If yes, don't worry, later in this example project you will see the full power of this "strange looking"
;   assembly language dialect; MDS will show you user defined macro instruction, run time conditions, math expressions,
;   data type sensitivity, and many other innovations. The MDS advanced assembler for PicoBlaze can really make your a
;   lot work easier, when you learn how to use it properly, and it's just an assembler so it can't be too hard to learn.
; ----------------------------------------------------------------------------------------------------------------------
