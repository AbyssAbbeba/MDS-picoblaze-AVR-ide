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
/* %expect 1063 */
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
%code requires
{
    #include "CompilerParserInterface.h"
}

// Symbol semantic value
%union
{
    int number;                     //
    char * string;                  //
    CompilerExpr * expr;            //
    CompilerStatement * stmt;       //
    struct
    {
        unsigned char * data;   //
        int size;               //
    } array;                        //
};

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
%token D_NAMEREG        D_ORG           D_CONSTANT      D_RADIX         D_WHILE
%token D_DATA           D_DB            D_ENDC          D_ERROR
%token D_LIST           D_MESSG         D_NOLIST        D_PAGE          D_SKIP
%token D_SUBTITLE       D_TITLE         D_EXPAND        D_NOEXPAND      D_IF
%token D_IFN            D_IFDEF         D_IFNDEF        D_ELSEIFB       D_ELSEIFNB
%token D_ELSE           D_ELSEIF        D_ELSEIFN       D_ELSEIFDEF     D_ELSEIFNDEF
%token D_ENDIF          D_LOCAL         D_IFNB          D_IFB           D_ENDMACRO
%token D_ENDM           D_EXITM         D_REPT          D_MACRO         D_EQU
%token D_END            D_REGISTER      D_CODE          D_ENDW          D_FILL
%token D_VARIABLE       D_SET

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
%token<string>  IDENFIFIER      LABEL
%token<array>   STRING          INCLUDE
  // semantic value is a number
%token<number>  NUMBER

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Expressions
%type<expr>     expr            "expression"
%type<expr>     aaa             "address"
%type<expr>     sX              "register X"
%type<expr>     sY              "register Y"
%type<expr>     kk              "constant expression"
%type<expr>     ss              "scratch pad memory address"
%type<expr>     pp              "port ID"
%type<expr>     number          params          args            opt             opts
%type<expr>     id              string          args_str
// Statements - general
%type<stmt>     statements      stmt            inst_stmt       dir_stmt        macro_stmt
%type<stmt>     instruction     directive       macro           label
// Statements - directives
%type<stmt>     dir_cond_asm    dir_skip        dir_local       dir_exitm       dir_list
%type<stmt>     dir_include     dir_repeat      dir_org         dir_end
%type<stmt>     dir_set         dir_equ         dir_macro       dir_if          dir_ifn
%type<stmt>     dir_ifdef       dir_ifndef      dir_ifb         dir_ifnb        dir_elseifnb
%type<stmt>     dir_elseif      dir_elseifn     dir_elseifdef   dir_elseifndf   dir_elseifb
%type<stmt>     dir_constant    dir_variable    dir_radix       dir_while
%type<stmt>     dir_while_a     dir_endw        dir_error
%type<stmt>     dir_data        dir_db
%type<stmt>     dir_fill        dir_messg
%type<stmt>     dir_subtitle    dir_title       dir_page        dir_nolist
%type<stmt>     dir_expand      dir_noexpand    dir_code
%type<stmt>     dir_code_a      if_block        ifelse_block
%type<stmt>     else_block      dir_else        dir_if_a        dir_ifn_a       dir_ifdef_a
%type<stmt>     dir_ifndef_a    dir_ifb_a       dir_ifnb_a      dir_elseif_a    dir_elseifn_a
%type<stmt>     dir_elseifdef_a dir_elseifndf_a dir_elseifb_a   dir_elseifnb_a  dir_rept
%type<stmt>     dir_endm        dir_macro_d     dir_macro_a
%type<stmt>     dir_data_a      dir_db_a
%type<stmt>     dir_fill_a      dir_page_a
%type<stmt>     dir_expand_a    dir_noexpand_a
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
      id                            { $$ = new CompilerStatement(LOC(), ASMKCPSM3_MACRO, $id); }
    | id "(" ")"                    { $$ = new CompilerStatement(LOC(), ASMKCPSM3_MACRO, $id); }
    | id args                       { $$ = new CompilerStatement(LOC(), ASMKCPSM3_MACRO, $id->appendLink($args)); }
    | id "(" args ")"               { $$ = new CompilerStatement(LOC(), ASMKCPSM3_MACRO, $id->appendLink($args)); }
;

/*
 * Generally used non-terminal symbols
 */
aaa:
      expr                          { $$ = $expr; }
