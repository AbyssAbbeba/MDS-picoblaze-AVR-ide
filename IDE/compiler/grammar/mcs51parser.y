
// -----------------------------------------------------------------------------
// PROLOGUE - BISON DECLARATIONS
// -----------------------------------------------------------------------------

// Require version <version> or higher of Bison.
%require "2.4.1"
// Rename the external symbols used in the parser so that they start with prefix instead of ‘yy’.
%name-prefix "mcs51parser_"
// Generate a parser header file containing macro definitions, etc.
%defines "mcs51parser.h"
// Generate a pure, reentrant parser.
%define api.pure
// Generate the code processing the locations.
%locations
// Specify the programming language for the generated parser.
%language "C"
// Write an extra output file containing verbose descriptions of the parser states.
%verbose
// Expect exactly 7 shift/reduce conflicts in this grammar
/* %expect 7 */

// Compiler interface for the syntax analyzer; we need to have some things declared
// before we can declare other things, and this interface is quite essential here.
%code requires {
	#include "CompilerParserInterface.h"
}

// Symbol semantic values.
%union {
	int number;
	char * string;
	CompilerExpr * expr;
};

%{
	#include <iostream> 	// This is required by Bison
	using namespace std;	// This is required by Bison as well

	#include <cstdlib>	// We need free() here

	// Declaration of the lexer prototypes and other things required by Bison
	#include "mcs51lexer.h"

	// Name of the lexer function required by Bison
	#define mcs51parser_lex mcs51lexer_lex

	// Declaration of the error reporting function used by Bison
	int mcs51parser_error (
		YYLTYPE * yylloc,
		yyscan_t yyscanner,
		CompilerParserInterface * compiler,
		const char * errorInfo );
%}

// Declare an additional yyparse parameters
%parse-param {yyscan_t yyscanner}
%parse-param {CompilerParserInterface * compiler}

// Declare an additional yylex parameters
%lex-param   {yyscan_t yyscanner}

/*
 * DECLARATION OF THE GRAMMAR APLHABET - TERMINAL SYMBOLS
 */

/* Directives */
%token CSEG

/* Instructions */
%token ACALL	ADD	ADDC	AJMP	ANL	CJNE	CLR	CPL	DA	DEC
%token DIV	DJNZ	INC	JB	JBC	JC	JMP	JNB	JNC	JNZ
%token JZ	LCALL	LJMP	MOV	MOVC	MOVX	MUL	NOP	ORL	POP
%token PUSH	RET	RETI	RL	RLC	RR	RRC	SETB	SJMP	SUBB
%token SWAP	XCH	XCHD	XRL

/* Fixed operands, i.e. those which have no value */
%token INDIRECT_R0	INDIRECT_R1	INDIRECT_A_DPTR	INDIRECT_A_PC	INDIRECT_DPTR
%token REG_A		REG_AB		BIT_C		REG_DPTR	REG_0
%token REG_1		REG_2		REG_3		REG_4		REG_5
%token REG_6		REG_7

/* Other terminal symbols */
%token COMMENT		AT		END_OF_LINE
// Named tokens (to make the grammar rules more readable)
%token IMMEDIATE	"#"
%token LP		"("
%token RP		")"
%token COMMA		","
%token PLUS		"+"
%token MINUS		"-"
%token ASTERISK		"*"
%token SLASH		"/"

/* Operator precedence (the one declared later has the higher precedence) */
%left "+" "-"
%left "*"

/* Terminal symbols with semantic value */
  // semantic value as string
%token<string>		IDENFIFIER	STRING		LABEL
  // semantic value as number
%token<number>		NUMBER

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
%type<expr>		expr		address		immediate
%type<number>		number

// Each time the parser discards symbol with certain semantic types, their memory have to bee freed
/* %destructor { delete $$; } <expr> */
%destructor { free($$); } <string>

// The start symbol
%start input

// -----------------------------------------------------------------------------
// GRAMMAR RULES
// -----------------------------------------------------------------------------
%%

input:
	  line
	| input END_OF_LINE line
;

line:
	  /* empty */
	| directive
	| directive COMMENT
	| instruction_statement
;

