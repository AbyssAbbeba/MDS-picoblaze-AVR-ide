// =============================================================================
/**
 * @brief
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 */
// =============================================================================


// -----------------------------------------------------------------------------
// PROLOGUE - BISON DECLARATIONS
// -----------------------------------------------------------------------------

// Require version <version> or higher of Bison.
%require "2.5"
// Rename the external symbols used in the parser so that they start with prefix instead of ‘yy’.
%name-prefix "CompilerCPreProcCalcPar_"
// Generate a parser header file containing macro definitions, etc.
%defines "CompilerCPreProcCalcPar.h"
// Generate a pure, reentrant parser.
%define api.pure
// Generate the code processing the locations.
%locations
// Specify the programming language for the generated parser.
%language "C"
// Write an extra output file containing verbose descriptions of the parser states.
%verbose
// Expect exactly <n> shift/reduce conflicts in this grammar.
%expect 0
// Expect exactly <n> reduce/reduce conflicts in this grammar.
%expect-rr 0
// Bison declaration to request verbose, specific error message strings when yyerror is called.
%error-verbose
// Enable LAC (Lookahead Correction) to improve syntax error handling.
%define parse.lac full

// Symbol semantic value.
%union
{
    int64_t number;
};


/*
 * Interpreter's interface for the syntax analyzer; we need to have some things declared
 * before we can declare other things.
 */
%code requires
{
    // Compiler preprocessor interface.
    #include "CompilerCPreprocessorIntr.h"

    #ifndef YY_TYPEDEF_YY_SCANNER_T
    #define YY_TYPEDEF_YY_SCANNER_T
        typedef void* yyscan_t;
    #endif // YY_TYPEDEF_YY_SCANNER_T
}

%{
    // Standard header files.
    #include <cmath>
    #include <string>
    #include <cstdlib>
    #include <cstring>
    #include <cstdint>
    #include <iostream>

    using namespace std;

    // For i18n only
    #include <QObject>

    // Declaration of the lexer prototypes and other things required by Bison.
    #include "CompilerCPreProcCalcLex.h"

    // Name of the lexer func required by Bison.
    #define CompilerCPreProcCalcPar_lex CompilerCPreProcCalcLex_lex

    // Declaration of the error reporting function used by Bison.
    inline int CompilerCPreProcCalcPar_error ( YYLTYPE * yylloc,
                                               yyscan_t yyscanner,
                                               CompilerCPreprocessorIntr * preprocessor,
                                               const char * errorInfo );
%}

// Declare an additional yyparse parameters.
%parse-param { yyscan_t yyscanner }
%parse-param { CompilerCPreprocessorIntr * preprocessor }

// Declare an additional yylex parameters.
%lex-param   { yyscan_t yyscanner }

// Declare code which has to be invoked before parsing actually begins.
%initial-action
{
    @$.first_column = @$.last_column = 1;
    @$.first_line   = @$.last_line   = 1;
};

/*
 * DECLARATION OF THE GRAMMAR APLHABET - TERMINAL SYMBOLS
 */
%token P_LEFT           "("
%token P_RIGHT          ")"

%token O_COLON          ":"
%token O_QUESTION_MARK  "?"
%token O_COMMA          ","
%token O_SLASH          "/"
%token O_PLUS           "+"
%token O_MINUS          "-"
%token O_ASTERISK       "*"
%token O_NOT            "!"
%token O_MOD            "%"
%token O_SHL            "<<"
%token O_SHR            ">>"
%token O_AND            "&&"
%token O_OR             "||"
%token O_BITAND         "&"
%token O_BITOR          "|"
%token O_BITXOR         "^"
%token O_EQ             "=="
%token O_NE             "!="
%token O_LT             "<"
%token O_LE             "<="
%token O_GT             ">"
%token O_GE             ">="
%token O_BITNOT         "~"
%token O_SIZEOF         "sizeof"

%token D_CHAR           "char"
%token D_CONST          "const"
%token D_DOUBLE         "double"
%token D_FLOAT          "float"
%token D_INT            "int"
%token D_LONG           "long"
%token D_SHORT          "short"
%token D_SIGNED         "signed"
%token D_UNSIGNED       "unsigned"
%token D_VOID           "void"

/* Operator precedence (the one declared later has the higher precedence). */
%left ","
%right "?" ":"
%left "||"
%left "&&"
%left "|"
%left "^"
%left "&"
%left "==" "!="
%left ">=" ">"
%left "<" "<="
%left "<<" ">>"
%left "+" "-"
%left "*" "/" "%"
%right "~" "!"
%right UPLUS UMINUS
%left "(" ")"

