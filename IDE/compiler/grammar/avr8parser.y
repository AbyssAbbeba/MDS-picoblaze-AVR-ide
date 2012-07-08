/**
 * @brief
 * ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */


// -----------------------------------------------------------------------------
// PROLOGUE - BISON DECLARATIONS
// -----------------------------------------------------------------------------

// Require version <version> or higher of Bison.
%require "2.5"
// Rename the external symbols used in the parser so that they start with prefix instead of ‘yy’.
%name-prefix "avr8parser_"
// Generate a parser header file containing macro definitions, etc.
%defines "avr8parser.h"
// Generate a pure, reentrant parser.
%define api.pure
// Generate the code processing the locations.
%locations
// Specify the programming language for the generated parser.
%language "C"
// Write an extra output file containing verbose descriptions of the parser states.
%verbose
// Expect exactly <n> shift/reduce conflicts in this grammar
%expect 925
// Expect exactly <n> reduce/reduce conflicts in this grammar
%expect-rr 0
/* Type of parser tables within the LR family, in this case we use LALR (Look-Ahead LR parser) */
%define lr.type lalr
/* Bison declaration to request verbose, specific error message strings when yyerror is called */
%error-verbose
/* Enable LAC (Lookahead Correction) to improve syntax error handling. */
%define parse.lac full

/*
 * Compiler interface for the syntax analyzer; we need to have some things declared
 * before we can declare other things, and this interface is quite essential here.
 */
%code requires {
	#include "CompilerParserInterface.h"
}

// Symbol semantic value
%union {
	int number;			//
	char * string;			//
	CompilerExpr * expr;		//
	CompilerStatement * stmt;	//
	struct {
		unsigned char * data;	//
		int size;		//
	} array;			//
};

