// =============================================================================
/**
 * @brief
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 */
// =============================================================================


// -----------------------------------------------------------------------------
// PROLOGUE - BISON DECLARATIONS
// -----------------------------------------------------------------------------

// Require version <version> or higher of Bison.
%require "2.5"
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
// Expect exactly <n> shift/reduce conflicts in this grammar
%expect 290
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
    using namespace StatementTypes; // This NS is heavily used here.

    // Declaration of the lexer prototypes and other things required by Bison
    #include "mcs51lexer.h"

    // Name of the lexer function required by Bison
    #define mcs51parser_lex mcs51lexer_lex

    // Make a few things a little easier...
    #define LOC(location) \
        ( compiler->toSourceLocation(location) )

    // Error messages
    #define N_OPERANDS_EXPECTED(location, instruction, number) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("invalid number of operands, instruction %1 takes %2 operand(s)").arg(instruction).arg(number).toStdString() );
    #define MISSIGN_COMMA(location, expression) \
        CompilerExpr::completeDelete(expression); \
        location.first_line = location.last_line; \
        location.first_column = location.last_column; \
        location.last_column++; \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("comma (`,') expected between operands").toStdString() );
    #define REQ_FIRST_OPR(instruction, operand, location, expression) \
        CompilerExpr::completeDelete(expression); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("the first operand of %1 is required to be one of these: { %2 }").arg(instruction).arg(operand).toStdString() );
    #define NO_LABEL_EXPECTED(location, directive, statement) \
        CompilerStatement::completeDelete(statement); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("cannot declare a label before %1 directive").arg(directive).toStdString() );
    #define ARG_REQUIRED_C(location, control) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("assembler control %1 requires a single argument").arg(control).toStdString() );
    #define ARG_REQUIRED_D(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 requires a single argument").arg(directive).toStdString() );
    #define NO_ARG_EXPECTED_C(location, control) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("assembler control %1 takes no arguments").arg(control).toStdString() );
    #define NO_ARG_EXPECTED_D(directive, args, location) \
        args->completeDelete(); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 takes no arguments").arg(directive).toStdString() );
    #define MISSING_AT_OPERATOR(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("segment specifier %1 requires `AT' operator before the start address").arg(directive).toStdString() );
    #define DECL_ID_EXPECTED(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 requires an identifier for the symbol (or macro) which it defines").arg(directive).toStdString() );
    #define DEPRECATED_DIR(location, directive, substitute) \
        compiler->parserMessage ( location, CompilerBase::MT_WARNING, \
            QObject::tr("%1 is a deprecated directive, consider usage of directive %2 instead").arg(directive).arg(substitute).toStdString() );

    // Declaration of the error reporting function used by Bison
    inline int mcs51parser_error (
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
%token D_DB             D_MACRO         D_ENDM          D_BIT           D_BSEG
%token D_BYTE           D_CODE          D_CSEG          D_DATA          D_DBIT
%token D_DS             D_DSEG          D_DW            D_ELSE          D_ELSEIF
%token D_ELSEIFDEF      D_ELSEIFN       D_ELSEIFNDEF    D_END           D_ENDIF
%token D_EQU            D_FLAG          D_IDATA         D_IF            D_IFDEF
%token D_IFN            D_IFNDEF        D_ISEG          D_LIST          D_NAME
%token D_NOLIST         D_ORG           D_REPT          D_SET           D_SKIP
%token D_USING          D_XDATA         D_XSEG          D_IFB           D_IFNB
%token D_LOCAL          D_ELSEIFNB      D_EXITM         D_ELSEIFB

/* Assembler Controls */
%token C_COND           C_GO            C_NODEBUG       C_NOSYMBOLS     C_RS
%token C_CONDONLY       C_IC            C_NOGE          C_NOTABS        C_SA
%token C_DA             C_NOGEN         C_NOXR          C_SAVE          C_RESTORE
%token C_DATE           C_LI            C_NOLI          C_NOXREF        C_SB
%token C_DB             C_LIST          C_NOLIST        C_PAGELENGTH    C_SYMBOLS
%token C_DEBUG          C_MACRO         C_NOMACRO       C_PAGEWIDTH     C_TITLE
%token C_EJ             C_MO            C_NOMO          C_PAGING        C_TT
%token C_EJECT          C_MOD51         C_NOMOD51       C_PHILIPS       C_WARNING
%token C_ERROR          C_MR            C_NOMR          C_PI            C_XR
%token C_GE             C_NOBUILTIN     C_NOPAGING      C_PL            C_XREF
%token C_GEN            C_NOCOND        C_NOPI          C_PW            C_GENONLY
%token C_NODB           C_NOSB

/* Instructions */
%token I_ACALL  I_ADD   I_ADDC  I_AJMP  I_ANL   I_CJNE  I_CLR   I_CPL   I_DA    I_DEC
%token I_DIV    I_DJNZ  I_INC   I_JB    I_JBC   I_JC    I_JMP   I_JNB   I_JNC   I_JNZ
%token I_JZ     I_LCALL I_LJMP  I_MOV   I_MOVC  I_MOVX  I_MUL   I_NOP   I_ORL   I_POP
%token I_PUSH   I_RET   I_RETI  I_RL    I_RLC   I_RR    I_RRC   I_SETB  I_SJMP  I_SUBB
%token I_SWAP   I_XCH   I_XCHD  I_XRL

/* Fixed operands, i.e. those which have no value */
%token IND_R0           "@R0"
%token IND_R1           "@R1"
%token IND_A_DPTR       "@A+DPTR"
%token IND_A_PC         "@A+PC"
%token IND_DPTR         "@DPTR"
%token REG_A            "processor register A"
%token REG_AB           "processor \"register\" AB"
%token BIT_C            "processor bit C"
%token REG_DPTR         "processor register DPTR"
%token REG_0            "processor register R0"
%token REG_1            "processor register R1"
%token REG_2            "processor register R2"
%token REG_3            "processor register R3"
%token REG_4            "processor register R4"
%token REG_5            "processor register R5"
%token REG_6            "processor register R6"
%token REG_7            "processor register R7"

/* Other terminal symbols */
%token COMMENT
%token AT
%token EOL              "end of line"
%token END      0       "end of file"
// Named tokens (to make the grammar rules more readable)
%token IMMEDIATE        "#"
%token LP               "("
%token RP               ")"
%token COMMA            ","
// Expression operators
%token HIGH
%token LOW
%token PLUS             "+"
%token MINUS            "-"
%token ASTERISK         "*"
%token SLASH            "/"
%token DOT              "."
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
%left "."
%left LOW HIGH
// Right-to-left
%right "~"

/* Terminal symbols with semantic value */
  // semantic value is a string
%token<string>  IDENFIFIER      LABEL
%token<array>   STRING          INCLUDE
  // semantic value is a number
%token<number>  NUMBER

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Expressions
%type<expr>     expr            "expression"
%type<expr>     e_expr          "expression "
%type<expr>     number          params          args            const_a         const_c
%type<expr>     id              string          args_str
// Statements - general
%type<stmt>     statements      stmt            inst_stmt       dir_stmt        macro_stmt
%type<stmt>     instruction     directive       macro           label
// Statements - directives
%type<stmt>     dir_macro_d     dir_macro       dir_endm        dir_db          dir_include
%type<stmt>     dir_cond_asm    if_block        ifelse_block    else_block      dir_if
%type<stmt>     dir_ifn         dir_ifdef       dir_ifndef      dir_ifb         dir_ifnb
%type<stmt>     dir_elseif      dir_elseifn     dir_elseifdef   dir_elseifndf   dir_elseifb
%type<stmt>     dir_elseifnb    dir_rept        dir_dbit        dir_ds          dir_dw
%type<stmt>     dir_skip        dir_name        dir_local       dir_exitm       dir_list
%type<stmt>     dir_nolist      dir_repeat      dir_org         dir_bseg        dir_cseg
%type<stmt>     dir_dseg        dir_iseg        dir_xseg        dir_using       dir_data
%type<stmt>     dir_byte        dir_idata       dir_xdata       dir_code        dir_set
%type<stmt>     dir_equ dir_bit dir_flag        dir_if_a        dir_ifn_a       dir_ifdef_a
%type<stmt>     dir_ifndef_a    dir_ifb_a       dir_ifnb_a      dir_else        dir_elseif_a
%type<stmt>     dir_elseifn_a   dir_elseifdef_a dir_elseifndf_a dir_elseifb_a   dir_elseifnb_a
%type<stmt>     dir_macro_a     dir_bseg_a      dir_cseg_a      dir_dseg_a      dir_iseg_a
%type<stmt>     dir_xseg_a      end_of_code
// Statements - assembler controls
%type<stmt>     asm_ctrl_stmt   asm_ctrl        ctrl_cond       ctrl_nodebug    ctrl_nosymbols
%type<stmt>     ctrl_condonly   ctrl_notabs     ctrl_nogen      ctrl_save       ctrl_date
%type<stmt>     ctrl_noxref     ctrl_list       ctrl_nolist     ctrl_pagelength ctrl_symbols
%type<stmt>     ctrl_debug      ctrl_macro      ctrl_nomacro    ctrl_pagewidth  ctrl_title
%type<stmt>     ctrl_paging     ctrl_eject      ctrl_mod51      ctrl_nomod51    ctrl_philips
%type<stmt>     ctrl_warning    ctrl_error      ctrl_nobuiltin  ctrl_nopaging   ctrl_xref
%type<stmt>     ctrl_gen        ctrl_nocond     ctrl_genonly    ctrl_restore
// Statements - instructions
%type<stmt>     ins_acall       ins_add         ins_addc        ins_ajmp        ins_anl
%type<stmt>     ins_cjne        ins_clr         ins_cpl         ins_da          ins_dec
%type<stmt>     ins_div         ins_djnz        ins_inc         ins_jb          ins_jbc
%type<stmt>     ins_jc          ins_jmp         ins_jnb         ins_jnc         ins_jnz
%type<stmt>     ins_jz          ins_lcall       ins_ljmp        ins_mov         ins_movc
%type<stmt>     ins_movx        ins_mul         ins_nop         ins_orl         ins_pop
%type<stmt>     ins_push        ins_ret         ins_reti        ins_rl          ins_rlc
%type<stmt>     ins_rr          ins_rrc         ins_setb        ins_sjmp        ins_subb
%type<stmt>     ins_swap        ins_xch         ins_xchd        ins_xrl


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
    | asm_ctrl_stmt                 { $$ = $1; }
    | inst_stmt                     { $$ = $1; }
    | macro_stmt                    { $$ = $1; }
;
dir_stmt:
      directive                     { $$ = $directive; }
    | directive COMMENT             { $$ = $directive; }
;
asm_ctrl_stmt:
      asm_ctrl                      { $$ = $asm_ctrl; }
    | asm_ctrl COMMENT              { $$ = $asm_ctrl; }
    | label asm_ctrl                {
                        /* Syntax error */
                        $$ = NULL;
                        NO_LABEL_EXPECTED(@label, "an assembler control", $label->appendLink($asm_ctrl));
                    }
    | label asm_ctrl COMMENT        {
                        /* Syntax error */
                        $$ = NULL;
                        NO_LABEL_EXPECTED(@label, "an assembler control", $label->appendLink($asm_ctrl));
                    }
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
      id                            { $$ = new CompilerStatement(LOC(@$), ASM51_MACRO, $id); }
    | id "(" ")"                    { $$ = new CompilerStatement(LOC(@$), ASM51_MACRO, $id); }
    | id args                       { $$ = new CompilerStatement(LOC(@$), ASM51_MACRO, $id->appendLink($args)); }
    | id "(" args ")"               { $$ = new CompilerStatement(LOC(@$), ASM51_MACRO, $id->appendLink($args)); }
;

/*
 * Generally used non-terminal symbols
 */
number:
      NUMBER                        { $$ = new CompilerExpr($1); }
;
id:
      IDENFIFIER                    { $$ = new CompilerExpr($1); }
;
string:
      STRING                        { $$ = new CompilerExpr(CompilerExpr::Value($1.data, $1.size)); }
;
label:
      LABEL                         { $$ = new CompilerStatement(LOC(@$), ASM51_LABEL, new CompilerExpr($1)); }
;
const_a:
      REG_A {
        $$ = new CompilerExpr("A");
        compiler->parserMessage (
            @1, CompilerBase::MT_REMARK,
            QObject::tr("treating keyword `A' as a constant (equals to 0xE0)").toStdString() );
    }
;
const_c:
      BIT_C {
        $$ = new CompilerExpr("C");
        compiler->parserMessage (
            @1, CompilerBase::MT_REMARK,
            QObject::tr("treating keyword `C' as a constant (equals to 0xD7)").toStdString() );
    }
;
expr:
      id                            { $$ = $id; }
    | number                        { $$ = $number; }
    | "(" expr ")"                  { $$ = $2; }
    | LOW expr                      { $$ = new CompilerExpr(CompilerExpr::OPER_LOW, $2); }
    | HIGH expr                     { $$ = new CompilerExpr(CompilerExpr::OPER_HIGH, $2); }
    | "~" expr                      { $$ = new CompilerExpr('~', $2); }
    | expr "+" expr                 { $$ = new CompilerExpr($1, '+', $3); }
    | expr "-" expr                 { $$ = new CompilerExpr($1, '-', $3); }
    | expr "*" expr                 { $$ = new CompilerExpr($1, '*', $3); }
    | expr "/" expr                 { $$ = new CompilerExpr($1, '/', $3); }
    | expr "%" expr                 { $$ = new CompilerExpr($1, '%', $3); }
    | expr "." expr                 { $$ = new CompilerExpr($1, '.', $3); }
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
    | expr "=>" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_GE, $3); }
    | expr ">" expr                 { $$ = new CompilerExpr($1, '>', $3); }
    | expr ">>" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_SHR, $3); }
    | expr "<<" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_SHL, $3); }
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

    /* Allow `A' to be used as a constant for +/- */
    | const_a "+" expr              { $$ = new CompilerExpr($1, '+', $3); }
    | const_a "-" expr              { $$ = new CompilerExpr($1, '-', $3); }
    | expr "+" const_a              { $$ = new CompilerExpr($1, '+', $3); }
    | expr "-" const_a              { $$ = new CompilerExpr($1, '-', $3); }

    /* Allow `C' to be used as a constant for +/- */
    | const_c "+" expr              { $$ = new CompilerExpr($1, '+', $3); }
    | const_c "-" expr              { $$ = new CompilerExpr($1, '-', $3); }
    | expr "+" const_c              { $$ = new CompilerExpr($1, '+', $3); }
    | expr "-" const_c              { $$ = new CompilerExpr($1, '-', $3); }
