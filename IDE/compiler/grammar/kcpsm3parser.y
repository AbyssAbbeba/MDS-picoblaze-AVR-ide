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
%name-prefix "kcpsm3parser_"
// Generate a parser header file containing macro definitions, etc.
%defines "kcpsm3parser.h"
// Generate a pure, reentrant parser.
%define api.pure
// Generate the code processing the locations.
%locations
// Specify the programming language for the generated parser.
%language "C"
// Write an extra output file containing verbose descriptions of the parser states.
%verbose
// Expect exactly <n> shift/reduce conflicts in this grammar
%expect 336
// Expect exactly <n> reduce/reduce conflicts in this grammar
%expect-rr 0
/* Type of parser tables within the LR family, in this case we use LALR (Look-Ahead LR parser) */
%define lr.type lalr
/* Bison declaration to request verbose, specific error message strings when yyerror is called */
%error-verbose
/* Enable LAC (Lookahead Correction) to improve syntax error handling. */
%define parse.lac full

// Symbol semantic value
%union
{
    int number;                 //
    char * string;              //
    CompilerExpr * expr;        //
    CompilerStatement * stmt;   //
    struct
    {
        unsigned char * data;   //
        int size;               //
    } array;                    //
};

/*
 * Compiler interface for the syntax analyzer; we need to have some things declared
 * before we can declare other things, and this interface is quite essential here.
 */
%code requires
{
    #include "CompilerParserInterface.h"
}

%{
    #include <QObject>              // For i18n, nothing else.
    #include <cstdlib>              // We need free() here.
    #include <iostream>             // This is required by Bison.
    using namespace std;            // This is required by Bison as well.
    using namespace StatementTypes; // This NS is heavily used here.

    // Declaration of the lexer prototypes and other things required by Bison
    #include "kcpsm3lexer.h"

    // Name of the lexer function required by Bison
    #define kcpsm3parser_lex kcpsm3lexer_lex

    // Make a few things a little easier...
    #define LOC(location) \
        ( compiler->toSourceLocation(location) )

    // Error messages
    #define N_OPERANDS_EXPECTED(location, instruction, number) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("invalid number of operands, instruction %1 takes %2 operand(s)").arg(instruction).arg(number).toStdString() );
    #define NN_OPERANDS_EXPECTED(location, instruction, number, number2) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("invalid number of operands, instruction %1 takes %2 or %3 operands").arg(instruction).arg(number).arg(number2).toStdString() );
    #define NO_LABEL_EXPECTED(location, directive, statement) \
        CompilerStatement::completeDelete(statement); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("cannot declare a label before %1 directive").arg(directive).toStdString() );
    #define ARG_REQUIRED_D(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 requires a single argument").arg(directive).toStdString() );
    #define NO_ARG_EXPECTED_D(directive, arguments, location) \
        arguments->completeDelete(); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 takes no arguments").arg(directive).toStdString() );
    #define DECL_ID_EXPECTED(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 requires an identifier for the symbol (or macro) which it defines").arg(directive).toStdString() );
    #define MISSING_AT_OPERATOR(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 requires `AT' operator before the start address").arg(directive).toStdString() );
    #define MISSIGN_COMMA(location, expression) \
        CompilerExpr::completeDelete(expression); \
        location.first_line = location.last_line; \
        location.first_column = location.last_column; \
        location.last_column++; \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("comma (`,') expected between operands").toStdString() );

    // Declaration of the error reporting function used by Bison
    inline int kcpsm3parser_error ( YYLTYPE * yylloc,
                                    yyscan_t yyscanner,
                                    CompilerParserInterface * compiler,
                                    const char * errorInfo );
%}

// Declare an additional yyparse parameters
%parse-param { yyscan_t yyscanner }
%parse-param { CompilerParserInterface * compiler }

// Declare an additional yylex parameters
%lex-param   { yyscan_t yyscanner }

// Declare that the code which must be invoked before parsing actually begins
%initial-action
{
    @$.first_column = @$.last_column = 1;
    @$.first_line   = @$.last_line   = 1;
};

/*
 * DECLARATION OF THE GRAMMAR APLHABET - TERMINAL SYMBOLS
 */

/* Directives */
%token D_NAMEREG        D_ORG           D_CONSTANT      D_WHILE         D_DB
%token D_LIST           D_MESSG         D_NOLIST        D_SKIP          D_ERROR
%token D_TITLE          D_EXPAND        D_NOEXPAND      D_IF            D_PORT
%token D_IFN            D_IFDEF         D_IFNDEF        D_ELSEIFB       D_ELSEIFNB
%token D_ELSE           D_ELSEIF        D_ELSEIFN       D_ELSEIFDEF     D_ELSEIFNDEF
%token D_ENDIF          D_LOCAL         D_IFNB          D_IFB
%token D_ENDM           D_EXITM         D_REPT          D_MACRO         D_EQU
%token D_END            D_REG           D_CODE          D_ENDW          D_WARNING
%token D_VARIABLE       D_SET           D_DEFINE        D_UNDEFINE      D_ENDR
%token D_REGAUTO        D_SPRAUTO       D_DATA

/* Instructions */
%token I_JUMP           I_CALL          I_RETURN        I_ADD           I_ADDCY
%token I_SUB            I_SUBCY         I_COMPARE       I_ENABLE_INT    I_DISABLE_INT
%token I_LOAD           I_AND           I_OR            I_XOR           I_TEST
%token I_SLX            I_SLA           I_RL            I_INPUT         I_OUTPUT
%token I_SRX            I_SRA           I_RR            I_SL0           I_SL1
%token I_STORE          I_FETCH         I_SR0           I_SR1           I_RETURNI_DIS
%token I_RETURNI_ENA

/* Fixed operands, i.e. those which have no value */
%token Z NZ C NC

/* Other terminal symbols */
%token COMMENT
%token AT
%token EOL              "end of line"
%token END      0       "end of file"
// Named tokens (to make the grammar rules more readable)
%token LP               "("
%token RP               ")"
%token IMMEDIATE        "#"
%token COMMA            ","
// Expression operators
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

// Expression functions
%token F_HIGH           F_LOW