%{
	#include <QObject>		// For i18n, nothing else.
	#include <cstdlib>		// We need free() here.
	#include <iostream> 		// This is required by Bison.
	using namespace std;		// This is required by Bison as well.
	using namespace StatementTypes;	// This NS is heavily used here.

	// Declaration of the lexer prototypes and other things required by Bison
	#include "avr8lexer.h"

	// Name of the lexer function required by Bison
	#define avr8parser_lex avr8lexer_lex

	// Make a few things a little easier...
	#define LOC() \
		( compiler->toSourceLocation(yylloc) )

	// Error messages
	#define NO_LABEL_EXPECTED(loc, directive, stmt) \
		CompilerStatement::completeDelete(stmt); \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("cannot declare a label before %1 directive").arg(directive).toStdString() );
	#define ARG_REQUIRED_D(loc, directive) \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("directive %1 requires a single argument").arg(directive).toStdString() );
	#define NO_ARG_EXPECTED_D(directive, args, loc) \
		args->completeDelete(); \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("directive %1 takes no arguments").arg(directive).toStdString() );
	#define MISSING_AT_OPERATOR(loc, directive) \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("segment specifier %1 requires `AT' operator before the start address").arg(directive).toStdString() );
	#define DECL_ID_EXPECTED(loc, directive) \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("directive %1 requires an identifier for the symbol (or macro) which it defines").arg(directive).toStdString() );
	#define DEPRECATED_SYNTAX(loc) \
		compiler->parserMessage ( loc, CompilerBase::MT_WARNING, \
			QObject::tr("deprecated directive syntax").toStdString() );

	// Declaration of the error reporting function used by Bison
	inline int avr8parser_error (
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

// Declare that the code which must be invoked before parsing actually begins
%initial-action {
	@$.first_column = @$.last_column = 1;
	@$.first_line   = @$.last_line   = 1;
};

/*
 * DECLARATION OF THE GRAMMAR APLHABET - TERMINAL SYMBOLS
 */

/* Directives */
%token D_IF		D_IFN		D_IFDEF		D_IFNDEF	D_ELSEIFB
%token D_ELSEIFNB	D_ELSE		D_ELSEIF	D_ELSEIFN	D_ELSEIFDEF
%token D_ELSEIFNDEF	D_ENDIF		D_LOCAL		D_IFNB		D_IFB
%token D_SKIP		D_ENDM		D_EXITM		D_REPT		D_SET
%token D_MACRO		D_BYTE		D_CSEG		D_DB		D_DEF
%token D_DEVICE		D_DSEG		D_DW		D_EQU		D_ESEG
%token D_EXIT		D_LIST		D_LISTMAC	D_NOLIST	D_ORG

/* Instructions */
%token I_ADD	I_ADC	I_ADIW	I_SUB	I_SUBI	I_SBC	I_SBCI	I_SBIW	I_AND	I_ANDI
%token I_OR	I_ORI	I_EOR	I_COM	I_NEG	I_SBR	I_CBR	I_INC	I_DEC	I_TST
%token I_CLR	I_SER	I_MUL	I_RJMP	I_IJMP	I_JMP	I_RCALL	I_ICALL	I_CALL	I_RET
%token I_RETI	I_CPSE	I_CP	I_CPC	I_CPI	I_SBRC	I_SBRS	I_SBIC	I_SBIS	I_BRBS
%token I_BRBC	I_BREQ	I_BRNE	I_BRCS	I_BRCC	I_BRSH	I_BRLO	I_BRMI	I_BRPL	I_BRGE
%token I_BRLT	I_BRHS	I_BRHC	I_BRTS	I_BRTC	I_BRVS	I_BRVC	I_BRIE	I_BRID	I_MOV
%token I_LDI	I_LDS	I_LD	I_STS	I_ST	I_STD	I_LPM	I_IN	I_OUT	I_PUSH
%token I_POP	I_LSL	I_LSR	I_ROL	I_ROR	I_ASR	I_SWAP	I_BSET	I_BCLR	I_SBI
%token I_CBI	I_BST	I_BLD	I_SEC	I_CLC	I_SEN	I_CLN	I_SEZ	I_CLZ	I_SEI
%token I_CLI	I_SES	I_CLS	I_SEV	I_CLV	I_SET	I_CLT	I_SEH	I_CLH	I_NOP
%token I_SLEEP	I_WDR	I_LDD

/* Fixed operands, i.e. those which have no value */
%token X	Y	Z

/* Other terminal symbols */
%token COMMENT	AT_SIGN	AT
%token EOL		"end of line"
%token END	0	"end of file"
// Named tokens (to make the grammar rules more readable)
%token LP		"("
%token RP		")"
%token COMMA		","
// Expression operators
%token ASSIGN		"="
%token PLUS		"+"
%token MINUS		"-"
%token ASTERISK		"*"
%token SLASH		"/"
%token NOT		"!"
%token MOD		"%"
%token SHL		"<<"
%token SHR		">>"
%token AND		"&&"
%token OR		"||"
%token XOR		"^^"
%token BITAND		"&"
%token BITOR		"|"
%token BITXOR		"^"
%token EQ		"=="
%token NE		"!="
%token LT		"<"
%token LE		"<="
%token GT		">"
%token GE		">="
%token COMPLEMET	"~"
%token ADD_ASSIGN	"+="
%token SUB_ASSIGN	"-="
%token MUL_ASSIGN	"*="
%token DIV_ASSIGN	"/="
%token MOD_ASSIGN	"%="
%token SHL_ASSIGN	"<<="
%token SHR_ASSIGN	">>="
%token AND_ASSIGN	"&="
%token ORB_ASSIGN	"|="
%token XOR_ASSIGN	"^="
%token INCREMENT	"++"
%token DECREMENT	"--"

// Expression functions
%token F_HIGH		F_LOW		F_BYTE2		F_BYTE3		F_BYTE4
%token F_LWRD		F_HWRD		F_PAGE		F_EXP2		F_LOG2

/* Operator precedence (the one declared later has the higher precedence) */
// Left-to-right
%left "||"
%left "^^"
%left "&&"
%left "|"
%left "^"
%left "&"
%left "==" "!="
%left "<" "<=" ">=" ">"
%left "<<" ">>"
%left "+" "-"
%left "*" "/" "%"
%left "++" "--"
// Right-to-left
%right "&=" "^=" "|="
%right "<<=" ">>="
%right "*=" "/=" "%="
%right "+=" "-="
%right "!" "~"

/* Terminal symbols with semantic value */
  // semantic value is a string
%token<string>	IDENFIFIER	LABEL
%token<array>	STRING		INCLUDE
  // semantic value is a number
%token<number>	NUMBER

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Expressions
%type<expr>	expr		"expression"
%type<expr>	number		params		args		id		string
%type<expr>	args_str
// Statements - general
%type<stmt>	statements	stmt		inst_stmt	dir_stmt	macro_stmt
%type<stmt>	instruction	directive	macro		label
// Statements - directives
%type<stmt>	dir_macro_d	dir_macro	dir_exitm	dir_db		dir_include
%type<stmt>	dir_cond_asm	if_block	ifelse_block	else_block	dir_if
%type<stmt>	dir_ifn		dir_ifdef	dir_ifndef	dir_ifb		dir_ifnb
%type<stmt>	dir_elseif	dir_elseifn	dir_elseifdef	dir_elseifndf	dir_elseifb
%type<stmt>	dir_elseifnb	dir_rept	dir_dw		dir_cseg	dir_cseg_a
%type<stmt>	dir_skip	dir_local	dir_exit	dir_list	dir_listmac
%type<stmt>	dir_nolist	dir_repeat	dir_org		dir_eseg	dir_eseg_a
%type<stmt>	dir_dseg	dir_byte	dir_set		dir_byte_a	dir_equ
%type<stmt>	dir_if_a	dir_ifn_a	dir_ifdef_a	dir_macro_a	dir_dseg_a
%type<stmt>	dir_ifndef_a	dir_ifb_a	dir_ifnb_a	dir_else	dir_elseif_a
%type<stmt>	dir_elseifn_a	dir_elseifdef_a	dir_elseifndf_a	dir_elseifb_a	dir_elseifnb_a
%type<stmt>	dir_endm	dir_device	dir_def

// Each time the parser discards symbol with certain semantic types, their memory have to bee freed
%destructor {
	if ( NULL != $$ ) {
		$$->completeDelete();
	}
} <expr>
%destructor {
	if ( NULL != $$ ) {
		$$->completeDelete();
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
	  statements			{ compiler->syntaxAnalysisComplete($statements); YYACCEPT; }
;
statements:
	  stmt				{ $$ = $stmt; }
	| error				{ $$ = NULL; }
	| statements EOL stmt		{ $$ = $1->addLink($stmt); }
	| statements error EOL stmt	{ $$ = $1->addLink($stmt); }
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
	  directive			{ $$ = $directive; }
	| directive COMMENT		{ $$ = $directive; }
;
inst_stmt:
	  instruction			{ $$ = $instruction; }
	| instruction COMMENT		{ $$ = $instruction; }
	| label instruction		{ $$ = $label->addLink($instruction); }
	| label instruction COMMENT	{ $$ = $label->addLink($instruction); }
;
macro_stmt:
	  macro				{ $$ = $macro; }
	| macro COMMENT			{ $$ = $macro; }
	| label macro			{ $$ = $label->addLink($macro); }
	| label macro COMMENT		{ $$ = $label->addLink($macro); }
;
macro:
	  id				{ $$ = new CompilerStatement(LOC(), ASMAVR8_MACRO, $id); }
	| id "(" ")"			{ $$ = new CompilerStatement(LOC(), ASMAVR8_MACRO, $id); }
	| id args			{ $$ = new CompilerStatement(LOC(), ASMAVR8_MACRO, $id->addLink($args)); }
	| id "(" args ")"		{ $$ = new CompilerStatement(LOC(), ASMAVR8_MACRO, $id->addLink($args)); }
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
	  LABEL				{ $$ = new CompilerStatement(LOC(), ASMAVR8_LABEL, new CompilerExpr($1)); }
;
expr:
	  id				{ $$ = $id; }
	| number			{ $$ = $number; }
	| "(" expr ")"			{ $$ = $2; }
	| AT_SIGN expr			{ $$ = new CompilerExpr(CompilerExpr::OPER_AT, $2); }
	| F_LOW "(" expr ")"		{ $$ = new CompilerExpr(CompilerExpr::OPER_LOW, $3); }
	| F_HIGH "(" expr ")"		{ $$ = new CompilerExpr(CompilerExpr::OPER_HIGH, $3); }
	| F_BYTE2 "(" expr ")"		{ $$ = new CompilerExpr("byte2", 'C', $3); }
	| F_BYTE3 "(" expr ")"		{ $$ = new CompilerExpr("byte3", 'C', $3); }
	| F_BYTE4 "(" expr ")"		{ $$ = new CompilerExpr("byte4", 'C', $3); }
	| F_LWRD "(" expr ")"		{ $$ = new CompilerExpr("lwrd", 'C', $3); }
	| F_HWRD "(" expr ")"		{ $$ = new CompilerExpr("hwrd", 'C', $3); }
	| F_PAGE "(" expr ")"		{ $$ = new CompilerExpr("page", 'C', $3); }
	| F_EXP2 "(" expr ")"		{ $$ = new CompilerExpr("exp2", 'C', $3); }
	| F_LOG2 "(" expr ")"		{ $$ = new CompilerExpr("log2", 'C', $3); }
	| "~" expr			{ $$ = new CompilerExpr('~', $2); }
	| "!" expr			{ $$ = new CompilerExpr('!', $2); }
	| expr "+" expr			{ $$ = new CompilerExpr($1, '+', $3); }
	| expr "-" expr			{ $$ = new CompilerExpr($1, '-', $3); }
	| expr "*" expr			{ $$ = new CompilerExpr($1, '*', $3); }
	| expr "/" expr			{ $$ = new CompilerExpr($1, '/', $3); }
	| expr "%" expr			{ $$ = new CompilerExpr($1, '%', $3); }
	| expr "||" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_LOR, $3); }
	| expr "^^" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_LXOR, $3); }
	| expr "&&" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_LAND, $3); }
	| expr "|" expr			{ $$ = new CompilerExpr($1, '|', $3); }
	| expr "^" expr			{ $$ = new CompilerExpr($1, '^', $3); }
	| expr "&" expr			{ $$ = new CompilerExpr($1, '&', $3); }
	| expr "==" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_EQ, $3); }
	| expr "!=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_NE, $3); }
	| expr "<" expr			{ $$ = new CompilerExpr($1, '<', $3); }
	| expr "<=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_LE, $3); }
	| expr ">=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_GE, $3); }
	| expr ">" expr			{ $$ = new CompilerExpr($1, '>', $3); }
	| expr ">>" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_SHR, $3); }
	| expr "<<" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_SHL, $3); }
	| expr "+=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_ADD_ASSIGN, $3); }
	| expr "-=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_SUB_ASSIGN, $3); }
	| expr "*=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_MUL_ASSIGN, $3); }
	| expr "/=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_DIV_ASSIGN, $3); }
	| expr "%=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_MOD_ASSIGN, $3); }
	| expr "<<=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_SHL_ASSIGN, $3); }
	| expr ">>=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_SHR_ASSIGN, $3); }
	| expr "&=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_AND_ASSIGN, $3); }
	| expr "|=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_ORB_ASSIGN, $3); }
	| expr "^=" expr		{ $$ = new CompilerExpr($1, CompilerExpr::OPER_XOR_ASSIGN, $3); }
	| expr "=" expr			{ $$ = new CompilerExpr($1, '=', $3); }
	| "++" expr			{ $$ = new CompilerExpr(CompilerExpr::OPER_INC, $2); }
	| "--" expr			{ $$ = new CompilerExpr(CompilerExpr::OPER_DEC, $2); }
	| expr "++"			{ $$ = new CompilerExpr($1, CompilerExpr::OPER_INC); }
	| expr "--"			{ $$ = new CompilerExpr($1, CompilerExpr::OPER_DEC); }
	| error				{ $$ = NULL; }
	| expr expr			{
						/* Syntax error */
						$$ = $1->addLink($2);
						if ( 0 == YYRECOVERING() ) {
							@1.first_line = @1.last_line; \
							@1.first_column = @1.last_column; \
							@1.last_column++; \
							compiler->parserMessage (
								@1, CompilerBase::MT_ERROR,
								QObject::tr("missing operator").toStdString() );
						}
					}
