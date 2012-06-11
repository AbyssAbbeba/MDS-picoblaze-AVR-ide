
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
// Expect exactly 7 reduce/reduce conflicts in this grammar
/* %expect-rr 7 */

/* 
 * Compiler interface for the syntax analyzer; we need to have some things declared
 * before we can declare other things, and this interface is quite essential here.
 */
%code requires {
	#include "CompilerParserInterface.h"
}

// Symbol semantic values.
%union {
	int number;
	char * string;
	CompilerExpr * expr;
	CompilerStatement * stmt;
	struct {
		unsigned char * data;
		int size;
	} array;
};

%{
	#include <QObject>		// For i18n, nothing else.
	#include <cstdlib>		// We need free() here.
	#include <iostream> 		// This is required by Bison.
	using namespace std;		// This is required by Bison as well.
	using namespace StatementTypes;	// This NS is heavily used here.

	// Declaration of the lexer prototypes and other things required by Bison
	#include "mcs51lexer.h"

	// Name of the lexer function required by Bison
	#define mcs51parser_lex mcs51lexer_lex

	// Make a few things a little easier...
	#define LOC() \
		( compiler->toSourceLocation(yylloc) )

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
%token DB	MACRO	ENDM

/* Instructions */
%token ACALL	ADD	ADDC	AJMP	ANL	CJNE	CLR	CPL	DA	DEC
%token DIV	DJNZ	INC	JB	JBC	JC	JMP	JNB	JNC	JNZ
%token JZ	LCALL	LJMP	MOV	MOVC	MOVX	MUL	NOP	ORL	POP
%token PUSH	RET	RETI	RL	RLC	RR	RRC	SETB	SJMP	SUBB
%token SWAP	XCH	XCHD	XRL

/* Fixed operands, i.e. those which have no value */
%token IND_R0		IND_R1		IND_A_DPTR	IND_A_PC	IND_DPTR
%token REG_A		REG_AB		BIT_C		REG_DPTR	REG_0
%token REG_1		REG_2		REG_3		REG_4		REG_5
%token REG_6		REG_7

/* Other terminal symbols */
%token COMMENT		AT		EOL
// Named tokens (to make the grammar rules more readable)
%token IMMEDIATE	"#"
%token LP		"("
%token RP		")"
%token COMMA		","
%token PLUS		"+"
%token MINUS		"-"
%token ASTERISK		"*"
%token SLASH		"/"
%token DOT		"."

// Nicer error messages referring to “end of file” instead of “$end”
%token END 0 "end of file"

/* Operator precedence (the one declared later has the higher precedence) */
%left "+" "-"
%left "*"
%left "."
%left ","

/* Terminal symbols with semantic value */
  // semantic value is a string
%token<string>	IDENFIFIER	LABEL
%token<array>	STRING
  // semantic value is a number
%token<number>	NUMBER

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Integers
/* %type<number> */
// Expressions
%type<expr>	expr		id		string		args_str	e_expr
%type<expr>	number		params		args		const_a		const_c
// Statements - general
%type<stmt>	statements	stmt		inst_stmt	dir_stmt	macro_stmt
%type<stmt>	instruction	directive	macro		label
// Statements - directives
%type<stmt>	dir_macro_d	dir_macro	dir_db
// Statements - instructions
%type<stmt>	ins_acall	ins_add		ins_addc	ins_ajmp	ins_anl
%type<stmt>	ins_cjne	ins_clr		ins_cpl		ins_da		ins_dec
%type<stmt>	ins_div		ins_djnz	ins_inc		ins_jb		ins_jbc
%type<stmt>	ins_jc		ins_jmp		ins_jnb		ins_jnc		ins_jnz
%type<stmt>	ins_jz		ins_lcall	ins_ljmp	ins_mov		ins_movc
%type<stmt>	ins_movx	ins_mul		ins_nop		ins_orl		ins_pop
%type<stmt>	ins_push	ins_ret		ins_reti	ins_rl		ins_rlc
%type<stmt>	ins_rr		ins_rrc		ins_setb	ins_sjmp	ins_subb
%type<stmt>	ins_swap	ins_xch		ins_xchd	ins_xrl


// Each time the parser discards symbol with certain semantic types, their memory have to bee freed
%destructor {
	if ( NULL != $$ ) {
		$$->first()->completeDelete();
	}
} <expr>
%destructor {
	if ( NULL != $$ ) {
		$$->first()->completeDelete();
	}
} <stmt>
%destructor {
	free($$.data);
} <array>

// The start symbol
%start input


// -----------------------------------------------------------------------------
// GRAMMAR RULES
// -----------------------------------------------------------------------------

%%

/*
 * Basic code structure
 */
input:
	statements			{ compiler->syntaxAnalysisComplete($1); }
;
statements:
	  stmt				{ $$ = $1; }
	| statements EOL stmt		{
						if ( NULL == $1 ) {
							$$ = $3;
						} else {
							$$ = $1->addLink($3);
						}
					}
;
stmt:
	  /* empty */			{ $$ = NULL; }
	| COMMENT			{ $$ = NULL; }
	| label				{ $$ = $1; }
	| label COMMENT			{ $$ = $1; }
	| dir_stmt			{ $$ = $1; }
	| inst_stmt			{ $$ = $1; }
	| macro_stmt			{ $$ = $1; }
;
dir_stmt:
	  directive			{ $$ = $1; }
	| directive COMMENT		{ $$ = $1; }
;
inst_stmt:
	  instruction			{ $$ = $1; }
	| instruction COMMENT		{ $$ = $1; }
	| label instruction		{ $$ = $1->addLink($2); }
	| label instruction COMMENT	{ $$ = $1->addLink($2); }
;
macro_stmt:
	  macro				{ $$ = $1; }
	| macro COMMENT			{ $$ = $1; }
	| label macro			{ $$ = $1->addLink($2); }
	| label macro COMMENT		{ $$ = $1->addLink($2); }
;
macro:
	  id				{ $$ = new CompilerStatement(LOC(), ASM51_MACRO, $1); }
	| id "(" ")"			{ $$ = new CompilerStatement(LOC(), ASM51_MACRO, $1); }
	| id args			{ $$ = new CompilerStatement(LOC(), ASM51_MACRO, $1->addLink($2)); }
	| id "(" args ")"		{ $$ = new CompilerStatement(LOC(), ASM51_MACRO, $1->addLink($3)); }
;
instruction:
	  ins_acall	{ $$ = $1; }	| ins_add	{ $$ = $1; }
	| ins_addc	{ $$ = $1; }	| ins_ajmp	{ $$ = $1; }
	| ins_anl	{ $$ = $1; }	| ins_cjne	{ $$ = $1; }
	| ins_clr	{ $$ = $1; }	| ins_cpl	{ $$ = $1; }
	| ins_da	{ $$ = $1; }	| ins_dec	{ $$ = $1; }
	| ins_div	{ $$ = $1; }	| ins_djnz	{ $$ = $1; }
	| ins_inc	{ $$ = $1; }	| ins_jb	{ $$ = $1; }
	| ins_jbc	{ $$ = $1; }	| ins_jc	{ $$ = $1; }
	| ins_jmp	{ $$ = $1; }	| ins_jnb	{ $$ = $1; }
	| ins_jnc	{ $$ = $1; }	| ins_jnz	{ $$ = $1; }
	| ins_jz	{ $$ = $1; }	| ins_lcall	{ $$ = $1; }
	| ins_ljmp	{ $$ = $1; }	| ins_mov	{ $$ = $1; }
	| ins_movc	{ $$ = $1; }	| ins_movx	{ $$ = $1; }
	| ins_mul	{ $$ = $1; }	| ins_nop	{ $$ = $1; }
	| ins_orl	{ $$ = $1; }	| ins_pop	{ $$ = $1; }
	| ins_push	{ $$ = $1; }	| ins_ret	{ $$ = $1; }
	| ins_reti	{ $$ = $1; }	| ins_rl	{ $$ = $1; }
	| ins_rlc	{ $$ = $1; }	| ins_rr	{ $$ = $1; }
	| ins_rrc	{ $$ = $1; }	| ins_setb	{ $$ = $1; }
	| ins_sjmp	{ $$ = $1; }	| ins_subb	{ $$ = $1; }
	| ins_swap	{ $$ = $1; }	| ins_xch	{ $$ = $1; }
	| ins_xchd	{ $$ = $1; }	| ins_xrl	{ $$ = $1; }
;
directive:
	  dir_db			{ $$ = $1; }
	| dir_macro			{ $$ = $1; }
;

/*
 * Generally used non-terminal symbols
 */
number:
	  NUMBER			{ $$ = new CompilerExpr($1); }
;
id:
	  IDENFIFIER			{ $$ = new CompilerExpr($1); }
;
string:
	  STRING			{ $$ = new CompilerExpr(CompilerExpr::Value($1.data, $1.size)); }
;
label:
	  LABEL				{ $$ = new CompilerStatement(LOC(), ASM51_LABEL, new CompilerExpr($1)); }
;
const_a:
	REG_A	{
		$$ = new CompilerExpr("A");
		compiler->parserMessage (
			compiler->toSourceLocation(@1),
			CompilerBase::MT_REMARK,
			QObject::tr("Using keyword `A' as constant (0xE0).").toStdString() );
	}
;
const_c:
	BIT_C	{
		$$ = new CompilerExpr("C");
		compiler->parserMessage (
			compiler->toSourceLocation(@1),
			CompilerBase::MT_REMARK,
			QObject::tr("Using keyword `C' as constant (0xD7).").toStdString() );
	}
;
expr:
	  id				{ $$ = $1; }
	| number			{ $$ = $1; }
	| "(" expr ")"			{ $$ = $2; }
	| expr "+" expr			{ $$ = new CompilerExpr($1, '+', $3); }
	| expr "-" expr			{ $$ = new CompilerExpr($1, '-', $3); }
	| expr "*" expr			{ $$ = new CompilerExpr($1, '*', $3); }
	| expr "/" expr			{ $$ = new CompilerExpr($1, '/', $3); }
	| expr "." expr			{ $$ = new CompilerExpr($1, '.', $3); }

	/* Allow `A' to be used as a constant */
	| const_a "+" expr		{ $$ = new CompilerExpr($1, '+', $3); }
	| const_a "-" expr		{ $$ = new CompilerExpr($1, '-', $3); }
	| const_a "*" expr		{ $$ = new CompilerExpr($1, '*', $3); }
	| const_a "/" expr		{ $$ = new CompilerExpr($1, '/', $3); }
	| const_a "." expr		{ $$ = new CompilerExpr($1, '.', $3); }
	| expr "+" const_a		{ $$ = new CompilerExpr($1, '+', $3); }
	| expr "-" const_a		{ $$ = new CompilerExpr($1, '-', $3); }
	| expr "*" const_a		{ $$ = new CompilerExpr($1, '*', $3); }
	| expr "/" const_a		{ $$ = new CompilerExpr($1, '/', $3); }
	| expr "." const_a		{ $$ = new CompilerExpr($1, '.', $3); }

	/* Allow `C' to be used as a constant */
	| const_c "+" expr		{ $$ = new CompilerExpr($1, '+', $3); }
	| const_c "-" expr		{ $$ = new CompilerExpr($1, '-', $3); }
	| const_c "*" expr		{ $$ = new CompilerExpr($1, '*', $3); }
	| const_c "/" expr		{ $$ = new CompilerExpr($1, '/', $3); }
	| const_c "." expr		{ $$ = new CompilerExpr($1, '.', $3); }
	| expr "+" const_c		{ $$ = new CompilerExpr($1, '+', $3); }
	| expr "-" const_c		{ $$ = new CompilerExpr($1, '-', $3); }
	| expr "*" const_c		{ $$ = new CompilerExpr($1, '*', $3); }
	| expr "/" const_c		{ $$ = new CompilerExpr($1, '/', $3); }
	| expr "." const_c		{ $$ = new CompilerExpr($1, '.', $3); }
;
e_expr:		// Expression allowing use of single `A' or `C', e.g. `A', this cannot be used with instructions
	  expr				{ $$ = $1; }
	| const_a			{ $$ = $1; }
	| const_c			{ $$ = $1; }
;
args:		// List of arguments without strings, e.g. `(1+3), XYZ, 0x4b'
	  args "," e_expr		{ $$ = $1->addLink($3); }
	| e_expr			{ $$ = $1; }
;
args_str:	// List of arguments possibly with strings, e.g. `ABC, "XYZ - QWE", 2 * 0x4b'
	  args_str "," string		{ $$ = $1->addLink($3); }
	| args_str "," e_expr		{ $$ = $1->addLink($3); }
	| string			{ $$ = $1; }
	| e_expr			{ $$ = $1; }
;
params:		// List of identifiers, e.g. `target, source, offset, size, mode'
	  params "," id			{ $$ = $1->addLink($3); }
	| id				{ $$ = $1; }
;

/*
 * Directives
 */
dir_macro:
	  dir_macro_d
	  statements EOL
	  endm				{ $$ = $1->createBranch($2); }
	| dir_macro_d
	  endm				{ $$ = $1; }
;
dir_macro_d:
	  id MACRO EOL			{ $$ = new CompilerStatement(LOC(), ASM51_DIR_MACRO, $1); }
	| id MACRO COMMENT EOL		{ $$ = new CompilerStatement(LOC(), ASM51_DIR_MACRO, $1); }
	| id MACRO params EOL 		{ $$ = new CompilerStatement(LOC(), ASM51_DIR_MACRO, $1->addLink($3)); }
	| id MACRO params COMMENT EOL	{ $$ = new CompilerStatement(LOC(), ASM51_DIR_MACRO, $1->addLink($3)); }
;
endm:
	  ENDM COMMENT
	| ENDM
;
dir_db:
	  DB args_str			{ $$ = new CompilerStatement(LOC(), ASM51_DIR_DB, $2); }
	| label DB args_str		{ $$ = $1->addLink(new CompilerStatement(LOC(), ASM51_DIR_DB, $3)); }
;

/*
 * Instructions
 */
ins_acall:
	  ACALL e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ACALL, $2); }
