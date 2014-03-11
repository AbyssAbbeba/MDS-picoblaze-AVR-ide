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
%name-prefix "AsmPicoBlazeParser_"
// Generate a parser header file containing macro definitions, etc.
%defines "AsmPicoBlazeParser.h"
// Generate a pure, reentrant parser.
%define api.pure
// Generate the code processing the locations.
%locations
// Specify the programming language for the generated parser.
%language "C"
// Write an extra output file containing verbose descriptions of the parser states.
%verbose
// Expect exactly <n> shift/reduce conflicts in this grammar
%expect 125
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
    #include "AsmPicoBlazeLexer.h"

    // Name of the lexer function required by Bison
    #define AsmPicoBlazeParser_lex AsmPicoBlazeLexer_lex

    // Make a few things a little easier...
    #define LOC(location) \
        ( compiler->toSourceLocation(location) )

    // Error messages
    #define N_OPERANDS_EXPECTED(location, instruction, number) \
        compiler->parserMessage ( location, \
                                  CompilerBase::MT_ERROR, \
                                  QObject::tr ( "invalid number of operands, instruction %1 takes %2 operand(s)" ) \
                                              .arg(instruction).arg(number).toStdString() );
    #define NN_OPERANDS_EXPECTED(location, instruction, number, number2) \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("invalid number of operands, instruction %1 takes %2 or %3 operand(s)") \
                       .arg(instruction).arg(number).arg(number2).toStdString() );
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
    #define MISSIGN_COMMA(location, expression) \
        CompilerExpr::completeDelete(expression); \
        location.first_line = location.last_line; \
        location.first_column = location.last_column; \
        location.last_column++; \
        compiler->parserMessage ( location, CompilerBase::MT_ERROR, \
            QObject::tr("comma (`,') expected between operands").toStdString() );

    // Declaration of the error reporting function used by Bison
    inline int AsmPicoBlazeParser_error ( YYLTYPE * yylloc,
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

/* Instructions */
%token I_JUMP           I_CALL          I_RETURN        I_JUMP_Z        I_CALL_Z
%token I_RETURN_Z       I_JUMP_NZ       I_CALL_NZ       I_RETURN_NZ     I_JUMP_C
%token I_CALL_C         I_RETURN_C      I_JUMP_NC       I_CALL_NC       I_RETURN_NC
%token I_SUB            I_SUBCY         I_COMPARE       I_ENABLE_INT    I_DISABLE_INT
%token I_LOAD           I_AND           I_OR            I_XOR           I_TEST
%token I_SLX            I_SLA           I_RL            I_INPUT         I_OUTPUT
%token I_SRX            I_SRA           I_RR            I_SL0           I_SL1
%token I_STORE          I_FETCH         I_SR0           I_SR1           I_RETURNI_DIS
%token I_RETURNI_ENA    I_HWBUILD       I_STAR          I_TESTCY        I_COMPARECY
%token I_REGBANK_A      I_REGBANK_B     I_OUTPUTK       I_ADD           I_ADDCY
%token I_LD_RET

/* Pseudo-instructions */
%token I_CPL            I_CPL2          I_INC           I_DEC           I_SET
%token I_CLR            I_SETB          I_CLRB          I_DJNZ          I_IJNZ
%token I_NOTB

/* Special macros */
%token M_RTIF           M_RTELSEIF      M_RTELSE        M_RTENDIF       M_RTWHILE
%token M_RTENDW         M_RTFOR         M_RTENDF

/* Fixed operands, i.e. those which have no value */
%token Z NZ C NC

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
%type<expr>     id              string          cond            cndval          mark
%type<expr>     cond0
// Statements - general
%type<stmt>     statements      stmt            inst_stmt       dir_stmt        macro_stmt
%type<stmt>     instruction     directive       macro           label
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
// Statements - instructions
%type<stmt>     inst_jump       inst_call       inst_return     inst_add        inst_addcy
%type<stmt>     inst_sub        inst_subcy      inst_compare    inst_returni    inst_enable_int
%type<stmt>     inst_load       inst_and        inst_or         inst_xor        inst_disable_int
%type<stmt>     inst_test       inst_store      inst_fetch      inst_sr0        inst_sr1
%type<stmt>     inst_srx        inst_sra        inst_rr         inst_sl0        inst_sl1
%type<stmt>     inst_slx        inst_sla        inst_rl         inst_input      inst_output
%type<stmt>     inst_testcy     inst_comparecy  inst_outputk    inst_star       inst_hwbuild
%type<stmt>     inst_regbank    inst_ld_ret
// Statements - pseudo-instructions
%type<stmt>     inst_cpl        inst_cpl2       inst_inc        inst_dec        inst_set
%type<stmt>     inst_clr        inst_setb       inst_clrb       inst_djnz       inst_ijnz
%type<stmt>     inst_notb
// Statements - special macros
%type<stmt>     dir_rt_cond     rtif_block      rtelseif_block  rtelse_block    dir_rtif
%type<stmt>     dir_rtelseif    dir_rtelse      dir_rtendif     dir_rtwhile     dir_rtendw
%type<stmt>     dir_rtfor       dir_rtendf      dir_rtwhile_b   dir_rtfor_b     dir_rt_cond_a

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
    | inst_stmt                     { $$ = $1;   }
    | macro_stmt                    { $$ = $1;   }
;
dir_stmt:
      directive                     { $$ = $directive; }
    | directive COMMENT             { $$ = $directive; }
;
inst_stmt:
      instruction                   { $$ = $instruction;                     }
    | instruction COMMENT           { $$ = $instruction;                     }
    | label instruction             { $$ = $label->appendLink($instruction); }
    | label instruction COMMENT     { $$ = $label->appendLink($instruction); }
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
    | id "(" args ")"               { $$ = new CompilerExpr($id,CompilerExpr::OPER_CALL, $args, LOC(@$));  }
    | id "(" ")"                    {
                                        /* Syntax Error */
                                        $$ = $id;
                                        compiler->parserMessage ( @$,
                                                                  CompilerBase::MT_ERROR,
                                                                  QObject::tr("missing argument(s)").toStdString() );
                                    }
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
opr:            // Matches any operand, intended for error recovery only
      expr                          { $expr->completeDelete(); }
    | Z
    | NZ
    | C
    | NC
;
oprs:           // Matches any set of operands, intended for error recovery only
      oprs "," opr                  { }
    | opr                           { }
;
eopr:           // Matches any operand, intended for error recovery only
      expr                          { $expr->completeDelete(); }
    | "#" expr                      { $expr->completeDelete(); }
    | "@" expr                      { $expr->completeDelete(); }
;
eoprs:          // Matches any set of operands, intended for error recovery only
      eoprs "," eopr                { }
    | eopr                          { }
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
    | dir_rt_cond   { $$ = $1; }    | dir_rtwhile_b { $$ = $1; }
    | dir_rtfor_b   { $$ = $1; }    | dir_failjmp   { $$ = $1; }
    | dir_orgspr    { $$ = $1; }    | dir_initspr   { $$ = $1; }
    | dir_mergespr  { $$ = $1; }
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
dir_rt_cond:
      dir_rt_cond_a                 { $$ = $dir_rt_cond_a; }
    | label dir_rt_cond_a           { $$ = $label->appendLink($dir_rt_cond_a); }
;
dir_rt_cond_a:
      rtif_block rtelseif_block
      rtelse_block dir_rtendif      {
                                        $$ = new CompilerStatement ( CompilerSourceLocation(), ASMPICOBLAZE_RT_COND );
                                        $$->createBranch ( $rtif_block -> appendLink($rtelseif_block)
                                                                       -> appendLink($rtelse_block)
                                                                       -> appendLink($dir_rtendif) );
                                    }
;

rtif_block:
      dir_rtif statements EOL       { $$ = $dir_rtif->createBranch($2); }
;
rtelseif_block:
      /* empty */                   { $$ = nullptr; }
    | dir_rtelseif statements EOL   { $$ = $dir_rtelseif->createBranch($2); }
;
rtelse_block:
      /* empty */                   { $$ = nullptr; }
    | dir_rtelse statements EOL     { $$ = $dir_rtelse->createBranch($statements); }
    | dir_rtelse                    { $$ = $dir_rtelse; }
;
cond:
      cond0                         { $$ = $cond0; }
    | "(" cond0 ")"                 { $$ = $cond0; }
;
cond0:
      cndval "==" cndval            { $$ = new CompilerExpr($1, CompilerExpr::OPER_EQ,   $3, LOC(@$)); }
    | cndval "!=" cndval            { $$ = new CompilerExpr($1, CompilerExpr::OPER_NE,   $3, LOC(@$)); }
    | cndval "<=" cndval            { $$ = new CompilerExpr($1, CompilerExpr::OPER_LE,   $3, LOC(@$)); }
    | cndval ">=" cndval            { $$ = new CompilerExpr($1, CompilerExpr::OPER_GE,   $3, LOC(@$)); }
    | cndval "<"  cndval            { $$ = new CompilerExpr($1, CompilerExpr::OPER_LT,   $3, LOC(@$)); }
    | cndval ">"  cndval            { $$ = new CompilerExpr($1, CompilerExpr::OPER_GT,   $3, LOC(@$)); }
    | cndval "&"  cndval            { $$ = new CompilerExpr($1, CompilerExpr::OPER_BAND, $3, LOC(@$)); }
    | cndval "!&" cndval            { $$ = new CompilerExpr($1, CompilerExpr::OPER_NAND, $3, LOC(@$)); }
;
cndval:
      id                            { $$ = new CompilerExpr($id, LOC(@$));                              }
    | "#" id                        { $$ = new CompilerExpr($id, CompilerExpr::OPER_HASH, LOC(@$));     }
    | number                        { $$ = new CompilerExpr($number, LOC(@$));                          }
    | "#" number                    { $$ = new CompilerExpr($number, CompilerExpr::OPER_HASH, LOC(@$)); }
;
dir_rtif:
      M_RTIF cond                   { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_RTIF, $cond); }
    | M_RTIF                        {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@M_RTIF, "RTIF");
                                    }