;
args:		// List of arguments without strings, e.g. `(1+3), XYZ, 0x4b'
	  args "," expr		{ $$ = $1->addLink($expr); }
	| expr			{ $$ = $expr; }
;
args_str:	// List of arguments possibly with strings, e.g. `ABC, "XYZ - QWE", 2 * 0x4b'
	  args_str "," string		{ $$ = $1->addLink($string); }
	| args_str "," expr		{ $$ = $1->addLink($expr); }
	| string			{ $$ = $string; }
	| expr			{ $$ = $expr; }
;
params:		// List of identifiers, e.g. `target, source, offset, size, mode'
	  params "," id			{ $$ = $1->addLink($id); }
	| id				{ $$ = $id; }
;

/*
 * Directives
 */
directive:
	  dir_byte	{ $$ = $1; }	| dir_cond_asm	{ $$ = $1; }
	| dir_dw	{ $$ = $1; }	| dir_skip	{ $$ = $1; }
	| dir_local	{ $$ = $1; }	| dir_exitm	{ $$ = $1; }
	| dir_list	{ $$ = $1; }	| dir_include	{ $$ = $1; }
	| dir_nolist	{ $$ = $1; }	| dir_repeat	{ $$ = $1; }
	| dir_org	{ $$ = $1; }	| dir_listmac	{ $$ = $1; }
	| dir_eseg	{ $$ = $1; }	| dir_dseg	{ $$ = $1; }
	| dir_set	{ $$ = $1; }	| dir_equ	{ $$ = $1; }
	| dir_macro	{ $$ = $1; }	| dir_db	{ $$ = $1; }
	| dir_exit	{ $$ = $1; }	| dir_cseg	{ $$ = $1; }
	| dir_device	{ $$ = $1; }	| dir_def	{ $$ = $1; }
