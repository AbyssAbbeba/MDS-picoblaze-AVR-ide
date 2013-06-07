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
%expect 763
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
    #include "avr8lexer.h"

    // Name of the lexer function required by Bison
    #define avr8parser_lex avr8lexer_lex

    // Make a few things a little easier...
    #define LOC() \
        ( compiler->toSourceLocation(yylloc) )

    // Error messages
    #define N_OPERANDS_EXPECTED(location, instruction, number) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("invalid number of operands, instruction %1 takes %2 operand(s)").arg(instruction).arg(number).toStdString() );
    #define REQ_FIRST_OPR(instruction, operand, location, expression) \
        CompilerExpr::completeDelete(expression); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("the first operand of %1 is required to be one of these: { %2 }").arg(instruction).arg(operand).toStdString() );
    #define REQ_SECOND_OPR(instruction, operand, location, expression) \
        CompilerExpr::completeDelete(expression); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("the second operand of %1 is required to be one of these: { %2 }").arg(instruction).arg(operand).toStdString() );
    #define NO_LABEL_EXPECTED(location, directive, stmt) \
        CompilerStatement::completeDelete(stmt); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("cannot declare a label before %1 directive").arg(directive).toStdString() );
    #define ARG_REQUIRED_D(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 requires a single argument").arg(directive).toStdString() );
    #define NO_ARG_EXPECTED_D(directive, arguments, location) \
        arguments->completeDelete(); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 takes no arguments").arg(directive).toStdString() );
    #define MISSING_AT_OPERATOR(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("segment specifier %1 requires `AT' operator before the start address").arg(directive).toStdString() );
    #define DECL_ID_EXPECTED(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 requires an identifier for the symbol (or macro) which it defines").arg(directive).toStdString() );
    #define DEPRECATED_SYNTAX(location) \
        compiler->parserMessage ( location, CompilerBase::MT_WARNING, \
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
%token D_IF             D_IFN           D_IFDEF         D_IFNDEF        D_ELSEIFB
%token D_ELSEIFNB       D_ELSE          D_ELSEIF        D_ELSEIFN       D_ELSEIFDEF
%token D_ELSEIFNDEF     D_ENDIF         D_LOCAL         D_IFNB          D_IFB
%token D_SKIP           D_ENDM          D_EXITM         D_REPT          D_SET
%token D_MACRO          D_BYTE          D_CSEG          D_DB            D_DEF
%token D_DEVICE         D_DSEG          D_DW            D_EQU           D_ESEG
%token D_EXIT           D_LIST          D_LISTMAC       D_NOLIST        D_ORG

/* Instructions */
%token I_ADD    I_ADC   I_ADIW  I_SUB   I_SUBI  I_SBC   I_SBCI  I_SBIW  I_AND   I_ANDI
%token I_OR     I_ORI   I_EOR   I_COM   I_NEG   I_SBR   I_CBR   I_INC   I_DEC   I_TST
%token I_CLR    I_SER   I_MUL   I_RJMP  I_IJMP  I_JMP   I_RCALL I_ICALL I_CALL  I_RET
%token I_RETI   I_CPSE  I_CP    I_CPC   I_CPI   I_SBRC  I_SBRS  I_SBIC  I_SBIS  I_BRBS
%token I_BRBC   I_BREQ  I_BRNE  I_BRCS  I_BRCC  I_BRSH  I_BRLO  I_BRMI  I_BRPL  I_BRGE
%token I_BRLT   I_BRHS  I_BRHC  I_BRTS  I_BRTC  I_BRVS  I_BRVC  I_BRIE  I_BRID  I_MOV
%token I_LDI    I_LDS   I_LD    I_STS   I_ST    I_STD   I_LPM   I_IN    I_OUT   I_PUSH
%token I_POP    I_LSL   I_LSR   I_ROL   I_ROR   I_ASR   I_SWAP  I_BSET  I_BCLR  I_SBI
%token I_CBI    I_BST   I_BLD   I_SEC   I_CLC   I_SEN   I_CLN   I_SEZ   I_CLZ   I_SEI
%token I_CLI    I_SES   I_CLS   I_SEV   I_CLV   I_SET   I_CLT   I_SEH   I_CLH   I_NOP
%token I_SLEEP  I_WDR   I_LDD

/* Fixed operands, i.e. those which have no value */
%token X        Y       Z

/* Other terminal symbols */
%token COMMENT  AT_SIGN AT
%token EOL              "end of line"
%token END      0       "end of file"
// Named tokens (to make the grammar rules more readable)
%token LP               "("
%token RP               ")"
%token COMMA            ","
// Expression operators
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
%token<string>  IDENTIFIER      LABEL
%token<array>   STRING          INCLUDE
  // semantic value is a number
%token<number>  NUMBER

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Expressions
%type<expr>     expr            "expression"
%type<expr>     number          params          args            id              string
%type<expr>     args_str
// Statements - general
%type<stmt>     statements      stmt            inst_stmt       dir_stmt        macro_stmt
%type<stmt>     instruction     directive       macro           label
// Statements - directives
%type<stmt>     dir_macro_d     dir_macro       dir_exitm       dir_db          dir_include
%type<stmt>     dir_cond_asm    if_block        ifelse_block    else_block      dir_if
%type<stmt>     dir_ifn         dir_ifdef       dir_ifndef      dir_ifb         dir_ifnb
%type<stmt>     dir_elseif      dir_elseifn     dir_elseifdef   dir_elseifndf   dir_elseifb
%type<stmt>     dir_elseifnb    dir_rept        dir_dw          dir_cseg        dir_cseg_a
%type<stmt>     dir_skip        dir_local       dir_exit        dir_list        dir_listmac
%type<stmt>     dir_nolist      dir_repeat      dir_org         dir_eseg        dir_eseg_a
%type<stmt>     dir_dseg        dir_byte        dir_set         dir_byte_a      dir_equ
%type<stmt>     dir_if_a        dir_ifn_a       dir_ifdef_a     dir_macro_a     dir_dseg_a
%type<stmt>     dir_ifndef_a    dir_ifb_a       dir_ifnb_a      dir_else        dir_elseif_a
%type<stmt>     dir_elseifn_a   dir_elseifdef_a dir_elseifndf_a dir_elseifb_a   dir_elseifnb_a
%type<stmt>     dir_endm        dir_device      dir_def
// Statements - instructions
%type<stmt>     ins_add         ins_adc         ins_adiw        ins_sub         ins_subi
%type<stmt>     ins_sbc         ins_sbci        ins_sbiw        ins_and         ins_andi
%type<stmt>     ins_or          ins_ori         ins_eor         ins_com         ins_neg
%type<stmt>     ins_sbr         ins_cbr         ins_inc         ins_dec         ins_tst
%type<stmt>     ins_clr         ins_ser         ins_mul         ins_rjmp        ins_ijmp
%type<stmt>     ins_jmp         ins_rcall       ins_icall       ins_call        ins_ret
%type<stmt>     ins_reti        ins_cpse        ins_cp          ins_cpc         ins_cpi
%type<stmt>     ins_sbrc        ins_sbrs        ins_sbic        ins_sbis        ins_brbs
%type<stmt>     ins_brbc        ins_breq        ins_brne        ins_brcs        ins_brcc
%type<stmt>     ins_brsh        ins_brlo        ins_brmi        ins_brpl        ins_brge
%type<stmt>     ins_brlt        ins_brhs        ins_brhc        ins_brts        ins_brtc
%type<stmt>     ins_brvs        ins_brvc        ins_brie        ins_brid        ins_mov
%type<stmt>     ins_ld          ins_ldd         ins_sts         ins_st          ins_std
%type<stmt>     ins_lpm         ins_in          ins_out         ins_push        ins_pop
%type<stmt>     ins_lsl         ins_lsr         ins_rol         ins_ror         ins_asr
%type<stmt>     ins_swap        ins_bset        ins_bclr        ins_sbi         ins_cbi
%type<stmt>     ins_bst         ins_bld         ins_sec         ins_clc         ins_sen
%type<stmt>     ins_cln         ins_sez         ins_clz         ins_sei         ins_cli
%type<stmt>     ins_ses         ins_cls         ins_sev         ins_clv         ins_set
%type<stmt>     ins_clt         ins_seh         ins_clh         ins_nop         ins_sleep
%type<stmt>     ins_ldi         ins_lds         ins_wdr

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
      id                            { $$ = new CompilerStatement(LOC(), ASMAVR8_MACRO, $id); }
    | id "(" ")"                    { $$ = new CompilerStatement(LOC(), ASMAVR8_MACRO, $id); }
    | id args                       { $$ = new CompilerStatement(LOC(), ASMAVR8_MACRO, $id->appendLink($args)); }
    | id "(" args ")"               { $$ = new CompilerStatement(LOC(), ASMAVR8_MACRO, $id->appendLink($args)); }
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
      LABEL                         { $$ = new CompilerStatement(LOC(), ASMAVR8_LABEL, new CompilerExpr($1)); }
;
expr:
      id                            { $$ = $id; }
    | number                        { $$ = $number; }
    | "(" expr ")"                  { $$ = $2; }
    | AT_SIGN expr                  { $$ = new CompilerExpr(CompilerExpr::OPER_AT, $2); }
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
    | expr "=" expr                 { $$ = new CompilerExpr($1, '=', $3); }
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
      dir_byte      { $$ = $1; }    | dir_cond_asm  { $$ = $1; }
    | dir_dw        { $$ = $1; }    | dir_skip      { $$ = $1; }
    | dir_local     { $$ = $1; }    | dir_exitm     { $$ = $1; }
    | dir_list      { $$ = $1; }    | dir_include   { $$ = $1; }
    | dir_nolist    { $$ = $1; }    | dir_repeat    { $$ = $1; }
    | dir_org       { $$ = $1; }    | dir_listmac   { $$ = $1; }
    | dir_eseg      { $$ = $1; }    | dir_dseg      { $$ = $1; }
    | dir_set       { $$ = $1; }    | dir_equ       { $$ = $1; }
    | dir_macro     { $$ = $1; }    | dir_db        { $$ = $1; }
    | dir_exit      { $$ = $1; }    | dir_cseg      { $$ = $1; }
    | dir_device    { $$ = $1; }    | dir_def       { $$ = $1; }
;
dir_cond_asm:
      if_block ifelse_block else_block D_ENDIF {
        $$ = new CompilerStatement(LOC(), ASMAVR8_COND_ASM);
        $$->createBranch ( $if_block->appendLink($ifelse_block)->appendLink($else_block) );
    }
    | label if_block ifelse_block else_block D_ENDIF {
        $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_COND_ASM));
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
      D_ELSE                        { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSE); }
    | label D_ELSE                  {
                        /* Syntax error */
                        NO_LABEL_EXPECTED(@label, "ELSE", $label);
                        $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSE);
                    }
    | label D_ELSE args             {
                        /* Syntax error */
                        NO_ARG_EXPECTED_D("ELSE", $args, @args);
                        NO_LABEL_EXPECTED(@label, "ELSE", $label);
                        $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSE);
                    }
    | D_ELSE args                   {
                        /* Syntax error */
                        NO_ARG_EXPECTED_D("ELSE", $args, @args);
                        $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSE);
                    }
