// =============================================================================
/**
 * @brief
 *
 * ...
 *
 * (C) copyright 2014, 2015 Moravia Microsystems, s.r.o.
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
    CompilerExpr      * expr;    //
    CompilerStatement * stmt;    //

    //
    CompilerExpr::Operator oper;

    //
    CompilerCDeclaration::Type type;

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
 * We need to have some things declared before we can declare other things...
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

    #define ALTER_SYMBOL_TYPE(symbol, type) \
        dynamic_cast<CompilerCParserExtension*>(compiler->m_parserExtension)-> \
            newIdentifier(symbol->lVal().m_data.m_symbol, CompilerCParserExtension::type);

    #define ENTER_SCOPE() \
        dynamic_cast<CompilerCParserExtension*>(compiler->m_parserExtension)->enterScopeBlock();

    #define LEAVE_SCOPE() \
        dynamic_cast<CompilerCParserExtension*>(compiler->m_parserExtension)->leaveScopeBlock();

    #define TYPEDEF_FLAG \
        dynamic_cast<CompilerCParserExtension*>(compiler->m_parserExtension)->m_typedef

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
%token KW_ALIGNAS       "_Alignas"
%token KW_ALIGNOF       "_Alignof"
%token KW_ATOMIC        "_Atomic"
%token KW_GENERIC       "_Generic"
%token KW_NORETURN      "_Noreturn"
%token KW_STATIC_ASSERT "_Static_assert"

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
%token DATA_THR_LOCAL   "_Thread_local"

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
%token<string>    W_STRING      "wide string literal"
%token<string>    W16_STRING    "char16_t wide string literal"
%token<string>    W32_STRING    "char32_t wide string literal"
%token<string>    U8_STRING     "UTF-8 string literal"
%token<integer>   CHARACTER     "character constant"
%token<integer>   W_CHARACTER   "wide character constant"
%token<integer>   W16_CHARACTER "char16_t wide character constant"
%token<integer>   W32_CHARACTER "char32_t wide character constant"
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
// Expressions.
%type<expr> constant                            integer-constant                floating-constant
%type<expr> character-constant                  string                          string-literal
%type<expr> init-declarator-std                 identifier                      identifier-opt
%type<expr> primary-expression                  postfix-expression              argument-expression-list
%type<expr> argument-expression-list-opt        unary-expression                cast-expression
%type<expr> multiplicative-expression           additive-expression             shift-expression
%type<expr> relational-expression               equality-expression             AND-expression
%type<expr> exclusive-OR-expression             inclusive-OR-expression         logical-AND-expression
%type<expr> logical-OR-expression               conditional-expression          assignment-expression
%type<expr> assignment-expression-opt           expression                      expression-opt
%type<expr> constant-expression                 allocation-specifier            init-declarator-list-opt
%type<expr> init-declarator-list                init-declarator                 struct-or-union-specifier
%type<expr> struct-declaration-list             struct-declaration              struct-declarator-list
%type<expr> struct-declarator                   enum-specifier                  enumerator-list
%type<expr> enumerator                          declarator                      declarator-opt
%type<expr> direct-declarator                   pointer                         pointer-opt
%type<expr> parameter-type-list                 parameter-list                  parameter-declaration
%type<expr> identifier-list                     identifier-list-opt             type-name
%type<expr> abstract-declarator                 abstract-declarator-opt         direct-abstract-declarator
%type<expr> direct-abstract-declarator-opt      typedef-name                    initializer
%type<expr> initializer-list                    designation                     designation-opt
%type<expr> designator-list                     designator                      declaration-list
%type<expr> declaration-list-opt                declaration                     declaration-specifiers
%type<expr> declaration-specifiers-opt          specifier-qualifier             specifier-qualifier-list
%type<expr> type-specifier                      type-qualifier-list             type-qualifier-list-opt
%type<expr> generic-selection                   generic-assoc-list              generic-association
%type<expr> enumeration-constant                alignment-specifier             static_assert-declaration
%type<expr> atomic-type-specifier
// Data types.
%type<type> storage-class-specifier             struct-or-union                 function-specifier
%type<type> type-qualifier
// Statements.
%type<stmt> inline-assembly                     statement                       labeled-statement
%type<stmt> compound-statement                  block-item-list                 block-item-list-opt
%type<stmt> block-item                          expression-statement            selection-statement
%type<stmt> iteration-statement                 jump-statement                  translation-unit
%type<stmt> external-declaration                function-definition
// Operators.
%type<oper> unary-operator                      assignment-operator

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
%start start-symbol

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
                                                   CompilerExpr::OPER_DATATYPE,
                                                   CompilerCDeclaration::DT_LONG ),
                                LOC(@$) );
    }

    | ULL_INTEGER
    {
        $$ = new CompilerExpr ( $[ULL_INTEGER],
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                new CompilerExpr ( CompilerCDeclaration::DT_UNSIGNED,
                                                   CompilerExpr::OPER_DATATYPE,
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

    | W_CHARACTER
    {
        $$ = new CompilerExpr ( $[W_CHARACTER],
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_SHORT,
                                LOC(@$) );
    }

    | W16_CHARACTER
    {
        $$ = new CompilerExpr ( $[W16_CHARACTER],
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_INT,
                                LOC(@$) );
    }

    | W32_CHARACTER
    {
        $$ = new CompilerExpr ( $[W32_CHARACTER],
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_LONG,
                                LOC(@$) );
    }
;

string:
    STRING
    {
        $$ = new CompilerExpr ( CompilerValue((unsigned char*) $[STRING].data, $[STRING].size, false), LOC(@$) );
    }

    | W_STRING
    {
        $$ = new CompilerExpr ( CompilerValue((unsigned char*) $[W_STRING].data, $[W_STRING].size, false),
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_SHORT,
                                LOC(@$) );
    }

    | W16_STRING
    {
        $$ = new CompilerExpr ( CompilerValue((unsigned char*) $[W16_STRING].data, $[W16_STRING].size, false),
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_INT,
                                LOC(@$) );
    }

    | W32_STRING
    {
        $$ = new CompilerExpr ( CompilerValue((unsigned char*) $[W32_STRING].data, $[W32_STRING].size, false),
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_LONG,
                                LOC(@$) );
    }

    | U8_STRING
    {
        $$ = new CompilerExpr ( CompilerValue((unsigned char*) $[U8_STRING].data, $[U8_STRING].size, false),
                                CompilerExpr::OPER_FIXED_DATATYPE,
                                CompilerCDeclaration::DT_LONG_LONG,
                                LOC(@$) );
    }

string-literal:
    string
    {
        $$ = $[string];
    }

    | string-literal string
    {
        $$ = $1;
        $$->m_location = LOC(@$);

        CompilerValue::Data::CharArray * lStr;
        if ( CompilerValue::TYPE_EXPR == $$->m_lValue.m_type )
        {
            lStr = &( $$->m_lValue.m_data.m_expr->m_lValue.m_data.m_array );
        }
        else
        {
            lStr = &( $$->m_lValue.m_data.m_array );
        }

        CompilerValue::Data::CharArray * rStr;
        if ( CompilerValue::TYPE_EXPR == $[string]->m_lValue.m_type )
        {
            lStr = &( $[string]->m_lValue.m_data.m_expr->m_lValue.m_data.m_array );
        }
        else
        {
            lStr = &( $[string]->m_lValue.m_data.m_array );
        }

        lStr->m_data = (unsigned char*) realloc(lStr->m_data, lStr->m_size + rStr->m_size + 1);
        memcpy(lStr->m_data + lStr->m_size, rStr->m_data, rStr->m_size);
        lStr->m_size += rStr->m_size;
        lStr->m_data[lStr->m_size] = '\0';
        free(rStr->m_data);
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

comma-opt:
    /* empty */
    | ","
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

    | generic-selection
    {
        $$ = $[generic-selection];
    }
