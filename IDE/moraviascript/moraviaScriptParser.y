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
    MScriptStatement * stmt;    //
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
%token KW_FUNC          "func"
%token KW_DO            "do"
%token KW_RETURN        "return"
%token KW_BREAK         "break"
%token KW_CONTINUE      "continue"
%token KW_SWITCH        "switch"
%token KW_CASE          "case"
%token KW_DELETE        "delete"

/* Other terminal symbols */
%token O_COLON          ":"
%token O_QUESTION_MARK  "?"
%token EOS              ";"
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
%left ","
// Right-to-left
%right "~"
%right "="

/* Terminal symbols with semantic value */
%token<string>  IDENFIFIER
%token<array>   STRING
%token<integer> INTEGER
%token<real>    REAL

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Expressions
%type<expr>     expr            id              real            integer         string          e_expr
%type<expr>     id_list
// Statements - general
%type<stmt>     statements      stmt            cnd_exec        if_block        else_block      return
%type<stmt>     switch          cases           case            delete          continue        break
%type<stmt>     trigger         function        for_loop        while_loop      do_while_loop

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
      statements                    {
                                        core->syntaxAnalysisComplete($statements);
                                        YYACCEPT;
                                    }
;

// Sequence of statements.
statements:
      stmt                          { $$ = $stmt; }
    | error                         { $$ = NULL; }
    | statements ";" stmt           { $$ = $1->appendLink($stmt); }
    | statements error ";" stmt     { $$ = $1->appendLink($stmt); }
    | "{" statements "}"            {
                                        $$ = new MScriptStatement(@$, STMT_SCOPE);
                                        $$->createBranch($2);
                                    }
;

// Single statement.
stmt:
      /* empty */                   { $$ = NULL;                                       }
    | trigger                       { $$ = $trigger;                                   }
    | function                      { $$ = $function;                                  }
    | return                        { $$ = $return;                                    }
    | cnd_exec                      { $$ = $cnd_exec;                                  }
    | for_loop                      { $$ = $for_loop;                                  }
    | while_loop                    { $$ = $while_loop;                                }
    | do_while_loop                 { $$ = $do_while_loop;                             }
    | continue                      { $$ = $continue;                                  }
    | break                         { $$ = $break;                                     }
    | switch                        { $$ = $switch;                                    }
    | delete                        { $$ = $delete;                                    }
    | expr                          { $$ = new MScriptStatement(@$, STMT_EXPR, $expr); }
;

// Conditional execution, i.e. if ( ... ) { ... } [ else { ... } ].
cnd_exec:
      if_block else_block           {
                                        $$ = new MScriptStatement(@$, STMT_CONDITIONAL_EXEC);
                                        $$->createBranch($if_block->appendLink($else_block));
                                    }
;

if_block:
      KW_IF "(" expr ")"
      "{" statements "}"            { $$ = (new MScriptStatement(@$, STMT_IF, $expr))->createBranch($statements); }
    | KW_IF "(" expr ")" stmt       { $$ = (new MScriptStatement(@$, STMT_IF, $expr))->createBranch($stmt); }
;

else_block:
      /* empty */                   { $$ = NULL; }
    | KW_ELSE "{" statements "}"    { $$ = (new MScriptStatement(@$, STMT_ELSE))->createBranch($statements); }
    | KW_ELSE stmt                  { $$ = (new MScriptStatement(@$, STMT_ELSE))->createBranch($stmt); }
;

trigger:
      "/" expr "/"
      "{" statements "}"            { $$ = (new MScriptStatement(@$, STMT_TRIGGER, $expr))->createBranch($statements); }
    | "/" expr "/" stmt             { $$ = (new MScriptStatement(@$, STMT_TRIGGER, $expr))->createBranch($stmt); }
;

function:
      KW_FUNC id "(" id_list ")"
      "{" statements "}"            {
                                        $$ = new MScriptStatement(@$, STMT_FUNCTION, $id->appendLink($id_list));
                                        $$->createBranch($statements);
                                    }
    | KW_FUNC id "(" id_list ")"
      stmt                          {
                                        $$ =new MScriptStatement(@$, STMT_FUNCTION, $id->appendLink($id_list));
                                        $$->createBranch($stmt);
                                    }
;