;
dir_rtelseif:
      M_RTELSEIF cond               { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_RTELSEIF, $cond); }
    | M_RTELSEIF                    {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@M_RTELSEIF, "RTELSEIF");
                                    }
    | label M_RTELSEIF expr         {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label, "RTELSEIF", $label);
                                        $expr->completeDelete();
                                    }
    | label M_RTELSEIF              {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@M_RTELSEIF, "RTELSEIF");
                                        NO_LABEL_EXPECTED(@label, "RTELSEIF", $label);
                                    }
;
dir_rtelse:
      M_RTELSE                      { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_RTELSE); }
    | label M_RTELSE                {
                                        /* Syntax error */
                                        NO_LABEL_EXPECTED(@label, "RTELSE", $label);
                                        $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_RTELSE);
                                    }
    | label M_RTELSE args           {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("RTELSE", $args, @args);
                                        NO_LABEL_EXPECTED(@label, "RTELSE", $label);
                                        $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_RTELSE);
                                    }
    | M_RTELSE args                 {
                                        /* Syntax error */
                                        NO_ARG_EXPECTED_D("RTELSE", $args, @args);
                                        $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_RTELSE);
                                    }
;
dir_rtendif:
      M_RTENDIF                     { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_RTENDIF); }
;
dir_rtwhile_b:
      dir_rtwhile statements EOL
      dir_rtendw                    { $$ = $dir_rtwhile->createBranch($statements->appendLink($dir_rtendw)); }