;
dir_if:
      dir_if_a EOL                  { $$ = $dir_if_a; }
    | dir_if_a COMMENT EOL          { $$ = $dir_if_a; }
;
dir_if_a:
      D_IF expr                     { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_IF, $expr); }
    | D_IF                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IF, "IF"); }
;
dir_ifn:
      dir_ifn_a EOL                 { $$ = $dir_ifn_a; }
    | dir_ifn_a COMMENT EOL         { $$ = $dir_ifn_a; }
;
dir_ifn_a:
      D_IFN expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_IFN, $expr); }
    | D_IFN                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFN, "IFN"); }
;
dir_ifdef:
      dir_ifdef_a EOL               { $$ = $dir_ifdef_a; }
    | dir_ifdef_a COMMENT EOL       { $$ = $dir_ifdef_a; }
;
dir_ifdef_a:
      D_IFDEF id                    { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_IFDEF, $id); }
    | D_IFDEF                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFDEF, "IFDEF"); }
;
dir_ifndef:
      dir_ifndef_a EOL              { $$ = $dir_ifndef_a; }
    | dir_ifndef_a COMMENT EOL      { $$ = $dir_ifndef_a; }
;
dir_ifndef_a:
      D_IFNDEF id                   { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_IFNDEF, $id); }
    | D_IFNDEF                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFNDEF, "IFNDEF"); }
