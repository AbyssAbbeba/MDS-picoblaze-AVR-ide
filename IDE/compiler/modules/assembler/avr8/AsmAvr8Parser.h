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
#line 59 "AsmAvr8Parser.y"

    #include "CompilerParserInterface.h"
    #ifndef YY_TYPEDEF_YY_SCANNER_T
            #define YY_TYPEDEF_YY_SCANNER_T
            typedef void* yyscan_t;
    #endif // YY_TYPEDEF_YY_SCANNER_T



/* Line 2068 of yacc.c  */
#line 48 "AsmAvr8Parser.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     END = 0,
     D_IF = 258,
     D_IFN = 259,
     D_IFDEF = 260,
     D_IFNDEF = 261,
     D_ELSEIFB = 262,
     D_ELSEIFNB = 263,
     D_ELSE = 264,
     D_ELSEIF = 265,
     D_ELSEIFN = 266,
     D_ELSEIFDEF = 267,
     D_ELSEIFNDEF = 268,
     D_ENDIF = 269,
     D_LOCAL = 270,
     D_IFNB = 271,
     D_IFB = 272,
     D_SKIP = 273,
     D_ENDM = 274,
     D_EXITM = 275,
     D_REPT = 276,
     D_SET = 277,
     D_MACRO = 278,
     D_BYTE = 279,
     D_CSEG = 280,
     D_DB = 281,
     D_DEF = 282,
     D_DEVICE = 283,
     D_DSEG = 284,
     D_DW = 285,
     D_EQU = 286,
     D_ESEG = 287,
     D_EXIT = 288,
     D_LIST = 289,
     D_LISTMAC = 290,
     D_NOLIST = 291,
     D_ORG = 292,
     I_ADD = 293,
     I_ADC = 294,
     I_ADIW = 295,
     I_SUB = 296,
     I_SUBI = 297,
     I_SBC = 298,
     I_SBCI = 299,
     I_SBIW = 300,
     I_AND = 301,
     I_ANDI = 302,
     I_OR = 303,
     I_ORI = 304,
     I_EOR = 305,
     I_COM = 306,
     I_NEG = 307,
     I_SBR = 308,
     I_CBR = 309,
     I_INC = 310,
     I_DEC = 311,
     I_TST = 312,
     I_CLR = 313,
     I_SER = 314,
     I_MUL = 315,
     I_RJMP = 316,
     I_IJMP = 317,
     I_JMP = 318,
     I_RCALL = 319,
     I_ICALL = 320,
     I_CALL = 321,
     I_RET = 322,
     I_RETI = 323,
     I_CPSE = 324,
     I_CP = 325,
     I_CPC = 326,
     I_CPI = 327,
     I_SBRC = 328,
     I_SBRS = 329,
     I_SBIC = 330,
     I_SBIS = 331,
     I_BRBS = 332,
     I_BRBC = 333,
     I_BREQ = 334,
     I_BRNE = 335,
     I_BRCS = 336,
     I_BRCC = 337,
     I_BRSH = 338,
     I_BRLO = 339,
     I_BRMI = 340,
     I_BRPL = 341,
     I_BRGE = 342,
     I_BRLT = 343,
     I_BRHS = 344,
     I_BRHC = 345,
     I_BRTS = 346,
     I_BRTC = 347,
     I_BRVS = 348,
     I_BRVC = 349,
     I_BRIE = 350,
     I_BRID = 351,
     I_MOV = 352,
     I_LDI = 353,
     I_LDS = 354,
     I_LD = 355,
     I_STS = 356,
     I_ST = 357,
     I_STD = 358,
     I_LPM = 359,
     I_IN = 360,
     I_OUT = 361,
     I_PUSH = 362,
     I_POP = 363,
     I_LSL = 364,
     I_LSR = 365,
     I_ROL = 366,
     I_ROR = 367,
     I_ASR = 368,
     I_SWAP = 369,
     I_BSET = 370,
     I_BCLR = 371,
     I_SBI = 372,
     I_CBI = 373,
     I_BST = 374,
     I_BLD = 375,
     I_SEC = 376,
     I_CLC = 377,
     I_SEN = 378,
     I_CLN = 379,
     I_SEZ = 380,
     I_CLZ = 381,
     I_SEI = 382,
     I_CLI = 383,
     I_SES = 384,
     I_CLS = 385,
     I_SEV = 386,
     I_CLV = 387,
     I_SET = 388,
     I_CLT = 389,
     I_SEH = 390,
     I_CLH = 391,
     I_NOP = 392,
     I_SLEEP = 393,
     I_WDR = 394,
     I_LDD = 395,
     X = 396,
     Y = 397,
     Z = 398,
     COMMENT = 399,
     AT_SIGN = 400,
     AT = 401,
     EOL = 402,
     LP = 403,
     RP = 404,
     COMMA = 405,
     ASSIGN = 406,
     PLUS = 407,
     MINUS = 408,
     ASTERISK = 409,
     SLASH = 410,
     NOT = 411,
     MOD = 412,
     SHL = 413,
     SHR = 414,
     AND = 415,
     OR = 416,
     XOR = 417,
     BITAND = 418,
     BITOR = 419,
     BITXOR = 420,
     EQ = 421,
     NE = 422,
     LT = 423,
     LE = 424,
     GT = 425,
     GE = 426,
     COMPLEMET = 427,
     ADD_ASSIGN = 428,
     SUB_ASSIGN = 429,
     MUL_ASSIGN = 430,
     DIV_ASSIGN = 431,
     MOD_ASSIGN = 432,
     SHL_ASSIGN = 433,
     SHR_ASSIGN = 434,
     AND_ASSIGN = 435,
     ORB_ASSIGN = 436,
     XOR_ASSIGN = 437,
     INCREMENT = 438,
     DECREMENT = 439,
     F_HIGH = 440,
     F_LOW = 441,
     F_BYTE2 = 442,
     F_BYTE3 = 443,
     F_BYTE4 = 444,
     F_LWRD = 445,
     F_HWRD = 446,
     F_PAGE = 447,
     F_EXP2 = 448,
     F_LOG2 = 449,
     IDENTIFIER = 450,
     LABEL = 451,
     STRING = 452,
     INCLUDE = 453,
     NUMBER = 454
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 44 "AsmAvr8Parser.y"

    int number;                     //
    char * string;                  //
    CompilerExpr * expr;            //
    CompilerStatement * stmt;       //
    struct {
        unsigned char * data;   //
        int size;               //
    } array;                        //



/* Line 2068 of yacc.c  */
#line 278 "AsmAvr8Parser.h"
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



