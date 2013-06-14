// =============================================================================
/**
 * @brief
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 */
// =============================================================================


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
%expect 796
// Expect exactly <n> reduce/reduce conflicts in this grammar
%expect-rr 0
/* Type of parser tables within the LR family, in this case we use LALR (Look-Ahead LR parser) */
%define lr.type lalr
/* Bison declaration to request verbose, specific error message strings when yyerror is called */
%error-verbose
/* Enable LAC (Lookahead Correction) to improve syntax error handling. */
%define parse.lac full

// Symbol semantic value
%union {
    int number;                     //
    char * string;                  //
    CompilerExpr * expr;            //
    CompilerStatement * stmt;       //
    struct {
        unsigned char * data;   //
        int size;               //
    } array;                        //
};

/*
 * Compiler interface for the syntax analyzer; we need to have some things declared
 * before we can declare other things, and this interface is quite essential here.
 */
%code requires {
    #include "CompilerParserInterface.h"
}

%{
    #include <QObject>              // For i18n, nothing else.
    #include <cstdlib>              // We need free() here.
    #include <iostream>             // This is required by Bison.
    using namespace std;            // This is required by Bison as well.
    using namespace CompilerStatementTypes; // This NS is heavily used here.

    // Declaration of the lexer prototypes and other things required by Bison
    #include "pic8lexer.h"

    // Name of the lexer function required by Bison
    #define pic8parser_lex pic8lexer_lex

    // Make a few things a little easier...
    #define LOC() \
        ( compiler->toSourceLocation(yylloc) )

    // Error messages
    #define N_OPERANDS_EXPECTED(location, instruction, number) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("invalid number of operands, instruction %1 takes %2 operand(s)").arg(instruction).arg(number).toStdString() );
    #define NO_LABEL_EXPECTED(location, directive, statement) \
        CompilerStatement::completeDelete(statement); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("cannot declare a label before %1 directive").arg(directive).toStdString() );
    #define ARG_REQUIRED_D(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 requires a single argument").arg(directive).toStdString() );
    #define ARGS_REQUIRED_D(location, directive, number) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 requires a %2 arguments").arg(directive).arg(number).toStdString() );
    #define NO_ARG_EXPECTED_D(directive, arguments, location) \
        arguments->completeDelete(); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 takes no arguments").arg(directive).toStdString() );
    #define DECL_ID_EXPECTED(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 requires an identifier for the symbol (or macro) which it defines").arg(directive).toStdString() );
    #define DEPRECATED_DIR(location, directive, substitute) \
        compiler->parserMessage ( location, CompilerBase::MT_WARNING, \
            QObject::tr("%1 is a deprecated directive, consider usage of directive %2 instead").arg(directive).arg(substitute).toStdString() );

    // Declaration of the error reporting function used by Bison
    inline int pic8parser_error (
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
%token D_DEFINE         D_UNDEFINE      D_CONSTANT      D_ORG           D_SET
%token D_PROCESSOR      D_RADIX         D_VARIABLE      D_WHILE         D___BADRAM
%token D___BADROM       D___CONFIG      D_CONFIG        D___IDLOCS      D___MAXRAM
%token D___MAXROM       D_CBLOCK        D_DA            D_DATA          D_DE
%token D_DT             D_DW            D_ENDC          D_FILL          D_RES
%token D_ERROR          D_ERRORLEVEL    D_LIST          D_MESSG         D_NOLIST
%token D_PAGE           D_DB            D_SUBTITLE      D_TITLE         D_EXPAND
%token D_NOEXPAND       D_ACCESS_OVR    D_BANKISEL      D_BANKSEL       D_CODE
%token D_CODE_PACK      D_EXTERN        D_GLOBAL        D_IDATA         D_IDATA_ACS
%token D_PAGESEL        D_PAGESELW      D_UDATA         D_UDATA_ACS     D_UDATA_OVR
%token D_UDATA_SHR      D_IF            D_IFN           D_IFDEF         D_IFNDEF
%token D_ELSEIFB        D_ELSEIFNB      D_ELSE          D_ELSEIF        D_ELSEIFN
%token D_ELSEIFDEF      D_ELSEIFNDEF    D_ENDIF         D_LOCAL         D_IFNB
%token D_IFB            D_SKIP          D_ENDMACRO      D_ENDM          D_EXITM
%token D_REPT           D_MACRO         D_EQU           D_ENDW          D_END
%token D_DC_IFDEF       D_DC_IFNDEF     D_DC_ENDIF

/* Instructions */
%token I_ADDWF          I_ANDWF         I_CLRF          I_CLRW          I_COMF
%token I_DECF           I_DECFSZ        I_INCF          I_INCFSZ        I_IORWF
%token I_MOVF           I_MOVWF         I_NOP           I_RLF           I_RRF
%token I_SUBWF          I_SWAPF         I_XORWF         I_BCF           I_BSF
%token I_BTFSC          I_BTFSS         I_ADDLW         I_ANDLW         I_CALL
%token I_CLRWDT         I_GOTO          I_IORLW         I_MOVLW         I_RETFIE
%token I_RETLW          I_RETURN        I_SLEEP         I_SUBLW         I_XORLW
%token I_OPTION         I_TRIS

/* Other terminal symbols */
%token COMMENT
%token EOL              "end of line"
%token END      0       "end of file"
// Named tokens (to make the grammar rules more readable)
%token LP               "("
%token RP               ")"
%token COMMA            ","
%token COLON            ":"
// Expression operators
%token INTERVAL         ".."
%token ASSIGN           "="
%token PLUS             "+"
%token MINUS            "-"
%token ASTERISK         "*"
%token SLASH            "/"
%token NOT              "!"
%token MOD              "%"
%token SHL              "<<"
%token SHR              ">>"
%token AND              "&&"
%token OR               "||"
%token XOR              "^^"
%token BITAND           "&"
%token BITOR            "|"
%token BITXOR           "^"
%token EQ               "=="
%token NE               "!="
%token LT               "<"
%token LE               "<="
%token GT               ">"
%token GE               ">="
%token COMPLEMET        "~"
%token ADD_ASSIGN       "+="
%token SUB_ASSIGN       "-="
%token MUL_ASSIGN       "*="
%token DIV_ASSIGN       "/="
%token MOD_ASSIGN       "%="
%token SHL_ASSIGN       "<<="
%token SHR_ASSIGN       ">>="
%token AND_ASSIGN       "&="
%token ORB_ASSIGN       "|="
%token XOR_ASSIGN       "^="
%token INCREMENT        "++"
%token DECREMENT        "--"

// Expression functions
%token F_HIGH           F_LOW           F_BYTE2         F_BYTE3         F_BYTE4
%token F_LWRD           F_HWRD          F_PAGE          F_EXP2          F_LOG2
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
%token<string>  IDENTIFIER      LABEL
%token<array>   STRING          INCLUDE
  // semantic value is a number
%token<number>  NUMBER

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Expressions

%type<expr>     expr            "expression"
%type<expr>     number          params          args            opt             opts
%type<expr>     err_lvls        id              string          args_str        intervals
// Statements - general
%type<stmt>     statements      stmt            inst_stmt       dir_stmt        macro_stmt
%type<stmt>     instruction     directive       macro           label
// Statements - directives
%type<stmt>     dir_cond_asm    dir_skip        dir_local       dir_exitm       dir_list
%type<stmt>     dir_include     dir_repeat      dir_org         dir_end         dir_idata_acs_a
%type<stmt>     dir_set         dir_equ         dir_macro       dir_if          dir_ifn
%type<stmt>     dir_ifdef       dir_ifndef      dir_ifb         dir_ifnb        dir_elseifnb
%type<stmt>     dir_elseif      dir_elseifn     dir_elseifdef   dir_elseifndf   dir_elseifb
%type<stmt>     dir_constant    dir_variable    dir_processor   dir_radix       dir_while
%type<stmt>     dir_while_a     dir_endw        dir___badram    dir___badrom    dir_error
%type<stmt>     dir___config    dir_config      dir___idlocs    dir___maxram    dir___maxrom
%type<stmt>     dir_cblock      dir_da          dir_data        dir_db          dir_de
%type<stmt>     dir_dt          dir_dw          dir_fill        dir_res         dir_messg
%type<stmt>     dir_subtitle    dir_title       dir_errorlevel  dir_page        dir_nolist
%type<stmt>     dir_expand      dir_noexpand    dir_access_ovr  dir_code        dir_access_ovr_a
%type<stmt>     dir_code_pack   dir_extern      dir_global      dir_bankisel    dir_banksel
%type<stmt>     dir_pagesel     dir_pageselw    dir_udata       dir_udata_acs   dir_udata_ovr
%type<stmt>     dir_udata_shr   dir_idata       dir_idata_acs   dir_cblock_a    label_list
%type<stmt>     dir_code_a      dir_code_pack_a dir_udata_a     dir_udata_acs_a label_list_ml
%type<stmt>     dir_udata_ovr_a dir_udata_shr_a dir_idata_a     if_block        ifelse_block
%type<stmt>     else_block      dir_else        dir_if_a        dir_ifn_a       dir_ifdef_a
%type<stmt>     dir_ifndef_a    dir_ifb_a       dir_ifnb_a      dir_elseif_a    dir_elseifn_a
%type<stmt>     dir_elseifdef_a dir_elseifndf_a dir_elseifb_a   dir_elseifnb_a  dir_rept
%type<stmt>     dir_endm        dir_macro_d     dir_macro_a     dir___config_a  dir___idlocs_a
%type<stmt>     dir_da_a        dir_data_a      dir_db_a        dir_de_a        dir_dt_a
%type<stmt>     dir_dw_a        dir_cblock_b    dir_res_a       dir_fill_a      dir_page_a
%type<stmt>     dir_expand_a    dir_noexpand_a  dir_define      dir_undefine    dir_dc_ifdef
%type<stmt>     dir_dc_ifndef   dir_dc_endif    dir_define_a    dir_undefine_a  dir_dc_ifdef_a
%type<stmt>     dir_dc_ifndef_a dir_dc_endif_a
// Statements - instructions
%type<stmt>     ins_addwf       ins_andwf       ins_clrf        ins_clrw        ins_comf
%type<stmt>     ins_decf        ins_decfsz      ins_incf        ins_incfsz      ins_iorwf
%type<stmt>     ins_movf        ins_movwf       ins_nop         ins_rlf         ins_rrf
%type<stmt>     ins_subwf       ins_swapf       ins_xorwf       ins_bcf         ins_bsf
%type<stmt>     ins_btfsc       ins_btfss       ins_addlw       ins_andlw       ins_call
%type<stmt>     ins_clrwdt      ins_goto        ins_iorlw       ins_movlw       ins_retfie
%type<stmt>     ins_retlw       ins_return      ins_sleep       ins_sublw       ins_xorlw
%type<stmt>     ins_option      ins_tris

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
      statements                    { compiler->syntaxAnalysisComplete($statements); YYACCEPT; }
;
statements:
      stmt                          { $$ = $stmt; }
    | error                         { $$ = NULL; }
    | statements EOL stmt           { $$ = $1->appendLink($stmt); }
    | statements error EOL stmt     { $$ = $1->appendLink($stmt); }
;
stmt:
      /* empty */                   { $$ = NULL; }
    | COMMENT                       { $$ = NULL; }
    | label                         { $$ = $1; }
    | label COMMENT                 { $$ = $1; }
    | dir_stmt                      { $$ = $1; }
    | inst_stmt                     { $$ = $1; }
    | macro_stmt                    { $$ = $1; }
;
dir_stmt:
      directive                     { $$ = $directive; }
    | directive COMMENT             { $$ = $directive; }
;
inst_stmt:
      instruction                   { $$ = $instruction; }
    | instruction COMMENT           { $$ = $instruction; }
    | label instruction             { $$ = $label->appendLink($instruction); }
    | label instruction COMMENT     { $$ = $label->appendLink($instruction); }
;
macro_stmt:
      macro                         { $$ = $macro; }
    | macro COMMENT                 { $$ = $macro; }
    | label macro                   { $$ = $label->appendLink($macro); }
    | label macro COMMENT           { $$ = $label->appendLink($macro); }
;
macro:
      id                            { $$ = new CompilerStatement(LOC(), ASMPIC8_MACRO, $id); }
    | id "(" ")"                    { $$ = new CompilerStatement(LOC(), ASMPIC8_MACRO, $id); }
    | id args                       { $$ = new CompilerStatement(LOC(), ASMPIC8_MACRO, $id->appendLink($args)); }
    | id "(" args ")"               { $$ = new CompilerStatement(LOC(), ASMPIC8_MACRO, $id->appendLink($args)); }
;

/*
 * Generally used non-terminal symbols
 */
number:
      NUMBER                        { $$ = new CompilerExpr($1); }
;
id:
      IDENTIFIER                    { $$ = new CompilerExpr($1); }
;
string:
      STRING                        { $$ = new CompilerExpr(CompilerValue($1.data, $1.size)); }
;
label:
      LABEL                         { $$ = new CompilerStatement(LOC(), ASMPIC8_LABEL, new CompilerExpr($1)); }
;
expr:
      id                            { $$ = $id; }
    | number                        { $$ = $number; }
    | "(" expr ")"                  { $$ = $2; }
    | F_LOW "(" expr ")"            { $$ = new CompilerExpr(CompilerExpr::OPER_LOW, $3); }
    | F_HIGH "(" expr ")"           { $$ = new CompilerExpr(CompilerExpr::OPER_HIGH, $3); }
    | F_BYTE2 "(" expr ")"          { $$ = new CompilerExpr("byte2", 'C', $3); }
    | F_BYTE3 "(" expr ")"          { $$ = new CompilerExpr("byte3", 'C', $3); }
    | F_BYTE4 "(" expr ")"          { $$ = new CompilerExpr("byte4", 'C', $3); }
    | F_LWRD "(" expr ")"           { $$ = new CompilerExpr("lwrd", 'C', $3); }
    | F_HWRD "(" expr ")"           { $$ = new CompilerExpr("hwrd", 'C', $3); }
    | F_PAGE "(" expr ")"           { $$ = new CompilerExpr("page", 'C', $3); }
    | F_EXP2 "(" expr ")"           { $$ = new CompilerExpr("exp2", 'C', $3); }
    | F_LOG2 "(" expr ")"           { $$ = new CompilerExpr("log2", 'C', $3); }
    | F_V "(" expr ")"              { $$ = new CompilerExpr("v", 'C', $3); }
    | "~" expr                      { $$ = new CompilerExpr('~', $2); }
    | "!" expr                      { $$ = new CompilerExpr('!', $2); }
    | expr "+" expr                 { $$ = new CompilerExpr($1, '+', $3); }
    | expr "-" expr                 { $$ = new CompilerExpr($1, '-', $3); }
    | expr "*" expr                 { $$ = new CompilerExpr($1, '*', $3); }
    | expr "/" expr                 { $$ = new CompilerExpr($1, '/', $3); }
    | expr "%" expr                 { $$ = new CompilerExpr($1, '%', $3); }
    | expr "||" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LOR, $3); }
    | expr "^^" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LXOR, $3); }
    | expr "&&" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LAND, $3); }
    | expr "|" expr                 { $$ = new CompilerExpr($1, '|', $3); }
    | expr "^" expr                 { $$ = new CompilerExpr($1, '^', $3); }
    | expr "&" expr                 { $$ = new CompilerExpr($1, '&', $3); }
    | expr "==" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_EQ, $3); }
    | expr "!=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_NE, $3); }
    | expr "<" expr                 { $$ = new CompilerExpr($1, '<', $3); }
    | expr "<=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LE, $3); }
    | expr ">=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_GE, $3); }
    | expr ">" expr                 { $$ = new CompilerExpr($1, '>', $3); }
    | expr ">>" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_SHR, $3); }
    | expr "<<" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_SHL, $3); }
    | expr "+=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_ADD_ASSIGN, $3); }
    | expr "-=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_SUB_ASSIGN, $3); }
    | expr "*=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_MUL_ASSIGN, $3); }
    | expr "/=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_DIV_ASSIGN, $3); }
    | expr "%=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_MOD_ASSIGN, $3); }
    | expr "<<=" expr               { $$ = new CompilerExpr($1, CompilerExpr::OPER_SHL_ASSIGN, $3); }
    | expr ">>=" expr               { $$ = new CompilerExpr($1, CompilerExpr::OPER_SHR_ASSIGN, $3); }
    | expr "&=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_AND_ASSIGN, $3); }
    | expr "|=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_ORB_ASSIGN, $3); }
    | expr "^=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_XOR_ASSIGN, $3); }
    | "++" expr                     { $$ = new CompilerExpr(CompilerExpr::OPER_INC, $2); }
    | "--" expr                     { $$ = new CompilerExpr(CompilerExpr::OPER_DEC, $2); }
    | expr "++"                     { $$ = new CompilerExpr($1, CompilerExpr::OPER_INC); }
    | expr "--"                     { $$ = new CompilerExpr($1, CompilerExpr::OPER_DEC); }
    | expr expr                     {
                        /* Syntax error */
                        $$ = $1->appendLink($2);
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
args:           // List of arguments without strings, e.g. `(1+3), XYZ, 0x4b'
      args "," expr         { $$ = $1->appendLink($expr); }
    | expr                  { $$ = $expr; }
;
args_str:       // List of arguments possibly with strings, e.g. `ABC, "XYZ - QWE", 2 * 0x4b'
      args_str "," string           { $$ = $1->appendLink($string); }
    | args_str "," expr             { $$ = $1->appendLink($expr); }
    | string                        { $$ = $string; }
    | expr                  { $$ = $expr; }
;
params:         // List of identifiers, e.g. `target, source, offset, size, mode'
      params "," id                 { $$ = $1->appendLink($id); }
    | id                            { $$ = $id; }
;

/*
 * Directives
 */
directive:
      dir_cond_asm  { $$ = $1; }    | dir_skip      { $$ = $1; }
    | dir_local     { $$ = $1; }    | dir_exitm     { $$ = $1; }
    | dir_list      { $$ = $1; }    | dir_include   { $$ = $1; }
    | dir_nolist    { $$ = $1; }    | dir_repeat    { $$ = $1; }
    | dir_org       { $$ = $1; }    | dir_end       { $$ = $1; }
    | dir_set       { $$ = $1; }    | dir_equ       { $$ = $1; }
    | dir_macro     { $$ = $1; }    | dir_constant  { $$ = $1; }
    | dir_variable  { $$ = $1; }    | dir_processor { $$ = $1; }
    | dir_radix     { $$ = $1; }    | dir_while     { $$ = $1; }
    | dir___badram  { $$ = $1; }    | dir___badrom  { $$ = $1; }
    | dir___config  { $$ = $1; }    | dir_config    { $$ = $1; }
    | dir___idlocs  { $$ = $1; }    | dir___maxram  { $$ = $1; }
    | dir___maxrom  { $$ = $1; }    | dir_cblock    { $$ = $1; }
    | dir_da        { $$ = $1; }    | dir_data      { $$ = $1; }
    | dir_db        { $$ = $1; }    | dir_de        { $$ = $1; }
    | dir_dt        { $$ = $1; }    | dir_dw        { $$ = $1; }
    | dir_fill      { $$ = $1; }    | dir_res       { $$ = $1; }
    | dir_messg     { $$ = $1; }    | dir_subtitle  { $$ = $1; }
    | dir_title     { $$ = $1; }    | dir_errorlevel{ $$ = $1; }
    | dir_page      { $$ = $1; }    | dir_access_ovr{ $$ = $1; }
    | dir_expand    { $$ = $1; }    | dir_noexpand  { $$ = $1; }
    | dir_code      { $$ = $1; }    | dir_code_pack { $$ = $1; }
    | dir_extern    { $$ = $1; }    | dir_global    { $$ = $1; }
    | dir_bankisel  { $$ = $1; }    | dir_banksel   { $$ = $1; }
    | dir_pagesel   { $$ = $1; }    | dir_pageselw  { $$ = $1; }
    | dir_udata     { $$ = $1; }    | dir_udata_acs { $$ = $1; }
    | dir_udata_ovr { $$ = $1; }    | dir_udata_shr { $$ = $1; }
    | dir_idata     { $$ = $1; }    | dir_idata_acs { $$ = $1; }
    | dir_error     { $$ = $1; }    | dir_define    { $$ = $1; }
    | dir_undefine  { $$ = $1; }    | dir_dc_ifdef  { $$ = $1; }
    | dir_dc_ifndef { $$ = $1; }    | dir_dc_endif  { $$ = $1; }
;
dir_cond_asm:
      if_block ifelse_block else_block D_ENDIF {
        $$ = new CompilerStatement(LOC(), ASMPIC8_COND_ASM);
        $$->createBranch ( $if_block->appendLink($ifelse_block)->appendLink($else_block) );
    }
    | label if_block ifelse_block else_block D_ENDIF {
        $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_COND_ASM));
        $$->createBranch ( $if_block->appendLink($ifelse_block)->appendLink($else_block) );
    }