;
e_expr:         // Expression allowing use of single `A' or `C', e.g. `A', this cannot be used with instructions
      expr                          { $$ = $expr; }
    | const_a                       { $$ = $const_a; }
    | const_c                       { $$ = $const_c; }
;
opr:            // Matches any operand, intended for error recovery only
      e_expr                        { $e_expr->completeDelete(); }
    | "#" e_expr                    { $e_expr->completeDelete(); }

    | IND_R0                        | IND_R1
    | IND_A_DPTR                    | IND_A_PC
    | IND_DPTR                      | REG_AB
    | REG_DPTR                      | REG_0
    | REG_1                         | REG_2
    | REG_3                         | REG_4
    | REG_5                         | REG_6
    | REG_7
;
oprs:           // Matches any set of operands, intended for error recovery only
      oprs "," opr                  { }
    | opr                           { }
;
args:           // List of arguments without strings, e.g. `(1+3), XYZ, 0x4b'
      args "," e_expr               { $$ = $1->appendLink($e_expr); }
    | e_expr                        { $$ = $e_expr; }
;
args_str:       // List of arguments possibly with strings, e.g. `ABC, "XYZ - QWE", 2 * 0x4b'
      args_str "," string           { $$ = $1->appendLink($string); }
    | args_str "," e_expr           { $$ = $1->appendLink($e_expr); }
    | string                        { $$ = $string; }
    | e_expr                        { $$ = $e_expr; }
;
params:         // List of identifiers, e.g. `target, source, offset, size, mode'
      params "," id                 { $$ = $1->appendLink($id); }
    | id                            { $$ = $id; }
;

/*
 * Directives
 */
directive:
      dir_idata     { $$ = $1; }    | dir_byte      { $$ = $1; DEPRECATED_DIR(@1, "BYTE", "DATA"); }
    | dir_bit       { $$ = $1; }    | dir_flag      { $$ = $1; DEPRECATED_DIR(@1, "FLAG", "BIT"); }
    | dir_cond_asm  { $$ = $1; }    | dir_dbit      { $$ = $1; }
    | dir_ds        { $$ = $1; }    | dir_dw        { $$ = $1; }
    | dir_skip      { $$ = $1; }    | dir_name      { $$ = $1; }
    | dir_local     { $$ = $1; }    | dir_exitm     { $$ = $1; }
    | dir_list      { $$ = $1; }    | dir_include   { $$ = $1; }
    | dir_nolist    { $$ = $1; }    | dir_repeat    { $$ = $1; }
    | dir_org       { $$ = $1; }    | dir_bseg      { $$ = $1; }
    | dir_cseg      { $$ = $1; }    | dir_dseg      { $$ = $1; }
    | dir_iseg      { $$ = $1; }    | dir_xseg      { $$ = $1; }
    | dir_using     { $$ = $1; }    | dir_data      { $$ = $1; }
    | dir_xdata     { $$ = $1; }    | dir_code      { $$ = $1; }
    | dir_set       { $$ = $1; }    | dir_equ       { $$ = $1; }
    | dir_macro     { $$ = $1; }    | dir_db        { $$ = $1; }
    | end_of_code   { $$ = $1; }
;
dir_cond_asm:
      if_block ifelse_block else_block D_ENDIF {
        $$ = new CompilerStatement(LOC(@$), ASM51_COND_ASM);
        $$->createBranch ( $if_block->appendLink($ifelse_block)->appendLink($else_block) );
    }
    | label if_block ifelse_block else_block D_ENDIF {
        $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_COND_ASM));
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
      D_ELSE                        { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ELSE); }
    | label D_ELSE                  {
                        /* Syntax error */
                        NO_LABEL_EXPECTED(@label, "ELSE", $label);
                        $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ELSE);
                    }
    | label D_ELSE args             {
                        /* Syntax error */
                        NO_ARG_EXPECTED_D("ELSE", $args, @args);
                        NO_LABEL_EXPECTED(@label, "ELSE", $label);
                        $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ELSE);
                    }
    | D_ELSE args                   {
                        /* Syntax error */
                        NO_ARG_EXPECTED_D("ELSE", $args, @args);
                        $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ELSE);
                    }
;
dir_if:
      dir_if_a EOL                  { $$ = $dir_if_a; }
    | dir_if_a COMMENT EOL          { $$ = $dir_if_a; }
;
dir_if_a:
      D_IF e_expr                   { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_IF, $e_expr); }
    | D_IF                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IF, "IF"); }
;
dir_ifn:
      dir_ifn_a EOL                 { $$ = $dir_ifn_a; }
    | dir_ifn_a COMMENT EOL         { $$ = $dir_ifn_a; }
;
dir_ifn_a:
      D_IFN e_expr                  { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_IFN, $e_expr); }
    | D_IFN                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFN, "IFN"); }