;
dir_cond_asm:
	  if_block ifelse_block else_block D_ENDIF {
		$$ = new CompilerStatement(LOC(), ASMAVR8_COND_ASM);
		$$->createBranch ( $if_block->addLink($ifelse_block)->addLink($else_block) );
	}
	| label if_block ifelse_block else_block D_ENDIF {
		$$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_COND_ASM));
		$$->createBranch ( $if_block->addLink($ifelse_block)->addLink($else_block) );
	}
;
if_block:
	  dir_if	statements EOL	{ $$ = $1->createBranch($2); }
	| dir_ifn	statements EOL	{ $$ = $1->createBranch($2); }
	| dir_ifdef	statements EOL	{ $$ = $1->createBranch($2); }
	| dir_ifndef	statements EOL	{ $$ = $1->createBranch($2); }
	| dir_ifb	statements EOL	{ $$ = $1->createBranch($2); }
	| dir_ifnb	statements EOL	{ $$ = $1->createBranch($2); }
	| dir_if			{ $$ = $1; }
	| dir_ifn			{ $$ = $1; }
	| dir_ifdef			{ $$ = $1; }
	| dir_ifndef			{ $$ = $1; }
	| dir_ifb			{ $$ = $1; }
	| dir_ifnb			{ $$ = $1; }
;
ifelse_block:
	  /* empty */			{ $$ = NULL; }
	| dir_elseif	statements EOL	{ $$ = $1->createBranch($2); }
	| dir_elseifn	statements EOL	{ $$ = $1->createBranch($2); }
	| dir_elseifdef	statements EOL	{ $$ = $1->createBranch($2); }
	| dir_elseifndf	statements EOL	{ $$ = $1->createBranch($2); }
	| dir_elseifb	statements EOL	{ $$ = $1->createBranch($2); }
	| dir_elseifnb	statements EOL	{ $$ = $1->createBranch($2); }
	| dir_elseif			{ $$ = $1; }
	| dir_elseifn			{ $$ = $1; }
	| dir_elseifdef			{ $$ = $1; }
	| dir_elseifndf			{ $$ = $1; }
	| dir_elseifb			{ $$ = $1; }
	| dir_elseifnb			{ $$ = $1; }
;
else_block:
	  /* empty */			{ $$ = NULL; }
	| dir_else statements EOL	{ $$ = $dir_else->createBranch($statements); }
	| dir_else			{ $$ = $dir_else; }
;
dir_else:
	  D_ELSE			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSE); }
	| label D_ELSE			{
						/* Syntax error */
						NO_LABEL_EXPECTED(@label, "ELSE", $label);
						$$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSE);
					}
	| label D_ELSE args		{
						/* Syntax error */
						NO_ARG_EXPECTED_D("ELSE", $args, @args);
						NO_LABEL_EXPECTED(@label, "ELSE", $label);
						$$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSE);
					}
	| D_ELSE args			{
						/* Syntax error */
						NO_ARG_EXPECTED_D("ELSE", $args, @args);
						$$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSE);
					}
;
dir_if:
	  dir_if_a EOL			{ $$ = $dir_if_a; }
	| dir_if_a COMMENT EOL		{ $$ = $dir_if_a; }
;
dir_if_a:
	  D_IF expr			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_IF, $expr); }
	| D_IF				{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IF, "IF"); }
;
dir_ifn:
	  dir_ifn_a EOL			{ $$ = $dir_ifn_a; }
	| dir_ifn_a COMMENT EOL		{ $$ = $dir_ifn_a; }
;
dir_ifn_a:
	  D_IFN expr			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_IFN, $expr); }
	| D_IFN				{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFN, "IFN"); }
;
dir_ifdef:
	  dir_ifdef_a EOL		{ $$ = $dir_ifdef_a; }
	| dir_ifdef_a COMMENT EOL	{ $$ = $dir_ifdef_a; }
;
dir_ifdef_a:
	  D_IFDEF id			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_IFDEF, $id); }
	| D_IFDEF			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFDEF, "IFDEF"); }
;
dir_ifndef:
	  dir_ifndef_a EOL		{ $$ = $dir_ifndef_a; }
	| dir_ifndef_a COMMENT EOL	{ $$ = $dir_ifndef_a; }
;
dir_ifndef_a:
	  D_IFNDEF id			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_IFNDEF, $id); }
	| D_IFNDEF			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFNDEF, "IFNDEF"); }
;
dir_ifb:
	  dir_ifb_a EOL			{ $$ = $dir_ifb_a; }
	| dir_ifb_a COMMENT EOL		{ $$ = $dir_ifb_a; }
;
dir_ifb_a:
	  D_IFB id			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_IFB, $id); }
	| D_IFB				{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFB, "IFB"); }
;
dir_ifnb:
	  dir_ifnb_a EOL		{ $$ = $dir_ifnb_a; }
	| dir_ifnb_a COMMENT EOL	{ $$ = $dir_ifnb_a; }
;
dir_ifnb_a:
	  D_IFNB id			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_IFNB, $id); }
	| D_IFNB			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFNB, "IFNB"); }
;
dir_elseif:
	  dir_elseif_a EOL		{ $$ = $dir_elseif_a; }
	| dir_elseif_a COMMENT EOL	{ $$ = $dir_elseif_a; }