instruction_statement:
	  instruction
	| instruction COMMENT
	| LABEL instruction
	| LABEL instruction COMMENT
;

directive:
	  CSEG /* TODO: this is nonsense, so make it right ;-) */
;

instruction:
	  ins_acall
	| ins_add
	| ins_addc
	| ins_ajmp
	| ins_anl
	| ins_cjne
	| ins_clr
	| ins_cpl
	| ins_da
	| ins_dec
	| ins_div
	| ins_djnz
	| ins_inc
	| ins_jb
	| ins_jbc
	| ins_jc
	| ins_jmp
	| ins_jnb
	| ins_jnc
	| ins_jnz
	| ins_jz
	| ins_lcall
	| ins_ljmp
	| ins_mov
	| ins_movc
	| ins_movx
	| ins_mul
	| ins_nop
	| ins_orl
	| ins_pop
	| ins_push
	| ins_ret
	| ins_reti
	| ins_rl
	| ins_rlc
	| ins_rr
	| ins_rrc
	| ins_setb
	| ins_sjmp
	| ins_subb
	| ins_swap
	| ins_xch
	| ins_xchd
	| ins_xrl
;

immediate:
	  "#" expr	{ $$ = $2; }
;

address:
	  expr
;

number:
	  NUMBER	{ $$ = $1; }
	| REG_A		{ $$ = 0xE0; }
	| BIT_C		{ $$ = 0xD7; }
;

expr:
	  IDENFIFIER	{ $$ = new CompilerExpr($1); }
	| number	{ $$ = new CompilerExpr($1); }
	| expr "+" expr	{ $$ = new CompilerExpr($1, '+', $3); }
	| expr "-" expr	{ $$ = new CompilerExpr($1, '-', $3); }
	| expr "*" expr	{ $$ = new CompilerExpr($1, '*', $3); }
	| expr "/" expr	{ $$ = new CompilerExpr($1, '/', $3); }
	| "(" expr ")"	{ $$ = $2; }
;

ins_acall:
	  ACALL address
;
ins_add:
	  ADD REG_A "," immediate
	| ADD REG_A "," address
	| ADD REG_A "," INDIRECT_R0
	| ADD REG_A "," INDIRECT_R1
	| ADD REG_A "," REG_0
	| ADD REG_A "," REG_1
	| ADD REG_A "," REG_2
	| ADD REG_A "," REG_3
	| ADD REG_A "," REG_4
	| ADD REG_A "," REG_5
	| ADD REG_A "," REG_6
	| ADD REG_A "," REG_7
;
ins_addc:
	  ADDC REG_A "," immediate
	| ADDC REG_A "," address
	| ADDC REG_A "," INDIRECT_R0
	| ADDC REG_A "," INDIRECT_R1
	| ADDC REG_A "," REG_0
	| ADDC REG_A "," REG_1
	| ADDC REG_A "," REG_2
	| ADDC REG_A "," REG_3
	| ADDC REG_A "," REG_4
	| ADDC REG_A "," REG_5
	| ADDC REG_A "," REG_6
	| ADDC REG_A "," REG_7
;
ins_ajmp:
	  AJMP address
;
ins_anl:
	  ANL address "," REG_A
	| ANL address "," immediate
	| ANL REG_A "," immediate
	| ANL REG_A "," address
	| ANL REG_A "," INDIRECT_R0
	| ANL REG_A "," INDIRECT_R1
	| ANL REG_A "," REG_0
	| ANL REG_A "," REG_1
	| ANL REG_A "," REG_2
	| ANL REG_A "," REG_3
	| ANL REG_A "," REG_4
	| ANL REG_A "," REG_5
	| ANL REG_A "," REG_6
	| ANL REG_A "," REG_7
	| ANL BIT_C "," address
	| ANL BIT_C "," "/" address
;
ins_cjne:
	  CJNE REG_A "," immediate "," address
	| CJNE REG_A "," address "," address
	| CJNE INDIRECT_R0 "," immediate "," address
	| CJNE INDIRECT_R1 "," immediate "," address
	| CJNE REG_0 "," immediate "," address
	| CJNE REG_1 "," immediate "," address
	| CJNE REG_2 "," immediate "," address
	| CJNE REG_3 "," immediate "," address
	| CJNE REG_4 "," immediate "," address
	| CJNE REG_5 "," immediate "," address
	| CJNE REG_6 "," immediate "," address
	| CJNE REG_7 "," immediate "," address
