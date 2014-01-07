// =============================================================================
/**
 * @brief
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 */
// =============================================================================


// -----------------------------------------------------------------------------
// PROLOGUE - BISON DECLARATIONS
// -----------------------------------------------------------------------------

// Require version <version> or higher of Bison.
%require "2.5"
// Rename the external symbols used in the parser so that they start with prefix instead of ‘yy’.
%name-prefix "CompilerCParser_"
// Generate a parser header file containing macro definitions, etc.
%defines "CompilerCParser.h"
// Generate a pure, reentrant parser.
%define api.pure
// Generate the code processing the locations.
%locations
// Specify the programming language for the generated parser.
%language "C"
// Write an extra output file containing verbose descriptions of the parser states.
%verbose
// Expect exactly <n> shift/reduce conflicts in this grammar.
/* %expect 0 */
// Expect exactly <n> reduce/reduce conflicts in this grammar.
/* %expect-rr 0 */
// Type of parser tables within the LR family, in this case we use LALR (Look-Ahead LR parser).
%define lr.type lalr
// Bison declaration to request verbose, specific error message strings when yyerror is called.
%error-verbose
// Enable LAC (Lookahead Correction) to improve syntax error handling.
%define parse.lac full

// Symbol semantic value.
%union
{
    CompilerExpr      * expr;   //
    CompilerStatement * stmt;   //

    int64_t integer;            //
    double real;                //
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
    #include "CompilerParserInterface.h"
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

    // For i18n only
    #include <QObject>

    using namespace std;                    // This is required by Bison as well.
    using namespace CompilerStatementTypes; // This NS is heavily used here.

    // Declaration of the lexer prototypes and other things required by Bison.
    #include "CompilerCLexer.h"

    // Name of the lexer func required by Bison.
    #define CompilerCParser_lex CompilerCLexer_lex

    // Make a few things a little easier...
    #define LOC(location) \
        ( compiler->toSourceLocation(location) )

    // Declaration of the error reporting function used by Bison.
    inline int CompilerCParser_error ( YYLTYPE * yylloc,
                                       yyscan_t yyscanner,
                                       CompilerParserInterface * compiler,
                                       const char * errorInfo );
%}

// Declare an additional yyparse parameters.
%parse-param { yyscan_t yyscanner }
%parse-param { CompilerParserInterface * compiler }

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
// -----------------------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>  TODO: declare grammar aplhabet here! <<<<<<<<<<<<<<<<<<<<<<<<<<<
// -----------------------------------------------------------------------------------------
%token EOS              ";"

%token KW_AUTO          "auto"
%token KW_BREAK         "break"
//%token KW_BOOL;         "bool"
%token KW_CASE          "case"
%token KW_CONST         "const"
%token KW_CONTINUE      "continue"
%token KW_DEFAULT       "default"
%token KW_DO            "do"
%token KW_ELSE          "else"
%token KW_EXTERN        "extern"
%token KW_FOR           "for"
%token KW_GOTO          "goto"
%token KW_IF            "if"
%token KW_REGISTER      "register"
%token KW_RETURN        "return"
%token KW_SIZEOF        "sizeof"
%token KW_STATIC        "static"
%token KW_SWITCH        "switch"
%token KW_TYPEDEF       "typedef"
%token KW_VOLATILE      "volatile"
%token KW_WHILE         "while"

%token DATA_CHAR        "char"
%token DATA_DOUBLE      "double"
%token DATA_ENUM        "enum"
%token DATA_FLOAT       "float"
%token DATA_INT         "int"
%token DATA_LONG        "long"
%token DATA_SHORT       "short"
%token DATA_SIGNED      "signed"
%token DATA_STRUCT      "struct"
%token DATA_UNION       "union"
%token DATA_UNSIGNED    "unsigned"
%token DATA_VOID        "void"

%token B_RND_LEFT       "("
%token B_RND_RIGHT      ")"
%token B_SQR_LEFT       "["
%token B_SQR_RIGHT      "]"
%token B_CRL_LEFT       "{"
%token B_CRL_RIGHT      "}"