;
if_block:
      dir_if        statements EOL  { $$ = $1->createBranch($2); }
    | dir_ifn       statements EOL  { $$ = $1->createBranch($2); }
    | dir_ifdef     statements EOL  { $$ = $1->createBranch($2); }
    | dir_ifndef    statements EOL  { $$ = $1->createBranch($2); }
    | dir_ifb       statements EOL  { $$ = $1->createBranch($2); }
    | dir_ifnb      statements EOL  { $$ = $1->createBranch($2); }
    | dir_if                        { $$ = $1; }
    | dir_ifn                       { $$ = $1; }
    | dir_ifdef                     { $$ = $1; }
    | dir_ifndef                    { $$ = $1; }
    | dir_ifb                       { $$ = $1; }
    | dir_ifnb                      { $$ = $1; }
;
ifelse_block:
      /* empty */                   { $$ = NULL; }
    | dir_elseif    statements EOL  { $$ = $1->createBranch($2); }
    | dir_elseifn   statements EOL  { $$ = $1->createBranch($2); }
    | dir_elseifdef statements EOL  { $$ = $1->createBranch($2); }
    | dir_elseifndf statements EOL  { $$ = $1->createBranch($2); }
    | dir_elseifb   statements EOL  { $$ = $1->createBranch($2); }
    | dir_elseifnb  statements EOL  { $$ = $1->createBranch($2); }
    | dir_elseif                    { $$ = $1; }
    | dir_elseifn                   { $$ = $1; }
    | dir_elseifdef                 { $$ = $1; }
    | dir_elseifndf                 { $$ = $1; }
    | dir_elseifb                   { $$ = $1; }
    | dir_elseifnb                  { $$ = $1; }
