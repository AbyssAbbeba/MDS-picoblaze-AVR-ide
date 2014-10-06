// =============================================================================
/**
 * @brief
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
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
%name-prefix "AsmAdaptableParser_"
// Generate a parser header file containing macro definitions, etc.
%defines "AsmAdaptableParser.h"
// Generate a pure, reentrant parser.
%define api.pure
// Generate the code processing the locations.
%locations
// Specify the programming language for the generated parser.
%language "C"
// Write an extra output file containing verbose descriptions of the parser states.
%verbose
// Expect exactly <n> shift/reduce conflicts in this grammar
%expect 47
// Expect exactly <n> reduce/reduce conflicts in this grammar
%expect-rr 0
/* Type of parser tables within the LR family, in this case we use LALR (Look-Ahead LR parser) */
%define lr.type lalr
/* Bison declaration to request verbose, specific error message strings when yyerror is called */
%error-verbose
/* Enable LAC (Lookahead Correction) to improve syntax error handling. */
%define parse.lac full

// Symbol semantic value
%union
{
    int number;                 //
    char * string;              //
    CompilerExpr * expr;        //
    CompilerStatement * stmt;   //
    struct
    {
        unsigned char * data;   //
        int size;               //
    } array;                    //
};

/*
 * Compiler interface for the syntax analyzer; we need to have some things declared
 * before we can declare other things, and this interface is quite essential here.
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
    #include <QObject>              // For i18n, nothing else.
    #include <cstdlib>              // We need free() here.
    #include <cstring>              // We need strcmp() here.
    #include <iostream>             // This is required by Bison.
    using namespace std;            // This is required by Bison as well.
    using namespace CompilerStatementTypes; // This NS is heavily used here.

    // Declaration of the lexer prototypes and other things required by Bison
    #include "AsmAdaptableLexer.h"

    // Name of the lexer function required by Bison
    #define AsmAdaptableParser_lex AsmAdaptableLexer_lex

    // Make a few things a little easier...
    #define LOC(location) \
        ( compiler->toSourceLocation(location) )

    // Error messages
    #define NO_LABEL_EXPECTED(location, directive, statement) \
        CompilerStatement::completeDelete(statement); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("cannot declare a label before %1 directive").arg(directive).toStdString() );
    #define ARG_REQUIRED_D(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 requires a single argument").arg(directive).toStdString() );
    #define NO_ARG_EXPECTED_D(directive, arguments, location) \
        arguments->completeDelete(); \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 takes no arguments").arg(directive).toStdString() );
    #define DECL_ID_EXPECTED(location, directive) \
        compiler->parserMessage ( location, \
                                  CompilerBase::MT_ERROR, \
                                  QObject::tr ( "directive %1 requires an identifier for the symbol (or macro) which " \
                                                "it defines" ) \
                                              .arg(directive).toStdString() );
    #define MISSING_AT_OPERATOR(location, directive) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("directive %1 requires `AT' operator before the start address").arg(directive).toStdString() );

    // Declaration of the error reporting function used by Bison
    inline int AsmAdaptableParser_error ( YYLTYPE * yylloc,
                                       yyscan_t yyscanner,
                                       CompilerParserInterface * compiler,
                                       const char * errorInfo );
%}

// Declare an additional yyparse parameters
%parse-param { yyscan_t yyscanner }
%parse-param { CompilerParserInterface * compiler }

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

/* Directives */
%token D_NAMEREG        D_ORG           D_CONSTANT      D_WHILE         D_DB
%token D_LIST           D_MESSG         D_NOLIST        D_SKIP          D_ERROR
%token D_TITLE          D_EXPAND        D_NOEXPAND      D_IF            D_PORT
%token D_IFN            D_IFDEF         D_IFNDEF        D_ELSEIFB       D_ELSEIFNB
%token D_ELSE           D_ELSEIF        D_ELSEIFN       D_ELSEIFDEF     D_ELSEIFNDEF
%token D_ENDIF          D_LOCAL         D_IFNB          D_IFB           D_LIMIT
%token D_ENDM           D_EXITM         D_REPT          D_MACRO         D_EQU
%token D_END            D_REG           D_CODE          D_ENDW          D_WARNING
%token D_VARIABLE       D_SET           D_DEFINE        D_UNDEFINE      D_ENDR
%token D_AUTOREG        D_AUTOSPR       D_DATA          D_DEVICE        D_ADDRESS
%token D_FAILJMP        D_ORGSPR        D_INITSPR       D_MERGESPR

/* Other terminal symbols */
%token COMMENT
%token AT
%token AT_MARK          "@"
%token EOL              "end of line"
%token END      0       "end of file"
// Named tokens (to make the grammar rules more readable)
%token LP               "("
%token RP               ")"
%token IMMEDIATE        "#"
%token COMMA            ","
%token COLON            ":"
%token INTERVAL         ".."
// Expression operators
%token PLUS             "+"
%token MINUS            "-"
%token ASTERISK         "*"
%token SLASH            "/"
%token NOT              "!"
%token MOD              "%"
%token SHL              "<<"
%token SHR              ">>"
%token AND              "&&"
%token OR               "||"
%token BITAND           "&"
%token BITNAND          "!&"
%token BITOR            "|"
%token BITXOR           "^"
%token EQ               "=="
%token NE               "!="
%token LT               "<"
%token LE               "<="
%token GT               ">"
%token GE               ">="
%token COMPLEMENT       "~"

// Expression functions
%token F_HIGH           F_LOW

/* Operator precedence (the one declared later has the higher precedence) */
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

/* Terminal symbols with semantic value */
  // semantic value is a string
%token<string>  IDENTIFIER      SUBST_MARK      INCLUDE
%token<array>   STRING
  // semantic value is a number
%token<number>  NUMBER

/*
 * DECLARATION OF NON-TERMINAL SYMBOLS
 */
