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
/* %expect 39 */
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
    CompilerExpr      * expr;    //
    CompilerStatement * stmt;    //

    CompilerExpr::Operator oper; //

    int64_t integer;             //
    double real;                 //
    char * symbol;               //

    struct
    {
        char * data;             //
        unsigned int size;       //
    } string;                    //
};


/*
 * Interpreter's interface for the syntax analyzer; we need to have some things declared
 * before we can declare other things.
 */
%code requires
{
    #include "CompilerParserInterface.h"
    #include "CompilerCParserExtension.h"
    #include "CompilerCDeclaration.h"

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
    #define LOC(location)                                                                       \
        ( compiler->locationMap().translate ( CompilerSourceLocation ( 0,                       \
                                                                       location.first_line,     \
                                                                       location.last_line,      \
                                                                       location.first_column,   \
                                                                       location.last_column ) ) )

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
%token EOS              ";"
%token LOCATION_SPEC    "__at__"
%token INLINE_ASSEMBLY  "__asm__"
%token CRITICAL_BLOCK   "__critical__"
%token ELIPSIS          "..."

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
%token DATA_COMPLEX     "_Complex"
%token DATA_BOOL        "_Bool"
%token DATA_IMAGINARY   "_Imaginary"

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

/* Precedence for non operators, placed here to solve certain shift/reduce conflicts. */
%nonassoc IFX
%nonassoc "else"

/* Terminal symbols with semantic value. */
%token<symbol>    IDENTIFIER    "identifier"
%token<symbol>    TYPEDEF_NAME  "type name"
%token<symbol>    ENUM_CONST    "enumeration constant"
%token<string>    STRING        "string literal"
%token<string>    L_STRING      "long string literal"
%token<integer>   CHARACTER     "character constant"
%token<integer>   L_CHARACTER   "long character constant"
%token<integer>   INTEGER       "integer constant"
%token<integer>   U_INTEGER     "unsigned integer constant"
%token<integer>   L_INTEGER     "long integer constant"
%token<integer>   LL_INTEGER    "long long integer constant"
%token<integer>   UL_INTEGER    "unsigned long integer constant"
%token<integer>   ULL_INTEGER   "unsigned long long integer constant"
%token<real>      FLOAT         "floating constant"
%token<real>      DOUBLE        "double floating constant"
%token<real>      LONG_DOUBLE   "long double floating constant"

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Operators.
%type<oper> unary-operator                      assignment-operator
// Expressions.
%type<expr> constant                            integer-constant                floating-constant
%type<expr> enumeration-constant                character-constant              string-literal string
%type<expr> lstring                             identifier                      identifier-opt
%type<expr> primary-expression                  postfix-expression              argument-expression-list
%type<expr> argument-expression-list-opt        unary-expression                cast-expression
%type<expr> multiplicative-expression           additive-expression             shift-expression
%type<expr> relational-expression               equality-expression             AND-expression
%type<expr> exclusive-OR-expression             inclusive-OR-expression         logical-AND-expression
%type<expr> logical-OR-expression               conditional-expression          assignment-expression
%type<expr> assignment-expression-opt           expression                      expression-opt
%type<expr> constant-expression
// Statements.
/* %type<stmt> */

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
} <string>

// The start symbol.
%start translation-unit

// -----------------------------------------------------------------------------
// GRAMMAR RULES
// -----------------------------------------------------------------------------

%%

// =============================================================================
// PART 1: Expressions
// =============================================================================

constant:
    integer-constant
    {
        $$ = $[integer-constant];
    }

    | floating-constant
    {
        $$ = $[floating-constant];
    }

    | enumeration-constant
    {
        $$ = $[enumeration-constant];
    }

    | character-constant
    {
        $$ = $[character-constant];
    }
;

integer-constant:
    INTEGER
    {
        $$ = new CompilerExpr ( $[INTEGER], LOC(@$) );
    }

    | U_INTEGER
    {
        $$ = new CompilerExpr ( $[U_INTEGER],
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_UNSIGNED,
                                LOC(@$) );
    }

    | L_INTEGER
    {
        $$ = new CompilerExpr ( $[L_INTEGER],
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_LONG,
                                LOC(@$) );
    }

    | LL_INTEGER
    {
        $$ = new CompilerExpr ( $[LL_INTEGER],
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_LONG_LONG,
                                LOC(@$) );
    }

    | UL_INTEGER
    {
        $$ = new CompilerExpr ( $[UL_INTEGER],
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                new CompilerExpr ( CompilerCDeclaration::DT_UNSIGNED,
                                                   CompilerExpr::OPER_FIXED_DATATYPE,
                                                   CompilerCDeclaration::DT_LONG ),
                                LOC(@$) );
    }

    | ULL_INTEGER
    {
        $$ = new CompilerExpr ( $[ULL_INTEGER],
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                new CompilerExpr ( CompilerCDeclaration::DT_UNSIGNED,
                                                   CompilerExpr::OPER_FIXED_DATATYPE,
                                                   CompilerCDeclaration::DT_LONG_LONG ),
                                LOC(@$) );
    }
