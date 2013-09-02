/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse         moraviaScriptParser_parse
#define yylex           moraviaScriptParser_lex
#define yyerror         moraviaScriptParser_error
#define yylval          moraviaScriptParser_lval
#define yychar          moraviaScriptParser_char
#define yydebug         moraviaScriptParser_debug
#define yynerrs         moraviaScriptParser_nerrs
#define yylloc          moraviaScriptParser_lloc

/* Copy the first part of user declarations.  */


/* Line 268 of yacc.c  */
#line 79 "moraviaScriptParser.cxx"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* "%code requires" blocks.  */

/* Line 288 of yacc.c  */
#line 63 "moraviaScriptParser.y"

    #include "MScriptParserInterface.h"



/* Line 288 of yacc.c  */
#line 109 "moraviaScriptParser.cxx"

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

/* Line 293 of yacc.c  */
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



/* Line 293 of yacc.c  */
#line 209 "moraviaScriptParser.cxx"
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


/* Copy the second part of user declarations.  */

/* Line 343 of yacc.c  */
#line 67 "moraviaScriptParser.y"

    #include <QObject>                // For i18n, nothing else.
    #include <cstdlib>                // We need free() here.
    #include <cstring>                // We need strncmp() here.
    #include <iostream>               // This is required by Bison.
    using namespace std;              // This is required by Bison as well.
    using namespace MScriptStmtTypes; // This NS is heavily used here.

    // Declaration of the lexer prototypes and other things required by Bison.
    #include "moraviaScriptLexer.h"

    // Name of the lexer func required by Bison.
    #define moraviaScriptParser_lex moraviaScriptLexer_lex

    // Declaration of the error reporting function used by Bison.
    inline int moraviaScriptParser_error ( YYLTYPE * yylloc,
                                           yyscan_t yyscanner,
                                           MScriptParserInterface * core,
                                           const char * errorInfo );