// Expressions
%type<expr>     expr            number          params          args            args_str
%type<expr>     id              string          mark            mac_args        mac_arg
// Statements - general
%type<stmt>     statements      stmt            label           dir_stmt        macro_stmt
%type<stmt>     directive       macro
// Statements - directives
%type<stmt>     dir_cond_asm    dir_skip        dir_local       dir_exitm       dir_list
%type<stmt>     dir_include     dir_repeat      dir_org         dir_end         dir_reg
%type<stmt>     dir_set         dir_equ         dir_macro       dir_if          dir_ifn
%type<stmt>     dir_ifdef       dir_ifndef      dir_ifb         dir_ifnb        dir_elseifnb
%type<stmt>     dir_elseif      dir_elseifn     dir_elseifdef   dir_elseifndf   dir_elseifb
%type<stmt>     dir_constant    dir_variable    dir_while       dir_namereg     dir_port
%type<stmt>     dir_while_a     dir_endw        dir_error       dir_undefine    dir_define
%type<stmt>     dir_title       dir_nolist      dir_autoreg     dir_autoreg_a   dir_autospr
%type<stmt>     dir_expand      dir_noexpand    dir_code        dir_db          dir_messg
%type<stmt>     dir_code_a      if_block        ifelse_block    dir_endif       dir_warning
%type<stmt>     else_block      dir_else        dir_if_a        dir_ifn_a       dir_ifdef_a
%type<stmt>     dir_ifndef_a    dir_ifb_a       dir_ifnb_a      dir_elseif_a    dir_elseifn_a
%type<stmt>     dir_elseifdef_a dir_elseifndf_a dir_elseifb_a   dir_elseifnb_a  dir_rept
%type<stmt>     dir_endm        dir_macro_d     dir_macro_a     dir_endr        dir_endr_a
%type<stmt>     dir_db_a        dir_endm_a      dir_expand_a    dir_noexpand_a  dir_autospr_a
%type<stmt>     dir_data        dir_limit       dir_device      dir_failjmp     dir_orgspr
%type<stmt>     dir_initspr     dir_mergespr

/*
 * Symbol destructors:
 * each time the parser discards symbol with certain semantic types, their memory have to bee freed.
 */
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

// The start symbol
%start input

// -----------------------------------------------------------------------------
// GRAMMAR RULES
// -----------------------------------------------------------------------------

%%

/*
 * Basic code structure
 */
input:
      statements                    { compiler->processCodeTree($statements); YYACCEPT; }
;
statements:
      stmt                          { $$ = $stmt;                 }
    | statements EOL stmt           { $$ = $1->appendLink($stmt); }
;
stmt:
      /* empty */                   { $$ = nullptr; }
    | error                         { $$ = nullptr; }
    | COMMENT                       { $$ = nullptr; }
    | label                         { $$ = $1;   }
    | label COMMENT                 { $$ = $1;   }
    | dir_stmt                      { $$ = $1;   }
    | macro_stmt                    { $$ = $1;   }
;
dir_stmt:
      directive                     { $$ = $directive; }
    | directive COMMENT             { $$ = $directive; }
;
macro_stmt:
      macro                         { $$ = $macro;                     }
    | macro COMMENT                 { $$ = $macro;                     }
    | label macro                   { $$ = $label->appendLink($macro); }
    | label macro COMMENT           { $$ = $label->appendLink($macro); }
;
macro:
      id                            { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_MACRO, $id);                    }
    | id "(" ")"                    { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_MACRO, $id);                    }
    | id args                       { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_MACRO, $id->appendLink($args)); }
    | id "(" args ")"               { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_MACRO, $id->appendLink($args)); }
;

/*
 * Generally used non-terminal symbols
 */
number:
      NUMBER                        { $$ = new CompilerExpr($NUMBER, LOC(@$)); }
;
id:
      IDENTIFIER                    { $$ = new CompilerExpr($IDENTIFIER, LOC(@$)); }
;
mark:
      SUBST_MARK                    { $$ = new CompilerExpr($SUBST_MARK, LOC(@$)); }
;
string:
      STRING                        { $$ = new CompilerExpr(CompilerValue($STRING.data, $STRING.size), LOC(@$)); }
;
label:
      id ":"                        { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_LABEL, $id ); }
;
expr:
      id                            { $$ = $id;                                                            }
    | mark                          { $$ = $mark;                                                          }
    | number                        { $$ = $number;                                                        }
    | "(" expr ")"                  { $$ = $2;                                                             }
    | F_LOW  "(" expr ")"           { $$ = new CompilerExpr(CompilerExpr::OPER_LOW, $3, LOC(@$));          }
    | F_HIGH "(" expr ")"           { $$ = new CompilerExpr(CompilerExpr::OPER_HIGH, $3, LOC(@$));         }
    | "-" expr %prec UMINUS         { $$ = new CompilerExpr($2, CompilerExpr::OPER_ADD_INV, LOC(@$));      }
    | "+" expr %prec UPLUS          { $$ = new CompilerExpr($2, CompilerExpr::OPER_INT_PROM, LOC(@$));     }
    | "~" expr                      { $$ = new CompilerExpr($2, '~', LOC(@$));                             }
    | "!" expr                      { $$ = new CompilerExpr($2, '!', LOC(@$));                             }
    | expr "+"  expr                { $$ = new CompilerExpr($1, '+', $3, LOC(@$));                         }
    | expr "-"  expr                { $$ = new CompilerExpr($1, '-', $3, LOC(@$));                         }
    | expr "*"  expr                { $$ = new CompilerExpr($1, '*', $3, LOC(@$));                         }
    | expr "/"  expr                { $$ = new CompilerExpr($1, '/', $3, LOC(@$));                         }
    | expr "%"  expr                { $$ = new CompilerExpr($1, '%', $3, LOC(@$));                         }
    | expr "|"  expr                { $$ = new CompilerExpr($1, '|', $3, LOC(@$));                         }
    | expr "^"  expr                { $$ = new CompilerExpr($1, '^', $3, LOC(@$));                         }
    | expr "&"  expr                { $$ = new CompilerExpr($1, '&', $3, LOC(@$));                         }
    | expr "<"  expr                { $$ = new CompilerExpr($1, '<', $3, LOC(@$));                         }
    | expr ">"  expr                { $$ = new CompilerExpr($1, '>', $3, LOC(@$));                         }
    | expr "||" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LOR,  $3,    LOC(@$));  }
    | expr "&&" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LAND, $3,    LOC(@$));  }
    | expr "==" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_EQ,   $3,    LOC(@$));  }
    | expr "!=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_NE,   $3,    LOC(@$));  }
    | expr "<=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_LE,   $3,    LOC(@$));  }
    | expr ">=" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_GE,   $3,    LOC(@$));  }
    | expr ">>" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_SHR,  $3,    LOC(@$));  }
    | expr "<<" expr                { $$ = new CompilerExpr($1, CompilerExpr::OPER_SHL,  $3,    LOC(@$));  }
    | id "(" mac_args ")"           { $$ = new CompilerExpr($id,CompilerExpr::OPER_CALL, $mac_args, LOC(@$));  }
    | id "(" ")"                    {
                                        /* Syntax Error */
                                        $$ = $id;
                                        compiler->parserMessage ( @$,
                                                                  CompilerBase::MT_ERROR,
                                                                  QObject::tr("missing argument(s)").toStdString() );
                                    }
;
mac_arg:       // One macro/instruction agrument
      "#" expr                      { $$ = new CompilerExpr($expr, CompilerExpr::OPER_HASH, LOC(@$)); }
    | "@" expr                      { $$ = new CompilerExpr($expr, CompilerExpr::OPER_AT,   LOC(@$)); }