;
dir_rtwhile:
      M_RTWHILE cond                { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_RTWHILE, $cond); }
    | M_RTWHILE                     {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@M_RTWHILE, "RTWHILE");
                                    }
    | label M_RTWHILE expr          {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        NO_LABEL_EXPECTED(@label, "RTWHILE", $label);
                                        $expr->completeDelete();
                                    }
    | label M_RTWHILE               {
                                        /* Syntax Error */
                                        $$ = nullptr;
                                        ARG_REQUIRED_D(@M_RTWHILE, "RTWHILE");
                                        NO_LABEL_EXPECTED(@label, "RTWHILE", $label);
                                    }
;
dir_rtendw:
      M_RTENDW                      { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_RTENDW); }
;
dir_rtfor_b:
      dir_rtfor statements EOL
      dir_rtendf                    { $$ = $dir_rtfor->createBranch($statements->appendLink($dir_rtendf)); }
;
dir_rtfor:
      M_RTFOR id "," expr           {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_DIR_RTFOR,
                                                                     $id->appendLink($expr) );
                                    }
    | M_RTFOR id "," expr ".." expr {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_DIR_RTFOR,
                                                                     $id->appendLink($4)->appendLink($6) );
                                    }
    | M_RTFOR id "," expr ".." expr
      "," expr
                                    {
                                        $id->appendLink($4)->appendLink($6)->appendLink($8);
                                        $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_RTFOR, $id);
                                    }
;
dir_rtendf:
      M_RTENDF                      { $$ = new CompilerStatement(LOC(@$), ASMPICOBLAZE_DIR_RTENDF); }
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

/*
 * Instructions
 */