;
dir_ifdef:
      dir_ifdef_a EOL               { $$ = $dir_ifdef_a; }
    | dir_ifdef_a COMMENT EOL       { $$ = $dir_ifdef_a; }
;
dir_ifdef_a:
      D_IFDEF id                    { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_IFDEF, $id); }
    | D_IFDEF                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFDEF, "IFDEF"); }
;
dir_ifndef:
      dir_ifndef_a EOL              { $$ = $dir_ifndef_a; }
    | dir_ifndef_a COMMENT EOL      { $$ = $dir_ifndef_a; }
;
dir_ifndef_a:
      D_IFNDEF id                   { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_IFNDEF, $id); }
    | D_IFNDEF                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFNDEF, "IFNDEF"); }
;
dir_ifb:
      dir_ifb_a EOL                 { $$ = $dir_ifb_a; }
    | dir_ifb_a COMMENT EOL         { $$ = $dir_ifb_a; }
;
dir_ifb_a:
      D_IFB id                      { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_IFB, $id); }
    | D_IFB                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFB, "IFB"); }
;
dir_ifnb:
      dir_ifnb_a EOL                { $$ = $dir_ifnb_a; }
    | dir_ifnb_a COMMENT EOL        { $$ = $dir_ifnb_a; }
;
dir_ifnb_a:
      D_IFNB id                     { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_IFNB, $id); }
    | D_IFNB                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFNB, "IFNB"); }
;
dir_elseif:
      dir_elseif_a EOL              { $$ = $dir_elseif_a; }
    | dir_elseif_a COMMENT EOL      { $$ = $dir_elseif_a; }
;
dir_elseif_a:
      D_ELSEIF e_expr               { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ELSEIF, $e_expr); }
    | D_ELSEIF                      { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIF, "ELSEIF"); }
    | label D_ELSEIF e_expr         { /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIF", $label); $e_expr->completeDelete(); }
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
      D_ELSEIFN e_expr              { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ELSEIFN, $e_expr); }
    | D_ELSEIFN                     { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFN, "ELSEIFN"); }
    | label D_ELSEIFN e_expr        { /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFN", $label); $e_expr->completeDelete(); }
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
      D_ELSEIFDEF e_expr            { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ELSEIFDEF, $e_expr); }
    | D_ELSEIFDEF                   { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFDEF, "ELSEIFDEF"); }
    | label D_ELSEIFDEF e_expr      { /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFDEF", $label); $e_expr->completeDelete(); }
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
      D_ELSEIFNDEF e_expr           { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ELSEIFNDEF, $e_expr); }
    | D_ELSEIFNDEF                  { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFNDEF, "ELSEIFNDEF"); }
    | label D_ELSEIFNDEF e_expr     { /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFNDEF", $label); $e_expr->completeDelete(); }
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
      D_ELSEIFB e_expr              { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ELSEIFB, $e_expr); }
    | D_ELSEIFB                     { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFB, "ELSEIFB"); }
    | label D_ELSEIFB e_expr        { /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFB", $label); $e_expr->completeDelete(); }
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
      D_ELSEIFNB e_expr             { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ELSEIFNB, $e_expr); }
    | D_ELSEIFNB                    { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIFNB, "ELSEIFNB"); }
    | label D_ELSEIFNB e_expr       { /* Syntax Error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "ELSEIFNB", $label); $e_expr->completeDelete(); }
    | label D_ELSEIFNB              {
                        /* Syntax Error */
                        $$ = NULL;
                        ARG_REQUIRED_D(@D_ELSEIFNB, "ELSEIFNB");
                        NO_LABEL_EXPECTED(@label, "ELSEIFNB", $label);
                    }
;
dir_org:
      D_ORG e_expr                  { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ORG, $e_expr); }
    | label D_ORG e_expr            { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_ORG, $e_expr)); }
    | D_ORG                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_ORG, "ORG"); }
    | label D_ORG                   { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_ORG, "ORG"); $label->completeDelete(); }
;
dir_bseg:
      dir_bseg_a                    { $$ = $dir_bseg_a; }
    | label dir_bseg_a              { $$ = $label->appendLink($dir_bseg_a); }
;
dir_bseg_a:
      D_BSEG                        { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_BSEG); }
    | D_BSEG AT e_expr              { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_BSEG, $e_expr); }
    | D_BSEG e_expr                 {
                        /* Syntax error */ MISSING_AT_OPERATOR(@D_BSEG, "D_BSEG");
                        $$ = new CompilerStatement(LOC(@$), ASM51_DIR_BSEG, $e_expr);
                    }
;
dir_cseg:
      dir_cseg_a                    { $$ = $dir_cseg_a; }
    | label dir_cseg_a              { $$ = $label->appendLink($dir_cseg_a); }
;
dir_cseg_a:
      D_CSEG                        { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_CSEG); }
    | D_CSEG AT e_expr              { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_CSEG, $e_expr); }
    | D_CSEG e_expr                 {
                        /* Syntax error */ MISSING_AT_OPERATOR(@D_CSEG, "D_CSEG");
                        $$ = new CompilerStatement(LOC(@$), ASM51_DIR_CSEG, $e_expr);
                    }
;
dir_dseg:
      dir_dseg_a                    { $$ = $dir_dseg_a; }
    | label dir_dseg_a              { $$ = $label->appendLink($dir_dseg_a); }
;
dir_dseg_a:
      D_DSEG                        { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_DSEG); }
    | D_DSEG AT e_expr              { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_DSEG, $e_expr); }
    | D_DSEG e_expr                 {
                        /* Syntax error */ MISSING_AT_OPERATOR(@D_DSEG, "D_DSEG");
                        $$ = new CompilerStatement(LOC(@$), ASM51_DIR_DSEG, $e_expr);
                    }
;
dir_iseg:
      dir_iseg_a                    { $$ = $dir_iseg_a; }
    | label dir_iseg_a              { $$ = $label->appendLink($dir_iseg_a); }
;
dir_iseg_a:
      D_ISEG                        { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ISEG); }
    | D_ISEG AT e_expr              { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ISEG, $e_expr); }
    | D_ISEG e_expr                 {
                        /* Syntax error */ MISSING_AT_OPERATOR(@D_ISEG, "D_ISEG");
                        $$ = new CompilerStatement(LOC(@$), ASM51_DIR_ISEG, $e_expr);
                    }
;
dir_xseg:
      dir_xseg_a                    { $$ = $dir_xseg_a; }
    | label dir_xseg_a              { $$ = $label->appendLink($dir_xseg_a); }
;
dir_xseg_a:
      D_XSEG                        { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_XSEG); }
    | D_XSEG AT e_expr              { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_XSEG, $e_expr); }
    | D_XSEG e_expr                 {
                        /* Syntax error */ MISSING_AT_OPERATOR(@D_XSEG, "D_XSEG");
                        $$ = new CompilerStatement(LOC(@$), ASM51_DIR_XSEG, $e_expr);
                    }
;
dir_dbit:
      D_DBIT e_expr                 { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_DBIT, $e_expr); }
    | label D_DBIT e_expr           { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_DBIT, $e_expr)); }
    | D_DBIT                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DBIT, "DBIT"); }
    | label D_DBIT                  { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DBIT, "DBIT"); $label->completeDelete(); }
;
dir_ds:
      D_DS e_expr                   { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_DS, $e_expr); }
    | label D_DS e_expr             { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_DS, $e_expr)); }
    | D_DS                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DS, "D_DS"); }
    | label D_DS                    { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DS, "D_DS"); $label->completeDelete(); }
;
dir_dw:
      D_DW e_expr                   { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_DW, $e_expr); }
    | label D_DW e_expr             { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_DW, $e_expr)); }
    | D_DW                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DW, "D_DW"); }
    | label D_DW                    { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DW, "D_DW"); $label->completeDelete(); }
;
dir_skip:
      D_SKIP e_expr                 { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_SKIP, $e_expr); }
    | label D_SKIP e_expr           { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_SKIP, $e_expr)); }
    | D_SKIP                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_SKIP, "SKIP"); }
    | label D_SKIP                  { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_SKIP, "SKIP"); $label->completeDelete(); }
;
dir_name:
      D_NAME string                 { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_NAME, $string); }
    | label D_NAME string           { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_NAME, $string)); }
    | D_NAME                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_NAME, "NAME"); }
    | label D_NAME                  { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_NAME, "NAME"); $label->completeDelete(); }
;
dir_local:
      D_LOCAL id                    { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_LOCAL, $id); }
    | label D_LOCAL id              { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_LOCAL, $id)); }
    | D_LOCAL                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_LOCAL, "LOCAL"); }
    | label D_LOCAL                 { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_LOCAL, "LOCAL"); $label->completeDelete(); }
;
dir_exitm:
      D_EXITM                       { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_EXITM); }
    | label D_EXITM                 { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_EXITM)); }
    | D_EXITM args                  { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("EXITM", $args, @args); }
    | label D_EXITM args            { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("EXITM", $args, @args); $label->completeDelete(); }
;
dir_list:
      D_LIST                        { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_LIST); }
    | label D_LIST                  { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_LIST)); }
    | D_LIST args                   { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("LIST", $args, @args); }
    | label D_LIST args             { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("LIST", $args, @args); $label->completeDelete(); }
;
dir_nolist:
      D_NOLIST                      { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_NOLIST); }
    | label D_NOLIST                { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_NOLIST)); }
    | D_NOLIST args                 { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("NOLIST", $args, @args); }
    | label D_NOLIST args           { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("NOLIST", $args, @args); $label->completeDelete(); }
;
dir_using:
      D_USING e_expr                { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_USING, $e_expr); }
    | label D_USING e_expr          { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_USING, $e_expr)); }
    | D_USING                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_USING, "USING"); }
    | label D_USING                 { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_USING, "USING"); $label->completeDelete(); }