;

generic-selection:
    "_Generic" "(" assignment-expression "," generic-assoc-list ")"
    {
        $$ = new CompilerExpr ( $[assignment-expression],
                                CompilerExpr::OPER_GENERIC,
                                $[generic-assoc-list],
                                LOC(@$) );
    }
;

generic-assoc-list:
    generic-association
    {
        $$ = $[generic-association];
    }

    | generic-assoc-list "," generic-association
    {
        $$ = $1->appendLink($[generic-association]);
    }
;

generic-association:
    type-name ":" assignment-expression
    {
        $$ = new CompilerExpr ( $[type-name],
                                CompilerExpr::OPER_COLON,
                                $[assignment-expression],
                                LOC(@$) );
    }

    | "default" ":" assignment-expression
    {
        $$ = new CompilerExpr ( (CompilerExpr*) nullptr,
                                CompilerExpr::OPER_COLON,
                                $[assignment-expression],
                                LOC(@$) );
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

    | "(" type-name ")" "{" initializer-list comma-opt "}"
    {
        $$ = new CompilerExpr ( $[type-name],
                                CompilerExpr::OPER_CAST,
                                $[initializer-list],
                                LOC(@$) );
    }
;

argument-expression-list:
    assignment-expression
    {
        $$ = $[assignment-expression];
    }

    | argument-expression-list "," assignment-expression
    {
        $$ = $1->appendLink($[assignment-expression]);
    }
;

argument-expression-list-opt:
      /* empty */
    {
        $$ = nullptr;
    }

    | argument-expression-list
    {
        $$ = $[argument-expression-list];
    }
;

// -----------------------------------------------------------------------------
// PART 1.3: Unary operators
// -----------------------------------------------------------------------------

unary-expression:
    postfix-expression
    {
        $$ = $[postfix-expression];
    }

    | "++" unary-expression
    {
        $$ = new CompilerExpr ( $2,
                                CompilerExpr::OPER_PRE_INC,
                                LOC(@$) );
    }

    | "--" unary-expression
    {
        $$ = new CompilerExpr ( $2,
                                CompilerExpr::OPER_PRE_DEC,
                                LOC(@$) );
    }

    | unary-operator cast-expression
    {
        $$ = new CompilerExpr ( $[cast-expression],
                                $[unary-operator],
                                LOC(@$) );
    }

    | "sizeof" unary-expression
    {
        $$ = new CompilerExpr ( $2,
                                CompilerExpr::OPER_SIZEOF,
                                LOC(@$) );
    }

    | "sizeof" "(" type-name ")"
    {
        $$ = new CompilerExpr ( $[type-name],
                                CompilerExpr::OPER_SIZEOF,
                                LOC(@$) );
    }

    | "_Alignof" "(" type-name ")"
    {
        $$ = new CompilerExpr ( $[type-name],
                                CompilerExpr::OPER_ALIGNOF,
                                LOC(@$) );
    }
;

unary-operator:
    "&"
    {
        $$ = CompilerExpr::OPER_ADDR;
    }

    | "*"
    {
        $$ = CompilerExpr::OPER_DEREF;
    }

    | "+"
    {
        $$ = CompilerExpr::OPER_INT_PROM;
    }

    | "-"
    {
        $$ = CompilerExpr::OPER_ADD_INV;
    }

    | "~"
    {
        $$ = CompilerExpr::OPER_CMPL;
    }

    | "!"
    {
        $$ = CompilerExpr::OPER_NOT;
    }
;

// -----------------------------------------------------------------------------
// PART 1.4: Cast operators
// -----------------------------------------------------------------------------

cast-expression:
    unary-expression
    {
        $$ = $[unary-expression];
    }

    | "(" type-name ")" cast-expression
    {
        $$ = new CompilerExpr ( $[type-name],
                                CompilerExpr::OPER_CAST,
                                $4,
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 1.5: Multiplicative operators
// -----------------------------------------------------------------------------

multiplicative-expression:
    cast-expression
    {
        $$ = $[cast-expression];
    }

    | multiplicative-expression "*" cast-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_MULT,
                                $[cast-expression],
                                LOC(@$) );
    }

    | multiplicative-expression "/" cast-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_DIV,
                                $[cast-expression],
                                LOC(@$) );
    }

    | multiplicative-expression "%" cast-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_MOD,
                                $[cast-expression],
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 1.6: Additive operators
// -----------------------------------------------------------------------------