;
ins_add:
	  ADD REG_A "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADD__A_IMM, $5); }
	| ADD REG_A "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADD__A_DIR, $4); }
	| ADD REG_A "," IND_R0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADD__A_IR0); }
	| ADD REG_A "," IND_R1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADD__A_IR1); }
	| ADD REG_A "," REG_0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADD__A_R0); }
	| ADD REG_A "," REG_1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADD__A_R1); }
	| ADD REG_A "," REG_2		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADD__A_R2); }
	| ADD REG_A "," REG_3		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADD__A_R3); }
	| ADD REG_A "," REG_4		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADD__A_R4); }
	| ADD REG_A "," REG_5		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADD__A_R5); }
	| ADD REG_A "," REG_6		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADD__A_R6); }
	| ADD REG_A "," REG_7		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADD__A_R7); }
;
ins_addc:
	  ADDC REG_A "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADDC__A_IMM, $5); }
	| ADDC REG_A "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADDC__A_DIR, $4); }
	| ADDC REG_A "," IND_R0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADDC__A_IR0); }
	| ADDC REG_A "," IND_R1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADDC__A_IR1); }
	| ADDC REG_A "," REG_0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADDC__A_R0); }
	| ADDC REG_A "," REG_1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADDC__A_R1); }
	| ADDC REG_A "," REG_2		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADDC__A_R2); }
	| ADDC REG_A "," REG_3		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADDC__A_R3); }
	| ADDC REG_A "," REG_4		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADDC__A_R4); }
	| ADDC REG_A "," REG_5		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADDC__A_R5); }
	| ADDC REG_A "," REG_6		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADDC__A_R6); }
	| ADDC REG_A "," REG_7		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ADDC__A_R7); }
