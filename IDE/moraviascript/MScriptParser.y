// =============================================================================
/**
 * @brief
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
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
%name-prefix "MScriptParser_"
// Generate a parser header file containing macro definitions, etc.
%defines "MScriptParser.h"
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
// Type of parser tables within the LR family, in this case we use LALR (Look-Ahead LR parser).
%define lr.type lalr
// Bison declaration to request verbose, specific error message strings when yyerror is called.
%error-verbose
// Enable LAC (Lookahead Correction) to improve syntax error handling.
%define parse.lac full

// Symbol semantic value.
%union
{
    MScriptExpr * expr;         //
    MScriptStatement * stmt;    //

    bool boolean;               //
    long long integer;          //
    double real;                //
    double imaginary;           //
    char * symbol;              //

    struct
    {
        char * data;            //
        unsigned int size;      //
    } string;                   //
};

/*
 * Interpreter's interface for the syntax analyzer; we need to have some things declared
 * before we can declare other things.
 */
%code requires
{
    #include "MScriptParserInterface.h"
}

%{
    #include <QObject>                // For i18n only

    #include <cmath>
    #include <string>
    #include <cstdlib>
    #include <cstring>
    #include <iostream>

    using namespace std;              // This is required by Bison as well.
    using namespace MScriptStmtTypes; // This NS is heavily used here.

    // Declaration of the lexer prototypes and other things required by Bison.
    #include "MScriptLexer.h"

    // Name of the lexer func required by Bison.
    #define MScriptParser_lex MScriptLexer_lex

    // Declaration of the error reporting function used by Bison.
    inline int MScriptParser_error ( YYLTYPE * yylloc,
                                     yyscan_t yyscanner,
                                     MScriptParserInterface * core,
                                     const char * errorInfo );
%}

// Declare an additional yyparse parameters.
%parse-param { yyscan_t yyscanner }
%parse-param { MScriptParserInterface * core }

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

/* Language key words. */
%token KW_IF            "if"
%token KW_ELSE          "else"
%token KW_FOR           "for"
%token KW_WHILE         "while"
%token KW_FUNC          "func"
%token KW_DO            "do"
%token KW_RETURN        "return"
%token KW_BREAK         "break"
%token KW_CONTINUE      "continue"
%token KW_SWITCH        "switch"
%token KW_CASE          "case"
%token KW_DEFAULT       "default"
%token KW_DELETE        "delete"
%token KW_VAR           "var"
%token KW_CONST         "const"
%token KW_NAMESPACE     "namespace"
%token KW_INCLUDE       "include"
%token KW_EVAL          "eval"
%token KW_MESSAGE       "message"
%token KW_WARNING       "warning"
%token KW_ABORT         "abort"

/* Special values: */
%token SV_EMPTY         "EMPTY"
%token SV_INFINITY      "INFINITY"
%token SV_NAN           "NAN"
%token SV_PI            "PI"
%token SV_E             "E"

/* Built-in functions: */
 // Datatype recognition.
%token F_ISBOOL         "isbool"
%token F_ISINT          "isint"
%token F_ISSTRING       "isstring"
%token F_ISFLOAT        "isfloat"
%token F_ISCOMPLEX      "iscomplex"
%token F_ISEMPTY        "isempty"
 // Datatype conversion.
%token F_BOOL           "bool"
%token F_INT            "int"
%token F_STRING         "f_string"
%token F_FLOAT          "float"
%token F_COMPLEX        "complex"
 // Floating point value classification.
%token F_ISNAN          "isnan"
%token F_ISINFINITY     "isinfinity"
%token F_ISPOSITIVE     "ispositive"
%token F_ISNEGATIVE     "isnegative"
%token F_ISFINITE       "isfinite"
 // Complex plane decomposition.
%token F_RE             "Re"
%token F_IM             "Im"
 // Trigonometric functions.