instruction:
    /* Program Control Group */
      inst_jump         { $$ = $1; } | inst_call        { $$ = $1; }
    | inst_return       { $$ = $1; } | inst_ld_ret      { $$ = $1; }
    /* Arithmetic Group */
    | inst_add          { $$ = $1; } | inst_addcy       { $$ = $1; }
    | inst_sub          { $$ = $1; } | inst_subcy       { $$ = $1; }
    /* Interrupt Group */
    | inst_returni      { $$ = $1; } | inst_enable_int  { $$ = $1; }
    | inst_disable_int  { $$ = $1; }
    /* Register Loading Group */
    | inst_load         { $$ = $1; } | inst_star        { $$ = $1; }
    /* Logical Group */
    | inst_and          { $$ = $1; } | inst_or          { $$ = $1; }
    | inst_xor          { $$ = $1; }
    /* Test & Compare Group */
    | inst_test         { $$ = $1; } | inst_compare     { $$ = $1; }
    | inst_testcy       { $$ = $1; } | inst_comparecy   { $$ = $1; }
    /* Storage Group */
    | inst_store        { $$ = $1; } | inst_fetch       { $$ = $1; }
    /* Shift And Rotate Group */
    | inst_sr0          { $$ = $1; } | inst_sr1         { $$ = $1; }
    | inst_srx          { $$ = $1; } | inst_sra         { $$ = $1; }
    | inst_rr           { $$ = $1; } | inst_sl0         { $$ = $1; }
    | inst_sl1          { $$ = $1; } | inst_slx         { $$ = $1; }
    | inst_sla          { $$ = $1; } | inst_rl          { $$ = $1; }
    /* Input/Output Group */
    | inst_input        { $$ = $1; } | inst_output      { $$ = $1; }
    | inst_outputk      { $$ = $1; }
    /* Version Control Group */
    | inst_hwbuild      { $$ = $1; }
    /* Register Bank Selection Group */
    | inst_regbank      { $$ = $1; }

    /* Pseudo-instructions */
    | inst_cpl          { $$ = $1; } | inst_cpl2        { $$ = $1; }
    | inst_inc          { $$ = $1; } | inst_dec         { $$ = $1; }
    | inst_set          { $$ = $1; } | inst_clr         { $$ = $1; }
    | inst_setb         { $$ = $1; } | inst_clrb        { $$ = $1; }
    | inst_djnz         { $$ = $1; } | inst_ijnz        { $$ = $1; }
    | inst_notb         { $$ = $1; }
;

/* Program Control Group */
inst_jump:
      I_JUMP expr                   { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_JUMP_AAA,    $expr ); }
    | I_JUMP_Z expr                 { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_JUMP_Z_AAA,  $expr ); }
    | I_JUMP_NZ expr                { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_JUMP_NZ_AAA, $expr ); }
    | I_JUMP_C expr                 { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_JUMP_C_AAA,  $expr ); }
    | I_JUMP_NC expr                { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_JUMP_NC_AAA, $expr ); }
    | I_JUMP "@"
      "(" expr "," expr ")"         {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_JUMP_SX_SY,
                                                                     $4->appendLink($6) );
                                    }
    | I_JUMP opr "," opr "," oprs   { /* Syntax Error */ $$ = nullptr; NN_OPERANDS_EXPECTED(@1, "JUMP", 1, 2);   }
    | I_JUMP                        { /* Syntax Error */ $$ = nullptr; NN_OPERANDS_EXPECTED(@1, "JUMP", 1, 2);   }
    | I_JUMP opr oprs               { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                  }
;
inst_call:
      I_CALL expr                   { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_CALL_AAA,    $expr ); }
    | I_CALL_Z expr                 { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_CALL_Z_AAA,  $expr ); }
    | I_CALL_NZ expr                { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_CALL_NZ_AAA, $expr ); }
    | I_CALL_C expr                 { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_CALL_C_AAA,  $expr ); }
    | I_CALL_NC expr                { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_CALL_NC_AAA, $expr ); }
    | I_CALL "@"
      "(" expr "," expr ")"         {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_CALL_SX_SY,
                                                                     $4->appendLink($6) );
                                    }
    | I_CALL opr "," opr "," oprs   { /* Syntax Error */ $$ = nullptr; NN_OPERANDS_EXPECTED(@1, "CALL", 1, 2);   }
    | I_CALL                        { /* Syntax Error */ $$ = nullptr; NN_OPERANDS_EXPECTED(@1, "CALL", 1, 2);   }
    | I_CALL opr oprs               { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                  }
;
inst_return:
      I_RETURN                      { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_RETURN    ); }
    | I_RETURN_Z                    { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_RETURN_Z  ); }
    | I_RETURN_NZ                   { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_RETURN_NZ ); }
    | I_RETURN_C                    { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_RETURN_C  ); }
    | I_RETURN_NC                   { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_RETURN_NC ); }
    | I_RETURN opr "," oprs         { /* Syntax Error */ $$ = nullptr; NN_OPERANDS_EXPECTED(@1, "RETURN", 0, 1); }