;
else_block:
      /* empty */                   { $$ = NULL; }
    | dir_else statements EOL       { $$ = $dir_else->createBranch($statements); }
    | dir_else                      { $$ = $dir_else; }
;
dir_else:
      D_ELSE                        { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ELSE); }
    | label D_ELSE                  {
                        /* Syntax error */
                        NO_LABEL_EXPECTED(@label, "ELSE", $label);
                        $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ELSE);
                    }
    | label D_ELSE args             {
                        /* Syntax error */
                        NO_ARG_EXPECTED_D("ELSE", $args, @args);
                        NO_LABEL_EXPECTED(@label, "ELSE", $label);
                        $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ELSE);
                    }
    | D_ELSE args                   {
                        /* Syntax error */
                        NO_ARG_EXPECTED_D("ELSE", $args, @args);
                        $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ELSE);
                    }
;
dir_if:
      dir_if_a EOL                  { $$ = $dir_if_a; }
    | dir_if_a COMMENT EOL          { $$ = $dir_if_a; }
;
dir_if_a:
      D_IF expr                     { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IF, $expr); }
    | D_IF                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IF, "IF"); }
;
dir_ifn:
      dir_ifn_a EOL                 { $$ = $dir_ifn_a; }
    | dir_ifn_a COMMENT EOL         { $$ = $dir_ifn_a; }
;
dir_ifn_a:
      D_IFN expr                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IFN, $expr); }
    | D_IFN                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFN, "IFN"); }
;
dir_ifdef:
      dir_ifdef_a EOL               { $$ = $dir_ifdef_a; }
    | dir_ifdef_a COMMENT EOL       { $$ = $dir_ifdef_a; }
;
dir_ifdef_a:
      D_IFDEF id                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IFDEF, $id); }
    | D_IFDEF                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFDEF, "IFDEF"); }
;
dir_ifndef:
      dir_ifndef_a EOL              { $$ = $dir_ifndef_a; }
    | dir_ifndef_a COMMENT EOL      { $$ = $dir_ifndef_a; }
;
dir_ifndef_a:
      D_IFNDEF id                   { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IFNDEF, $id); }
    | D_IFNDEF                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFNDEF, "IFNDEF"); }
;
dir_ifb:
      dir_ifb_a EOL                 { $$ = $dir_ifb_a; }
    | dir_ifb_a COMMENT EOL         { $$ = $dir_ifb_a; }
;
dir_ifb_a:
      D_IFB id                      { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IFB, $id); }
    | D_IFB                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFB, "IFB"); }
;
dir_ifnb:
      dir_ifnb_a EOL                { $$ = $dir_ifnb_a; }
    | dir_ifnb_a COMMENT EOL        { $$ = $dir_ifnb_a; }
;
dir_ifnb_a:
      D_IFNB id                     { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IFNB, $id); }
    | D_IFNB                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFNB, "IFNB"); }
;
dir_elseif:
      dir_elseif_a EOL              { $$ = $dir_elseif_a; }
    | dir_elseif_a COMMENT EOL      { $$ = $dir_elseif_a; }