/* Line 343 of yacc.c  */
#line 256 "moraviaScriptParser.cxx"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
# define YYCOPY_NEEDED 1
#endif


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  52
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1588

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  10
/* YYNRULES -- Number of rules.  */
#define YYNRULES  84
/* YYNRULES -- Number of states.  */
#define YYNSTATES  188

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   322

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    11,    13,    15,    18,
      22,    26,    30,    34,    39,    46,    50,    53,    59,    67,
      77,    83,    91,    99,   106,   109,   113,   117,   120,   125,
     129,   131,   132,   134,   138,   141,   146,   147,   149,   151,
     153,   155,   157,   161,   165,   169,   173,   177,   181,   185,
     189,   193,   197,   201,   205,   209,   213,   217,   221,   225,
     229,   233,   237,   241,   245,   249,   253,   257,   261,   265,
     269,   273,   277,   280,   283,   286,   289,   292,   295,   298,
     301,   307,   311,   316,   320
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      69,     0,    -1,    70,    -1,    -1,    71,    -1,    70,    71,
      -1,    18,    -1,     1,    -1,    77,    18,    -1,     9,    76,
      18,    -1,    11,    76,    18,    -1,    10,    76,    18,    -1,
      15,    74,    18,    -1,    26,    77,    26,    71,    -1,     7,
      74,    19,    75,    20,    71,    -1,    23,    70,    24,    -1,
      23,    24,    -1,     3,    19,    77,    20,    71,    -1,     3,
      19,    77,    20,    71,     4,    71,    -1,     5,    19,    76,
      18,    76,    18,    76,    20,    71,    -1,     6,    19,    77,
      20,    71,    -1,     8,    71,     6,    19,    77,    20,    18,
      -1,    12,    19,    77,    20,    23,    72,    24,    -1,    12,
      19,    77,    20,    23,    24,    -1,    73,    70,    -1,    72,
      73,    70,    -1,    13,    77,    16,    -1,    14,    16,    -1,
      73,    13,    77,    16,    -1,    73,    14,    16,    -1,    64,
      -1,    -1,    74,    -1,    75,    25,    74,    -1,    36,    74,
      -1,    75,    25,    36,    74,    -1,    -1,    77,    -1,    74,
      -1,    66,    -1,    67,    -1,    65,    -1,    19,    77,    20,
      -1,    77,    27,    77,    -1,    77,    28,    77,    -1,    77,
      29,    77,    -1,    77,    26,    77,    -1,    77,    31,    77,
      -1,    77,    37,    77,    -1,    77,    38,    77,    -1,    77,
      36,    77,    -1,    77,    35,    77,    -1,    77,    34,    77,
      -1,    77,    39,    77,    -1,    77,    40,    77,    -1,    77,
      41,    77,    -1,    77,    42,    77,    -1,    77,    44,    77,
      -1,    77,    43,    77,    -1,    77,    33,    77,    -1,    77,
      32,    77,    -1,    77,    45,    77,    -1,    77,    47,    77,
      -1,    77,    48,    77,    -1,    77,    49,    77,    -1,    77,
      50,    77,    -1,    77,    51,    77,    -1,    77,    52,    77,
      -1,    77,    53,    77,    -1,    77,    54,    77,    -1,    77,
      55,    77,    -1,    77,    56,    77,    -1,    46,    77,    -1,
      30,    77,    -1,    27,    77,    -1,    28,    77,    -1,    57,
      77,    -1,    58,    77,    -1,    77,    57,    -1,    77,    58,
      -1,    77,    17,    77,    16,    77,    -1,    77,    25,    77,
      -1,    74,    21,    77,    22,    -1,    74,    19,    20,    -1,
      74,    19,    77,    20,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   251,   251,   255,   263,   264,   269,   272,   275,   278,
     281,   284,   287,   290,   293,   298,   302,   309,   316,   327,
     332,   335,   339,   344,   356,   357,   362,   363,   364,   365,
     373,   378,   379,   380,   381,   382,   387,   388,   394,   395,
     396,   397,   400,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   434,   435,   436,   437,   438,   439,   440,   441,
     444,   450,   453,   456,   457
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "$undefined", "\"if\"", "\"else\"",
  "\"for\"", "\"while\"", "\"func\"", "\"do\"", "\"return\"", "\"break\"",
  "\"continue\"", "\"switch\"", "\"case\"", "\"default\"", "\"delete\"",
  "\":\"", "\"?\"", "\";\"", "\"(\"", "\")\"", "\"[\"", "\"]\"", "\"{\"",
  "\"}\"", "\",\"", "\"/\"", "\"+\"", "\"-\"", "\"*\"", "\"!\"", "\"%\"",
  "\"<<\"", "\">>\"", "\"&&\"", "\"||\"", "\"&\"", "\"|\"", "\"^\"",
  "\"==\"", "\"!=\"", "\"<\"", "\"<=\"", "\">\"", "\">=\"", "\"=\"",
  "\"~\"", "\"+=\"", "\"-=\"", "\"*=\"", "\"/=\"", "\"%=\"", "\"<<=\"",
  "\">>=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"++\"", "\"--\"", "UMINUS",
  "UPLUS", "FCALL", "POST_DEC", "POST_INC", "\"idenfifier\"", "\"string\"",
  "\"integer\"", "\"real number\"", "$accept", "input", "statements",
  "stmt", "switch_body", "cases", "id", "param_list", "e_expr", "expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    68,    69,    69,    70,    70,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    72,    72,    73,    73,    73,    73,
      74,    75,    75,    75,    75,    75,    76,    76,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     2,     1,     1,     2,     3,
       3,     3,     3,     4,     6,     3,     2,     5,     7,     9,
       5,     7,     7,     6,     2,     3,     3,     2,     4,     3,
       1,     0,     1,     3,     2,     4,     0,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     2,     2,     2,     2,     2,
       5,     3,     4,     3,     4
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     7,     0,     0,     0,     0,     0,    36,    36,    36,
       0,     0,     6,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    30,    41,    39,    40,     0,     0,     4,    38,
       0,     0,    36,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,    16,     0,     0,    74,    75,    73,    72,
      76,    77,     1,     5,     0,     0,     0,     8,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    78,    79,
       0,     0,     0,    31,     0,     9,    11,    10,     0,    12,
      42,    15,     0,    83,     0,     0,     0,    81,    46,    43,
      44,    45,    47,    60,    59,    52,    51,    50,    48,    49,
      53,    54,    55,    56,    58,    57,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,     0,    36,     0,
       0,    32,     0,     0,     0,    13,    46,    84,    82,     0,
      17,     0,    20,    34,     0,     0,     0,     0,    80,     0,
      36,    14,     0,    33,     0,     0,     0,    23,     0,     0,
      18,     0,    35,    21,     0,    27,    22,     0,     0,     0,
       0,     0,    26,     0,     0,    29,    19,    28
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    26,    27,    28,   168,   169,    29,   142,    36,    30
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -52
static const yytype_int16 yypact[] =
{
     204,   -52,     1,    26,    29,   -51,   605,   622,   622,   622,
      34,   -51,   -52,   622,   505,   622,   622,   622,   622,   622,
     622,   622,   -52,   -52,   -52,   -52,    71,   237,   -52,    10,
     793,   622,   622,   622,    53,    73,    59,  1129,    62,    63,
     622,    64,   835,   -52,   572,  1171,    12,    12,    12,    12,
      12,    12,   -52,   -52,    27,   622,   622,   -52,   622,   622,
     622,   622,   622,   622,   622,   622,   622,   622,   622,   622,
     622,   622,   622,   622,   622,   622,   622,   622,   622,   622,
     622,   622,   622,   622,   622,   622,   622,   622,   -52,   -52,
     877,    68,   919,   -32,    69,   -52,   -52,   -52,   961,   -52,
     -52,   -52,   605,   -52,  1003,  1045,   665,  1213,    12,   131,
     131,    12,    12,     8,     8,  1449,  1426,  1492,  1472,   -17,
    1511,  1511,   139,   139,  1530,  1530,  1360,  1327,  1327,  1292,
    1292,  1292,  1255,  1255,  1213,  1213,  1213,   605,   622,   605,
     -51,   -52,    24,   622,    66,   -52,   -15,   -52,   -52,   622,
      83,    77,   -52,   -52,   605,   -31,  1087,     4,  1393,   605,
     622,   -52,   -51,   -52,    82,   622,   116,   -52,    54,   438,
     -52,   114,   -52,   -52,   708,   -52,   -52,   438,   622,   119,
     304,   605,   -52,   371,   751,   -52,   -52,   -52
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -52,   -52,   -13,    -6,   -52,   -30,    -3,   -52,    -2,    43
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -26
static const yytype_int16 yytable[] =
{
      35,    44,    34,    57,   140,   162,    38,    39,    41,    59,
      60,    61,    62,    22,    63,    64,    65,   165,   166,    68,
      31,    53,    71,    72,    73,    74,    75,    76,   167,    54,
      91,    55,    22,    22,    59,    60,    61,    62,    53,    63,
      88,    89,    88,    89,   154,    32,    13,   103,    33,   155,
      37,    37,    37,    40,    16,    17,    42,    18,    45,    46,
      47,    48,    49,    50,    51,    88,    89,   165,   166,    88,
      89,    52,    93,    19,    90,    37,    92,    95,   176,    94,
      96,    97,    99,    98,    20,    21,   138,   159,   143,   157,
     141,    22,    23,    24,    25,   160,   145,   104,   105,   106,
     173,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   150,   175,   152,   181,   185,   151,   153,   177,     0,
       0,     0,     0,     0,     0,   146,     0,     0,   161,     0,
       0,     0,   163,   170,     0,     0,   180,    59,   171,   172,
      62,     0,    63,     0,   183,    59,    60,    61,    62,     0,
      63,    64,    65,     0,    53,   186,     0,    53,     0,     0,
       0,    37,     0,     0,     0,     0,   156,     0,    88,    89,
       0,     0,   158,     0,     0,     0,    88,    89,     0,     0,
       0,     0,     0,    37,    -3,     1,     0,     2,   174,     3,
       4,     5,     6,     7,     8,     9,    10,     0,     0,    11,
       0,   184,    12,    13,     0,     0,     0,    14,     0,     0,
      15,    16,    17,     0,    18,     0,     0,    -2,     1,     0,
       2,     0,     3,     4,     5,     6,     7,     8,     9,    10,
      19,     0,    11,     0,     0,    12,    13,     0,     0,     0,
      14,    20,    21,    15,    16,    17,     0,    18,    22,    23,
      24,    25,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    20,    21,     0,     0,     0,     0,
       0,    22,    23,    24,    25,     1,     0,     2,     0,     3,
       4,     5,     6,     7,     8,     9,    10,   -24,   -24,    11,
       0,     0,    12,    13,     0,     0,     0,    14,   -24,     0,
      15,    16,    17,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    20,    21,     0,     0,     0,     0,     0,    22,    23,
      24,    25,     1,     0,     2,     0,     3,     4,     5,     6,
       7,     8,     9,    10,   -25,   -25,    11,     0,     0,    12,
      13,     0,     0,     0,    14,   -25,     0,    15,    16,    17,
       0,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    20,    21,
       0,     0,     0,     0,     0,    22,    23,    24,    25,     1,
       0,     2,     0,     3,     4,     5,     6,     7,     8,     9,
      10,   178,   179,    11,     0,     0,    12,    13,     0,     0,
       0,    14,     0,     0,    15,    16,    17,     0,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    20,    21,     0,     0,     0,
       0,     0,    22,    23,    24,    25,     1,     0,     2,     0,
       3,     4,     5,     6,     7,     8,     9,    10,     0,     0,
      11,     0,     0,    12,    13,     0,     0,     0,    14,    43,
       0,    15,    16,    17,     0,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,    21,     0,     0,     0,     0,     0,    22,
      23,    24,    25,     1,     0,     2,     0,     3,     4,     5,
       6,     7,     8,     9,    10,     0,     0,    11,     0,     0,
      12,    13,     0,     0,     0,    14,   101,     0,    15,    16,
      17,     0,    18,     0,     0,     0,     1,     0,     2,     0,
       3,     4,     5,     6,     7,     8,     9,    10,    19,     0,
      11,     0,     0,    12,    13,     0,     0,     0,    14,    20,
      21,    15,    16,    17,     0,    18,    22,    23,    24,    25,
       0,    13,     0,     0,     0,     0,     0,     0,     0,    16,
      17,    19,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    20,    21,     0,     0,     0,     0,    19,    22,
      23,    24,    25,     0,     0,     0,     0,     0,     0,    20,
      21,   149,    56,     0,     0,     0,    22,    23,    24,    25,
      58,    59,    60,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,   182,    56,     0,     0,     0,     0,
       0,     0,     0,    58,    59,    60,    61,    62,     0,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,   187,    56,     0,
       0,     0,     0,     0,     0,     0,    58,    59,    60,    61,
      62,     0,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      56,    57,     0,     0,     0,     0,     0,     0,    58,    59,
      60,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    56,     0,     0,   100,     0,     0,     0,     0,
      58,    59,    60,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    56,     0,     0,   137,     0,     0,
       0,     0,    58,    59,    60,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    56,     0,     0,   139,
       0,     0,     0,     0,    58,    59,    60,    61,    62,     0,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    56,     0,
       0,   144,     0,     0,     0,     0,    58,    59,    60,    61,
      62,     0,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      56,     0,     0,   147,     0,     0,     0,     0,    58,    59,
      60,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    56,     0,     0,     0,     0,   148,     0,     0,
      58,    59,    60,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    56,     0,     0,   164,     0,     0,
       0,     0,    58,    59,    60,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    56,     0,     0,     0,
       0,     0,     0,     0,    58,    59,    60,    61,    62,     0,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    56,     0,
       0,     0,     0,     0,     0,     0,    58,   102,    60,    61,
      62,     0,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      60,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    56,     0,     0,     0,     0,     0,     0,     0,
       0,    59,    60,    61,    62,     0,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,    80,    81,    82,    83,    84,    56,
       0,     0,    88,    89,     0,     0,     0,     0,    59,    60,
      61,    62,     0,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,    80,    81,    82,    56,     0,     0,     0,     0,    88,
      89,     0,     0,    59,    60,    61,    62,     0,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    56,     0,     0,
       0,     0,     0,     0,    88,    89,    59,    60,    61,    62,
       0,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,    88,    89,    59,
      60,    61,    62,     0,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      88,    89,    59,    60,    61,    62,     0,    63,    64,    65,
      66,     0,    68,    69,    70,    71,    72,    73,    74,    75,
      76,     0,     0,     0,     0,    59,    60,    61,    62,     0,
      63,    64,    65,    88,    89,    68,    69,    70,    71,    72,
      73,    74,    75,    76,     0,     0,     0,     0,    59,    60,
      61,    62,     0,    63,    64,    65,    88,    89,    68,     0,
      70,    71,    72,    73,    74,    75,    76,     0,    59,    60,
      61,    62,     0,    63,    64,    65,     0,     0,     0,    88,
      89,    71,    72,    73,    74,    75,    76,    59,    60,    61,
      62,     0,    63,    64,    65,     0,     0,     0,     0,    88,
      89,     0,    73,    74,    75,    76,    59,    60,    61,    62,
       0,    63,    64,    65,     0,     0,     0,     0,    88,    89,
       0,    73,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    88,    89
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-52))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       6,    14,     5,    18,    36,    36,     8,     9,    11,    26,
      27,    28,    29,    64,    31,    32,    33,    13,    14,    36,
      19,    27,    39,    40,    41,    42,    43,    44,    24,    19,
      32,    21,    64,    64,    26,    27,    28,    29,    44,    31,
      57,    58,    57,    58,    20,    19,    19,    20,    19,    25,
       7,     8,     9,    19,    27,    28,    13,    30,    15,    16,
      17,    18,    19,    20,    21,    57,    58,    13,    14,    57,
      58,     0,    19,    46,    31,    32,    33,    18,    24,     6,
      18,    18,    18,    40,    57,    58,    18,     4,    19,    23,
      93,    64,    65,    66,    67,    18,   102,    54,    55,    56,
      18,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,   137,    16,   139,    20,    16,   138,   140,   168,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,   154,    -1,
      -1,    -1,   155,   159,    -1,    -1,   169,    26,   160,   162,
      29,    -1,    31,    -1,   177,    26,    27,    28,    29,    -1,
      31,    32,    33,    -1,   180,   181,    -1,   183,    -1,    -1,
      -1,   138,    -1,    -1,    -1,    -1,   143,    -1,    57,    58,
      -1,    -1,   149,    -1,    -1,    -1,    57,    58,    -1,    -1,
      -1,    -1,    -1,   160,     0,     1,    -1,     3,   165,     5,
       6,     7,     8,     9,    10,    11,    12,    -1,    -1,    15,
      -1,   178,    18,    19,    -1,    -1,    -1,    23,    -1,    -1,
      26,    27,    28,    -1,    30,    -1,    -1,     0,     1,    -1,
       3,    -1,     5,     6,     7,     8,     9,    10,    11,    12,
      46,    -1,    15,    -1,    -1,    18,    19,    -1,    -1,    -1,
      23,    57,    58,    26,    27,    28,    -1,    30,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    64,    65,    66,    67,     1,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      -1,    -1,    18,    19,    -1,    -1,    -1,    23,    24,    -1,
      26,    27,    28,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    64,    65,
      66,    67,     1,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    -1,    -1,    18,
      19,    -1,    -1,    -1,    23,    24,    -1,    26,    27,    28,
      -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      -1,    -1,    -1,    -1,    -1,    64,    65,    66,    67,     1,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    -1,    -1,    18,    19,    -1,    -1,
      -1,    23,    -1,    -1,    26,    27,    28,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    -1,    -1,    -1,
      -1,    -1,    64,    65,    66,    67,     1,    -1,     3,    -1,
       5,     6,     7,     8,     9,    10,    11,    12,    -1,    -1,
      15,    -1,    -1,    18,    19,    -1,    -1,    -1,    23,    24,
      -1,    26,    27,    28,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    -1,    -1,    -1,    -1,    -1,    64,
      65,    66,    67,     1,    -1,     3,    -1,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    -1,    15,    -1,    -1,
      18,    19,    -1,    -1,    -1,    23,    24,    -1,    26,    27,
      28,    -1,    30,    -1,    -1,    -1,     1,    -1,     3,    -1,
       5,     6,     7,     8,     9,    10,    11,    12,    46,    -1,
      15,    -1,    -1,    18,    19,    -1,    -1,    -1,    23,    57,
      58,    26,    27,    28,    -1,    30,    64,    65,    66,    67,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,
      28,    46,    30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    -1,    -1,    -1,    -1,    46,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    16,    17,    -1,    -1,    -1,    64,    65,    66,    67,
      25,    26,    27,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    -1,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      17,    18,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    17,    -1,    -1,    20,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    17,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    17,    -1,    -1,    20,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    17,    -1,
      -1,    20,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      17,    -1,    -1,    20,    -1,    -1,    -1,    -1,    25,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    17,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      25,    26,    27,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    17,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    -1,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    -1,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    17,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    27,    28,    29,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    47,    48,    49,    50,    51,    52,    53,    17,
      -1,    -1,    57,    58,    -1,    -1,    -1,    -1,    26,    27,
      28,    29,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    -1,    47,
      48,    49,    50,    51,    17,    -1,    -1,    -1,    -1,    57,
      58,    -1,    -1,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    -1,    47,    48,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    26,    27,    28,    29,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    26,
      27,    28,    29,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    26,    27,    28,    29,    -1,    31,    32,    33,
      34,    -1,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    -1,    26,    27,    28,    29,    -1,
      31,    32,    33,    57,    58,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    -1,    -1,    -1,    26,    27,
      28,    29,    -1,    31,    32,    33,    57,    58,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    -1,    26,    27,
      28,    29,    -1,    31,    32,    33,    -1,    -1,    -1,    57,
      58,    39,    40,    41,    42,    43,    44,    26,    27,    28,
      29,    -1,    31,    32,    33,    -1,    -1,    -1,    -1,    57,
      58,    -1,    41,    42,    43,    44,    26,    27,    28,    29,
      -1,    31,    32,    33,    -1,    -1,    -1,    -1,    57,    58,
      -1,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     5,     6,     7,     8,     9,    10,    11,
      12,    15,    18,    19,    23,    26,    27,    28,    30,    46,
      57,    58,    64,    65,    66,    67,    69,    70,    71,    74,
      77,    19,    19,    19,    74,    71,    76,    77,    76,    76,
      19,    74,    77,    24,    70,    77,    77,    77,    77,    77,
      77,    77,     0,    71,    19,    21,    17,    18,    25,    26,
      27,    28,    29,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      77,    76,    77,    19,     6,    18,    18,    18,    77,    18,
      20,    24,    26,    20,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    20,    18,    20,
      36,    74,    75,    19,    20,    71,    77,    20,    22,    16,
      71,    76,    71,    74,    20,    25,    77,    23,    77,     4,
      18,    71,    36,    74,    20,    13,    14,    24,    72,    73,
      71,    76,    74,    18,    77,    16,    24,    73,    13,    14,
      70,    20,    16,    70,    77,    16,    71,    16
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      YY_LAC_DISCARD ("YYBACKUP");				\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (&yylloc, yyscanner, core, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, yyscanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, yyscanner, core); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t yyscanner, MScriptParserInterface * core)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, yyscanner, core)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    yyscan_t yyscanner;
    MScriptParserInterface * core;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (yyscanner);
  YYUSE (core);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t yyscanner, MScriptParserInterface * core)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, yyscanner, core)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    yyscan_t yyscanner;
    MScriptParserInterface * core;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, yyscanner, core);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, yyscan_t yyscanner, MScriptParserInterface * core)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, yyscanner, core)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    yyscan_t yyscanner;
    MScriptParserInterface * core;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , yyscanner, core);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, yyscanner, core); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