;

floating-constant:
    FLOAT
    {
        $$ = new CompilerExpr ( $[FLOAT], LOC(@$) );
    }

    | DOUBLE
    {
        $$ = new CompilerExpr ( $[DOUBLE],
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_LONG,
                                LOC(@$) );
    }

    | LONG_DOUBLE
    {
        $$ = new CompilerExpr ( $[LONG_DOUBLE],
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_LONG_LONG,
                                LOC(@$) );
    }
;

enumeration-constant:
    ENUM_CONST
    {
        $$ = new CompilerExpr ( $[ENUM_CONST], LOC(@$) );
    }
;

character-constant:
    CHARACTER
    {
        $$ = new CompilerExpr ( $[CHARACTER], LOC(@$) );
    }

    | L_CHARACTER
    {
        $$ = new CompilerExpr ( $[L_CHARACTER],
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_LONG,
                                LOC(@$) );
    }
;

string-literal:
    string
    {
        $$ = $[string];
    }

    | lstring
    {
        $$ = $[lstring];
    }
;

string:
    STRING
    {
        $$ = new CompilerExpr ( CompilerValue((unsigned char*) $[STRING].data, $[STRING].size, false), LOC(@$) );
    }

    | string STRING
    {
        $$ = $1;
        $$->m_location = LOC(@$);
        CompilerValue::Data::CharArray & lStr = $$->m_lValue.m_data.m_array;
        lStr.m_data = (unsigned char*) realloc(lStr.m_data, lStr.m_size + $[STRING].size + 1);
        memcpy(lStr.m_data + lStr.m_size, $[STRING].data, $[STRING].size);
        lStr.m_size += $[STRING].size;
        lStr.m_data[lStr.m_size] = '\0';
        free($[STRING].data);
    }
;

lstring:
    L_STRING
    {
        $$ = new CompilerExpr ( CompilerValue((unsigned char*) $[L_STRING].data, $[L_STRING].size, false),
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_LONG,
                                LOC(@$) );
    }

    | lstring STRING
    {
        $$ = $1;
        $$->m_location = LOC(@$);
        CompilerValue::Data::CharArray & lStr = $$->m_lValue.m_data.m_array;
        lStr.m_data = (unsigned char*) realloc(lStr.m_data, lStr.m_size + $[STRING].size + 1);
        memcpy(lStr.m_data + lStr.m_size, $[STRING].data, $[STRING].size);
        lStr.m_size += $[STRING].size;
        lStr.m_data[lStr.m_size] = '\0';
        free($[STRING].data);
    }
;

identifier:
    IDENTIFIER
    {
        $$ = new CompilerExpr ( $[IDENTIFIER], LOC(@$) );
    }
;

identifier-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | identifier
    {
        $$ = $[identifier];
    }
;

// -----------------------------------------------------------------------------
// PART 1.1: Primary expressions
// -----------------------------------------------------------------------------

primary-expression:
    identifier
    {
        $$ = $[identifier];
    }

    | constant
    {
        $$ = $[constant];
    }

    | string-literal
    {
        $$ = $[string-literal];
    }

    | "(" expression ")"
    {
        $$ = $[expression];
    }
;

// -----------------------------------------------------------------------------
// PART 1.2: Postfix operators
// -----------------------------------------------------------------------------

postfix-expression:
    primary-expression
    {
        $$ = $[primary-expression];
    }

    | postfix-expression "[" expression "]"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_INDEX,
                                $[expression],
                                LOC(@$) );
    }

    | postfix-expression "(" argument-expression-list-opt ")"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_CALL,
                                $[argument-expression-list-opt],
                                LOC(@$) );
    }

    | postfix-expression "." identifier
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_DOT,
                                $[identifier],
                                LOC(@$) );
    }

    | postfix-expression "->" identifier
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_ARROW,
                                $[identifier],
                                LOC(@$) );
    }

    | postfix-expression "++"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_INC,
                                LOC(@$) );
    }

    | postfix-expression "--"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_DEC,
                                LOC(@$) );
    }

    | "(" type-name ")" "{" initializer-list "}"
    {
        $$ = new CompilerExpr ( $[type-name],
                                CompilerExpr::OPER_CAST,
                                $[initializer-list]
                                LOC(@$) );
    }

    | "(" type-name ")" "{" initializer-list "," "}"
    {
        $$ = new CompilerExpr ( $[type-name],
                                CompilerExpr::OPER_CAST,
                                $[initializer-list]
                                LOC(@$) );
    }