/* Terminal symbols with semantic value. */
%token<number> NUMBER "number"

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
%type<number> expr dt dt_ptr dt_int dt_int_basic dt_float

// The start symbol.
%start input

// -----------------------------------------------------------------------------
// GRAMMAR RULES
// -----------------------------------------------------------------------------

%%

// The start symbol.
input:
      expr                          {
                                        // result ready
                                        preprocessor->m_exprResult = (bool) $expr;
                                        YYACCEPT;
                                    }
    | /* empty */                   {
                                        // TODO: error, empty expression
                                        preprocessor->m_exprResult = false;
                                        YYACCEPT;
                                    }
;

// Expression.
expr:
    // Single value expressions.
      NUMBER                        { $$ = $NUMBER; }

    // Parentheses and comma.
    | "(" expr ")"                  { $$ = $2; }
    | expr "," expr                 {
                                        // TODO: warning, comma in expression
                                        $$ = $3;
                                    }

    // Binary operators.
    | expr "/" expr                 {
                                        if ( 0 == $3 )
                                        {
                                            $$ = 0;
                                            // TODO: error, dicision by zero
                                        }
                                        else
                                        {
                                            $$ = $1 / $3;
                                        }

                                    }
    | expr "%" expr                 {
                                        if ( 0 == $3 )
                                        {
                                            $$ = 0;
                                            // TODO: error, dicision by zero
                                        }
                                        else
                                        {
                                            $$ = $1 % $3;
                                        }

                                    }
    | expr "+" expr                 { $$ = $1 + $3; }
    | expr "-" expr                 { $$ = $1 - $3; }
    | expr "*" expr                 { $$ = $1 * $3; }
    | expr "<<" expr                { $$ = $1 << $3; }
    | expr ">>" expr                { $$ = $1 >> $3; }
    | expr "&&" expr                { $$ = $1 && $3; }
    | expr "||" expr                { $$ = $1 || $3; }
    | expr "&" expr                 { $$ = $1 & $3; }
    | expr "|" expr                 { $$ = $1 | $3; }
    | expr "^" expr                 { $$ = $1 ^ $3; }
    | expr "==" expr                { $$ = $1 == $3; }
    | expr "!=" expr                { $$ = $1 != $3; }
    | expr "<" expr                 { $$ = $1 < $3; }
    | expr "<=" expr                { $$ = $1 <= $3; }
    | expr ">" expr                 { $$ = $1 > $3; }
    | expr ">=" expr                { $$ = $1 >= $3; }

    // Unary operators.
    | "~" expr                      { $$ = ~ $2; }
    | "!" expr                      { $$ = ! $2; }
    | "+" expr  %prec UPLUS         { $$ =   $2; }
    | "-" expr  %prec UMINUS        { $$ = - $2; }
    | "sizeof" "(" dt ")"           { $$ =   $dt; }


    // Ternary operator.
    | expr "?" expr ":" expr        { $$ = $1 ? $3 : $5; }
;

// Datatypes
dt:
      dt_const dt_int               { $$ = $dt_int; }
    | dt_const dt_float             { $$ = $dt_float; }
    | dt_const dt_ptr               { $$ = $dt_ptr; }
;

dt_ptr:
      "void" dt_const ptr ref       { $$ = 1; }
    | dt_int dt_const ptr ref       { $$ = 1; }
    | dt_float dt_const ptr ref     { $$ = 1; }
;

ptr:
      "*"
    | ptr "*"
;

ref:
      /* empty */
    | "&"
;

dt_const:
      /* empty */
    | "const"
;

dt_signed:
      /* empty */
    | "signed"
    | "unsigned"
;

dt_int:
    dt_signed dt_int_basic          { $$ = $dt_int_basic; }
;

dt_int_basic:
      "char"                        {  $$ = 1; }
    | "short"                       {  $$ = 1; }
    | "int"                         {  $$ = 2; }
    | "long"                        {  $$ = 4; }
    | "long" "int"                  {  $$ = 4; }
    | "long" "long" "int"           {  $$ = 8; }
;

dt_float:
      "float"                       {  $$ = 4; }
    | "double"                      {  $$ = 8; }
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison.
inline int CompilerCPreProcCalcPar_error ( YYLTYPE * yylloc,
                                           yyscan_t,
                                           CompilerCPreprocessorIntr * preprocessor,
                                           const char * errorInfo )
{
    preprocessor->m_compilerCore->parserMessage ( preprocessor->m_compilerCore->toSourceLocation(yylloc),
                                                  CompilerBase::MT_ERROR,
                                                  errorInfo );

    return 0;
}