;
mac_args:       // List of macro/instruction agruments
      mac_args "," mac_arg          { $$ = $1->appendLink($mac_arg); }
    | mac_arg                       { $$ = $mac_arg;                }
;
args:           // List of arguments without strings, e.g. `(1+3), XYZ, 0x4b'
      args "," expr                 { $$ = $1->appendLink($expr); }
    | expr                          { $$ = $expr;                 }
;
args_str:       // List of arguments possibly with strings, e.g. `ABC, "XYZ - QWE", 2 * 0x4b'
      args_str "," string           { $$ = $1->appendLink($string); }
    | args_str "," expr             { $$ = $1->appendLink($expr);   }
    | string                        { $$ = $string;                 }
    | expr                          { $$ = $expr;                   }
;
params:         // List of identifiers, e.g. `target, source, offset, size, mode'
      params "," id                 { $$ = $1->appendLink($id); }
    | id                            { $$ = $id;                 }
;

/*
 * Directives
 */
directive:
      dir_cond_asm  { $$ = $1; }    | dir_skip      { $$ = $1; }
    | dir_local     { $$ = $1; }    | dir_exitm     { $$ = $1; }
    | dir_list      { $$ = $1; }    | dir_include   { $$ = $1; }
    | dir_nolist    { $$ = $1; }    | dir_repeat    { $$ = $1; }
    | dir_org       { $$ = $1; }    | dir_end       { $$ = $1; }
    | dir_set       { $$ = $1; }    | dir_equ       { $$ = $1; }
    | dir_macro     { $$ = $1; }    | dir_constant  { $$ = $1; }
    | dir_variable  { $$ = $1; }    | dir_namereg   { $$ = $1; }
    | dir_while     { $$ = $1; }    | dir_reg       { $$ = $1; }
    | dir_db        { $$ = $1; }    | dir_define    { $$ = $1; }
    | dir_port      { $$ = $1; }    | dir_undefine  { $$ = $1; }
    | dir_messg     { $$ = $1; }    | dir_title     { $$ = $1; }
    | dir_expand    { $$ = $1; }    | dir_noexpand  { $$ = $1; }
    | dir_autospr   { $$ = $1; }    | dir_autoreg   { $$ = $1; }
    | dir_code      { $$ = $1; }    | dir_error     { $$ = $1; }
    | dir_warning   { $$ = $1; }    | dir_data      { $$ = $1; }
    | dir_limit     { $$ = $1; }    | dir_device    { $$ = $1; }
    | dir_failjmp   { $$ = $1; }    | dir_mergespr  { $$ = $1; }
    | dir_orgspr    { $$ = $1; }    | dir_initspr   { $$ = $1; }
;
dir_cond_asm:
      if_block ifelse_block
      else_block dir_endif          {
                                        $$ = new CompilerStatement ( CompilerSourceLocation(),
                                                                     ASMPICOBLAZE_COND_ASM );
                                        $$->createBranch ( $if_block -> appendLink($ifelse_block)
                                                                     -> appendLink($else_block)
                                                                     -> appendLink($dir_endif) );
                                    }
    | label if_block ifelse_block
      else_block dir_endif          {
                                        $$ = $label->appendLink(new CompilerStatement(CompilerSourceLocation(),
                                                                                      ASMPICOBLAZE_COND_ASM));
                                        $$->createBranch ( $if_block -> appendLink($ifelse_block)
                                                                     -> appendLink($else_block)
                                                                     -> appendLink($dir_endif) );
                                    }
;
if_block:
      dir_if        statements EOL  { $$ = $1->createBranch($2); }
    | dir_ifn       statements EOL  { $$ = $1->createBranch($2); }
    | dir_ifdef     statements EOL  { $$ = $1->createBranch($2); }
    | dir_ifndef    statements EOL  { $$ = $1->createBranch($2); }
    | dir_ifb       statements EOL  { $$ = $1->createBranch($2); }
    | dir_ifnb      statements EOL  { $$ = $1->createBranch($2); }
    | dir_if                        { $$ = $1;                   }
    | dir_ifn                       { $$ = $1;                   }
    | dir_ifdef                     { $$ = $1;                   }
    | dir_ifndef                    { $$ = $1;                   }
    | dir_ifb                       { $$ = $1;                   }
    | dir_ifnb                      { $$ = $1;                   }
;
ifelse_block:
      /* empty */                   { $$ = nullptr;                 }
    | dir_elseif    statements EOL  { $$ = $1->createBranch($2); }
    | dir_elseifn   statements EOL  { $$ = $1->createBranch($2); }
    | dir_elseifdef statements EOL  { $$ = $1->createBranch($2); }
    | dir_elseifndf statements EOL  { $$ = $1->createBranch($2); }
    | dir_elseifb   statements EOL  { $$ = $1->createBranch($2); }
    | dir_elseifnb  statements EOL  { $$ = $1->createBranch($2); }
    | dir_elseif                    { $$ = $1;                   }
    | dir_elseifn                   { $$ = $1;                   }
    | dir_elseifdef                 { $$ = $1;                   }
    | dir_elseifndf                 { $$ = $1;                   }
    | dir_elseifb                   { $$ = $1;                   }
    | dir_elseifnb                  { $$ = $1;                   }
;
else_block:
      /* empty */                   { $$ = nullptr;                                 }
    | dir_else statements EOL       { $$ = $dir_else->createBranch($statements); }
    | dir_else                      { $$ = $dir_else;                            }
;
dir_endif:
      D_ENDIF                       { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ENDIF); }
;
dir_else:
      D_ELSE                        { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ELSE); }
    | label D_ELSE                  {
                                        /* Syntax error */
                                        NO_LABEL_EXPECTED(@label, "ELSE", $label);
                                        $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ELSE);
                                    }
    | label D_ELSE args             {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("ELSE", $args, @args);
                                        NO_LABEL_EXPECTED(@label, "ELSE", $label);
                                        $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ELSE);
                                    }
    | D_ELSE args                   {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("ELSE", $args, @args);
                                        $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ELSE);
                                    }
;
dir_if:
      dir_if_a EOL                  { $$ = $dir_if_a; }
    | dir_if_a COMMENT EOL          { $$ = $dir_if_a; }
;
dir_if_a:
      D_IF expr                     { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_IF, $expr); }
    | D_IF                          { /* Syntax error */ $$ = nullptr; ARG_REQUIRED_D(@D_IF, "IF"); }
;
dir_ifn:
      dir_ifn_a EOL                 { $$ = $dir_ifn_a; }
    | dir_ifn_a COMMENT EOL         { $$ = $dir_ifn_a; }