;
ins_ajmp:
	  AJMP e_expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_AJMP__DIR, $2); }
;
ins_anl:
	  ANL expr "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__DIR_A, $2); }
	| ANL expr "," "#" e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__DIR_IMM, $2->addLink($5)); }
	| ANL REG_A "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__A_IMM, $5); }
	| ANL REG_A "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__A_DIR, $4); }
	| ANL REG_A "," IND_R0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__A_IR0); }
	| ANL REG_A "," IND_R1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__A_IR1); }
	| ANL REG_A "," REG_0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__A_R0); }
	| ANL REG_A "," REG_1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__A_R1); }
	| ANL REG_A "," REG_2		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__A_R2); }
	| ANL REG_A "," REG_3		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__A_R3); }
	| ANL REG_A "," REG_4		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__A_R4); }
	| ANL REG_A "," REG_5		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__A_R5); }
	| ANL REG_A "," REG_6		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__A_R6); }
	| ANL REG_A "," REG_7		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__A_R7); }
	| ANL BIT_C "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__C_DIR, $4); }
	| ANL BIT_C "," "/" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_ANL__C_SL_DIR, $5); }
;
ins_cjne:
	  CJNE REG_A  ","     e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_CJNE__A_DIR_DIR, $4->addLink($6)); }
	| CJNE REG_A  "," "#" e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_CJNE__A_IMM_DIR, $5->addLink($7)); }
	| CJNE IND_R0 "," "#" e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_CJNE__IR0_IMM_DIR, $5->addLink($7)); }
	| CJNE IND_R1 "," "#" e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_CJNE__IR1_IMM_DIR, $5->addLink($7)); }
	| CJNE REG_0  "," "#" e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_CJNE__R0_IMM_DIR, $5->addLink($7)); }
	| CJNE REG_1  "," "#" e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_CJNE__R1_IMM_DIR, $5->addLink($7)); }
	| CJNE REG_2  "," "#" e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_CJNE__R2_IMM_DIR, $5->addLink($7)); }
	| CJNE REG_3  "," "#" e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_CJNE__R3_IMM_DIR, $5->addLink($7)); }
	| CJNE REG_4  "," "#" e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_CJNE__R4_IMM_DIR, $5->addLink($7)); }
	| CJNE REG_5  "," "#" e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_CJNE__R5_IMM_DIR, $5->addLink($7)); }
	| CJNE REG_6  "," "#" e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_CJNE__R6_IMM_DIR, $5->addLink($7)); }
	| CJNE REG_7  "," "#" e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_CJNE__R7_IMM_DIR, $5->addLink($7)); }