%token F_SIN            "sin"
%token F_COS            "cos"
%token F_TAN            "tan"
// Inverse trigonometric functions.
%token F_ARCSIN         "arcsin"
%token F_ARCCOS         "arccos"
%token F_ARCTAN         "arctan"
// Hyperbolic functions.
%token F_SINH           "sinh"
%token F_COSH           "cosh"
%token F_TANH           "tanh"
// Inverse hyperbolic functions.
%token F_ARCSINH        "arcsinh"
%token F_ARCCOSH        "arccosh"
%token F_ARCTANH        "arctanh"
 // Other functions.
%token F_CEIL           "ceil"
%token F_ROUND          "round"
%token F_FLOOR          "floor"
%token F_ABS            "abs"
%token F_MIN            "min"
%token F_MAX            "max"
%token F_LENGTH         "length"

/* Other terminal symbols */
%token O_COLON          ":"
%token O_QUESTION_MARK  "?"
%token EOS              ";"
%token END      0       "end of file"
// Brackets.
%token B_RND_LEFT       "("
%token B_RND_RIGHT      ")"
%token B_SQR_LEFT       "["
%token B_SQR_RIGHT      "]"
%token B_CRL_LEFT       "{"
%token B_CRL_RIGHT      "}"
// Expression operators.
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
%token O_ASSIGN         "="
%token O_COMPLEMET      "~"
%token O_ADD_ASSIGN     "+="
%token O_SUB_ASSIGN     "-="
%token O_MUL_ASSIGN     "*="
%token O_DIV_ASSIGN     "/="
%token O_MOD_ASSIGN     "%="
%token O_SHL_ASSIGN     "<<="
%token O_SHR_ASSIGN     ">>="
%token O_BAND_ASSIGN    "&="
%token O_BOR_ASSIGN     "|="
%token O_LAND_ASSIGN    "&&="
%token O_LOR_ASSIGN     "||="
%token O_XOR_ASSIGN     "^="
%token O_POW_ASSIGN     "**="
%token O_INCREMENT      "++"
%token O_DECREMENT      "--"
%token O_POWER          "**"

/* Operator precedence (the one declared later has the higher precedence). */
%left ","
%right "&=" "^=" "|=" "&&=" "||="
%right "<<=" ">>="
%right "**="
%right "*=" "/=" "%="
%right "+=" "-="
%right "="
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
%left "**"
%right "~" "!"
%right UPLUS UMINUS
%right "++" "--"
%left "[" "]"
%left FCALL
%left POST_INC POST_DEC
%left "(" ")"

/* Precedence for non operators, placed here to solve certain shift/reduce conflicts. */
%nonassoc "else"

/* Terminal symbols with semantic value. */
%token<symbol>    IDENTIFIER    "idenfifier"
%token<symbol>    NSID          "idenfifier with namespace"
%token<string>    STRING        "string"
%token<integer>   INTEGER       "integer"
%token<real>      REAL          "real number"
%token<imaginary> IMAGINARY     "imaginary unit"
%token<boolean>   BOOLEAN       "boolean number"

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Expressions.
%type<expr>     expr            e_expr          e_int           id              sid             string
%type<expr>     param           param_list      decl            decl_list       indexes         native_f
// Statements - general.
%type<stmt>     statements      stmt            cases           switch_body     scope

// Each time the parser discards symbol with certain semantic types, their memory have to bee freed.
%destructor
{
    if ( nullptr != $$ )
    {
        $$->completeDelete();
    }
} <expr>

%destructor
{
    if ( nullptr != $$ )
    {
        $$->completeDelete();
    }
} <stmt>

%destructor
{
    free($$.data);
} <array>

// The start symbol.
%start input

// -----------------------------------------------------------------------------
// GRAMMAR RULES
// -----------------------------------------------------------------------------

%%

/*
 * Basic code structure
 */

// The start symbol, i.e. the entire program code.
input:
      statements                    {
                                        core->syntaxAnalysisComplete($statements);
                                        YYACCEPT;
                                    }
    | /* empty */                   {
                                        core->syntaxAnalysisComplete(nullptr);
                                        YYACCEPT;
                                    }