;
dir_elseif_a:
      D_ELSEIF expr                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ELSEIF, $expr); }
    | D_ELSEIF                      { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIF, "ELSEIF"); }
    | label D_ELSEIF expr           { /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIF", $label); $expr->completeDelete(); }
    | label D_ELSEIF                {
                        /* Syntax Error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_ELSEIF, "ELSEIF");
                        NO_LABEL_EXPECTED(@label, "ELSEIF", $label);
                    }
;
dir_elseifn:
      dir_elseifn_a EOL             { $$ = $dir_elseifn_a; }
    | dir_elseifn_a COMMENT EOL     { $$ = $dir_elseifn_a; }
;
dir_elseifn_a:
      D_ELSEIFN expr                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ELSEIFN, $expr); }
    | D_ELSEIFN                     { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFN, "ELSEIFN"); }
    | label D_ELSEIFN expr          {
                        /* Syntax Error */
                        $$ = NULL;
                        NO_LABEL_EXPECTED(@label, "ELSEIFN", $label);
                        $expr->completeDelete();
                    }
    | label D_ELSEIFN               {
                        /* Syntax Error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_ELSEIFN, "ELSEIFN");
                        NO_LABEL_EXPECTED(@label, "ELSEIFN", $label);
                    }
;
dir_elseifdef:
      dir_elseifdef_a EOL           { $$ = $dir_elseifdef_a; }
    | dir_elseifdef_a COMMENT EOL   { $$ = $dir_elseifdef_a; }
;
dir_elseifdef_a:
      D_ELSEIFDEF expr              { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ELSEIFDEF, $expr); }
    | D_ELSEIFDEF                   { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFDEF, "ELSEIFDEF"); }
    | label D_ELSEIFDEF expr        { /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFDEF", $label); $expr->completeDelete(); }
    | label D_ELSEIFDEF             {
                        /* Syntax Error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_ELSEIFDEF, "ELSEIFDEF");
                        NO_LABEL_EXPECTED(@label, "ELSEIFDEF", $label);
                    }
;
dir_elseifndf:
      dir_elseifndf_a EOL           { $$ = $dir_elseifndf_a; }
    | dir_elseifndf_a COMMENT EOL   { $$ = $dir_elseifndf_a; }
;
dir_elseifndf_a:
      D_ELSEIFNDEF expr             { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ELSEIFNDEF, $expr); }
    | D_ELSEIFNDEF                  { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFNDEF, "ELSEIFNDEF"); }
    | label D_ELSEIFNDEF expr       { /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFNDEF", $label); $expr->completeDelete(); }
    | label D_ELSEIFNDEF            {
                        /* Syntax Error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_ELSEIFNDEF, "ELSEIFNDEF");
                        NO_LABEL_EXPECTED(@label, "ELSEIFNDEF", $label);
                    }
;
dir_elseifb:
      dir_elseifb_a EOL             { $$ = $dir_elseifb_a; }
    | dir_elseifb_a COMMENT EOL     { $$ = $dir_elseifb_a; }
;
dir_elseifb_a:
      D_ELSEIFB expr                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ELSEIFB, $expr); }
    | D_ELSEIFB                     { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFB, "ELSEIFB"); }
    | label D_ELSEIFB expr          {
                        /* Syntax Error */
                        $$ = NULL;
                        NO_LABEL_EXPECTED(@label, "ELSEIFB", $label);
                        $expr->completeDelete();
                    }
    | label D_ELSEIFB               {
                        /* Syntax Error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_ELSEIFB, "ELSEIFB");
                        NO_LABEL_EXPECTED(@label, "ELSEIFB", $label);
                    }
;
dir_elseifnb:
      dir_elseifnb_a EOL            { $$ = $dir_elseifnb_a; }
    | dir_elseifnb_a COMMENT EOL    { $$ = $dir_elseifnb_a; }
;
dir_elseifnb_a:
      D_ELSEIFNB expr               { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ELSEIFNB, $expr); }
    | D_ELSEIFNB                    { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFNB, "ELSEIFNB"); }
    | label D_ELSEIFNB expr         {
                        /* Syntax Error */
                        $$ = NULL;
                        NO_LABEL_EXPECTED(@label, "ELSEIFNB", $label);
                        $expr->completeDelete();
                    }
    | label D_ELSEIFNB              {
                        /* Syntax Error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_ELSEIFNB, "ELSEIFNB");
                        NO_LABEL_EXPECTED(@label, "ELSEIFNB", $label);
                    }
;
dir_org:
      D_ORG expr                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ORG, $expr); }
    | label D_ORG expr              { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_ORG, $expr)); }
    | D_ORG                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_ORG, "ORG"); }
    | label D_ORG                   { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_ORG, "ORG"); $label->completeDelete(); }
;
dir_skip:
      D_SKIP expr                   { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_SKIP, $expr); }
    | label D_SKIP expr             { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_SKIP, $expr)); }
    | D_SKIP                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_SKIP, "SKIP"); }
    | label D_SKIP                  { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_SKIP, "SKIP"); $label->completeDelete(); }
;
dir_local:
      D_LOCAL id                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_LOCAL, $id); }
    | label D_LOCAL id              { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_LOCAL, $id)); }
    | D_LOCAL                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_LOCAL, "LOCAL"); }
    | label D_LOCAL                 { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_LOCAL, "LOCAL"); $label->completeDelete(); }
;
dir_exitm:
      D_EXITM                       { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_EXITM); }
    | label D_EXITM                 { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_EXITM)); }
    | D_EXITM args                  { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("EXITM", $args, @args); }
    | label D_EXITM args            { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("EXITM", $args, @args); $label->completeDelete(); }
;
dir_nolist:
      D_NOLIST                      { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_NOLIST); }
    | label D_NOLIST                { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_NOLIST)); }
    | D_NOLIST args                 { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("NOLIST", $args, @args); }
    | label D_NOLIST args           { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("NOLIST", $args, @args); $label->completeDelete(); }
;
dir_set:
      id D_SET expr                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_SET, $id->appendLink($expr)); }
    | id D_SET                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_SET, "SET"); $id->completeDelete(); }
    | D_SET                         { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_SET, "SET"); ARG_REQUIRED_D(@D_SET, "SET"); }
    | D_SET expr                    { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_SET, "SET"); $expr->completeDelete(); }
    | label D_SET expr              { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_SET, "SET"); NO_LABEL_EXPECTED(@label, "SET", $label->appendArgsLink($expr)) }
    | label id D_SET expr           { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "SET", $label->appendArgsLink($id->appendLink($expr))); }
;
dir_equ:
      id D_EQU expr                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_EQU, $id->appendLink($expr)); }
    | id D_EQU                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_EQU, "EQU"); $id->completeDelete(); }
    | D_EQU                         { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_EQU, "EQU"); ARG_REQUIRED_D(@D_EQU, "EQU"); }
    | D_EQU expr                    { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_EQU, "EQU"); $expr->completeDelete(); }
    | label D_EQU expr              { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_EQU, "EQU"); NO_LABEL_EXPECTED(@label, "EQU", $label->appendArgsLink($expr)) }
    | label id D_EQU expr           { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "EQU", $label->appendArgsLink($id->appendLink($expr))); }
;
dir_repeat:
      dir_rept
      statements EOL
      dir_endm                      { $$ = $dir_rept->createBranch($statements->appendLink($dir_endm)); }
    | dir_rept
      dir_endm                      { $$ = $dir_rept->createBranch($dir_endm); }
;
dir_rept:
      D_REPT expr COMMENT           { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_REPT, $expr); }
    | D_REPT expr                   { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_REPT, $expr); }
    | D_REPT COMMENT                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_REPT); }
    | label D_REPT expr COMMENT     { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_REPT, $expr)); }
    | label D_REPT expr             { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_REPT, $expr)); }
    | D_REPT                        { /* Syntax error */ $$ = NULL;   ARG_REQUIRED_D(@D_REPT, "REPT (TIMES)"); }
    | label D_REPT COMMENT          { /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_REPT, "REPT (TIMES)"); }
    | label D_REPT                  { /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_REPT, "REPT (TIMES)"); }
;
dir_macro:
      dir_macro_d
      statements EOL
      dir_endm                      { $$ = $dir_macro_d->createBranch($statements->appendLink($dir_endm)); }
    | dir_macro_d
      dir_endm                      { $$ = $dir_macro_d->createBranch($dir_endm); }
;
dir_macro_d:
      dir_macro_a EOL               { $$ = $dir_macro_a; }
    | dir_macro_a COMMENT EOL       { $$ = $dir_macro_a; }
    | dir_macro_a params EOL        { $$ = $dir_macro_a->appendArgsLink($params); }
    | dir_macro_a params COMMENT EOL{ $$ = $dir_macro_a->appendArgsLink($params); }
;
dir_macro_a:
      id D_MACRO                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_MACRO, $id); }
    | D_MACRO                       { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_MACRO, "MACRO"); }
    | label D_MACRO                 {
                        /* Syntax error */
                        $$ = NULL;
                        NO_LABEL_EXPECTED(@label, "MACRO", $label);
                        DECL_ID_EXPECTED(@D_MACRO, "MACRO");
                    }
;
dir_endm:
      dir_endm_a COMMENT            { $$ = NULL; }
    | dir_endm_a                    { $$ = NULL; }
    | label dir_endm_a COMMENT      { $$ = $label; }
    | label dir_endm_a              { $$ = $label; }
;
dir_endm_a:
      D_ENDM                        { }
    | D_ENDM args                   { /* Syntax error */ NO_ARG_EXPECTED_D("ENDM", $args, @args); }
;
dir_include:
      INCLUDE                       {
                        $$ = new CompilerStatement (
                            LOC(),
                            ASMPIC8_INCLUDE,
                            new CompilerExpr (
                                CompilerValue($INCLUDE.data, $INCLUDE.size)
                            )
                        );
                    }
    | label INCLUDE                 {
                        $$ = $label->appendLink (
                            new CompilerStatement (
                                LOC(),
                                ASMPIC8_INCLUDE,
                                new CompilerExpr (
                                    CompilerValue ( $INCLUDE.data, $INCLUDE.size )
                                )
                            )
                        );
                    }
;
dir_end:
      D_END                         { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_END); }
    | label D_END                   { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_END)); }
    | D_END args                    {
                        /* Syntax error */
                        NO_ARG_EXPECTED_D("END", $args, @args);
                        $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_END);
                    }
    | label D_END args              {
                        /* Syntax error */
                        NO_ARG_EXPECTED_D("END", $args, @args);
                        $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_END));
                    }
;
dir_constant:
      D_CONSTANT id "=" expr        { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CONSTANT, $id->appendLink($expr)); }
    | label D_CONSTANT id "=" expr  { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_CONSTANT, $id->appendLink($expr))); }
    | D_CONSTANT expr               {
                        /* Syntax error */
                        $$ = NULL;
                        DECL_ID_EXPECTED(@D_CONSTANT, "CONSTANT");
                        $expr->completeDelete();
                    }
    | label D_CONSTANT expr         {
                        /* Syntax error */
                        $$ = NULL;
                        DECL_ID_EXPECTED(@D_CONSTANT, "CONSTANT");
                        $label->completeDelete();
                        $expr->completeDelete();
                    }
;
dir_variable:
      D_VARIABLE id "=" expr        { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_VARIABLE, $id->appendLink($expr)); }
    | label D_VARIABLE id "=" expr  { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_VARIABLE, $id->appendLink($expr))); }
    | D_VARIABLE expr               {
                        /* Syntax error */
                        $$ = NULL;
                        DECL_ID_EXPECTED(@D_VARIABLE, "VARIABLE");
                        $expr->completeDelete();
                    }
    | label D_VARIABLE expr         {
                        /* Syntax error */
                        $$ = NULL;
                        DECL_ID_EXPECTED(@D_VARIABLE, "VARIABLE");
                        $label->completeDelete();
                        $expr->completeDelete();
                    }
