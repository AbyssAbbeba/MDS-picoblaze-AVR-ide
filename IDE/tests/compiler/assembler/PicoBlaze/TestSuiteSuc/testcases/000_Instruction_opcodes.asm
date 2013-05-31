; Compiler test case for Assembler
; instruction opcodes


TESTsymb1	SET		10h
TESTsymb2	SET		0x11    
TESTsymb3	SET		12
TESTsymb4	SET		d13
TESTsymb5	EQU		14h
TESTsymb6	EQU		0x15    
TESTsymb7	EQU		16
TESTsymb8	EQU		d17
TESTsymb9	REG		18h
TESTsymb10	REG		19h
TESTsymb11	REG		1Ah
TESTsymb12	REG		1Bh
TESTsymb13	DATA		1Ch
TESTsymb14	DATA		0x1D
TESTsymb15	DATA		12
TESTsymb16	DATA		d13
TESTsymb17	CODE		10h + 20h
TESTsymb18	CODE		20h 
TESTsymb19	CODE		21h
TESTsymb20	CODE		22h
TESTsymb21	DEFINE		23h
TESTsymb22	DEFINE		24h
TESTsymb23	DEFINE		25h
TESTsymb24	DEFINE		TESTsymb1 + TESTsymb11


	      ORG		0x000
	      JUMP		Start:
	      ADDRESS		0x3FF
	      JUMP		Interrupt:
	      
	      JUMP		0x000			
Start:    							
	      JUMP		Z, 0x000
	      JUMP		NZ,0x000
	      JUMP		C, 0x000
	      JUMP		NC,0x000
	      
	 ; Arithmetics
	      ADD 		testsymb9,#12 
	      ADDCY 		testsymb9,#20
	      SUB 		testsymb9,#ffh 
	      SUBCY 		testsymb9,#0xff 
	      COMPARE 		testsymb9,#0 
	      
	      ADD 		testsymn9,testsymb9 
	      ADDCY 		testsymb9,testsymb9 
	      SUB 		testsymb9,testsymb9
	      SUBCY 		testsymb9,testsymb10 
	      COMPARE 		testsymb9,testsymb11

	; Logical
	      LOAD 		testsymb9,#01h 
	      AND		testsymb9,#0 
	      OR 		testsymb9,#1 
	      XOR 		testsymb9,#2 
	      TEST 		testsymb9,#3 

	      LOAD 		testsymb9,testsymb9 
	      AND 		testsymb9,testsymb9 
	      OR 		testsymb9,testsymb9
	      XOR 		testsymb9,testsymb9 
	      TEST 		testsymb9,testsymb9 
	; Shift and rotate      
	      SR0 		TESTsymb10
	      SR1 		TESTsymb10
	      SRX 		TESTsymb10
	      SRA 		TESTsymb10
	      RR  		TESTsymb10
	      
	      SL0 		TESTsymb10
	      SL1 		TESTsymb10
	      SLX 		TESTsymb10
	      SLA 		TESTsymb10
	      RL 		TESTsymb10
       ; Others
	      CALL		Podprog: 		 
	      CALL 		Z,Podprog: 
	      CALL 		NZ,Podprog: 
	      CALL 		C,Podprog: 
	      CALL 		NC,Podprog: 
	      
	      STORE		TESTsymb11,TESTsymb17 
	      STORE		TESTsymb14,@50
	      FETCH		TESTsymb13,TESTsymb17
	      FETCH		TESTsymb12,@32

Podprog:	      
	      RETURN
	      RETURN		Z
	      RETURN		NZ
	      RETURN		C
	      RETURN		NC


  ;  JUMP aaa ADD sX,kk LOAD sX,kk SR0 sX
;JUMP Z,aaa ADDCY sX,kk AND sX,kk SR1 sX
;JUMP NZ,aaa SUB sX,kk OR sX,kk SRX sX
;JUMP C,aaa SUBCY sX,kk XOR sX,kk SRA sX
;JUMP NC,aaa COMPARE sX,kk TEST sX,kk RR sX
;CALL aaa ADD sX,sY LOAD sX,sY SL0 sX
;CALL Z,aaa ADDCY sX,sY AND sX,sY SL1 sX
;CALL NZ,aaa SUB sX,sY OR sX,sY SLX sX
;CALL C,aaa SUBCY sX,sY XOR sX,sY SLA sX
;CALL NC,aaa COMPARE sX,sY TEST sX,sY RL sX