additive-expression:
    multiplicative-expression
    {
        $$ = $[multiplicative-expression];
    }

    | additive-expression "+" multiplicative-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_ADD,
                                $[multiplicative-expression],
                                LOC(@$) );
    }

    | additive-expression "-" multiplicative-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_SUB,
                                $[multiplicative-expression],
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 1.7: Bitwise shift operators
// -----------------------------------------------------------------------------

shift-expression:
    additive-expression
    {
        $$ = $[additive-expression];
    }

    | shift-expression "<<" additive-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_SHL,
                                $[additive-expression],
                                LOC(@$) );
    }

    | shift-expression ">>" additive-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_SHR,
                                $[additive-expression],
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 1.8: Relational operators
// -----------------------------------------------------------------------------

relational-expression:
    shift-expression
    {
        $$ = $[shift-expression];
    }

    | relational-expression "<" shift-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_LT,
                                $[shift-expression],
                                LOC(@$) );
    }

    | relational-expression ">" shift-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_GT,
                                $[shift-expression],
                                LOC(@$) );
    }

    | relational-expression "<=" shift-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_LE,
                                $[shift-expression],
                                LOC(@$) );
    }

    | relational-expression ">=" shift-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_GE,
                                $[shift-expression],
                                LOC(@$) );
    }

;

// -----------------------------------------------------------------------------
// PART 1.9: Equality operators
// -----------------------------------------------------------------------------

equality-expression:
    relational-expression
    {
        $$ = $[relational-expression];
    }

    | equality-expression "==" relational-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_EQ,
                                $[relational-expression],
                                LOC(@$) );
    }

    | equality-expression "!=" relational-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_NE,
                                $[relational-expression],
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 1.10: Bitwise AND operator
// -----------------------------------------------------------------------------

AND-expression:
    equality-expression
    {
        $$ = $[equality-expression];
    }

    | AND-expression "&" equality-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_BAND,
                                $[equality-expression],
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 1.11: Bitwise exclusive OR operator
// -----------------------------------------------------------------------------

exclusive-OR-expression:
    AND-expression
    {
        $$ = $[AND-expression];
    }

    | exclusive-OR-expression "^" AND-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_BXOR,
                                $[AND-expression],
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 1.12: Bitwise inclusive OR operator
// -----------------------------------------------------------------------------

inclusive-OR-expression:
    exclusive-OR-expression
    {
        $$ = $[exclusive-OR-expression];
    }

    | inclusive-OR-expression "|" exclusive-OR-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_BOR,
                                $[exclusive-OR-expression],
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 1.13: Logical AND operator
// -----------------------------------------------------------------------------

logical-AND-expression:
    inclusive-OR-expression
    {
        $$ = $[inclusive-OR-expression];
    }

    | logical-AND-expression "&&" inclusive-OR-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_LAND,
                                $[inclusive-OR-expression],
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 1.14: Logical OR operator
// -----------------------------------------------------------------------------

logical-OR-expression:
    logical-AND-expression
    {
        $$ = $[logical-AND-expression];
    }

    | logical-OR-expression "||" logical-AND-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_LOR,
                                $[logical-AND-expression],
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 1.15: Conditional operator
// -----------------------------------------------------------------------------

conditional-expression:
    logical-OR-expression
    {
        $$ = $[logical-OR-expression];
    }

    | logical-OR-expression "?" expression ":" conditional-expression
    {
        $$ = new CompilerExpr ( $[logical-OR-expression],
                                CompilerExpr::OPER_CONDITION,
                                new CompilerExpr ( $[expression],
                                                   CompilerExpr::OPER_COLON,
                                                   $5 ),
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 1.16: Assignment operators
// -----------------------------------------------------------------------------

assignment-expression:
    conditional-expression
    {
        $$ = $[conditional-expression];
    }

    | unary-expression assignment-operator assignment-expression
    {
        $$ = new CompilerExpr ( $[unary-expression],
                                $[assignment-operator],
                                $3,
                                LOC(@$) );
    }
;

assignment-expression-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | assignment-expression
    {
        $$ = $[assignment-expression];
    }
;

assignment-operator:
    "="
    {
        $$ = CompilerExpr::OPER_ASSIGN;
    }

    | "*="
    {
        $$ = CompilerExpr::OPER_MUL_ASSIGN;
    }

    | "/="
    {
        $$ = CompilerExpr::OPER_DIV_ASSIGN;
    }

    | "%="
    {
        $$ = CompilerExpr::OPER_MOD_ASSIGN;
    }

    | "+="
    {
        $$ = CompilerExpr::OPER_ADD_ASSIGN;
    }

    | "-="
    {
        $$ = CompilerExpr::OPER_SUB_ASSIGN;
    }

    | "<<="
    {
        $$ = CompilerExpr::OPER_SHL_ASSIGN;
    }

    | ">>="
    {
        $$ = CompilerExpr::OPER_SHR_ASSIGN;
    }

    | "&="
    {
        $$ = CompilerExpr::OPER_AND_ASSIGN;
    }

    | "^="
    {
        $$ = CompilerExpr::OPER_XOR_ASSIGN;
    }

    | "|="
    {
        $$ = CompilerExpr::OPER_ORB_ASSIGN;
    }
;

// -----------------------------------------------------------------------------
// PART 1.17: Comma operator
// -----------------------------------------------------------------------------

expression:
    assignment-expression
    {
        $$ = $[assignment-expression];
    }

    | expression "," assignment-expression
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_COMMA,
                                $[assignment-expression],
                                LOC(@$) );
    }