;
dir_processor:
      D_PROCESSOR id                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_PROCESSOR, $id); }
    | label D_PROCESSOR id          { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_PROCESSOR, $id)); }
    | D_PROCESSOR                   { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_PROCESSOR, "PROCESSOR"); }
    | label D_PROCESSOR             { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_PROCESSOR, "PROCESSOR"); $label->completeDelete(); }
;
dir_radix:
      D_RADIX id                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_RADIX, $id); }
    | label D_RADIX id              { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_RADIX, $id)); }
    | D_RADIX                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_RADIX, "RADIX"); }
    | label D_RADIX                 { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_RADIX, "RADIX"); $label->completeDelete(); }
;
dir_while:
      dir_while_a
      statements EOL
      dir_endw                      { $$ = $dir_while_a->createBranch($statements->appendLink($dir_endw)); }
    | dir_while_a
      dir_endw                      { $$ = $dir_while_a->createBranch($dir_endw); }
;
dir_while_a:
      D_WHILE expr COMMENT          { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_WHILE, $expr); }
    | D_WHILE expr                  { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_WHILE, $expr); }
    | D_WHILE COMMENT               { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_WHILE); }
    | label D_WHILE expr COMMENT    { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_WHILE, $expr)); }
    | label D_WHILE expr            { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_WHILE, $expr)); }
    | D_WHILE                       { /* Syntax error */ $$ = NULL;   ARG_REQUIRED_D(@D_WHILE, "WHILE"); }
    | label D_WHILE COMMENT         { /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_WHILE, "WHILE"); }
    | label D_WHILE                 { /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_WHILE, "WHILE"); }
;
dir_endw:
      dir_endw_a COMMENT            { $$ = NULL; }
    | dir_endw_a                    { $$ = NULL; }
    | label dir_endw_a COMMENT      { $$ = $label; }
    | label dir_endw_a              { $$ = $label; }
;
dir_endw_a:
      D_ENDW                        { }
    | D_ENDW args                   { /* Syntax error */ NO_ARG_EXPECTED_D("ENDM", $args, @args); }
;
intervals:
      intervals "," expr ".." expr  { $$ = $1->appendLink(new CompilerExpr($3, CompilerExpr::OPER_INTERVALS, $5)); }
    | intervals "," expr            { $$ = $1->appendLink(new CompilerExpr($3, CompilerExpr::OPER_INTERVALS, $3->copyEntireChain())); }
    | expr ".." expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_INTERVALS, $3); }
    | expr                          { $$ = new CompilerExpr($1, CompilerExpr::OPER_INTERVALS, $1->copyEntireChain()); }
    | /* empty */                   {
                        /* Syntax error */
                        $$ = NULL;
                        compiler->parserMessage (
                            yylloc, CompilerBase::MT_ERROR,
                            QObject::tr("missing address interval").toStdString() );
                    }
;
dir___badram:
      D___BADRAM intervals          { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR___BADRAM, $intervals); }
    | label D___BADRAM intervals    { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR___BADRAM, $intervals)); }
;
dir___badrom:
      D___BADROM intervals          { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR___BADROM, $intervals); }
    | label D___BADROM intervals    { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR___BADROM, $intervals)); }
;
dir___config:
      dir___config_a                { $$ = $dir___config_a; }
    | label dir___config_a          { $$ = $label->appendLink($dir___config_a); }
;
dir___config_a:
      D___CONFIG expr               { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR___CONFIG, $expr); }
    | D___CONFIG expr "," expr      { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR___CONFIG, $2->appendLink($4)); }
    | D___CONFIG                    { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D___CONFIG, "__CONFIG"); }
;
dir_config:
    D_CONFIG                        { $$ = NULL; /* TODO: make this supported */}
;
dir_define:
      dir_define_a                  { $$ = $dir_define_a; }
    | label dir_define_a            { /* Syntax error */ $$ = $dir_define_a; NO_LABEL_EXPECTED(@label, "#define", $label); }
;
dir_define_a:
      D_DEFINE id                   { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DEFINE, $id); }
    | D_DEFINE id string            { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DEFINE, $id->appendLink($string)); }
    | D_DEFINE                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DEFINE, "#define"); }
;
dir_undefine:
      dir_undefine_a                { $$ = $dir_undefine_a; }
    | label dir_undefine_a          { /* Syntax error */ $$ = $dir_undefine_a; NO_LABEL_EXPECTED(@label, "#undefine", $label); }
;
dir_undefine_a:
      D_UNDEFINE id                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UNDEFINE, $id); }
    | D_UNDEFINE                    { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_UNDEFINE, "#undefine"); }
;
dir_dc_ifdef:
      dir_dc_ifdef_a                { $$ = $dir_dc_ifdef_a; }
    | label dir_dc_ifdef_a          { /* Syntax error */ $$ = $dir_dc_ifdef_a; NO_LABEL_EXPECTED(@label, "#ifdef", $label); }
;
dir_dc_ifdef_a: // Double-cross (#) ifdef - `#ifdef'.
      D_DC_IFDEF id                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DC_IFDEF, $id); }
    | D_DC_IFDEF                    { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DC_IFDEF, "#ifdef"); }
;
dir_dc_ifndef:
      dir_dc_ifndef_a               { $$ = $dir_dc_ifndef_a; }
    | label dir_dc_ifndef_a         { /* Syntax error */ $$ = $dir_dc_ifndef_a; NO_LABEL_EXPECTED(@label, "#ifndef", $label); }
;
dir_dc_ifndef_a:// Double-cross (#) ifndef - `#ifndef'.
      D_DC_IFNDEF id                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DC_IFNDEF, $id); }
    | D_DC_IFNDEF                   { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DC_IFNDEF, "#ifndef"); }
;
dir_dc_endif:
      dir_dc_endif_a                { $$ = $dir_dc_endif_a; }
    | label dir_dc_endif_a          { /* Syntax error */ $$ = $dir_dc_endif_a; NO_LABEL_EXPECTED(@label, "#endif", $label); }
;
dir_dc_endif_a: // Double-cross (#) endif - `#endif'.
      D_DC_ENDIF                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DC_ENDIF); }
    | D_DC_ENDIF args               { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("#endif", $args, @args); }
;
dir___idlocs:
      dir___idlocs_a                { $$ = $dir___idlocs_a; }
    | label dir___idlocs_a          { $$ = $label->appendLink($dir___idlocs_a); }
;
dir___idlocs_a:
      D___IDLOCS expr "," expr      { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR___IDLOCS, $2->appendLink($4)); }
    | D___IDLOCS                    { /* Syntax error */ $$ = NULL; ARGS_REQUIRED_D(@D___IDLOCS, "__IDLOCS", 2); }
    | D___IDLOCS expr               {
                        /* Syntax error */
                        $$ = NULL;
                        ARGS_REQUIRED_D(@D___IDLOCS, "__IDLOCS", 2);
                        $expr->completeDelete();
                    }
;
dir___maxram:
      D___MAXRAM expr               { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR___MAXRAM, $expr); }
    | label D___MAXRAM expr         { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR___MAXRAM, $expr)); }
    | D___MAXRAM                    { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D___MAXRAM, "__MAXRAM"); }
    | label D___MAXRAM              {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D___MAXRAM, "__MAXRAM");
                        $label->completeDelete();
                    }
;
dir___maxrom:
      D___MAXROM expr               { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR___MAXRAM, $expr); }
    | label D___MAXROM expr         { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR___MAXRAM, $expr)); }
    | D___MAXROM                    { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D___MAXROM, "__MAXROM"); }
    | label D___MAXROM              {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D___MAXROM, "__MAXROM");
                        $label->completeDelete();
                    }
;
dir_cblock:
      dir_cblock_a
      label_list_ml EOL
      D_ENDC                        { $$ = $dir_cblock_a->createBranch($label_list_ml); }
;
dir_cblock_a:
      dir_cblock_b                  { $$ = $dir_cblock_b; }
    | label dir_cblock_b            { $$ = $label->appendLink($dir_cblock_b); }
;
dir_cblock_b:
      D_CBLOCK EOL                  { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CBLOCK); }
    | D_CBLOCK COMMENT EOL          { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CBLOCK); }
    | D_CBLOCK expr EOL             { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CBLOCK, $expr); }
    | D_CBLOCK expr COMMENT EOL     { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CBLOCK, $expr); }
;
label_list_ml:
      label_list_ml EOL
      label_list                    { $$ = $1->appendLink($3); }
    | label_list                    { $$ = $1; }
;
label_list:
    /* empty */                     { $$ = NULL; }
    | label_list "," id             { $$ = $1->appendLink(new CompilerStatement(LOC(), ASMPIC8_LABEL, $id)); }
    | label_list "," label expr     { $$ = $1->appendLink($label->appendArgsLink($expr)); }
    | label_list "," id ":" expr    { $$ = $1->appendLink(new CompilerStatement(LOC(), ASMPIC8_LABEL, $id->appendLink($expr))); }
;
dir_da:
      dir_da_a                      { $$ = $dir_da_a; }
    | label dir_da_a                { $$ = $label->appendLink($dir_da_a); }
