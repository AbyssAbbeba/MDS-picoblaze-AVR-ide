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

    uint64_t integer;           //
    long double real;           //
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
%token O_NE             "<>"
%token O_NE2            "!="
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
%type<expr>     expr
// Statements - general.
%type<stmt>     statements      stmt

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
                                    }
    | expr ";"                      {
                                    }
    | "return" e_expr ";"           {
                                    }
    | datatype id ";"               {
                                    }
    | datatype id "(" param_list ")" ";"
                                    {
                                    }
    | "if" "(" expr ")" stmt        {
                                    }
    | "if" "(" expr ")" stmt "else" stmt
                                    {
                                    }
    | "for" "(" e_expr ";" e_expr ";" e_expr ")" stmt
                                    {
                                    }
    | "while" "(" expr ")" stmt     {
                                    }
    | "do" stmt "while" "(" expr ")" ";"
                                    {
                                    }
    | "switch" "(" expr ")" "{" switch_body "}"
                                    {
                                    }
;


// `switch' statement body.
switch_body:
      cases statements              {}
    | switch_body cases statements  {}
;


// Sequence of `case:' and/or `default:' statements, used inside `switch' statement.
cases:
      "case" expr ":"               {}
    | "default" ":"                 {}
    | cases "case" expr ":"         {}
    | cases "default" ":"           {}
;


// Scope.
scope:
      "{" statements "}"            {}
    | "{" /* empty */ "}"           {}
;


// Identifier.
id:
      IDENTIFIER                    {}
;



param:
      id                            {}
    | "&" id                        {}
    | "*" id                        {}
;


// List of function parameters.
param_list:
      /* empty */                   {}
    | param                         {}
    | param_list "," param          {}
;


// Expression, possibly empty.
e_expr:
      /* empty */                   {  }
    | expr                          {  }
;


// String constant.
string:
      STRING                        {  }
;


// Datatypes
datatype:
      "char"                        {  }
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
    // Single value expressions.
      id                            {  }
    | string                        {  }
    | INTEGER                       {  }
    | REAL                          {  }

    // Parentheses.
    | "(" expr ")"                  {  }

    // Binary operators.
    | expr "/" expr                 {  }
    | expr "+" expr                 {  }
    | expr "-" expr                 {  }
    | expr "*" expr                 {  }
    | expr "!" expr                 {  }
    | expr "%" expr                 {  }
    | expr "<<" expr                {  }
    | expr ">>" expr                {  }
    | expr "&&" expr                {  }
    | expr "||" expr                {  }
    | expr "&" expr                 {  }
    | expr "|" expr                 {  }
    | expr "^" expr                 {  }
    | expr "==" expr                {  }
    | expr "<>" expr                {  }
    | expr "!=" expr                {  }
    | expr "<" expr                 {  }
    | expr "<=" expr                {  }
    | expr ">" expr                 {  }
    | expr ">=" expr                {  }
    | expr "=" expr                 {  }
    | expr "~" expr                 {  }
    | expr "+=" expr                {  }
    | expr "-=" expr                {  }
    | expr "*=" expr                {  }
    | expr "/=" expr                {  }
    | expr "%=" expr                {  }
    | expr "<<=" expr               {  }
    | expr ">>=" expr               {  }
    | expr "&=" expr                {  }
    | expr "|=" expr                {  }
    | expr "&&=" expr               {  }
    | expr "||=" expr               {  }
    | expr "^=" expr                {  }

    // Unary operators.
    | "~" expr                      {  }
    | "!" expr                      {  }
    | "+" expr  %prec UPLUS         {  }
    | "-" expr  %prec UMINUS        {  }
    | "++" expr                     {  }
    | "--" expr                     {  }
    | expr "++" %prec POST_INC      {  }
    | expr "--" %prec POST_DEC      {  }

    // Ternary operator.
    | expr "?" expr ":" expr        {  }

    // Comma operator.
    | expr "," expr                 {  }

    // Array element access.
    | id indexes                    {  }

    // Function call
    | id "(" ")" %prec FCALL        {  }
    | id "(" expr ")" %prec FCALL   {  }
;


indexes:
      "[" expr "]"                  {  }
    | indexes "[" expr "]"          {  }
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