;
sX:
      expr                          { $$ = $expr; }
;
sY:
      expr                          { $$ = $expr; }
;
kk:
      expr                          { $$ = $expr; }
;
ss:
      expr                          { $$ = $expr; }
;
pp:
      expr                          { $$ = $expr; }
;
number:
      NUMBER                        { $$ = new CompilerExpr($1, LOC()); }
;
id:
      IDENFIFIER                    { $$ = new CompilerExpr($1, LOC()); }
;
string:
      STRING                        { $$ = new CompilerExpr(CompilerExpr::Value($1.data, $1.size), LOC()); }
;
label:
      LABEL                         { $$ = new CompilerStatement(LOC(), ASMKCPSM3_LABEL, new CompilerExpr($1, LOC())); }
;
expr:
      id                            { $$ = $id; }
    | number                        { $$ = $number; }
    | "(" expr ")"                  { $$ = $2; }
    | F_LOW "(" expr ")"            { $$ = new CompilerExpr(CompilerExpr::OPER_LOW, $3, LOC()); }
    | F_HIGH "(" expr ")"           { $$ = new CompilerExpr(CompilerExpr::OPER_HIGH, $3, LOC()); }
    | F_BYTE2 "(" expr ")"          { $$ = new CompilerExpr("byte2", 'C', $3, LOC()); }
    | F_BYTE3 "(" expr ")"          { $$ = new CompilerExpr("byte3", 'C', $3, LOC()); }
    | F_BYTE4 "(" expr ")"          { $$ = new CompilerExpr("byte4", 'C', $3, LOC()); }
    | F_LWRD "(" expr ")"           { $$ = new CompilerExpr("lwrd", 'C', $3, LOC()); }
    | F_HWRD "(" expr ")"           { $$ = new CompilerExpr("hwrd", 'C', $3, LOC()); }
    | F_PAGE "(" expr ")"           { $$ = new CompilerExpr("page", 'C', $3, LOC()); }
    | F_EXP2 "(" expr ")"           { $$ = new CompilerExpr("exp2", 'C', $3, LOC()); }
    | F_LOG2 "(" expr ")"           { $$ = new CompilerExpr("log2", 'C', $3, LOC()); }
    | F_V "(" expr ")"              { $$ = new CompilerExpr("v", 'C', $3, LOC()); }
    | "~" expr                      { $$ = new CompilerExpr('~', $2, LOC()); }
    | "!" expr                      { $$ = new CompilerExpr('!', $2, LOC()); }
    | expr "+" expr                 { $$ = new CompilerExpr($1, '+', $3, LOC()); }
    | expr "-" expr                 { $$ = new CompilerExpr($1, '-', $3, LOC()); }
    | expr "*" expr                 { $$ = new CompilerExpr($1, '*', $3, LOC()); }
    | expr "/" expr                 { $$ = new CompilerExpr($1, '/', $3, LOC()); }
    | expr "%" expr                 { $$ = new CompilerExpr($1, '%', $3, LOC()); }
    | expr "||" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LOR, $3, LOC()); }
    | expr "^^" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LXOR, $3, LOC()); }
    | expr "&&" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LAND, $3, LOC()); }
    | expr "|" expr                 { $$ = new CompilerExpr($1, '|', $3, LOC()); }
    | expr "^" expr                 { $$ = new CompilerExpr($1, '^', $3, LOC()); }
    | expr "&" expr                 { $$ = new CompilerExpr($1, '&', $3, LOC()); }
    | expr "==" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_EQ, $3, LOC()); }
    | expr "!=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_NE, $3, LOC()); }
    | expr "<" expr                 { $$ = new CompilerExpr($1, '<', $3, LOC()); }
    | expr "<=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LE, $3, LOC()); }
    | expr ">=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_GE, $3, LOC()); }
    | expr ">" expr                 { $$ = new CompilerExpr($1, '>', $3, LOC()); }
    | expr ">>" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_SHR, $3, LOC()); }
    | expr "<<" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_SHL, $3, LOC()); }
    | error                         { $$ = NULL; }
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
    | dir_variable  { $$ = $1; }
    | dir_radix     { $$ = $1; }    | dir_while     { $$ = $1; }
    | dir_data      { $$ = $1; }
    | dir_db        { $$ = $1; }
    | dir_fill      { $$ = $1; }
    | dir_messg     { $$ = $1; }    | dir_subtitle  { $$ = $1; }
    | dir_title     { $$ = $1; }
    | dir_page      { $$ = $1; }
    | dir_expand    { $$ = $1; }    | dir_noexpand  { $$ = $1; }
    | dir_code      { $$ = $1; }
    | dir_error     { $$ = $1; }