;

expression-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | expression
    {
        $$ = $[expression];
    }
;

// -----------------------------------------------------------------------------
// PART 1.17: Constant expression
// -----------------------------------------------------------------------------

constant-expression:
    conditional-expression
    {
        $$ = $[conditional-expression];
    }
;

// =============================================================================
// PART 2: Declarations
// =============================================================================

declaration:
    declaration-specifiers init-declarator-list-opt ";"
    {
        $$ = new CompilerExpr ( $[declaration-specifiers],
                                CompilerExpr::OPER_DECLARATION,
                                $[init-declarator-list-opt],
                                LOC(@$) );

        TYPEDEF_FLAG = false;
    }
;

declaration-specifiers:
    storage-class-specifier declaration-specifiers-opt
    {
        $$ = new CompilerExpr ( $[declaration-specifiers-opt],
                                CompilerExpr::OPER_DATATYPE,
                                $[storage-class-specifier],
                                LOC(@$) );
    }

    | type-specifier declaration-specifiers-opt
    {
        $$ = new CompilerExpr ( $[declaration-specifiers-opt],
                                CompilerExpr::OPER_DATATYPE,
                                $[type-specifier],
                                LOC(@$) );
    }

    | type-qualifier declaration-specifiers-opt
    {
        $$ = new CompilerExpr ( $[declaration-specifiers-opt],
                                CompilerExpr::OPER_DATATYPE,
                                $[type-qualifier],
                                LOC(@$) );
    }

    | function-specifier declaration-specifiers-opt
    {
        $$ = new CompilerExpr ( $[declaration-specifiers-opt],
                                CompilerExpr::OPER_DATATYPE,
                                $[function-specifier],
                                LOC(@$) );
    }

    | alignment-specifier declaration-specifiers-opt
    {
        $$ = new CompilerExpr ( $[declaration-specifiers-opt],
                                CompilerExpr::OPER_DATATYPE,
                                $[alignment-specifier],
                                LOC(@$) );
    }
;

declaration-specifiers-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | declaration-specifiers
    {
        $$ = $[declaration-specifiers];
    }
;

init-declarator-list-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | init-declarator-list
    {
        $$ = $[init-declarator-list];
    }
;

init-declarator-list:
    init-declarator
    {
        $$ = $[init-declarator];
    }

    | init-declarator-list "," init-declarator
    {
        $$ = $1->appendLink($[init-declarator]);
    }
;

init-declarator:
    init-declarator-std
    {
        $$ = $[init-declarator-std];
    }

    | allocation-specifier init-declarator-std
    {
        $$ = new CompilerExpr ( $[allocation-specifier],
                                CompilerExpr::OPER_PAIR,
                                $[init-declarator-std],
                                LOC(@$) );
    }
;

init-declarator-std:
    declarator
    {
        $$ = $[declarator];
    }

    | declarator "=" initializer
    {
        $$ = new CompilerExpr ( $[declarator],
                                CompilerExpr::OPER_INIT,
                                $[initializer],
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 2.1: Storage class specifiers
// -----------------------------------------------------------------------------

storage-class-specifier:
    "typedef"
    {
        $$ = CompilerCDeclaration::A_TYPEDEF;
        TYPEDEF_FLAG = true;
    }

    | "extern"
    {
        $$ = CompilerCDeclaration::A_EXTERN;
    }

    | "static"
    {
        $$ = CompilerCDeclaration::A_STATIC;
    }

    | "_Thread_local"
    {
        $$ = CompilerCDeclaration::A_THREAD_LOCAL;
    }

    | "auto"
    {
        $$ = CompilerCDeclaration::A_AUTO;
    }

    | "register"
    {
        $$ = CompilerCDeclaration::A_REGISTER;
    }
;

// -----------------------------------------------------------------------------
// PART 2.2: Type specifiers
// -----------------------------------------------------------------------------

type-specifier:
    "void"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::DT_VOID),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }

    | "char"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::DT_CHAR),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }

    | "short"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::DT_SHORT),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }

    | "int"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::DT_INT),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }

    | "long"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::DT_LONG),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }

    | "float"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::DT_FLOAT),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }

    | "double"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::DT_DOUBLE),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }

    | "signed"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::DT_SIGNED),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }

    | "unsigned"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::DT_UNSIGNED),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }

    | "_Bool"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::DT_BOOL),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }

    | "_Complex"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::DT_COMPLEX),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }

    | "_Imaginary"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::DT_IMAGINARY),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }
/* // TODO:
    | atomic-type-specifier
    {
        $$ = $[atomic-type-specifier];
    }*/

    | struct-or-union-specifier
    {
        $$ = $[struct-or-union-specifier];
    }

    | enum-specifier
    {
        $$ = $[enum-specifier];
    }

    | typedef-name
    {
        $$ = $[typedef-name];
    }
;

// -----------------------------------------------------------------------------
// PART 2.2.1:  Structure and union specifiers
// -----------------------------------------------------------------------------

struct-or-union-specifier:
    struct-or-union identifier-opt "{" { ENTER_SCOPE(); } struct-declaration-list { LEAVE_SCOPE(); } "}"
    {
        $$ = new CompilerExpr ( $[struct-or-union],
                                CompilerExpr::OPER_DATATYPE,
                                new CompilerExpr ( $[identifier-opt],
                                                   CompilerExpr::OPER_PAIR,
                                                   $[struct-declaration-list] ),
                                LOC(@$) );
    }

    | struct-or-union identifier
    {
        $$ = new CompilerExpr ( $[struct-or-union],
                                CompilerExpr::OPER_DATATYPE,
                                $[identifier],
                                LOC(@$) );
    }
;