;
dir_data:
      id D_DATA e_expr              { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_DATA, $id->appendLink($e_expr)); }
    | id D_DATA                     { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DATA, "DATA"); $id->completeDelete(); }
    | D_DATA                        { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_DATA, "DATA"); ARG_REQUIRED_D(@D_DATA, "DATA"); }
    | D_DATA e_expr                 { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_DATA, "DATA"); $e_expr->completeDelete(); }
    | label D_DATA e_expr           { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_DATA, "DATA"); NO_LABEL_EXPECTED(@label, "DATA", $label->appendArgsLink($e_expr)) }
    | label id D_DATA e_expr        { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "DATA", $label->appendArgsLink($id->appendLink($e_expr))); }
;
dir_byte:
      id D_BYTE e_expr              { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_BYTE, $id->appendLink($e_expr)); }
    | id D_BYTE                     { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_BYTE, "BYTE"); $id->completeDelete(); }
    | D_BYTE                        { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_BYTE, "BYTE"); ARG_REQUIRED_D(@D_BYTE, "BYTE"); }
    | D_BYTE e_expr                 { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_BYTE, "BYTE"); $e_expr->completeDelete(); }
    | label D_BYTE e_expr           { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_BYTE, "BYTE"); NO_LABEL_EXPECTED(@label, "BYTE", $label->appendArgsLink($e_expr)) }
    | label id D_BYTE e_expr        { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "BYTE", $label->appendArgsLink($id->appendLink($e_expr))); }
;
dir_idata:
      id D_IDATA e_expr             { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_IDATA, $id->appendLink($e_expr)); }
    | id D_IDATA                    { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IDATA, "IDATA"); $id->completeDelete(); }
    | D_IDATA                       { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_IDATA, "IDATA"); ARG_REQUIRED_D(@D_IDATA, "IDATA"); }
    | D_IDATA e_expr                { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_IDATA, "IDATA"); $e_expr->completeDelete(); }
    | label D_IDATA e_expr          { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_IDATA, "IDATA"); NO_LABEL_EXPECTED(@label, "IDATA", $label->appendArgsLink($e_expr)) }
    | label id D_IDATA e_expr       { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "IDATA", $label->appendArgsLink($id->appendLink($e_expr))); }
;
dir_xdata:
      id D_XDATA e_expr             { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_XDATA, $id->appendLink($e_expr)); }
    | id D_XDATA                    { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_XDATA, "XDATA"); $id->completeDelete(); }
    | D_XDATA                       { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_XDATA, "XDATA"); ARG_REQUIRED_D(@D_XDATA, "XDATA"); }
    | D_XDATA e_expr                { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_XDATA, "XDATA"); $e_expr->completeDelete(); }
    | label D_XDATA e_expr          { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_XDATA, "XDATA"); NO_LABEL_EXPECTED(@label, "XDATA", $label->appendArgsLink($e_expr)) }
    | label id D_XDATA e_expr       { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "XDATA", $label->appendArgsLink($id->appendLink($e_expr))); }
;
dir_code:
      id D_CODE e_expr              { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_CODE, $id->appendLink($e_expr)); }
    | id D_CODE                     { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_CODE, "CODE"); $id->completeDelete(); }
    | D_CODE                        { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_CODE, "CODE"); ARG_REQUIRED_D(@D_CODE, "CODE"); }
    | D_CODE e_expr                 { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_CODE, "CODE"); $e_expr->completeDelete(); }
    | label D_CODE e_expr           { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_CODE, "CODE"); NO_LABEL_EXPECTED(@label, "CODE", $label->appendArgsLink($e_expr)) }
    | label id D_CODE e_expr        { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "CODE", $label->appendArgsLink($id->appendLink($e_expr))); }
;
dir_set:
      id D_SET e_expr               { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_SET, $id->appendLink($e_expr)); }
    | id D_SET                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_SET, "SET"); $id->completeDelete(); }
    | D_SET                         { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_SET, "SET"); ARG_REQUIRED_D(@D_SET, "SET"); }
    | D_SET e_expr                  { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_SET, "SET"); $e_expr->completeDelete(); }
    | label D_SET e_expr            { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_SET, "SET"); NO_LABEL_EXPECTED(@label, "SET", $label->appendArgsLink($e_expr)) }
    | label id D_SET e_expr         { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "SET", $label->appendArgsLink($id->appendLink($e_expr))); }
;
dir_equ:
      id D_EQU e_expr               { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_EQU, $id->appendLink($e_expr)); }
    | id D_EQU                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_EQU, "EQU"); $id->completeDelete(); }
    | D_EQU                         { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_EQU, "EQU"); ARG_REQUIRED_D(@D_EQU, "EQU"); }
    | D_EQU e_expr                  { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_EQU, "EQU"); $e_expr->completeDelete(); }
    | label D_EQU e_expr            { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_EQU, "EQU"); NO_LABEL_EXPECTED(@label, "EQU", $label->appendArgsLink($e_expr)) }
    | label id D_EQU e_expr         { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "EQU", $label->appendArgsLink($id->appendLink($e_expr))); }
;
dir_bit:
      id D_BIT e_expr               { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_BIT, $id->appendLink($e_expr)); }
    | id D_BIT                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_BIT, "BIT"); $id->completeDelete(); }
    | D_BIT                         { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_BIT, "BIT"); ARG_REQUIRED_D(@D_BIT, "BIT"); }
    | D_BIT e_expr                  { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_BIT, "BIT"); $e_expr->completeDelete(); }
    | label D_BIT e_expr            { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_BIT, "BIT"); NO_LABEL_EXPECTED(@label, "BIT", $label->appendArgsLink($e_expr)) }
    | label id D_BIT e_expr         { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "BIT", $label->appendArgsLink($id->appendLink($e_expr))); }
;
dir_flag:
      id D_FLAG e_expr              { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_FLAG, $id->appendLink($e_expr)); }
    | id D_FLAG                     { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_FLAG, "FLAG"); $id->completeDelete(); }
    | D_FLAG                        { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_FLAG, "FLAG"); ARG_REQUIRED_D(@D_FLAG, "FLAG"); }
    | D_FLAG e_expr                 { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_FLAG, "FLAG"); $e_expr->completeDelete(); }
    | label D_FLAG e_expr           { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_FLAG, "FLAG"); NO_LABEL_EXPECTED(@label, "FLAG", $label->appendArgsLink($e_expr)) }
    | label id D_FLAG e_expr        { /* Syntax error */ $$ = NULL; NO_LABEL_EXPECTED(@label, "FLAG", $label->appendArgsLink($id->appendLink($e_expr))); }
;
dir_repeat:
      dir_rept
      statements EOL
      dir_endm                      { $$ = $dir_rept->createBranch($statements->appendLink($dir_endm)); }
    | dir_rept
      dir_endm                      { $$ = $dir_rept->createBranch($dir_endm); }
;
dir_rept:
      D_REPT e_expr COMMENT         { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_REPT, $e_expr); }
    | D_REPT e_expr                 { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_REPT, $e_expr); }
    | D_REPT COMMENT                { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_REPT); }
    | label D_REPT e_expr COMMENT   { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_REPT, $e_expr)); }
    | label D_REPT e_expr           { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_REPT, $e_expr)); }
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
      id D_MACRO                    { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_MACRO, $id); }
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
      D_DB args_str                 { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_DB, $args_str); }
    | label D_DB args_str           { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_DB, $args_str)); }
    | D_DB                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DB, "DB"); }
    | label D_DB                    { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DB, "DB"); $label->completeDelete(); }
;
dir_include:
      INCLUDE                       {
                        $$ = new CompilerStatement (
                            LOC(@$),
                            ASM51_INCLUDE,
                            new CompilerExpr (
                                CompilerExpr::Value($INCLUDE.data, $INCLUDE.size)
                            )
                        );
                    }
    | label INCLUDE                 {
                        $$ = $label->appendLink (
                            new CompilerStatement (
                                LOC(@$),
                                ASM51_INCLUDE,
                                new CompilerExpr (
                                    CompilerExpr::Value ( $INCLUDE.data, $INCLUDE.size )
                                )
                            )
                        );
                    }
;
end_of_code:
      dir_end                       { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_END); }
    | label dir_end                 { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_END)); }
    | dir_end EOL                   { $$ = new CompilerStatement(LOC(@$), ASM51_DIR_END); }
    | label dir_end EOL             { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASM51_DIR_END)); }
;
dir_end:
      D_END
    | D_END COMMENT
    | D_END args COMMENT            { /* Syntax error */ NO_ARG_EXPECTED_D("END", $args, @args); }
    | D_END args                    { /* Syntax error */ NO_ARG_EXPECTED_D("END", $args, @args); }
;

/*
 * Assembler controls
 */
asm_ctrl:
      ctrl_cond             { $$ = $1; }    | ctrl_nodebug          { $$ = $1; }
    | ctrl_nosymbols        { $$ = $1; }    | ctrl_condonly         { $$ = $1; }
    | ctrl_notabs           { $$ = $1; }    | ctrl_nogen            { $$ = $1; }
    | ctrl_save             { $$ = $1; }    | ctrl_date             { $$ = $1; }
    | ctrl_noxref           { $$ = $1; }    | ctrl_list             { $$ = $1; }
    | ctrl_nolist           { $$ = $1; }    | ctrl_pagelength       { $$ = $1; }
    | ctrl_symbols          { $$ = $1; }    | ctrl_debug            { $$ = $1; }
    | ctrl_macro            { $$ = $1; }    | ctrl_nomacro          { $$ = $1; }
    | ctrl_pagewidth        { $$ = $1; }    | ctrl_title            { $$ = $1; }
    | ctrl_paging           { $$ = $1; }    | ctrl_eject            { $$ = $1; }
    | ctrl_mod51            { $$ = $1; }    | ctrl_nomod51          { $$ = $1; }
    | ctrl_philips          { $$ = $1; }    | ctrl_warning          { $$ = $1; }
    | ctrl_error            { $$ = $1; }    | ctrl_nobuiltin        { $$ = $1; }
    | ctrl_nopaging         { $$ = $1; }    | ctrl_xref             { $$ = $1; }
    | ctrl_gen              { $$ = $1; }    | ctrl_nocond           { $$ = $1; }
    | ctrl_genonly          { $$ = $1; }    | ctrl_restore          { $$ = $1; }