;
inst_ld_ret:
      I_LD_RET expr "," "#" expr    {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_LD_RET_SX_KK,
                                                                     $2->appendLink($5) );
                                    }
    | I_LD_RET
      eopr "," eopr "," eoprs       { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "LDRET", 2); }
    | I_LD_RET eopr                 { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "LDRET", 2); }
    | I_LD_RET                      { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "LDRET", 2); }
    | I_LD_RET eopr eoprs           { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);             }
;
/* Arithmetic Group */
inst_add:
      I_ADD expr "," "#" expr       { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_ADD_SX_KK,$2->appendLink($5)); }
    | I_ADD expr "," expr           { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_ADD_SX_SY,$2->appendLink($4)); }
    | I_ADD eopr "," eopr "," eoprs { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "ADD", 2);                 }
    | I_ADD eopr                    { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "ADD", 2);                 }
    | I_ADD                         { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "ADD", 2);                 }
    | I_ADD eopr eoprs              { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                           }
;
inst_addcy:
      I_ADDCY expr "," "#" expr     {$$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_ADDCY_SX_KK,$2->appendLink($5));}
    | I_ADDCY expr "," expr         {$$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_ADDCY_SX_SY,$2->appendLink($4));}
    | I_ADDCY eopr "," eopr "," eoprs { /* Syntax Error */ $$=nullptr; N_OPERANDS_EXPECTED(@1, "ADDCY", 2);               }
    | I_ADDCY eopr                  { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "ADDCY", 2);               }
    | I_ADDCY                       { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "ADDCY", 2);               }
    | I_ADDCY eopr eoprs            { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                           }
;
inst_sub:
      I_SUB expr "," "#" expr       { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_SUB_SX_KK,$2->appendLink($5)); }
    | I_SUB expr "," expr           { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_SUB_SX_SY,$2->appendLink($4)); }
    | I_SUB eopr "," eopr "," eoprs { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SUB", 2);                 }
    | I_SUB eopr                    { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "SUB", 2);                 }
    | I_SUB                         { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SUB", 2);                 }
    | I_SUB eopr eoprs              { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                           }
;
inst_subcy:
      I_SUBCY expr "," "#" expr     {$$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_SUBCY_SX_KK,$2->appendLink($5));}
    | I_SUBCY expr "," expr         {$$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_SUBCY_SX_SY,$2->appendLink($4));}
    | I_SUBCY eopr "," eopr "," eoprs { /* Syntax Error */ $$=nullptr; N_OPERANDS_EXPECTED(@1, "SUBCY", 2);               }
    | I_SUBCY eopr                  { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "SUBCY", 2);               }
    | I_SUBCY                       { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SUBCY", 2);               }
    | I_SUBCY eopr eoprs            { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                           }
;
/* Interrupt Group */
inst_returni:
      I_RETURNI_ENA                 { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_RETURNI_ENA );           }
    | I_RETURNI_DIS                 { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_RETURNI_DIS );           }
    | I_RETURNI_ENA eoprs           { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "RETURNI ENABLE", 0);     }
    | I_RETURNI_DIS eoprs           { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "RETURNI DISABLE", 0);    }
;
inst_enable_int:
      I_ENABLE_INT                  { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_ENABLE_INT );             }
    | I_ENABLE_INT eoprs            { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "ENABLE INTERRUPT", 0);    }
;
inst_disable_int:
      I_DISABLE_INT                 { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_DISABLE_INT );            }
    | I_DISABLE_INT eoprs           { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "DISABLE INTERRUPT", 0);   }
;
/* Register Loading Group */
inst_load:
      I_LOAD expr "," "#" expr      { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_LOAD_SX_KK,$2->appendLink($5));}
    | I_LOAD expr "," expr          { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_LOAD_SX_SY,$2->appendLink($4));}
    | I_LOAD "&" I_RETURN expr
      "," "#" expr                  {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_LD_RET_SX_KK,
                                                                     $4->appendLink($7) );
                                    }
    | I_LOAD eopr "," eopr "," eoprs{ /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "LOAD", 2); }
    | I_LOAD eopr                   { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "LOAD", 2); }
    | I_LOAD                        { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "LOAD", 2); }
    | I_LOAD eopr eoprs             { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);         }
;
inst_star:
      I_STAR expr "," expr          { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_STAR_SX_SY,$2->appendLink($4));}
    | I_STAR eopr "," eopr "," eoprs{ /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "LOAD", 2);             }
    | I_STAR eopr                   { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "LOAD", 2);             }
    | I_STAR                        { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "LOAD", 2);             }
    | I_STAR eopr eoprs             { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                     }