;
dir_ifb:
      dir_ifb_a EOL                 { $$ = $dir_ifb_a; }
    | dir_ifb_a COMMENT EOL         { $$ = $dir_ifb_a; }
;
dir_ifb_a:
      D_IFB id                      { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_IFB, $id); }
    | D_IFB                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFB, "IFB"); }
;
dir_ifnb:
      dir_ifnb_a EOL                { $$ = $dir_ifnb_a; }
    | dir_ifnb_a COMMENT EOL        { $$ = $dir_ifnb_a; }
;
dir_ifnb_a:
      D_IFNB id                     { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_IFNB, $id); }
    | D_IFNB                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFNB, "IFNB"); }
;
dir_elseif:
      dir_elseif_a EOL              { $$ = $dir_elseif_a; }
    | dir_elseif_a COMMENT EOL      { $$ = $dir_elseif_a; }
;
dir_elseif_a:
      D_ELSEIF expr         { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSEIF, $expr); }
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
      D_ELSEIFN expr                { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSEIFN, $expr); }
    | D_ELSEIFN                     { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFN, "ELSEIFN"); }
    | label D_ELSEIFN expr  { /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFN", $label); $expr->completeDelete(); }
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
      D_ELSEIFDEF expr              { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSEIFDEF, $expr); }
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
      D_ELSEIFNDEF expr             { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSEIFNDEF, $expr); }
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
      D_ELSEIFB expr                { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSEIFB, $expr); }
    | D_ELSEIFB                     { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFB, "ELSEIFB"); }
    | label D_ELSEIFB expr  { /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFB", $label); $expr->completeDelete(); }
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
      D_ELSEIFNB expr               { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ELSEIFNB, $expr); }
    | D_ELSEIFNB                    { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFNB, "ELSEIFNB"); }
    | label D_ELSEIFNB expr { /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFNB", $label); $expr->completeDelete(); }
    | label D_ELSEIFNB              {
                        /* Syntax Error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_ELSEIFNB, "ELSEIFNB");
                        NO_LABEL_EXPECTED(@label, "ELSEIFNB", $label);
                    }
;
dir_org:
      D_ORG expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ORG, $expr); }
    | label D_ORG expr              { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_ORG, $expr)); }
    | D_ORG                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_ORG, "ORG"); }
    | label D_ORG                   { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_ORG, "ORG"); $label->completeDelete(); }
;
dir_cseg:
      dir_cseg_a                    { $$ = $dir_cseg_a; }
    | label dir_cseg_a              { $$ = $label->appendLink($dir_cseg_a); }
;
dir_cseg_a:
      D_CSEG                        { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_CSEG); }
    | D_CSEG AT expr                { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_CSEG, $expr); }
    | D_CSEG expr                   {
                        /* Syntax error */ MISSING_AT_OPERATOR(@D_CSEG, "CSEG");
                        $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_CSEG, $expr);
                    }
;
dir_dseg:
      dir_dseg_a                    { $$ = $dir_dseg_a; }
    | label dir_dseg_a              { $$ = $label->appendLink($dir_dseg_a); }
;
dir_dseg_a:
      D_DSEG                        { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DSEG); }
    | D_DSEG AT expr                { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DSEG, $expr); }
    | D_DSEG expr                   {
                        /* Syntax error */ MISSING_AT_OPERATOR(@D_DSEG, "DSEG");
                        $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DSEG, $expr);
                    }
;
dir_eseg:
      dir_eseg_a                    { $$ = $dir_eseg_a; }
    | label dir_eseg_a              { $$ = $label->appendLink($dir_eseg_a); }
;
dir_eseg_a:
      D_ESEG                        { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ESEG); }
    | D_ESEG AT expr                { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ESEG, $expr); }
    | D_ESEG expr                   {
                        /* Syntax error */ MISSING_AT_OPERATOR(@D_ESEG, "ESEG");
                        $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_ESEG, $expr);
                    }
;
dir_dw:
      D_DW expr                     { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DW, $expr); }
    | label D_DW expr               { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_DW, $expr)); }
    | D_DW                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DW, "D_DW"); }
    | label D_DW                    { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DW, "D_DW"); $label->completeDelete(); }