;
dir_cond_asm:
      if_block ifelse_block else_block D_ENDIF {
        $$ = new CompilerStatement(LOC(), ASMKCPSM3_COND_ASM);
        $$->createBranch ( $if_block->appendLink($ifelse_block)->appendLink($else_block) );
    }
    | label if_block ifelse_block else_block D_ENDIF {
        $$ = $label->appendLink(new CompilerStatement(LOC(), ASMKCPSM3_COND_ASM));
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
      D_ELSE                        { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_ELSE); }
    | label D_ELSE                  {
                                        /* Syntax error */
                                        NO_LABEL_EXPECTED(@label, "ELSE", $label);
                                        $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_ELSE);
                                    }
    | label D_ELSE args             {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("ELSE", $args, @args);
                                        NO_LABEL_EXPECTED(@label, "ELSE", $label);
                                        $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_ELSE);
                                    }
    | D_ELSE args                   {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("ELSE", $args, @args);
                                        $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_ELSE);
                                    }
;
dir_if:
      dir_if_a EOL                  { $$ = $dir_if_a; }
    | dir_if_a COMMENT EOL          { $$ = $dir_if_a; }
;
dir_if_a:
      D_IF expr                     { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_IF, $expr); }
    | D_IF                          { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IF, "IF"); }
;
dir_ifn:
      dir_ifn_a EOL                 { $$ = $dir_ifn_a; }
    | dir_ifn_a COMMENT EOL         { $$ = $dir_ifn_a; }
;
dir_ifn_a:
      D_IFN expr                    { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_IFN, $expr); }
    | D_IFN                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFN, "IFN"); }
;
dir_ifdef:
      dir_ifdef_a EOL               { $$ = $dir_ifdef_a; }
    | dir_ifdef_a COMMENT EOL       { $$ = $dir_ifdef_a; }
;
dir_ifdef_a:
      D_IFDEF id                    { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_IFDEF, $id); }
    | D_IFDEF                       { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFDEF, "IFDEF"); }
;
dir_ifndef:
      dir_ifndef_a EOL              { $$ = $dir_ifndef_a; }
    | dir_ifndef_a COMMENT EOL      { $$ = $dir_ifndef_a; }
;
dir_ifndef_a:
      D_IFNDEF id                   { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_IFNDEF, $id); }
    | D_IFNDEF                      { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFNDEF, "IFNDEF"); }
;
dir_ifb:
      dir_ifb_a EOL                 { $$ = $dir_ifb_a; }
    | dir_ifb_a COMMENT EOL         { $$ = $dir_ifb_a; }
;
dir_ifb_a:
      D_IFB id                      { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_IFB, $id); }
    | D_IFB                         { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFB, "IFB"); }
;
dir_ifnb:
      dir_ifnb_a EOL                { $$ = $dir_ifnb_a; }
    | dir_ifnb_a COMMENT EOL        { $$ = $dir_ifnb_a; }
;
dir_ifnb_a:
      D_IFNB id                     { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_IFNB, $id); }
    | D_IFNB                        { /* Syntax error */ $$ = NULL; ARG_REQUIRED_D(@D_IFNB, "IFNB"); }
;
dir_elseif:
      dir_elseif_a EOL              { $$ = $dir_elseif_a; }
    | dir_elseif_a COMMENT EOL      { $$ = $dir_elseif_a; }