/* Given a state stack such that *YYBOTTOM is its bottom, such that
   *YYTOP is either its top or is YYTOP_EMPTY to indicate an empty
   stack, and such that *YYCAPACITY is the maximum number of elements it
   can hold without a reallocation, make sure there is enough room to
   store YYADD more elements.  If not, allocate a new stack using
   YYSTACK_ALLOC, copy the existing elements, and adjust *YYBOTTOM,
   *YYTOP, and *YYCAPACITY to reflect the new capacity and memory
   location.  If *YYBOTTOM != YYBOTTOM_NO_FREE, then free the old stack
   using YYSTACK_FREE.  Return 0 if successful or if no reallocation is
   required.  Return 1 if memory is exhausted.  */
static int
yy_lac_stack_realloc (YYSIZE_T *yycapacity, YYSIZE_T yyadd,
#if YYDEBUG
                      char const *yydebug_prefix,
                      char const *yydebug_suffix,
#endif
                      yytype_int16 **yybottom,
                      yytype_int16 *yybottom_no_free,
                      yytype_int16 **yytop, yytype_int16 *yytop_empty)
{
  YYSIZE_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYSIZE_T yysize_new = yysize_old + yyadd;
  if (*yycapacity < yysize_new)
    {
      YYSIZE_T yyalloc = 2 * yysize_new;
      yytype_int16 *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      if (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_prefix,
                      yydebug_suffix));
          return 1;
        }
      if (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        (yytype_int16*) YYSTACK_ALLOC (yyalloc * sizeof *yybottom_new);
      if (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_prefix,
                      yydebug_suffix));
          return 1;
        }
      if (*yytop != yytop_empty)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      if (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  return 0;
}