;
dir_skip:
      D_SKIP expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_SKIP, $expr); }
    | label D_SKIP expr             { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_SKIP, $expr)); }
    | D_SKIP                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_SKIP, "SKIP"); }
    | label D_SKIP                  { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_SKIP, "SKIP"); $label->completeDelete(); }
;
dir_local:
      D_LOCAL id                    { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_LOCAL, $id); }
    | label D_LOCAL id              { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_LOCAL, $id)); }
    | D_LOCAL                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_LOCAL, "LOCAL"); }
    | label D_LOCAL                 { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_LOCAL, "LOCAL"); $label->completeDelete(); }
;
dir_exitm:
      D_EXITM                       { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_EXITM); }
    | label D_EXITM                 { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_EXITM)); }
    | D_EXITM args                  { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("EXITM", $args, @args); }
    | label D_EXITM args            { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("EXITM", $args, @args); $label->completeDelete(); }
;
dir_list:
      D_LIST                        { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_LIST); }
    | label D_LIST                  { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_LIST)); }
    | D_LIST args                   { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("LIST", $args, @args); }
    | label D_LIST args             { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("LIST", $args, @args); $label->completeDelete(); }
;
dir_nolist:
      D_NOLIST                      { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_NOLIST); }
    | label D_NOLIST                { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_NOLIST)); }
    | D_NOLIST args                 { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("NOLIST", $args, @args); }
    | label D_NOLIST args           { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("NOLIST", $args, @args); $label->completeDelete(); }
;
dir_set:
      id D_SET expr                 { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_SET, $id->appendLink($expr)); }
    | D_SET id "=" expr             { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_SET, $id->appendLink($expr)); DEPRECATED_SYNTAX(@D_SET); }
    | label D_SET id "=" expr       { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_SET, $id->appendLink($expr))); DEPRECATED_SYNTAX(@D_SET); }
    | id D_SET                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_SET, "SET"); $id->completeDelete(); }
    | D_SET                         { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_SET, "SET"); ARG_REQUIRED_D(@D_SET, "SET"); }
    | D_SET expr                    { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_SET, "SET"); $expr->completeDelete(); }
    | label D_SET expr              { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_SET, "SET"); NO_LABEL_EXPECTED(@label, "SET", $label->appendArgsLink($expr)) }
    | label id D_SET expr           { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "SET", $label->appendArgsLink($id->appendLink($expr))); }
;
dir_equ:
      id D_EQU expr                 { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_EQU, $id->appendLink($expr)); }
    | D_EQU id "=" expr             { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_EQU, $id->appendLink($expr)); DEPRECATED_SYNTAX(@D_EQU); }
    | label D_EQU id "=" expr       { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_EQU, $id->appendLink($expr))); DEPRECATED_SYNTAX(@D_EQU); }
    | id D_EQU                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_EQU, "EQU"); $id->completeDelete(); }
    | D_EQU                         { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_EQU, "EQU"); ARG_REQUIRED_D(@D_EQU, "EQU"); }
    | D_EQU expr                    { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_EQU, "EQU"); $expr->completeDelete(); }
    | label D_EQU expr              { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_EQU, "EQU"); NO_LABEL_EXPECTED(@label, "EQU", $label->appendArgsLink($expr)) }
    | label id D_EQU expr           { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "EQU", $label->appendArgsLink($id->appendLink($expr))); }
;
dir_def:
      id D_DEF expr                 { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DEF, $id->appendLink($expr)); }
    | D_DEF id "=" expr             { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DEF, $id->appendLink($expr)); DEPRECATED_SYNTAX(@D_DEF); }
    | label D_DEF id "=" expr       { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_DEF, $id->appendLink($expr))); DEPRECATED_SYNTAX(@D_DEF); }
    | id D_DEF                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DEF, "DEF"); $id->completeDelete(); }
    | D_DEF                         { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_DEF, "DEF"); ARG_REQUIRED_D(@D_DEF, "DEF"); }
    | D_DEF expr                    { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_DEF, "DEF"); $expr->completeDelete(); }
    | label D_DEF expr              { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_DEF, "DEF"); NO_LABEL_EXPECTED(@label, "DEF", $label->appendArgsLink($expr)) }
    | label id D_DEF expr           { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "DEF", $label->appendArgsLink($id->appendLink($expr))); }
;
dir_device:
      D_DEVICE                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DEVICE, "DEVICE"); }
    | label D_DEVICE                { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DEVICE, "DEVICE"); $label->completeDelete(); }
    | D_DEVICE id                   { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DEVICE, $id); }
    | label D_DEVICE id             { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_DEVICE, $id)); }
;
dir_repeat:
      dir_rept
      statements EOL
      dir_endm                      { $$ = $dir_rept->createBranch($statements->appendLink($dir_endm)); }
    | dir_rept
      dir_endm                      { $$ = $dir_rept->createBranch($dir_endm); }
;
dir_rept:
      D_REPT expr COMMENT           { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_REPT, $expr); }
    | D_REPT expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_REPT, $expr); }
    | D_REPT COMMENT                { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_REPT); }
    | label D_REPT expr COMMENT     { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_REPT, $expr)); }
    | label D_REPT expr             { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_REPT, $expr)); }
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
      id D_MACRO                    { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_MACRO, $id); }
    | D_MACRO id                    { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_MACRO, $id); DEPRECATED_SYNTAX(@D_MACRO); }
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
dir_db:
      D_DB args_str                 { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_DB, $args_str); }
    | label D_DB args_str           { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_DB, $args_str)); }
    | D_DB                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DB, "DB"); }
    | label D_DB                    { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DB, "DB"); $label->completeDelete(); }
;
dir_byte:
      dir_byte_a                    { $$ = $dir_byte_a; }
    | label dir_byte_a              { $$ = $label->appendLink($dir_byte_a); }