;
ctrl_cond:
      C_COND                        { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_COND); }
    | C_COND ctrl_arg               { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "COND"); }
;
ctrl_nodebug:
      C_NODEBUG                     { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_NODEBUG); }
    | C_NODEBUG ctrl_arg            { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "NODEBUG"); }
;
ctrl_nosymbols:
      C_NOSYMBOLS                   { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_NOSYMBOLS); }
    | C_NOSYMBOLS ctrl_arg          { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "NOSYMBOLS"); }
;
ctrl_condonly:
      C_CONDONLY                    { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_CONDONLY); }
    | C_CONDONLY ctrl_arg           { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "CONDONLY"); }
;
ctrl_notabs:
      C_NOTABS                      { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_NOTABS); }
    | C_NOTABS ctrl_arg             { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "NOTABS"); }
;
ctrl_nogen:
      C_NOGEN                       { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_NOGEN); }
    | C_NOGEN ctrl_arg              { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "NOGEN"); }
;
ctrl_save:
      C_SAVE                        { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_SAVE); }
    | C_SAVE ctrl_arg               { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "SAVE"); }
;
ctrl_date:
      C_DATE "(" string ")"         { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_DATE, $string); }
    | C_DATE "(" ")"                { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_DATE, "DATE"); }
    | C_DATE                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_DATE, "DATE"); }
;
ctrl_noxref:
      C_NOXREF                      { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_NOXREF); }
    | C_NOXREF ctrl_arg             { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "NOXREF"); }
;
ctrl_list:
      C_LIST                        { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_LIST); }
    | C_LIST ctrl_arg               { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "LIST"); }
;
ctrl_nolist:
      C_NOLIST                      { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_NOLIST); }
    | C_NOLIST ctrl_arg             { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "NOLIST"); }
;
ctrl_symbols:
      C_SYMBOLS                     { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_SYMBOLS); }
    | C_SYMBOLS ctrl_arg            { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "SYMBOLS"); }
;
ctrl_debug:
      C_DEBUG                       { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_DEBUG); }
    | C_DEBUG ctrl_arg              { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "DEBUG"); }
;
ctrl_macro:
      C_MACRO "(" number ")"        { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_MACRO, $number); }
    | C_MACRO "(" ")"               { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_MACRO, "MACRO"); }
    | C_MACRO                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_MACRO, "MACRO"); }
;
ctrl_nomacro:
      C_NOMACRO                     { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_NOMACRO); }
    | C_NOMACRO ctrl_arg            { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "NOMACRO"); }
;
ctrl_pagewidth:
      C_PAGEWIDTH "(" number ")"    { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_PAGEWIDTH, $number); }
    | C_PAGEWIDTH "(" ")"           { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_PAGEWIDTH, "PAGEWIDTH"); }
    | C_PAGEWIDTH                   { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_PAGEWIDTH, "PAGEWIDTH"); }
;
ctrl_pagelength:
      C_PAGELENGTH "(" number ")"   { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_PAGELENGTH, $number); }
    | C_PAGELENGTH "(" ")"          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_PAGELENGTH, "PAGELENGTH"); }
    | C_PAGELENGTH                  { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_PAGELENGTH, "PAGELENGTH"); }
;
ctrl_title:
      C_TITLE "(" string ")"        { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_TITLE, $string); }
    | C_TITLE "(" ")"               { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_TITLE, "TITLE"); }
    | C_TITLE                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_TITLE, "TITLE"); }
;
ctrl_paging:
      C_PAGING                      { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_PAGING); }
    | C_PAGING ctrl_arg             { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "PAGING"); }
;
ctrl_eject:
      C_EJECT                       { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_EJECT); }
    | C_EJECT ctrl_arg              { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "EJECT"); }
;
ctrl_mod51:
      C_MOD51                       { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_MOD51); }
    | C_MOD51 ctrl_arg              { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "MOD51"); }
;
ctrl_nomod51:
      C_NOMOD51                     { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_NOMOD51); }
    | C_NOMOD51 ctrl_arg            { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "NOMOD51"); }
;
ctrl_philips:
      C_PHILIPS                     { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_PHILIPS); }
    | C_PHILIPS ctrl_arg            { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "PHILIPS"); }
;
ctrl_warning:
      C_WARNING "(" string ")"      { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_WARNING, $string); }
    | C_WARNING "(" ")"             { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_WARNING, "WARNING"); }
    | C_WARNING                     { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_WARNING, "WARNING"); }
;
ctrl_error:
      C_ERROR "(" string ")"        { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_ERROR, $string); }
    | C_ERROR "(" ")"               { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_ERROR, "ERROR"); }
    | C_ERROR                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_C(@C_ERROR, "ERROR"); }
;
ctrl_nobuiltin:
      C_NOBUILTIN                   { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_NOBUILTIN); }
    | C_NOBUILTIN ctrl_arg          { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "NOBUILTIN"); }
;
ctrl_nopaging:
      C_NOPAGING                    { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_NOPAGING); }
    | C_NOPAGING ctrl_arg           { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "NOPAGING"); }
;
ctrl_xref:
      C_XREF                        { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_XREF); }
    | C_XREF ctrl_arg               { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "XREF"); }
;
ctrl_gen:
      C_GEN                         { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_GEN); }
    | C_GEN ctrl_arg                { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "GEN"); }
;
ctrl_nocond:
      C_NOCOND                      { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_NOCOND); }
    | C_NOCOND ctrl_arg             { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "NOCOND"); }
;
ctrl_genonly:
      C_GENONLY                     { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_GENONLY); }
    | C_GENONLY ctrl_arg            { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "GENONLY"); }
;
ctrl_restore:
      C_RESTORE                     { $$ = new CompilerStatement(LOC(@$), ASM51_CNT_RESTORE); }
    | C_RESTORE ctrl_arg            { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_C(@ctrl_arg, "RESTORE"); }
;
ctrl_arg:       // Intended for error handling only!
      "(" string ")"                { $2->completeDelete(); }
    | "(" number ")"                { $2->completeDelete(); }
    | "(" id ")"                    { $2->completeDelete(); }
;
/*
 * Instructions
 */
instruction:
      ins_acall     { $$ = $1; }    | ins_add       { $$ = $1; }
    | ins_addc      { $$ = $1; }    | ins_ajmp      { $$ = $1; }
    | ins_anl       { $$ = $1; }    | ins_cjne      { $$ = $1; }
    | ins_clr       { $$ = $1; }    | ins_cpl       { $$ = $1; }
    | ins_da        { $$ = $1; }    | ins_dec       { $$ = $1; }
    | ins_div       { $$ = $1; }    | ins_djnz      { $$ = $1; }
    | ins_inc       { $$ = $1; }    | ins_jb        { $$ = $1; }
    | ins_jbc       { $$ = $1; }    | ins_jc        { $$ = $1; }
    | ins_jmp       { $$ = $1; }    | ins_jnb       { $$ = $1; }
    | ins_jnc       { $$ = $1; }    | ins_jnz       { $$ = $1; }
    | ins_jz        { $$ = $1; }    | ins_lcall     { $$ = $1; }
    | ins_ljmp      { $$ = $1; }    | ins_mov       { $$ = $1; }
    | ins_movc      { $$ = $1; }    | ins_movx      { $$ = $1; }
    | ins_mul       { $$ = $1; }    | ins_nop       { $$ = $1; }
    | ins_orl       { $$ = $1; }    | ins_pop       { $$ = $1; }
    | ins_push      { $$ = $1; }    | ins_ret       { $$ = $1; }
    | ins_reti      { $$ = $1; }    | ins_rl        { $$ = $1; }
    | ins_rlc       { $$ = $1; }    | ins_rr        { $$ = $1; }
    | ins_rrc       { $$ = $1; }    | ins_setb      { $$ = $1; }
    | ins_sjmp      { $$ = $1; }    | ins_subb      { $$ = $1; }
    | ins_swap      { $$ = $1; }    | ins_xch       { $$ = $1; }
    | ins_xchd      { $$ = $1; }    | ins_xrl       { $$ = $1; }
;
ins_acall:
      I_ACALL e_expr                { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ACALL, $e_expr); }
    | I_ACALL opr "," oprs          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ACALL", 1); }
    | I_ACALL                       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ACALL", 1); }
    | I_ACALL "#" e_expr            { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("ACALL", "<code>", @1, $e_expr); }
;
ins_add:
      I_ADD REG_A "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADD__A_IMM, $e_expr); }
    | I_ADD REG_A "," e_expr        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADD__A_DIR, $e_expr); }
    | I_ADD REG_A "," IND_R0        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADD__A_IR0); }
    | I_ADD REG_A "," IND_R1        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADD__A_IR1); }
    | I_ADD REG_A "," REG_0         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADD__A_R0); }
    | I_ADD REG_A "," REG_1         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADD__A_R1); }
    | I_ADD REG_A "," REG_2         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADD__A_R2); }
    | I_ADD REG_A "," REG_3         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADD__A_R3); }
    | I_ADD REG_A "," REG_4         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADD__A_R4); }
    | I_ADD REG_A "," REG_5         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADD__A_R5); }
    | I_ADD REG_A "," REG_6         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADD__A_R6); }
    | I_ADD REG_A "," REG_7         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADD__A_R7); }
    | I_ADD opr "," opr "," oprs    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ADD", 2); }
    | I_ADD opr                     { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ADD", 2); }
    | I_ADD                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ADD", 2); }
    | I_ADD REG_A oprs              { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL); }
    | I_ADD expr "," oprs           { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("ADD", "A", @1, $expr); }
    | I_ADD "#" expr "," oprs       { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("ADD", "A", @1, $expr); }