;
dir_da_a:
      D_DA args_str                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DA, $args_str); }
    | id D_DA args_str              { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DA, $id->appendLink($args_str)); }
    | D_DA                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DA, "DA"); }
    | id D_DA                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DA, "DA"); $id->completeDelete(); }
;
dir_data:
      dir_data_a                    { $$ = $dir_data_a; }
    | label dir_data_a              { $$ = $label->appendLink($dir_data_a); }
;
dir_data_a:
      D_DATA args_str               { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DATA, $args_str); }
    | id D_DATA args_str            { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DATA, $id->appendLink($args_str)); }
    | D_DATA                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DATA, "DATA"); }
    | id D_DATA                     { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DATA, "DATA"); $id->completeDelete(); }
;
dir_db:
      dir_db_a                      { $$ = $dir_db_a; }
    | label dir_db_a                { $$ = $label->appendLink($dir_db_a); }
;
dir_db_a:
      D_DB args_str                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DB, $args_str); }
    | id D_DB args_str              { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DB, $id->appendLink($args_str)); }
    | D_DB                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DB, "DB"); }
    | id D_DB                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DB, "DB"); $id->completeDelete(); }
;
dir_de:
      dir_de_a                      { $$ = $dir_de_a; }
    | label dir_de_a                { $$ = $label->appendLink($dir_de_a); }
;
dir_de_a:
      D_DE args_str                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DE, $args_str); }
    | id D_DE args_str              { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DE, $id->appendLink($args_str)); }
    | D_DE                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DE, "DE"); }
    | id D_DE                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DE, "DE"); $id->completeDelete(); }
;
dir_dt:
      dir_dt_a                      { $$ = $dir_dt_a; }
    | label dir_dt_a                { $$ = $label->appendLink($dir_dt_a); }
;
dir_dt_a:
      D_DT args_str                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DT, $args_str); }
    | id D_DT args_str              { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DT, $id->appendLink($args_str)); }
    | D_DT                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DT, "DT"); }
    | id D_DT                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DT, "DT"); $id->completeDelete(); }
;
dir_dw:
      dir_dw_a                      { $$ = $dir_dw_a; }
    | label dir_dw_a                { $$ = $label->appendLink($dir_dw_a); }
;
dir_dw_a:
      D_DW args_str                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DW, $args_str); }
    | id D_DW args_str              { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_DW, $id->appendLink($args_str)); }
    | D_DW                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DW, "DW"); }
    | id D_DW                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DW, "DW"); $id->completeDelete(); }
;
dir_fill:
      dir_fill_a                    { $$ = $dir_fill_a; }
    | label dir_fill_a              { $$ = $label->appendLink($dir_fill_a); }
;
dir_fill_a:
      D_FILL expr "," expr          { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_FILL, (new CompilerExpr())->appendLink($2)->appendLink($4)); }
    | id D_FILL expr "," expr       { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_FILL, $id->appendLink($3)->appendLink($5)); }
    | D_FILL                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_FILL, "FILL"); }
    | id D_FILL                     { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_FILL, "FILL"); $id->completeDelete(); }
    | D_FILL "(" instruction ")"
      "," expr                      {
                        $$ = new CompilerStatement (
                            LOC(),
                            ASMPIC8_DIR_FILL,
                            (new CompilerExpr())->appendLink(new CompilerExpr())->appendLink($expr) );
                        $$->createBranch($instruction);
                    }
    | id D_FILL "(" instruction ")"
      "," expr                      {
                        $$ = new CompilerStatement (
                            LOC(),
                            ASMPIC8_DIR_FILL,
                            $id->appendLink(new CompilerExpr())->appendLink($expr) );
                        $$->createBranch($instruction);
                    }
;
dir_res:
      dir_res_a                     { $$ = $dir_res_a; }
    | label dir_res_a               { $$ = $label->appendLink($dir_res_a); }
;
dir_res_a:
      D_RES expr                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_RES, $expr); }
    | id D_RES expr                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_RES, $id->appendLink($expr)); }
    | D_RES                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_RES, "RES"); }
    | id D_RES                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_RES, "RES"); $id->completeDelete(); }
;
dir_error:
      D_ERROR string                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ERROR, $string); }
    | label D_ERROR string          { $$ = $label->appendLink($$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ERROR, $string)); }
    | label D_ERROR                 { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_ERROR, "ERROR"); $label->completeDelete(); }
    | D_ERROR                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_ERROR, "ERROR"); }
;
dir_messg:
      D_MESSG string                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_MESSG, $string); }
    | label D_MESSG string          { $$ = $label->appendLink($$ = new CompilerStatement(LOC(), ASMPIC8_DIR_MESSG, $string)); }
    | label D_MESSG                 { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_MESSG, "MESSG"); $label->completeDelete(); }
    | D_MESSG                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_MESSG, "MESSG"); }
;
dir_subtitle:
      D_SUBTITLE string             { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_SUBTITLE, $string); }
    | label D_SUBTITLE string       { $$ = $label->appendLink($$ = new CompilerStatement(LOC(), ASMPIC8_DIR_SUBTITLE, $string)); }
    | label D_SUBTITLE              {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_SUBTITLE, "SUBTITLE");
                        $label->completeDelete();
                    }
    | D_SUBTITLE                    {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_SUBTITLE, "SUBTITLE");
                    }
;
dir_title:
      D_TITLE string                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_TITLE, $string); }
    | label D_TITLE string          { $$ = $label->appendLink($$ = new CompilerStatement(LOC(), ASMPIC8_DIR_TITLE, $string)); }
    | label D_TITLE                 { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_TITLE, "TITLE"); $label->completeDelete(); }
    | D_TITLE                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_TITLE, "TITLE"); }
;
dir_errorlevel:
      D_ERRORLEVEL err_lvls         { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ERRORLEVEL, $err_lvls); }
    | label D_ERRORLEVEL err_lvls   { $$ = $label->appendLink($$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ERRORLEVEL, $err_lvls)); }
    | label D_ERRORLEVEL            {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_ERRORLEVEL, "ERRORLEVEL");
                        $label->completeDelete();
                    }
    | D_ERRORLEVEL                  {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_ERRORLEVEL, "ERRORLEVEL");
                    }
;
err_lvls:
      err_lvls "," expr             { $$ = $1->appendLink($expr); }
    | err_lvls "," "+" expr         { $$ = $1->appendLink(new CompilerExpr('+', $expr)); }
    | err_lvls "," "-" expr         { $$ = $1->appendLink(new CompilerExpr('-', $expr)); }
    | expr                          { $$ = $expr; }
    | "+" expr                      { $$ = new CompilerExpr('+', $expr); }
    | "-" expr                      { $$ = new CompilerExpr('-', $expr); }
;
dir_page:
      dir_page_a                    { $$ = $dir_page_a; }
    | dir_page_a args               { /* Syntax error */ $$ = $dir_page_a; NO_ARG_EXPECTED_D("PAGE", $args, @args); }
;
dir_page_a:
      D_PAGE                        { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_PAGE); }
    | label D_PAGE                  {
                        /* Syntax error */
                        $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_PAGE);
                        NO_LABEL_EXPECTED(@label, "PAGE", $label);
                    }
;
dir_expand:
      dir_expand_a                  { $$ = $dir_expand_a; }
    | dir_expand_a args             { /* Syntax error */ $$ = $dir_expand_a; NO_ARG_EXPECTED_D("EXPAND", $args, @args); }
;
dir_expand_a:
      D_EXPAND                      { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_EXPAND); }
    | label D_EXPAND                {
                        /* Syntax error */
                        $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_EXPAND);
                        NO_LABEL_EXPECTED(@label, "EXPAND", $label);
                    }
;
dir_noexpand:
      dir_noexpand_a                { $$ = $dir_noexpand_a; }
    | dir_noexpand_a args           { /* Syntax error */ $$ = $dir_noexpand_a; NO_ARG_EXPECTED_D("NOEXPAND", $args, @args); }
;
dir_noexpand_a:
      D_NOEXPAND                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_NOEXPAND); }
    | label D_NOEXPAND              {
                        /* Syntax error */
                        $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_NOEXPAND);
                        NO_LABEL_EXPECTED(@label, "NOEXPAND", $label);
                    }
;
dir_list:
      D_LIST opts                   { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_LIST, $opts); }
    | D_LIST                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_LIST, "LIST"); }
    | label D_LIST opts             {
                        /* Syntax error */
                        $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_LIST, $opts);
                        NO_LABEL_EXPECTED(@label, "LIST", $label);
                    }
    | label D_LIST                  {
                        /* Syntax error */
                        $$ = NULL;
                        NO_LABEL_EXPECTED(@label, "LIST", $label);
                        ARG_REQUIRED_D(@D_LIST, "LIST");
                    }
;
opts:
      opts "," opt                  { $$ = $1->appendLink($opt); }
    | opt                           { $$ = $opt; }
;
opt:
      id "=" expr                   { $$ = new CompilerExpr($id, '=', $expr); }
    | id                            { $$ = $id; }
;
dir_access_ovr:
      dir_access_ovr_a              { $$ = $1; }
    | label dir_access_ovr_a        { /* Syntax error */ $$ = $2; NO_LABEL_EXPECTED(@label, "ACCESS_OVR", $label); }