;
dir_byte_a:
      D_BYTE expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_BYTE, $expr); }
    | D_BYTE                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_BYTE, "BYTE"); }
;
dir_include:
      INCLUDE                       {
                        $$ = new CompilerStatement (
                            LOC(),
                            ASMAVR8_INCLUDE,
                            new CompilerExpr (
                                CompilerValue($INCLUDE.data, $INCLUDE.size)
                            )
                        );
                    }
    | label INCLUDE                 {
                        $$ = $label->appendLink (
                            new CompilerStatement (
                                LOC(),
                                ASMAVR8_INCLUDE,
                                new CompilerExpr (
                                    CompilerValue ( $INCLUDE.data, $INCLUDE.size )
                                )
                            )
                        );
                    }
;
dir_exit:
      D_EXIT                        { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_EXIT); }
    | label D_EXIT                  { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_EXIT)); }
    | D_EXIT args                   {
                        /* Syntax error */
                        NO_ARG_EXPECTED_D("EXIT", $args, @args);
                        $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_EXIT);
                    }
    | label D_EXIT args             {
                        /* Syntax error */
                        NO_ARG_EXPECTED_D("EXIT", $args, @args);
                        $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_EXIT));
                    }
;
dir_listmac:
      D_LISTMAC                     { $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_LISTMAC); }
    | label D_LISTMAC               { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_LISTMAC)); }
    | D_LISTMAC args                {
                        /* Syntax error */
                        NO_ARG_EXPECTED_D("LISTMAC", $args, @args);
                        $$ = new CompilerStatement(LOC(), ASMAVR8_DIR_EXIT);
                    }
    | label D_LISTMAC  args         {
                        /* Syntax error */
                        NO_ARG_EXPECTED_D("LISTMAC", $args, @args);
                        $$ = $label->appendLink(new CompilerStatement(LOC(), ASMAVR8_DIR_EXIT));
                    }
;

/*
 * Instructions
 */
instruction:
    /* ARITHMETIC AND LOGIC INSTRUCTIONS */
      ins_add       { $$ = $1; }    | ins_adc       { $$ = $1; }
    | ins_adiw      { $$ = $1; }    | ins_sub       { $$ = $1; }
    | ins_subi      { $$ = $1; }    | ins_sbc       { $$ = $1; }
    | ins_sbci      { $$ = $1; }    | ins_sbiw      { $$ = $1; }
    | ins_and       { $$ = $1; }    | ins_andi      { $$ = $1; }
    | ins_or        { $$ = $1; }    | ins_ori       { $$ = $1; }
    | ins_eor       { $$ = $1; }    | ins_com       { $$ = $1; }
    | ins_neg       { $$ = $1; }    | ins_sbr       { $$ = $1; }
    | ins_cbr       { $$ = $1; }    | ins_inc       { $$ = $1; }
    | ins_dec       { $$ = $1; }    | ins_tst       { $$ = $1; }
    | ins_clr       { $$ = $1; }    | ins_ser       { $$ = $1; }
    | ins_mul       { $$ = $1; }
    /* BRANCH INSTRUCTIONS */
    | ins_rjmp      { $$ = $1; }    | ins_ijmp      { $$ = $1; }
    | ins_jmp       { $$ = $1; }    | ins_rcall     { $$ = $1; }
    | ins_icall     { $$ = $1; }    | ins_call      { $$ = $1; }
    | ins_ret       { $$ = $1; }    | ins_reti      { $$ = $1; }
    | ins_cpse      { $$ = $1; }    | ins_cp        { $$ = $1; }
    | ins_cpc       { $$ = $1; }    | ins_cpi       { $$ = $1; }
    | ins_sbrc      { $$ = $1; }    | ins_sbrs      { $$ = $1; }
    | ins_sbic      { $$ = $1; }    | ins_sbis      { $$ = $1; }
    | ins_brbs      { $$ = $1; }    | ins_brbc      { $$ = $1; }
    | ins_breq      { $$ = $1; }    | ins_brne      { $$ = $1; }
    | ins_brcs      { $$ = $1; }    | ins_brcc      { $$ = $1; }
    | ins_brsh      { $$ = $1; }    | ins_brlo      { $$ = $1; }
    | ins_brmi      { $$ = $1; }    | ins_brpl      { $$ = $1; }
    | ins_brge      { $$ = $1; }    | ins_brlt      { $$ = $1; }
    | ins_brhs      { $$ = $1; }    | ins_brhc      { $$ = $1; }
    | ins_brts      { $$ = $1; }    | ins_brtc      { $$ = $1; }
    | ins_brvs      { $$ = $1; }    | ins_brvc      { $$ = $1; }
    | ins_brie      { $$ = $1; }    | ins_brid      { $$ = $1; }
    /* DATA TRANSFER INSTRUCTIONS */
    | ins_mov       { $$ = $1; }    | ins_ldi       { $$ = $1; }
    | ins_lds       { $$ = $1; }    | ins_ld        { $$ = $1; }
    | ins_ldd       { $$ = $1; }    | ins_sts       { $$ = $1; }
    | ins_st        { $$ = $1; }    | ins_std       { $$ = $1; }
    | ins_lpm       { $$ = $1; }    | ins_in        { $$ = $1; }
    | ins_out       { $$ = $1; }    | ins_push      { $$ = $1; }
    | ins_pop       { $$ = $1; }
    /* BIT AND BIT-TEST INSTRUCTIONS */
    | ins_lsl       { $$ = $1; }    | ins_lsr       { $$ = $1; }
    | ins_rol       { $$ = $1; }    | ins_ror       { $$ = $1; }
    | ins_asr       { $$ = $1; }    | ins_swap      { $$ = $1; }
    | ins_bset      { $$ = $1; }    | ins_bclr      { $$ = $1; }
    | ins_sbi       { $$ = $1; }    | ins_cbi       { $$ = $1; }
    | ins_bst       { $$ = $1; }    | ins_bld       { $$ = $1; }
    | ins_sec       { $$ = $1; }    | ins_clc       { $$ = $1; }
    | ins_sen       { $$ = $1; }    | ins_cln       { $$ = $1; }
    | ins_sez       { $$ = $1; }    | ins_clz       { $$ = $1; }
    | ins_sei       { $$ = $1; }    | ins_cli       { $$ = $1; }
    | ins_ses       { $$ = $1; }    | ins_cls       { $$ = $1; }
    | ins_sev       { $$ = $1; }    | ins_clv       { $$ = $1; }
    | ins_set       { $$ = $1; }    | ins_clt       { $$ = $1; }
    | ins_seh       { $$ = $1; }    | ins_clh       { $$ = $1; }
    | ins_nop       { $$ = $1; }    | ins_sleep     { $$ = $1; }
    | ins_wdr       { $$ = $1; }