;
dir_elseif_a:
      D_ELSEIF expr                 { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_ELSEIF, $expr); }
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
      D_ELSEIFN expr                { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_ELSEIFN, $expr); }
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
      D_ELSEIFDEF expr              { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_ELSEIFDEF, $expr); }
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
      D_ELSEIFNDEF expr             { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_ELSEIFNDEF, $expr); }
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
      D_ELSEIFB expr                { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_ELSEIFB, $expr); }
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
      D_ELSEIFNB expr               { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_ELSEIFNB, $expr); }
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
      D_ORG expr                    { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_ORG, $expr); }
    | label D_ORG expr              { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMKCPSM3_DIR_ORG, $expr)); }
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
      D_SKIP expr                   { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_SKIP, $expr); }
    | label D_SKIP expr             { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_SKIP, $expr)); }
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
      D_LOCAL id                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_LOCAL, $id); }
    | label D_LOCAL id              { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_LOCAL, $id)); }
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
      D_EXITM                       { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_EXITM); }
    | label D_EXITM                 { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_EXITM)); }
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
dir_nolist:
      D_NOLIST                      { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_NOLIST); }
    | label D_NOLIST                { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_NOLIST)); }
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
      id D_SET expr                 { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_SET, $id->appendLink($expr)); }
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
      id D_EQU expr                 { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_EQU, $id->appendLink($expr)); }
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
      id D_MACRO                    { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_MACRO, $id); }
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
      D_END                         { $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_END); }
    | label D_END                   { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMKCPSM3_DIR_END)); }
    | D_END args                    {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("END", $args, @args);
                                        $$ = new CompilerStatement(LOC(), ASMKCPSM3_DIR_END);
                                    }
    | label D_END args              {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("END", $args, @args);
                                        $$ = $label->appendLink(new CompilerStatement(LOC(), ASMKCPSM3_DIR_END));
                                    }
;
dir_constant:
      D_CONSTANT id "=" expr        { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_CONSTANT, $id->appendLink($expr)); }
    | label D_CONSTANT id "=" expr  {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(),
                                                                                            ASMPIC8_DIR_CONSTANT, $id -> appendLink ( $expr ) ) );
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
      D_VARIABLE id "=" expr        { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_VARIABLE, $id->appendLink($expr)); }
    | label D_VARIABLE id "=" expr  {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(),
                                                                                            ASMPIC8_DIR_VARIABLE, $id -> appendLink ( $expr ) ) );
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
dir_radix:
      D_RADIX id                    { $$ = new CompilerStatement(LOC(), ASMPIC8_DIR_RADIX, $id); }
    | label D_RADIX id              { $$ = $label->appendLink(new CompilerStatement(LOC(), ASMPIC8_DIR_RADIX, $id)); }
    | D_RADIX                       {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_RADIX, "RADIX");
                                    }
    | label D_RADIX                 {
                                        /* Syntax error */
                                        $$ = NULL;
                                        ARG_REQUIRED_D(@D_RADIX, "RADIX");
                                        $label->completeDelete();
                                    }
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
      I_JUMP aaa                    { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_JUMP_AAA,    $aaa ); }
    | I_JUMP Z "," aaa              { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_JUMP_Z_AAA,  $aaa ); }
    | I_JUMP NZ "," aaa             { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_JUMP_NZ_AAA, $aaa ); }
    | I_JUMP C "," aaa              { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_JUMP_C_AAA,  $aaa ); }
    | I_JUMP NC "," aaa             { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_JUMP_NC_AAA, $aaa ); }
;
inst_call:
      I_CALL aaa                    { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_CALL_AAA,    $aaa ); }
    | I_CALL Z "," aaa              { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_CALL_Z_AAA,  $aaa ); }
    | I_CALL NZ "," aaa             { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_CALL_NZ_AAA, $aaa ); }
    | I_CALL C "," aaa              { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_CALL_C_AAA,  $aaa ); }
    | I_CALL NC "," aaa             { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_CALL_NC_AAA, $aaa ); }
;
inst_return:
      I_RETURN                      { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_RETURN    ); }
    | I_RETURN Z                    { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_RETURN_Z  ); }
    | I_RETURN NZ                   { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_RETURN_NZ ); }
    | I_RETURN C                    { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_RETURN_C  ); }
    | I_RETURN NC                   { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_RETURN_NC ); }
;
/* Arithmetic Group */
inst_add:
      I_ADD sX "," kk               { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_ADD_SX_KK, $sX->appendLink($kk) ); }
    | I_ADD sX "," sY               { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_ADD_SX_SY, $sX->appendLink($sY) ); }
;
inst_addcy:
      I_ADDCY sX "," kk             { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_ADDCY_SX_KK, $sX->appendLink($kk) ); }
    | I_ADDCY sX "," sY             { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_ADDCY_SX_SY, $sX->appendLink($sY) ); }
;
inst_sub:
      I_SUB sX "," kk               { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_SUB_SX_KK, $sX->appendLink($kk) ); }
    | I_SUB sX "," sY               { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_SUB_SX_SY, $sX->appendLink($sY) ); }