/* Operator precedence (the one declared later has the higher precedence) */
// Left-to-right
%left "||"
%left "&&"
%left "|"
%left "^"
%left "&"
%left "==" "!="
%left "<" "<=" ">=" ">"
%left "<<" ">>"
%left "+" "-"
%left "*" "/" "%"
// Right-to-left
%right "~" "!"

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
%type<expr>     expr            number          params          args            args_str
%type<expr>     id              string
// Statements - general
%type<stmt>     statements      stmt            inst_stmt       dir_stmt        macro_stmt
%type<stmt>     instruction     directive       macro           label
// Statements - directives
%type<stmt>     dir_cond_asm    dir_skip        dir_local       dir_exitm       dir_list
%type<stmt>     dir_include     dir_repeat      dir_org         dir_end         dir_reg
%type<stmt>     dir_set         dir_equ         dir_macro       dir_if          dir_ifn
%type<stmt>     dir_ifdef       dir_ifndef      dir_ifb         dir_ifnb        dir_elseifnb
%type<stmt>     dir_elseif      dir_elseifn     dir_elseifdef   dir_elseifndf   dir_elseifb
%type<stmt>     dir_constant    dir_variable    dir_while       dir_namereg     dir_port
%type<stmt>     dir_while_a     dir_endw        dir_error       dir_undefine    dir_define
%type<stmt>     dir_title       dir_nolist      dir_regauto     dir_regauto_a   dir_sprauto
%type<stmt>     dir_expand      dir_noexpand    dir_code        dir_db          dir_messg
%type<stmt>     dir_code_a      if_block        ifelse_block    dir_endif       dir_warning
%type<stmt>     else_block      dir_else        dir_if_a        dir_ifn_a       dir_ifdef_a
%type<stmt>     dir_ifndef_a    dir_ifb_a       dir_ifnb_a      dir_elseif_a    dir_elseifn_a
%type<stmt>     dir_elseifdef_a dir_elseifndf_a dir_elseifb_a   dir_elseifnb_a  dir_rept
%type<stmt>     dir_endm        dir_macro_d     dir_macro_a     dir_endr        dir_endr_a
%type<stmt>     dir_db_a        dir_endm_a      dir_expand_a    dir_noexpand_a  dir_sprauto_a
%type<stmt>     dir_data
// Statements - instructions
%type<stmt>     inst_jump       inst_call       inst_return     inst_add        inst_addcy
%type<stmt>     inst_sub        inst_subcy      inst_compare    inst_returni    inst_enable_int
%type<stmt>     inst_load       inst_and        inst_or         inst_xor        inst_disable_int
%type<stmt>     inst_test       inst_store      inst_fetch      inst_sr0        inst_sr1
%type<stmt>     inst_srx        inst_sra        inst_rr         inst_sl0        inst_sl1
%type<stmt>     inst_slx        inst_sla        inst_rl         inst_input      inst_output

/*
 * Symbol destructors:
 * each time the parser discards symbol with certain semantic types, their memory have to bee freed.
 */
%destructor
{
    if ( NULL != $$ )
    {
        $$->completeDelete();
    }
} <expr>

%destructor
{
    if ( NULL != $$ )
    {
        $$->completeDelete();
    }
} <stmt>

%destructor
{
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
      id                            { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_MACRO, $id); }
    | id "(" ")"                    { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_MACRO, $id); }
    | id args                       { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_MACRO, $id->appendLink($args)); }
    | id "(" args ")"               { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_MACRO, $id->appendLink($args)); }
;

/*
 * Generally used non-terminal symbols
 */
number:
      NUMBER                        { $$ = new CompilerExpr($1, LOC(@$)); }
;
id:
      IDENFIFIER                    { $$ = new CompilerExpr($1, LOC(@$)); }
;
string:
      STRING                        { $$ = new CompilerExpr(CompilerExpr::Value($1.data, $1.size), LOC(@$)); }
;
label:
      LABEL                         {$$=new CompilerStatement(LOC(@$), ASMKCPSM3_LABEL, new CompilerExpr($1, LOC(@$)));}
;
expr:
      id                            { $$ = $id; }
    | number                        { $$ = $number; }
    | "(" expr ")"                  { $$ = $2; }
    | F_LOW  "(" expr ")"           { $$ = new CompilerExpr(CompilerExpr::OPER_LOW, $3, LOC(@$)); }
    | F_HIGH "(" expr ")"           { $$ = new CompilerExpr(CompilerExpr::OPER_HIGH, $3, LOC(@$)); }
    | "~" expr                      { $$ = new CompilerExpr('~', $2, LOC(@$)); }
    | "!" expr                      { $$ = new CompilerExpr('!', $2, LOC(@$)); }
    | expr "+"  expr                { $$ = new CompilerExpr($1, '+', $3, LOC(@$)); }
    | expr "-"  expr                { $$ = new CompilerExpr($1, '-', $3, LOC(@$)); }
    | expr "*"  expr                { $$ = new CompilerExpr($1, '*', $3, LOC(@$)); }
    | expr "/"  expr                { $$ = new CompilerExpr($1, '/', $3, LOC(@$)); }
    | expr "%"  expr                { $$ = new CompilerExpr($1, '%', $3, LOC(@$)); }
    | expr "||" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LOR, $3, LOC(@$)); }
    | expr "&&" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LAND, $3, LOC(@$)); }
    | expr "|"  expr                { $$ = new CompilerExpr($1, '|', $3, LOC(@$)); }
    | expr "^"  expr                { $$ = new CompilerExpr($1, '^', $3, LOC(@$)); }
    | expr "&"  expr                { $$ = new CompilerExpr($1, '&', $3, LOC(@$)); }
    | expr "==" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_EQ, $3, LOC(@$)); }
    | expr "!=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_NE, $3, LOC(@$)); }
    | expr "<"  expr                { $$ = new CompilerExpr($1, '<', $3, LOC(@$)); }
    | expr "<=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LE, $3, LOC(@$)); }
    | expr ">=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_GE, $3, LOC(@$)); }
    | expr ">"  expr                { $$ = new CompilerExpr($1, '>', $3, LOC(@$)); }
    | expr ">>" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_SHR, $3, LOC(@$)); }
    | expr "<<" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_SHL, $3, LOC(@$)); }
    | expr expr                     {
                                        /* Syntax error */
                                        $$ = $1->appendLink($2);
                                        if ( 0 == YYRECOVERING() )
                                        {
                                            @1.first_line = @1.last_line; \
                                            @1.first_column = @1.last_column; \
                                            @1.last_column++; \
                                            compiler->parserMessage ( @1,
                                                                      CompilerBase::MT_ERROR,
                                                                      QObject::tr("missing operator").toStdString() );
                                        }
                                    }
;
args:           // List of arguments without strings, e.g. `(1+3), XYZ, 0x4b'
      args "," expr                 { $$ = $1->appendLink($expr); }
    | expr                          { $$ = $expr; }
;
args_str:       // List of arguments possibly with strings, e.g. `ABC, "XYZ - QWE", 2 * 0x4b'
      args_str "," string           { $$ = $1->appendLink($string); }
    | args_str "," expr             { $$ = $1->appendLink($expr); }
    | string                        { $$ = $string; }
    | expr                          { $$ = $expr; }
;
params:         // List of identifiers, e.g. `target, source, offset, size, mode'
      params "," id                 { $$ = $1->appendLink($id); }
    | id                            { $$ = $id; }
;
opr:            // Matches any operand, intended for error recovery only
      expr                          { $expr->completeDelete(); }
    | Z
    | NZ
    | C
    | NC
;
oprs:           // Matches any set of operands, intended for error recovery only
      oprs "," opr                  { }
    | opr                           { }