for_loop:
      KW_FOR
      "(" e_expr ";" e_expr ";" e_expr ")"
      "{" statements "}"            {
                                        MScriptExpr * b = new MScriptExpr($5, MScriptExpr::OPER_SEMICOLON, $7, @KW_FOR);
                                        MScriptExpr * a = new MScriptExpr($3, MScriptExpr::OPER_SEMICOLON, b,  @KW_FOR);
                                        $$ = new MScriptStatement(@$, STMT_FOR, a);
                                        $$->createBranch($statements);
                                    }
    | KW_FOR
      "(" e_expr ";" e_expr ";" e_expr ")"
      stmt                          {
                                        MScriptExpr * b = new MScriptExpr($5, MScriptExpr::OPER_SEMICOLON, $7, @KW_FOR);
                                        MScriptExpr * a = new MScriptExpr($3, MScriptExpr::OPER_SEMICOLON, b,  @KW_FOR);
                                        $$ = new MScriptStatement(@$, STMT_FOR, a);
                                        $$->createBranch($stmt);
                                    }
;

while_loop:
      KW_WHILE "(" expr ")"
      "{" statements "}"            { $$ = (new MScriptStatement(@$, STMT_WHILE, $expr))->createBranch($statements); }
    | KW_WHILE "(" expr ")" stmt    { $$ = (new MScriptStatement(@$, STMT_WHILE, $expr))->createBranch($stmt); }
;

do_while_loop:
      KW_DO "{" statements "}"
      KW_WHILE "(" expr ")"         { $$ =(new MScriptStatement(@$, STMT_DO_WHILE, $expr))->createBranch($statements); }
    | KW_DO stmt
      KW_WHILE "(" expr ")"         { $$ =(new MScriptStatement(@$, STMT_DO_WHILE, $expr))->createBranch($stmt); }
;

return:
      KW_RETURN                     { $$ = new MScriptStatement(@$, STMT_RETURN); }
    | KW_RETURN expr                { $$ = new MScriptStatement(@$, STMT_RETURN, $expr); }
;

continue:
      KW_CONTINUE                   { $$ = new MScriptStatement(@$, STMT_CONTINUE); }
    | KW_CONTINUE expr              { $$ = new MScriptStatement(@$, STMT_CONTINUE, $expr); }
;

break:
      KW_BREAK                      { $$ = new MScriptStatement(@$, STMT_BREAK); }
    | KW_BREAK expr                 { $$ = new MScriptStatement(@$, STMT_BREAK, $expr); }
;

switch:
      KW_SWITCH "(" expr ")"
      "{" cases "}"                 {
                                        $$ = new MScriptStatement(@$, STMT_SWITCH, $expr);
                                        $$->createBranch($cases);
                                    }
;

cases:
      /* empty */                   { $$ = NULL;                  }
    | case                          { $$ = $case;                 }
    | cases case                    { $$ = $1->appendLink($case); }
;

case:
      KW_CASE expr ":" statements   {
                                        $$ = new MScriptStatement(@$, STMT_CASE, $expr);
                                        $$->createBranch($statements);
                                    }
;

delete:
      KW_DELETE id                  { $$ = new MScriptStatement(@$, STMT_DELETE, $id); }
;

id_list:
      /* empty */                   { $$ = NULL;                }
    | id                            { $$ = $id;                 }
    | id_list "," id                { $$ = $1->appendLink($id); }
;

id:
      IDENFIFIER                    { $$ = new MScriptExpr($IDENFIFIER, @$); }
;
integer:
      INTEGER                       { $$ = new MScriptExpr($INTEGER, @$); }
;
real:
      REAL                          { $$ = new MScriptExpr($REAL, @$); }
;
string:
      STRING                        { $$ = new MScriptExpr(MScriptExpr::Value($STRING.data, $STRING.size), @$); }
;
e_expr:
      /* empty */                   { $$ = NULL;  }
    | expr                          { $$ = $expr; }
