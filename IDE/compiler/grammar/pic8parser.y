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
%name-prefix "pic8parser_"
// Generate a parser header file containing macro definitions, etc.
%defines "pic8parser.h"
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
	#include "pic8lexer.h"

	// Name of the lexer function required by Bison
	#define pic8parser_lex pic8lexer_lex

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
	inline int pic8parser_error (
		YYLTYPE * yylloc,
		yyscan_t yyscanner,
		CompilerParserInterface * compiler,
		const char * errorInfo );

	void pic8lexer_closeFile(yyscan_t yyscanner);
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
%token D_DEFINE		D_UNDEFINE	D_CONSTANT	D_ORG		D_SET
%token D_PROCESSOR	D_RADIX		D_VARIABLE	D_WHILE		D___BADRAM
%token D___BADROM	D___CONFIG	D_CONFIG	D___IDLOCS	D___MAXRAM
%token D___MAXROM	D_CBLOCK	D_DA		D_DATA		D_DE
%token D_DT		D_DW		D_ENDC		D_FILL		D_RES
%token D_ERROR		D_ERRORLEVEL	D_LIST		D_MESSG		D_NOLIST
%token D_PAGE		D_SPACE		D_SUBTITLE	D_TITLE		D_EXPAND
%token D_NOEXPAND	D_ACCESS_OVR	D_BANKISEL	D_BANKSEL	D_CODE
%token D_CODE_PACK	D_EXTERN	D_GLOBAL	D_IDATA		D_IDATA_ACS
%token D_PAGESEL	D_PAGESELW	D_UDATA		D_UDATA_ACS	D_UDATA_OVR
%token D_UDATA_SHR	D_IF		D_IFN		D_IFDEF		D_IFNDEF
%token D_ELSEIFB	D_ELSEIFNB	D_ELSE		D_ELSEIF	D_ELSEIFN
%token D_ELSEIFDEF	D_ELSEIFNDEF	D_ENDIF		D_LOCAL		D_IFNB
%token D_IFB		D_SKIP		D_ENDMACRO	D_ENDM		D_EXITM
%token D_REPT		D_MACRO		D_EQU		D_ENDW		D_END

/* Instructions */
%token I_ADDWF		I_ANDWF		I_CLRF		I_CLRW		I_COMF
%token I_DECF		I_DECFSZ	I_INCF		I_INCFSZ	I_IORWF
%token I_MOVF		I_MOVWF		I_NOP		I_RLF		I_RRF
%token I_SUBWF		I_SWAPF		I_XORWF		I_BCF		I_BSF
%token I_BTFSC		I_BTFSS		I_ADDLW		I_ANDLW		I_CALL
%token I_CLRWDT		I_GOTO		I_IORLW		I_MOVLW		I_RETFIE
%token I_RETLW		I_RETURN	I_SLEEP		I_SUBLW		I_XORLW
%token I_OPTION		I_TRIS

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
%token F_V

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
inline int pic8parser_error(YYLTYPE * yylloc, yyscan_t, CompilerParserInterface * compiler, const char * errorInfo) {
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