;

argument-expression-list:
      assignment-expression
    | argument-expression-list "," assignment-expression
;

argument-expression-list-opt:
      /* empty */
    | argument-expression-list
;

// -----------------------------------------------------------------------------
// PART 1.3: Unary operators
// -----------------------------------------------------------------------------

unary-expression:
      postfix-expression
    | "++" unary-expression
    | "--" unary-expression
    | unary-operator cast-expression
    | "sizeof" unary-expression
    | "sizeof" "(" type-name ")"
;

unary-operator:
      "&"
    | "*"
    | "+"
    | "-"
    | "~"
    | "!"
;

// -----------------------------------------------------------------------------
// PART 1.4: Cast operators
// -----------------------------------------------------------------------------

cast-expression:
      unary-expression
    | "(" type-name ")" cast-expression
;

// -----------------------------------------------------------------------------
// PART 1.5: Multiplicative operators
// -----------------------------------------------------------------------------

multiplicative-expression:
      cast-expression
    | multiplicative-expression "*" cast-expression
    | multiplicative-expression "/" cast-expression
    | multiplicative-expression "%" cast-expression
;

// -----------------------------------------------------------------------------
// PART 1.6: Additive operators
// -----------------------------------------------------------------------------

additive-expression:
      multiplicative-expression
    | additive-expression "+" multiplicative-expression
    | additive-expression "-" multiplicative-expression
;

// -----------------------------------------------------------------------------
// PART 1.7: Bitwise shift operators
// -----------------------------------------------------------------------------

shift-expression:
      additive-expression
    | shift-expression "<<" additive-expression
    | shift-expression ">>" additive-expression
;

// -----------------------------------------------------------------------------
// PART 1.8: Relational operators
// -----------------------------------------------------------------------------

relational-expression:
      shift-expression
    | relational-expression "<" shift-expression
    | relational-expression ">" shift-expression
    | relational-expression "<=" shift-expression
    | relational-expression ">=" shift-expression
;

// -----------------------------------------------------------------------------
// PART 1.9: Equality operators
// -----------------------------------------------------------------------------

equality-expression:
      relational-expression
    | equality-expression "==" relational-expression
    | equality-expression "!=" relational-expression
;

// -----------------------------------------------------------------------------
// PART 1.10: Bitwise AND operator
// -----------------------------------------------------------------------------

AND-expression:
      equality-expression
    | AND-expression "&" equality-expression
;

// -----------------------------------------------------------------------------
// PART 1.11: Bitwise exclusive OR operator
// -----------------------------------------------------------------------------

exclusive-OR-expression:
      AND-expression
    | exclusive-OR-expression "^" AND-expression
;

// -----------------------------------------------------------------------------
// PART 1.12: Bitwise inclusive OR operator
// -----------------------------------------------------------------------------

inclusive-OR-expression:
      exclusive-OR-expression
    | inclusive-OR-expression "|" exclusive-OR-expression
;

// -----------------------------------------------------------------------------
// PART 1.13: Logical AND operator
// -----------------------------------------------------------------------------

logical-AND-expression:
      inclusive-OR-expression
    | logical-AND-expression "&&" inclusive-OR-expression
;

// -----------------------------------------------------------------------------
// PART 1.14: Logical OR operator
// -----------------------------------------------------------------------------

logical-OR-expression:
      logical-AND-expression
    | logical-OR-expression "||" logical-AND-expression
;

// -----------------------------------------------------------------------------
// PART 1.15: Conditional operator
// -----------------------------------------------------------------------------

conditional-expression:
      logical-OR-expression
    | logical-OR-expression "?" expression ":" conditional-expression
;

// -----------------------------------------------------------------------------
// PART 1.16: Assignment operators
// -----------------------------------------------------------------------------

assignment-expression:
      conditional-expression
    | unary-expression assignment-operator assignment-expression
;

assignment-expression-opt:
      /* empty */
    | assignment-expression
;

assignment-operator:
      "="
    | "*="
    | "/="
    | "%="
    | "+="
    | "-="
    | "<<="
    | ">>="
    | "&="
    | "^="
    | "|="
;

// -----------------------------------------------------------------------------
// PART 1.17: Comma operator
// -----------------------------------------------------------------------------

expression:
      assignment-expression
    | expression "," assignment-expression
;

expression-opt:
      /* empty */
    | expression