;
dir_ifn_a:
      D_IFN expr                    { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_IFN, $expr); }
    | D_IFN                         { /* Syntax error */ $$ = nullptr; ARG_REQUIRED_D(@D_IFN, "IFN"); }
;
dir_ifdef:
      dir_ifdef_a EOL               { $$ = $dir_ifdef_a; }
    | dir_ifdef_a COMMENT EOL       { $$ = $dir_ifdef_a; }
;
dir_ifdef_a:
      D_IFDEF id                    { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_IFDEF, $id); }
    | D_IFDEF                       { /* Syntax error */ $$ = nullptr; ARG_REQUIRED_D(@D_IFDEF, "IFDEF"); }
;
dir_ifndef:
      dir_ifndef_a EOL              { $$ = $dir_ifndef_a; }
    | dir_ifndef_a COMMENT EOL      { $$ = $dir_ifndef_a; }
;
dir_ifndef_a:
      D_IFNDEF id                   { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_IFNDEF, $id); }
    | D_IFNDEF                      { /* Syntax error */ $$ = nullptr; ARG_REQUIRED_D(@D_IFNDEF, "IFNDEF"); }
;
dir_ifb:
      dir_ifb_a EOL                 { $$ = $dir_ifb_a; }
    | dir_ifb_a COMMENT EOL         { $$ = $dir_ifb_a; }
;
dir_ifb_a:
      D_IFB id                      { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_IFB, $id); }
    | D_IFB                         { /* Syntax error */ $$ = nullptr; ARG_REQUIRED_D(@D_IFB, "IFB"); }
;
dir_ifnb:
      dir_ifnb_a EOL                { $$ = $dir_ifnb_a; }
    | dir_ifnb_a COMMENT EOL        { $$ = $dir_ifnb_a; }
;
dir_ifnb_a:
      D_IFNB id                     { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_IFNB, $id); }
    | D_IFNB                        { /* Syntax error */ $$ = nullptr; ARG_REQUIRED_D(@D_IFNB, "IFNB"); }
;
dir_elseif:
      dir_elseif_a EOL              { $$ = $dir_elseif_a; }
    | dir_elseif_a COMMENT EOL      { $$ = $dir_elseif_a; }
;
dir_elseif_a:
      D_ELSEIF expr                 { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ELSEIF, $expr); }
    | D_ELSEIF                      { /* Syntax Error */ $$ = nullptr; ARG_REQUIRED_D(@D_ELSEIF, "ELSEIF"); }
    | label D_ELSEIF expr           {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label, "ELSEIF", $label);
                                        $expr->completeDelete();
                                    }
    | label D_ELSEIF                {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ELSEIF, "ELSEIF");
                                        NO_LABEL_EXPECTED(@label, "ELSEIF", $label);
                                    }
;
dir_elseifn:
      dir_elseifn_a EOL             { $$ = $dir_elseifn_a; }
    | dir_elseifn_a COMMENT EOL     { $$ = $dir_elseifn_a; }
;
dir_elseifn_a:
      D_ELSEIFN expr                { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ELSEIFN, $expr); }
    | D_ELSEIFN                     {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ELSEIFN, "ELSEIFN");
                                    }
    | label D_ELSEIFN expr          {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label, "ELSEIFN", $label);
                                        $expr->completeDelete();
                                    }
    | label D_ELSEIFN               {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ELSEIFN, "ELSEIFN");
                                        NO_LABEL_EXPECTED(@label, "ELSEIFN", $label);
                                    }
;
dir_elseifdef:
      dir_elseifdef_a EOL           { $$ = $dir_elseifdef_a; }
    | dir_elseifdef_a COMMENT EOL   { $$ = $dir_elseifdef_a; }
;
dir_elseifdef_a:
      D_ELSEIFDEF expr              { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ELSEIFDEF, $expr); }
    | D_ELSEIFDEF                   {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ELSEIFDEF, "ELSEIFDEF");
                                    }
    | label D_ELSEIFDEF expr        {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label, "ELSEIFDEF", $label);
                                        $expr->completeDelete();
                                    }
    | label D_ELSEIFDEF             {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ELSEIFDEF, "ELSEIFDEF");
                                        NO_LABEL_EXPECTED(@label, "ELSEIFDEF", $label);
                                    }
;
dir_elseifndf:
      dir_elseifndf_a EOL           { $$ = $dir_elseifndf_a; }
    | dir_elseifndf_a COMMENT EOL   { $$ = $dir_elseifndf_a; }
;
dir_elseifndf_a:
      D_ELSEIFNDEF expr             { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ELSEIFNDEF, $expr); }
    | D_ELSEIFNDEF                  {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ELSEIFNDEF, "ELSEIFNDEF");
                                    }
    | label D_ELSEIFNDEF expr       {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label, "ELSEIFNDEF", $label);
                                        $expr->completeDelete();
                                    }
    | label D_ELSEIFNDEF            {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ELSEIFNDEF, "ELSEIFNDEF");
                                        NO_LABEL_EXPECTED(@label, "ELSEIFNDEF", $label);
                                    }
;
dir_elseifb:
      dir_elseifb_a EOL             { $$ = $dir_elseifb_a; }
    | dir_elseifb_a COMMENT EOL     { $$ = $dir_elseifb_a; }
;
dir_elseifb_a:
      D_ELSEIFB expr                { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ELSEIFB, $expr); }
    | D_ELSEIFB                     {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ELSEIFB, "ELSEIFB");
                                    }
    | label D_ELSEIFB expr          {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label, "ELSEIFB", $label);
                                        $expr->completeDelete();
                                    }
    | label D_ELSEIFB               {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ELSEIFB, "ELSEIFB");
                                        NO_LABEL_EXPECTED(@label, "ELSEIFB", $label);
                                    }
;
dir_elseifnb:
      dir_elseifnb_a EOL            { $$ = $dir_elseifnb_a; }
    | dir_elseifnb_a COMMENT EOL    { $$ = $dir_elseifnb_a; }
;
dir_elseifnb_a:
      D_ELSEIFNB expr               { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ELSEIFNB, $expr); }
    | D_ELSEIFNB                    {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ELSEIFNB, "ELSEIFNB");
                                    }
    | label D_ELSEIFNB expr         {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label, "ELSEIFNB", $label);
                                        $expr->completeDelete();
                                    }
    | label D_ELSEIFNB              {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ELSEIFNB, "ELSEIFNB");
                                        NO_LABEL_EXPECTED(@label, "ELSEIFNB", $label);
                                    }