;
/* Logical Group */
inst_and:
      I_AND expr "," "#" expr       { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_AND_SX_KK,$2->appendLink($5)); }
    | I_AND expr "," expr           { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_AND_SX_SY,$2->appendLink($4)); }
    | I_AND eopr "," eopr "," eoprs { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "AND", 2);              }
    | I_AND eopr                    { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "AND", 2);              }
    | I_AND                         { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "AND", 2);              }
    | I_AND eopr eoprs              { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                     }
;
inst_or:
      I_OR expr "," "#" expr        { $$ = new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_OR_SX_KK,$2->appendLink($5));}
    | I_OR expr "," expr            { $$ = new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_OR_SX_SY,$2->appendLink($4));}
    | I_OR eopr "," eopr "," eoprs  { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "OR", 2);               }
    | I_OR eopr                     { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "OR", 2);               }
    | I_OR                          { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "OR", 2);               }
    | I_OR eopr eoprs               { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                     }
;
inst_xor:
      I_XOR expr "," "#" expr       { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_XOR_SX_KK,$2->appendLink($5)); }
    | I_XOR expr "," expr           { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_XOR_SX_SY,$2->appendLink($4)); }
    | I_XOR eopr "," eopr "," eoprs { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "XOR", 2);              }
    | I_XOR eopr                    { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "XOR", 2);              }
    | I_XOR                         { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "XOR", 2);              }
    | I_XOR eopr eoprs              { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                     }
;
/* Storage Group */
inst_store:
      I_STORE expr "," expr         {$$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_STORE_SX_SS,$2->appendLink($4));}
    | I_STORE expr "," "@" expr     {$$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_STORE_SX_SY,$2->appendLink($5));}
    | I_STORE eopr "," eopr "," eoprs { /* Syntax Error */ $$=nullptr; N_OPERANDS_EXPECTED(@1, "STORE", 2);            }
    | I_STORE eopr                  { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "STORE", 2);            }
    | I_STORE                       { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "STORE", 2);            }
    | I_STORE eopr eoprs            { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                     }
;
inst_fetch:
      I_FETCH expr "," expr         {$$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_FETCH_SX_SS,$2->appendLink($4));}
    | I_FETCH expr "," "@" expr     {$$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_FETCH_SX_SY,$2->appendLink($5));}
    | I_FETCH eopr "," eopr "," eoprs { /* Syntax Error */$$= nullptr; N_OPERANDS_EXPECTED(@1, "FETCH", 2);            }
    | I_FETCH eopr                  { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "FETCH", 2);            }
    | I_FETCH                       { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "FETCH", 2);            }
    | I_FETCH eopr eoprs            { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                     }
;
/* Shift And Rotate Group */
inst_sr0:
      I_SR0 expr                    { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_SR0_SX, $2 );    }
    | I_SR0 eopr "," eoprs          { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SR0", 1);     }
    | I_SR0                         { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SR0", 1);     }
;
inst_sr1:
      I_SR1 expr                    { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_SR1_SX, $2 );    }
    | I_SR1 eopr "," eoprs          { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SR1", 1);     }
    | I_SR1                         { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SR1", 1);     }
;
inst_srx:
      I_SRX expr                    { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_SRX_SX, $2 );    }
    | I_SRX eopr "," eoprs          { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SRX", 1);     }
    | I_SRX                         { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SRX", 1);     }
;
inst_sra:
      I_SRA expr                    { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_SRA_SX, $2 ); }
    | I_SRA eopr "," eoprs          { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SRA", 1);     }
    | I_SRA                         { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SRA", 1);     }
;
inst_rr:
      I_RR expr                     { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_RR_SX, $2 ); }
    | I_RR eopr "," eoprs           { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "RR", 1);      }
    | I_RR                          { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "RR", 1);      }
;
inst_sl0:
      I_SL0 expr                    { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_SL0_SX, $2 ); }
    | I_SL0 eopr "," eoprs          { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SL0", 1);     }
    | I_SL0                         { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SL0", 1);     }
;
inst_sl1:
      I_SL1 expr                    { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_SL1_SX, $2 ); }
    | I_SL1 eopr "," eoprs          { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SL1", 1);     }
    | I_SL1                         { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SL1", 1);     }
;
inst_slx:
      I_SLX expr                    { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_SLX_SX, $2 ); }
    | I_SLX eopr "," eoprs          { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SLX", 1);     }
    | I_SLX                         { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SLX", 1);     }
;
inst_sla:
      I_SLA expr                    { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_SLA_SX, $2 ); }
    | I_SLA eopr "," eoprs          { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SLA", 1);     }
    | I_SLA                         { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "SLA", 1);     }
;
inst_rl:
      I_RL expr                     { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_RL_SX, $2 ); }
    | I_RL eopr "," eoprs           { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "RL", 1);      }
    | I_RL                          { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "RL", 1);      }
;
/* Input/Output Group */
inst_input:
      I_INPUT expr "," expr         {$$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_INPUT_SX_PP,$2->appendLink($4));}
    | I_INPUT expr "," "@" expr     {$$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_INPUT_SX_SY,$2->appendLink($5));}
    | I_INPUT eopr "," eopr "," eoprs {/* Syntax Error */$$ = nullptr; N_OPERANDS_EXPECTED(@1, "INPUT", 2);               }
    | I_INPUT eopr                  { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "INPUT", 2);               }
    | I_INPUT                       { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "INPUT", 2);               }
    | I_INPUT eopr eoprs            { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                           }