;

// Sequence of statements.
statements:
      stmt                          { $$ = $stmt;                 }
    | statements stmt               { $$ = $1->appendLink($stmt); }
;

// Single statement.
stmt:
      ";"                           {
                                        $$ = nullptr;
                                    }
    | error                         {
                                        $$ = nullptr;
                                    }
    | scope                         {
                                        $$ = $scope;
                                    }
    | expr ";"                      {
                                        $$ = new MScriptStatement(@$, STMT_EXPR, $expr);
                                    }
    | "return" e_expr ";"           {
                                        $$ = new MScriptStatement(@$, STMT_RETURN, $e_expr);
                                    }
    | "continue" e_int ";"          {
                                        $$ = new MScriptStatement(@$, STMT_CONTINUE, $e_int);
                                    }
    | "break" e_int ";"             {
                                        $$ = new MScriptStatement(@$, STMT_BREAK, $e_int);
                                    }
    | "delete" id ";"               {
                                        $$ = new MScriptStatement(@$, STMT_DELETE, $id);
                                    }
    | "var" decl_list ";"           {
                                        $$ = new MScriptStatement(@$, STMT_VAR, $decl_list);
                                    }
    | "const" decl_list ";"         {
                                        $$ = new MScriptStatement(@$, STMT_CONST, $decl_list);
                                    }
    | "include" string ";"          {
                                        $$ = new MScriptStatement(@$, STMT_INCLUDE, $string);
                                    }
    | "eval" "(" string ")" ";"     {
                                        $$ = new MScriptStatement(@$, STMT_EVAL, $string);
                                    }
    | "message" "(" string ")" ";"  {
                                        $$ = new MScriptStatement(@$, STMT_MESSAGE, $string);
                                    }
    | "warning" "(" string ")" ";"  {
                                        $$ = new MScriptStatement(@$, STMT_WARNING, $string);
                                    }
    | "abort" "(" string ")" ";"    {
                                        $$ = new MScriptStatement(@$, STMT_ABORT, $string);
                                    }
    | "namespace" id "{" statements "}"
                                    {
                                        $$ = new MScriptStatement(@$, STMT_NAMESPACE, $id);
                                        $$->createBranch($statements);
                                    }
    | "namespace" id "{" /* empty */ "}"
                                    {
                                        $$ = new MScriptStatement(@$, STMT_NAMESPACE, $id);
                                        core->parserMessage ( @$,
                                                              MScriptBase::MT_WARNING,
                                                              QObject::tr("empty namespace").toStdString() );
                                    }
    | "/" expr "/" stmt             {
                                        $$ = new MScriptStatement(@$, STMT_TRIGGER, $expr);
                                        $$->createBranch($4);
                                    }
    | "func" id "(" param_list ")" scope
                                    {
                                        $$ = new MScriptStatement(@$, STMT_FUNCTION, $id->appendLink($param_list));
                                        $$->createBranch($scope);
                                    }
    | "if" "(" expr ")" stmt        {
                                        MScriptStatement * ifBlock = new MScriptStatement(@1, STMT_IF, $expr);
                                        ifBlock->createBranch($5);

                                        $$ = new MScriptStatement(@$, STMT_CONDITION);
                                        $$->createBranch(ifBlock);
                                    }
    | "if" "(" expr ")" stmt "else" stmt
                                    {
                                        MScriptStatement * ifBlock = new MScriptStatement(@1, STMT_IF, $3);
                                        ifBlock->createBranch($5);

                                        MScriptStatement * elseBlock = new MScriptStatement(@6, STMT_ELSE);
                                        elseBlock->createBranch($7);

                                        $$ = new MScriptStatement(@$, STMT_CONDITION);
                                        $$->createBranch(ifBlock->appendLink(elseBlock));
                                    }
    | "for" "(" e_expr ";" e_expr ";" e_expr ")" stmt
                                    {
                                        $$ = new MScriptStatement(@$, STMT_FOR, $3->appendLink($5)->appendLink($7));
                                        $$->createBranch($9);
                                    }
    | "while" "(" expr ")" stmt     {
                                        $$ = (new MScriptStatement(@$, STMT_WHILE, $expr))->createBranch($5);
                                    }
    | "do" stmt "while" "(" expr ")" ";"
                                    {
                                        $$ =(new MScriptStatement(@$, STMT_DO_WHILE, $expr))->createBranch($2);
                                    }
    | "switch" "(" expr ")" "{" switch_body "}"
                                    {
                                        $$ = new MScriptStatement(@$, STMT_SWITCH, $expr);
                                        $$->createBranch($switch_body);
                                    }
    | "switch" "(" expr ")" "{" /* empty */ "}"
                                    {
                                        // Empty scope -> optimize by rewriting it to expression statement.
                                        $$ = new MScriptStatement(@$, STMT_EXPR, $expr);
                                        core->parserMessage ( @$,
                                                              MScriptBase::MT_WARNING,
                                                              QObject::tr("empty switch body").toStdString() );
                                    }