;
dir_access_ovr_a:
      D_ACCESS_OVR                  { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ACCESS_OVR); }
    | D_ACCESS_OVR expr             {
                        $$ = new CompilerStatement (
                            LOC(),
                            ASMPIC8_DIR_ACCESS_OVR,
                            (new CompilerExpr())->appendLink($expr)
                        );
                    }
    | id D_ACCESS_OVR               { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ACCESS_OVR, $id->appendLink(new CompilerExpr())); }
    | id D_ACCESS_OVR expr          { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_ACCESS_OVR, $id->appendLink($expr)); }
;
dir_code:
      dir_code_a                    { $$ = $1; }
    | label dir_code_a              { /* Syntax error */ $$ = $2; NO_LABEL_EXPECTED(@label, "CODE", $label); }
;
dir_code_a:
      D_CODE                        { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CODE); }
    | D_CODE expr                   { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CODE, (new CompilerExpr())->appendLink($expr)); }
    | id D_CODE                     { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CODE, $id->appendLink(new CompilerExpr())); }
    | id D_CODE expr                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CODE, $id->appendLink($expr)); }
;
dir_code_pack:
      dir_code_pack_a               { $$ = $1; }
    | label dir_code_pack_a         { /* Syntax error */ $$ = $2; NO_LABEL_EXPECTED(@label, "CODE_PACK", $label); }
;
dir_code_pack_a:
      D_CODE_PACK                   { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CODE_PACK); }
    | D_CODE_PACK expr              { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CODE_PACK, (new CompilerExpr())->appendLink($expr)); }
    | id D_CODE_PACK                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CODE_PACK, $id->appendLink(new CompilerExpr())); }
    | id D_CODE_PACK expr           { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CODE_PACK, $id->appendLink($expr)); }
;
dir_extern:
      D_EXTERN params               { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_EXTERN, $params); }
    | D_EXTERN                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_EXTERN, "EXTERN"); }
    | label D_EXTERN params         {
                        /* Syntax error */
                        $$ = NULL;
                        NO_LABEL_EXPECTED(@label, "EXTERN", $label->appendArgsLink($params));
                    }
    | label D_EXTERN                {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_EXTERN, "EXTERN");
                        NO_LABEL_EXPECTED(@label, "EXTERN", $label);
                    }
;
dir_global:
      D_GLOBAL params               { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_GLOBAL, $params); }
    | D_GLOBAL                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_GLOBAL, "GLOBAL"); }
    | label D_GLOBAL params         {
                        /* Syntax error */
                        $$ = NULL;
                        NO_LABEL_EXPECTED(@label, "GLOBAL", $label->appendArgsLink($params));
                    }
    | label D_GLOBAL                {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_GLOBAL, "GLOBAL");
                        NO_LABEL_EXPECTED(@label, "GLOBAL", $label);
                    }
;
dir_bankisel:
      D_BANKISEL id                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_BANKISEL, $id); }
    | label D_BANKISEL id           {
                        /* Syntax error */
                        $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_BANKISEL, $id);
                        NO_LABEL_EXPECTED(@label, "BANKISEL", $label);
                    }
    | label D_BANKISEL              {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_BANKISEL, "BANKISEL");
                        NO_LABEL_EXPECTED(@label, "BANKISEL", $label);
                    }
    | D_BANKISEL                    {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_BANKISEL, "BANKISEL");
                    }
;
dir_banksel:
      D_BANKSEL id                  { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_BANKSEL, $id); }
    | label D_BANKSEL id            {
                        /* Syntax error */
                        $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_BANKSEL, $id);
                        NO_LABEL_EXPECTED(@label, "BANKSEL", $label);
                    }
    | label D_BANKSEL               {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_BANKSEL, "BANKSEL");
                        NO_LABEL_EXPECTED(@label, "BANKSEL", $label);
                    }
    | D_BANKSEL                     {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_BANKSEL, "BANKSEL");
                    }
;
dir_pagesel:
      D_PAGESEL id                  { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_PAGESEL, $id); }
    | label D_PAGESEL id            {
                        /* Syntax error */
                        $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_PAGESEL, $id);
                        NO_LABEL_EXPECTED(@label, "PAGESEL", $label);
                    }
    | label D_PAGESEL               {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_PAGESEL, "PAGESEL");
                        NO_LABEL_EXPECTED(@label, "PAGESEL", $label);
                    }
    | D_PAGESEL                     {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_PAGESEL, "PAGESEL");
                    }
;
dir_pageselw:
      D_PAGESELW id                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_PAGESELW, $id); }
    | label D_PAGESELW id           {
                        /* Syntax error */
                        $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_PAGESELW, $id);
                        NO_LABEL_EXPECTED(@label, "PAGESELW", $label);
                    }
    | label D_PAGESELW              {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_PAGESELW, "PAGESELW");
                        NO_LABEL_EXPECTED(@label, "PAGESELW", $label);
                    }
    | D_PAGESELW                    {
                        /* Syntax error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_PAGESELW, "PAGESELW");
                    }
;
dir_udata:
      dir_udata_a                   { $$ = $1; }
    | label dir_udata_a             { /* Syntax error */ $$ = $2; NO_LABEL_EXPECTED(@label, "UDATA", $label); }
;
dir_udata_a:
      D_UDATA                       { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA); }
    | D_UDATA expr                  { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA, (new CompilerExpr())->appendLink($expr)); }
    | id D_UDATA                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA, $id->appendLink(new CompilerExpr())); }
    | id D_UDATA expr               { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA, $id->appendLink($expr)); }
;
dir_udata_acs:
      dir_udata_acs_a               { $$ = $1; }
    | label dir_udata_acs_a         { /* Syntax error */ $$ = $2; NO_LABEL_EXPECTED(@label, "UDATA_ACS", $label); }
;
dir_udata_acs_a:
      D_UDATA_ACS                   { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA_ACS); }
    | D_UDATA_ACS expr              {
                        $$ = new CompilerStatement (
                            LOC(),
                            ASMPIC8_DIR_UDATA_ACS,
                            (new CompilerExpr())->appendLink($expr));
                    }
    | id D_UDATA_ACS                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA_ACS, $id->appendLink(new CompilerExpr())); }
    | id D_UDATA_ACS expr           { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA_ACS, $id->appendLink($expr)); }
;
dir_udata_ovr:
      dir_udata_ovr_a               { $$ = $1; }
    | label dir_udata_ovr_a         { /* Syntax error */ $$ = $2; NO_LABEL_EXPECTED(@label, "UDATA_OVR", $label); }
;
dir_udata_ovr_a:
      D_UDATA_OVR                   { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA_OVR); }
    | D_UDATA_OVR expr              {
                        $$ = new CompilerStatement (
                            LOC(),
                            ASMPIC8_DIR_UDATA_OVR,
                            (new CompilerExpr())->appendLink($expr));
                    }
    | id D_UDATA_OVR                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA_OVR, $id->appendLink(new CompilerExpr())); }
    | id D_UDATA_OVR expr           { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA_OVR, $id->appendLink($expr)); }
;
dir_udata_shr:
      dir_udata_shr_a               { $$ = $1; }
    | label dir_udata_shr_a         { /* Syntax error */ $$ = $2; NO_LABEL_EXPECTED(@label, "UDATA_SHR", $label); }
;
dir_udata_shr_a:
      D_UDATA_SHR                   { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA_SHR); }
    | D_UDATA_SHR expr              {
                        $$ = new CompilerStatement (
                            LOC(),
                            ASMPIC8_DIR_UDATA_SHR,
                            (new CompilerExpr())->appendLink($expr));
                    }
    | id D_UDATA_SHR                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA_SHR, $id->appendLink(new CompilerExpr())); }
    | id D_UDATA_SHR expr           { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_UDATA_SHR, $id->appendLink($expr)); }
;
dir_idata:
      dir_idata_a                   { $$ = $1; }
    | label dir_idata_a             { /* Syntax error */ $$ = $2; NO_LABEL_EXPECTED(@label, "IDATA", $label); }
;
dir_idata_a:
      D_IDATA                       { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IDATA); }
    | D_IDATA expr                  { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IDATA, (new CompilerExpr())->appendLink($expr)); }
    | id D_IDATA                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IDATA, $id->appendLink(new CompilerExpr())); }
    | id D_IDATA expr               { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IDATA, $id->appendLink($expr)); }
;
dir_idata_acs:
      dir_idata_acs_a               { $$ = $1; }
    | label dir_idata_acs_a         { /* Syntax error */ $$ = $2; NO_LABEL_EXPECTED(@label, "IDATA_ACS", $label); }
;
dir_idata_acs_a:
      D_IDATA_ACS                   { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IDATA_ACS); }
    | D_IDATA_ACS expr              {
                        $$ = new CompilerStatement (
                            LOC(),
                            ASMPIC8_DIR_IDATA_ACS,
                            (new CompilerExpr())->appendLink($expr));
                    }
    | id D_IDATA_ACS                { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IDATA_ACS, $id->appendLink(new CompilerExpr())); }
    | id D_IDATA_ACS expr           { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_IDATA_ACS, $id->appendLink($expr)); }
;

/*
 * Instructions
 */