;
ins_clr:
	  CLR expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_CLR__DIR, $2); }
	| CLR BIT_C			{ $$ = new CompilerStatement(LOC(), ASM51_INS_CLR__C); }
	| CLR REG_A			{ $$ = new CompilerStatement(LOC(), ASM51_INS_CLR__A); }
;
ins_cpl:
	  CPL expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_CPL__DIR, $2); }
	| CPL BIT_C			{ $$ = new CompilerStatement(LOC(), ASM51_INS_CPL__C); }
	| CPL REG_A			{ $$ = new CompilerStatement(LOC(), ASM51_INS_CPL__A); }
;
ins_da:
	  DA REG_A			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DA__A); }
;
ins_dec:
	  DEC expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__DIR, $2); }
	| DEC const_c			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__DIR, $2); }
	| DEC REG_A			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__A); }
	| DEC IND_R0			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__IR0); }
	| DEC IND_R1			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__IR1); }
	| DEC REG_0			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__R0); }
	| DEC REG_1			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__R1); }
	| DEC REG_2			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__R2); }
	| DEC REG_3			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__R3); }
	| DEC REG_4			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__R4); }
	| DEC REG_5			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__R5); }
	| DEC REG_6			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__R6); }
	| DEC REG_7			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DEC__R7); }