;
ins_addc:
      I_ADDC REG_A "," "#" e_expr   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADDC__A_IMM, $e_expr); }
    | I_ADDC REG_A "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADDC__A_DIR, $e_expr); }
    | I_ADDC REG_A "," IND_R0       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADDC__A_IR0); }
    | I_ADDC REG_A "," IND_R1       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADDC__A_IR1); }
    | I_ADDC REG_A "," REG_0        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADDC__A_R0); }
    | I_ADDC REG_A "," REG_1        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADDC__A_R1); }
    | I_ADDC REG_A "," REG_2        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADDC__A_R2); }
    | I_ADDC REG_A "," REG_3        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADDC__A_R3); }
    | I_ADDC REG_A "," REG_4        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADDC__A_R4); }
    | I_ADDC REG_A "," REG_5        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADDC__A_R5); }
    | I_ADDC REG_A "," REG_6        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADDC__A_R6); }
    | I_ADDC REG_A "," REG_7        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ADDC__A_R7); }
    | I_ADDC opr "," opr "," oprs   { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ADDC", 2); }
    | I_ADDC opr                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ADDC", 2); }
    | I_ADDC                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ADDC", 2); }
    | I_ADDC REG_A oprs             { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL); }
    | I_ADDC expr "," oprs          { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("ADDC", "A", @1, $expr); }
    | I_ADDC "#" expr "," oprs      { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("ADDC", "A", @1, $expr); }
;
ins_ajmp:
      I_AJMP e_expr                 { $$ = new CompilerStatement(LOC(@$), ASM51_INS_AJMP__DIR, $e_expr); }
    | I_AJMP opr "," oprs           { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "AJMP", 1); }
    | I_AJMP                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "AJMP", 1); }
    | I_AJMP "#" e_expr             { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("AJMP", "<code>", @1, $e_expr); }
;
ins_anl:
      I_ANL expr "," REG_A          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__DIR_A, $expr); }
    | I_ANL expr "," "#" e_expr     { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__DIR_IMM, $expr->appendLink($e_expr)); }
    | I_ANL REG_A "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__A_IMM, $e_expr); }
    | I_ANL REG_A "," e_expr        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__A_DIR, $e_expr); }
    | I_ANL REG_A "," IND_R0        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__A_IR0); }
    | I_ANL REG_A "," IND_R1        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__A_IR1); }
    | I_ANL REG_A "," REG_0         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__A_R0); }
    | I_ANL REG_A "," REG_1         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__A_R1); }
    | I_ANL REG_A "," REG_2         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__A_R2); }
    | I_ANL REG_A "," REG_3         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__A_R3); }
    | I_ANL REG_A "," REG_4         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__A_R4); }
    | I_ANL REG_A "," REG_5         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__A_R5); }
    | I_ANL REG_A "," REG_6         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__A_R6); }
    | I_ANL REG_A "," REG_7         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__A_R7); }
    | I_ANL BIT_C "," e_expr        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__C_DIR, $e_expr); }
    | I_ANL BIT_C "," "/" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ANL__C_SL_DIR, $e_expr); }
    | I_ANL opr "," opr "," oprs    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ANL", 2); }
    | I_ANL opr                     { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ANL", 2); }
    | I_ANL                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ANL", 2); }
    | I_ANL REG_A oprs              { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL); }
    | I_ANL "#" expr "," oprs               { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("ANL", "A | C | <idata>", @1, $expr); }
;
ins_cjne:
      I_CJNE REG_A  ","     e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CJNE__A_DIR_DIR, $4->appendLink($6)); }
    | I_CJNE REG_A  "," "#" e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CJNE__A_IMM_DIR, $5->appendLink($7)); }
    | I_CJNE IND_R0 "," "#" e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CJNE__IR0_IMM_DIR, $5->appendLink($7)); }
    | I_CJNE IND_R1 "," "#" e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CJNE__IR1_IMM_DIR, $5->appendLink($7)); }
    | I_CJNE REG_0  "," "#" e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CJNE__R0_IMM_DIR, $5->appendLink($7)); }
    | I_CJNE REG_1  "," "#" e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CJNE__R1_IMM_DIR, $5->appendLink($7)); }
    | I_CJNE REG_2  "," "#" e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CJNE__R2_IMM_DIR, $5->appendLink($7)); }
    | I_CJNE REG_3  "," "#" e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CJNE__R3_IMM_DIR, $5->appendLink($7)); }
    | I_CJNE REG_4  "," "#" e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CJNE__R4_IMM_DIR, $5->appendLink($7)); }
    | I_CJNE REG_5  "," "#" e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CJNE__R5_IMM_DIR, $5->appendLink($7)); }
    | I_CJNE REG_6  "," "#" e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CJNE__R6_IMM_DIR, $5->appendLink($7)); }
    | I_CJNE REG_7  "," "#" e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CJNE__R7_IMM_DIR, $5->appendLink($7)); }
    | I_CJNE opr "," opr "," opr "," oprs           { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "CJNE", 3); }
    | I_CJNE opr "," opr                            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "CJNE", 3); }
    | I_CJNE opr                                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "CJNE", 3); }
    | I_CJNE                                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "CJNE", 3); }
    | I_CJNE expr "," oprs                          { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("CJNE", "A | @R0 | @R1 | R0 | R1 | R2 | R3 | R4 | R5 | R6 | R7", @1, $expr); }
    | I_CJNE "#" expr "," oprs                      { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("CJNE", "A | @R0 | @R1 | R0 | R1 | R2 | R3 | R4 | R5 | R6 | R7", @1, $expr); }
;
ins_clr:
      I_CLR expr                    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CLR__DIR, $expr); }
    | I_CLR BIT_C                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CLR__C); }
    | I_CLR REG_A                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CLR__A); }
    | I_CLR opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "CLR", 1); }
    | I_CLR                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "CLR", 1); }
;
ins_cpl:
      I_CPL expr                    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CPL__DIR, $expr); }
    | I_CPL BIT_C                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CPL__C); }
    | I_CPL REG_A                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_CPL__A); }
    | I_CPL opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "CPL", 1); }
    | I_CPL                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "CPL", 1); }
;
ins_da:
      I_DA REG_A                    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DA__A); }
    | I_DA opr "," oprs             { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "DA", 1); }
    | I_DA                          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "DA", 1); }
    | I_DA expr                     { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("DA", "A", @1, $expr); }
    | I_DA "#" e_expr               { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("DA", "A", @1, $e_expr); }
;
ins_dec:
      I_DEC expr                    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__DIR, $expr); }
    | I_DEC const_c                 { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__DIR, $const_c); }
    | I_DEC REG_A                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__A); }
    | I_DEC IND_R0                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__IR0); }
    | I_DEC IND_R1                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__IR1); }
    | I_DEC REG_0                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__R0); }
    | I_DEC REG_1                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__R1); }
    | I_DEC REG_2                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__R2); }
    | I_DEC REG_3                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__R3); }
    | I_DEC REG_4                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__R4); }
    | I_DEC REG_5                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__R5); }
    | I_DEC REG_6                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__R6); }
    | I_DEC REG_7                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DEC__R7); }
    | I_DEC opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "DEC", 1); }
    | I_DEC                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "DEC", 1); }
;
ins_div:
      I_DIV REG_AB                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DIV__AB); }
    | I_DIV opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "DIV", 1); }
    | I_DIV                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "DIV", 1); }
    | I_DIV e_expr                  { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("DIV", "AB", @1, $e_expr); }
    | I_DIV "#" e_expr              { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("DIV", "AB", @1, $e_expr); }
;
ins_djnz:
      I_DJNZ e_expr "," e_expr      { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DJNZ__DIR_DIR, $2->appendLink($4)); }
    | I_DJNZ REG_0 "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DJNZ__R0_DIR, $e_expr); }
    | I_DJNZ REG_1 "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DJNZ__R1_DIR, $e_expr); }
    | I_DJNZ REG_2 "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DJNZ__R2_DIR, $e_expr); }
    | I_DJNZ REG_3 "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DJNZ__R3_DIR, $e_expr); }
    | I_DJNZ REG_4 "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DJNZ__R4_DIR, $e_expr); }
    | I_DJNZ REG_5 "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DJNZ__R5_DIR, $e_expr); }
    | I_DJNZ REG_6 "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DJNZ__R6_DIR, $e_expr); }
    | I_DJNZ REG_7 "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_DJNZ__R7_DIR, $e_expr); }
    | I_DJNZ opr "," opr "," oprs   { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "DJNZ", 2); }
    | I_DJNZ opr                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "DJNZ", 2); }
    | I_DJNZ                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "DJNZ", 2); }
;
ins_inc:
      I_INC REG_A                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__A); }
    | I_INC expr                    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__DIR, $expr); }
    | I_INC const_c                 { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__DIR, $const_c); }
    | I_INC IND_R0                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__IR0); }
    | I_INC IND_R1                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__IR1); }
    | I_INC REG_0                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__R0); }
    | I_INC REG_1                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__R1); }
    | I_INC REG_2                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__R2); }
    | I_INC REG_3                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__R3); }
    | I_INC REG_4                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__R4); }
    | I_INC REG_5                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__R5); }
    | I_INC REG_6                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__R6); }
    | I_INC REG_7                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__R7); }
    | I_INC REG_DPTR                { $$ = new CompilerStatement(LOC(@$), ASM51_INS_INC__DPTR); }
    | I_INC opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "INC", 1); }
    | I_INC                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "INC", 1); }