;
dir_elseif_a:
	  D_ELSEIF expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSEIF, $expr); }
	| D_ELSEIF			{ /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIF, "ELSEIF"); }
	| label D_ELSEIF expr		{ /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIF", $label); $expr->completeDelete(); }
	| label D_ELSEIF		{
						/* Syntax Error */
						$$ = NULL;
						ARG_REQUIRED_D(@D_ELSEIF, "ELSEIF");
						NO_LABEL_EXPECTED(@label, "ELSEIF", $label);
					}
;
dir_elseifn:
	  dir_elseifn_a EOL		{ $$ = $dir_elseifn_a; }
	| dir_elseifn_a COMMENT EOL	{ $$ = $dir_elseifn_a; }
;
dir_elseifn_a:
	  D_ELSEIFN expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSEIFN, $expr); }
	| D_ELSEIFN			{ /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFN, "ELSEIFN"); }
	| label D_ELSEIFN expr	{ /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFN", $label); $expr->completeDelete(); }
	| label D_ELSEIFN		{
						/* Syntax Error */
						$$ = NULL;
						ARG_REQUIRED_D(@D_ELSEIFN, "ELSEIFN");
						NO_LABEL_EXPECTED(@label, "ELSEIFN", $label);
					}
;
dir_elseifdef:
	  dir_elseifdef_a EOL		{ $$ = $dir_elseifdef_a; }
	| dir_elseifdef_a COMMENT EOL	{ $$ = $dir_elseifdef_a; }
;
dir_elseifdef_a:
	  D_ELSEIFDEF expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSEIFDEF, $expr); }
	| D_ELSEIFDEF			{ /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFDEF, "ELSEIFDEF"); }
	| label D_ELSEIFDEF expr	{ /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFDEF", $label); $expr->completeDelete(); }
	| label D_ELSEIFDEF		{
						/* Syntax Error */
						$$ = NULL;
						ARG_REQUIRED_D(@D_ELSEIFDEF, "ELSEIFDEF");
						NO_LABEL_EXPECTED(@label, "ELSEIFDEF", $label);
					}
;
dir_elseifndf:
	  dir_elseifndf_a EOL		{ $$ = $dir_elseifndf_a; }
	| dir_elseifndf_a COMMENT EOL	{ $$ = $dir_elseifndf_a; }
;
dir_elseifndf_a:
	  D_ELSEIFNDEF expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSEIFNDEF, $expr); }
	| D_ELSEIFNDEF			{ /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFNDEF, "ELSEIFNDEF"); }
	| label D_ELSEIFNDEF expr	{ /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFNDEF", $label); $expr->completeDelete(); }
	| label D_ELSEIFNDEF		{
						/* Syntax Error */
						$$ = NULL;
						ARG_REQUIRED_D(@D_ELSEIFNDEF, "ELSEIFNDEF");
						NO_LABEL_EXPECTED(@label, "ELSEIFNDEF", $label);
					}
;
dir_elseifb:
	  dir_elseifb_a EOL		{ $$ = $dir_elseifb_a; }
	| dir_elseifb_a COMMENT EOL	{ $$ = $dir_elseifb_a; }
;
dir_elseifb_a:
	  D_ELSEIFB expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSEIFB, $expr); }
	| D_ELSEIFB			{ /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFB, "ELSEIFB"); }
	| label D_ELSEIFB expr	{ /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFB", $label); $expr->completeDelete(); }
	| label D_ELSEIFB		{
						/* Syntax Error */
						$$ = NULL;
						ARG_REQUIRED_D(@D_ELSEIFB, "ELSEIFB");
						NO_LABEL_EXPECTED(@label, "ELSEIFB", $label);
					}
;
dir_elseifnb:
	  dir_elseifnb_a EOL		{ $$ = $dir_elseifnb_a; }
	| dir_elseifnb_a COMMENT EOL	{ $$ = $dir_elseifnb_a; }
;
dir_elseifnb_a:
	  D_ELSEIFNB expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSEIFNB, $expr); }
	| D_ELSEIFNB			{ /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFNB, "ELSEIFNB"); }
	| label D_ELSEIFNB expr	{ /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFNB", $label); $expr->completeDelete(); }
	| label D_ELSEIFNB		{
						/* Syntax Error */
						$$ = NULL;
						ARG_REQUIRED_D(@D_ELSEIFNB, "ELSEIFNB");
						NO_LABEL_EXPECTED(@label, "ELSEIFNB", $label);
					}
;
dir_org:
	  D_ORG expr			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ORG, $expr); }
	| label D_ORG expr		{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_ORG, $expr)); }
	| D_ORG				{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_ORG, "ORG"); }
	| label D_ORG			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_ORG, "ORG"); $label->completeDelete(); }
;
dir_cseg:
	  dir_cseg_a			{ $$ = $dir_cseg_a; }
	| label dir_cseg_a		{ $$ = $label->addLink($dir_cseg_a); }
;
dir_cseg_a:
	  D_CSEG			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_CSEG); }
	| D_CSEG AT expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_CSEG, $expr); }
	| D_CSEG expr			{
						/* Syntax error */ MISSING_AT_OPERATOR(@D_CSEG, "CSEG");
						$$ = new CompilerStatement(LOC(), ASMAVR8_DIR_CSEG, $expr);
					}
;
dir_dseg:
	  dir_dseg_a			{ $$ = $dir_dseg_a; }
	| label dir_dseg_a		{ $$ = $label->addLink($dir_dseg_a); }
;
dir_dseg_a:
	  D_DSEG			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DSEG); }
	| D_DSEG AT expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DSEG, $expr); }
	| D_DSEG expr			{
						/* Syntax error */ MISSING_AT_OPERATOR(@D_DSEG, "DSEG");
						$$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DSEG, $expr);
					}
;
dir_eseg:
	  dir_eseg_a			{ $$ = $dir_eseg_a; }
	| label dir_eseg_a		{ $$ = $label->addLink($dir_eseg_a); }