struct-or-union:
    "struct"
    {
        $$ = CompilerCDeclaration::DT_STRUCT;
    }

    | "union"
    {
        $$ = CompilerCDeclaration::DT_UNION;
    }
;

struct-declaration-list:
    struct-declaration
    {
        $$ = $[struct-declaration];
    }

    | struct-declaration-list struct-declaration
    {
        $$ = $1->appendLink($[struct-declaration]);
    }
;

struct-declaration:
    specifier-qualifier-list struct-declarator-list ";"
    {
        $$ = new CompilerExpr ( $[specifier-qualifier-list],
                                CompilerExpr::OPER_DECLARATION,
                                $[struct-declarator-list],
                                LOC(@$) );
    }

    | static_assert-declaration
    {
        $$ = $[static_assert-declaration];
    }
;

specifier-qualifier-list:
    specifier-qualifier
    {
        $$ = $[specifier-qualifier];
    }

    | specifier-qualifier-list specifier-qualifier
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_DATATYPE,
                                $[specifier-qualifier],
                                LOC(@$) );
    }
;

specifier-qualifier:
    type-specifier
    {
        $$ = $[type-specifier];
    }

    | type-qualifier
    {
        $$ = new CompilerExpr ( CompilerValue($[type-qualifier]),
                                CompilerExpr::OPER_DATATYPE,
                                LOC(@$) );
    }
;

struct-declarator-list:
    struct-declarator
    {
        $$ = $[struct-declarator];
    }

    | struct-declarator-list "," struct-declarator
    {
        $$ = $1->appendLink($[struct-declarator]);
    }
;

struct-declarator:
    declarator
    {
        $$ = $[declarator];
    }

    | declarator-opt ":" constant-expression
    {
        $$ = new CompilerExpr ( $[declarator-opt],
                                CompilerExpr::OPER_COLON,
                                $[constant-expression],
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 2.2.2: Enumeration specifiers
// -----------------------------------------------------------------------------

enum-specifier:
    "enum" identifier-opt "{" enumerator-list comma-opt "}"
    {
        $$ = new CompilerExpr ( CompilerCDeclaration::DT_ENUM,
                                CompilerExpr::OPER_DATATYPE,
                                new CompilerExpr ( $[identifier-opt],
                                                   CompilerExpr::OPER_PAIR,
                                                   $[enumerator-list] ),
                                LOC(@$) );
    }

    | "enum" identifier
    {
        $$ = new CompilerExpr ( CompilerCDeclaration::DT_ENUM,
                                CompilerExpr::OPER_DATATYPE,
                                $[identifier],
                                LOC(@$) );
    }
;

enumerator-list:
    enumerator
    {
        $$ = $[enumerator];
    }

    | enumerator-list "," enumerator
    {
        $$ = $1->appendLink($[enumerator]);
    }
;

enumerator:
    identifier
    {
        $$ = $[identifier];
        ALTER_SYMBOL_TYPE($[identifier], TYPE_ENUM_CONST);
    }

    | identifier "=" { ALTER_SYMBOL_TYPE($[identifier], TYPE_ENUM_CONST); } constant-expression
    {
        $$ = new CompilerExpr ( $[identifier],
                                CompilerExpr::OPER_ASSIGN,
                                $[constant-expression],
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 2.3: Atomic type specifier and type qualifier
// -----------------------------------------------------------------------------

atomic-type-specifier:
    "_Atomic" "(" type-name ")"
    {
        $$ = new CompilerExpr ( CompilerValue(CompilerCDeclaration::A_ATOMIC),
                                CompilerExpr::OPER_DATATYPE,
                                $[type-name],
                                LOC(@$) );
    }
;

type-qualifier:
    "const"
    {
        $$ = CompilerCDeclaration::A_CONST;
    }

    | "restrict"
    {
        $$ = CompilerCDeclaration::A_RESTRICT;
    }

    | "volatile"
    {
        $$ = CompilerCDeclaration::A_VOLATILE;
    }

    | "_Atomic"
    {
        $$ = CompilerCDeclaration::A_ATOMIC;
    }
;

// -----------------------------------------------------------------------------
// PART 2.4: Function specifier and alignment specifier
// -----------------------------------------------------------------------------

function-specifier:
    "inline"
    {
        $$ = CompilerCDeclaration::A_INLINE;
    }

    | "_Noreturn"
    {
        $$ = CompilerCDeclaration::A_NORETURN;
    }
;

alignment-specifier:
    "_Alignas" "(" type-name ")"
    {
        $$ = new CompilerExpr ( $[type-name],
                                CompilerExpr::OPER_ALIGNAS,
                                LOC(@$) );
    }

    | "_Alignas" "(" constant-expression ")"
    {
        $$ = new CompilerExpr ( $[constant-expression],
                                CompilerExpr::OPER_ALIGNAS,
                                LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 2.5:  Declarator
// -----------------------------------------------------------------------------

declarator:
    pointer-opt direct-declarator
    {
        $$ = new CompilerExpr ( $[pointer-opt],
                                CompilerExpr::OPER_PAIR,
                                $[direct-declarator],
                                LOC(@$) );
    }
;

declarator-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | declarator
    {
        $$ = $[declarator];
    }
;

direct-declarator:
    identifier
    {
        $$ = $[identifier];

        if ( true == TYPEDEF_FLAG )
        {
            ALTER_SYMBOL_TYPE($[identifier], TYPE_TYPEDEF);
        }
    }

    | "(" declarator ")"
    {
        $$ = $[declarator];
    }

    | direct-declarator "[" type-qualifier-list-opt assignment-expression-opt "]"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_ARRAY,
                                new CompilerExpr ( $[type-qualifier-list-opt],
                                                   CompilerExpr::OPER_DECLARATION,
                                                   $[assignment-expression-opt] ),
                                LOC(@$) );
    }

    | direct-declarator "[" "static" type-qualifier-list-opt assignment-expression "]"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_ARRAY,
                                new CompilerExpr ( new CompilerExpr ( $[type-qualifier-list-opt],
                                                                      CompilerExpr::OPER_DATATYPE,
                                                                      CompilerCDeclaration::A_STATIC ),
                                                   CompilerExpr::OPER_DECLARATION,
                                                   $[assignment-expression] ),
                                LOC(@$) );
    }

    | direct-declarator "[" type-qualifier-list "static" assignment-expression "]"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_ARRAY,
                                new CompilerExpr ( new CompilerExpr ( $[type-qualifier-list],
                                                                      CompilerExpr::OPER_DATATYPE,
                                                                      CompilerCDeclaration::A_STATIC ),
                                                   CompilerExpr::OPER_DECLARATION,
                                                   $[assignment-expression] ),
                                LOC(@$) );
    }

    | direct-declarator "[" type-qualifier-list-opt "*" "]"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_ARRAY,
                                new CompilerExpr ( $[type-qualifier-list-opt],
                                                   CompilerExpr::OPER_POINTER,
                                                   LOC(@3) ),
                                LOC(@$) );
    }

    | direct-declarator "(" parameter-type-list ")"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_FUNCTION,
                                $[parameter-type-list],
                                LOC(@$) );
    }

    | direct-declarator "(" identifier-list-opt ")"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_FUNCTION,
                                $[identifier-list-opt],
                                LOC(@$) );
    }