;
ins_jb:
      I_JB e_expr "," e_expr        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_JB__DIR_DIR, $2->appendLink($4)); }
    | I_JB opr "," opr "," oprs     { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JB", 2); }
    | I_JB opr                      { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JB", 2); }
    | I_JB                          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JB", 2); }
;
ins_jbc:
      I_JBC e_expr "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_JBC__DIR_DIR, $2->appendLink($4)); }
    | I_JBC opr "," opr "," oprs    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JBC", 2); }
    | I_JBC opr                     { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JBC", 2); }
    | I_JBC                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JBC", 2); }
;
ins_jc:
      I_JC e_expr                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_JC__DIR, $e_expr); }
    | I_JC opr "," oprs             { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JC", 1); }
    | I_JC                          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JC", 1); }
    | I_JC "#" e_expr               { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("JC", "<code>", @1, $e_expr); }
;
ins_jmp:
      I_JMP IND_A_DPTR              { $$ = new CompilerStatement(LOC(@$), ASM51_INS_JMP__IA_DPTR); }
    | I_JMP e_expr                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_JMP__DIR, $e_expr); }
    | I_JMP opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JMP", 1); }
    | I_JMP                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JMP", 1); }
    | I_JMP "#" e_expr              { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("JMP", "@A+DPTR | <code>", @1, $e_expr); }
;
ins_jnb:
      I_JNB e_expr                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_JNB__DIR, $e_expr); }
    | I_JNB opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JNB", 1); }
    | I_JNB                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JNB", 1); }
    | I_JNB "#" e_expr              { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("JNB", "<code>", @1, $e_expr); }
;
ins_jnc:
      I_JNC e_expr                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_JNC__DIR, $e_expr); }
    | I_JNC opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JNC", 1); }
    | I_JNC                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JNC", 1); }
    | I_JNC "#" e_expr              { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("JNC", "<code>", @1, $e_expr); }
;
ins_jnz:
      I_JNZ e_expr                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_JNZ__DIR, $e_expr); }
    | I_JNZ opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JNZ", 1); }
    | I_JNZ                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JNZ", 1); }
    | I_JNZ "#" e_expr              { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("JNZ", "<code>", @1, $e_expr); }
;
ins_jz:
      I_JZ e_expr                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_JZ__DIR, $e_expr); }
    | I_JZ opr "," oprs             { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JZ", 1); }
    | I_JZ                          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "JZ", 1); }
    | I_JZ "#" e_expr               { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("JZ", "<code>", @1, $e_expr); }
;
ins_lcall:
      I_LCALL e_expr                { $$ = new CompilerStatement(LOC(@$), ASM51_INS_LCALL__DIR, $e_expr); }
    | I_LCALL opr "," oprs          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "LCALL", 1); }
    | I_LCALL                       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "LCALL", 1); }
    | I_LCALL "#" e_expr            { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("LCALL", "<code>", @1, $e_expr); }
;
ins_ljmp:
      I_LJMP e_expr                 { $$ = new CompilerStatement(LOC(@$), ASM51_INS_LJMP__DIR, $e_expr); }
    | I_LJMP opr "," oprs           { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "LJMP", 1); }
    | I_LJMP                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "LJMP", 1); }
    | I_LJMP "#" e_expr             { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("LJMP", "<code>", @1, $e_expr); }
;
ins_mov:
      I_MOV expr "," expr           { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_DIR, $2->appendLink($4)); }
    | I_MOV expr "," "#" e_expr     { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_IMM, $expr->appendLink($e_expr)); }
    | I_MOV expr "," IND_R0         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_IR0, $expr); }
    | I_MOV expr "," IND_R1         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_IR1, $expr); }
    | I_MOV expr "," REG_0          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_R0, $expr); }
    | I_MOV expr "," REG_1          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_R1, $expr); }
    | I_MOV expr "," REG_2          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_R2, $expr); }
    | I_MOV expr "," REG_3          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_R3, $expr); }
    | I_MOV expr "," REG_4          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_R4, $expr); }
    | I_MOV expr "," REG_5          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_R5, $expr); }
    | I_MOV expr "," REG_6          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_R6, $expr); }
    | I_MOV expr "," REG_7          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_R7, $expr); }
    | I_MOV expr "," BIT_C          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_C, $expr); }
    | I_MOV expr "," REG_A          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DIR_A, $expr); }
    | I_MOV IND_R0 "," "#" e_expr   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__IR0_IMM, $e_expr); }
    | I_MOV IND_R1 "," "#" e_expr   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__IR1_IMM, $e_expr); }
    | I_MOV IND_R0 "," expr         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__IR0_DIR, $expr); }
    | I_MOV IND_R1 "," expr         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__IR1_DIR, $expr); }
    | I_MOV IND_R0 "," REG_A        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__IR0_A); }
    | I_MOV IND_R1 "," REG_A        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__IR1_A); }
    | I_MOV REG_DPTR "," "#" e_expr { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__DPTR_IMM, $e_expr); }
    | I_MOV REG_0 "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R0_IMM, $e_expr); }
    | I_MOV REG_1 "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R1_IMM, $e_expr); }
    | I_MOV REG_2 "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R2_IMM, $e_expr); }
    | I_MOV REG_3 "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R3_IMM, $e_expr); }
    | I_MOV REG_4 "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R4_IMM, $e_expr); }
    | I_MOV REG_5 "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R5_IMM, $e_expr); }
    | I_MOV REG_6 "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R6_IMM, $e_expr); }
    | I_MOV REG_7 "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R7_IMM, $e_expr); }
    | I_MOV REG_0 "," expr          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R0_DIR, $expr); }
    | I_MOV REG_1 "," expr          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R1_DIR, $expr); }
    | I_MOV REG_2 "," expr          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R2_DIR, $expr); }
    | I_MOV REG_3 "," expr          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R3_DIR, $expr); }
    | I_MOV REG_4 "," expr          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R4_DIR, $expr); }
    | I_MOV REG_5 "," expr          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R5_DIR, $expr); }
    | I_MOV REG_6 "," expr          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R6_DIR, $expr); }
    | I_MOV REG_7 "," expr          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R7_DIR, $expr); }
    | I_MOV REG_0 "," REG_A         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R0_A); }
    | I_MOV REG_1 "," REG_A         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R1_A); }
    | I_MOV REG_2 "," REG_A         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R2_A); }
    | I_MOV REG_3 "," REG_A         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R3_A); }
    | I_MOV REG_4 "," REG_A         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R4_A); }
    | I_MOV REG_5 "," REG_A         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R5_A); }
    | I_MOV REG_6 "," REG_A         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R6_A); }
    | I_MOV REG_7 "," REG_A         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__R7_A); }
    | I_MOV REG_A "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__A_IMM, $e_expr); }
    | I_MOV REG_A "," IND_R0        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__A_IR0); }
    | I_MOV REG_A "," IND_R1        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__A_IR1); }
    | I_MOV REG_A "," REG_0         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__A_R0); }
    | I_MOV REG_A "," REG_1         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__A_R1); }
    | I_MOV REG_A "," REG_2         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__A_R2); }
    | I_MOV REG_A "," REG_3         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__A_R3); }
    | I_MOV REG_A "," REG_4         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__A_R4); }
    | I_MOV REG_A "," REG_5         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__A_R5); }
    | I_MOV REG_A "," REG_6         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__A_R6); }
    | I_MOV REG_A "," REG_7         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__A_R7); }
    | I_MOV REG_A "," e_expr        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__A_DIR, $e_expr); }
    | I_MOV BIT_C "," e_expr        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOV__C_DIR, $e_expr); }
    | I_MOV opr "," opr "," oprs    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "MOV", 2); }
    | I_MOV opr                     { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "MOV", 2); }
    | I_MOV                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "MOV", 2); }
;
ins_movc:
      I_MOVC REG_A "," IND_A_DPTR   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOVC__A_IA_DPTR); }
    | I_MOVC REG_A "," IND_A_PC     { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOVC__A_IA_PC); }
    | I_MOVC opr "," opr "," oprs   { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "MOVC", 2); }
    | I_MOVC opr                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "MOVC", 2); }
    | I_MOVC                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "MOVC", 2); }
;
ins_movx:
      I_MOVX IND_DPTR "," REG_A     { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOVX__IDPTR_A); }
    | I_MOVX IND_R0 "," REG_A       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOVX__IR0_A); }
    | I_MOVX IND_R1 "," REG_A       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOVX__IR1_A); }
    | I_MOVX REG_A "," IND_DPTR     { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOVX__A_IDPTR); }
    | I_MOVX REG_A "," IND_R0       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOVX__A_IR0); }
    | I_MOVX REG_A "," IND_R1       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MOVX__A_IR1); }
    | I_MOVX opr "," opr "," oprs   { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "MOVX", 2); }
    | I_MOVX opr                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "MOVX", 2); }
    | I_MOVX                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "MOVX", 2); }
;
ins_mul:
      I_MUL REG_AB                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_MUL__AB); }
    | I_MUL opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "MUL", 1); }
    | I_MUL                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "MUL", 1); }
    | I_MUL e_expr                  { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("MUL", "AB", @1, $e_expr); }
    | I_MUL "#" e_expr              { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("MUL", "AB", @1, $e_expr); }
;
ins_nop:
      I_NOP                         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_NOP); }
    | I_NOP oprs                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "NOP", 0); }