;
eopr:           // Matches any operand, intended for error recovery only
      expr                          { $expr->completeDelete(); }
    | "#" expr                      { $expr->completeDelete(); }
;
eoprs:          // Matches any set of operands, intended for error recovery only
      eoprs "," eopr                { }
    | eopr                          { }
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
    | dir_variable  { $$ = $1; }    | dir_namereg   { $$ = $1; }
    | dir_while     { $$ = $1; }    | dir_reg       { $$ = $1; }
    | dir_db        { $$ = $1; }    | dir_define    { $$ = $1; }
    | dir_port      { $$ = $1; }    | dir_undefine  { $$ = $1; }
    | dir_messg     { $$ = $1; }    | dir_title     { $$ = $1; }
    | dir_expand    { $$ = $1; }    | dir_noexpand  { $$ = $1; }
    | dir_sprauto   { $$ = $1; }    | dir_regauto   { $$ = $1; }
    | dir_code      { $$ = $1; }    | dir_error     { $$ = $1; }
    | dir_warning   { $$ = $1; }    | dir_data      { $$ = $1; }
;
dir_cond_asm:
      if_block ifelse_block else_block dir_endif
                                    {
                                        $$ = new CompilerStatement ( CompilerBase::SourceLocation(),
                                                                     ASMKCPSM3_COND_ASM );
                                        $$->createBranch ( $if_block -> appendLink($ifelse_block)
                                                                     -> appendLink($else_block)
                                                                     -> appendLink($dir_endif) );
                                    }
    | label if_block ifelse_block else_block dir_endif
                                    {
                                        $$ = $label->appendLink(new CompilerStatement(CompilerBase::SourceLocation(),
                                                                                      ASMKCPSM3_COND_ASM));
                                        $$->createBranch ( $if_block -> appendLink($ifelse_block)
                                                                     -> appendLink($else_block)
                                                                     -> appendLink($dir_endif) );
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
dir_endif:
      D_ENDIF                       { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ENDIF); }
;
dir_else:
      D_ELSE                        { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ELSE); }
    | label D_ELSE                  {
                                        /* Syntax error */
                                        NO_LABEL_EXPECTED(@label, "ELSE", $label);
                                        $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ELSE);
                                    }
    | label D_ELSE args             {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("ELSE", $args, @args);
                                        NO_LABEL_EXPECTED(@label, "ELSE", $label);
                                        $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ELSE);
                                    }
    | D_ELSE args                   {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("ELSE", $args, @args);
                                        $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ELSE);
                                    }
;
dir_if:
      dir_if_a EOL                  { $$ = $dir_if_a; }
    | dir_if_a COMMENT EOL          { $$ = $dir_if_a; }
;
dir_if_a:
      D_IF expr                     { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_IF, $expr); }
    | D_IF                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IF, "IF"); }
;
dir_ifn:
      dir_ifn_a EOL                 { $$ = $dir_ifn_a; }
    | dir_ifn_a COMMENT EOL         { $$ = $dir_ifn_a; }
;
dir_ifn_a:
      D_IFN expr                    { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_IFN, $expr); }
    | D_IFN                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFN, "IFN"); }
;
dir_ifdef:
      dir_ifdef_a EOL               { $$ = $dir_ifdef_a; }
    | dir_ifdef_a COMMENT EOL       { $$ = $dir_ifdef_a; }
;
dir_ifdef_a:
      D_IFDEF id                    { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_IFDEF, $id); }
    | D_IFDEF                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFDEF, "IFDEF"); }
;
dir_ifndef:
      dir_ifndef_a EOL              { $$ = $dir_ifndef_a; }
    | dir_ifndef_a COMMENT EOL      { $$ = $dir_ifndef_a; }
;
dir_ifndef_a:
      D_IFNDEF id                   { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_IFNDEF, $id); }
    | D_IFNDEF                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFNDEF, "IFNDEF"); }
;
dir_ifb:
      dir_ifb_a EOL                 { $$ = $dir_ifb_a; }
    | dir_ifb_a COMMENT EOL         { $$ = $dir_ifb_a; }
;
dir_ifb_a:
      D_IFB id                      { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_IFB, $id); }
    | D_IFB                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFB, "IFB"); }
;
dir_ifnb:
      dir_ifnb_a EOL                { $$ = $dir_ifnb_a; }
    | dir_ifnb_a COMMENT EOL        { $$ = $dir_ifnb_a; }
;
dir_ifnb_a:
      D_IFNB id                     { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_IFNB, $id); }
    | D_IFNB                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFNB, "IFNB"); }
;
dir_elseif:
      dir_elseif_a EOL              { $$ = $dir_elseif_a; }
    | dir_elseif_a COMMENT EOL      { $$ = $dir_elseif_a; }