;

// `switch' statement body.
switch_body:
      cases statements                    { $$ = $cases->createBranch($statements);                 }
    | switch_body cases statements        { $$ = $1->appendLink($cases->createBranch($statements)); }
;

// Sequence of `case:' and/or `default:' statements, used inside `switch' statement.
cases:
      "case" expr ":"               { $$ = new MScriptStatement(@$, STMT_CASE, $expr); }
    | "default" ":"                 { $$ = new MScriptStatement(@$, STMT_DEFAULT);     }
    | cases "case" expr ":"         { $$ = $1->appendArgsLink($expr);                  }
    | cases "default" ":"           {
                                        $1->completeDelete();
                                        $$ = new MScriptStatement(@$, STMT_DEFAULT);
                                    }
;

// Scope.
scope:
      "{" statements "}"            {
                                        $$ = new MScriptStatement(@$, STMT_SCOPE);
                                        $$->createBranch($statements);
                                    }
    | "{" /* empty */ "}"           {
                                        // Empty scope.
                                        $$ = new MScriptStatement(@$, STMT_SCOPE);
                                        core->parserMessage ( @$,
                                                              MScriptBase::MT_WARNING,
                                                              QObject::tr("empty scope").toStdString() );
                                    }
;

// Simple identifier (without namespaces).
sid:
      IDENTIFIER                    { $$ = new MScriptExpr($IDENTIFIER, @$); }
;

// Arbitrary identifier.
id:
      sid                           { $$ = $sid;                       }
    | NSID                          { $$ = new MScriptExpr($NSID, @$); }
;

// String constant.
string:
      STRING                        { $$ = new MScriptExpr(MScriptValue($STRING.data, $STRING.size), @$); }
;

// Variable or constant declarations.
decl:
      id                            { $$ = $id;                                                       }
    | id "=" expr                   { $$ = new MScriptExpr($id, MScriptExpr::OPER_ASSIGN, $expr, @$); }
;

// List of variable or constant declarations.
decl_list:
      decl                          { $$ = $decl;                 }
    | decl_list "," decl            { $$ = $1->appendLink($decl); }
;

param:
      sid                           { $$ = $sid;                                                       }
    | sid "=" expr                  { $$ = new MScriptExpr($sid, MScriptExpr::OPER_ASSIGN, $expr, @$); }
    | "&" sid                       { $$ = $sid; $sid->m_operator = MScriptExpr::OPER_REF;             }
;

// List of function parameters.
param_list:
      /* empty */                   { $$ = nullptr;                   }
    | param                         { $$ = $param;                 }
    | param_list "," param          { $$ = $1->appendLink($param); }
;

// Integer, possibly empty.
e_int:
      /* empty */                   { $$ = nullptr;                          }
    | INTEGER                       { $$ = new MScriptExpr($INTEGER, @$); }
;

// Expression, possibly empty.
e_expr:
      /* empty */                   { $$ = nullptr;  }
    | expr                          { $$ = $expr; }
;