/* Establish the initial context for the current lookahead if no initial
   context is currently established.

   We define a context as a snapshot of the parser stacks.  We define
   the initial context for a lookahead as the context in which the
   parser initially examines that lookahead in order to select a
   syntactic action.  Thus, if the lookahead eventually proves
   syntactically unacceptable (possibly in a later context reached via a
   series of reductions), the initial context can be used to determine
   the exact set of tokens that would be syntactically acceptable in the
   lookahead's place.  Moreover, it is the context after which any
   further semantic actions would be erroneous because they would be
   determined by a syntactically unacceptable token.

   YY_LAC_ESTABLISH should be invoked when a reduction is about to be
   performed in an inconsistent state (which, for the purposes of LAC,
   includes consistent states that don't know they're consistent because
   their default reductions have been disabled).  Iff there is a
   lookahead token, it should also be invoked before reporting a syntax
   error.  This latter case is for the sake of the debugging output.

   For parse.lac=full, the implementation of YY_LAC_ESTABLISH is as
   follows.  If no initial context is currently established for the
   current lookahead, then check if that lookahead can eventually be
   shifted if syntactic actions continue from the current context.
   Report a syntax error if it cannot.  */
#define YY_LAC_ESTABLISH                                         \
do {                                                             \
  if (!yy_lac_established)                                       \
    {                                                            \
      YYDPRINTF ((stderr,                                        \
                  "LAC: initial context established for %s\n",   \
                  yytname[yytoken]));                            \
      yy_lac_established = 1;                                    \
      {                                                          \
        int yy_lac_status =                                      \
          yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken); \
        if (yy_lac_status == 2)                                  \
          goto yyexhaustedlab;                                   \
        if (yy_lac_status == 1)                                  \
          goto yyerrlab;                                         \
      }                                                          \
    }                                                            \
} while (YYID (0))

