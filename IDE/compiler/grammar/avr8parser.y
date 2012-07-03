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
/* %expect 0 */
// Expect exactly <n> reduce/reduce conflicts in this grammar
/* %expect-rr 0 */
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
/*
	// Error messages
	#define N_OPERANDS_EXPECTED(loc, ins, n) \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("invalid number of operands, instruction %1 takes %2 operand(s)").arg(ins).arg(n).toStdString() );
	#define MISSIGN_COMMA(loc, expr) \
		CompilerExpr::completeDelete(expr); \
		loc.first_line = loc.last_line; \
		loc.first_column = loc.last_column; \
		loc.last_column++; \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("comma (`,') expected between operands").toStdString() );
	#define REQ_FIRST_OPR(ins, opr, loc, expr) \
		CompilerExpr::completeDelete(expr); \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("the first operand of %1 is required to be one of these: { %2 }").arg(ins).arg(opr).toStdString() );
	#define NO_LABEL_EXPECTED(loc, directive, stmt) \
		CompilerStatement::completeDelete(stmt); \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("cannot declare a label before %1 directive").arg(directive).toStdString() );
	#define ARG_REQUIRED_C(loc, control) \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("assembler control %1 requires a single argument").arg(control).toStdString() );
	#define ARG_REQUIRED_D(loc, directive) \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("directive %1 requires a single argument").arg(directive).toStdString() );
	#define NO_ARG_EXPECTED_C(loc, control) \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("assembler control %1 takes no arguments").arg(control).toStdString() );
	#define NO_ARG_EXPECTED_D(directive, args, loc) \
		args->completeDelete(); \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("directive %1 takes no arguments").arg(directive).toStdString() );
	#define MISSING_AT_OPERATOR(loc, directive) \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("segment specifier %1 requires `AT' operator before the start address").arg(directive).toStdString() );
	#define DELC_ID_EXPECTED(loc, directive) \
		compiler->parserMessage ( loc, CompilerBase::MT_ERROR, \
			QObject::tr("directive %1 requires an identifier for the symbol (or macro) which it defines").arg(directive).toStdString() );
	#define DEPRECATED_DIR(loc, directive, substitute) \
		compiler->parserMessage ( loc, CompilerBase::MT_WARNING, \
			QObject::tr("%1 is a deprecated directive, consider usage of directive %2 instead").arg(directive).arg(substitute).toStdString() );
*/
	// Declaration of the error reporting function used by Bison
	inline int avr8parser_error (
		YYLTYPE * yylloc,
		yyscan_t yyscanner,
		CompilerParserInterface * compiler,
		const char * errorInfo );

	void avr8lexer_closeFile(yyscan_t yyscanner);
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
%token D_EXIT		D_INCLUDE	D_LIST		D_LISTMAC	D_NOLIST
%token D_ORG

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
%token I_SLEEP	I_WDR

/* Fixed operands, i.e. those which have no value */
%token R0	R1	R2	R3	R4	R5	R6	R7
%token R8	R9	R10	R11	R12	R13	R14	R15
%token R16	R17	R18	R19	R20	R21	R22	R23
%token R24	R25	R26	R27	R28	R29	R30	R31
%token X	Y	Z

/* Other terminal symbols */
%token COMMENT
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
%left "<" "<=" "=>" ">"
%left "<<" ">>"
%left "+" "-"
%left "*" "/" "%"
%left "++" "--"
// Right-to-left
%right "&=" "^=" "|="
%right "<<=" ">>="
%right "*=" "/=" "%="
%right "+=" "−="
%right "="
%right "~"

/* Terminal symbols with semantic value */
  // semantic value is a string
%token<string>	IDENFIFIER	LABEL
%token<array>	STRING		INCLUDE
  // semantic value is a number
%token<number>	NUMBER

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Integers
/* %type<number> */
// Expressions
/*
%type<expr>	expr		id		string		args_str	e_expr
%type<expr>	number		params		args		const_a		const_c
// Statements - general
%type<stmt>	statements	stmt		inst_stmt	dir_stmt	macro_stmt
%type<stmt>	instruction	directive	macro		label
// Statements - directives
%type<stmt>	dir_macro_d	dir_macro	dir_endm	dir_db		dir_include
%type<stmt>	dir_cond_asm	if_block	ifelse_block	else_block	dir_if
%type<stmt>	dir_ifn		dir_ifdef	dir_ifndef	dir_ifb		dir_ifnb
%type<stmt>	dir_elseif	dir_elseifn	dir_elseifdef	dir_elseifndf	dir_elseifb
%type<stmt>	dir_elseifnb	dir_rept	dir_dbit	dir_ds		dir_dw
%type<stmt>	dir_skip	dir_name	dir_local	dir_exitm	dir_list
%type<stmt>	dir_nolist	dir_repeat	dir_org		dir_bseg	dir_cseg
%type<stmt>	dir_dseg	dir_iseg	dir_xseg	dir_using	dir_data
%type<stmt>	dir_byte	dir_idata	dir_xdata	dir_code	dir_set
%type<stmt>	dir_equ	dir_bit	dir_flag	dir_if_a	dir_ifn_a	dir_ifdef_a
%type<stmt>	dir_ifndef_a	dir_ifb_a	dir_ifnb_a	dir_else	dir_elseif_a
%type<stmt>	dir_elseifn_a	dir_elseifdef_a	dir_elseifndf_a	dir_elseifb_a	dir_elseifnb_a
%type<stmt>	dir_macro_a	dir_bseg_a	dir_cseg_a	dir_dseg_a	dir_iseg_a
%type<stmt>	dir_xseg_a	end_of_code
// Statements - assembler controls
%type<stmt>	asm_ctrl_stmt	asm_ctrl	ctrl_cond	ctrl_nodebug	ctrl_nosymbols
%type<stmt>	ctrl_condonly	ctrl_notabs	ctrl_nogen	ctrl_save	ctrl_date
%type<stmt>	ctrl_noxref	ctrl_list	ctrl_nolist	ctrl_pagelength	ctrl_symbols
%type<stmt>	ctrl_debug	ctrl_macro	ctrl_nomacro	ctrl_pagewidth	ctrl_title
%type<stmt>	ctrl_paging	ctrl_eject	ctrl_mod51	ctrl_nomod51	ctrl_philips
%type<stmt>	ctrl_warning	ctrl_error	ctrl_nobuiltin	ctrl_nopaging	ctrl_xref
%type<stmt>	ctrl_gen	ctrl_nocond	ctrl_genonly	ctrl_restore
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
*/

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