;
ins_clr:
	  CLR address
	| CLR BIT_C
	| CLR REG_A
;
ins_cpl:
	  CPL address
	| CPL BIT_C
	| CPL REG_A
;
ins_da:
	  DA REG_A
;
ins_dec:
	  DEC address
	| DEC REG_A
	| DEC INDIRECT_R0
	| DEC INDIRECT_R1
	| DEC REG_0
	| DEC REG_1
	| DEC REG_2
	| DEC REG_3
	| DEC REG_4
	| DEC REG_5
	| DEC REG_6
	| DEC REG_7
;
ins_div:
	  DIV REG_AB
;
ins_djnz:
	  DJNZ address "," address
	| DJNZ REG_0 "," address
	| DJNZ REG_1 "," address
	| DJNZ REG_2 "," address
	| DJNZ REG_3 "," address
	| DJNZ REG_4 "," address
	| DJNZ REG_5 "," address
	| DJNZ REG_6 "," address
	| DJNZ REG_7 "," address
;
ins_inc:
	  INC REG_A
	| INC address
	| INC INDIRECT_R0
	| INC INDIRECT_R1
	| INC REG_0
	| INC REG_1
	| INC REG_2
	| INC REG_3
	| INC REG_4
	| INC REG_5
	| INC REG_6
	| INC REG_7
	| INC REG_DPTR
;
ins_jb:
	  JB address "," address
;
ins_jbc:
	  JBC address "," address
;
ins_jc:
	  JC address
;
ins_jmp:
	  JMP INDIRECT_A_DPTR
	| JMP address
;
ins_jnb:
	  JNB address
;
ins_jnc:
	  JNC address
;
ins_jnz:
	  JNZ address
;
ins_jz:
	  JZ address
;
ins_lcall:
	  LCALL address
;
ins_ljmp:
	  LJMP address
;
ins_mov:
	  MOV address "," address
	| MOV address "," immediate	{ std::cout << "MOV: " << $2 << ", " << $4; }
	| MOV address "," INDIRECT_R0
	| MOV address "," INDIRECT_R1
	| MOV address "," REG_0
	| MOV address "," REG_1
	| MOV address "," REG_2
	| MOV address "," REG_3
	| MOV address "," REG_4
	| MOV address "," REG_5
	| MOV address "," REG_6
	| MOV address "," REG_7
	| MOV address "," BIT_C
	| MOV address "," REG_A
	| MOV INDIRECT_R0 "," immediate
	| MOV INDIRECT_R1 "," immediate
	| MOV INDIRECT_R0 "," address
	| MOV INDIRECT_R1 "," address
	| MOV INDIRECT_R0 "," REG_A
	| MOV INDIRECT_R1 "," REG_A
	| MOV REG_DPTR "," immediate
	| MOV REG_0 "," immediate
	| MOV REG_1 "," immediate
	| MOV REG_2 "," immediate
	| MOV REG_3 "," immediate
	| MOV REG_4 "," immediate
	| MOV REG_5 "," immediate
	| MOV REG_6 "," immediate
	| MOV REG_7 "," immediate
	| MOV REG_0 "," address
	| MOV REG_1 "," address
	| MOV REG_2 "," address
	| MOV REG_3 "," address
	| MOV REG_4 "," address
	| MOV REG_5 "," address
	| MOV REG_6 "," address
	| MOV REG_7 "," address
	| MOV REG_0 "," REG_A
	| MOV REG_1 "," REG_A
	| MOV REG_2 "," REG_A
	| MOV REG_3 "," REG_A
	| MOV REG_4 "," REG_A
	| MOV REG_5 "," REG_A
	| MOV REG_6 "," REG_A
	| MOV REG_7 "," REG_A
	| MOV REG_A "," immediate
	| MOV REG_A "," INDIRECT_R0
	| MOV REG_A "," INDIRECT_R1
	| MOV REG_A "," REG_0
	| MOV REG_A "," REG_1
	| MOV REG_A "," REG_2
	| MOV REG_A "," REG_3
	| MOV REG_A "," REG_4
	| MOV REG_A "," REG_5
	| MOV REG_A "," REG_6
	| MOV REG_A "," REG_7
	| MOV REG_A "," address
	| MOV BIT_C "," address