;
dir_elseif_a:
      D_ELSEIF expr                 { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ELSEIF, $expr); }
    | D_ELSEIF                      { /* Syntax Error */ $$ = NULL; ARG_REQUIRED_D(@D_ELSEIF, "ELSEIF"); }
    | label D_ELSEIF expr           {
                                        /* Syntax Error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label, "ELSEIF", $label);
                                        $expr->completeDelete();
                                    }
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
      D_ELSEIFN expr                { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ELSEIFN, $expr); }
    | D_ELSEIFN                     {
                                        /* Syntax Error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_ELSEIFN, "ELSEIFN");
                                    }
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
      D_ELSEIFDEF expr              { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ELSEIFDEF, $expr); }
    | D_ELSEIFDEF                   {
                                        /* Syntax Error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_ELSEIFDEF, "ELSEIFDEF");
                                    }
    | label D_ELSEIFDEF expr        {
                                        /* Syntax Error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label, "ELSEIFDEF", $label);
                                        $expr->completeDelete();
                                    }
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
      D_ELSEIFNDEF expr             { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ELSEIFNDEF, $expr); }
    | D_ELSEIFNDEF                  {
                                        /* Syntax Error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_ELSEIFNDEF, "ELSEIFNDEF");
                                    }
    | label D_ELSEIFNDEF expr       {
                                        /* Syntax Error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label, "ELSEIFNDEF", $label);
                                        $expr->completeDelete();
                                    }
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
      D_ELSEIFB expr                { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ELSEIFB, $expr); }
    | D_ELSEIFB                     {
                                        /* Syntax Error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_ELSEIFB, "ELSEIFB");
                                    }
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
      D_ELSEIFNB expr               { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ELSEIFNB, $expr); }
    | D_ELSEIFNB                    {
                                        /* Syntax Error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_ELSEIFNB, "ELSEIFNB");
                                    }
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
      D_ORG expr                    { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ORG, $expr); }
    | label D_ORG expr              { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ORG, $expr)); }
    | D_ORG                         {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_ORG, "ORG");
                                    }
    | label D_ORG                   {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_ORG, "ORG");
                                        $label->completeDelete();
                                    }
;
dir_skip:
      D_SKIP expr                   { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_SKIP, $expr); }
    | label D_SKIP expr             { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_SKIP, $expr)); }
    | D_SKIP                        {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_SKIP, "SKIP");
                                    }
    | label D_SKIP                  {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_SKIP, "SKIP");
                                        $label->completeDelete();
                                    }
;
dir_local:
      D_LOCAL id                    { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_LOCAL, $id); }
    | label D_LOCAL id              { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMKCPSM3_LOCAL, $id)); }
    | D_LOCAL                       {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_LOCAL, "LOCAL");
                                    }
    | label D_LOCAL                 {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_LOCAL, "LOCAL");
                                        $label->completeDelete();
                                    }
;
dir_exitm:
      D_EXITM                       { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_EXITM); }
    | label D_EXITM                 { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_EXITM)); }
    | D_EXITM args                  {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_ARG_EXPECTED_D("EXITM", $args, @args);
                                    }
    | label D_EXITM args            {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_ARG_EXPECTED_D("EXITM", $args, @args);
                                        $label->completeDelete();
                                    }
;
dir_list:
      D_LIST                        { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_LIST); }
    | label D_LIST                  { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_LIST)); }
    | D_LIST args                   {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_ARG_EXPECTED_D("LIST", $args, @args);
                                    }
    | label D_LIST args             {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_ARG_EXPECTED_D("LIST", $args, @args);
                                        $label->completeDelete();
                                    }
;
dir_nolist:
      D_NOLIST                      { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_NOLIST); }
    | label D_NOLIST                { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_NOLIST)); }
    | D_NOLIST args                 {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_ARG_EXPECTED_D("NOLIST", $args, @args);
                                    }
    | label D_NOLIST args           {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_ARG_EXPECTED_D("NOLIST", $args, @args);
                                        $label->completeDelete();
                                    }
;
dir_set:
      id D_SET expr                 { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_SET, $id->appendLink($expr)); }
    | id D_SET                      {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_SET, "SET");
                                        $id->completeDelete();
                                    }
    | D_SET                         {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_SET, "SET");
                                        ARG_REQUIRED_D(@D_SET, "SET");
                                    }
    | D_SET expr                    {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_SET, "SET");
                                        $expr->completeDelete();
                                    }
    | label D_SET expr              {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_SET, "SET");
                                        NO_LABEL_EXPECTED(@label, "SET", $label->appendArgsLink($expr))
                                    }
    | label id D_SET expr           {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label, "SET", $label->appendArgsLink($id->appendLink($expr)));
                                    }
;
dir_equ:
      id D_EQU expr                 { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_EQU, $id->appendLink($expr)); }
    | id D_EQU                      {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_EQU, "EQU");
                                        $id->completeDelete();
                                    }
    | D_EQU                         {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_EQU, "EQU");
                                        ARG_REQUIRED_D(@D_EQU, "EQU");
                                    }
    | D_EQU expr                    {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_EQU, "EQU");
                                        $expr->completeDelete();
                                    }
    | label D_EQU expr              {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_EQU, "EQU");
                                        NO_LABEL_EXPECTED(@label, "EQU", $label->appendArgsLink($expr))
                                    }
    | label id D_EQU expr           {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label, "EQU", $label->appendArgsLink($id->appendLink($expr)));
                                    }
;
dir_reg:
      id D_REG expr                 { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_REG, $id->appendLink($expr)); }
    | id D_REG                      {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_REG, "REG");
                                        $id->completeDelete();
                                    }
    | D_REG                         {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_REG, "REG");
                                        ARG_REQUIRED_D(@D_REG, "REG");
                                    }
    | D_REG expr                    {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_REG, "REG");
                                        $expr->completeDelete();
                                    }
    | label D_REG expr              {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_REG, "REG");
                                        NO_LABEL_EXPECTED(@label, "REG", $label->appendArgsLink($expr))
                                    }
    | label id D_REG expr           {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label, "REG", $label->appendArgsLink($id->appendLink($expr)));
                                    }
;
dir_undefine:
      D_UNDEFINE id                 { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_DIR_UNDEFINE,$id); }
    | D_UNDEFINE                    {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_UNDEFINE, "UNDEFINE");
                                    }
    | label D_UNDEFINE              {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_UNDEFINE, "UNDEFINE");
                                        NO_LABEL_EXPECTED(@label, "UNDEFINE", $label)
                                    }
    | label D_UNDEFINE id           {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label, "UNDEFINE", $label->appendArgsLink($id));
                                    }
;
dir_define:
      id D_DEFINE expr              { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_DEFINE, $id->appendLink($expr)); }
    | id D_DEFINE                   {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_DEFINE, "DEFINE");
                                        $id->completeDelete();
                                    }
    | D_DEFINE                      {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_DEFINE, "DEFINE");
                                        ARG_REQUIRED_D(@D_DEFINE, "DEFINE");
                                    }
    | D_DEFINE expr                 {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_DEFINE, "DEFINE");
                                        $expr->completeDelete();
                                    }
    | label D_DEFINE expr           {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_DEFINE, "DEFINE");
                                        NO_LABEL_EXPECTED(@label, "DEFINE", $label->appendArgsLink($expr))
                                    }
    | label id D_DEFINE expr        {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label, "DEFINE", $label->appendArgsLink($id->appendLink($expr)));
                                    }
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
      id D_MACRO                    { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_MACRO, $id); }
    | D_MACRO                       { /* Syntax error */ $$ = NULL; DECL_ID_EXPECTED(@D_MACRO, "MACRO"); }
    | label D_MACRO                 {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label, "MACRO", $label);
                                        DECL_ID_EXPECTED(@D_MACRO, "MACRO");
                                    }
;
dir_endm:
      dir_endm_a COMMENT            { $$ = $dir_endm_a; }
    | dir_endm_a                    { $$ = $dir_endm_a; }
    | label dir_endm_a COMMENT      { $$ = $label->appendLink($dir_endm_a); }
    | label dir_endm_a              { $$ = $label->appendLink($dir_endm_a); }
;
dir_endm_a:
      D_ENDM                        { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ENDM); }
    | D_ENDM args                   { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("ENDM", $args, @args); }
;
dir_include:
      INCLUDE                       {
                                        CompilerBase::SourceLocation location = compiler -> toSourceLocation ( compiler -> m_yyllocStack.back() );
                                        location.m_fileNumber = compiler -> getFileNumber(1);

                                        $$ = new CompilerStatement ( location,
                                                                     ASMKCPSM3_INCLUDE,
                                                                     new CompilerExpr ( compiler->getFileNumber ( (const char*)$INCLUDE.data ) ) );
                                    }
    | label INCLUDE                 {
                                        CompilerBase::SourceLocation location = compiler -> toSourceLocation ( compiler -> m_yyllocStack.back() );
                                        location.m_fileNumber = compiler -> getFileNumber(1);

                                        $$ = $label->appendLink ( new CompilerStatement ( location,
                                                                                          ASMKCPSM3_INCLUDE,
                                                                                          new CompilerExpr ( compiler->getFileNumber ( (const char*)$INCLUDE.data ) ) ) );
                                    }