;
ins_div:
	  DIV REG_AB			{ $$ = new CompilerStatement(LOC(), ASM51_INS_DIV__AB); }
;
ins_djnz:
	  DJNZ e_expr "," e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_DJNZ__DIR_DIR, $2->addLink($4)); }
	| DJNZ REG_0 "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_DJNZ__R0_DIR, $4); }
	| DJNZ REG_1 "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_DJNZ__R1_DIR, $4); }
	| DJNZ REG_2 "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_DJNZ__R2_DIR, $4); }
	| DJNZ REG_3 "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_DJNZ__R3_DIR, $4); }
	| DJNZ REG_4 "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_DJNZ__R4_DIR, $4); }
	| DJNZ REG_5 "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_DJNZ__R5_DIR, $4); }
	| DJNZ REG_6 "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_DJNZ__R6_DIR, $4); }
	| DJNZ REG_7 "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_DJNZ__R7_DIR, $4); }
;
ins_inc:
	  INC REG_A			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__A); }
	| INC expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__DIR, $2); }
	| INC const_c			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__DIR, $2); }
	| INC IND_R0			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__IR0); }
	| INC IND_R1			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__IR1); }
	| INC REG_0			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__R0); }
	| INC REG_1			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__R1); }
	| INC REG_2			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__R2); }
	| INC REG_3			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__R3); }
	| INC REG_4			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__R4); }
	| INC REG_5			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__R5); }
	| INC REG_6			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__R6); }
	| INC REG_7			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__R7); }
	| INC REG_DPTR			{ $$ = new CompilerStatement(LOC(), ASM51_INS_INC__DPTR); }