;
inst_output:
      I_OUTPUT expr "," expr        {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_OUTPUT_SX_PP,
                                                                     $2->appendLink ( $4 ) );
                                    }
    | I_OUTPUT expr "," "@" expr    {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_OUTPUT_SX_SY,
                                                                     $2->appendLink ( $5 ) );
                                    }
    | I_OUTPUT
      eopr "," eopr "," eoprs       { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "OUTPUT", 2); }
    | I_OUTPUT eopr                 { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "OUTPUT", 2); }
    | I_OUTPUT                      { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "OUTPUT", 2); }
    | I_OUTPUT eopr eoprs           { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);              }
;
inst_outputk:
      I_OUTPUTK "#" expr "," expr   {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_OUTPUTK_KK_P,
                                                                     $3->appendLink ( $5 ) );
                                    }
    | I_OUTPUTK
      eopr "," eopr "," eoprs       { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "OUTPUTK", 2); }
    | I_OUTPUTK eopr                { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "OUTPUTK", 2); }
    | I_OUTPUTK                     { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "OUTPUTK", 2); }
    | I_OUTPUTK eopr eoprs          { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);               }
;
/* Test & Compare Group */
inst_test:
      I_TEST expr "," "#" expr      { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_TEST_SX_KK,$2->appendLink($5));}
    | I_TEST expr "," expr          { $$=new CompilerStatement(LOC(@$),ASMPICOBLAZE_INS_TEST_SX_SY,$2->appendLink($4));}
    | I_TEST eopr "," eopr "," eoprs{ /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "TEST", 2);                }
    | I_TEST eopr                   { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "TEST", 2);                }
    | I_TEST                        { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "TEST", 2);                }
    | I_TEST eopr eoprs             { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                           }
;
inst_testcy:
      I_TESTCY expr "," "#" expr    {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_TESTCY_SX_KK,
                                                                     $2->appendLink($5));
                                    }
    | I_TESTCY expr "," expr        {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_TESTCY_SX_SY,
                                                                     $2->appendLink($4));
                                    }
    | I_TESTCY
      eopr "," eopr "," eoprs       { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "TESTCY", 2); }
    | I_TESTCY eopr                 { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "TESTCY", 2); }
    | I_TESTCY                      { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "TESTCY", 2); }
    | I_TESTCY eopr eoprs           { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);              }
;
inst_compare:
      I_COMPARE expr "," "#" expr   {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_COMPARE_SX_KK,
                                                                     $2->appendLink ( $5 ) );
                                    }
    | I_COMPARE expr "," expr       {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_COMPARE_SX_SY,
                                                                     $2->appendLink($4) );
                                    }
    | I_COMPARE
      eopr "," eopr "," eoprs       { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "COMPARE", 2); }
    | I_COMPARE eopr                { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "COMPARE", 2); }
    | I_COMPARE                     { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "COMPARE", 2); }
    | I_COMPARE eopr eoprs          { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);               }
;
inst_comparecy:
      I_COMPARECY expr "," "#" expr {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_COMPARECY_SX_KK,
                                                                     $2->appendLink ( $5 ) );
                                    }
    | I_COMPARECY expr "," expr     {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_COMPARECY_SX_SY,
                                                                     $2->appendLink($4) );
                                    }
    | I_COMPARECY
      eopr "," eopr "," eoprs       { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "COMPARECY", 2); }
    | I_COMPARECY eopr              { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "COMPARECY", 2); }
    | I_COMPARECY                   { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "COMPARECY", 2); }
    | I_COMPARECY eopr eoprs        { /* Syntax Error */ $$ = nullptr; MISSIGN_COMMA(@2, nullptr);                 }
;


/* Version Control Group */
inst_hwbuild:
      I_HWBUILD expr                { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_HWBUILD_SX, $expr ); }
    | I_HWBUILD eopr "," eoprs      { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "HWBUILD", 1);        }
    | I_HWBUILD                     { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@1, "HWBUILD", 1);        }