;
/* ARITHMETIC AND LOGIC INSTRUCTIONS */
ins_add:
      I_ADD expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ADD, $2->appendLink($4)); }
    | I_ADD oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ADD", 2); }
;
ins_adc:
      I_ADC expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ADC, $2->appendLink($4)); }
    | I_ADC oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ADC", 2); }
;
ins_adiw:
      I_ADIW expr "," expr          { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ADIW, $2->appendLink($4)); }
    | I_ADIW oprs_2                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ADIW", 2); }
;
ins_sub:
      I_SUB expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SUB, $2->appendLink($4)); }
    | I_SUB oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SUB", 2); }
;
ins_subi:
      I_SUBI expr "," expr          { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SUBI, $2->appendLink($4)); }
    | I_SUBI oprs_2                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SUBI", 2); }
;
ins_sbc:
      I_SBC expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBC, $2->appendLink($4)); }
    | I_SBC oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SBC", 2); }
;
ins_sbci:
      I_SBCI expr "," expr          { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBCI, $2->appendLink($4)); }
    | I_SBCI oprs_2                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SBCI", 2); }
;
ins_sbiw:
      I_SBIW expr "," expr          { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBIW, $2->appendLink($4)); }
    | I_SBIW oprs_2                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SBIW", 2); }
;
ins_and:
      I_AND expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_AND, $2->appendLink($4)); }
    | I_AND oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "AND", 2); }
;
ins_andi:
      I_ANDI expr "," expr          { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ANDI, $2->appendLink($4)); }
    | I_ANDI oprs_2                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ANDI", 2); }
;
ins_or:
      I_OR expr "," expr            { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_OR, $2->appendLink($4)); }
    | I_OR oprs_2                   { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "OR", 2); }
;
ins_ori:
      I_ORI expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ORI, $2->appendLink($4)); }
    | I_ORI oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ORI", 2); }
;
ins_eor:
      I_EOR expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_EOR, $2->appendLink($4)); }
    | I_EOR oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "EOR", 2); }
;
ins_com:
      I_COM expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_COM, $2); }
    | I_COM oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "COM", 1); }
;
ins_neg:
      I_NEG expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_NEG, $2); }
    | I_NEG oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "NEG", 1); }
;
ins_sbr:
      I_SBR expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBR, $2->appendLink($4)); }
    | I_SBR oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SBR", 2); }
;
ins_cbr:
      I_CBR expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CBR, $2->appendLink($4)); }
    | I_CBR oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CBR", 2); }
;
ins_inc:
      I_INC expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_INC, $2); }
    | I_INC oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "INC", 1); }
;
ins_dec:
      I_DEC expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_DEC, $2); }
    | I_DEC oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "DEC", 1); }
;
ins_tst:
      I_TST expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_TST, $2); }
    | I_TST oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "TST", 1); }
;
ins_clr:
      I_CLR expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLR, $2); }
    | I_CLR oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CLR", 1); }
;
ins_ser:
      I_SER expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SER, $2); }
    | I_SER oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SER", 1); }
;
ins_mul:
      I_MUL expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_MUL, $2->appendLink($4)); }
    | I_MUL oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "MUL", 2); }
;
/* BRANCH INSTRUCTIONS */
ins_rjmp:
      I_RJMP expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_RJMP, $2); }
    | I_RJMP oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "RJMP", 1); }
;
ins_ijmp:
      I_IJMP                        { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_IJMP); }
    | I_IJMP oprs_0                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "IJMP", 0); }
;
ins_jmp:
      I_JMP expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_JMP, $2); }
    | I_JMP oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "JMP", 1); }
;
ins_rcall:
      I_RCALL expr                  { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_RCALL, $2); }
    | I_RCALL oprs_1                { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "RCALL", 1); }
;
ins_icall:
      I_ICALL                       { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ICALL); }
    | I_ICALL oprs_0                { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ICALL", 0); }
;
ins_call:
      I_CALL expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CALL, $2); }
    | I_CALL oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CALL", 1); }
;
ins_ret:
      I_RET                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_RET); }
    | I_RET oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "RET", 0); }
;
ins_reti:
      I_RETI                        { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_RETI); }
    | I_RETI oprs_0                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "RETI", 0); }
;
ins_cpse:
      I_CPSE expr "," expr          { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CPSE, $2->appendLink($4)); }
    | I_CPSE oprs_2                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CPSE", 2); }