;
dir_org:
      D_ORG expr                    { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ORG, $expr); }
    | label D_ORG expr              {
                                        $$ = $label->appendLink ( new CompilerStatement ( LOC(@$),
                                                                                          ASMPICOBLAZE_DIR_ORG,
                                                                                          $expr ) );
                                    }
    | D_ORG                         {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ORG, "ORG");
                                    }
    | label D_ORG                   {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ORG, "ORG");
                                        $label->completeDelete();
                                    }
    | D_ADDRESS expr                { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ORG, $expr); }
    | label D_ADDRESS expr          {
                                        $$ = $label->appendLink ( new CompilerStatement ( LOC(@$),
                                                                                          ASMPICOBLAZE_DIR_ORG,
                                                                                          $expr ) );
                                    }
    | D_ADDRESS                     {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ADDRESS, "ADDRESS");
                                    }
    | label D_ADDRESS               {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ADDRESS, "ADDRESS");
                                        $label->completeDelete();
                                    }
;
dir_skip:
      D_SKIP expr                   { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_SKIP, $expr); }
    | label D_SKIP expr             {
                                        $$ = $label->appendLink ( new CompilerStatement ( LOC(@$),
                                                                                          ASMPICOBLAZE_DIR_SKIP,
                                                                                          $expr ) );
                                    }
    | D_SKIP                        {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_SKIP, "SKIP");
                                    }
    | label D_SKIP                  {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_SKIP, "SKIP");
                                        $label->completeDelete();
                                    }
;
dir_local:
      D_LOCAL id                    { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_LOCAL, $id); }
    | label D_LOCAL id              { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMPICOBLAZE_LOCAL, $id));}
    | D_LOCAL                       {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_LOCAL, "LOCAL");
                                    }
    | label D_LOCAL                 {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_LOCAL, "LOCAL");
                                        $label->completeDelete();
                                    }
;
dir_exitm:
      D_EXITM                       { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_EXITM); }
    | label D_EXITM                 { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_EXITM)); }
    | D_EXITM args                  {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_ARG_EXPECTED_D("EXITM", $args, @args);
                                    }
    | label D_EXITM args            {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_ARG_EXPECTED_D("EXITM", $args, @args);
                                        $label->completeDelete();
                                    }
;
dir_list:
      D_LIST                        { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_LIST); }
    | label D_LIST                  { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_LIST)); }
    | D_LIST args                   {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_ARG_EXPECTED_D("LIST", $args, @args);
                                    }
    | label D_LIST args             {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_ARG_EXPECTED_D("LIST", $args, @args);
                                        $label->completeDelete();
                                    }
;
dir_nolist:
      D_NOLIST                      { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_NOLIST); }
    | label D_NOLIST                { $$ = $label->appendLink(new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_NOLIST)); }
    | D_NOLIST args                 {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_ARG_EXPECTED_D("NOLIST", $args, @args);
                                    }
    | label D_NOLIST args           {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_ARG_EXPECTED_D("NOLIST", $args, @args);
                                        $label->completeDelete();
                                    }
;
dir_set:
      id D_SET expr                 { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_SET,$id->appendLink($expr));}
    | id D_SET                      {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_SET, "SET");
                                        $id->completeDelete();
                                    }
    | D_SET                         {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_SET, "SET");
                                        ARG_REQUIRED_D(@D_SET, "SET");
                                    }
    | D_SET expr                    {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_SET, "SET");
                                        $expr->completeDelete();
                                    }
    | label D_SET expr              {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_SET, "SET");
                                        NO_LABEL_EXPECTED(@label, "SET", $label->appendArgsLink($expr))
                                    }
    | label id D_SET expr           {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label,"SET",$label->appendArgsLink($id->appendLink($expr)));
                                    }
;
dir_equ:
      id D_EQU expr                 { $$ = new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_EQU,$id->appendLink($expr));}
    | id D_EQU                      {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_EQU, "EQU");
                                        $id->completeDelete();
                                    }
    | D_EQU                         {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_EQU, "EQU");
                                        ARG_REQUIRED_D(@D_EQU, "EQU");
                                    }
    | D_EQU expr                    {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_EQU, "EQU");
                                        $expr->completeDelete();
                                    }
    | label D_EQU expr              {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_EQU, "EQU");
                                        NO_LABEL_EXPECTED(@label, "EQU", $label->appendArgsLink($expr))
                                    }
    | label id D_EQU expr           {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label,"EQU",$label->appendArgsLink($id->appendLink($expr)));
                                    }
;
dir_reg:
      id D_REG expr                 { $$ = new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_REG,$id->appendLink($expr)); }
    | id D_REG                      {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_REG, "REG");
                                        $id->completeDelete();
                                    }
    | D_REG                         {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_REG, "REG");
                                        ARG_REQUIRED_D(@D_REG, "REG");
                                    }
    | D_REG expr                    {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_REG, "REG");
                                        $expr->completeDelete();
                                    }
    | label D_REG expr              {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_REG, "REG");
                                        NO_LABEL_EXPECTED(@label, "REG", $label->appendArgsLink($expr))
                                    }
    | label id D_REG expr           {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label,"REG",$label->appendArgsLink($id->appendLink($expr)));
                                    }
;
dir_undefine:
      D_UNDEFINE id                 { $$ = new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_UNDEFINE,$id); }
    | D_UNDEFINE                    {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_UNDEFINE, "UNDEFINE");
                                    }
    | label D_UNDEFINE              {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_UNDEFINE, "UNDEFINE");
                                        NO_LABEL_EXPECTED(@label, "UNDEFINE", $label)
                                    }
    | label D_UNDEFINE id           {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label, "UNDEFINE", $label->appendArgsLink($id));
                                    }
;
dir_define:
      id D_DEFINE expr              { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_DEFINE,$id->appendLink($expr));}
    | id D_DEFINE                   {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_DEFINE, "DEFINE");
                                        $id->completeDelete();
                                    }
    | D_DEFINE                      {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_DEFINE, "DEFINE");
                                        ARG_REQUIRED_D(@D_DEFINE, "DEFINE");
                                    }
    | D_DEFINE expr                 {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_DEFINE, "DEFINE");
                                        $expr->completeDelete();
                                    }
    | label D_DEFINE expr           {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_DEFINE, "DEFINE");
                                        NO_LABEL_EXPECTED(@label, "DEFINE", $label->appendArgsLink($expr))
                                    }
    | label id D_DEFINE expr        {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label,
                                                          "DEFINE",
                                                          $label->appendArgsLink($id->appendLink($expr)));
                                    }
;
dir_macro:
      dir_macro_d
      statements EOL
      dir_endm                      { $$ = $dir_macro_d->createBranch($statements->appendLink($dir_endm)); }
    | dir_macro_d
      dir_endm                      { $$ = $dir_macro_d->createBranch($dir_endm); }
