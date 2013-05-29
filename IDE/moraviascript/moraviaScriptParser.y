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
// Expect exactly <n> shift/reduce conflicts in this grammar
/* %expect 290 */
// Expect exactly <n> reduce/reduce conflicts in this grammar
/* %expect-rr 0 */
/* Type of parser tables within the LR family, in this case we use LALR (Look-Ahead LR parser) */
%define lr.type lalr
/* Bison declaration to request verbose, specific error message strings when yyerror is called */
%error-verbose
/* Enable LAC (Lookahead Correction) to improve syntax error handling. */
%define parse.lac full

// Symbol semantic value
%union
{
    int integer;                //
    float real;                 //
    char * string;              //
    MScriptExpr * expr;        //
    CompilerStatement * stmt;   //
    struct
    {
        unsigned char * data;   //
        int size;               //
    } array;                    //
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
    #include <QObject>                // For i18n, nothing else.
    #include <cstdlib>                // We need free() here.
    #include <iostream>               // This is required by Bison.
    using namespace std;              // This is required by Bison as well.
    using namespace MScriptStmtTypes; // This NS is heavily used here.

    // Declaration of the lexer prototypes and other things required by Bison
    #include "moraviaScriptLexer.h"

    // Name of the lexer function required by Bison
    #define moraviaScriptParser_lex moraviaScriptLexer_lex

    // Declaration of the error reporting function used by Bison
    inline int moraviaScriptParser_error ( YYLTYPE * yylloc,
                                           yyscan_t yyscanner,
                                           MScriptParserInterface * core,
                                           const char * errorInfo );
%}

// Declare an additional yyparse parameters
%parse-param { yyscan_t yyscanner }
%parse-param { MScriptParserInterface * core }

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

/* Language key words. */
%token KW_IF            "if"
%token KW_ELSE          "else"
%token KW_FOR           "for"
%token KW_WHILE         "while"

/* Other terminal symbols */
%token COMMENT
%token AT
%token EOS              "end of statement (`;')"
/* %token EOL              "end of line" */
%token END      0       "end of file"
// Brackets.
%token B_RND_LEFT       "("
%token B_RND_RIGHT      ")"
%token B_SQR_LEFT       "["
%token B_SQR_RIGHT      "]"
%token B_CRL_LEFT       "{"
%token B_CRL_RIGHT      "}"
// Operators.
%token O_AT             "@"
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
%token O_NE             "<>"
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
%token O_AND_ASSIGN     "&="
%token O_ORB_ASSIGN     "|="
%token O_XOR_ASSIGN     "^="
%token O_INCREMENT      "++"
%token O_DECREMENT      "--"

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
// Right-to-left
%right "~"

/* Terminal symbols with semantic value */
%token<string>  IDENFIFIER
%token<array>   STRING
%token<integer> INTEGER
%token<real>    REAL

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Expressions
%type<expr>     expr            id      real            integer         string
// Statements - general
%type<stmt>     statements      stmt    cnd_exec        if_block        else_block

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

// The start symbol
%start input

// -----------------------------------------------------------------------------
// GRAMMAR RULES
// -----------------------------------------------------------------------------

%%

/*
 * Basic code structure
 */

// The entire program code.
input:
      statements                    { core->syntaxAnalysisComplete($statements); YYACCEPT; }
;

// Sequence of statements.
statements:
      stmt                          { $$ = $stmt; }
    | error                         { $$ = NULL; }
    | statements EOS stmt           { $$ = $1->appendLink($stmt); }
    | statements error EOS stmt     { $$ = $1->appendLink($stmt); }
;

// Single statement.
stmt:
      /* empty */                   { $$ = NULL; }
    | COMMENT                       { $$ = NULL; }
    | cnd_exec                      { $$ = $1; }
    | expr                          { $$ = new MScriptStatement(@$, STMT_EXPR, $expr); }
;


// Conditional execution, i.e. if ( ... ) { ... } [ else { ... } ].
cnd_exec:
      if_block else_block           {
                                        $$ = new MScriptStatement(@$, STMT_CONDITIONAL_EXEC);
                                        $$->createBranch($if_block->appendLink($else_block);
                                    }
;

if_block:
      KW_IF "(" expr ")"
      "{" statements "}"            { $$ = new MScriptStatement(@$, STMT_IF, $expr)->createBranch($statements); }
    | KW_IF "(" expr ")" stmt       { $$ = new MScriptStatement(@$, STMT_IF, $expr)->createBranch($statements); }
;

else_block:
      /* empty */                   { $$ = NULL; }
    | KW_ELSE "{" statements "}"    { $$ = new MScriptStatement(@$, STMT_ELSE, $expr)->createBranch($statements); }
    | KW_ELSE stmt                  { $$ = new MScriptStatement(@$, STMT_ELSE, $expr)->createBranch($statements); }
;


integer:
      INTEGER                       { $$ = new MScriptExpr($INTEGER); }
;
real:
      REAL                          { $$ = new MScriptExpr($REAL); }
;
id:
      IDENFIFIER                    { $$ = new MScriptExpr($IDENFIFIER); }
;
string:
      STRING                        { $$ = new MScriptExpr(MScriptExpr::Value($STRING.data, $STRING.size)); }
;
expr:
      id                            { $$ = $id; }
    | integer                       { $$ = $integer; }
    | real                          { $$ = $real; }
    | "(" expr ")"                  { $$ = $expr; }
;