instruction:
    /* BYTE-ORIENTED FILE REGISTER OPERATIONS */
      ins_addwf     { $$ = $1; }    | ins_andwf     { $$ = $1; }
    | ins_clrf      { $$ = $1; }    | ins_clrw      { $$ = $1; }
    | ins_comf      { $$ = $1; }    | ins_decf      { $$ = $1; }
    | ins_decfsz    { $$ = $1; }    | ins_incf      { $$ = $1; }
    | ins_incfsz    { $$ = $1; }    | ins_iorwf     { $$ = $1; }
    | ins_movf      { $$ = $1; }    | ins_movwf     { $$ = $1; }
    | ins_nop       { $$ = $1; }    | ins_rlf       { $$ = $1; }
    | ins_rrf       { $$ = $1; }    | ins_subwf     { $$ = $1; }
    | ins_swapf     { $$ = $1; }    | ins_xorwf     { $$ = $1; }
    /* BIT-ORIENTED FILE REGISTER OPERATIONS */
    | ins_bcf       { $$ = $1; }    | ins_bsf       { $$ = $1; }
    | ins_btfsc     { $$ = $1; }    | ins_btfss     { $$ = $1; }
    /* LITERAL AND CONTROL OPERATIONS */
    | ins_addlw     { $$ = $1; }    | ins_andlw     { $$ = $1; }
    | ins_call      { $$ = $1; }    | ins_clrwdt    { $$ = $1; }
    | ins_goto      { $$ = $1; }    | ins_iorlw     { $$ = $1; }
    | ins_movlw     { $$ = $1; }    | ins_retfie    { $$ = $1; }
    | ins_retlw     { $$ = $1; }    | ins_return    { $$ = $1; }
    | ins_sleep     { $$ = $1; }    | ins_sublw     { $$ = $1; }
    | ins_xorlw     { $$ = $1; }    | ins_option    { $$ = $1; }
    | ins_tris      { $$ = $1; }
;
/* BYTE-ORIENTED FILE REGISTER OPERATIONS */
ins_addwf:
      I_ADDWF expr "," expr { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_ADDWF, $2->appendLink($4)); }
    | I_ADDWF oprs_2        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ADDWF", 2); }
;
ins_andwf:
      I_ANDWF expr "," expr { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_ANDWF, $2->appendLink($4)); }
    | I_ANDWF oprs_2        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ANDWF", 2); }
;
ins_clrf:
      I_CLRF expr           { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_CLRF, $2); }
    | I_CLRF oprs_1         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CLRF", 1); }
;
ins_clrw:
      I_CLRW                { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_CLRW); }
    | I_CLRW oprs_0         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CLRW", 0); }
;
ins_comf:
      I_COMF expr "," expr  { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_COMF, $2->appendLink($4)); }
    | I_COMF oprs_2         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "COMF", 2); }
;
ins_decf:
      I_DECF expr "," expr  { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_DECF, $2->appendLink($4)); }
    | I_DECF oprs_2         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "DECF", 2); }
;
ins_decfsz:
      I_DECFSZ expr "," expr{ $$ = new CompilerStatement(LOC(), ASMPIC8_INS_DECFSZ, $2->appendLink($4)); }
    | I_DECFSZ oprs_2       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "DECFSZ", 2); }
;
ins_incf:
      I_INCF expr "," expr  { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_INCF, $2->appendLink($4)); }
    | I_INCF oprs_2         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "INCF", 2); }
;
ins_incfsz:
      I_INCFSZ expr "," expr{ $$ = new CompilerStatement(LOC(), ASMPIC8_INS_INCFSZ, $2->appendLink($4)); }
    | I_INCFSZ oprs_2       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "INCFSZ", 2); }
;
ins_iorwf:
      I_IORWF expr "," expr { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_IORWF, $2->appendLink($4)); }
    | I_IORWF oprs_2        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "IORWF", 2); }
;
ins_movf:
      I_MOVF expr "," expr  { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_MOVF, $2->appendLink($4)); }
    | I_MOVF oprs_2         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "MOVF", 2); }
;
ins_movwf:
      I_MOVWF expr          { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_MOVWF, $2); }
    | I_MOVWF oprs_1        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "MOVWF", 1); }
;
ins_nop:
      I_NOP                 { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_NOP); }
    | I_NOP oprs_0          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "NOP", 0); }
;
ins_rlf:
      I_RLF expr "," expr   { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_RLF, $2->appendLink($4)); }
    | I_RLF oprs_2          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "RLF", 2); }
;
ins_rrf:
      I_RRF expr "," expr   { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_RRF, $2->appendLink($4)); }
    | I_RRF oprs_2          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "RRF", 2); }
;
ins_subwf:
      I_SUBWF expr "," expr { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_SUBWF, $2->appendLink($4)); }
    | I_SUBWF oprs_2        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SUBWF", 2); }
;
ins_swapf:
      I_SWAPF expr "," expr { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_SWAPF, $2->appendLink($4)); }
    | I_SWAPF oprs_2        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SWAPF", 2); }
;
ins_xorwf:
      I_XORWF expr "," expr { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_XORWF, $2->appendLink($4)); }
    | I_XORWF oprs_2        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "XORWF", 2); }
;
/* BIT-ORIENTED FILE REGISTER OPERATIONS */
ins_bcf:
      I_BCF expr "," expr   { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_BCF, $2->appendLink($4)); }
    | I_BCF oprs_2          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BCF", 2); }
;
ins_bsf:
      I_BSF expr "," expr   { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_BSF, $2->appendLink($4)); }
    | I_BSF oprs_2          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BSF", 2); }
;
ins_btfsc:
      I_BTFSC expr "," expr { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_BTFSC, $2->appendLink($4)); }
    | I_BTFSC oprs_2        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BTFSC", 2); }
;
ins_btfss:
      I_BTFSS expr "," expr { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_BTFSS, $2->appendLink($4)); }
    | I_BTFSS oprs_2        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BTFSS", 2); }
;
    /* LITERAL AND CONTROL OPERATIONS */
ins_addlw:
      I_ADDLW expr          { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_ADDLW, $2); }
    | I_ADDLW oprs_1        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ADDLW", 1); }
;
ins_andlw:
      I_ANDLW expr          { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_ANDLW, $2); }
    | I_ANDLW oprs_1        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ANDLW", 1); }
;
ins_call:
      I_CALL expr           { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_CALL, $2); }
    | I_CALL oprs_1         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CALL", 1); }
;
ins_clrwdt:
      I_CLRWDT              { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_CLRWDT); }
    | I_CLRWDT oprs_0       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CLRWDT", 0); }
;
ins_goto:
      I_GOTO expr           { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_GOTO, $2); }
    | I_GOTO oprs_1         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "GOTO", 1); }
;
ins_iorlw:
      I_IORLW expr          { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_IORLW, $2); }
    | I_IORLW oprs_1        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "IORLW", 1); }
;
ins_movlw:
      I_MOVLW expr          { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_MOVLW, $2); }
    | I_MOVLW oprs_1        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "MOVLW", 1); }
;
ins_retfie:
      I_RETFIE              { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_RETFIE); }
    | I_RETFIE oprs_0       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "RETFIE", 0); }
;
ins_retlw:
      I_RETLW expr          { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_RETLW, $2); }
    | I_RETLW oprs_1        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "RETLW", 1); }
;
ins_return:
      I_RETURN              { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_RETURN); }
    | I_RETURN oprs_0       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "RETURN", 0); }
;
ins_sleep:
      I_SLEEP               { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_SLEEP); }
    | I_SLEEP oprs_0        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SLEEP", 0); }
;
ins_sublw:
      I_SUBLW expr          { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_SUBLW, $2); }
    | I_SUBLW oprs_1        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SUBLW", 1); }
;
ins_xorlw:
      I_XORLW expr          { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_XORLW, $2); }
    | I_XORLW oprs_1        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "XORLW", 1); }
;
ins_option:
      I_OPTION expr         { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_OPTION, $2); }
    | I_OPTION oprs_1       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "OPTION", 1); }
;
ins_tris:
      I_TRIS expr           { $$ = new CompilerStatement(LOC(), ASMPIC8_INS_TRIS, $2); }
    | I_TRIS oprs_1         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "TRIS", 1); }
;

/* Dummy operands for detailed error reporting. */
oprs_0: // Accept any number of operands except for zero.
      args                  { $1->completeDelete(); }
;
oprs_1: // Accept any number of operands except for one.
      /* empty */           { /* nothing has to be done here */ }
    | expr "," args         { $1->appendLink($3)->completeDelete(); }
;
oprs_2: // Accept any number of operands except for two.
      /* empty */           { /* nothing has to be done here */ }
    | expr                  { $1->completeDelete(); }
    | expr "," expr "," args{ $1->appendLink($3)->appendLink($5)->completeDelete(); }
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison
inline int pic8parser_error ( YYLTYPE * yylloc,
                              yyscan_t,
                              CompilerParserInterface * compiler,
                              const char * errorInfo )
{
    std::string errStr;
    if ( 0 == strncmp(errorInfo , "syntax error, unexpected ", 25) )
    {
        if ( (strlen(errorInfo) > 27) && ('_' == errorInfo[26]) )
        {
            switch ( errorInfo[25] )
            {
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
        }
        else
        {
            errStr = QObject::tr("unexpected ").toStdString();
            errStr += reinterpret_cast<const char *>( long(errorInfo) + 25 );
            errorInfo = errStr.c_str();
        }
    }
    else if ( 0 == strncmp(errorInfo , "syntax error", 12) )
    {
        errorInfo = QObject::tr("syntax not understood").toStdString().c_str();
    }

    compiler->parserMessage(compiler->toSourceLocation(yylloc), CompilerBase::MT_ERROR, errorInfo);
    return 0;
}