;
dir_eseg_a:
	  D_ESEG			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ESEG); }
	| D_ESEG AT expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ESEG, $expr); }
	| D_ESEG expr			{
						/* Syntax error */ MISSING_AT_OPERATOR(@D_ESEG, "ESEG");
						$$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ESEG, $expr);
					}
;
dir_dw:
	  D_DW expr			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DW, $expr); }
	| label D_DW expr		{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_DW, $expr)); }
	| D_DW				{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DW, "D_DW"); }
	| label D_DW			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DW, "D_DW"); $label->completeDelete(); }
;
dir_skip:
	  D_SKIP expr			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_SKIP, $expr); }
	| label D_SKIP expr		{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_SKIP, $expr)); }
	| D_SKIP			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_SKIP, "SKIP"); }
	| label D_SKIP			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_SKIP, "SKIP"); $label->completeDelete(); }
;
dir_local:
	  D_LOCAL id			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_LOCAL, $id); }
	| label D_LOCAL id		{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_LOCAL, $id)); }
	| D_LOCAL			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_LOCAL, "LOCAL"); }
	| label D_LOCAL			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_LOCAL, "LOCAL"); $label->completeDelete(); }
;
dir_exitm:
	  D_EXITM			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_EXITM); }
	| label D_EXITM			{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_EXITM)); }
	| D_EXITM args			{ /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("EXITM", $args, @args); }
	| label D_EXITM args		{ /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("EXITM", $args, @args); $label->completeDelete(); }
;
dir_list:
	  D_LIST			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_LIST); }
	| label D_LIST			{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_LIST)); }
	| D_LIST args			{ /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("LIST", $args, @args); }
	| label D_LIST args		{ /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("LIST", $args, @args); $label->completeDelete(); }
;
dir_nolist:
	  D_NOLIST			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_NOLIST); }
	| label D_NOLIST		{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_NOLIST)); }
	| D_NOLIST args			{ /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("NOLIST", $args, @args); }
	| label D_NOLIST args		{ /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("NOLIST", $args, @args); $label->completeDelete(); }
;
dir_set:
	  id D_SET expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_SET, $id->addLink($expr)); }
	| D_SET id "=" expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_SET, $id->addLink($expr)); DEPRECATED_SYNTAX(@D_SET); }
	| label D_SET id "=" expr	{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_SET, $id->addLink($expr))); DEPRECATED_SYNTAX(@D_SET); }
	| id D_SET			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_SET, "SET"); $id->completeDelete(); }
	| D_SET				{ /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_SET, "SET"); ARG_REQUIRED_D(@D_SET, "SET"); }
	| D_SET expr			{ /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_SET, "SET"); $expr->completeDelete(); }
	| label D_SET expr		{ /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_SET, "SET"); NO_LABEL_EXPECTED(@label, "SET", $label->addArgsLink($expr)) }
	| label id D_SET expr		{ /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "SET", $label->addArgsLink($id->addLink($expr))); }
;
dir_equ:
	  id D_EQU expr			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_EQU, $id->addLink($expr)); }
	| D_EQU id "=" expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_EQU, $id->addLink($expr)); DEPRECATED_SYNTAX(@D_EQU); }
	| label D_EQU id "=" expr	{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_EQU, $id->addLink($expr))); DEPRECATED_SYNTAX(@D_EQU); }
	| id D_EQU			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_EQU, "EQU"); $id->completeDelete(); }
	| D_EQU				{ /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_EQU, "EQU"); ARG_REQUIRED_D(@D_EQU, "EQU"); }
	| D_EQU expr			{ /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_EQU, "EQU"); $expr->completeDelete(); }
	| label D_EQU expr		{ /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_EQU, "EQU"); NO_LABEL_EXPECTED(@label, "EQU", $label->addArgsLink($expr)) }
	| label id D_EQU expr		{ /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "EQU", $label->addArgsLink($id->addLink($expr))); }
;
dir_def:
	  id D_DEF expr			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DEF, $id->addLink($expr)); }
	| D_DEF id "=" expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DEF, $id->addLink($expr)); DEPRECATED_SYNTAX(@D_DEF); }
	| label D_DEF id "=" expr	{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_DEF, $id->addLink($expr))); DEPRECATED_SYNTAX(@D_DEF); }
	| id D_DEF			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DEF, "DEF"); $id->completeDelete(); }
	| D_DEF				{ /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_DEF, "DEF"); ARG_REQUIRED_D(@D_DEF, "DEF"); }
	| D_DEF expr			{ /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_DEF, "DEF"); $expr->completeDelete(); }
	| label D_DEF expr		{ /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_DEF, "DEF"); NO_LABEL_EXPECTED(@label, "DEF", $label->addArgsLink($expr)) }
	| label id D_DEF expr		{ /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "DEF", $label->addArgsLink($id->addLink($expr))); }
;
dir_device:
	  D_DEVICE			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DEVICE, "DEVICE"); }
	| label D_DEVICE		{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DEVICE, "DEVICE"); $label->completeDelete(); }
	| D_DEVICE id			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DEVICE, $id); }
	| label D_DEVICE id		{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_DEVICE, $id)); }
;
dir_repeat:
	  dir_rept
	  statements EOL
	  dir_endm			{ $$ = $dir_rept->createBranch($statements->addLink($dir_endm)); }
	| dir_rept
	  dir_endm			{ $$ = $dir_rept->createBranch($dir_endm); }
;
dir_rept:
	  D_REPT expr COMMENT		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_REPT, $expr); }
	| D_REPT expr			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_REPT, $expr); }
	| D_REPT COMMENT		{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_REPT); }
	| label D_REPT expr COMMENT	{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_REPT, $expr)); }
	| label D_REPT expr		{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_REPT, $expr)); }
	| D_REPT			{ /* Syntax error */ $$ = NULL;   ARG_REQUIRED_D(@D_REPT, "REPT (TIMES)"); }
	| label D_REPT COMMENT		{ /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_REPT, "REPT (TIMES)"); }
	| label D_REPT			{ /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_REPT, "REPT (TIMES)"); }