;
dir_macro_d:
      dir_macro_a EOL               { $$ = $dir_macro_a; }
    | dir_macro_a COMMENT EOL       { $$ = $dir_macro_a; }
    | dir_macro_a params EOL        { $$ = $dir_macro_a->appendArgsLink($params); }
    | dir_macro_a params COMMENT EOL{ $$ = $dir_macro_a->appendArgsLink($params); }
;
dir_macro_a:
      id D_MACRO                    { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_MACRO, $id); }
    | D_MACRO                       { /* Syntax error */ $$ = nullptr; DECL_ID_EXPECTED(@D_MACRO, "MACRO"); }
    | label D_MACRO                 {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label, "MACRO", $label);
                                        DECL_ID_EXPECTED(@D_MACRO, "MACRO");
                                    }
;
dir_endm:
      dir_endm_a COMMENT            { $$ = $dir_endm_a; }
    | dir_endm_a                    { $$ = $dir_endm_a; }
    | label dir_endm_a COMMENT      { $$ = $label->appendLink($dir_endm_a); }
    | label dir_endm_a              { $$ = $label->appendLink($dir_endm_a); }
;
dir_endm_a:
      D_ENDM                        { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ENDM); }
    | D_ENDM args                   { /* Syntax error */ $$ = nullptr; NO_ARG_EXPECTED_D("ENDM", $args, @args); }
;
dir_include:
      INCLUDE                       {
                                        CompilerSourceLocation location = compiler->toSourceLocation
                                                                            ( compiler -> m_yyllocStack.back() );
                                        location.m_fileNumber = compiler -> getFileNumber(1);

                                        CompilerExpr * arg = new CompilerExpr(compiler->getFileNumber($INCLUDE));
                                        delete [] $INCLUDE;
                                        $$ = new CompilerStatement ( location,
                                                                     ASMPICOBLAZE_INCLUDE,
                                                                     arg );
                                    }
    | label INCLUDE                 {
                                        CompilerSourceLocation location = compiler->toSourceLocation
                                                                              ( compiler -> m_yyllocStack.back() );
                                        location.m_fileNumber = compiler -> getFileNumber(1);


                                        CompilerExpr * arg = new CompilerExpr(compiler->getFileNumber($INCLUDE));
                                        delete [] $INCLUDE;
                                        $$ = $label->appendLink ( new CompilerStatement ( location,
                                                                                          ASMPICOBLAZE_INCLUDE,
                                                                                          arg ) );
                                    }
;
dir_end:
      D_END                         { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_END); }
    | label D_END                   { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_END)); }
    | D_END args                    {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("END", $args, @args);
                                        $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_END);
                                    }
    | label D_END args              {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("END", $args, @args);
                                        $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_END));
                                    }
;
dir_constant:
      D_CONSTANT id "," expr        { $$ = new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_EQU,$id->appendLink($expr)); }
    | label D_CONSTANT id "," expr  {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMPICOBLAZE_DIR_EQU,
                                                                                            $id -> appendLink($expr) ));
                                    }
    | D_CONSTANT expr               {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_CONSTANT, "CONSTANT");
                                        $expr->completeDelete();
                                    }
    | label D_CONSTANT expr         {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_CONSTANT, "CONSTANT");
                                        $label->completeDelete();
                                        $expr->completeDelete();
                                    }
;
dir_variable:
      D_VARIABLE id "," expr        { $$ = new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_SET,$id->appendLink($expr)); }
    | label D_VARIABLE id "," expr  {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMPICOBLAZE_DIR_SET,
                                                                                            $id->appendLink($expr) ) );
                                    }
    | D_VARIABLE expr               {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_VARIABLE, "VARIABLE");
                                        $expr->completeDelete();
                                    }
    | label D_VARIABLE expr         {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_VARIABLE, "VARIABLE");
                                        $label->completeDelete();
                                        $expr->completeDelete();
                                    }
;
dir_namereg:
      D_NAMEREG expr "," id        { $$ = new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_REG,$id->appendLink($expr)); }
    | label D_NAMEREG expr "," id  {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMPICOBLAZE_DIR_REG,
                                                                                            $id->appendLink($expr) ) );
                                    }
    | D_NAMEREG expr                {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_NAMEREG, "NAMEREG");
                                        $expr->completeDelete();
                                    }
    | label D_NAMEREG expr          {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_NAMEREG, "NAMEREG");
                                        $label->completeDelete();
                                        $expr->completeDelete();
                                    }
;
dir_repeat:
      dir_rept
      statements EOL
      dir_endr                      { $$ = $dir_rept->createBranch($statements->appendLink($dir_endr)); }
    | dir_rept
      dir_endr                      { $$ = $dir_rept->createBranch($dir_endr); }
;
dir_rept:
      D_REPT expr COMMENT           { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_REPT, $expr);               }
    | D_REPT expr                   { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_REPT, $expr);               }
    | D_REPT COMMENT                { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_REPT);                      }
    | label D_REPT expr COMMENT     {$$=$label->appendLink(new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_REPT,$expr));}
    | label D_REPT expr             {$$=$label->appendLink(new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_REPT,$expr));}
    | D_REPT                        { /* Syntax error */ $$ = nullptr;   ARG_REQUIRED_D(@D_REPT, "REPT");              }
    | label D_REPT COMMENT          { /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_REPT, "REPT");                 }
    | label D_REPT                  { /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_REPT, "REPT");                 }
;
dir_endr:
      dir_endr_a COMMENT            { $$ = $dir_endr_a;                     }
    | dir_endr_a                    { $$ = $dir_endr_a;                     }
    | label dir_endr_a COMMENT      { $$ = $label->appendLink($dir_endr_a); }
    | label dir_endr_a              { $$ = $label->appendLink($dir_endr_a); }
;
dir_endr_a:
      D_ENDR                        { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ENDR);               }
    | D_ENDR args                   { /* Syntax error */ $$ = nullptr; NO_ARG_EXPECTED_D("ENDR", $args, @args); }
;
dir_while:
      dir_while_a
      statements EOL
      dir_endw                      { $$ = $dir_while_a->createBranch($statements->appendLink($dir_endw)); }
    | dir_while_a
      dir_endw                      { $$ = $dir_while_a->createBranch($dir_endw);                          }
;
dir_while_a:
      D_WHILE expr COMMENT          { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_WHILE, $expr); }
    | D_WHILE expr                  { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_WHILE, $expr); }
    | D_WHILE COMMENT               { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_WHILE);        }
    | label D_WHILE expr COMMENT    {
                                        $$ = $label->appendLink ( new CompilerStatement ( LOC(@$),
                                                                                          ASMPICOBLAZE_DIR_WHILE,
                                                                                          $expr ) );
                                    }
    | label D_WHILE expr            {
                                        $$ = $label->appendLink ( new CompilerStatement ( LOC(@$),
                                                                                          ASMPICOBLAZE_DIR_WHILE,
                                                                                          $expr ) );
                                    }
    | D_WHILE                       { /* Syntax error */ $$ = nullptr;   ARG_REQUIRED_D(@D_WHILE, "WHILE"); }
    | label D_WHILE COMMENT         { /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_WHILE, "WHILE");    }
    | label D_WHILE                 { /* Syntax error */ $$ = $label; ARG_REQUIRED_D(@D_WHILE, "WHILE");    }