%token O_COLON          ":"
%token O_QUESTION_MARK  "?"
%token O_COMMA          ","
%token O_DOT            "."
%token O_ARROW          "->"
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
%token O_BITNOT         "~"
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
%token O_DECREMENT      "--"
%token O_INCREMENT      "++"

/* Operator precedence (the one declared later has the higher precedence). */
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
%left POST_INC POST_DEC
%left "." "->"
%left FCALL
%left "[" "]"
%left "(" ")"

/* Precedence for non operators, placed here to solve certain shift/reduce conflicts. */
%nonassoc "else"


/* Terminal symbols with semantic value. */
%token<symbol>    IDENTIFIER    "idenfifier"
%token<string>    STRING        "string"
%token<integer>   INTEGER       "integer"
%token<real>      REAL          "real number"

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Expressions.
%type<expr>     expr            e_expr      id      string      param       param_list      indexes     datatype
// Statements - general.
%type<stmt>     statements      stmt        cases   scope       switch_body

// Each time the parser discards symbol with certain semantic types, their memory have to bee freed.
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
} <string>

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
                                        compiler->processCodeTree($statements);
                                        YYACCEPT;
                                    }
    | /* empty */                   {
                                        compiler->processCodeTree(NULL);
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
    | scope                         {
                                        $$ = $scope;
                                    }
    | expr ";"                      {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_EXPR, $expr);
                                    }
    | "break" ";"                   {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_BREAK, NULL);
                                    }
    | "continue" ";"                {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_CONTINUE, NULL);
                                    }
    | "return" e_expr ";"           {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_RETURN, $e_expr);
                                    }
    | datatype id ";"               {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_VAR, $datatype->appendLink($id));
                                    }
    | datatype id "(" param_list ")" ";"
                                    {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_FUNC, $datatype->appendLink($id)->appendLink($param_list));
                                    }
    | datatype id "(" param_list ")" scope
                                    {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_FUNC, $datatype->appendLink($id)->appendLink($param_list));
                                        $$->createBranch($6);
                                    }
    | "if" "(" expr ")" stmt        {
                                        CompilerStatement *ifBlock = new CompilerStatement(LOC(@1), C_STMT_IF, $expr);
                                        ifBlock->createBranch($5);

                                        $$ = new CompilerStatement(LOC(@$), C_STMT_CONDITION);
                                        $$->createBranch(ifBlock);
                                    }
    | "if" "(" expr ")" stmt "else" stmt
                                    {
                                        CompilerStatement *ifBlock = new CompilerStatement(LOC(@1), C_STMT_IF, $3);
                                        ifBlock->createBranch($5);

                                        CompilerStatement *elseBlock = new CompilerStatement(LOC(@6), C_STMT_ELSE);
                                        elseBlock->createBranch($7);

                                        $$ = new CompilerStatement(LOC(@$), C_STMT_CONDITION);
                                        $$->createBranch(ifBlock->appendLink(elseBlock));
                                    }
    | "for" "(" e_expr ";" e_expr ";" e_expr ")" stmt
                                    {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_FOR, $3->appendLink($5)->appendLink($7));
                                        $$->createBranch($9);
                                    }
    | "while" "(" expr ")" stmt     {
                                        $$ = (new CompilerStatement(LOC(@$), C_STMT_WHILE, $expr))->createBranch($5);
                                    }
    | "do" stmt "while" "(" expr ")" ";"
                                    {
                                        $$ = (new CompilerStatement(LOC(@$), C_STMT_DO_WHILE, $expr))->createBranch($2);
                                    }
    | "switch" "(" expr ")" "{" switch_body "}"
                                    {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_SWITCH, $expr);
                                        $$->createBranch($switch_body);
                                    }
    | "switch" "(" expr ")" "{" /* empty */ "}"
                                    {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_SWITCH, $expr);
                                    }
;


// `switch' statement body.
switch_body:
      cases statements              { $$ = $cases->createBranch($statements);                  }
    | switch_body cases statements  { $$ = $1->appendLink($cases->createBranch($statements));  }