;

pointer:
    "*" type-qualifier-list-opt
    {
        $$ = new CompilerExpr ( $[type-qualifier-list-opt],
                                CompilerExpr::OPER_POINTER,
                                LOC(@$) );
    }

    | "*" type-qualifier-list-opt pointer
    {
        $$ = new CompilerExpr ( $[type-qualifier-list-opt],
                                CompilerExpr::OPER_POINTER,
                                $3,
                                LOC(@$) );
    }
;

pointer-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | pointer
    {
        $$ = $[pointer];
    }
;

type-qualifier-list:
    type-qualifier
    {
        $$ = new CompilerExpr ( CompilerValue($[type-qualifier]),
                                CompilerExpr::OPER_DATATYPE );
    }

    | type-qualifier-list type-qualifier
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_DATATYPE,
                                $[type-qualifier],
                                LOC(@$) );
    }
;

type-qualifier-list-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | type-qualifier-list
    {
        $$ = $[type-qualifier-list];
    }
;

parameter-type-list:
    parameter-list
    {
        $$ = $[parameter-list];
    }

    | parameter-list "," "..."
    {
        $$ = $[parameter-list]->appendLink(new CompilerExpr(LOC(@3)));
    }
;

parameter-list:
    parameter-declaration
    {
        $$ = $[parameter-declaration];
    }

    | parameter-list "," parameter-declaration
    {
        $$ = $1->appendLink($[parameter-declaration]);
    }
;

parameter-declaration:
    declaration-specifiers declarator
    {
        $$ = new CompilerExpr ( $[declaration-specifiers],
                                CompilerExpr::OPER_DECLARATION,
                                $[declarator],
                                LOC(@$) );

        TYPEDEF_FLAG = false;
    }

    | declaration-specifiers abstract-declarator-opt
    {
        $$ = new CompilerExpr ( $[declaration-specifiers],
                                CompilerExpr::OPER_DECLARATION,
                                $[abstract-declarator-opt],
                                LOC(@$) );

        TYPEDEF_FLAG = false;
    }
;

identifier-list:
    identifier
    {
        $$ = $[identifier];
    }

    | identifier-list "," identifier
    {
        $$ = $1->appendLink($[identifier]);
    }
;

identifier-list-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | identifier-list
    {
        $$ = $[identifier-list];
    }
;

// -----------------------------------------------------------------------------
// PART 2.6:  Type names
// -----------------------------------------------------------------------------

type-name:
    specifier-qualifier-list abstract-declarator-opt
    {
        $$ = new CompilerExpr ( $[specifier-qualifier-list],
                                CompilerExpr::OPER_DATATYPE,
                                $[abstract-declarator-opt],
                                LOC(@$) );
    }
;

abstract-declarator:
    pointer
    {
        $$ = $[pointer];
    }

    | pointer-opt direct-abstract-declarator
    {
        $$ = new CompilerExpr ( $[pointer-opt],
                                CompilerExpr::OPER_DATATYPE,
                                $[direct-abstract-declarator],
                                LOC(@$) );
    }
;

abstract-declarator-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | abstract-declarator
    {
        $$ = $[abstract-declarator];
    }
;

direct-abstract-declarator:
    "(" abstract-declarator ")"
    {
        $$ = $[abstract-declarator];
    }

    | direct-abstract-declarator-opt "[" assignment-expression-opt "]"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_ARRAY,
/*                                 new CompilerExpr ( $[type-qualifier-list-opt], */
/*                                                    CompilerExpr::OPER_DECLARATION, */
                                                   $[assignment-expression-opt] /*)*/,
                                LOC(@$) );
    }
// TODO:
/*    | direct-abstract-declarator-opt "[" "static" type-qualifier-list-opt assignment-expression "]"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_ARRAY,
                                new CompilerExpr ( new CompilerExpr ( $[type-qualifier-list-opt],
                                                                      CompilerExpr::OPER_DATATYPE,
                                                                      CompilerCDeclaration::A_STATIC ),
                                                   CompilerExpr::OPER_DECLARATION,
                                                   $[assignment-expression] ),
                                LOC(@$) );
    }

    | direct-abstract-declarator-opt "[" type-qualifier-list "static" assignment-expression "]"
    {
        $$ = new CompilerExpr ( $1,
                                CompilerExpr::OPER_ARRAY,
                                new CompilerExpr ( new CompilerExpr ( $[type-qualifier-list],
                                                                      CompilerExpr::OPER_DATATYPE,
                                                                      CompilerCDeclaration::A_STATIC ),
                                                   CompilerExpr::OPER_DECLARATION,
                                                   $[assignment-expression] ),
                                LOC(@$) );
    }*/

    | direct-abstract-declarator-opt "[" "*" "]"
    {
        $$ = new CompilerExpr ( $[direct-abstract-declarator-opt],
                                CompilerExpr::OPER_ARRAY,
                                new CompilerExpr(CompilerExpr::OPER_POINTER, LOC(@3)),
                                LOC(@$) );
    }

    | direct-abstract-declarator-opt "[" parameter-type-list "]"
    {
        $$ = new CompilerExpr ( $[direct-abstract-declarator-opt],
                                CompilerExpr::OPER_ARRAY,
                                $[parameter-type-list],
                                LOC(@$) );
    }