// Expression.
expr:
    // Single value expressions.
      id                            { $$ = $id;                                              }
    | string                        { $$ = $string;                                          }
    | BOOLEAN                       { $$ = new MScriptExpr($BOOLEAN, @$);                    }
    | REAL                          { $$ = new MScriptExpr($REAL, @$);                       }
    | INTEGER                       { $$ = new MScriptExpr($INTEGER, @$);                    }
    | IMAGINARY                     { $$ = new MScriptExpr(MScriptValue(0, $IMAGINARY), @$); }

    // Built-in constants.
    | "EMPTY"                       { $$ = new MScriptExpr(@$);                              }
    | "INFINITY"                    { $$ = new MScriptExpr(INFINITY, @$);                    }
    | "NAN"                         { $$ = new MScriptExpr(NAN, @$);                         }
    | "PI"                          { $$ = new MScriptExpr(3.1415926535897932384d, @$);      }
    | "E"                           { $$ = new MScriptExpr(2.71828182845904523536d, @$);     }

    // Parentheses.
    | "(" expr ")"                  { $$ = $2; }

    // Binary operators.
    | expr "+" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_ADD,  $3, @$);        }
    | expr "-" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_SUB,  $3, @$);        }
    | expr "*" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_MULT, $3, @$);        }
    | expr "/" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_DIV,  $3, @$);        }
    | expr "%" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_MOD,  $3, @$);        }
    | expr "|" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_BOR,  $3, @$);        }
    | expr "^" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_BXOR, $3, @$);        }
    | expr "&" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_BAND, $3, @$);        }
    | expr "<" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_LT,   $3, @$);        }
    | expr ">" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_GT,   $3, @$);        }
    | expr "||" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_LOR,  $3, @$);        }
    | expr "&&" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_LAND, $3, @$);        }
    | expr "==" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_EQ,   $3, @$);        }
    | expr "!=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_NE,   $3, @$);        }
    | expr "<=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_LE,   $3, @$);        }
    | expr ">=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_GE,   $3, @$);        }
    | expr ">>" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_SHR,  $3, @$);        }
    | expr "<<" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_SHL,  $3, @$);        }
    | expr "**" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_POW,  $3, @$);        }
    | expr "="  expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_ASSIGN,      $3, @$); }
    | expr "=" "&" expr             { $$ = new MScriptExpr($1, MScriptExpr::OPER_ASSIGN_REF,  $4, @$); }
    | expr "+=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_ADD_ASSIGN,  $3, @$); }
    | expr "-=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_SUB_ASSIGN,  $3, @$); }
    | expr "*=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_MUL_ASSIGN,  $3, @$); }
    | expr "/=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_DIV_ASSIGN,  $3, @$); }
    | expr "%=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_MOD_ASSIGN,  $3, @$); }
    | expr "<<=" expr               { $$ = new MScriptExpr($1, MScriptExpr::OPER_SHL_ASSIGN,  $3, @$); }
    | expr ">>=" expr               { $$ = new MScriptExpr($1, MScriptExpr::OPER_SHR_ASSIGN,  $3, @$); }
    | expr "&=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_BAND_ASSIGN, $3, @$); }
    | expr "|=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_BOR_ASSIGN,  $3, @$); }
    | expr "&&=" expr               { $$ = new MScriptExpr($1, MScriptExpr::OPER_LAND_ASSIGN, $3, @$); }
    | expr "||=" expr               { $$ = new MScriptExpr($1, MScriptExpr::OPER_LOR_ASSIGN,  $3, @$); }
    | expr "^=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_XOR_ASSIGN,  $3, @$); }
    | expr "**=" expr               { $$ = new MScriptExpr($1, MScriptExpr::OPER_POW_ASSIGN,  $3, @$); }

    // Unary opeators.
    | "~" expr                      { $$ = new MScriptExpr($2, MScriptExpr::OPER_CMPL,     @$); }
    | "!" expr                      { $$ = new MScriptExpr($2, MScriptExpr::OPER_NOT,      @$); }
    | "+" expr  %prec UPLUS         { $$ = new MScriptExpr($2, MScriptExpr::OPER_INT_PROM, @$); }
    | "-" expr  %prec UMINUS        { $$ = new MScriptExpr($2, MScriptExpr::OPER_ADD_INV,  @$); }
    | "++" expr                     { $$ = new MScriptExpr($2, MScriptExpr::OPER_PRE_INC,  @$); }
    | "--" expr                     { $$ = new MScriptExpr($2, MScriptExpr::OPER_PRE_DEC,  @$); }
    | expr "++" %prec POST_INC      { $$ = new MScriptExpr($1, MScriptExpr::OPER_POST_INC, @$); }
    | expr "--" %prec POST_DEC      { $$ = new MScriptExpr($1, MScriptExpr::OPER_POST_DEC, @$); }

    // Ternary operator.
    | expr "?" expr ":" expr        {
                                        MScriptExpr * choices = new MScriptExpr($3, MScriptExpr::OPER_COLON, $5, @$);
                                        $$ = new MScriptExpr($1, MScriptExpr::OPER_TERNARY, choices, @$);
                                    }

    // Comma "operator".
    | expr "," expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_COMMA, $3, @$); }

    // Array element access.
    | id indexes                    { $$ = new MScriptExpr($id, MScriptExpr::OPER_INDEX, $indexes, @$); }

    // Function call.
    | id "(" ")" %prec FCALL        { $$ = new MScriptExpr($id, MScriptExpr::OPER_CALL, @$);     }
    | id "(" expr ")" %prec FCALL   { $$ = new MScriptExpr($id, MScriptExpr::OPER_CALL, $3, @$); }

    // Built-in functions.
    | native_f                      { $$ = $native_f; }
