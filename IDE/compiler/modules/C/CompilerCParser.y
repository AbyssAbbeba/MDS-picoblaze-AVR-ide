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
    // C Datatypes enum
    #include "CompilerCDatatypes.h"

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

%token KW_BREAK         "break"
%token KW_CASE          "case"
%token KW_CONTINUE      "continue"
%token KW_DEFAULT       "default"
%token KW_DO            "do"
%token KW_ELSE          "else"
%token KW_FOR           "for"
%token KW_GOTO          "goto"
%token KW_IF            "if"
%token KW_INLINE        "inline"
%token KW_RETURN        "return"
%token KW_RESTRICT      "restrict"
%token KW_SIZEOF        "sizeof"
%token KW_SWITCH        "switch"
%token KW_WHILE         "while"

%token DATA_AUTO        "auto"
%token DATA_CHAR        "char"
%token DATA_CONST       "const"
%token DATA_DOUBLE      "double"
%token DATA_ENUM        "enum"
%token DATA_EXTERN      "extern"
%token DATA_FLOAT       "float"
%token DATA_INT         "int"
%token DATA_LONG        "long"
%token DATA_REGISTER    "register"
%token DATA_SHORT       "short"
%token DATA_SIGNED      "signed"
%token DATA_STATIC      "static"
%token DATA_STRUCT      "struct"
%token DATA_TYPEDEF     "typedef"
%token DATA_UNION       "union"
%token DATA_UNSIGNED    "unsigned"
%token DATA_VOLATILE    "volatile"
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
%token O_XOR_ASSIGN     "^="
%token O_DECREMENT      "--"
%token O_INCREMENT      "++"

/* Operator precedence (the one declared later has the higher precedence). */
%left ","
%right "&=" "^=" "|="
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
%type<expr>     decl            decl_list   dt      dt_attr     member_access               union       union_body
%type<expr>     struct          struct_body enum    enum_body   ptr_attr    declarations    expr_comma 
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
    | declarations                  {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_VAR, $declarations);
                                    }
    | dt_attr datatype id "(" param_list ")" stmt
                                    {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_FUNC, $dt_attr->appendLink($datatype)->appendLink($id)->appendLink($param_list));
                                        $$->createBranch($7);
                                    }
    | "inline" dt_attr datatype id "(" param_list ")" stmt
                                    {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_INLINE_FUNC, $dt_attr->appendLink($datatype)->appendLink($id)->appendLink($param_list));
                                        $$->createBranch($8);
                                    }
    | "typedef" datatype id ";"     {
                                         $$ = new CompilerStatement(LOC(@$), C_STMT_TYPEDEF, $datatype->appendLink($id));
                                    }
    | "if" "(" expr_comma ")" stmt  {
                                        CompilerStatement *ifBlock = new CompilerStatement(LOC(@1), C_STMT_IF, $3);
                                        ifBlock->createBranch($5);

                                        $$ = new CompilerStatement(LOC(@$), C_STMT_CONDITION);
                                        $$->createBranch(ifBlock);
                                    }
    | "if" "(" expr_comma ")" stmt "else" stmt
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
    | "for" "(" declarations e_expr ";" e_expr ")" stmt
                                    {
                                        $$ = new CompilerStatement(LOC(@$),
                                                                   C_STMT_FOR,
                                                                   $3->appendLink($4)->appendLink($6));
                                        $$->createBranch($8);
                                    }
    | "while" "(" expr_comma ")" stmt     {
                                        $$ = (new CompilerStatement(LOC(@$), C_STMT_WHILE, $3))->createBranch($5);
                                    }
    | "do" stmt "while" "(" expr_comma ")" ";"
                                    {
                                        $$ = (new CompilerStatement(LOC(@$), C_STMT_DO_WHILE, $5))->createBranch($2);
                                    }
    | "switch" "(" expr_comma ")" "{" switch_body "}"
                                    {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_SWITCH, $3);
                                        $$->createBranch($switch_body);
                                    }
    | "goto" id                     {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_GOTO, $id);
                                    }
;


// `switch' statement body.
switch_body:
      /* empty */                   { $$ = NULL;                                               }
    | cases statements              { $$ = $cases->createBranch($statements);                  }
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
      "{" /* empty */ "}"           {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_SCOPE);
                                    }
    | "{" statements "}"            {
                                        $$ = new CompilerStatement(LOC(@$), C_STMT_SCOPE);
                                        $$->createBranch($statements);
                                    }
;


// Identifier.
id:
      IDENTIFIER                    { $$ = new CompilerExpr($IDENTIFIER, LOC(@$));                  }
;


ptr_attr:
      /* Empty */                   {
                                        $$ = NULL;
                                    }
    | "*" ptr_attr                  {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_PTR),
                                                              CompilerExpr::OPER_PTRATTR,
                                                              $2,
                                                              LOC(@$));
                                    }
    | "const"                       {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_CONST),
                                                              CompilerExpr::OPER_PTRATTR,
                                                              LOC(@$));
                                    }
    | "restrict"                    {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_RESTRICT),
                                                              CompilerExpr::OPER_PTRATTR,
                                                              LOC(@$));
                                    }