;

// -----------------------------------------------------------------------------
// PART 1.17: Constant expression
// -----------------------------------------------------------------------------

constant-expression:
      conditional-expression
;

// =============================================================================
// PART 2: Declarations
// =============================================================================

declaration:
      declaration-specifiers init-declarator-list-opt ";"
;

declaration-specifiers:
      storage-class-specifier declaration-specifiers-opt
    | type-specifier declaration-specifiers-opt
    | type-qualifier declaration-specifiers-opt
    | function-specifier declaration-specifiers-opt
    | allocation-specifier declaration-specifiers-opt
;

declaration-specifiers-opt:
      /* empty */
    | declaration-specifiers
;

init-declarator-list-opt:
      /* empty */
    | init-declarator-list
;

init-declarator-list:
      init-declarator
    | init-declarator-list "," init-declarator
;

init-declarator:
      declarator
    | declarator "=" initializer
;

// -----------------------------------------------------------------------------
// PART 2.1: Storage class specifiers
// -----------------------------------------------------------------------------

storage-class-specifier:
      "typedef"
    | "extern"
    | "static"
    | "auto"
    | "register"
;

// -----------------------------------------------------------------------------
// PART 2.2: Type specifiers
// -----------------------------------------------------------------------------

type-specifier:
      "void"
    | "char"
    | "short"
    | "int"
    | "long"
    | "float"
    | "double"
    | "signed"
    | "unsigned"
    | "_Bool"
    | "_Complex"
    | "_Imaginary"
    | struct-or-union-specifier
    | enum-specifier
    | typedef-name
;

// -----------------------------------------------------------------------------
// PART 2.2.1:  Structure and union specifiers
// -----------------------------------------------------------------------------

struct-or-union-specifier:
      struct-or-union identifier-opt "{" struct-declaration-list "}"
    | struct-or-union identifier
;

struct-or-union:
      "struct"
    | "union"
;

struct-declaration-list:
      struct-declaration
    | struct-declaration-list struct-declaration
;

struct-declaration:
      specifier-qualifier-list struct-declarator-list ";"
;

specifier-qualifier-list:
      specifier-qualifier
    | specifier-qualifier-list specifier-qualifier
;

specifier-qualifier:
      type-specifier
    | type-qualifier
;

struct-declarator-list:
      struct-declarator
    | struct-declarator-list "," struct-declarator
;

struct-declarator:
      declarator
    | declarator-opt ":" constant-expression
;

// -----------------------------------------------------------------------------
// PART 2.2.2:  Enumeration specifiers
// -----------------------------------------------------------------------------

enum-specifier:
      "enum" identifier-opt "{" enumerator-list "}"
    | "enum" identifier-opt "{" enumerator-list "," "}"
    | "enum" identifier
;

enumerator-list:
      enumerator
    | enumerator-list "," enumerator
;

enumerator:
      enumeration-constant
    | enumeration-constant "=" constant-expression
;

// -----------------------------------------------------------------------------
// PART 2.3:  Type qualifier
// -----------------------------------------------------------------------------

type-qualifier:
      "const"
    | "restrict"
    | "volatile"
;

// -----------------------------------------------------------------------------
// PART 2.4:  Function specifier
// -----------------------------------------------------------------------------

function-specifier:
      "inline"
;

// -----------------------------------------------------------------------------
// PART 2.5:  Declarator
// -----------------------------------------------------------------------------

declarator:
      pointer-opt direct-declarator
;

declarator-opt:
      /* empty */
    | declarator
;

direct-declarator:
      identifier
    | "(" declarator ")"
    | direct-declarator "[" type-qualifier-list-opt assignment-expression-opt "]"
    | direct-declarator "[" "static" type-qualifier-list-opt assignment-expression "]"
    | direct-declarator "[" type-qualifier-list "static" assignment-expression "]"
    | direct-declarator "[" type-qualifier-list-opt "*" "]"
    | direct-declarator "(" parameter-type-list ")"
    | direct-declarator "(" identifier-list-opt ")"
;

pointer:
      "*" type-qualifier-list-opt
    | "*" type-qualifier-list-opt pointer
;

pointer-opt:
      /* empty */
    | pointer
;

type-qualifier-list:
      type-qualifier
    | type-qualifier-list type-qualifier
;

type-qualifier-list-opt:
      /* empty */
    | type-qualifier-list
;

parameter-type-list:
      parameter-list
    | parameter-list "," "..."
;

parameter-list:
      parameter-declaration
    | parameter-list "," parameter-declaration
;

parameter-declaration:
      declaration-specifiers declarator
    | declaration-specifiers abstract-declarator-opt