;

// Array element access.
indexes:
      "[" expr "]"                  { $$ = $expr;                                                   }
    | indexes "[" expr "]"          { $$ = new MScriptExpr($1, MScriptExpr::OPER_INDEX, $expr, @$); }
;

// Built-in functions.
native_f:
      "isbool" "(" expr ")"         { $$ = new MScriptExpr($expr, MScriptExpr::OPER_IS_BOOL,     @$); }
    | "isint" "(" expr ")"          { $$ = new MScriptExpr($expr, MScriptExpr::OPER_IS_INT,      @$); }
    | "isstring" "(" expr ")"       { $$ = new MScriptExpr($expr, MScriptExpr::OPER_IS_STRING,   @$); }
    | "isfloat" "(" expr ")"        { $$ = new MScriptExpr($expr, MScriptExpr::OPER_IS_FLOAT,    @$); }
    | "iscomplex" "(" expr ")"      { $$ = new MScriptExpr($expr, MScriptExpr::OPER_IS_COMPLEX,  @$); }
    | "isempty" "(" id ")"          { $$ = new MScriptExpr($id,   MScriptExpr::OPER_IS_EMPTY,    @$); }

    | "bool" "(" expr ")"           { $$ = new MScriptExpr($expr, MScriptExpr::OPER_TO_BOOL,     @$); }
    | "int" "(" expr ")"            { $$ = new MScriptExpr($expr, MScriptExpr::OPER_TO_INT,      @$); }
    | "f_string" "(" expr ")"       { $$ = new MScriptExpr($expr, MScriptExpr::OPER_TO_STRING,   @$); }
    | "float" "(" expr ")"          { $$ = new MScriptExpr($expr, MScriptExpr::OPER_TO_FLOAT,    @$); }
    | "complex"
      "(" expr "," expr ")"         {
                                        $$ = new MScriptExpr ( new MScriptExpr($3, MScriptExpr::OPER_TO_FLOAT, @3),
                                                               MScriptExpr::OPER_TO_COMPLEX,
                                                               new MScriptExpr($5, MScriptExpr::OPER_TO_FLOAT, @5),
                                                               @$ );
                                    }

    | "isnan" "(" expr ")"          { $$ = new MScriptExpr($expr, MScriptExpr::OPER_IS_NAN,      @$); }
    | "isinfinity" "(" expr ")"     { $$ = new MScriptExpr($expr, MScriptExpr::OPER_IS_INFINITY, @$); }
    | "ispositive" "(" expr ")"     { $$ = new MScriptExpr($expr, MScriptExpr::OPER_IS_POSITIVE, @$); }
    | "isnegative" "(" expr ")"     { $$ = new MScriptExpr($expr, MScriptExpr::OPER_IS_NEGATIVE, @$); }
    | "isfinite" "(" expr ")"       { $$ = new MScriptExpr($expr, MScriptExpr::OPER_IS_FINITE,   @$); }

    | "Re" "(" expr ")"             { $$ = new MScriptExpr($expr, MScriptExpr::OPER_REAL,        @$); }
    | "Im" "(" expr ")"             { $$ = new MScriptExpr($expr, MScriptExpr::OPER_IMG_UNIT,    @$); }

    | "sin" "(" expr ")"            { $$ = new MScriptExpr($expr, MScriptExpr::OPER_SIN,         @$); }
    | "cos" "(" expr ")"            { $$ = new MScriptExpr($expr, MScriptExpr::OPER_COS,         @$); }
    | "tan" "(" expr ")"            { $$ = new MScriptExpr($expr, MScriptExpr::OPER_TAN,         @$); }

    | "arcsin" "(" expr ")"         { $$ = new MScriptExpr($expr, MScriptExpr::OPER_ARCSIN,      @$); }
    | "arccos" "(" expr ")"         { $$ = new MScriptExpr($expr, MScriptExpr::OPER_ARCCOS,      @$); }
    | "arctan" "(" expr ")"         { $$ = new MScriptExpr($expr, MScriptExpr::OPER_ARCTAN,      @$); }

    | "sinh" "(" expr ")"           { $$ = new MScriptExpr($expr, MScriptExpr::OPER_SINH,        @$); }
    | "cosh" "(" expr ")"           { $$ = new MScriptExpr($expr, MScriptExpr::OPER_COSH,        @$); }
    | "tanh" "(" expr ")"           { $$ = new MScriptExpr($expr, MScriptExpr::OPER_TANH,        @$); }

    | "arcsinh" "(" expr ")"        { $$ = new MScriptExpr($expr, MScriptExpr::OPER_ARCSINH,     @$); }
    | "arccosh" "(" expr ")"        { $$ = new MScriptExpr($expr, MScriptExpr::OPER_ARCCOSH,     @$); }
    | "arctanh" "(" expr ")"        { $$ = new MScriptExpr($expr, MScriptExpr::OPER_ARCTANH,     @$); }

    | "ceil" "(" expr ")"           { $$ = new MScriptExpr($expr, MScriptExpr::OPER_CEIL,        @$); }
    | "round" "(" expr ")"          { $$ = new MScriptExpr($expr, MScriptExpr::OPER_ROUND,       @$); }
    | "floor" "(" expr ")"          { $$ = new MScriptExpr($expr, MScriptExpr::OPER_FLOOR,       @$); }
    | "abs" "(" expr ")"            { $$ = new MScriptExpr($expr, MScriptExpr::OPER_ABS,         @$); }
    | "min" "(" expr "," expr ")"   { $$ = new MScriptExpr($3,    MScriptExpr::OPER_MIN, $5,     @$); }
    | "max" "(" expr "," expr ")"   { $$ = new MScriptExpr($3,    MScriptExpr::OPER_MAX, $5,     @$); }
    | "length" "(" expr ")"         { $$ = new MScriptExpr($expr, MScriptExpr::OPER_LENGTH,      @$); }
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison.
inline int MScriptParser_error ( YYLTYPE * yylloc,
                                 yyscan_t,
                                 MScriptParserInterface * core,
                                 const char * errorInfo )
{
    core->parserMessage ( yylloc,
                          MScriptBase::MT_WARNING,
                          errorInfo );
    return 0;
}