;


// Sequence of `case:' and/or `default:' statements, used inside `switch' statement.
cases:
      "case" expr ":"               { $$ = new CompilerStatement(LOC(@$), C_STMT_CASE, $expr);      }
    | "default" ":"                 { $$ = new CompilerStatement(LOC(@$), C_STMT_DEFAULT);          }
    | cases "case" expr ":"         { $$ = $1->appendArgsLink($expr);                          }
    | cases "default" ":"           {
                                        $1->completeDelete();
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_DEFAULT);
                                    }
;


// Scope.
scope:
      "{" statements "}"            {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_SCOPE);
                                        $$->createBranch($statements);
                                    }
    | "{" /* empty */ "}"           {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_SCOPE);
                                    }
;


// Identifier.
id:
      IDENTIFIER                    { $$ = new CompilerExpr($IDENTIFIER, LOC(@$));                  }
;



param:
      datatype id                   {
                                        $$ = $id;
                                    }
    | datatype "&" id               {
                                        $$ = $id;
                                        $id->m_operator = CompilerExpr::OPER_REF;
                                    }
    | datatype "*" id               {
                                        $$ = $id;
                                        $id->m_operator = CompilerExpr::OPER_DEREF;
                                    }
;


// List of function parameters.
param_list:
      /* empty */                   { $$ = NULL;                                                    }
    | param                         { $$ = $param;                                                  }
    | param_list "," param          { $$ = $1->appendLink($param);                                  }
;


// Expression, possibly empty.
e_expr:
      /* empty */                   { $$ = NULL;                                                    }
    | expr                          { $$ = $expr;                                                   }
;


// String constant.
string:
      STRING                        {
                                        //$ = new CompilerExpr(CompilerValue($STRING.data, $STRING.size), LOC(@$));
                                    }
;


// Datatypes
datatype:
      /* empty */                   {  }
    | "char"                        {  }
    | "double"                      {  }
    | "enum"                        {  }
    | "float"                       {  }
    | "int"                         {  }
    | "long"                        {  }
    | "short"                       {  }
    | "signed"                      {  }
    | "struct"                      {  }
    | "union"                       {  }
    | "unsigned"                    {  }
;