;

identifier-list:
      identifier
    | identifier-list "," identifier
;

identifier-list-opt:
      /* empty */
    | identifier-list
;

// -----------------------------------------------------------------------------
// PART 2.6:  Type names
// -----------------------------------------------------------------------------

type-name:
      specifier-qualifier-list abstract-declarator-opt
;

abstract-declarator:
      pointer
    | pointer-opt direct-abstract-declarator
;

abstract-declarator-opt:
      /* empty */
    | abstract-declarator
;

direct-abstract-declarator:
      "(" abstract-declarator ")"
    | direct-abstract-declarator-opt "[" assignment-expression-opt "]"
    | direct-abstract-declarator-opt "[" "*" "]"
    | direct-abstract-declarator-opt "[" parameter-type-list "]"
;

direct-abstract-declarator-opt:
      /* empty */
    | direct-abstract-declarator
;

// -----------------------------------------------------------------------------
// PART 2.7:  Type definitions
// -----------------------------------------------------------------------------
/* (CompilerCParserExtension*) (compiler->m_parserExtension) */
typedef-name:
      TYPEDEF_NAME
;

// -----------------------------------------------------------------------------
// PART 2.8:  Initialization
// -----------------------------------------------------------------------------

initializer:
      assignment-expression
    | "{" initializer-list "}"
    | "{" initializer-list "," "}"
;

initializer-list:
      designation-opt initializer
    | initializer-list "," designation-opt initializer
;

designation:
      designator-list "="
;

designation-opt:
      /* empty */
    | designation
;

designator-list:
      designator
    | designator-list designator
;

designator:
      "[" constant-expression "]"
    | "." identifier
;

// =============================================================================
// PART 3: Statements and blocks
// =============================================================================

statement:
      labeled-statement
    | compound-statement
    | expression-statement
    | selection-statement
    | iteration-statement
    | jump-statement
    | inline-assembly
;

// -----------------------------------------------------------------------------
// PART 3.1:  Labeled statements
// -----------------------------------------------------------------------------

labeled-statement:
      identifier ":" statement
    | "case" constant-expression ":" statement
    | "default" constant-expression ":" statement
;

// -----------------------------------------------------------------------------
// PART 3.2:  Compound statement
// -----------------------------------------------------------------------------

compound-statement:
      critical-specifier-opt "{" block-item-list-opt "}"
;

block-item-list:
      block-item
    | block-item-list block-item
;

block-item-list-opt:
      /* empty */
    | block-item-list
;

block-item:
      declaration
    | statement
;

// -----------------------------------------------------------------------------
// PART 3.3: Expression and null statements
// -----------------------------------------------------------------------------

expression-statement:
    expression-opt ";"
;

// -----------------------------------------------------------------------------
// PART 3.4: Selection statements
// -----------------------------------------------------------------------------

selection-statement:
      "if" "(" expression ")" statement %prec IFX
    | "if" "(" expression ")" statement "else" statement
    | "switch" "(" expression ")" statement
;

// -----------------------------------------------------------------------------
// PART 3.5: Iteration statements
// -----------------------------------------------------------------------------

iteration-statement:
      "while" "(" expression ")" statement
    | "do" statement "while" "(" expression ")" ";"
    | "for" "(" expression-opt ";" expression-opt ";" expression-opt ")" statement
    | "for" "(" declaration expression-opt ";" expression-opt ")" statement
;

// -----------------------------------------------------------------------------
// PART 3.6: Jump statements
// -----------------------------------------------------------------------------

jump-statement:
      "goto" identifier ";"
    | "continue" ";"
    | "break" ";"
    | "return" expression-opt ";"
;

// =============================================================================
// PART 4: External definitions
// =============================================================================

translation-unit:
      external-declaration
    | translation-unit external-declaration
;

external-declaration:
      function-definition
    | declaration
;

// -----------------------------------------------------------------------------
// PART 4.1: Function definitions
// -----------------------------------------------------------------------------

function-definition:
      declaration-specifiers declarator declaration-list-opt compound-statement
;

declaration-list:
      declaration
    | declaration-list declaration
;

declaration-list-opt:
      /* empty */
    | declaration-list
;

// =============================================================================
// PART 5: Implementation specific extensions
// =============================================================================

allocation-specifier:
      "__at__" "(" string-literal "," integer-constant ")"
/*                                     { $$ = new CompilerExpr($string, CompilerExpr::OPER_AT, $expr, LOC(@$)); } */
;

critical-specifier-opt:
      /* empty */
    | "__critical__"
;

inline-assembly:
      "__asm__" "(" string-literal ")" ";"
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE
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