;


decl:
      id                            {
                                        $$ = $id;
                                    }
    | id indexes                    {
                                        $$ = new CompilerExpr($1, CompilerExpr::OPER_INDEX, $2, LOC(@$));
                                    }
    | id indexes "=" "{" expr_comma "}"
                                    {
                                        $$ = new CompilerExpr($1,
                                                              CompilerExpr::OPER_INDEX,
                                                              $2->appendLink($5),
                                                              LOC(@$));
                                    }
    | decl "=" expr                 {
                                        $$ = new CompilerExpr($1, CompilerExpr::OPER_ASSIGN, $expr, LOC(@$));
                                    }
;


// List of function parameters.
decl_list:
      decl                          { $$ = $decl;                                                   }
    | decl_list "," decl            { $$ = $1->appendLink($decl);                                   }
;


declarations:
    dt_attr datatype decl_list ";"  { $$ = $1->appendLink($2)->appendLink($3);                      }
;




param:
      datatype id                   {
                                        $$ = $datatype->appendLink($id);
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
      /* empty */                   { $$ = new CompilerExpr(LOC(@$));                               }
    | expr_comma                    { $$ = $expr_comma;                                             }
;


expr_comma:
      expr                          { $$ = $expr; }
    | expr_comma "," expr           {
                                        $$ = new CompilerExpr($1, CompilerExpr::OPER_COMMA, $3, LOC(@$));
                                    }
;


// String constant.
string:
      STRING                        {
                                         $$ = new CompilerExpr(std::string($STRING.data, $STRING.size), LOC(@$));
                                    }
;


datatype:
     dt                             { $$ = $dt;                          }
    | datatype dt                   { $$ = $1->appendLink($dt);          }
    | enum                          {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_ENUM),
                                                              CompilerExpr::OPER_DATATYPE,
                                                              $1,
                                                              LOC(@$));
                                    }
    | struct                        {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_STRUCT),
                                                              CompilerExpr::OPER_DATATYPE,
                                                              $1,
                                                              LOC(@$));
                                    }
    | union                         {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_UNION),
                                                              CompilerExpr::OPER_DATATYPE,
                                                              $1,
                                                              LOC(@$));
                                    }
;

// Datatypes
dt:
     id                            {
                                        $$ = new CompilerExpr($id,
                                                              CompilerExpr::OPER_CUSTOM_DATATYPE,
                                                              LOC(@$));
                                    }
    | "*" ptr_attr                  {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_PTR),
                                                              CompilerExpr::OPER_PTRATTR,
                                                              $ptr_attr,
                                                              LOC(@$));
                                    }
    | "char"                        {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_CHAR),
                                                              CompilerExpr::OPER_DATATYPE,
                                                              LOC(@$));
                                    }
    | "double"                      {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_DOUBLE),
                                                              CompilerExpr::OPER_DATATYPE,
                                                              LOC(@$));
                                    }
    | "float"                       {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_FLOAT),
                                                              CompilerExpr::OPER_DATATYPE,
                                                              LOC(@$));
                                    }
    | "int"                         {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_INT),
                                                              CompilerExpr::OPER_DATATYPE,
                                                              LOC(@$));
                                    }
    | "long"                        {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_LONG),
                                                              CompilerExpr::OPER_DATATYPE,
                                                              LOC(@$));
                                    }
    | "short"                       {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_SHORT),
                                                              CompilerExpr::OPER_DATATYPE,
                                                              LOC(@$));
                                    }
    | "signed"                      {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_SIGNED),
                                                              CompilerExpr::OPER_DATATYPE,
                                                              LOC(@$));
                                    }
    | "unsigned"                    {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_UNSIGNED),
                                                              CompilerExpr::OPER_DATATYPE,
                                                              LOC(@$));
                                    }
    | "void"                        {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_VOID),
                                                              CompilerExpr::OPER_DATATYPE,
                                                              LOC(@$));
                                    }
;


dt_attr:
      /* Empty */                   {
                                        $$ = NULL;
                                    }
    | "auto"                        {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_AUTO),
                                                              CompilerExpr::OPER_DATAATTR,
                                                              LOC(@$));
                                    }
    | "const"                       {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_CONST),
                                                              CompilerExpr::OPER_DATAATTR,
                                                              LOC(@$));
                                    }
    | "extern"                      {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_EXTERN),
                                                              CompilerExpr::OPER_DATAATTR,
                                                              LOC(@$));
                                    }
    | "register"                    {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_REGISTER),
                                                              CompilerExpr::OPER_DATAATTR,
                                                              LOC(@$));
                                    }
    | "static"                      {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_STATIC),
                                                              CompilerExpr::OPER_DATAATTR,
                                                              LOC(@$));
                                    }
    | "volatile"                    {
                                        $$ = new CompilerExpr(CompilerValue(CompilerCDatatypes::DT_VOLATILE),
                                                              CompilerExpr::OPER_DATAATTR,
                                                              LOC(@$));
                                    }