;

direct-abstract-declarator-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | direct-abstract-declarator
    {
        $$ = $[direct-abstract-declarator];
    }
;

// -----------------------------------------------------------------------------
// PART 2.7:  Type definitions
// -----------------------------------------------------------------------------

typedef-name:
    TYPEDEF_NAME
    {
        $$ = new CompilerExpr ( $[TYPEDEF_NAME], LOC(@$) );
    }
;

// -----------------------------------------------------------------------------
// PART 2.8:  Initialization
// -----------------------------------------------------------------------------

initializer:
    assignment-expression
    {
        $$ = $[assignment-expression];
    }

    | "{" initializer-list comma-opt "}"
    {
        $$ = $[initializer-list];
    }
;

initializer-list:
    designation-opt initializer
    {
        $$ = new CompilerExpr ( $[designation-opt],
                                CompilerExpr::OPER_INIT,
                                $[initializer],
                                LOC(@$) );
    }

    | initializer-list "," designation-opt initializer
    {
        $$ = $1->appendLink ( new CompilerExpr ( $[designation-opt],
                                                 CompilerExpr::OPER_INIT,
                                                 $[initializer] ) );
    }
;

designation:
    designator-list "="
    {
        $$ = $[designator-list];
    }
;

designation-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | designation
    {
        $$ = $[designation];
    }
;

designator-list:
    designator
    {
        $$ = $[designator];
    }

    | designator-list designator
    {
        $$ = $1->appendLink($[designator]);
    }
;

designator:
    "[" constant-expression "]"
    {
        $$ = new CompilerExpr ( $[constant-expression],
                                CompilerExpr::OPER_INDEX,
                                LOC(@$) );
    }

    | "." identifier
    {
        $$ = new CompilerExpr ( $[identifier],
                                CompilerExpr::OPER_DOT,
                                LOC(@$) );

    }
;

static_assert-declaration:
    "_Static_assert" "(" constant-expression "," string-literal ")"
    {
        $$ = new CompilerExpr ( $[constant-expression],
                                CompilerExpr::OPER_ASSERT,
                                $[string-literal],
                                LOC(@$) );
    }
;

// =============================================================================
// PART 3: Statements and blocks
// =============================================================================

statement:
    labeled-statement
    {
        $$ = $[labeled-statement];
    }

    | compound-statement
    {
        $$ = $[compound-statement];
    }

    | expression-statement
    {
        $$ = $[expression-statement];
    }

    | selection-statement
    {
        $$ = $[selection-statement];
    }

    | iteration-statement
    {
        $$ = $[iteration-statement];
    }

    | jump-statement
    {
        $$ = $[jump-statement];
    }

    | inline-assembly
    {
        $$ = $[inline-assembly];
    }
;

// -----------------------------------------------------------------------------
// PART 3.1:  Labeled statements
// -----------------------------------------------------------------------------

labeled-statement:
    identifier ":" statement
    {
        $$ = new CompilerStatement ( LOC(@$), C_LABEL, $[identifier] );
        $$->appendLink($[statement]);
    }

    | "case" constant-expression ":" statement
    {
        $$ = new CompilerStatement ( LOC(@$), C_CASE, $[constant-expression] );
        $$->createBranch($[statement]);
    }

    | "default" ":" statement
    {
        $$ = new CompilerStatement ( LOC(@$), C_DEFAULT );
        $$->createBranch($[statement]);
    }
;

// -----------------------------------------------------------------------------
// PART 3.2:  Compound statement
// -----------------------------------------------------------------------------

compound-statement:
    "{" { ENTER_SCOPE(); } block-item-list-opt { LEAVE_SCOPE(); } "}"
    {
        $$ = new CompilerStatement ( LOC(@$), C_COMPOUND );
        $$->createBranch($[block-item-list-opt]);
    }

    | critical-specifier "{" { ENTER_SCOPE(); } block-item-list-opt { LEAVE_SCOPE(); } "}"
    {
        $$ = new CompilerStatement ( LOC(@$), C_CRITICAL_COMPOUND );
        $$->createBranch($[block-item-list-opt]);
    }
;

block-item-list:
    block-item
    {
        $$ = $[block-item];
    }

    | block-item-list block-item
    {
        $$ = $1->appendLink($[block-item]);
    }
;

block-item-list-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | block-item-list
    {
        $$ = $[block-item-list];
    }
;

block-item:
    declaration
    {
        $$ = new CompilerStatement ( LOC(@$), C_DECLARATION, $[declaration] );
    }

    | statement
    {
        $$ = $[statement];
    }
;

// -----------------------------------------------------------------------------
// PART 3.3: Expression and null statements
// -----------------------------------------------------------------------------

expression-statement:
    ";"
    {
        $$ = new CompilerStatement ( LOC(@$), C_NULL );
    }

    | expression ";"
    {
        $$ = new CompilerStatement ( LOC(@$), C_EXPRESSION, $[expression] );
    }
;

// -----------------------------------------------------------------------------
// PART 3.4: Selection statements
// -----------------------------------------------------------------------------