;
dir_endw:
      dir_endw_a COMMENT            { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ENDW); }
    | dir_endw_a                    { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ENDW); }
    | label dir_endw_a COMMENT      { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ENDW)); }
    | label dir_endw_a              { $$ = $label->appendLink(new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ENDW)); }
;
dir_endw_a:
      D_ENDW                        { }
    | D_ENDW args                   { /* Syntax error */ NO_ARG_EXPECTED_D("ENDM", $args, @args); }
;
dir_limit:
      D_LIMIT id "," expr           {
                                        const char * limSel = $id->lVal().m_data.m_symbol;
                                        if ( ( 0 != strcmp("R", limSel) )
                                                 &&
                                             ( 0 != strcmp("D", limSel) )
                                                 &&
                                             ( 0 != strcmp("C", limSel) ) )
                                        {
                                            $$ = nullptr;
                                            compiler->parserMessage ( compiler->toSourceLocation(@id),
                                                                      CompilerBase::MT_ERROR,
                                                                      QObject::tr("limit selector `%1' not understood")
                                                                                 .arg(limSel).toStdString() );
                                        }
                                        else
                                        {
                                            $$ = new CompilerStatement ( LOC(@$),
                                                                         ASMPICOBLAZE_DIR_LIMIT,
                                                                         $id->appendLink($expr) );
                                        }
                                    }
    | label D_LIMIT id "," expr     {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label,
                                                          "LIMIT",
                                                          $label->appendArgsLink ( $id->appendLink($expr) ) );
                                    }
;
dir_device:
      D_DEVICE id                   { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_DIR_DEVICE, $id ); }
    | D_DEVICE string               {
                                        std::string procType ( (const char *) $string->lVal().m_data.m_array.m_data,
                                                                $string->lVal().m_data.m_array.m_size );
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_DIR_DEVICE,
                                                                     new CompilerExpr ( procType, LOC(@string) ) );
                                        compiler->parserMessage ( compiler->toSourceLocation(@string),
                                                                  CompilerBase::MT_WARNING,
                                                                  QObject::tr ( "processor type (`%1') should be "
                                                                                "specified without double quotes "
                                                                                "(`\"')" )
                                                                              . arg ( procType.c_str() )
                                                                              . toStdString() );
                                        $string->completeDelete();
                                    }
    | label D_DEVICE id             {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED ( @label,
                                                            "DEVICE",
                                                            $label->appendArgsLink($id) );
                                    }
    | label D_DEVICE string         {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED ( @label,
                                                            "DEVICE",
                                                            $label->appendArgsLink($string) );
                                    }
;
dir_data:
      id D_DATA expr                { $$ = new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_DATA,$id->appendLink($expr));}
    | id D_DATA                     {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_DATA, "DATA");
                                        $id->completeDelete();
                                    }
    | D_DATA                        {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_DATA, "DATA");
                                        ARG_REQUIRED_D(@D_DATA, "DATA");
                                    }
    | D_DATA expr                   {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_DATA, "DATA");
                                        $expr->completeDelete();
                                    }
    | label D_DATA expr             {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_DATA, "DATA");
                                        NO_LABEL_EXPECTED(@label, "DATA", $label->appendArgsLink($expr));
                                    }
    | label id D_DATA expr          {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label,
                                                          "DATA",
                                                          $label->appendArgsLink ( $id->appendLink($expr) ) );
                                    }
;
dir_port:
      id D_PORT expr                { $$ = new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_PORT,$id->appendLink($expr));}
    | id D_PORT                     {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_PORT, "PORT");
                                        $id->completeDelete();
                                    }
    | D_PORT                        {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_PORT, "PORT");
                                        ARG_REQUIRED_D(@D_PORT, "PORT");
                                    }
    | D_PORT expr                   {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_PORT, "PORT");
                                        $expr->completeDelete();
                                    }
    | label D_PORT expr             {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_PORT, "PORT");
                                        NO_LABEL_EXPECTED(@label, "PORT", $label->appendArgsLink($expr));
                                    }
    | label id D_PORT expr          {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label,
                                                          "PORT",
                                                          $label->appendArgsLink ( $id->appendLink($expr) ) );
                                    }
;
dir_autoreg:
      dir_autoreg_a                 { $$ = $dir_autoreg_a; }
    | label dir_autoreg_a           {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label, "AUTOREG", $label->appendLink($dir_autoreg_a));
                                    }
;
dir_autoreg_a:
      id D_AUTOREG                  { $$ = new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_AUTOREG,$id); }
    | id D_AUTOREG AT expr          {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_DIR_AUTOREG,
                                                                     $id->appendLink ( $expr ) );
                                    }
    | id D_AUTOREG expr             {
                                        /* Syntax error */
                                        MISSING_AT_OPERATOR(@D_AUTOREG, "AUTOREG");
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_DIR_AUTOREG,
                                                                     $id->appendLink($expr) );
                                    }
    | D_AUTOREG                     {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_AUTOREG, "AUTOREG");
                                    }
    | D_AUTOREG AT expr             {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_AUTOREG, "AUTOREG");
                                        $expr->completeDelete();
                                    }
    | D_AUTOREG expr                {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_AUTOREG, "AUTOREG");
                                        MISSING_AT_OPERATOR(@D_AUTOREG, "D_AUTOREG");
                                        $expr->completeDelete();
                                    }
;
dir_autospr:
      dir_autospr_a                 { $$ = $dir_autospr_a; }
    | label dir_autospr_a           {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label, "AUTOSPR", $label->appendLink($dir_autospr_a));
                                    }
;
dir_autospr_a:
      id D_AUTOSPR                  { $$ = new CompilerStatement(LOC(@$),ASMPICOBLAZE_DIR_AUTOSPR,$id); }
    | id D_AUTOSPR AT expr          {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_DIR_AUTOSPR,
                                                                     $id->appendLink ( $expr ) );
                                    }
    | id D_AUTOSPR expr             {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_DIR_AUTOSPR,
                                                                     $id -> appendLink ( new CompilerExpr() )
                                                                         -> appendLink ( $expr ) );
                                    }
    | D_AUTOSPR                     {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_AUTOSPR, "AUTOSPR");
                                    }
    | D_AUTOSPR AT expr             {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_AUTOSPR, "AUTOSPR");
                                        $expr->completeDelete();
                                    }
    | D_AUTOSPR expr                {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        DECL_ID_EXPECTED(@D_AUTOSPR, "AUTOSPR");
                                        MISSING_AT_OPERATOR(@D_AUTOSPR, "D_AUTOSPR");
                                        $expr->completeDelete();
                                    }