;
ins_movc:
	  MOVC REG_A "," INDIRECT_A_DPTR
	| MOVC REG_A "," INDIRECT_A_PC
;
ins_movx:
	  MOVX INDIRECT_DPTR "," REG_A
	| MOVX INDIRECT_R0 "," REG_A
	| MOVX INDIRECT_R1 "," REG_A
	| MOVX REG_A "," INDIRECT_DPTR
	| MOVX REG_A "," INDIRECT_R0
	| MOVX REG_A "," INDIRECT_R1
;
ins_mul:
	  MUL REG_AB
;
ins_nop:
	  NOP
;
ins_orl:
	  ORL address "," REG_A
	| ORL address "," immediate
	| ORL REG_A "," immediate
	| ORL REG_A "," address
	| ORL REG_A "," INDIRECT_R0
	| ORL REG_A "," INDIRECT_R1
	| ORL REG_A "," REG_0
	| ORL REG_A "," REG_1
	| ORL REG_A "," REG_2
	| ORL REG_A "," REG_3
	| ORL REG_A "," REG_4
	| ORL REG_A "," REG_5
	| ORL REG_A "," REG_6
	| ORL REG_A "," REG_7
	| ORL BIT_C "," address
	| ORL BIT_C "," "/" address
;
ins_pop:
	  POP address
;
ins_push:
	  PUSH address
;
ins_ret:
	  RET
;
ins_reti:
	  RETI
;
ins_rl:
	  RL REG_A
;
ins_rlc:
	  RLC REG_A
;
ins_rr:
	  RR REG_A
;
ins_rrc:
	  RRC REG_A
;
ins_setb:
	  SETB BIT_C
	| SETB address
;
ins_sjmp:
	  SJMP address
;
ins_subb:
	  SUBB REG_A "," immediate
	| SUBB REG_A "," address
	| SUBB REG_A "," INDIRECT_R0
	| SUBB REG_A "," INDIRECT_R1
	| SUBB REG_A "," REG_0
	| SUBB REG_A "," REG_1
	| SUBB REG_A "," REG_2
	| SUBB REG_A "," REG_3
	| SUBB REG_A "," REG_4
	| SUBB REG_A "," REG_5
	| SUBB REG_A "," REG_6
	| SUBB REG_A "," REG_7
;
ins_swap:
	  SWAP REG_A
;
ins_xch:
	  XCH REG_A "," INDIRECT_R0
	| XCH REG_A "," INDIRECT_R1
	| XCH REG_A "," REG_0
	| XCH REG_A "," REG_1
	| XCH REG_A "," REG_2
	| XCH REG_A "," REG_3
	| XCH REG_A "," REG_4
	| XCH REG_A "," REG_5
	| XCH REG_A "," REG_6
	| XCH REG_A "," REG_7
	| XCH REG_A "," address
;
ins_xchd:
	  XCHD REG_A "," INDIRECT_R0
	| XCHD REG_A "," INDIRECT_R1
;
ins_xrl:
	  XRL address "," REG_A
	| XRL address "," immediate
	| XRL REG_A "," immediate
	| XRL REG_A "," address
	| XRL REG_A "," INDIRECT_R0
	| XRL REG_A "," INDIRECT_R1
	| XRL REG_A "," REG_0
	| XRL REG_A "," REG_1
	| XRL REG_A "," REG_2
	| XRL REG_A "," REG_3
	| XRL REG_A "," REG_4
	| XRL REG_A "," REG_5
	| XRL REG_A "," REG_6
	| XRL REG_A "," REG_7
;

%%
// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison
int mcs51parser_error(YYLTYPE * yylloc, yyscan_t, CompilerParserInterface * compiler, const char * errorInfo) {
	compiler->parserMessage(yylloc, CompilerBase::MT_ERROR, errorInfo);
	return 0;
}