;
ins_cp:
      I_CP expr "," expr            { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CP, $2->appendLink($4)); }
    | I_CP oprs_2                   { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CP", 2); }
;
ins_cpc:
      I_CPC expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CPC, $2->appendLink($4)); }
    | I_CPC oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CPC", 2); }
;
ins_cpi:
      I_CPI expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CPI, $2->appendLink($4)); }
    | I_CPI oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CPI", 2); }
;
ins_sbrc:
      I_SBRC expr "," expr          { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBRC, $2->appendLink($4)); }
    | I_SBRC oprs_2                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SBRC", 2); }
;
ins_sbrs:
      I_SBRS expr "," expr          { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBRS, $2->appendLink($4)); }
    | I_SBRS oprs_2                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SBRS", 2); }
;
ins_sbic:
      I_SBIC expr "," expr          { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBIC, $2->appendLink($4)); }
    | I_SBIC oprs_2                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SBIC", 2); }
;
ins_sbis:
      I_SBIS expr "," expr          { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBIS, $2->appendLink($4)); }
    | I_SBIS oprs_2                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SBIS", 2); }
;
ins_brbs:
      I_BRBS expr "," expr          { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRBS, $2->appendLink($4)); }
    | I_BRBS oprs_2                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRBS", 2); }
;
ins_brbc:
      I_BRBC expr "," expr          { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRBC, $2->appendLink($4)); }
    | I_BRBC oprs_2                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRBC", 2); }
;
ins_breq:
      I_BREQ expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BREQ, $2); }
    | I_BREQ oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BREQ", 1); }
;
ins_brne:
      I_BRNE expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRNE, $2); }
    | I_BRNE oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRNE", 1); }
;
ins_brcs:
      I_BRCS expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRCS, $2); }
    | I_BRCS oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRCS", 1); }
;
ins_brcc:
      I_BRCC expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRCC, $2); }
    | I_BRCC oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRCC", 1); }
;
ins_brsh:
      I_BRSH expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRSH, $2); }
    | I_BRSH oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRSH", 1); }
;
ins_brlo:
      I_BRLO expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRLO, $2); }
    | I_BRLO oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRLO", 1); }
;
ins_brmi:
      I_BRMI expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRMI, $2); }
    | I_BRMI oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRMI", 1); }
;
ins_brpl:
      I_BRPL expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRPL, $2); }
    | I_BRPL oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRPL", 1); }
;
ins_brge:
      I_BRGE expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRGE, $2); }
    | I_BRGE oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRGE", 1); }
;
ins_brlt:
      I_BRLT expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRLT, $2); }
    | I_BRLT oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRLT", 1); }
;
ins_brhs:
      I_BRHS expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRHS, $2); }
    | I_BRHS oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRHS", 1); }
;
ins_brhc:
      I_BRHC expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRHC, $2); }
    | I_BRHC oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRHC", 1); }
;
ins_brts:
      I_BRTS expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRTS, $2); }
    | I_BRTS oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRTS", 1); }
;
ins_brtc:
      I_BRTC expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRTC, $2); }
    | I_BRTC oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRTC", 1); }
;
ins_brvs:
      I_BRVS expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRVS, $2); }
    | I_BRVS oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRVS", 1); }
;
ins_brvc:
      I_BRVC expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRVC, $2); }
    | I_BRVC oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRVC", 1); }
;
ins_brie:
      I_BRIE expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRIE, $2); }
    | I_BRIE oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRIE", 1); }
;
ins_brid:
      I_BRID expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BRID, $2); }
    | I_BRID oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BRID", 1); }
;
/* DATA TRANSFER INSTRUCTIONS */
ins_mov:
      I_MOV expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_MOV, $2->appendLink($4)); }
    | I_MOV oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "MOV", 2); }
;
ins_ldi:
      I_LDI expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LDI, $2->appendLink($4)); }
    | I_LDI oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "LDI", 2); }
;
ins_lds:
      I_LDS expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LDS, $2->appendLink($4)); }
    | I_LDS oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "LDS", 2); }
;
ins_ld:
      I_LD expr "," X               { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_X, $2); }
    | I_LD expr "," X "+"           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_Xp, $2); }
    | I_LD expr "," "-" X           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_mX, $2); }
    | I_LD expr "," Y               { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_Y, $2); }
    | I_LD expr "," Y "+"           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_Yp, $2); }
    | I_LD expr "," "-" Y           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_mY, $2); }
    | I_LD expr "," Z               { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_Z, $2); }
    | I_LD expr "," Z "+"           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_Zp, $2); }
    | I_LD expr "," "-" Z           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LD_mZ, $2); }
    | I_LD oprs_1 xyz               { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@$, "LD", 2); }
    | I_LD expr "," expr            { /* Syntax Error */ $$ = NULL; REQ_SECOND_OPR("LD", "X, X+, -X, Y, Y+, -Y, Z, Z+, -Z", @2, $2->appendLink($4)); }
;
ins_ldd:
      I_LDD expr "," Y "+" expr     { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LDD_Y, $2->appendLink($6)); }
    | I_LDD expr "," Z "+" expr     { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LDD_Z, $2->appendLink($6)); }
    | I_LDD oprs_2 "," yz expr      { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "LDD", 2); $expr->completeDelete(); }
    | I_LDD expr "," expr           { /* Syntax Error */ $$ = NULL; REQ_SECOND_OPR("LDD", "Y+<expr>, Z+<expr>", @2, $2->appendLink($4)); }
;
ins_sts:
      I_STS expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_STS, $2->appendLink($4)); }
    | I_STS oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "STS", 2); }