;
ins_orl:
      I_ORL expr "," REG_A          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__DIR_A, $expr); }
    | I_ORL expr "," "#" e_expr     { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__DIR_IMM, $expr->appendLink($e_expr)); }
    | I_ORL REG_A "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__A_IMM, $e_expr); }
    | I_ORL REG_A "," e_expr        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__A_DIR, $e_expr); }
    | I_ORL REG_A "," IND_R0        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__A_IR0); }
    | I_ORL REG_A "," IND_R1        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__A_IR1); }
    | I_ORL REG_A "," REG_0         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__A_R0); }
    | I_ORL REG_A "," REG_1         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__A_R1); }
    | I_ORL REG_A "," REG_2         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__A_R2); }
    | I_ORL REG_A "," REG_3         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__A_R3); }
    | I_ORL REG_A "," REG_4         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__A_R4); }
    | I_ORL REG_A "," REG_5         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__A_R5); }
    | I_ORL REG_A "," REG_6         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__A_R6); }
    | I_ORL REG_A "," REG_7         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__A_R7); }
    | I_ORL BIT_C "," e_expr        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__C_DIR, $e_expr); }
    | I_ORL BIT_C "," "/" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_ORL__C_SL_DIR, $e_expr); }
    | I_ORL opr "," opr "," oprs    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ORL", 2); }
    | I_ORL opr                     { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ORL", 2); }
    | I_ORL                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ORL", 2); }
    | I_ORL REG_A oprs              { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL); }
    | I_ORL "#" expr "," oprs       { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("ORL", "A | C | <idata>", @1, $expr); }
;
ins_pop:
      I_POP e_expr                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_POP__DIR, $e_expr); }
    | I_POP opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "POP", 1); }
    | I_POP                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "POP", 1); }
    | I_POP "#" e_expr              { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("POP", "<idata>", @1, $e_expr); }
;
ins_push:
      I_PUSH e_expr                 { $$ = new CompilerStatement(LOC(@$), ASM51_INS_PUSH__DIR, $e_expr); }
    | I_PUSH opr "," oprs           { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "PUSH", 1); }
    | I_PUSH                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "PUSH", 1); }
    | I_PUSH "#" e_expr             { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("PUSH", "<idata>", @1, $e_expr); }
;
ins_ret:
      I_RET                         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_RET); }
    | I_RET oprs                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RET", 0); }
;
ins_reti:
      I_RETI                        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_RETI); }
    | I_RETI oprs                   { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RETI", 0); }
;
ins_rl:
      I_RL REG_A                    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_RL__A); }
    | I_RL opr "," oprs             { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RL", 1); }
    | I_RL                          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RL", 1); }
    | I_RL expr                     { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("RL", "A", @1, $expr); }
    | I_RL "#" e_expr               { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("RL", "A", @1, $e_expr); }
;
ins_rlc:
      I_RLC REG_A                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_RLC__A); }
    | I_RLC opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RLC", 1); }
    | I_RLC                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RLC", 1); }
    | I_RLC expr                    { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("RLC", "A", @1, $expr); }
    | I_RLC "#" e_expr              { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("RLC", "A", @1, $e_expr); }
;
ins_rr:
      I_RR REG_A                    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_RR__A); }
    | I_RR opr "," oprs             { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RR", 1); }
    | I_RR                          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RR", 1); }
    | I_RR expr                     { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("RR", "A", @1, $expr); }
    | I_RR "#" e_expr               { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("RR", "A", @1, $e_expr); }
;
ins_rrc:
      I_RRC REG_A                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_RRC__A); }
    | I_RRC opr "," oprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RRC", 1); }
    | I_RRC                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RRC", 1); }
    | I_RRC expr                    { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("RRC", "A", @1, $expr); }
    | I_RRC "#" e_expr              { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("RRC", "A", @1, $e_expr); }
;
ins_setb:
      I_SETB expr                   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SETB__DIR, $expr); }
    | I_SETB const_a                { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SETB__DIR, $const_a); }
    | I_SETB BIT_C                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SETB__C); }
    | I_SETB opr "," oprs           { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SETB", 1); }
    | I_SETB                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SETB", 1); }
;
ins_sjmp:
      I_SJMP e_expr                 { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SJMP__DIR, $e_expr); }
    | I_SJMP opr "," oprs           { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SJMP", 1); }
    | I_SJMP                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SJMP", 1); }
    | I_SJMP "#" e_expr             { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("SJMP", "<code>", @1, $e_expr); }
;
ins_subb:
      I_SUBB REG_A "," "#" e_expr   { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SUBB__A_IMM, $e_expr); }
    | I_SUBB REG_A "," e_expr       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SUBB__A_DIR, $e_expr); }
    | I_SUBB REG_A "," IND_R0       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SUBB__A_IR0); }
    | I_SUBB REG_A "," IND_R1       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SUBB__A_IR1); }
    | I_SUBB REG_A "," REG_0        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SUBB__A_R0); }
    | I_SUBB REG_A "," REG_1        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SUBB__A_R1); }
    | I_SUBB REG_A "," REG_2        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SUBB__A_R2); }
    | I_SUBB REG_A "," REG_3        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SUBB__A_R3); }
    | I_SUBB REG_A "," REG_4        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SUBB__A_R4); }
    | I_SUBB REG_A "," REG_5        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SUBB__A_R5); }
    | I_SUBB REG_A "," REG_6        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SUBB__A_R6); }
    | I_SUBB REG_A "," REG_7        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SUBB__A_R7); }
    | I_SUBB opr "," opr "," oprs   { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SUBB", 2); }
    | I_SUBB opr                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SUBB", 2); }
    | I_SUBB                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SUBB", 2); }
    | I_SUBB REG_A oprs             { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL); }
    | I_SUBB expr "," oprs          { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("SUBB", "A", @1, $expr); }
    | I_SUBB "#" expr "," oprs      { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("SUBB", "A", @1, $expr); }
;
ins_swap:
      I_SWAP REG_A                  { $$ = new CompilerStatement(LOC(@$), ASM51_INS_SWAP__A); }
    | I_SWAP opr "," oprs           { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SWAP", 1); }
    | I_SWAP                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SWAP", 1); }
    | I_SWAP expr                   { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("SWAP", "A", @1, $expr); }
    | I_SWAP "#" e_expr             { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("SWAP", "A", @1, $e_expr); }
;
ins_xch:
      I_XCH REG_A "," IND_R0        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCH__A_IR0); }
    | I_XCH REG_A "," IND_R1        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCH__A_IR1); }
    | I_XCH REG_A "," REG_0         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCH__A_R0); }
    | I_XCH REG_A "," REG_1         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCH__A_R1); }
    | I_XCH REG_A "," REG_2         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCH__A_R2); }
    | I_XCH REG_A "," REG_3         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCH__A_R3); }
    | I_XCH REG_A "," REG_4         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCH__A_R4); }
    | I_XCH REG_A "," REG_5         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCH__A_R5); }
    | I_XCH REG_A "," REG_6         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCH__A_R6); }
    | I_XCH REG_A "," REG_7         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCH__A_R7); }
    | I_XCH REG_A "," e_expr        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCH__A_DIR, $e_expr); }
    | I_XCH opr "," opr "," oprs    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "XCH", 2); }
    | I_XCH opr                     { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "XCH", 2); }
    | I_XCH                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "XCH", 2); }
    | I_XCH REG_A oprs              { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL); }
    | I_XCH expr "," oprs           { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("XCH", "A", @1, $expr); }
    | I_XCH "#" expr "," oprs       { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("XCH", "A", @1, $expr); }
;
ins_xchd:
      I_XCHD REG_A "," IND_R0       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCHD__A_IR0); }
    | I_XCHD REG_A "," IND_R1       { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XCHD__A_IR1); }
    | I_XCHD opr "," opr "," oprs   { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "XCHD", 2); }
    | I_XCHD opr                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "XCHD", 2); }
    | I_XCHD                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "XCHD", 2); }
    | I_XCHD REG_A oprs             { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL); }
    | I_XCHD expr "," oprs          { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("XCHD", "A", @1, $expr); }
    | I_XCHD "#" expr "," oprs      { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("XCHD", "A", @1, $expr); }
;
ins_xrl:
      I_XRL expr "," REG_A          { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__DIR_A, $expr); }
    | I_XRL expr "," "#" e_expr     { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__DIR_IMM, $expr->appendLink($e_expr)); }
    | I_XRL REG_A "," "#" e_expr    { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__A_IMM, $e_expr); }
    | I_XRL REG_A "," e_expr        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__A_DIR, $e_expr); }
    | I_XRL REG_A "," IND_R0        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__A_IR0); }
    | I_XRL REG_A "," IND_R1        { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__A_IR1); }
    | I_XRL REG_A "," REG_0         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__A_R0); }
    | I_XRL REG_A "," REG_1         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__A_R1); }
    | I_XRL REG_A "," REG_2         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__A_R2); }
    | I_XRL REG_A "," REG_3         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__A_R3); }
    | I_XRL REG_A "," REG_4         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__A_R4); }
    | I_XRL REG_A "," REG_5         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__A_R5); }
    | I_XRL REG_A "," REG_6         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__A_R6); }
    | I_XRL REG_A "," REG_7         { $$ = new CompilerStatement(LOC(@$), ASM51_INS_XRL__A_R7); }
    | I_XRL opr "," opr "," oprs    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "XRL", 2); }
    | I_XRL opr                     { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "XRL", 2); }
    | I_XRL                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "XRL", 2); }
    | I_XRL "#" e_expr "," oprs     { /* Syntax Error */ $$ = NULL; REQ_FIRST_OPR("XRL", "A, <idata>", @1, $e_expr); }
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison
inline int mcs51parser_error(YYLTYPE * yylloc, yyscan_t, CompilerParserInterface * compiler, const char * errorInfo) {
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
                case 'C':
                    errStr = QObject::tr("unexpected assembler control $").toStdString();
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
