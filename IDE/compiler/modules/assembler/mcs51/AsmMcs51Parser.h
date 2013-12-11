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
#line 62 "AsmMcs51Parser.y"

    #include "CompilerParserInterface.h"
    #ifndef YY_TYPEDEF_YY_SCANNER_T
            #define YY_TYPEDEF_YY_SCANNER_T
            typedef void* yyscan_t;
    #endif // YY_TYPEDEF_YY_SCANNER_T



/* Line 2068 of yacc.c  */
#line 48 "AsmMcs51Parser.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     END = 0,
     D_DB = 258,
     D_MACRO = 259,
     D_ENDM = 260,
     D_BIT = 261,
     D_BSEG = 262,
     D_BYTE = 263,
     D_CODE = 264,
     D_CSEG = 265,
     D_DATA = 266,
     D_DBIT = 267,
     D_DS = 268,
     D_DSEG = 269,
     D_DW = 270,
     D_ELSE = 271,
     D_ELSEIF = 272,
     D_ELSEIFDEF = 273,
     D_ELSEIFN = 274,
     D_ELSEIFNDEF = 275,
     D_END = 276,
     D_ENDIF = 277,
     D_EQU = 278,
     D_FLAG = 279,
     D_IDATA = 280,
     D_IF = 281,
     D_IFDEF = 282,
     D_IFN = 283,
     D_IFNDEF = 284,
     D_ISEG = 285,
     D_LIST = 286,
     D_NAME = 287,
     D_NOLIST = 288,
     D_ORG = 289,
     D_REPT = 290,
     D_SET = 291,
     D_SKIP = 292,
     D_USING = 293,
     D_XDATA = 294,
     D_XSEG = 295,
     D_IFB = 296,
     D_IFNB = 297,
     D_LOCAL = 298,
     D_ELSEIFNB = 299,
     D_EXITM = 300,
     D_ELSEIFB = 301,
     C_COND = 302,
     C_GO = 303,
     C_NODEBUG = 304,
     C_NOSYMBOLS = 305,
     C_RS = 306,
     C_CONDONLY = 307,
     C_IC = 308,
     C_NOGE = 309,
     C_NOTABS = 310,
     C_SA = 311,
     C_DA = 312,
     C_NOGEN = 313,
     C_NOXR = 314,
     C_SAVE = 315,
     C_RESTORE = 316,
     C_DATE = 317,
     C_LI = 318,
     C_NOLI = 319,
     C_NOXREF = 320,
     C_SB = 321,
     C_DB = 322,
     C_LIST = 323,
     C_NOLIST = 324,
     C_PAGELENGTH = 325,
     C_SYMBOLS = 326,
     C_DEBUG = 327,
     C_MACRO = 328,
     C_NOMACRO = 329,
     C_PAGEWIDTH = 330,
     C_TITLE = 331,
     C_EJ = 332,
     C_MO = 333,
     C_NOMO = 334,
     C_PAGING = 335,
     C_TT = 336,
     C_EJECT = 337,
     C_MOD51 = 338,
     C_NOMOD51 = 339,
     C_PHILIPS = 340,
     C_WARNING = 341,
     C_ERROR = 342,
     C_MR = 343,
     C_NOMR = 344,
     C_PI = 345,
     C_XR = 346,
     C_GE = 347,
     C_NOBUILTIN = 348,
     C_NOPAGING = 349,
     C_PL = 350,
     C_XREF = 351,
     C_GEN = 352,
     C_NOCOND = 353,
     C_NOPI = 354,
     C_PW = 355,
     C_GENONLY = 356,
     C_NODB = 357,
     C_NOSB = 358,
     I_ACALL = 359,
     I_ADD = 360,
     I_ADDC = 361,
     I_AJMP = 362,
     I_ANL = 363,
     I_CJNE = 364,
     I_CLR = 365,
     I_CPL = 366,
     I_DA = 367,
     I_DEC = 368,
     I_DIV = 369,
     I_DJNZ = 370,
     I_INC = 371,
     I_JB = 372,
     I_JBC = 373,
     I_JC = 374,
     I_JMP = 375,
     I_JNB = 376,
     I_JNC = 377,
     I_JNZ = 378,
     I_JZ = 379,
     I_LCALL = 380,
     I_LJMP = 381,
     I_MOV = 382,
     I_MOVC = 383,
     I_MOVX = 384,
     I_MUL = 385,
     I_NOP = 386,
     I_ORL = 387,
     I_POP = 388,
     I_PUSH = 389,
     I_RET = 390,
     I_RETI = 391,
     I_RL = 392,
     I_RLC = 393,
     I_RR = 394,
     I_RRC = 395,
     I_SETB = 396,
     I_SJMP = 397,
     I_SUBB = 398,
     I_SWAP = 399,
     I_XCH = 400,
     I_XCHD = 401,
     I_XRL = 402,
     IND_R0 = 403,
     IND_R1 = 404,
     IND_A_DPTR = 405,
     IND_A_PC = 406,
     IND_DPTR = 407,
     REG_A = 408,
     REG_AB = 409,
     BIT_C = 410,
     REG_DPTR = 411,
     REG_0 = 412,
     REG_1 = 413,
     REG_2 = 414,
     REG_3 = 415,
     REG_4 = 416,
     REG_5 = 417,
     REG_6 = 418,
     REG_7 = 419,
     COMMENT = 420,
     AT = 421,
     EOL = 422,
     IMMEDIATE = 423,
     LP = 424,
     RP = 425,
     COMMA = 426,
     HIGH = 427,
     LOW = 428,
     PLUS = 429,
     MINUS = 430,
     ASTERISK = 431,
     SLASH = 432,
     DOT = 433,
     NOT = 434,
     MOD = 435,
     SHL = 436,
     SHR = 437,
     AND = 438,
     OR = 439,
     XOR = 440,
     BITAND = 441,
     BITOR = 442,
     BITXOR = 443,
     EQ = 444,
     NE = 445,
     LT = 446,
     LE = 447,
     GT = 448,
     GE = 449,
     COMPLEMET = 450,
     IDENTIFIER = 452,
     LABEL = 453,
     STRING = 454,
     INCLUDE = 455,
     NUMBER = 456
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 45 "AsmMcs51Parser.y"

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
#line 280 "AsmMcs51Parser.h"
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