;
ins_st:
      I_ST X "," expr               { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_X, $4); }
    | I_ST X "+" "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_Xp, $5); }
    | I_ST "-" X "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_mX, $5); }
    | I_ST Y "," expr               { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_Y, $4); }
    | I_ST Y "+" "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_Yp, $5); }
    | I_ST "-" Y "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_mY, $5); }
    | I_ST Z "," expr               { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_Z, $4); }
    | I_ST Z "+" "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_Zp, $5); }
    | I_ST "-" Z "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ST_mZ, $5); }
    | I_ST xyz oprs_1               { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@oprs_1, "ST", 2); }
    | I_ST expr "," expr            { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("ST", "X, X+, -X, Y, Y+, -Y, Z, Z+, -Z", @2, $2->appendLink($4)); }
;
ins_std:
      I_STD Y "+" expr "," expr     { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_STD_Y, $4->appendLink($6)); }
    | I_STD Z "+" expr "," expr     { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_STD_Z, $4->appendLink($6)); }
    | I_STD yz oprs_2               { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "STD", 2); }
    | I_STD expr "," expr           { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("STD", "Y+<expr>, Z+<expr>", @2, $2->appendLink($4)); }
;
ins_lpm:
      I_LPM                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LPM); }
    | I_LPM oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "LPM", 0); }
;
ins_in:
      I_IN expr "," expr            { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_IN, $2->appendLink($4)); }
    | I_IN oprs_2                   { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "IN", 2); }
;
ins_out:
      I_OUT expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_OUT, $2->appendLink($4)); }
    | I_OUT oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "OUT", 2); }
;
ins_push:
      I_PUSH expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_PUSH, $2); }
    | I_PUSH oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "PUSH", 1); }
;
ins_pop:
      I_POP expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_POP, $2); }
    | I_POP oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "POP", 1); }
;
/* BIT AND BIT-TEST INSTRUCTIONS */
ins_lsl:
      I_LSL expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LSL, $2); }
    | I_LSL oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "LSL", 1); }
;
ins_lsr:
      I_LSR expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_LSR, $2); }
    | I_LSR oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "LSR", 1); }
;
ins_rol:
      I_ROL expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ROL, $2); }
    | I_ROL oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ROL", 1); }
;
ins_ror:
      I_ROR expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ROR, $2); }
    | I_ROR oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ROR", 1); }
;
ins_asr:
      I_ASR expr                    { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_ASR, $2); }
    | I_ASR oprs_1                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ASR", 1); }
;
ins_swap:
      I_SWAP expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SWAP, $2); }
    | I_SWAP oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SWAP", 1); }
;
ins_bset:
      I_BSET expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BSET, $2); }
    | I_BSET oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BSET", 1); }
;
ins_bclr:
      I_BCLR expr                   { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BCLR, $2); }
    | I_BCLR oprs_1                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BCLR", 1); }
;
ins_sbi:
      I_SBI expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SBI, $2->appendLink($4)); }
    | I_SBI oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SBI", 2); }
;
ins_cbi:
      I_CBI expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CBI, $2->appendLink($4)); }
    | I_CBI oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CBI", 2); }
;
ins_bst:
      I_BST expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BST, $2->appendLink($4)); }
    | I_BST oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BST", 2); }
;
ins_bld:
      I_BLD expr "," expr           { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_BLD, $2->appendLink($4)); }
    | I_BLD oprs_2                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "BLD", 2); }
;
ins_sec:
      I_SEC                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SEC); }
    | I_SEC oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SEC", 0); }
;
ins_clc:
      I_CLC                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLC); }
    | I_CLC oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CLC", 0); }
;
ins_sen:
      I_SEN                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SEN); }
    | I_SEN oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SEN", 0); }
;
ins_cln:
      I_CLN                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLN); }
    | I_CLN oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CLN", 0); }
;
ins_sez:
      I_SEZ                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SEZ); }
    | I_SEZ oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SEZ", 0); }
;
ins_clz:
      I_CLZ                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLZ); }
    | I_CLZ oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CLZ", 0); }
;
ins_sei:
      I_SEI                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SEI); }
    | I_SEI oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SEI", 0); }
;
ins_cli:
      I_CLI                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLI); }
    | I_CLI oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CLI", 0); }
;
ins_ses:
      I_SES                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SES); }
    | I_SES oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SES", 0); }
;
ins_cls:
      I_CLS                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLS); }
    | I_CLS oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CLS", 0); }
;
ins_sev:
      I_SEV                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SEV); }
    | I_SEV oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SEV", 0); }
;
ins_clv:
      I_CLV                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLV); }
    | I_CLV oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CLV", 0); }
;
ins_set:
      I_SET                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SET); }
    | I_SET oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SET", 0); }
;
ins_clt:
      I_CLT                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLT); }
    | I_CLT oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CLT", 0); }
;
ins_seh:
      I_SEH                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SEH); }
    | I_SEH oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SEH", 0); }
;
ins_clh:
      I_CLH                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_CLH); }
    | I_CLH oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "CLH", 0); }
;
ins_nop:
      I_NOP                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_NOP); }
    | I_NOP oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "NOP", 0); }
;
ins_sleep:
      I_SLEEP                       { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_SLEEP); }
    | I_SLEEP oprs_0                { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SLEEP", 0); }
;
ins_wdr:
      I_WDR                         { $$ = new CompilerStatement(LOC(), ASMAVR8_INS_WDR); }
    | I_WDR oprs_0                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "WDR", 0); }
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
xyz:
      X
    | X "+"
    | "-" X
    | Y
    | Y "+"
    | "-" Y
    | Z
    | Z "+"
    | "-" Z
;
yz:
      Y "+"
    | Z "+"
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