;


struct:
      "struct" id "{" struct_body "}"   {
                                            $$ = new CompilerExpr($id,
                                                                  CompilerExpr::OPER_DATATYPE,
                                                                  $struct_body,
                                                                  LOC(@$));
                                        }
    | "struct" "{" struct_body "}"      {
                                            $$ = new CompilerExpr(NULL,
                                                                  CompilerExpr::OPER_DATATYPE,
                                                                  $struct_body,
                                                                  LOC(@$));
                                        }
    | "struct" id                       {
                                            $$ = new CompilerExpr($id, CompilerExpr::OPER_DATATYPE, LOC(@$));
                                        }
    | "struct" id "{" "}"               {
                                            $$ = new CompilerExpr($id, CompilerExpr::OPER_DATATYPE, LOC(@$));
                                        }
    | "struct" "{" "}"                  {
                                            $$ = NULL;
                                        }
;


struct_body:
      declarations                      { $$ = $declarations;                           }
    | struct_body declarations          { $$ = $1->appendLink($declarations); }
;


enum:
      "enum" id "{" enum_body "}"       {
                                            $$ = new CompilerExpr($id, CompilerExpr::OPER_DATATYPE, $enum_body, LOC(@$));
                                        }
    | "enum" "{" enum_body "}"          {
                                            $$ = new CompilerExpr(NULL, CompilerExpr::OPER_DATATYPE, $enum_body, LOC(@$));
                                        }
    | "enum" id                         {
                                            $$ = new CompilerExpr($id, CompilerExpr::OPER_DATATYPE, LOC(@$));
                                        }
;


enum_body:
      /* Empty */                       { $$ = NULL;                                                             }
    | id                                { $$ = $id;                                                              }
    | id "=" expr                       { $$ = new CompilerExpr($id, CompilerExpr::OPER_ASSIGN, $expr, LOC(@$)); }
    | enum_body "," id                  { $$ = new CompilerExpr($1, CompilerExpr::OPER_COMMA, $id, LOC(@$));     }
;


union:  
      "union" id "{" union_body "}"     {
                                            $$ = new CompilerExpr($id,
                                                                  CompilerExpr::OPER_DATATYPE,
                                                                  $union_body,
                                                                  LOC(@$));
                                        }
    | "union" "{" union_body "}"        {
                                            $$ = new CompilerExpr(NULL,
                                                                  CompilerExpr::OPER_DATATYPE,
                                                                  $union_body,
                                                                  LOC(@$));
                                        }
    | "union" id                        {
                                            $$ = new CompilerExpr($id, CompilerExpr::OPER_DATATYPE, LOC(@$));
                                        }
    | "union" id "{" "}"                {
                                            $$ = new CompilerExpr($id, CompilerExpr::OPER_DATATYPE, LOC(@$));
                                        }
    | "union" "{" "}"                   { $$ = NULL; }
;


union_body:
     declarations                       { $$ = $declarations;                 }
    | union_body declarations           { $$ = $1->appendLink($declarations); }
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
    | "&" expr                      { $$ = new CompilerExpr($2, CompilerExpr::OPER_REF, LOC(@$));                }
    | "*" expr                      { $$ = new CompilerExpr($2, CompilerExpr::OPER_DEREF, LOC(@$));              }


    // Ternary operator.
    | expr "?" expr ":" expr        {
                                        CompilerExpr *choices = new CompilerExpr($3,
                                                                                 CompilerExpr::OPER_COLON,
                                                                                 $5,
                                                                                 LOC(@$));
                                        $$ = new CompilerExpr($1, CompilerExpr::OPER_TERNARY, choices, LOC(@$));
                                    }

    // sizeof
    | "sizeof" "(" expr ")"         {
                                        $$ = new CompilerExpr($3, CompilerExpr::OPER_SIZEOF, LOC(@$));
                                    }

    // Array element access.
    | id indexes                    { $$ = new CompilerExpr($id, CompilerExpr::OPER_INDEX, $indexes, LOC(@$));   }

    // Struct index
    | member_access                 { $$ = $member_access; }

    // Function call
    | id "(" ")" %prec FCALL              { $$ = new CompilerExpr($id, CompilerExpr::OPER_CALL, LOC(@$));        }
    | id "(" expr_comma ")" %prec FCALL   { $$ = new CompilerExpr($id, CompilerExpr::OPER_CALL, $3, LOC(@$));    }
;


indexes:
      "[" expr "]"                  { $$ = $expr;                                                                }
    | indexes "[" expr "]"          { $$ = new CompilerExpr($1, CompilerExpr::OPER_INDEX, $expr, LOC(@$));       }
;


member_access:
      expr "." id                    { $$ = new CompilerExpr($expr, CompilerExpr::OPER_DOT, $id, LOC(@$));       }
    | expr "->" id                   { $$ = new CompilerExpr($expr, CompilerExpr::OPER_ARROW, $id, LOC(@$));     }
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