;
ins_jb:
	  JB e_expr "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_JB__DIR_DIR, $2->addLink($4)); }
;
ins_jbc:
	  JBC e_expr "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_JBC__DIR_DIR, $2->addLink($4)); }
;
ins_jc:
	  JC e_expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_JC__DIR, $2); }
;
ins_jmp:
	  JMP IND_A_DPTR		{ $$ = new CompilerStatement(LOC(), ASM51_INS_JMP__IA_DPTR); }
	| JMP e_expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_JMP__DIR, $2); }
;
ins_jnb:
	  JNB e_expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_JNB__DIR, $2); }
;
ins_jnc:
	  JNC e_expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_JNC__DIR, $2); }
;
ins_jnz:
	  JNZ e_expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_JNZ__DIR, $2); }
;
ins_jz:
	  JZ e_expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_JZ__DIR, $2); }
;
ins_lcall:
	  LCALL e_expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_LCALL__DIR, $2); }
;
ins_ljmp:
	  LJMP e_expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_LJMP__DIR, $2); }
;
ins_mov:
	  MOV expr "," expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_DIR, $2->addLink($4)); }
	| MOV expr "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_IMM, $2->addLink($5)); }
	| MOV expr "," IND_R0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_IR0, $2); }
	| MOV expr "," IND_R1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_IR1, $2); }
	| MOV expr "," REG_0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_R0, $2); }
	| MOV expr "," REG_1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_R1, $2); }
	| MOV expr "," REG_2		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_R2, $2); }
	| MOV expr "," REG_3		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_R3, $2); }
	| MOV expr "," REG_4		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_R4, $2); }
	| MOV expr "," REG_5		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_R5, $2); }
	| MOV expr "," REG_6		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_R6, $2); }
	| MOV expr "," REG_7		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_R7, $2); }
	| MOV expr "," BIT_C		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_C, $2); }
	| MOV expr "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DIR_A, $2); }
	| MOV IND_R0 "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__IR0_IMM, $5); }
	| MOV IND_R1 "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__IR1_IMM, $5); }
	| MOV IND_R0 "," expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__IR0_DIR, $4); }
	| MOV IND_R1 "," expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__IR1_DIR, $4); }
	| MOV IND_R0 "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__IR0_A); }
	| MOV IND_R1 "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__IR1_A); }
	| MOV REG_DPTR "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__DPTR_IMM, $5); }
	| MOV REG_0 "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R0_IMM, $5); }
	| MOV REG_1 "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R1_IMM, $5); }
	| MOV REG_2 "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R2_IMM, $5); }
	| MOV REG_3 "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R3_IMM, $5); }
	| MOV REG_4 "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R4_IMM, $5); }
	| MOV REG_5 "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R5_IMM, $5); }
	| MOV REG_6 "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R6_IMM, $5); }
	| MOV REG_7 "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R7_IMM, $5); }
	| MOV REG_0 "," expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R0_DIR, $4); }
	| MOV REG_1 "," expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R1_DIR, $4); }
	| MOV REG_2 "," expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R2_DIR, $4); }
	| MOV REG_3 "," expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R3_DIR, $4); }
	| MOV REG_4 "," expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R4_DIR, $4); }
	| MOV REG_5 "," expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R5_DIR, $4); }
	| MOV REG_6 "," expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R6_DIR, $4); }
	| MOV REG_7 "," expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R7_DIR, $4); }
	| MOV REG_0 "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R0_A); }
	| MOV REG_1 "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R1_A); }
	| MOV REG_2 "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R2_A); }
	| MOV REG_3 "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R3_A); }
	| MOV REG_4 "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R4_A); }
	| MOV REG_5 "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R5_A); }
	| MOV REG_6 "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R6_A); }
	| MOV REG_7 "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__R7_A); }
	| MOV REG_A "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__A_IMM, $5); }
	| MOV REG_A "," IND_R0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__A_IR0); }
	| MOV REG_A "," IND_R1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__A_IR1); }
	| MOV REG_A "," REG_0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__A_R0); }
	| MOV REG_A "," REG_1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__A_R1); }
	| MOV REG_A "," REG_2		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__A_R2); }
	| MOV REG_A "," REG_3		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__A_R3); }
	| MOV REG_A "," REG_4		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__A_R4); }
	| MOV REG_A "," REG_5		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__A_R5); }
	| MOV REG_A "," REG_6		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__A_R6); }
	| MOV REG_A "," REG_7		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__A_R7); }
	| MOV REG_A "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__A_DIR, $4); }
	| MOV BIT_C "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOV__C_DIR, $4); }
