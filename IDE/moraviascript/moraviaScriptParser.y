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
%name-prefix "moraviaScriptParser_"
// Generate a parser header file containing macro definitions, etc.
%defines "moraviaScriptParser.h"
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

    #include <string>
    #include <cstdlib>
    #include <cstring>
    #include <iostream>

    using namespace std;              // This is required by Bison as well.
    using namespace MScriptStmtTypes; // This NS is heavily used here.

    // Declaration of the lexer prototypes and other things required by Bison.
    #include "moraviaScriptLexer.h"

    // Name of the lexer func required by Bison.
    #define moraviaScriptParser_lex moraviaScriptLexer_lex

    // Declaration of the error reporting function used by Bison.
    inline int moraviaScriptParser_error ( YYLTYPE * yylloc,
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

/* Special values: */
%token SV_EMPTY         "EMPTY"

/* Built-in data types: */
%token DT_BOOL          "bool data type specifier"
%token DT_INT           "int data type specifier"
%token DT_FLOAT         "float data type specifier"
%token DT_COMPLEX       "complex data type specifier"
%token DT_STRING        "string data type specifier"
%token DT_OBJECT        "object data type specifier"

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
%token O_INCREMENT      "++"
%token O_DECREMENT      "--"

/* Operator precedence (the one declared later has the higher precedence) */
%left ","
%right "&=" "^=" "|=" "&&=" "||="
%right "<<=" ">>="
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
%right "~" "!"
%right UPLUS UMINUS
%right "++" "--"
%left "[" "]"
%left FCALL
%left POST_INC POST_DEC
%left "(" ")"

/* Precedence for non operators, placed here to solve certain shift/reduce conflicts. */
%nonassoc "else"

/* Terminal symbols with semantic value */
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
// Expressions
%type<expr>     expr            e_expr          e_int           id              sid             string
%type<expr>     param           param_list      decl            decl_list       indexes
// Statements - general
%type<stmt>     statements      stmt            cases           switch_body     scope

// Each time the parser discards symbol with certain semantic types, their memory have to bee freed
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
                                        core->syntaxAnalysisComplete(NULL);
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
                                        $$ = NULL;
                                    }
    | error                         {
                                        $$ = NULL;
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
      /* empty */                   { $$ = NULL;                   }
    | param                         { $$ = $param;                 }
    | param_list "," param          { $$ = $1->appendLink($param); }
;

// Integer, possibly empty.
e_int:
      /* empty */                   { $$ = NULL;                          }
    | INTEGER                       { $$ = new MScriptExpr($INTEGER, @$); }
;

// Expression, possibly empty.
e_expr:
      /* empty */                   { $$ = NULL;  }
    | expr                          { $$ = $expr; }
;

// Expression.
expr:
    // Single value expressions.
      id                            { $$ = $id;                                              }
    | string                        { $$ = $string;                                          }
    | "EMPTY"                       { $$ = new MScriptExpr(@$);                              }
    | BOOLEAN                       { $$ = new MScriptExpr($BOOLEAN, @$);                    }
    | REAL                          { $$ = new MScriptExpr($REAL, @$);                       }
    | INTEGER                       { $$ = new MScriptExpr($INTEGER, @$);                    }
    | IMAGINARY                     { $$ = new MScriptExpr(MScriptValue(0, $IMAGINARY), @$); }

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
    | expr "||" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_LOR,  $3, @$);        }
    | expr "&&" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_LAND, $3, @$);        }
    | expr "==" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_EQ,   $3, @$);        }
    | expr "!=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_NE,   $3, @$);        }
    | expr "<" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_LT,   $3, @$);        }
    | expr "<=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_LE,   $3, @$);        }
    | expr ">=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_GE,   $3, @$);        }
    | expr ">" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_GT,   $3, @$);        }
    | expr ">>" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_SHR,  $3, @$);        }
    | expr "<<" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_SHL,  $3, @$);        }
    | expr "=" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_ASSIGN,      $3, @$); }
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
;

// Array element access.
indexes:
      "[" expr "]"                  { $$ = $expr;                                                   }
    | indexes "[" expr "]"          { $$ = new MScriptExpr($1, MScriptExpr::OPER_INDEX, $expr, @$); }
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison.
inline int moraviaScriptParser_error ( YYLTYPE * yylloc,
                                       yyscan_t,
                                       MScriptParserInterface * core,
                                       const char * errorInfo )
{
    core->parserMessage ( yylloc,
                          MScriptBase::MT_WARNING,
                          errorInfo );
    return 0;
}