;
dir_end:
      D_END                         { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_END); }
    | label D_END                   { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_END)); }
    | D_END args                    {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("END", $args, @args);
                                        $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_END);
                                    }
    | label D_END args              {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("END", $args, @args);
                                        $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_END));
                                    }
;
dir_constant:
      D_CONSTANT id "," expr        { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_EQU, $id->appendLink($expr)); }
    | label D_CONSTANT id "," expr  {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMKCPSM3_DIR_EQU, $id -> appendLink ( $expr ) ) );
                                    }
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
      D_VARIABLE id "," expr        { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_SET, $id->appendLink($expr)); }
    | label D_VARIABLE id "," expr  {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMKCPSM3_DIR_SET, $id -> appendLink ( $expr ) ) );
                                    }
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
dir_namereg:
      D_NAMEREG id "," expr        { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_REG, $id->appendLink($expr)); }
    | label D_NAMEREG id "," expr  {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMKCPSM3_DIR_REG, $id -> appendLink ( $expr ) ) );
                                    }
    | D_NAMEREG expr                {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_NAMEREG, "NAMEREG");
                                        $expr->completeDelete();
                                    }
    | label D_NAMEREG expr          {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_NAMEREG, "NAMEREG");
                                        $label->completeDelete();
                                        $expr->completeDelete();
                                    }
;
dir_repeat:
      dir_rept
      statements EOL
      dir_endr                      { $$ = $dir_rept->createBranch($statements->appendLink($dir_endr)); }
    | dir_rept
      dir_endr                      { $$ = $dir_rept->createBranch($dir_endr); }
;
dir_rept:
      D_REPT expr COMMENT           { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_REPT, $expr); }
    | D_REPT expr                   { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_REPT, $expr); }
    | D_REPT COMMENT                { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_REPT); }
    | label D_REPT expr COMMENT     { $$ = $label->appendLink(new CompilerStatement(LOC(@$),ASMKCPSM3_DIR_REPT,$expr));}
    | label D_REPT expr             { $$ = $label->appendLink(new CompilerStatement(LOC(@$),ASMKCPSM3_DIR_REPT,$expr));}
    | D_REPT                        { /* Syntax error */ $$ = NULL;   ARG_REQUIRED_D(@D_REPT, "REPT"); }
    | label D_REPT COMMENT          { /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_REPT, "REPT"); }
    | label D_REPT                  { /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_REPT, "REPT"); }
;
dir_endr:
      dir_endr_a COMMENT            { $$ = $dir_endr_a; }
    | dir_endr_a                    { $$ = $dir_endr_a; }
    | label dir_endr_a COMMENT      { $$ = $label->appendLink($dir_endr_a); }
    | label dir_endr_a              { $$ = $label->appendLink($dir_endr_a); }
;
dir_endr_a:
      D_ENDR                        { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ENDR); }
    | D_ENDR args                   { /* Syntax error */ $$ = NULL; NO_ARG_EXPECTED_D("ENDR", $args, @args); }
;
dir_while:
      dir_while_a
      statements EOL
      dir_endw                      { $$ = $dir_while_a->createBranch($statements->appendLink($dir_endw)); }
    | dir_while_a
      dir_endw                      { $$ = $dir_while_a->createBranch($dir_endw); }
;
dir_while_a:
      D_WHILE expr COMMENT          { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_WHILE, $expr); }
    | D_WHILE expr                  { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_WHILE, $expr); }
    | D_WHILE COMMENT               { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_WHILE); }
    | label D_WHILE expr COMMENT    { $$=$label->appendLink(new CompilerStatement(LOC(@$),ASMKCPSM3_DIR_WHILE,$expr)); }
    | label D_WHILE expr            { $$=$label->appendLink(new CompilerStatement(LOC(@$),ASMKCPSM3_DIR_WHILE,$expr)); }
    | D_WHILE                       { /* Syntax error */ $$ = NULL;   ARG_REQUIRED_D(@D_WHILE, "WHILE"); }
    | label D_WHILE COMMENT         { /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_WHILE, "WHILE"); }
    | label D_WHILE                 { /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_WHILE, "WHILE"); }
;
dir_endw:
      dir_endw_a COMMENT            { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ENDW); }
    | dir_endw_a                    { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ENDW); }
    | label dir_endw_a COMMENT      { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ENDW)); }
    | label dir_endw_a              { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ENDW)); }
;
dir_endw_a:
      D_ENDW                        { }
    | D_ENDW args                   { /* Syntax error */ NO_ARG_EXPECTED_D("ENDM", $args, @args); }
;
dir_data:
      id D_DATA expr                { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_DATA, $id->appendLink($expr)); }
    | id D_DATA                     {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_DATA, "DATA");
                                        $id->completeDelete();
                                    }
    | D_DATA                        {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_DATA, "DATA");
                                        ARG_REQUIRED_D(@D_DATA, "DATA");
                                    }
    | D_DATA expr                   {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_DATA, "DATA");
                                        $expr->completeDelete();
                                    }
    | label D_DATA expr             {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_DATA, "DATA");
                                        NO_LABEL_EXPECTED(@label, "DATA", $label->appendArgsLink($expr));
                                    }
    | label id D_DATA expr          {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label,
                                                          "DATA",
                                                          $label->appendArgsLink ( $id->appendLink($expr) ) );
                                    }
;
dir_port:
      id D_PORT expr                { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_DIR_PORT,$id->appendLink($expr)); }
    | id D_PORT                     {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_PORT, "PORT");
                                        $id->completeDelete();
                                    }
    | D_PORT                        {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_PORT, "PORT");
                                        ARG_REQUIRED_D(@D_PORT, "PORT");
                                    }
    | D_PORT expr                   {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_PORT, "PORT");
                                        $expr->completeDelete();
                                    }
    | label D_PORT expr             {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_PORT, "PORT");
                                        NO_LABEL_EXPECTED(@label, "PORT", $label->appendArgsLink($expr));
                                    }
    | label id D_PORT expr          {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label,
                                                          "PORT",
                                                          $label->appendArgsLink ( $id->appendLink($expr) ) );
                                    }
;
dir_regauto:
      dir_regauto_a                 { $$ = $dir_regauto_a; }
    | label dir_regauto_a           {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label, "REGAUTO", $label->appendLink($dir_regauto_a));
                                    }
;
dir_regauto_a:
      id D_REGAUTO                  { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_DIR_REGAUTO,$id); }
    | id D_REGAUTO AT expr          { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_DIR_REGAUTO,$id->appendLink($expr));}
    | id D_REGAUTO expr             {
                                        /* Syntax error */
                                        MISSING_AT_OPERATOR(@D_REGAUTO, "REGAUTO");
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMKCPSM3_DIR_REGAUTO,
                                                                     $id->appendLink($expr) );
                                    }
    | D_REGAUTO                     {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_REGAUTO, "REGAUTO");
                                    }
    | D_REGAUTO AT expr             {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_REGAUTO, "REGAUTO");
                                        $expr->completeDelete();
                                    }
    | D_REGAUTO expr                {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_REGAUTO, "REGAUTO");
                                        MISSING_AT_OPERATOR(@D_REGAUTO, "D_REGAUTO");
                                        $expr->completeDelete();
                                    }