/* Discard any previous initial lookahead context because of Event,
   which may be a lookahead change or an invalidation of the currently
   established initial context for the current lookahead.

   The most common example of a lookahead change is a shift.  An example
   of both cases is syntax error recovery.  That is, a syntax error
   occurs when the lookahead is syntactically erroneous for the
   currently established initial context, so error recovery manipulates
   the parser stacks to try to find a new initial context in which the
   current lookahead is syntactically acceptable.  If it fails to find
   such a context, it discards the lookahead.  */
#if YYDEBUG
# define YY_LAC_DISCARD(Event)                                           \
do {                                                                     \
  if (yy_lac_established)                                                \
    {                                                                    \
      if (yydebug)                                                       \
        YYFPRINTF (stderr, "LAC: initial context discarded due to "      \
                   Event "\n");                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} while (YYID (0))
#else
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#endif

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return 2 if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
static int
yy_lac (yytype_int16 *yyesa, yytype_int16 **yyes,
        YYSIZE_T *yyes_capacity, yytype_int16 *yyssp, int yytoken)
{
  yytype_int16 *yyes_prev = yyssp;
  yytype_int16 *yyesp = yyes_prev;
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yytname[yytoken]));
  if (yytoken == YYUNDEFTOK)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      return 1;
    }
  while (1)
    {
      int yyrule = yypact[*yyesp];
      if (yypact_value_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          yyrule = yydefact[*yyesp];
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
        }
      else
        {
          yyrule = yytable[yyrule];
          if (yytable_value_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
          if (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              return 0;
            }
          yyrule = -yyrule;
        }
      {
        YYSIZE_T yylen = yyr2[yyrule];
        YYDPRINTF ((stderr, " R%d", yyrule - 1));
        if (yyesp != yyes_prev)
          {
            YYSIZE_T yysize = yyesp - *yyes + 1;
            if (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            else
              {
                yylen -= yysize;
                yyesp = yyes_prev;
              }
          }
        if (yylen)
          yyesp = yyes_prev -= yylen;
      }
      {
        int yystate;
        {
          int yylhs = yyr1[yyrule] - YYNTOKENS;
          yystate = yypgoto[yylhs] + *yyesp;
          if (yystate < 0 || YYLAST < yystate
              || yycheck[yystate] != *yyesp)
            yystate = yydefgoto[yylhs];
          else
            yystate = yytable[yystate];
        }
        if (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            *yyesp = yystate;
          }
        else
          {
            if (yy_lac_stack_realloc (yyes_capacity, 1,
#if YYDEBUG
                                      " (", ")",
#endif
                                      yyes, yyesa, &yyesp, yyes_prev))
              {
                YYDPRINTF ((stderr, "\n"));
                return 2;
              }
            *++yyesp = yystate;
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.  In order to see if a particular token T is a
   valid looakhead, invoke yy_lac (YYESA, YYES, YYES_CAPACITY, YYSSP, T).

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store or if
   yy_lac returned 2.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyesa, yytype_int16 **yyes,
                YYSIZE_T *yyes_capacity, yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
       In the first two cases, it might appear that the current syntax
       error should have been detected in the previous state when yy_lac
       was invoked.  However, at that time, there might have been a
       different syntax error that discarded a different initial context
       during error recovery, leaving behind the current lookahead.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      YYDPRINTF ((stderr, "Constructing syntax error message\n"));
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          int yyx;

          for (yyx = 0; yyx < YYNTOKENS; ++yyx)
            if (yyx != YYTERROR && yyx != YYUNDEFTOK)
              {
                {
                  int yy_lac_status = yy_lac (yyesa, yyes, yyes_capacity,
                                              yyssp, yyx);
                  if (yy_lac_status == 2)
                    return 2;
                  if (yy_lac_status == 1)
                    continue;
                }
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
# if YYDEBUG
      else if (yydebug)
        YYFPRINTF (stderr, "No expected tokens.\n");
# endif
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t yyscanner, MScriptParserInterface * core)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, yyscanner, core)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    yyscan_t yyscanner;
    MScriptParserInterface * core;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (yyscanner);
  YYUSE (core);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 65: /* "\"string\"" */

/* Line 1391 of yacc.c  */
#line 232 "moraviaScriptParser.y"
	{
    free((yyvaluep->array).data);
};

/* Line 1391 of yacc.c  */
#line 1877 "moraviaScriptParser.cxx"
	break;
      case 70: /* "statements" */

/* Line 1391 of yacc.c  */
#line 224 "moraviaScriptParser.y"
	{
    if ( NULL != (yyvaluep->stmt) )
    {
        (yyvaluep->stmt)->completeDelete();
    }
};

/* Line 1391 of yacc.c  */
#line 1891 "moraviaScriptParser.cxx"
	break;
      case 71: /* "stmt" */

/* Line 1391 of yacc.c  */
#line 224 "moraviaScriptParser.y"
	{
    if ( NULL != (yyvaluep->stmt) )
    {
        (yyvaluep->stmt)->completeDelete();
    }
};

/* Line 1391 of yacc.c  */
#line 1905 "moraviaScriptParser.cxx"
	break;
      case 72: /* "switch_body" */

/* Line 1391 of yacc.c  */
#line 224 "moraviaScriptParser.y"
	{
    if ( NULL != (yyvaluep->stmt) )
    {
        (yyvaluep->stmt)->completeDelete();
    }
};

/* Line 1391 of yacc.c  */
#line 1919 "moraviaScriptParser.cxx"
	break;
      case 73: /* "cases" */

/* Line 1391 of yacc.c  */
#line 224 "moraviaScriptParser.y"
	{
    if ( NULL != (yyvaluep->stmt) )
    {
        (yyvaluep->stmt)->completeDelete();
    }
};

/* Line 1391 of yacc.c  */
#line 1933 "moraviaScriptParser.cxx"
	break;
      case 74: /* "id" */

/* Line 1391 of yacc.c  */
#line 216 "moraviaScriptParser.y"
	{
    if ( NULL != (yyvaluep->expr) )
    {
        (yyvaluep->expr)->completeDelete();
    }
};

/* Line 1391 of yacc.c  */
#line 1947 "moraviaScriptParser.cxx"
	break;
      case 75: /* "param_list" */

/* Line 1391 of yacc.c  */
#line 216 "moraviaScriptParser.y"
	{
    if ( NULL != (yyvaluep->expr) )
    {
        (yyvaluep->expr)->completeDelete();
    }
};

/* Line 1391 of yacc.c  */
#line 1961 "moraviaScriptParser.cxx"
	break;
      case 76: /* "e_expr" */

/* Line 1391 of yacc.c  */
#line 216 "moraviaScriptParser.y"
	{
    if ( NULL != (yyvaluep->expr) )
    {
        (yyvaluep->expr)->completeDelete();
    }
};

/* Line 1391 of yacc.c  */
#line 1975 "moraviaScriptParser.cxx"
	break;
      case 77: /* "expr" */

/* Line 1391 of yacc.c  */
#line 216 "moraviaScriptParser.y"
	{
    if ( NULL != (yyvaluep->expr) )
    {
        (yyvaluep->expr)->completeDelete();
    }
};

/* Line 1391 of yacc.c  */
#line 1989 "moraviaScriptParser.cxx"
	break;

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (yyscan_t yyscanner, MScriptParserInterface * core);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (yyscan_t yyscanner, MScriptParserInterface * core)
#else
int
yyparse (yyscanner, core)
    yyscan_t yyscanner;
    MScriptParserInterface * core;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

    yytype_int16 yyesa[20];
    yytype_int16 *yyes;
    YYSIZE_T yyes_capacity;

  int yy_lac_established = 0;
  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  yyes = yyesa;
  yyes_capacity = sizeof yyesa / sizeof *yyes;
  if (YYMAXDEPTH < yyes_capacity)
    yyes_capacity = YYMAXDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

/* User initialization code.  */

/* Line 1590 of yacc.c  */
#line 97 "moraviaScriptParser.y"
{
    yylloc.first_column = yylloc.last_column = 1;
    yylloc.first_line   = yylloc.last_line   = 1;
}

/* Line 1590 of yacc.c  */
#line 2154 "moraviaScriptParser.cxx"
  yylsp[0] = yylloc;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    {
      YY_LAC_ESTABLISH;
      goto yydefault;
    }
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      YY_LAC_ESTABLISH;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  YY_LAC_DISCARD ("shift");

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
          case 2:

/* Line 1806 of yacc.c  */
#line 251 "moraviaScriptParser.y"
    {
                                        core->syntaxAnalysisComplete((yyvsp[(1) - (1)].stmt));
                                        YYACCEPT;
                                    }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 255 "moraviaScriptParser.y"
    {
                                        core->syntaxAnalysisComplete(NULL);
                                        YYACCEPT;
                                    }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 263 "moraviaScriptParser.y"
    { (yyval.stmt) = (yyvsp[(1) - (1)].stmt);                 }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 264 "moraviaScriptParser.y"
    { (yyval.stmt) = (yyvsp[(1) - (2)].stmt)->appendLink((yyvsp[(2) - (2)].stmt)); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 269 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) = NULL;
                                    }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 272 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) = NULL;
                                    }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 275 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) = new MScriptStatement((yyloc), STMT_EXPR, (yyvsp[(1) - (2)].expr));
                                    }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 278 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) = new MScriptStatement((yyloc), STMT_RETURN, (yyvsp[(2) - (3)].expr));
                                    }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 281 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) = new MScriptStatement((yyloc), STMT_CONTINUE, (yyvsp[(2) - (3)].expr));
                                    }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 284 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) = new MScriptStatement((yyloc), STMT_BREAK, (yyvsp[(2) - (3)].expr));
                                    }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 287 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) = new MScriptStatement((yyloc), STMT_DELETE, (yyvsp[(2) - (3)].expr));
                                    }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 290 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) = (new MScriptStatement((yyloc), STMT_TRIGGER, (yyvsp[(2) - (4)].expr)))->createBranch((yyvsp[(4) - (4)].stmt));
                                    }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 294 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) =new MScriptStatement((yyloc), STMT_FUNCTION, (yyvsp[(2) - (6)].expr)->appendLink((yyvsp[(4) - (6)].expr)));
                                        (yyval.stmt)->createBranch((yyvsp[(6) - (6)].stmt));
                                    }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 298 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) = new MScriptStatement((yyloc), STMT_SCOPE);
                                        (yyval.stmt)->createBranch((yyvsp[(2) - (3)].stmt));
                                    }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 302 "moraviaScriptParser.y"
    {
                                        // Empty scope -> optimize by deleting it.
                                        (yyval.stmt) = NULL;
                                        core->parserMessage ( (yyloc),
                                                              MScriptBase::MT_WARNING,
                                                              QObject::tr("empty scope").toStdString() );
                                    }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 309 "moraviaScriptParser.y"
    {
                                        MScriptStatement * ifBlock = new MScriptStatement((yylsp[(1) - (5)]), STMT_IF, (yyvsp[(3) - (5)].expr));
                                        ifBlock->createBranch((yyvsp[(5) - (5)].stmt));

                                        (yyval.stmt) = new MScriptStatement((yyloc), STMT_CONDITION);
                                        (yyval.stmt)->createBranch(ifBlock);
                                    }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 317 "moraviaScriptParser.y"
    {
                                        MScriptStatement * ifBlock = new MScriptStatement((yylsp[(1) - (7)]), STMT_IF, (yyvsp[(3) - (7)].expr));
                                        ifBlock->createBranch((yyvsp[(5) - (7)].stmt));

                                        MScriptStatement * elseBlock = new MScriptStatement((yylsp[(6) - (7)]), STMT_ELSE);
                                        elseBlock->createBranch((yyvsp[(7) - (7)].stmt));

                                        (yyval.stmt) = new MScriptStatement((yyloc), STMT_CONDITION);
                                        (yyval.stmt)->createBranch(ifBlock->appendLink(elseBlock));
                                    }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 328 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) = new MScriptStatement((yyloc), STMT_FOR, (yyvsp[(3) - (9)].expr)->appendLink((yyvsp[(5) - (9)].expr))->appendLink((yyvsp[(7) - (9)].expr)));
                                        (yyval.stmt)->createBranch((yyvsp[(9) - (9)].stmt));
                                    }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 332 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) = (new MScriptStatement((yyloc), STMT_WHILE, (yyvsp[(3) - (5)].expr)))->createBranch((yyvsp[(5) - (5)].stmt));
                                    }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 336 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) =(new MScriptStatement((yyloc), STMT_DO_WHILE, (yyvsp[(5) - (7)].expr)))->createBranch((yyvsp[(2) - (7)].stmt));
                                    }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 340 "moraviaScriptParser.y"
    {
                                        (yyval.stmt) = new MScriptStatement((yyloc), STMT_SWITCH, (yyvsp[(3) - (7)].expr));
                                        (yyval.stmt)->createBranch((yyvsp[(6) - (7)].stmt));
                                    }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 345 "moraviaScriptParser.y"
    {
                                        // Empty scope -> optimize by rewriting it to expression statement.
                                        (yyval.stmt) = new MScriptStatement((yyloc), STMT_EXPR, (yyvsp[(3) - (6)].expr));
                                        core->parserMessage ( (yyloc),
                                                              MScriptBase::MT_WARNING,
                                                              QObject::tr("empty switch body").toStdString() );
                                    }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 356 "moraviaScriptParser.y"
    { (yyval.stmt) = (yyvsp[(1) - (2)].stmt)->createBranch((yyvsp[(2) - (2)].stmt));                 }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 357 "moraviaScriptParser.y"
    { (yyval.stmt) = (yyvsp[(1) - (3)].stmt)->appendLink((yyvsp[(2) - (3)].stmt)->createBranch((yyvsp[(3) - (3)].stmt))); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 362 "moraviaScriptParser.y"
    { (yyval.stmt) = new MScriptStatement((yyloc), STMT_CASE, (yyvsp[(2) - (3)].expr)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 363 "moraviaScriptParser.y"
    { (yyval.stmt) = new MScriptStatement((yyloc), STMT_DEFAULT);     }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 364 "moraviaScriptParser.y"
    { (yyval.stmt) = (yyvsp[(1) - (4)].stmt)->appendArgsLink((yyvsp[(3) - (4)].expr));                  }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 365 "moraviaScriptParser.y"
    {
                                        (yyvsp[(1) - (3)].stmt)->completeDelete();
                                        (yyval.stmt) = new MScriptStatement((yyloc), STMT_DEFAULT);
                                    }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 373 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (1)].string), (yyloc)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 378 "moraviaScriptParser.y"
    { (yyval.expr) = NULL;                                                         }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 379 "moraviaScriptParser.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr);                                                          }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 380 "moraviaScriptParser.y"
    { (yyval.expr) = (yyvsp[(1) - (3)].expr)->appendLink((yyvsp[(3) - (3)].expr));                                          }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 381 "moraviaScriptParser.y"
    { (yyval.expr) = (yyvsp[(2) - (2)].expr); (yyvsp[(2) - (2)].expr)->m_operator = MScriptExpr::OPER_REF;                 }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 382 "moraviaScriptParser.y"
    { (yyval.expr) = (yyvsp[(1) - (4)].expr)->appendLink((yyvsp[(4) - (4)].expr)); (yyvsp[(4) - (4)].expr)->m_operator = MScriptExpr::OPER_REF; }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 387 "moraviaScriptParser.y"
    { (yyval.expr) = NULL;  }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 388 "moraviaScriptParser.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 394 "moraviaScriptParser.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr);                                                           }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 395 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (1)].integer), (yyloc));                                 }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 396 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (1)].real), (yyloc));                                    }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 397 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr(MScriptValue((yyvsp[(1) - (1)].array).data, (yyvsp[(1) - (1)].array).size), (yyloc)); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 400 "moraviaScriptParser.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 403 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_ADD,  (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 404 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_SUB,  (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 405 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_MULT, (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 406 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_DIV,  (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 407 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_MOD,  (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 408 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_BOR,  (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 409 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_BXOR, (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 410 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_BAND, (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 411 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_LOR,  (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 412 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_LAND, (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 413 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_EQ,   (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 414 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_NE,   (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 415 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_LT,   (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 416 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_LE,   (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 417 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_GE,   (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 418 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_GT,   (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 419 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_SHR,  (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 420 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_SHL,  (yyvsp[(3) - (3)].expr), (yyloc));       }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 421 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_ASSIGN,     (yyvsp[(3) - (3)].expr), (yyloc)); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 422 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_ADD_ASSIGN, (yyvsp[(3) - (3)].expr), (yyloc)); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 423 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_SUB_ASSIGN, (yyvsp[(3) - (3)].expr), (yyloc)); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 424 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_MUL_ASSIGN, (yyvsp[(3) - (3)].expr), (yyloc)); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 425 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_DIV_ASSIGN, (yyvsp[(3) - (3)].expr), (yyloc)); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 426 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_MOD_ASSIGN, (yyvsp[(3) - (3)].expr), (yyloc)); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 427 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_SHL_ASSIGN, (yyvsp[(3) - (3)].expr), (yyloc)); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 428 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_SHR_ASSIGN, (yyvsp[(3) - (3)].expr), (yyloc)); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 429 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_AND_ASSIGN, (yyvsp[(3) - (3)].expr), (yyloc)); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 430 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_ORB_ASSIGN, (yyvsp[(3) - (3)].expr), (yyloc)); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 431 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_XOR_ASSIGN, (yyvsp[(3) - (3)].expr), (yyloc)); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 434 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(2) - (2)].expr), MScriptExpr::OPER_CMPL,     (yyloc)); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 435 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(2) - (2)].expr), MScriptExpr::OPER_NOT,      (yyloc)); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 436 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(2) - (2)].expr), MScriptExpr::OPER_INT_PROM, (yyloc)); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 437 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(2) - (2)].expr), MScriptExpr::OPER_ADD_INV,  (yyloc)); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 438 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(2) - (2)].expr), MScriptExpr::OPER_PRE_INC,  (yyloc)); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 439 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(2) - (2)].expr), MScriptExpr::OPER_PRE_DEC,  (yyloc)); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 440 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (2)].expr), MScriptExpr::OPER_POST_INC, (yyloc)); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 441 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (2)].expr), MScriptExpr::OPER_POST_DEC, (yyloc)); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 444 "moraviaScriptParser.y"
    {
                                        MScriptExpr * choices = new MScriptExpr((yyvsp[(3) - (5)].expr), MScriptExpr::OPER_COLON, (yyvsp[(5) - (5)].expr), (yyloc));
                                        (yyval.expr) = new MScriptExpr((yyvsp[(1) - (5)].expr), MScriptExpr::OPER_TERNARY, choices, (yyloc));
                                    }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 450 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_COMMA, (yyvsp[(3) - (3)].expr), (yyloc)); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 453 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (4)].expr), MScriptExpr::OPER_INDEX, (yyvsp[(3) - (4)].expr), (yyloc)); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 456 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (3)].expr), MScriptExpr::OPER_CALL, (yyloc));     }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 457 "moraviaScriptParser.y"
    { (yyval.expr) = new MScriptExpr((yyvsp[(1) - (4)].expr), MScriptExpr::OPER_CALL, (yyvsp[(3) - (4)].expr), (yyloc)); }
    break;



/* Line 1806 of yacc.c  */
#line 3001 "moraviaScriptParser.cxx"
        default: break;
      }
    if (yychar_backup != yychar)
      YY_LAC_DISCARD ("yychar change");
  }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, yyscanner, core, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyesa, &yyes, &yyes_capacity, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        if (yychar != YYEMPTY)
          YY_LAC_ESTABLISH;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, yyscanner, core, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, yyscanner, core);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, yyscanner, core);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

  *++yyvsp = yylval;

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, yyscanner, core, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, yyscanner, core);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, yyscanner, core);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yyes != yyesa)
    YYSTACK_FREE (yyes);
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 460 "moraviaScriptParser.y"


// -----------------------------------------------------------------------------
// EPILOGUE - FUNCTION DEFINITIONS
// -----------------------------------------------------------------------------

// Definition of the error reporting function used by Bison.
inline int moraviaScriptParser_error ( YYLTYPE * yylloc,
                                       yyscan_t,
                                       MScriptParserInterface * core,
                                       const char * errorInfo )
{
    core->parserMessage ( yylloc,
                          MScriptBase::MT_WARNING,
                          errorInfo );
    return 0;
}