;
ins_movc:
	  MOVC REG_A "," IND_A_DPTR	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOVC__A_IA_DPTR); }
	| MOVC REG_A "," IND_A_PC	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOVC__A_IA_PC); }
;
ins_movx:
	  MOVX IND_DPTR "," REG_A	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOVX__IDPTR_A); }
	| MOVX IND_R0 "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOVX__IR0_A); }
	| MOVX IND_R1 "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOVX__IR1_A); }
	| MOVX REG_A "," IND_DPTR	{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOVX__A_IDPTR); }
	| MOVX REG_A "," IND_R0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOVX__A_IR0); }
	| MOVX REG_A "," IND_R1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_MOVX__A_IR1); }
;
ins_mul:
	  MUL REG_AB			{ $$ = new CompilerStatement(LOC(), ASM51_INS_MUL__AB); }
;
ins_nop:
	  NOP				{ $$ = new CompilerStatement(LOC(), ASM51_INS_NOP); }
;
ins_orl:
	  ORL expr "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__DIR_A, $2); }
	| ORL expr "," "#" e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__DIR_IMM, $2->addLink($5)); }
	| ORL REG_A "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__A_IMM, $5); }
	| ORL REG_A "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__A_DIR, $4); }
	| ORL REG_A "," IND_R0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__A_IR0); }
	| ORL REG_A "," IND_R1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__A_IR1); }
	| ORL REG_A "," REG_0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__A_R0); }
	| ORL REG_A "," REG_1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__A_R1); }
	| ORL REG_A "," REG_2		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__A_R2); }
	| ORL REG_A "," REG_3		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__A_R3); }
	| ORL REG_A "," REG_4		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__A_R4); }
	| ORL REG_A "," REG_5		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__A_R5); }
	| ORL REG_A "," REG_6		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__A_R6); }
	| ORL REG_A "," REG_7		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__A_R7); }
	| ORL BIT_C "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__C_DIR, $4); }
	| ORL BIT_C "," "/" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_ORL__C_SL_DIR, $5); }
;
ins_pop:
	  POP e_expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_POP__DIR, $2); }
;
ins_push:
	  PUSH e_expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_PUSH__DIR, $2); }
;
ins_ret:
	  RET				{ $$ = new CompilerStatement(LOC(), ASM51_INS_RET); }
;
ins_reti:
	  RETI				{ $$ = new CompilerStatement(LOC(), ASM51_INS_RETI); }
;
ins_rl:
	  RL REG_A			{ $$ = new CompilerStatement(LOC(), ASM51_INS_RL__A); }
;
ins_rlc:
	  RLC REG_A			{ $$ = new CompilerStatement(LOC(), ASM51_INS_RLC__A); }
;
ins_rr:
	  RR REG_A			{ $$ = new CompilerStatement(LOC(), ASM51_INS_RR__A); }
;
ins_rrc:
	  RRC REG_A			{ $$ = new CompilerStatement(LOC(), ASM51_INS_RRC__A); }