;
dir_sprauto:
      dir_sprauto_a                 { $$ = $dir_sprauto_a; }
    | label dir_sprauto_a           {
                                        /* Syntax error */
                                        $$ = NULL;
                                        NO_LABEL_EXPECTED(@label, "SPRAUTO", $label->appendLink($dir_sprauto_a));
                                    }
;
dir_sprauto_a:
      id D_SPRAUTO                  { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_DIR_SPRAUTO,$id); }
    | id D_SPRAUTO AT expr          { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_DIR_SPRAUTO,$id->appendLink($expr));}
    | id D_SPRAUTO expr             {
                                        /* Syntax error */
                                        MISSING_AT_OPERATOR(@D_SPRAUTO, "SPRAUTO");
                                        $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_SPRAUTO, $id->appendLink($expr));
                                    }
    | D_SPRAUTO                     {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_SPRAUTO, "SPRAUTO");
                                    }
    | D_SPRAUTO AT expr             {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_SPRAUTO, "SPRAUTO");
                                        $expr->completeDelete();
                                    }
    | D_SPRAUTO expr                {
                                        /* Syntax error */
                                        $$ = NULL;
                                        DECL_ID_EXPECTED(@D_SPRAUTO, "SPRAUTO");
                                        MISSING_AT_OPERATOR(@D_SPRAUTO, "D_SPRAUTO");
                                        $expr->completeDelete();
                                    }
;
dir_db:
      dir_db_a                      { $$ = $dir_db_a; }
    | label dir_db_a                { $$ = $label->appendLink($dir_db_a); }
;
dir_db_a:
      D_DB args_str                 { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_DB, $args_str); }
    | id D_DB args_str              { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_DB, $id->appendLink($args_str)); }
    | D_DB                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DB, "DB"); }
    | id D_DB                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_DB, "DB"); $id->completeDelete(); }
;
dir_error:
      D_ERROR string                { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_ERROR, $string); }
    | label D_ERROR string          {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMKCPSM3_DIR_ERROR,
                                                                                            $string ) );
                                    }
    | label D_ERROR                 {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_ERROR, "ERROR");
                                        $label->completeDelete();
                                    }
    | D_ERROR                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_ERROR, "ERROR"); }
;
dir_warning:
      D_WARNING string              { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_WARNING, $string); }
    | label D_WARNING string        {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMKCPSM3_DIR_WARNING,
                                                                                            $string ) );
                                    }
    | label D_WARNING               {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_WARNING, "WARNING");
                                        $label->completeDelete();
                                    }
    | D_WARNING                     { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_WARNING, "WARNING"); }
;
dir_messg:
      D_MESSG string                { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_MESSG, $string); }
    | label D_MESSG string          {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMKCPSM3_DIR_MESSG,
                                                                                            $string ) );
                                    }
    | label D_MESSG                 {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_MESSG, "MESSG");
                                        $label->completeDelete();
                                    }
    | D_MESSG                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_MESSG, "MESSG"); }
;
dir_title:
      D_TITLE string                { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_TITLE, $string); }
    | label D_TITLE string          {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMKCPSM3_DIR_TITLE,
                                                                                            $string ) );
                                    }
    | label D_TITLE                 {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_TITLE, "TITLE");
                                        $label->completeDelete();
                                    }
    | D_TITLE                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_TITLE, "TITLE"); }
;
dir_expand:
      dir_expand_a                  { $$ = $dir_expand_a; }
    | dir_expand_a args             { /* Syntax error */ $$ = $dir_expand_a; NO_ARG_EXPECTED_D("EXPAND",$args,@args); }
;
dir_expand_a:
      D_EXPAND                      { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_EXPAND); }
    | label D_EXPAND                {
                                        /* Syntax error */
                                        $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_EXPAND);
                                        NO_LABEL_EXPECTED(@label, "EXPAND", $label);
                                    }
;
dir_noexpand:
      dir_noexpand_a                { $$ = $dir_noexpand_a; }
    | dir_noexpand_a args           { /* Syntax error */ $$=$dir_noexpand_a;NO_ARG_EXPECTED_D("NOEXPAND",$args,@args); }
;
dir_noexpand_a:
      D_NOEXPAND                    { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_NOEXPAND); }
    | label D_NOEXPAND              {
                                        /* Syntax error */
                                        $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_NOEXPAND);
                                        NO_LABEL_EXPECTED(@label, "NOEXPAND", $label);
                                    }
;
dir_code:
      dir_code_a                    { $$ = $1; }
    | label dir_code_a              { /* Syntax error */ $$ = $2; NO_LABEL_EXPECTED(@label, "CODE", $label); }
;
dir_code_a:
      D_CODE                        { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_CODE); }
    | D_CODE expr                   {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMKCPSM3_DIR_CODE,
                                                                     ( new CompilerExpr() ) -> appendLink ( $expr ) );
                                    }
    | id D_CODE                     { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_CODE, $id->appendLink(new CompilerExpr())); }
    | id D_CODE expr                { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_DIR_CODE, $id->appendLink($expr)); }
;

/*
 * Instructions
 */
instruction:
    /* Program Control Group */
      inst_jump         { $$ = $1; } | inst_call        { $$ = $1; }
    | inst_return       { $$ = $1; }
    /* Arithmetic Group */
    | inst_add          { $$ = $1; } | inst_addcy       { $$ = $1; }
    | inst_sub          { $$ = $1; } | inst_subcy       { $$ = $1; }
    | inst_compare      { $$ = $1; }
    /* Interrupt Group */
    | inst_returni      { $$ = $1; } | inst_enable_int  { $$ = $1; }
    | inst_disable_int  { $$ = $1; }
    /* Logical Group */
    | inst_load         { $$ = $1; } | inst_and         { $$ = $1; }
    | inst_or           { $$ = $1; } | inst_xor         { $$ = $1; }
    | inst_test         { $$ = $1; }
    /* Storage Group */
    | inst_store        { $$ = $1; } | inst_fetch       { $$ = $1; }
    /* Shift And Rotate Group */
    | inst_sr0          { $$ = $1; } | inst_sr1         { $$ = $1; }
    | inst_srx          { $$ = $1; } | inst_sra         { $$ = $1; }
    | inst_rr           { $$ = $1; } | inst_sl0         { $$ = $1; }
    | inst_sl1          { $$ = $1; } | inst_slx         { $$ = $1; }
    | inst_sla          { $$ = $1; } | inst_rl          { $$ = $1; }
    /* Input/Output Group */
    | inst_input        { $$ = $1; } | inst_output      { $$ = $1; }
