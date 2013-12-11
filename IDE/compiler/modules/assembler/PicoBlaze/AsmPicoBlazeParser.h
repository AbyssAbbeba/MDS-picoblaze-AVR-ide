/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* "%code requires" blocks.  */

/* Line 2068 of yacc.c  */
#line 62 "AsmPicoBlazeParser.y"

    #include "CompilerParserInterface.h"
    #ifndef YY_TYPEDEF_YY_SCANNER_T
            #define YY_TYPEDEF_YY_SCANNER_T
            typedef void* yyscan_t;
    #endif // YY_TYPEDEF_YY_SCANNER_T



/* Line 2068 of yacc.c  */
#line 48 "AsmPicoBlazeParser.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     END = 0,
     D_NAMEREG = 258,
     D_ORG = 259,
     D_CONSTANT = 260,
     D_WHILE = 261,
     D_DB = 262,
     D_LIST = 263,
     D_MESSG = 264,
     D_NOLIST = 265,
     D_SKIP = 266,
     D_ERROR = 267,
     D_TITLE = 268,
     D_EXPAND = 269,
     D_NOEXPAND = 270,
     D_IF = 271,
     D_PORT = 272,
     D_IFN = 273,
     D_IFDEF = 274,
     D_IFNDEF = 275,
     D_ELSEIFB = 276,
     D_ELSEIFNB = 277,
     D_ELSE = 278,
     D_ELSEIF = 279,
     D_ELSEIFN = 280,
     D_ELSEIFDEF = 281,
     D_ELSEIFNDEF = 282,
     D_ENDIF = 283,
     D_LOCAL = 284,
     D_IFNB = 285,
     D_IFB = 286,
     D_LIMIT = 287,
     D_ENDM = 288,
     D_EXITM = 289,
     D_REPT = 290,
     D_MACRO = 291,
     D_EQU = 292,
     D_END = 293,
     D_REG = 294,
     D_CODE = 295,
     D_ENDW = 296,
     D_WARNING = 297,
     D_VARIABLE = 298,
     D_SET = 299,
     D_DEFINE = 300,
     D_UNDEFINE = 301,
     D_ENDR = 302,
     D_AUTOREG = 303,
     D_AUTOSPR = 304,
     D_DATA = 305,
     D_DEVICE = 306,
     D_ADDRESS = 307,
     I_JUMP = 308,
     I_CALL = 309,
     I_RETURN = 310,
     I_JUMP_Z = 311,
     I_CALL_Z = 312,
     I_RETURN_Z = 313,
     I_JUMP_NZ = 314,
     I_CALL_NZ = 315,
     I_RETURN_NZ = 316,
     I_JUMP_C = 317,
     I_CALL_C = 318,
     I_RETURN_C = 319,
     I_JUMP_NC = 320,
     I_CALL_NC = 321,
     I_RETURN_NC = 322,
     I_SUB = 323,
     I_SUBCY = 324,
     I_COMPARE = 325,
     I_ENABLE_INT = 326,
     I_DISABLE_INT = 327,
     I_LOAD = 328,
     I_AND = 329,
     I_OR = 330,
     I_XOR = 331,
     I_TEST = 332,
     I_SLX = 333,
     I_SLA = 334,
     I_RL = 335,
     I_INPUT = 336,
     I_OUTPUT = 337,
     I_SRX = 338,
     I_SRA = 339,
     I_RR = 340,
     I_SL0 = 341,
     I_SL1 = 342,
     I_STORE = 343,
     I_FETCH = 344,
     I_SR0 = 345,
     I_SR1 = 346,
     I_RETURNI_DIS = 347,
     I_RETURNI_ENA = 348,
     I_HWBUILD = 349,
     I_STAR = 350,
     I_TESTCY = 351,
     I_COMPARECY = 352,
     I_REGBANK_A = 353,
     I_REGBANK_B = 354,
     I_OUTPUTK = 355,
     I_ADD = 356,
     I_ADDCY = 357,
     I_LD_RET = 358,
     M_RTIF = 359,
     M_RTELSEIF = 360,
     M_RTELSE = 361,
     M_RTENDIF = 362,
     M_RTWHILE = 363,
     M_RTENDW = 364,
     M_RTFOR = 365,
     M_RTENDF = 366,
     Z = 367,
     NZ = 368,
     C = 369,
     NC = 370,
     COMMENT = 371,
     AT = 372,
     AT_MARK = 373,
     EOL = 374,
     LP = 375,
     RP = 376,
     IMMEDIATE = 377,
     COMMA = 378,
     INTERVAL = 379,
     PLUS = 380,
     MINUS = 381,
     ASTERISK = 382,
     SLASH = 383,
     NOT = 384,
     MOD = 385,
     SHL = 386,
     SHR = 387,
     AND = 388,
     OR = 389,
     BITAND = 390,
     BITNAND = 391,
     BITOR = 392,
     BITXOR = 393,
     EQ = 394,
     NE = 395,
     LT = 396,
     LE = 397,
     GT = 398,
     GE = 399,
     COMPLEMENT = 400,
     F_HIGH = 401,
     F_LOW = 402,
     UMINUS = 403,
     UPLUS = 404,
     IDENTIFIER = 405,
     SUBST_MARK = 406,
     LABEL = 407,
     INCLUDE = 408,
     STRING = 409,
     NUMBER = 410
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 45 "AsmPicoBlazeParser.y"

    int number;                 //
    char * string;              //
    CompilerExpr * expr;        //
    CompilerStatement * stmt;   //
    struct
    {
        unsigned char * data;   //
        int size;               //
    } array;                    //



/* Line 2068 of yacc.c  */
#line 235 "AsmPicoBlazeParser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif



#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