;
dir_macro:
	  dir_macro_d
	  statements EOL
	  dir_endm			{ $$ = $dir_macro_d->createBranch($statements->addLink($dir_endm)); }
	| dir_macro_d
	  dir_endm			{ $$ = $dir_macro_d->createBranch($dir_endm); }
;
dir_macro_d:
	  dir_macro_a EOL		{ $$ = $dir_macro_a; }
	| dir_macro_a COMMENT EOL	{ $$ = $dir_macro_a; }
	| dir_macro_a params EOL 	{ $$ = $dir_macro_a->addArgsLink($params); }
	| dir_macro_a params COMMENT EOL{ $$ = $dir_macro_a->addArgsLink($params); }
;
dir_macro_a:
	  id D_MACRO			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_MACRO, $id); }
	| D_MACRO id			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_MACRO, $id); DEPRECATED_SYNTAX(@D_MACRO); }
	| D_MACRO			{ /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_MACRO, "MACRO"); }
	| label D_MACRO			{
						/* Syntax error */
						$$ = NULL;
						NO_LABEL_EXPECTED(@label, "MACRO", $label);
						DECL_ID_EXPECTED(@D_MACRO, "MACRO");
					}
;
dir_endm:
	  dir_endm_a COMMENT		{ $$ = NULL; }
	| dir_endm_a			{ $$ = NULL; }
	| label dir_endm_a COMMENT	{ $$ = $label; }
	| label dir_endm_a		{ $$ = $label; }
;
dir_endm_a:
	  D_ENDM			{ }
	| D_ENDM args			{ /* Syntax error */ NO_ARG_EXPECTED_D("ENDM", $args, @args); }
;
dir_db:
	  D_DB args_str			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DB, $args_str); }
	| label D_DB args_str		{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_DB, $args_str)); }
	| D_DB				{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DB, "DB"); }
	| label D_DB			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DB, "DB"); $label->completeDelete(); }
;
dir_byte:
	  dir_byte_a			{ $$ = $dir_byte_a; }
	| label dir_byte_a		{ $$ = $label->addLink($dir_byte_a); }
;
dir_byte_a:
	  D_BYTE expr			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_BYTE, $expr); }
	| D_BYTE			{ /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_BYTE, "BYTE"); }
;
dir_include:
	  INCLUDE			{
						$$ = new CompilerStatement (
							LOC(),
							ASMAVR8_INCLUDE,
							new CompilerExpr (
								CompilerExpr::Value($INCLUDE.data, $INCLUDE.size)
							)
						);
					}
	| label INCLUDE			{
						$$ = $label->addLink (
							new CompilerStatement (
								LOC(),
								ASMAVR8_INCLUDE,
								new CompilerExpr (
									CompilerExpr::Value ( $INCLUDE.data, $INCLUDE.size )
								)
							)
						);
					}
;
dir_exit:
	  D_EXIT			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_EXIT); }
	| label D_EXIT			{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_EXIT)); }
;
dir_listmac:
	  D_LISTMAC			{ $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_LISTMAC); }
	| label D_LISTMAC		{ $$ = $label->addLink(new CompilerStatement(LOC(), ASMAVR8_DIR_LISTMAC)); }
;

/*
 * Instructions
 */