;
inst_subcy:
      I_SUBCY sX "," kk             { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_SUBCY_SX_KK, $sX->appendLink($kk) ); }
    | I_SUBCY sX "," sY             { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_SUBCY_SX_SY, $sX->appendLink($sY) ); }
;
inst_compare:
      I_COMPARE sX "," kk           { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_COMPARE_SX_KK, $sX->appendLink($kk) ); }
    | I_COMPARE sX "," sY           { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_COMPARE_SX_SY, $sX->appendLink($sY) ); }
;
/* Interrupt Group */
inst_returni:
    I_RETURNI_ENA                   { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_RETURNI_ENA ); }
  | I_RETURNI_DIS                   { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_RETURNI_DIS ); }
;
inst_enable_int:
      I_ENABLE_INT                  { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_ENABLE_INT ); }
;
inst_disable_int:
      I_DISABLE_INT                 { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_DISABLE_INT ); }
;
/* Logical Group */
inst_load:
      I_LOAD sX "," kk              { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_LOAD_SX_KK, $sX->appendLink($kk) ); }
    | I_LOAD sX "," sY              { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_LOAD_SX_SY, $sX->appendLink($sY) ); }
;
inst_and:
      I_AND sX "," kk               { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_AND_SX_KK, $sX->appendLink($kk) ); }
    | I_AND sX "," sY               { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_AND_SX_SY, $sX->appendLink($sY) ); }
;
inst_or:
      I_OR sX "," kk                { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_OR_SX_KK, $sX->appendLink($kk) ); }
    | I_OR sX "," sY                { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_OR_SX_SY, $sX->appendLink($sY) ); }
;
inst_xor:
      I_XOR sX "," kk               { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_XOR_SX_KK, $sX->appendLink($kk) ); }
    | I_XOR sX "," sY               { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_XOR_SX_SY, $sX->appendLink($sY) ); }
;
inst_test:
      I_TEST sX "," kk              { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_TEST_SX_KK, $sX->appendLink($kk) ); }
    | I_TEST sX "," sY              { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_TEST_SX_SY, $sX->appendLink($sY) ); }
;
/* Storage Group */
inst_store:
      I_STORE sX "," ss             { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_STORE_SX_SS, $sX->appendLink($ss) ); }
    | I_STORE sX "," "(" sY ")"     { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_STORE_SX_SY, $sX->appendLink($sY) ); }
;
inst_fetch:
      I_FETCH sX "," ss             { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_FETCH_SX_SS, $sX->appendLink($ss) ); }
    | I_FETCH sX "," "(" sY ")"     { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_FETCH_SX_SY, $sX->appendLink($sY) ); }
;
/* Shift And Rotate Group */
inst_sr0:
      I_SR0 sX                      { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_SR0_SX, $sX ); }
;
inst_sr1:
      I_SR1 sX                      { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_SR1_SX, $sX ); }
;
inst_srx:
      I_SRX sX                      { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_SRX_SX, $sX ); }
;
inst_sra:
      I_SRA sX                      { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_SRA_SX, $sX ); }
;
inst_rr:
      I_RR sX                       { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_RR_SX, $sX ); }
;
inst_sl0:
      I_SL0 sX                      { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_SL0_SX, $sX ); }
;
inst_sl1:
      I_SL1 sX                      { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_SL1_SX, $sX ); }
;
inst_slx:
      I_SLX sX                      { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_SLX_SX, $sX ); }
;
inst_sla:
      I_SLA sX                      { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_SLA_SX, $sX ); }
;
inst_rl:
      I_RL sX                       { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_RL_SX, $sX ); }
;
/* Input/Output Group */
inst_input:
      I_INPUT sX "," pp             { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_INPUT_SX_PP, $sX->appendLink($pp) ); }
    | I_INPUT sX "," "(" sY ")"     { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_INPUT_SX_SY, $sX->appendLink($sY) ); }
;
inst_output:
      I_OUTPUT sX "," pp            { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_OUTPUT_SX_PP, $sX->appendLink($pp) ); }
    | I_OUTPUT sX "," "(" sY ")"    { $$ = new CompilerStatement ( LOC(), ASMKCPSM3_INS_OUTPUT_SX_SY, $sX->appendLink($sY) ); }
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