;
dir_orgspr:
      D_ORGSPR expr                 { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ORGSPR, $expr); }
    | D_ORGSPR                      {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ORGSPR, "ORGSPR");
                                    }
;
dir_initspr:
      id D_INITSPR args_str         {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_DIR_INITSPR,
                                                                     $id->appendLink($args_str) );
                                    }
;
dir_mergespr:
      D_MERGESPR expr               { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_MERGESPR, $expr); }
    | D_MERGESPR                    {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_MERGESPR, "MERGESPR");
                                    }
;
dir_db:
      dir_db_a                      { $$ = $dir_db_a; }
    | label dir_db_a                { $$ = $label->appendLink($dir_db_a); }
;
dir_db_a:
      D_DB args_str                 { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_DB, $args_str); }
    | D_DB                          { /* Syntax error */ $$ = nullptr; ARG_REQUIRED_D(@D_DB, "DB"); }
;
dir_error:
      D_ERROR string                { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_ERROR, $string); }
    | label D_ERROR string          {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMPICOBLAZE_DIR_ERROR,
                                                                                            $string ) );
                                    }
    | label D_ERROR                 {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_ERROR, "ERROR");
                                        $label->completeDelete();
                                    }
    | D_ERROR                       { /* Syntax error */ $$ = nullptr; ARG_REQUIRED_D(@D_ERROR, "ERROR"); }
;
dir_warning:
      D_WARNING string              { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_WARNING, $string); }
    | label D_WARNING string        {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMPICOBLAZE_DIR_WARNING,
                                                                                            $string ) );
                                    }
    | label D_WARNING               {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_WARNING, "WARNING");
                                        $label->completeDelete();
                                    }
    | D_WARNING                     { /* Syntax error */ $$ = nullptr; ARG_REQUIRED_D(@D_WARNING, "WARNING"); }
;
dir_messg:
      D_MESSG string                { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_MESSG, $string); }
    | label D_MESSG string          {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMPICOBLAZE_DIR_MESSG,
                                                                                            $string ) );
                                    }
    | label D_MESSG                 {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_MESSG, "MESSG");
                                        $label->completeDelete();
                                    }
    | D_MESSG                       { /* Syntax error */ $$ = nullptr; ARG_REQUIRED_D(@D_MESSG, "MESSG"); }
;
dir_title:
      D_TITLE string                { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_TITLE, $string); }
    | label D_TITLE string          {
                                        $$ = $label -> appendLink ( new CompilerStatement ( LOC(@$),
                                                                                            ASMPICOBLAZE_DIR_TITLE,
                                                                                            $string ) );
                                    }
    | label D_TITLE                 {
                                        /* Syntax error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_TITLE, "TITLE");
                                        $label->completeDelete();
                                    }
    | D_TITLE                       { /* Syntax error */ $$ = nullptr; ARG_REQUIRED_D(@D_TITLE, "TITLE"); }
;
dir_expand:
      dir_expand_a                  { $$ = $dir_expand_a; }
    | dir_expand_a args             { /* Syntax error */ $$ = $dir_expand_a; NO_ARG_EXPECTED_D("EXPAND",$args,@args); }
;
dir_expand_a:
      D_EXPAND                      { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_EXPAND); }
    | label D_EXPAND                {
                                        /* Syntax error */
                                        $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_EXPAND);
                                        NO_LABEL_EXPECTED(@label, "EXPAND", $label);
                                    }
;
dir_noexpand:
      dir_noexpand_a                { $$ = $dir_noexpand_a; }
    | dir_noexpand_a args           { /* Syntax error */ $$=$dir_noexpand_a;NO_ARG_EXPECTED_D("NOEXPAND",$args,@args); }
;
dir_noexpand_a:
      D_NOEXPAND                    { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_NOEXPAND); }
    | label D_NOEXPAND              {
                                        /* Syntax error */
                                        $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_NOEXPAND);
                                        NO_LABEL_EXPECTED(@label, "NOEXPAND", $label);
                                    }
;
dir_code:
      dir_code_a                    { $$ = $1; }
    | label dir_code_a              { /* Syntax error */ $$ = $2; NO_LABEL_EXPECTED(@label, "CODE", $label); }
;
dir_code_a:
      D_CODE                        { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_CODE); }
    | D_CODE expr                   {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_DIR_CODE,
                                                                     ( new CompilerExpr() ) -> appendLink ( $expr ) );
                                    }
    | id D_CODE                     {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_DIR_CODE,
                                                                     $id->appendLink ( new CompilerExpr()) );
                                    }
    | id D_CODE expr                {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_DIR_CODE,
                                                                     $id->appendLink ( $expr ) );
                                    }
;
dir_failjmp:
      D_FAILJMP expr                { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_FAILJMP, $expr); }
    | D_FAILJMP                     {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@D_FAILJMP, "FAILJMP");
                                    }
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison
inline int AsmAdaptableParser_error ( YYLTYPE * yylloc,
                                   yyscan_t,
                                   CompilerParserInterface * compiler,
                                   const char * errorInfo )
{
    std::string errStr;

    if ( 0 == strncmp(errorInfo , "syntax error, unexpected ", 25) )
    {
        if ( (strlen(errorInfo) > 27) && ('_' == errorInfo[26]) )
        {
            switch ( errorInfo[25] )
            {
                case 'D':
                    errStr = QObject::tr("unexpected directive ").toStdString();
                    errStr += reinterpret_cast<const char *>( size_t(errorInfo) + 27 );
                    errorInfo = errStr.c_str();
                    break;
                case 'F':
                    errStr = QObject::tr("unexpected function ").toStdString();
                    errStr += reinterpret_cast<const char *>( size_t(errorInfo) + 27 );
                    errorInfo = errStr.c_str();
                    break;
                default:
                    errorInfo += 14;
                    break;
            }
        }
        else
        {
            errStr = QObject::tr("unexpected ").toStdString();
            errStr += reinterpret_cast<const char *>( size_t(errorInfo) + 25 );
            errorInfo = errStr.c_str();
        }
    }
    else if ( 0 == strncmp(errorInfo , "syntax error", 12) )
    {
        errorInfo = QObject::tr("syntax not understood").toStdString().c_str();
    }

    compiler->parserMessage(compiler->toSourceLocation(yylloc), CompilerBase::MT_ERROR, errorInfo);
    return 0;
}