instruction:
	/* ARITHMETIC AND LOGIC INSTRUCTIONS */
	  I_ADD		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ADD, $2->addLink($4)); }
	| I_ADC		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ADC, $2->addLink($4)); }
	| I_ADIW	expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ADIW, $2->addLink($4)); }
	| I_SUB		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SUB, $2->addLink($4)); }
	| I_SUBI	expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SUBI, $2->addLink($4)); }
	| I_SBC		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBC, $2->addLink($4)); }
	| I_SBCI	expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBCI, $2->addLink($4)); }
	| I_SBIW	expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBIW, $2->addLink($4)); }
	| I_AND		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_AND, $2->addLink($4)); }
	| I_ANDI	expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ANDI, $2->addLink($4)); }
	| I_OR		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_OR, $2->addLink($4)); }
	| I_ORI		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ORI, $2->addLink($4)); }
	| I_EOR		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_EOR, $2->addLink($4)); }
	| I_COM		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_COM, $2); }
	| I_NEG		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_NEG, $2); }
	| I_SBR		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBR, $2->addLink($4)); }
	| I_CBR		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CBR, $2->addLink($4)); }
	| I_INC		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_INC, $2); }
	| I_DEC		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_DEC, $2); }
	| I_TST		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_TST, $2); }
	| I_CLR		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLR, $2); }
	| I_SER		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SER, $2); }
	| I_MUL		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_MUL, $2->addLink($4)); }
	/* BRANCH INSTRUCTIONS */
	| I_RJMP	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_RJMP, $2); }
	| I_IJMP			{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_IJMP); }
	| I_JMP		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_JMP, $2); }
	| I_RCALL	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_RCALL, $2); }
	| I_ICALL			{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ICALL); }
	| I_CALL	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CALL, $2); }
	| I_RET				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_RET); }
	| I_RETI			{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_RETI); }
	| I_CPSE	expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CPSE, $2->addLink($4)); }
	| I_CP		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CP, $2->addLink($4)); }
	| I_CPC		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CPC, $2->addLink($4)); }
	| I_CPI		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CPI, $2->addLink($4)); }
	| I_SBRC	expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBRC, $2->addLink($4)); }
	| I_SBRS	expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBRS, $2->addLink($4)); }
	| I_SBIC	expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBIC, $2->addLink($4)); }
	| I_SBIS	expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBIS, $2->addLink($4)); }
	| I_BRBS	expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRBS, $2->addLink($4)); }
	| I_BRBC	expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRBC, $2->addLink($4)); }
	| I_BREQ	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BREQ, $2); }
	| I_BRNE	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRNE, $2); }
	| I_BRCS	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRCS, $2); }
	| I_BRCC	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRCC, $2); }
	| I_BRSH	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRSH, $2); }
	| I_BRLO	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRLO, $2); }
	| I_BRMI	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRMI, $2); }
	| I_BRPL	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRPL, $2); }
	| I_BRGE	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRGE, $2); }
	| I_BRLT	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRLT, $2); }
	| I_BRHS	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRHS, $2); }
	| I_BRHC	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRHC, $2); }
	| I_BRTS	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRTS, $2); }
	| I_BRTC	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRTC, $2); }
	| I_BRVS	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRVS, $2); }
	| I_BRVC	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRVC, $2); }
	| I_BRIE	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRIE, $2); }
	| I_BRID	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRID, $2); }
	/* DATA TRANSFER INSTRUCTIONS */
	| I_MOV		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_MOV, $2->addLink($4)); }
	| I_LDI		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LDI, $2->addLink($4)); }
	| I_LDS		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LDS, $2->addLink($4)); }
	| I_LD		expr "," X	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_X, $2); }
	| I_LD		expr "," X "+"	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_Xp, $2); }
	| I_LD		expr "," "-" X	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_mX, $2); }
	| I_LD		expr "," Y	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_Y, $2); }
	| I_LD		expr "," Y "+"	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_Yp, $2); }
	| I_LD		expr "," "-" Y	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_mY, $2); }
	| I_LD		expr "," Z	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_Z, $2); }
	| I_LD		expr "," Z "+"	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_Zp, $2); }
	| I_LD		expr "," "-" Z	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_mZ, $2); }
	| I_LDD		expr "," Y "+" expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LDD_Y, $2->addLink($6)); }
	| I_LDD		expr "," Z "+" expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LDD_Z, $2->addLink($6)); }
	| I_STS		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_STS, $2->addLink($4)); }
	| I_ST		X "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_X, $4); }
	| I_ST		X "+" "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_Xp, $5); }
	| I_ST		"-" X "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_mX, $5); }
	| I_ST		Y "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_Y, $4); }
	| I_ST		Y "+" "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_Yp, $5); }
	| I_ST		"-" Y "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_mY, $5); }
	| I_ST		Z "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_Z, $4); }
	| I_ST		Z "+" "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_Zp, $5); }
	| I_ST		"-" Z "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_mZ, $5); }
	| I_STD		Y "+" expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_STD_Y, $4->addLink($6)); }
	| I_STD		Z "+" expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_STD_Z, $4->addLink($6)); }
	| I_LPM				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LPM); }
	| I_IN		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_IN, $2->addLink($4)); }
	| I_OUT		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_OUT, $2->addLink($4)); }
	| I_PUSH	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_PUSH, $2); }
	| I_POP		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_POP, $2); }
	/* BIT AND BIT-TEST INSTRUCTIONS */
	| I_LSL		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LSL, $2); }
	| I_LSR		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LSR, $2); }
	| I_ROL		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ROL, $2); }
	| I_ROR		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ROR, $2); }
	| I_ASR		expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ASR, $2); }
	| I_SWAP	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SWAP, $2); }
	| I_BSET	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BSET, $2); }
	| I_BCLR	expr		{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BCLR, $2); }
	| I_SBI		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBI, $2->addLink($4)); }
	| I_CBI		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CBI, $2->addLink($4)); }
	| I_BST		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BST, $2->addLink($4)); }
	| I_BLD		expr "," expr	{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BLD, $2->addLink($4)); }
	| I_SEC				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SEC); }
	| I_CLC				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLC); }
	| I_SEN				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SEN); }
	| I_CLN				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLN); }
	| I_SEZ				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SEZ); }
	| I_CLZ				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLZ); }
	| I_SEI				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SEI); }
	| I_CLI				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLI); }
	| I_SES				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SES); }
	| I_CLS				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLS); }
	| I_SEV				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SEV); }
	| I_CLV				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLV); }
	| I_SET				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SET); }
	| I_CLT				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLT); }
	| I_SEH				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SEH); }
	| I_CLH				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLH); }
	| I_NOP				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_NOP); }
	| I_SLEEP			{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SLEEP); }
	| I_WDR				{ $$ = new CompilerStatement(LOC(), ASMAVR8_INS_WDR); }
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison
inline int avr8parser_error(YYLTYPE * yylloc, yyscan_t, CompilerParserInterface * compiler, const char * errorInfo) {
	if ( 0 == strncmp(errorInfo , "syntax error, unexpected ", 25) ) {
		std::string errStr;
		if ( (strlen(errorInfo) > 27) && ('_' == errorInfo[26]) ) {
			switch ( errorInfo[25] ) {
				case 'D':
					errStr = QObject::tr("unexpected directive ").toStdString();
					errStr += reinterpret_cast<const char *>( long(errorInfo) + 27 );
					errorInfo = errStr.c_str();
					break;
				case 'I':
					errStr = QObject::tr("unexpected instruction ").toStdString();
					errStr += reinterpret_cast<const char *>( long(errorInfo) + 27 );
					errorInfo = errStr.c_str();
					break;
				case 'F':
					errStr = QObject::tr("unexpected function ").toStdString();
					errStr += reinterpret_cast<const char *>( long(errorInfo) + 27 );
					errorInfo = errStr.c_str();
					break;
				default:
					errorInfo += 14;
					break;
			}
		} else {
			errStr = QObject::tr("unexpected ").toStdString();
			errStr += reinterpret_cast<const char *>( long(errorInfo) + 25 );
			errorInfo = errStr.c_str();
		}
	} else if ( 0 == strncmp(errorInfo , "syntax error", 12) ) {
		errorInfo = QObject::tr("syntax not understood").toStdString().c_str();
	}

	compiler->parserMessage(compiler->toSourceLocation(yylloc), CompilerBase::MT_ERROR, errorInfo);
	return 0;
}