;

/* Program Control Group */
inst_jump:
      I_JUMP expr                   { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_JUMP_AAA,    $expr ); }
    | I_JUMP Z "," expr             { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_JUMP_Z_AAA,  $expr ); }
    | I_JUMP NZ "," expr            { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_JUMP_NZ_AAA, $expr ); }
    | I_JUMP C "," expr             { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_JUMP_C_AAA,  $expr ); }
    | I_JUMP NC "," expr            { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_JUMP_NC_AAA, $expr ); }
    | I_JUMP opr "," opr "," oprs   { /* Syntax Error */ $$ = NULL; NN_OPERANDS_EXPECTED(@1, "JUMP", 1, 2);   }
    | I_JUMP                        { /* Syntax Error */ $$ = NULL; NN_OPERANDS_EXPECTED(@1, "JUMP", 1, 2);   }
    | I_JUMP opr oprs               { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                  }
;
inst_call:
      I_CALL expr                   { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_CALL_AAA,    $expr ); }
    | I_CALL Z "," expr             { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_CALL_Z_AAA,  $expr ); }
    | I_CALL NZ "," expr            { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_CALL_NZ_AAA, $expr ); }
    | I_CALL C "," expr             { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_CALL_C_AAA,  $expr ); }
    | I_CALL NC "," expr            { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_CALL_NC_AAA, $expr ); }
    | I_CALL opr "," opr "," oprs   { /* Syntax Error */ $$ = NULL; NN_OPERANDS_EXPECTED(@1, "CALL", 1, 2);   }
    | I_CALL                        { /* Syntax Error */ $$ = NULL; NN_OPERANDS_EXPECTED(@1, "CALL", 1, 2);   }
    | I_CALL opr oprs               { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                  }
;
inst_return:
      I_RETURN                      { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_RETURN    ); }
    | I_RETURN Z                    { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_RETURN_Z  ); }
    | I_RETURN NZ                   { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_RETURN_NZ ); }
    | I_RETURN C                    { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_RETURN_C  ); }
    | I_RETURN NC                   { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_RETURN_NC ); }
    | I_RETURN opr "," oprs         { /* Syntax Error */ $$ = NULL; NN_OPERANDS_EXPECTED(@1, "RETURN", 1, 2); }
;
/* Arithmetic Group */
inst_add:
      I_ADD expr "," "#" expr       { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_INS_ADD_SX_KK, $2->appendLink($5));}
    | I_ADD expr "," expr           { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_INS_ADD_SX_SY, $2->appendLink($4));}
    | I_ADD eopr "," eopr "," eoprs { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ADD", 2);                 }
    | I_ADD eopr                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ADD", 2);                 }
    | I_ADD                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ADD", 2);                 }
    | I_ADD eopr eoprs              { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                           }
;
inst_addcy:
      I_ADDCY expr "," "#" expr     { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_ADDCY_SX_KK,$2->appendLink($5));}
    | I_ADDCY expr "," expr         { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_ADDCY_SX_SY,$2->appendLink($4));}
    | I_ADDCY eopr "," eopr "," eoprs { /* Syntax Error */ $$=NULL; N_OPERANDS_EXPECTED(@1, "ADDCY", 2);               }
    | I_ADDCY eopr                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ADDCY", 2);               }
    | I_ADDCY                       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "ADDCY", 2);               }
    | I_ADDCY eopr eoprs            { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                           }
;
inst_sub:
      I_SUB expr "," "#" expr       { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_INS_SUB_SX_KK, $2->appendLink($5)); }
    | I_SUB expr "," expr           { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_INS_SUB_SX_SY, $2->appendLink($4)); }
    | I_SUB eopr "," eopr "," eoprs { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SUB", 2);                }
    | I_SUB eopr                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SUB", 2);                }
    | I_SUB                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SUB", 2);                }
    | I_SUB eopr eoprs              { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                          }
;
inst_subcy:
      I_SUBCY expr "," "#" expr     { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_INS_SUBCY_SX_KK, $2->appendLink($5));}
    | I_SUBCY expr "," expr         { $$ = new CompilerStatement(LOC(@$), ASMKCPSM3_INS_SUBCY_SX_SY, $2->appendLink($4));}
    | I_SUBCY eopr "," eopr "," eoprs { /* Syntax Error */ $$=NULL; N_OPERANDS_EXPECTED(@1, "SUBCY", 2);               }
    | I_SUBCY eopr                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "SUBCY", 2);               }
    | I_SUBCY                       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SUBCY", 2);               }
    | I_SUBCY eopr eoprs            { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                           }
;
inst_compare:
      I_COMPARE expr "," "#" expr   { $$=new CompilerStatement(LOC(@$),ASMKCPSM3_INS_COMPARE_SX_KK,$2->appendLink($5));}
    | I_COMPARE expr "," expr       { $$=new CompilerStatement(LOC(@$),ASMKCPSM3_INS_COMPARE_SX_SY,$2->appendLink($4));}
    | I_COMPARE eopr "," eopr "," eoprs {/* Syntax Error */$$=NULL; N_OPERANDS_EXPECTED(@1, "COMPARE", 2);             }
    | I_COMPARE eopr                { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "COMPARE", 2);             }
    | I_COMPARE                     { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "COMPARE", 2);             }
    | I_COMPARE eopr eoprs          { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                           }
;
/* Interrupt Group */
inst_returni:
      I_RETURNI_ENA                 { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_RETURNI_ENA );           }
    | I_RETURNI_DIS                 { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_RETURNI_DIS );           }
    | I_RETURNI_ENA eoprs           { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "RETURNI ENABLE", 0);  }
    | I_RETURNI_DIS eoprs           { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "RETURNI DISABLE", 0); }
;
inst_enable_int:
      I_ENABLE_INT                  { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_ENABLE_INT );             }
    | I_ENABLE_INT eoprs            { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "ENABLE INTERRUPT", 0); }
;
inst_disable_int:
      I_DISABLE_INT                 { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_DISABLE_INT );             }
    | I_DISABLE_INT eoprs           { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "DISABLE INTERRUPT", 0); }
;
/* Logical Group */
inst_load:
      I_LOAD expr "," "#" expr      { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_LOAD_SX_KK,$2->appendLink($5)); }
    | I_LOAD expr "," expr          { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_LOAD_SX_SY,$2->appendLink($4)); }
    | I_LOAD eopr "," eopr "," eoprs{ /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "LOAD", 2);                }
    | I_LOAD eopr                   { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "LOAD", 2);                }
    | I_LOAD                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "LOAD", 2);                }
    | I_LOAD eopr eoprs             { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                           }
;
inst_and:
      I_AND expr "," "#" expr       { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_AND_SX_KK,$2->appendLink($5)); }
    | I_AND expr "," expr           { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_AND_SX_SY,$2->appendLink($4)); }
    | I_AND eopr "," eopr "," eoprs { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "AND", 2);                }
    | I_AND eopr                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "AND", 2);                }
    | I_AND                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "AND", 2);                }
    | I_AND eopr eoprs              { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                          }