;
ins_setb:
	  SETB expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_SETB__DIR, $2); }
	| SETB const_a			{ $$ = new CompilerStatement(LOC(), ASM51_INS_SETB__DIR, $2); }
	| SETB BIT_C			{ $$ = new CompilerStatement(LOC(), ASM51_INS_SETB__C); }
;
ins_sjmp:
	  SJMP e_expr			{ $$ = new CompilerStatement(LOC(), ASM51_INS_SJMP__DIR, $2); }
;
ins_subb:
	  SUBB REG_A "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_SUBB__A_IMM, $5); }
	| SUBB REG_A "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_SUBB__A_DIR, $4); }
	| SUBB REG_A "," IND_R0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_SUBB__A_IR0); }
	| SUBB REG_A "," IND_R1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_SUBB__A_IR1); }
	| SUBB REG_A "," REG_0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_SUBB__A_R0); }
	| SUBB REG_A "," REG_1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_SUBB__A_R1); }
	| SUBB REG_A "," REG_2		{ $$ = new CompilerStatement(LOC(), ASM51_INS_SUBB__A_R2); }
	| SUBB REG_A "," REG_3		{ $$ = new CompilerStatement(LOC(), ASM51_INS_SUBB__A_R3); }
	| SUBB REG_A "," REG_4		{ $$ = new CompilerStatement(LOC(), ASM51_INS_SUBB__A_R4); }
	| SUBB REG_A "," REG_5		{ $$ = new CompilerStatement(LOC(), ASM51_INS_SUBB__A_R5); }
	| SUBB REG_A "," REG_6		{ $$ = new CompilerStatement(LOC(), ASM51_INS_SUBB__A_R6); }
	| SUBB REG_A "," REG_7		{ $$ = new CompilerStatement(LOC(), ASM51_INS_SUBB__A_R7); }
;
ins_swap:
	  SWAP REG_A			{ $$ = new CompilerStatement(LOC(), ASM51_INS_SWAP__A); }
;
ins_xch:
	  XCH REG_A "," IND_R0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCH__A_IR0); }
	| XCH REG_A "," IND_R1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCH__A_IR1); }
	| XCH REG_A "," REG_0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCH__A_R0); }
	| XCH REG_A "," REG_1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCH__A_R1); }
	| XCH REG_A "," REG_2		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCH__A_R2); }
	| XCH REG_A "," REG_3		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCH__A_R3); }
	| XCH REG_A "," REG_4		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCH__A_R4); }
	| XCH REG_A "," REG_5		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCH__A_R5); }
	| XCH REG_A "," REG_6		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCH__A_R6); }
	| XCH REG_A "," REG_7		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCH__A_R7); }
	| XCH REG_A "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCH__A_DIR, $4); }
;
ins_xchd:
	  XCHD REG_A "," IND_R0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCHD__A_IR0); }
	| XCHD REG_A "," IND_R1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XCHD__A_IR1); }
;
ins_xrl:
	  XRL expr "," REG_A		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__DIR_A, $2); }
	| XRL expr "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__DIR_IMM, $2->addLink($5)); }
	| XRL REG_A "," "#" e_expr	{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__A_IMM, $5); }
	| XRL REG_A "," e_expr		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__A_DIR, $4); }
	| XRL REG_A "," IND_R0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__A_IR0); }
	| XRL REG_A "," IND_R1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__A_IR1); }
	| XRL REG_A "," REG_0		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__A_R0); }
	| XRL REG_A "," REG_1		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__A_R1); }
	| XRL REG_A "," REG_2		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__A_R2); }
	| XRL REG_A "," REG_3		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__A_R3); }
	| XRL REG_A "," REG_4		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__A_R4); }
	| XRL REG_A "," REG_5		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__A_R5); }
	| XRL REG_A "," REG_6		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__A_R6); }
	| XRL REG_A "," REG_7		{ $$ = new CompilerStatement(LOC(), ASM51_INS_XRL__A_R7); }
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison
int mcs51parser_error(YYLTYPE * yylloc, yyscan_t, CompilerParserInterface * compiler, const char * errorInfo) {
	compiler->parserMessage(compiler->toSourceLocation(yylloc), CompilerBase::MT_ERROR, errorInfo);
	return 0;
}