selection-statement:
    "if" "(" expression ")" statement %prec IFX
    {
        $$ = new CompilerStatement ( LOC(@$), C_IF_ELSE, $[expression] );
        CompilerStatement * ifStatement = new CompilerStatement ( LOC(@1), C_IF );
        ifStatement->createBranch($[statement]);
        $$->createBranch(ifStatement);
    }

    | "if" "(" expression ")" statement "else" statement
    {
        $$ = new CompilerStatement ( LOC(@$), C_IF_ELSE, $[expression] );
        CompilerStatement * ifStatement = new CompilerStatement ( LOC(@1), C_IF );
        CompilerStatement * elseStatement = new CompilerStatement ( LOC(@6), C_ELSE );
        ifStatement->createBranch($5);
        elseStatement->createBranch($7);
        $$->createBranch(ifStatement->appendLink(elseStatement));
    }

    | "switch" "(" expression ")" statement
    {
        $$ = new CompilerStatement ( LOC(@$), C_SWITCH, $[expression] );
        $$->createBranch($[statement]);
    }
;

// -----------------------------------------------------------------------------
// PART 3.5: Iteration statements
// -----------------------------------------------------------------------------

iteration-statement:
    "while" "(" expression ")" statement
    {
        $$ = new CompilerStatement ( LOC(@$), C_WHILE, $[expression] );
        $$->createBranch($[statement]);
    }

    | "do" statement "while" "(" expression ")" ";"
    {
        $$ = new CompilerStatement ( LOC(@$), C_DO_WHILE, $[expression] );
        $$->createBranch($[statement]);
    }

    | "for" "(" expression-opt ";" expression-opt ";" expression-opt ")" statement
    {
        $3 = ( ( nullptr == $3 ) ? ( new CompilerExpr() ) : $3 );
        $5 = ( ( nullptr == $5 ) ? ( new CompilerExpr() ) : $5 );
        $7 = ( ( nullptr == $7 ) ? ( new CompilerExpr() ) : $7 );

        $$ = new CompilerStatement ( LOC(@$), C_FOR, $3->appendLink($5)->appendLink($7) );
        $$->createBranch($[statement]);
    }

    | "for" "(" declaration expression-opt ";" expression-opt ")" statement
    {
        $3 = ( ( nullptr == $3 ) ? ( new CompilerExpr() ) : $3 );
        $4 = ( ( nullptr == $4 ) ? ( new CompilerExpr() ) : $4 );
        $6 = ( ( nullptr == $6 ) ? ( new CompilerExpr() ) : $6 );

        $$ = new CompilerStatement ( LOC(@$), C_FOR_DECL, $3->appendLink($4)->appendLink($6) );
        $$->createBranch($[statement]);
    }
;

// -----------------------------------------------------------------------------
// PART 3.6: Jump statements
// -----------------------------------------------------------------------------

jump-statement:
    "goto" identifier ";"
    {
        $$ = new CompilerStatement ( LOC(@$), C_GOTO, $[identifier] );
    }

    | "continue" ";"
    {
        $$ = new CompilerStatement ( LOC(@$), C_CONTINUE );
    }

    | "break" ";"
    {
        $$ = new CompilerStatement ( LOC(@$), C_BREAK );
    }

    | "return" expression-opt ";"
    {
        $$ = new CompilerStatement ( LOC(@$), C_RETURN, $[expression-opt] );
    }
;

// =============================================================================
// PART 4: External definitions
// =============================================================================

start-symbol:
    /* empty */
    {
        compiler->processCodeTree(nullptr);
        YYACCEPT;
    }

    | translation-unit
    {
        compiler->processCodeTree($[translation-unit]);
        YYACCEPT;
    }
;

translation-unit:
    external-declaration
    {
        $$ = $[external-declaration];
    }

    | translation-unit external-declaration
    {
        $$ = $1->appendLink($[external-declaration]);
    }
;

external-declaration:
    function-definition
    {
        $$ = $[function-definition];
    }

    | declaration
    {
        $$ = new CompilerStatement ( LOC(@$), C_DECLARATION, $[declaration] );
    }

    | ";"
    {
        $$ = nullptr;
        compiler->parserMessage ( LOC(@$),
                                  CompilerBase::MT_WARNING,
                                  QObject::tr("null statement is not allowed here (ISO/IEC 9899:2011)").toStdString(),
                                  true );
    }
;

// -----------------------------------------------------------------------------
// PART 4.1: Function definitions
// -----------------------------------------------------------------------------

function-definition:
    declaration-specifiers declarator declaration-list-opt compound-statement
    {
        CompilerExpr * decl = new CompilerExpr ( $[declaration-specifiers],
                                                 CompilerExpr::OPER_DECLARATION,
                                                 $[declarator] );

        $$ = new CompilerStatement ( LOC(@$), C_FUNCTION_DEF, decl->appendLink($[declaration-list-opt]) );
        $$->createBranch($[compound-statement]);

        TYPEDEF_FLAG = false;
    }
;

declaration-list:
    declaration
    {
        $$ = $[declaration];
    }

    | declaration-list declaration
    {
        $$ = $1->appendLink($[declaration]);
    }
;

declaration-list-opt:
    /* empty */
    {
        $$ = nullptr;
    }

    | declaration-list
    {
        $$ = $[declaration-list];
    }
;

// =============================================================================
// PART 5: Implementation specific extensions
// =============================================================================

allocation-specifier:
    "__at__" "(" string-literal "," integer-constant ")"
    {
        $$ = new CompilerExpr ( $[string-literal],
                                CompilerExpr::OPER_AT,
                                $[integer-constant],
                                LOC(@$) );
    }
;

critical-specifier:
    "__critical__"
;

inline-assembly:
    "__asm__" "(" string-literal ")" ";"
    {
        $$ = new CompilerStatement ( LOC(@$), C_INLINE_ASSEMBLY, $[string-literal] );
    }
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