;
inst_or:
      I_OR expr "," "#" expr        { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_OR_SX_KK,$2->appendLink($5)); }
    | I_OR expr "," expr            { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_OR_SX_SY,$2->appendLink($4)); }
    | I_OR eopr "," eopr "," eoprs  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "OR", 2);                }
    | I_OR eopr                     { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "OR", 2);                }
    | I_OR                          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "OR", 2);                }
    | I_OR eopr eoprs               { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                         }
;
inst_xor:
      I_XOR expr "," "#" expr       { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_XOR_SX_KK,$2->appendLink($5)); }
    | I_XOR expr "," expr           { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_XOR_SX_SY,$2->appendLink($4)); }
    | I_XOR eopr "," eopr "," eoprs { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "XOR", 2);                }
    | I_XOR eopr                    { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "XOR", 2);                }
    | I_XOR                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "XOR", 2);                }
    | I_XOR eopr eoprs              { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                          }
;
inst_test:
      I_TEST expr "," "#" expr      { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_TEST_SX_KK,$2->appendLink($5) );}
    | I_TEST expr "," expr          { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_TEST_SX_SY,$2->appendLink($4) );}
    | I_TEST eopr "," eopr "," eoprs{ /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "TEST", 2);                }
    | I_TEST eopr                   { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "TEST", 2);                }
    | I_TEST                        { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "TEST", 2);                }
    | I_TEST eopr eoprs             { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                           }
;
/* Storage Group */
inst_store:
      I_STORE expr "," "#" expr     { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_STORE_SX_SS,$2->appendLink($5));}
    | I_STORE expr "," expr         { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_STORE_SX_SY,$2->appendLink($4));}
    | I_STORE eopr "," eopr "," eoprs { /* Syntax Error */ $$=NULL; N_OPERANDS_EXPECTED(@1, "STORE", 2);               }
    | I_STORE eopr                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "STORE", 2);               }
    | I_STORE                       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "STORE", 2);               }
    | I_STORE eopr eoprs            { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                           }
;
inst_fetch:
      I_FETCH expr "," "#" expr     { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_FETCH_SX_SS,$2->appendLink($5));}
    | I_FETCH expr "," expr         { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_FETCH_SX_SY,$2->appendLink($4));}
    | I_FETCH eopr "," eopr "," eoprs { /* Syntax Error */$$= NULL; N_OPERANDS_EXPECTED(@1, "FETCH", 2);               }
    | I_FETCH eopr                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "FETCH", 2);               }
    | I_FETCH                       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "FETCH", 2);               }
    | I_FETCH eopr eoprs            { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                           }
;
/* Shift And Rotate Group */
inst_sr0:
      I_SR0 expr                    { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_SR0_SX, $2 ); }
    | I_SR0 eopr "," eoprs          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SR0", 1);  }
    | I_SR0                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SR0", 1);  }
;
inst_sr1:
      I_SR1 expr                    { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_SR1_SX, $2 ); }
    | I_SR1 eopr "," eoprs          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SR1", 1);  }
    | I_SR1                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SR1", 1);  }
;
inst_srx:
      I_SRX expr                    { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_SRX_SX, $2 ); }
    | I_SRX eopr "," eoprs          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SRX", 1);  }
    | I_SRX                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SRX", 1);  }
;
inst_sra:
      I_SRA expr                    { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_SRA_SX, $2 ); }
    | I_SRA eopr "," eoprs          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SRA", 1);  }
    | I_SRA                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SRA", 1);  }
;
inst_rr:
      I_RR expr                     { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_RR_SX, $2 ); }
    | I_RR eopr "," eoprs          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RR", 1);   }
    | I_RR                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RR", 1);   }
;
inst_sl0:
      I_SL0 expr                    { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_SL0_SX, $2 ); }
    | I_SL0 eopr "," eoprs          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SL0", 1);  }
    | I_SL0                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SL0", 1);  }
;
inst_sl1:
      I_SL1 expr                    { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_SL1_SX, $2 ); }
    | I_SL1 eopr "," eoprs          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SL1", 1);  }
    | I_SL1                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SL1", 1);  }
;
inst_slx:
      I_SLX expr                    { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_SLX_SX, $2 ); }
    | I_SLX eopr "," eoprs          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SLX", 1);  }
    | I_SLX                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SLX", 1);  }
;
inst_sla:
      I_SLA expr                    { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_SLA_SX, $2 ); }
    | I_SLA eopr "," eoprs          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SLA", 1);  }
    | I_SLA                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "SLA", 1);  }
;
inst_rl:
      I_RL expr                     { $$ = new CompilerStatement ( LOC(@$), ASMKCPSM3_INS_RL_SX, $2 ); }
    | I_RL eopr "," eoprs          { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RL", 1);   }
    | I_RL                         { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "RL", 1);   }
;
/* Input/Output Group */
inst_input:
      I_INPUT expr "," "#" expr     { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_INPUT_SX_PP,$2->appendLink($5));}
    | I_INPUT expr "," expr         { $$ = new CompilerStatement(LOC(@$),ASMKCPSM3_INS_INPUT_SX_SY,$2->appendLink($4));}
    | I_INPUT eopr "," eopr "," eoprs {/* Syntax Error */$$ = NULL; N_OPERANDS_EXPECTED(@1, "INPUT", 2);               }
    | I_INPUT eopr                  { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "INPUT", 2);               }
    | I_INPUT                       { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "INPUT", 2);               }
    | I_INPUT eopr eoprs            { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                           }
;
inst_output:
      I_OUTPUT expr "," "#" expr    { $$=new CompilerStatement(LOC(@$),ASMKCPSM3_INS_OUTPUT_SX_PP,$2->appendLink($5)); }
    | I_OUTPUT expr "," expr        { $$=new CompilerStatement(LOC(@$),ASMKCPSM3_INS_OUTPUT_SX_SY,$2->appendLink($4)); }
    | I_OUTPUT eopr "," eopr "," eoprs {/* Syntax Error */$$= NULL; N_OPERANDS_EXPECTED(@1, "OUTPUT", 2);              }
    | I_OUTPUT eopr                 { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@2, "OUTPUT", 2);              }
    | I_OUTPUT                      { /* Syntax Error */ $$ = NULL; N_OPERANDS_EXPECTED(@1, "OUTPUT", 2);              }
    | I_OUTPUT eopr eoprs           { /* Syntax Error */ $$ = NULL; MISSIGN_COMMA(@2, NULL);                           }
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison
inline int kcpsm3parser_error ( YYLTYPE * yylloc,
                                yyscan_t, 
                                CompilerParserInterface * compiler, 
                                const char * errorInfo )
{
    if ( 0 == strncmp(errorInfo , "syntax error, unexpected ", 25) )
    {
        std::string errStr;
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
