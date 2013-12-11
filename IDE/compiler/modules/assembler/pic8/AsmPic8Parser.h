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
#line 59 "AsmPic8Parser.y"

    #include "CompilerParserInterface.h"
    #ifndef YY_TYPEDEF_YY_SCANNER_T
            #define YY_TYPEDEF_YY_SCANNER_T
            typedef void* yyscan_t;
    #endif // YY_TYPEDEF_YY_SCANNER_T



/* Line 2068 of yacc.c  */
#line 48 "AsmPic8Parser.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     END = 0,
     D_DEFINE = 258,
     D_UNDEFINE = 259,
     D_CONSTANT = 260,
     D_ORG = 261,
     D_SET = 262,
     D_PROCESSOR = 263,
     D_RADIX = 264,
     D_VARIABLE = 265,
     D_WHILE = 266,
     D___BADRAM = 267,
     D___BADROM = 268,
     D___CONFIG = 269,
     D_CONFIG = 270,
     D___IDLOCS = 271,
     D___MAXRAM = 272,
     D___MAXROM = 273,
     D_CBLOCK = 274,
     D_DA = 275,
     D_DATA = 276,
     D_DE = 277,
     D_DT = 278,
     D_DW = 279,
     D_ENDC = 280,
     D_FILL = 281,
     D_RES = 282,
     D_ERROR = 283,
     D_ERRORLEVEL = 284,
     D_LIST = 285,
     D_MESSG = 286,
     D_NOLIST = 287,
     D_PAGE = 288,
     D_DB = 289,
     D_SUBTITLE = 290,
     D_TITLE = 291,
     D_EXPAND = 292,
     D_NOEXPAND = 293,
     D_ACCESS_OVR = 294,
     D_BANKISEL = 295,
     D_BANKSEL = 296,
     D_CODE = 297,
     D_CODE_PACK = 298,
     D_EXTERN = 299,
     D_GLOBAL = 300,
     D_IDATA = 301,
     D_IDATA_ACS = 302,
     D_PAGESEL = 303,
     D_PAGESELW = 304,
     D_UDATA = 305,
     D_UDATA_ACS = 306,
     D_UDATA_OVR = 307,
     D_UDATA_SHR = 308,
     D_IF = 309,
     D_IFN = 310,
     D_IFDEF = 311,
     D_IFNDEF = 312,
     D_ELSEIFB = 313,
     D_ELSEIFNB = 314,
     D_ELSE = 315,
     D_ELSEIF = 316,
     D_ELSEIFN = 317,
     D_ELSEIFDEF = 318,
     D_ELSEIFNDEF = 319,
     D_ENDIF = 320,
     D_LOCAL = 321,
     D_IFNB = 322,
     D_IFB = 323,
     D_SKIP = 324,
     D_ENDMACRO = 325,
     D_ENDM = 326,
     D_EXITM = 327,
     D_REPT = 328,
     D_MACRO = 329,
     D_EQU = 330,
     D_ENDW = 331,
     D_END = 332,
     D_DC_IFDEF = 333,
     D_DC_IFNDEF = 334,
     D_DC_ENDIF = 335,
     I_ADDWF = 336,
     I_ANDWF = 337,
     I_CLRF = 338,
     I_CLRW = 339,
     I_COMF = 340,
     I_DECF = 341,
     I_DECFSZ = 342,
     I_INCF = 343,
     I_INCFSZ = 344,
     I_IORWF = 345,
     I_MOVF = 346,
     I_MOVWF = 347,
     I_NOP = 348,
     I_RLF = 349,
     I_RRF = 350,
     I_SUBWF = 351,
     I_SWAPF = 352,
     I_XORWF = 353,
     I_BCF = 354,
     I_BSF = 355,
     I_BTFSC = 356,
     I_BTFSS = 357,
     I_ADDLW = 358,
     I_ANDLW = 359,
     I_CALL = 360,
     I_CLRWDT = 361,
     I_GOTO = 362,
     I_IORLW = 363,
     I_MOVLW = 364,
     I_RETFIE = 365,
     I_RETLW = 366,
     I_RETURN = 367,
     I_SLEEP = 368,
     I_SUBLW = 369,
     I_XORLW = 370,
     I_OPTION = 371,
     I_TRIS = 372,
     COMMENT = 373,
     EOL = 374,
     LP = 375,
     RP = 376,
     COMMA = 377,
     COLON = 378,
     INTERVAL = 379,
     ASSIGN = 380,
     PLUS = 381,
     MINUS = 382,
     ASTERISK = 383,
     SLASH = 384,
     NOT = 385,
     MOD = 386,
     SHL = 387,
     SHR = 388,
     AND = 389,
     OR = 390,
     XOR = 391,
     BITAND = 392,
     BITOR = 393,
     BITXOR = 394,
     EQ = 395,
     NE = 396,
     LT = 397,
     LE = 398,
     GT = 399,
     GE = 400,
     COMPLEMET = 401,
     ADD_ASSIGN = 402,
     SUB_ASSIGN = 403,
     MUL_ASSIGN = 404,
     DIV_ASSIGN = 405,
     MOD_ASSIGN = 406,
     SHL_ASSIGN = 407,
     SHR_ASSIGN = 408,
     AND_ASSIGN = 409,
     ORB_ASSIGN = 410,
     XOR_ASSIGN = 411,
     INCREMENT = 412,
     DECREMENT = 413,
     F_HIGH = 414,
     F_LOW = 415,
     F_BYTE2 = 416,
     F_BYTE3 = 417,
     F_BYTE4 = 418,
     F_LWRD = 419,
     F_HWRD = 420,
     F_PAGE = 421,
     F_EXP2 = 422,
     F_LOG2 = 423,
     F_V = 424,
     IDENTIFIER = 427,
     LABEL = 428,
     STRING = 429,
     INCLUDE = 430,
     NUMBER = 431
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 44 "AsmPic8Parser.y"

    int number;                     //
    char * string;                  //
    CompilerExpr * expr;            //
    CompilerStatement * stmt;       //
    struct {
        unsigned char * data;   //
        int size;               //
    } array;                        //



/* Line 2068 of yacc.c  */
#line 253 "AsmPic8Parser.h"
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



