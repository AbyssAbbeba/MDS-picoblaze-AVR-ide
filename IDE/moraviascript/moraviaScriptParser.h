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
#line 63 "moraviaScriptParser.y"

    #include "MScriptParserInterface.h"



/* Line 2068 of yacc.c  */
#line 44 "moraviaScriptParser.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     END = 0,
     KW_IF = 258,
     KW_ELSE = 259,
     KW_FOR = 260,
     KW_WHILE = 261,
     KW_FUNC = 262,
     KW_DO = 263,
     KW_RETURN = 264,
     KW_BREAK = 265,
     KW_CONTINUE = 266,
     KW_SWITCH = 267,
     KW_CASE = 268,
     KW_DEFAULT = 269,
     KW_DELETE = 270,
     O_COLON = 271,
     O_QUESTION_MARK = 272,
     EOS = 273,
     B_RND_LEFT = 274,
     B_RND_RIGHT = 275,
     B_SQR_LEFT = 276,
     B_SQR_RIGHT = 277,
     B_CRL_LEFT = 278,
     B_CRL_RIGHT = 279,
     O_COMMA = 280,
     O_SLASH = 281,
     O_PLUS = 282,
     O_MINUS = 283,
     O_ASTERISK = 284,
     O_NOT = 285,
     O_MOD = 286,
     O_SHL = 287,
     O_SHR = 288,
     O_AND = 289,
     O_OR = 290,
     O_BITAND = 291,
     O_BITOR = 292,
     O_BITXOR = 293,
     O_EQ = 294,
     O_NE = 295,
     O_LT = 296,
     O_LE = 297,
     O_GT = 298,
     O_GE = 299,
     O_ASSIGN = 300,
     O_COMPLEMET = 301,
     O_ADD_ASSIGN = 302,
     O_SUB_ASSIGN = 303,
     O_MUL_ASSIGN = 304,
     O_DIV_ASSIGN = 305,
     O_MOD_ASSIGN = 306,
     O_SHL_ASSIGN = 307,
     O_SHR_ASSIGN = 308,
     O_AND_ASSIGN = 309,
     O_ORB_ASSIGN = 310,
     O_XOR_ASSIGN = 311,
     O_INCREMENT = 312,
     O_DECREMENT = 313,
     UMINUS = 314,
     UPLUS = 315,
     FCALL = 316,
     POST_DEC = 317,
     POST_INC = 318,
     IDENTIFIER = 319,
     STRING = 320,
     INTEGER = 321,
     REAL = 322
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 45 "moraviaScriptParser.y"

    int integer;                //
    float real;                 //
    char * string;              //
    MScriptExpr * expr;         //
    MScriptStatement * stmt;    //
    struct
    {
        unsigned char * data;   //
        int size;               //
    } array;                    //



/* Line 2068 of yacc.c  */
#line 144 "moraviaScriptParser.h"
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