;
expr:
    // Single value expressions.
      id                            { $$ = $id;      }
    | integer                       { $$ = $integer; }
    | real                          { $$ = $real;    }
    | string                        { $$ = $string;  }

    // Parentheses.
    | "(" expr ")"                  { $$ = $2; }

    // Binary operators.
    | expr "+" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_ADD,  $3, @$);       }
    | expr "-" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_SUB,  $3, @$);       }
    | expr "*" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_MULT, $3, @$);       }
    | expr "/" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_DIV,  $3, @$);       }
    | expr "%" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_MOD,  $3, @$);       }
    | expr "|" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_BOR,  $3, @$);       }
    | expr "^" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_BXOR, $3, @$);       }
    | expr "&" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_BAND, $3, @$);       }
    | expr "||" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_LOR,  $3, @$);       }
    | expr "&&" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_LAND, $3, @$);       }
    | expr "==" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_EQ,   $3, @$);       }
    | expr "!=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_NE,   $3, @$);       }
    | expr "<" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_LT,   $3, @$);       }
    | expr "<=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_LE,   $3, @$);       }
    | expr ">=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_GE,   $3, @$);       }
    | expr ">" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_GT,   $3, @$);       }
    | expr ">>" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_SHR,  $3, @$);       }
    | expr "<<" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_SHL,  $3, @$);       }
    | expr "+=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_ADD_ASSIGN, $3, @$); }
    | expr "-=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_SUB_ASSIGN, $3, @$); }
    | expr "*=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_MUL_ASSIGN, $3, @$); }
    | expr "/=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_DIV_ASSIGN, $3, @$); }
    | expr "%=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_MOD_ASSIGN, $3, @$); }
    | expr "<<=" expr               { $$ = new MScriptExpr($1, MScriptExpr::OPER_SHL_ASSIGN, $3, @$); }
    | expr ">>=" expr               { $$ = new MScriptExpr($1, MScriptExpr::OPER_SHR_ASSIGN, $3, @$); }
    | expr "&=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_AND_ASSIGN, $3, @$); }
    | expr "|=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_ORB_ASSIGN, $3, @$); }
    | expr "^=" expr                { $$ = new MScriptExpr($1, MScriptExpr::OPER_XOR_ASSIGN, $3, @$); }
    | expr "=" expr                 { $$ = new MScriptExpr($1, MScriptExpr::OPER_ASSIGN, $3,     @$); }

    // Unary opeators.
    | "+" expr                      { $$ = new MScriptExpr($2, MScriptExpr::OPER_INT_PROM, @$); }
    | "-" expr                      { $$ = new MScriptExpr($2, MScriptExpr::OPER_ADD_INV,  @$); }
    | "~" expr                      { $$ = new MScriptExpr($2, MScriptExpr::OPER_CMPL,     @$); }
    | "!" expr                      { $$ = new MScriptExpr($2, MScriptExpr::OPER_NOT,      @$); }
    | "++" expr                     { $$ = new MScriptExpr($2, MScriptExpr::OPER_PRE_INC,  @$); }
    | "--" expr                     { $$ = new MScriptExpr($2, MScriptExpr::OPER_PRE_DEC,  @$); }
    | expr "++"                     { $$ = new MScriptExpr($1, MScriptExpr::OPER_POST_INC, @$); }
    | expr "--"                     { $$ = new MScriptExpr($1, MScriptExpr::OPER_POST_DEC, @$); }

    // Ternary operator.
    | expr "?" expr ":" expr        {
                                        MScriptExpr * choices = new MScriptExpr($3, MScriptExpr::OPER_COLON, $5, @$);
                                        $$ = new MScriptExpr($1, MScriptExpr::OPER_TERNARY, choices, @$);
                                    }

    // Comma "operator".
    | expr "," expr                 { $$ = $1->appendLink($3); }

    // Array element access.
    | id "[" expr "]"               { $$ = new MScriptExpr($id, MScriptExpr::OPER_INDEX, $3, @$); }

    // Function call.
    | id "(" ")"                    { $$ = new MScriptExpr($id, MScriptExpr::OPER_CALL, @$);     }
    | id "(" expr ")"               { $$ = new MScriptExpr($id, MScriptExpr::OPER_CALL, $3, @$); }
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison
inline int moraviaScriptParser_error ( YYLTYPE * yylloc,
                                       yyscan_t,
                                       MScriptParserInterface * core,
                                       const char * errorInfo )
{
    std::cout << MScriptSrcLocation(yylloc) << "errorInfo = '" << errorInfo << "'\n";
    return 0;
}