;
/* Register Bank Selection Group */
inst_regbank:
      I_REGBANK_A                   { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_REGBANK_A );    }
    | I_REGBANK_B                   { $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_REGBANK_B );    }
    | I_REGBANK_A eoprs             { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "REGBANK A", 0); }
    | I_REGBANK_B eoprs             { /* Syntax Error */ $$ = nullptr; N_OPERANDS_EXPECTED(@2, "REGBANK B", 0); }
;

inst_cpl:
      I_CPL expr                    {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_XOR_SX_KK,
                                                                     $expr->appendLink (
                                                                        new CompilerExpr ( 0xff, LOC(@$) ) ) );
                                    }
;
inst_cpl2:
      I_CPL2 expr                   {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_XOR_SX_KK,
                                                                     $expr->copyChainLink()->appendLink (
                                                                        new CompilerExpr ( 0xff, LOC(@$) ) ) );

                                        $$->appendLink ( new CompilerStatement ( LOC(@$),
                                                                                 ASMPICOBLAZE_INS_ADD_SX_KK,
                                                                                 $expr->appendLink (
                                                                                    new CompilerExpr ( 0x01, LOC(@$) )
                                                        ) ) );
                                    }
;
inst_inc:
      I_INC expr                    {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_ADD_SX_KK,
                                                                     $expr->appendLink(new CompilerExpr(0x01,LOC(@$))));
                                    }
;
inst_dec:
      I_DEC expr                    {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_SUB_SX_KK,
                                                                     $expr->appendLink(new CompilerExpr(0x01,LOC(@$))));
                                    }
;
inst_set:
      I_SET expr                    {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_OR_SX_KK,
                                                                     $expr->appendLink(new CompilerExpr(0xff,LOC(@$))));
                                    }
;
inst_clr:
      I_CLR expr                    {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_AND_SX_KK,
                                                                     $expr->appendLink(new CompilerExpr(0x00,LOC(@$))));
                                    }
;
inst_setb:
      I_SETB expr "," expr          {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_OR_SX_KK,
                                                                     $2->appendLink (
                                                                        new CompilerExpr ( 0x01,
                                                                                           CompilerExpr::OPER_SHL,
                                                                                           $4,
                                                                                           LOC(@4) ) ) );
                                    }
;
inst_clrb:
      I_CLRB expr "," expr          {
                                        $$ = new CompilerStatement ( LOC(@$), ASMPICOBLAZE_INS_AND_SX_KK, $2->appendLink
                                        ( new CompilerExpr ( 0xff, '^', new CompilerExpr ( 0x01, CompilerExpr::OPER_SHL,
                                        $4, LOC(@4) ), LOC(@4) ) ) );
                                    }
;
inst_notb:
      I_NOTB expr "," expr          {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_XOR_SX_KK,
                                                                     $2->appendLink (
                                                                        new CompilerExpr ( 0x01,
                                                                                           CompilerExpr::OPER_SHL,
                                                                                           $4,
                                                                                           LOC(@4) ) ) );
                                    }
;
inst_djnz:
      I_DJNZ expr "," expr          {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_SUB_SX_KK,
                                                                     $2->appendLink(new CompilerExpr(0x01,LOC(@$))));

                                        $$->appendLink ( new CompilerStatement ( LOC(@$),
                                                                                 ASMPICOBLAZE_INS_JUMP_NZ_AAA,
                                                                                 $4 ) );
                                    }
;
inst_ijnz:
      I_IJNZ expr "," expr          {
                                        $$ = new CompilerStatement ( LOC(@$),
                                                                     ASMPICOBLAZE_INS_ADD_SX_KK,
                                                                     $2->appendLink(new CompilerExpr(0x01,LOC(@$))));

                                        $$->appendLink ( new CompilerStatement ( LOC(@$),
                                                                                 ASMPICOBLAZE_INS_JUMP_NZ_AAA,
                                                                                 $4 ) );
                                    }
;

%%

// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison
inline int AsmPicoBlazeParser_error ( YYLTYPE * yylloc,
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
                case 'I':
                    errStr = QObject::tr("unexpected instruction ").toStdString();
                    errStr += reinterpret_cast<const char *>( size_t(errorInfo) + 27 );
                    errorInfo = errStr.c_str();
                    break;
                case 'F':
                    errStr = QObject::tr("unexpected function ").toStdString();
                    errStr += reinterpret_cast<const char *>( size_t(errorInfo) + 27 );
                    errorInfo = errStr.c_str();
                    break;
                case 'M':
                    errStr = QObject::tr("unexpected special macro ").toStdString();
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