// Expression.
expr:
    // Single value expressions.    z
      id                            { $$ = $id;                                                                  }
    | string                        { $$ = $string;                                                              }
    | INTEGER                       { $$ = new CompilerExpr($INTEGER, LOC(@$));                                  }
    | REAL                          { $$ = new CompilerExpr($REAL, LOC(@$));                                     }

    // Parentheses.
    | "(" expr ")"                  { $$ = $2;                                                                   }

    // Binary operators.
    | expr "/" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_DIV,         $3, LOC(@$));   }
    | expr "+" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_ADD,         $3, LOC(@$));   }
    | expr "-" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_SUB,         $3, LOC(@$));   }
    | expr "*" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_MULT,        $3, LOC(@$));   }
    | expr "!" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_NOT,         $3, LOC(@$));   }
    | expr "%" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_MOD,         $3, LOC(@$));   }
    | expr "<<" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_SHL,         $3, LOC(@$));   }
    | expr ">>" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_SHR,         $3, LOC(@$));   }
    | expr "&&" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_LAND,        $3, LOC(@$));   }
    | expr "||" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_LOR,         $3, LOC(@$));   }
    | expr "&" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_BAND,        $3, LOC(@$));   }
    | expr "|" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_BOR,         $3, LOC(@$));   }
    | expr "^" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_BXOR,        $3, LOC(@$));   }
    | expr "==" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_EQ,          $3, LOC(@$));   }
    | expr "!=" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_NE,          $3, LOC(@$));   }
    | expr "<" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_LT,          $3, LOC(@$));   }
    | expr "<=" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_LE,          $3, LOC(@$));   }
    | expr ">" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_GT,          $3, LOC(@$));   }
    | expr ">=" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_GE,          $3, LOC(@$));   }
    | expr "=" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_ASSIGN,      $3, LOC(@$));   }
    | expr "~" expr                 {  $$ = new CompilerExpr($1, CompilerExpr::OPER_CMPL,        $3, LOC(@$));   }
    | expr "+=" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_ADD_ASSIGN,  $3, LOC(@$));   }
    | expr "-=" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_SUB_ASSIGN,  $3, LOC(@$));   }
    | expr "*=" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_MUL_ASSIGN,  $3, LOC(@$));   }
    | expr "/=" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_DIV_ASSIGN,  $3, LOC(@$));   }
    | expr "%=" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_MOD_ASSIGN,  $3, LOC(@$));   }
    | expr "<<=" expr               {  $$ = new CompilerExpr($1, CompilerExpr::OPER_SHL_ASSIGN,  $3, LOC(@$));   }
    | expr ">>=" expr               {  $$ = new CompilerExpr($1, CompilerExpr::OPER_SHR_ASSIGN,  $3, LOC(@$));   }
    | expr "&=" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_AND_ASSIGN,  $3, LOC(@$));   }
    | expr "|=" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_ORB_ASSIGN,  $3, LOC(@$));   }
    | expr "^=" expr                {  $$ = new CompilerExpr($1, CompilerExpr::OPER_XOR_ASSIGN,  $3, LOC(@$));   }

    // Unary operators.
    | "~" expr                      { $$ = new CompilerExpr($2, CompilerExpr::OPER_CMPL, LOC(@$));               }
    | "!" expr                      { $$ = new CompilerExpr($2, CompilerExpr::OPER_NOT, LOC(@$));                }
    | "+" expr  %prec UPLUS         { $$ = new CompilerExpr($2, CompilerExpr::OPER_INT_PROM, LOC(@$));           }
    | "-" expr  %prec UMINUS        { $$ = new CompilerExpr($2, CompilerExpr::OPER_ADD_INV, LOC(@$));            }
    | "++" expr                     { $$ = new CompilerExpr($2, CompilerExpr::OPER_PRE_INC, LOC(@$));            }
    | "--" expr                     { $$ = new CompilerExpr($2, CompilerExpr::OPER_PRE_DEC, LOC(@$));            }
    | expr "++" %prec POST_INC      { $$ = new CompilerExpr($1, CompilerExpr::OPER_POST_INC, LOC(@$));           }
    | expr "--" %prec POST_DEC      { $$ = new CompilerExpr($1, CompilerExpr::OPER_POST_DEC, LOC(@$));           }

    // Ternary operator.
    | expr "?" expr ":" expr        {
                                        CompilerExpr *choices = new CompilerExpr($3, CompilerExpr::OPER_COLON, $5, LOC(@$));
                                        $$ = new CompilerExpr($1, CompilerExpr::OPER_TERNARY, choices, LOC(@$));
                                    }

    // Comma operator.
    // | expr "," expr                 {  }

    // Array element access.
    | id indexes                    { $$ = new CompilerExpr($id, CompilerExpr::OPER_INDEX, $indexes, LOC(@$));   }

    // Struct index
    // | struct_index                  {  }

    // Function call
    | id "(" ")" %prec FCALL        { $$ = new CompilerExpr($id, CompilerExpr::OPER_CALL, LOC(@$));              }
    | id "(" expr ")" %prec FCALL   { $$ = new CompilerExpr($id, CompilerExpr::OPER_CALL, $3, LOC(@$));          }
;


indexes:
      "[" expr "]"                  { $$ = $expr;                                                                }
    | indexes "[" expr "]"          { $$ = new CompilerExpr($1, CompilerExpr::OPER_INDEX, $expr, LOC(@$));       }
;


struct_index:
      id "." id                     {  }
    | id "->" id                    {  }
    | id "." struct_index           {  }
    | id "->" struct_index          {  }
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison.
inline int CompilerCParser_error ( YYLTYPE * yylloc,
                                   yyscan_t,
                                   CompilerParserInterface * compiler,
                                   const char * errorInfo )
{
    compiler->parserMessage(compiler->toSourceLocation(yylloc), CompilerBase::MT_ERROR, errorInfo);
    return 0;
}
