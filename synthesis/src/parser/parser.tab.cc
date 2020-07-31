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
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 2 "parser.y"

#include<iostream>
#include <string>
#include "parsehelp.h"
# define YYSTYPE_IS_TRIVIAL 1
# define YYLTYPE_IS_TRIVIAL 1

#define PRINT cout <<yytext <<endl;

    extern int yylex();
    extern char *yytext;
    void yyerror(char const *str) {
		//MsgHandler::instance()->print(str);
		MsgHandler::instance()->print(50, OMCreator::instance()->getLine());
    }



/* Line 268 of yacc.c  */
#line 90 "parser.tab.cc"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     SYSTEM_IDENTIFIER = 259,
     STRING = 260,
     PATHPULSE_IDENTIFIER = 261,
     BASED_NUMBER = 262,
     DEC_NUMBER = 263,
     REALTIME = 264,
     VLOG_LE = 265,
     VLOG_GE = 266,
     VLOG_EG = 267,
     VLOG_EQ = 268,
     VLOG_NE = 269,
     VLOG_CEQ = 270,
     VLOG_CNE = 271,
     VLOG_LS = 272,
     VLOG_RS = 273,
     VLOG_RSS = 274,
     VLOG_SG = 275,
     VLOG_PO_POS = 276,
     VLOG_PO_NEG = 277,
     VLOG_PSTAR = 278,
     VLOG_STARP = 279,
     DEFAULT_NETTYPE = 280,
     VLOG_LOR = 281,
     VLOG_LAND = 282,
     VLOG_NAND = 283,
     VLOG_NOR = 284,
     VLOG_NXOR = 285,
     VLOG_TRIGGER = 286,
     VLOG_POW = 287,
     VLOG_always = 288,
     VLOG_and = 289,
     VLOG_assign = 290,
     VLOG_begin = 291,
     VLOG_buf = 292,
     VLOG_bufif0 = 293,
     VLOG_bufif1 = 294,
     VLOG_case = 295,
     VLOG_casex = 296,
     VLOG_casez = 297,
     VLOG_cmos = 298,
     VLOG_deassign = 299,
     VLOG_default = 300,
     VLOG_defparam = 301,
     VLOG_disable = 302,
     VLOG_edge = 303,
     VLOG_else = 304,
     VLOG_end = 305,
     VLOG_endcase = 306,
     VLOG_endfunction = 307,
     VLOG_endmodule = 308,
     VLOG_endprimitive = 309,
     VLOG_endspecify = 310,
     VLOG_endtable = 311,
     VLOG_endtask = 312,
     VLOG_event = 313,
     VLOG_for = 314,
     VLOG_automatic = 315,
     VLOG_force = 316,
     VLOG_forever = 317,
     VLOG_fork = 318,
     VLOG_function = 319,
     VLOG_highz0 = 320,
     VLOG_highz1 = 321,
     VLOG_if = 322,
     VLOG_initial = 323,
     VLOG_inout = 324,
     VLOG_input = 325,
     VLOG_integer = 326,
     VLOG_join = 327,
     VLOG_large = 328,
     VLOG_localparam = 329,
     VLOG_macromodule = 330,
     VLOG_medium = 331,
     VLOG_module = 332,
     VLOG_nand = 333,
     VLOG_negedge = 334,
     VLOG_nmos = 335,
     VLOG_nor = 336,
     VLOG_not = 337,
     VLOG_notif0 = 338,
     VLOG_notif1 = 339,
     VLOG_or = 340,
     VLOG_output = 341,
     VLOG_parameter = 342,
     VLOG_pmos = 343,
     VLOG_posedge = 344,
     VLOG_primitive = 345,
     VLOG_pull0 = 346,
     VLOG_pull1 = 347,
     VLOG_pulldown = 348,
     VLOG_pullup = 349,
     VLOG_rcmos = 350,
     VLOG_real = 351,
     VLOG_realtime = 352,
     VLOG_reg = 353,
     VLOG_release = 354,
     VLOG_repeat = 355,
     VLOG_rnmos = 356,
     VLOG_rpmos = 357,
     VLOG_rtran = 358,
     VLOG_rtranif0 = 359,
     VLOG_rtranif1 = 360,
     VLOG_scalared = 361,
     VLOG_signed = 362,
     VLOG_small = 363,
     VLOG_specify = 364,
     VLOG_specparam = 365,
     VLOG_strong0 = 366,
     VLOG_strong1 = 367,
     VLOG_supply0 = 368,
     VLOG_supply1 = 369,
     VLOG_table = 370,
     VLOG_task = 371,
     VLOG_time = 372,
     VLOG_tran = 373,
     VLOG_tranif0 = 374,
     VLOG_tranif1 = 375,
     VLOG_tri = 376,
     VLOG_tri0 = 377,
     VLOG_tri1 = 378,
     VLOG_triand = 379,
     VLOG_trior = 380,
     VLOG_trireg = 381,
     VLOG_vectored = 382,
     VLOG_wait = 383,
     VLOG_wand = 384,
     VLOG_weak0 = 385,
     VLOG_weak1 = 386,
     VLOG_while = 387,
     VLOG_wire = 388,
     VLOG_wor = 389,
     VLOG_xnor = 390,
     VLOG_xor = 391,
     VLOG_Shold = 392,
     VLOG_Speriod = 393,
     VLOG_Srecovery = 394,
     VLOG_Srecrem = 395,
     VLOG_Ssetup = 396,
     VLOG_Swidth = 397,
     VLOG_Ssetuphold = 398,
     VVLOG_attribute = 399,
     VLOG_Sremoval = 400,
     VLOG_Sskew = 401,
     VLOG_Stimeskew = 402,
     VLOG_Sfullskew = 403,
     VLOG_Snochange = 404,
     VLOG_TAND = 405,
     VLOG_generate = 406,
     VLOG_endgenerate = 407,
     VLOG_genvar = 408,
     UNARY_PREC = 409,
     less_than_VLOG_else = 410
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 20 "parser.y"

    char *text;
    std::vector < std::pair < char *, Expr * > >*vtext;
    bool BOOL;
    int INT;
    char CHAR;
    Module *mod;
    Param *param;
    LocalParam *lparam;
    Port *port;
    Range *range;
    Expr *expr;
    Const *CONST;
    Inst *inst;
    NetType nettype;
    Dir dir;
     std::vector < char >*VCHAR;
     std::pair < int, Range * >*typerange;
     std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >*dirsigtyperange;
     std::pair < char *, Expr * >*idassp;
     std::pair < Expr *, Expr * >*assgp;
     std::pair < char *, std::vector < Range * >*>*iddim;
     std::pair < std::vector < Expr * >, GenerateItem * >*exprsgenitems;
     std::vector < std::pair < std::vector < Expr * >,
        GenerateItem * > >*exprsgenitemsv;
    ParseInfo *parseInfo;
    GateType gtype;
    NameConn *nconn;
    DelayVal *dval;
    Delay *delay;
    DrStr drstr;
    DriveSt *dst;
    Stmt *stmt;
    RegInfo *reginfo;
    CaseItem *caseitem;
    EventCntrl *evctrl;
    Defparam *defparam;
    Specparam *sparam;
    SpecRefEvent *specre;
    SpecifyPathBase *specpath;
    PolOp pop;
    UDP *udp;
    GenerateItem *genitem;
     std::vector < GenerateItem * >*genitems;
     std::pair < EdgeType, Expr * >*edexpr;
     std::vector < std::pair < EdgeType, Expr * > >*edexprs;
     std::vector < Stmt * >*stmts;
     std::vector < ParseInfo * >*parseInfos;
     std::vector < Param * >*params;
     std::vector < LocalParam * >*lparams;
     std::vector < Expr * >*exprs;
     std::vector < Inst * >*insts;
     std::vector < NameConn * >*nconns;
     std::vector < std::pair < Expr *, Expr * >*>*assgps;
     std::vector < RegInfo * >*reginfos;
     std::vector < CaseItem * >*caseitems;
     std::vector < Defparam * >*defparams;
     std::vector < Specparam * >*sparams;
     std::vector < DelayVal * >*dvals;
     std::vector < char *>*ids;
     std::vector < Range * >*ranges;
     std::vector < std::pair < char *, std::vector < Range * >*> >*iddims;
     std::map < std::string, Expr *, compare_str > *attribtable;



/* Line 293 of yacc.c  */
#line 349 "parser.tab.cc"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 361 "parser.tab.cc"

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

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  28
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   7515

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  201
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  181
/* YYNRULES -- Number of rules.  */
#define YYNRULES  619
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1418

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   410

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   176,     2,   173,     2,   165,   158,     2,
     171,   172,   163,   161,   168,   162,   181,   164,   182,   183,
       2,     2,     2,     2,     2,     2,     2,     2,   155,   170,
     159,   169,   160,   154,   174,     2,   190,     2,     2,     2,
     187,     2,     2,     2,     2,     2,     2,   193,   195,     2,
     197,   198,   192,     2,     2,     2,     2,     2,     2,     2,
       2,   177,     2,   178,   157,   200,     2,     2,   185,     2,
       2,     2,   186,     2,   189,     2,     2,     2,   188,     2,
     194,     2,   196,   199,   191,     2,     2,     2,     2,     2,
     184,     2,     2,   179,   156,   180,   175,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   166,
     167
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    11,    13,    15,    18,
      22,    25,    26,    30,    32,    34,    38,    45,    51,    56,
      60,    64,    68,    72,    76,    80,    85,    90,    94,    98,
     102,   106,   110,   112,   115,   117,   118,   122,   126,   129,
     133,   136,   138,   142,   146,   150,   152,   153,   157,   159,
     162,   166,   169,   174,   177,   182,   189,   198,   200,   201,
     203,   209,   211,   213,   215,   217,   219,   222,   231,   237,
     243,   249,   255,   261,   267,   269,   270,   272,   274,   276,
     278,   280,   282,   284,   286,   289,   294,   299,   301,   305,
     309,   312,   315,   317,   319,   322,   325,   328,   331,   334,
     337,   340,   343,   346,   349,   352,   355,   359,   363,   367,
     371,   375,   379,   383,   387,   391,   395,   399,   403,   407,
     411,   415,   419,   423,   427,   431,   435,   439,   443,   447,
     451,   455,   461,   465,   467,   468,   471,   473,   475,   477,
     479,   481,   484,   492,   499,   504,   509,   513,   517,   524,
     531,   536,   543,   548,   555,   560,   562,   566,   571,   574,
     576,   578,   581,   586,   592,   596,   599,   604,   608,   610,
     612,   614,   616,   618,   620,   622,   624,   626,   628,   630,
     632,   634,   636,   638,   640,   642,   644,   646,   648,   650,
     652,   654,   656,   658,   660,   662,   664,   668,   671,   676,
     678,   682,   684,   687,   689,   690,   694,   699,   701,   705,
     707,   711,   715,   723,   731,   739,   747,   756,   758,   759,
     761,   762,   764,   767,   775,   782,   786,   788,   791,   799,
     806,   810,   814,   818,   820,   821,   822,   823,   824,   837,
     839,   841,   844,   848,   852,   853,   858,   859,   863,   867,
     872,   879,   886,   893,   899,   905,   912,   919,   921,   924,
     931,   938,   944,   947,   948,   953,   957,   959,   963,   968,
     974,   980,   987,   991,   998,  1005,  1006,  1012,  1018,  1022,
    1026,  1027,  1028,  1038,  1039,  1040,  1053,  1054,  1065,  1066,
    1080,  1082,  1085,  1091,  1095,  1099,  1109,  1115,  1116,  1117,
    1123,  1125,  1126,  1128,  1131,  1138,  1145,  1152,  1158,  1160,
    1162,  1163,  1165,  1167,  1169,  1171,  1173,  1178,  1181,  1182,
    1188,  1191,  1192,  1200,  1204,  1206,  1209,  1213,  1217,  1220,
    1221,  1232,  1234,  1236,  1240,  1241,  1248,  1249,  1254,  1257,
    1259,  1261,  1262,  1266,  1270,  1272,  1274,  1276,  1278,  1280,
    1282,  1284,  1286,  1288,  1290,  1292,  1294,  1298,  1300,  1303,
    1307,  1310,  1312,  1316,  1320,  1322,  1325,  1329,  1332,  1334,
    1338,  1343,  1348,  1351,  1354,  1355,  1361,  1366,  1368,  1372,
    1374,  1380,  1384,  1392,  1394,  1395,  1397,  1404,  1409,  1414,
    1416,  1420,  1426,  1432,  1437,  1441,  1443,  1445,  1447,  1449,
    1455,  1457,  1458,  1460,  1462,  1464,  1466,  1468,  1469,  1471,
    1473,  1475,  1477,  1479,  1483,  1486,  1488,  1492,  1496,  1499,
    1502,  1509,  1516,  1527,  1536,  1547,  1558,  1571,  1584,  1595,
    1603,  1611,  1619,  1627,  1635,  1643,  1645,  1649,  1651,  1654,
    1660,  1664,  1672,  1684,  1692,  1704,  1712,  1724,  1732,  1744,
    1746,  1748,  1750,  1756,  1760,  1766,  1773,  1780,  1784,  1786,
    1789,  1793,  1798,  1802,  1810,  1814,  1822,  1824,  1828,  1830,
    1832,  1833,  1836,  1839,  1844,  1849,  1853,  1855,  1856,  1858,
    1860,  1863,  1866,  1870,  1872,  1878,  1882,  1888,  1892,  1896,
    1897,  1898,  1907,  1910,  1915,  1919,  1920,  1921,  1930,  1933,
    1938,  1942,  1946,  1949,  1950,  1955,  1961,  1968,  1975,  1982,
    1989,  1996,  2003,  2009,  2017,  2023,  2031,  2045,  2057,  2071,
    2077,  2083,  2089,  2092,  2095,  2099,  2105,  2110,  2115,  2121,
    2127,  2133,  2143,  2149,  2159,  2165,  2171,  2174,  2180,  2183,
    2186,  2189,  2191,  2193,  2195,  2197,  2199,  2201,  2203,  2207,
    2212,  2214,  2217,  2220,  2222,  2224,  2225,  2227,  2228,  2229,
    2234,  2236,  2238,  2243,  2245,  2248,  2250,  2253,  2260,  2266,
    2268,  2269,  2271,  2274,  2276,  2278,  2280,  2282,  2284,  2286,
    2288,  2290,  2292,  2294,  2296,  2298,  2300,  2302,  2304,  2306,
    2308,  2310,  2312,  2314,  2316,  2318,  2320,  2322,  2324,  2326,
    2328,  2332,  2336,  2340,  2345,  2347,  2350,  2352,  2356,  2358,
    2359,  2362,  2363,  2366,  2371,  2372,  2373,  2386,  2387,  2388
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     202,     0,    -1,   203,    -1,    -1,   222,    -1,   203,   222,
      -1,     7,    -1,     8,    -1,     8,     7,    -1,    23,   206,
      24,    -1,    23,    24,    -1,    -1,   206,   168,   207,    -1,
     207,    -1,     3,    -1,     3,   169,   230,    -1,   205,    98,
     253,   324,   329,   170,    -1,   205,    98,   253,   329,   170,
      -1,   205,    71,   329,   170,    -1,   117,   329,   170,    -1,
      96,   244,   170,    -1,    97,   244,   170,    -1,    87,   309,
     170,    -1,    74,   312,   170,    -1,    58,   244,   170,    -1,
     205,    98,     1,   170,    -1,   205,    71,     1,   170,    -1,
     117,     1,   170,    -1,    96,     1,   170,    -1,    97,     1,
     170,    -1,    87,     1,   170,    -1,    74,     1,   170,    -1,
     208,    -1,   209,   208,    -1,   209,    -1,    -1,   231,   155,
     352,    -1,    45,   155,   352,    -1,    45,   352,    -1,     1,
     155,   352,    -1,   212,   211,    -1,   211,    -1,   171,   108,
     172,    -1,   171,    76,   172,    -1,   171,    73,   172,    -1,
     213,    -1,    -1,   243,   169,   230,    -1,   215,    -1,   324,
     215,    -1,   216,   168,   215,    -1,   173,   221,    -1,   173,
     171,   220,   172,    -1,   173,   221,    -1,   173,   171,   220,
     172,    -1,   173,   171,   220,   168,   220,   172,    -1,   173,
     171,   220,   168,   220,   168,   220,   172,    -1,   218,    -1,
      -1,   230,    -1,   230,   155,   230,   155,   230,    -1,     8,
      -1,     9,    -1,     3,    -1,   258,    -1,   377,    -1,    25,
     306,    -1,   144,   171,     3,   168,     5,   168,     5,   172,
      -1,   171,   225,   168,   226,   172,    -1,   171,   226,   168,
     225,   172,    -1,   171,   225,   168,    66,   172,    -1,   171,
     226,   168,    65,   172,    -1,   171,    66,   168,   225,   172,
      -1,   171,    65,   168,   226,   172,    -1,   223,    -1,    -1,
     113,    -1,   111,    -1,    91,    -1,   130,    -1,   114,    -1,
     112,    -1,    92,    -1,   131,    -1,   174,   243,    -1,   174,
     171,   228,   172,    -1,   174,   171,     1,   172,    -1,   229,
      -1,   228,    85,   229,    -1,   228,   168,   229,    -1,    89,
     230,    -1,    79,   230,    -1,   230,    -1,   232,    -1,   161,
     232,    -1,   162,   232,    -1,   175,   232,    -1,   158,   232,
      -1,   176,   232,    -1,   156,   232,    -1,   157,   232,    -1,
      28,   232,    -1,    29,   232,    -1,    30,   232,    -1,   176,
       1,    -1,   157,     1,    -1,   230,   157,   230,    -1,   230,
     163,   230,    -1,   230,   164,   230,    -1,   230,   165,   230,
      -1,   230,   161,   230,    -1,   230,   162,   230,    -1,   230,
      32,   230,    -1,   230,   158,   230,    -1,   230,   156,   230,
      -1,   230,    28,   230,    -1,   230,    29,   230,    -1,   230,
      30,   230,    -1,   230,   159,   230,    -1,   230,   160,   230,
      -1,   230,    17,   230,    -1,   230,    18,   230,    -1,   230,
      19,   230,    -1,   230,    13,   230,    -1,   230,    15,   230,
      -1,   230,    10,   230,    -1,   230,    11,   230,    -1,   230,
      14,   230,    -1,   230,    16,   230,    -1,   230,    26,   230,
      -1,   230,    27,   230,    -1,   230,   154,   230,   155,   230,
      -1,   231,   168,   230,    -1,   230,    -1,    -1,   231,   168,
      -1,   204,    -1,     9,    -1,     5,    -1,   243,    -1,     4,
      -1,   243,   248,    -1,   243,   248,   177,   230,   335,   230,
     178,    -1,   243,   177,   230,   335,   230,   178,    -1,   243,
     171,   231,   172,    -1,     4,   171,   231,   172,    -1,   171,
     230,   172,    -1,   179,   231,   180,    -1,   179,   230,   179,
     231,   180,   180,    -1,    70,   374,   253,   325,     3,   247,
      -1,    70,   326,     3,   247,    -1,    86,    98,   253,   325,
       3,   247,    -1,    86,   326,     3,   247,    -1,    69,    98,
     253,   325,     3,   247,    -1,    69,   326,     3,   247,    -1,
     233,    -1,   236,   168,   233,    -1,   236,   168,     3,   247,
      -1,   236,   170,    -1,   208,    -1,   237,    -1,   238,   237,
      -1,     3,   171,   231,   172,    -1,     3,   324,   171,   231,
     172,    -1,   171,   231,   172,    -1,     3,   324,    -1,     3,
     171,   322,   172,    -1,   240,   168,   239,    -1,   239,    -1,
      34,    -1,    78,    -1,    85,    -1,    81,    -1,   136,    -1,
     135,    -1,    37,    -1,    38,    -1,    39,    -1,    82,    -1,
      83,    -1,    84,    -1,    80,    -1,   101,    -1,    88,    -1,
     102,    -1,    43,    -1,    95,    -1,   118,    -1,   103,    -1,
     119,    -1,   120,    -1,   104,    -1,   105,    -1,    94,    -1,
      93,    -1,     3,    -1,   243,   181,     3,    -1,     3,   247,
      -1,   244,   168,     3,   247,    -1,     3,    -1,   245,   168,
       3,    -1,   324,    -1,   246,   324,    -1,   246,    -1,    -1,
     177,   230,   178,    -1,   248,   177,   230,   178,    -1,   318,
      -1,   249,   168,   318,    -1,   251,    -1,   250,   168,   251,
      -1,   250,   168,     3,    -1,   205,    70,   252,   253,   325,
       3,   247,    -1,   205,    69,   252,   253,   325,     3,   247,
      -1,   205,    86,   252,   253,   325,     3,   247,    -1,   205,
      86,   307,   253,   325,     3,   247,    -1,   205,    86,   307,
     253,   325,     3,   169,   230,    -1,   306,    -1,    -1,   107,
      -1,    -1,   243,    -1,   243,   248,    -1,   243,   248,   177,
     230,   335,   230,   178,    -1,   243,   177,   230,   335,   230,
     178,    -1,   179,   231,   180,    -1,   243,    -1,   243,   248,
      -1,   243,   248,   177,   230,   335,   230,   178,    -1,   243,
     177,   230,   335,   230,   178,    -1,   179,   231,   180,    -1,
     254,   169,   230,    -1,   257,   168,   256,    -1,   256,    -1,
      -1,    -1,    -1,    -1,   205,   263,     3,   259,   266,   260,
     265,   170,   261,   303,    53,   262,    -1,    77,    -1,    75,
      -1,   325,   219,    -1,   171,   249,   172,    -1,   171,   250,
     172,    -1,    -1,   173,   171,   267,   172,    -1,    -1,    87,
     326,   308,    -1,   267,   168,   308,    -1,   267,   168,    87,
     308,    -1,   205,   306,   253,   264,   244,   170,    -1,   205,
     306,   253,   264,   305,   170,    -1,   205,   306,   253,   223,
     305,   170,    -1,   126,   214,   264,   244,   170,    -1,   323,
     253,   264,   244,   170,    -1,   323,   306,   253,   325,   244,
     170,    -1,    86,   307,   253,   325,   244,   170,    -1,   270,
      -1,   205,   278,    -1,    70,   307,   253,   325,   244,   170,
      -1,    69,   307,   253,   325,   244,   170,    -1,   323,   253,
     264,     1,   170,    -1,   109,    55,    -1,    -1,   109,   269,
     332,    55,    -1,   109,     1,    55,    -1,   208,    -1,    46,
     216,   170,    -1,   205,   241,   240,   170,    -1,   205,   241,
     218,   240,   170,    -1,   205,   241,   223,   240,   170,    -1,
     205,   241,   224,   218,   240,   170,    -1,   242,   240,   170,
      -1,   242,   171,   226,   172,   240,   170,    -1,   242,   171,
     225,   172,   240,   170,    -1,    -1,     3,   271,   314,   240,
     170,    -1,    35,   224,   219,   257,   170,    -1,   205,    33,
     345,    -1,   205,    68,   345,    -1,    -1,    -1,   116,   358,
       3,   170,   272,   357,   352,   273,    57,    -1,    -1,    -1,
     116,   358,     3,   274,   171,   354,   172,   170,   210,   352,
     275,    57,    -1,    -1,    64,   358,   253,   326,     3,   170,
     276,   238,   345,    52,    -1,    -1,    64,   358,   253,   326,
       3,   277,   171,   236,   172,   170,   210,   345,    52,    -1,
     291,    -1,     1,   170,    -1,    35,     1,   169,   230,   170,
      -1,    35,     1,   170,    -1,    64,     1,    52,    -1,   144,
     171,     3,   168,     5,   168,     5,   172,   170,    -1,   144,
     171,     1,   172,   170,    -1,    -1,    -1,   151,   279,   281,
     280,   152,    -1,   282,    -1,    -1,   285,    -1,   282,   285,
      -1,   205,   306,   253,   264,   244,   170,    -1,   205,   306,
     253,   264,   305,   170,    -1,   205,   306,   253,   223,   305,
     170,    -1,   126,   214,   264,   244,   170,    -1,   270,    -1,
     285,    -1,    -1,   289,    -1,   287,    -1,   294,    -1,   299,
      -1,   283,    -1,    67,   171,   230,   172,    -1,   286,   285,
      -1,    -1,   286,   285,    49,   288,   285,    -1,   286,     1,
      -1,    -1,    40,   171,   230,   172,   290,   292,    51,    -1,
     153,     3,   170,    -1,   293,    -1,   292,   293,    -1,   231,
     155,   284,    -1,    45,   155,   285,    -1,    45,   285,    -1,
      -1,    59,   171,   298,   170,   230,   170,   298,   172,   295,
     285,    -1,     3,    -1,     3,    -1,   296,   169,   230,    -1,
      -1,    36,   155,   297,   300,   282,    50,    -1,    -1,    36,
     301,   282,    50,    -1,   302,   268,    -1,   268,    -1,   302,
      -1,    -1,     3,   169,   230,    -1,   305,   168,   304,    -1,
     304,    -1,   133,    -1,   121,    -1,   123,    -1,   113,    -1,
     129,    -1,   124,    -1,   122,    -1,   114,    -1,   134,    -1,
     125,    -1,    98,    -1,     3,   169,   230,    -1,   310,    -1,
     324,   310,    -1,   107,   324,   310,    -1,   327,   310,    -1,
     308,    -1,   310,   168,   308,    -1,     3,   169,   230,    -1,
     313,    -1,   324,   313,    -1,   107,   324,   313,    -1,   327,
     313,    -1,   311,    -1,   313,   168,   311,    -1,   173,   171,
     231,   172,    -1,   173,   171,   316,   172,    -1,   173,     8,
      -1,   173,     1,    -1,    -1,   181,     3,   171,   230,   172,
      -1,   181,     3,   171,   172,    -1,   315,    -1,   316,   168,
     315,    -1,   319,    -1,   181,     3,   171,   319,   172,    -1,
     179,   320,   180,    -1,   181,     3,   171,   179,   320,   180,
     172,    -1,   317,    -1,    -1,     3,    -1,     3,   177,   230,
     155,   230,   178,    -1,     3,   177,   230,   178,    -1,     3,
     177,     1,   178,    -1,   319,    -1,   320,   168,   319,    -1,
     181,     3,   171,   230,   172,    -1,   181,     3,   171,     1,
     172,    -1,   181,     3,   171,   172,    -1,   322,   168,   321,
      -1,   321,    -1,    70,    -1,    86,    -1,    69,    -1,   177,
     230,   155,   230,   178,    -1,   324,    -1,    -1,   324,    -1,
      71,    -1,    96,    -1,    97,    -1,   117,    -1,    -1,    71,
      -1,    96,    -1,    97,    -1,   117,    -1,     3,    -1,     3,
     169,   230,    -1,     3,   246,    -1,   328,    -1,   329,   168,
     328,    -1,   110,   340,   170,    -1,   336,   170,    -1,   333,
     170,    -1,    67,   171,   230,   172,   336,   170,    -1,    67,
     171,   230,   172,   333,   170,    -1,   137,   171,   342,   168,
     342,   168,   220,   343,   172,   170,    -1,   138,   171,   342,
     168,   220,   343,   172,   170,    -1,   139,   171,   342,   168,
     342,   168,   220,   343,   172,   170,    -1,   141,   171,   342,
     168,   342,   168,   220,   343,   172,   170,    -1,   143,   171,
     342,   168,   342,   168,   220,   168,   220,   343,   172,   170,
      -1,   140,   171,   342,   168,   342,   168,   220,   168,   220,
     343,   172,   170,    -1,   142,   171,   342,   168,   220,   168,
     230,   343,   172,   170,    -1,   142,   171,   342,   168,   220,
     172,   170,    -1,   145,   171,   342,   168,   220,   172,   170,
      -1,   146,   171,   342,   168,   220,   172,   170,    -1,   147,
     171,   342,   168,   220,   172,   170,    -1,   148,   171,   342,
     168,   220,   172,   170,    -1,   149,   171,   342,   168,   220,
     172,   170,    -1,   220,    -1,   331,   168,   220,    -1,   330,
      -1,   332,   330,    -1,   334,   169,   171,   331,   172,    -1,
     334,   169,   221,    -1,   171,    89,   338,   341,    12,     3,
     172,    -1,   171,    89,   338,   341,    12,   171,   232,   335,
     230,   172,   172,    -1,   171,    89,   338,   341,    20,     3,
     172,    -1,   171,    89,   338,   341,    20,   171,   232,   335,
     230,   172,   172,    -1,   171,    79,   338,   341,    12,     3,
     172,    -1,   171,    79,   338,   341,    12,   171,   232,   335,
     230,   172,   172,    -1,   171,    79,   338,   341,    20,     3,
     172,    -1,   171,    79,   338,   341,    20,   171,   232,   335,
     230,   172,   172,    -1,    21,    -1,    22,    -1,   155,    -1,
     337,   169,   171,   331,   172,    -1,   337,   169,   221,    -1,
     337,   169,   171,     1,   172,    -1,   171,   338,   341,    12,
     338,   172,    -1,   171,   338,   341,    20,   338,   172,    -1,
     171,     1,   172,    -1,     3,    -1,     3,   248,    -1,   338,
     168,     3,    -1,   338,   168,     3,   248,    -1,     3,   169,
     230,    -1,     3,   169,   230,   155,   230,   155,   230,    -1,
       6,   169,   230,    -1,     6,   169,   171,   230,   168,   230,
     172,    -1,   339,    -1,   340,   168,   339,    -1,   161,    -1,
     162,    -1,    -1,    89,   230,    -1,    79,   230,    -1,    89,
     232,   150,   230,    -1,    79,   232,   150,   230,    -1,   232,
     150,   230,    -1,   232,    -1,    -1,   344,    -1,   168,    -1,
     168,   243,    -1,   344,   168,    -1,   344,   168,   243,    -1,
       3,    -1,    35,   254,   169,   230,   170,    -1,    44,   254,
     170,    -1,    61,   254,   169,   230,   170,    -1,    99,   254,
     170,    -1,    36,   351,    50,    -1,    -1,    -1,    36,   155,
       3,   346,   210,   351,   347,    50,    -1,    36,    50,    -1,
      36,   155,     3,    50,    -1,    36,     1,    50,    -1,    -1,
      -1,    63,   155,     3,   348,   210,   351,    72,   349,    -1,
      63,    72,    -1,    63,   155,     3,    72,    -1,    47,   243,
     170,    -1,    31,   243,   170,    -1,    62,   345,    -1,    -1,
      63,   350,   351,    72,    -1,   100,   171,   230,   172,   345,
      -1,    40,   171,   230,   172,   212,    51,    -1,    41,   171,
     230,   172,   212,    51,    -1,    42,   171,   230,   172,   212,
      51,    -1,    40,   171,   230,   172,     1,    51,    -1,    41,
     171,   230,   172,     1,    51,    -1,    42,   171,   230,   172,
       1,    51,    -1,    67,   171,   230,   172,   352,    -1,    67,
     171,   230,   172,   352,    49,   352,    -1,    67,   171,     1,
     172,   352,    -1,    67,   171,     1,   172,   352,    49,   352,
      -1,    59,   171,   255,   169,   230,   170,   230,   170,   255,
     169,   230,   172,   345,    -1,    59,   171,   255,   169,   230,
     170,   230,   170,     1,   172,   345,    -1,    59,   171,   255,
     169,   230,   170,     1,   170,   255,   169,   230,   172,   345,
      -1,    59,   171,     1,   172,   345,    -1,   132,   171,   230,
     172,   345,    -1,   132,   171,     1,   172,   345,    -1,   217,
     352,    -1,   227,   352,    -1,   174,   163,   352,    -1,   174,
     171,   163,   172,   352,    -1,   255,   169,   230,   170,    -1,
     255,    10,   230,   170,    -1,   255,   169,   217,   230,   170,
      -1,   255,    10,   217,   230,   170,    -1,   255,   169,   227,
     230,   170,    -1,   255,   169,   100,   171,   230,   172,   227,
     230,   170,    -1,   255,    10,   227,   230,   170,    -1,   255,
      10,   100,   171,   230,   172,   227,   230,   170,    -1,   128,
     171,   230,   172,   352,    -1,     4,   171,   231,   172,   170,
      -1,     4,   170,    -1,   243,   171,   231,   172,   170,    -1,
     243,   170,    -1,     1,   170,    -1,   351,   345,    -1,   345,
      -1,   345,    -1,   170,    -1,   233,    -1,   234,    -1,   235,
      -1,   353,    -1,   354,   168,   353,    -1,   354,   168,     3,
     247,    -1,   208,    -1,   354,   170,    -1,   356,   355,    -1,
     355,    -1,   356,    -1,    -1,    60,    -1,    -1,    -1,   115,
     360,   361,    56,    -1,   363,    -1,   364,    -1,   368,   155,
     370,   170,    -1,   362,    -1,   363,   362,    -1,   365,    -1,
     364,   365,    -1,   368,   155,   369,   155,   370,   170,    -1,
      68,     3,   169,   204,   170,    -1,   366,    -1,    -1,   369,
      -1,   368,   369,    -1,   182,    -1,   183,    -1,   184,    -1,
     154,    -1,   185,    -1,   163,    -1,   165,    -1,   186,    -1,
     187,    -1,   188,    -1,   189,    -1,   190,    -1,   191,    -1,
     192,    -1,   193,    -1,   194,    -1,   195,    -1,   196,    -1,
     197,    -1,   198,    -1,   199,    -1,   200,    -1,   161,    -1,
     182,    -1,   183,    -1,   184,    -1,   162,    -1,    70,   245,
     170,    -1,    86,     3,   170,    -1,    98,     3,   170,    -1,
      98,    86,     3,   170,    -1,   371,    -1,   372,   371,    -1,
       3,    -1,   373,   168,     3,    -1,    98,    -1,    -1,   169,
     230,    -1,    -1,    70,     3,    -1,   376,   168,    70,     3,
      -1,    -1,    -1,    90,     3,   171,   373,   172,   170,   372,
     378,   367,   379,   359,    54,    -1,    -1,    -1,    90,     3,
     171,    86,   374,     3,   375,   168,   376,   172,   170,   380,
     359,    54,   381,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   252,   252,   252,   254,   254,   256,   260,   264,   270,
     274,   278,   284,   289,   298,   304,   312,   317,   321,   325,
     329,   333,   337,   341,   344,   351,   355,   359,   363,   367,
     371,   375,   381,   381,   383,   383,   385,   389,   395,   401,
     407,   412,   419,   419,   419,   421,   421,   423,   432,   437,
     443,   450,   454,   460,   464,   468,   472,   478,   482,   488,
     492,   498,   502,   506,   512,   513,   514,   515,   520,   524,
     528,   532,   536,   540,   546,   550,   556,   560,   564,   568,
     574,   578,   582,   586,   592,   599,   604,   609,   615,   621,
     630,   634,   638,   644,   648,   652,   656,   660,   664,   668,
     672,   676,   680,   684,   688,   693,   698,   702,   706,   710,
     714,   718,   722,   725,   729,   733,   737,   741,   745,   749,
     753,   757,   761,   765,   769,   773,   777,   781,   785,   789,
     793,   797,   803,   814,   819,   823,   832,   836,   840,   844,
     851,   855,   859,   864,   870,   879,   883,   887,   892,   899,
     905,   920,   928,   943,   951,   966,   970,   974,   984,   984,
     989,   992,   997,  1012,  1025,  1041,  1047,  1053,  1058,  1065,
    1070,  1075,  1080,  1085,  1090,  1095,  1100,  1105,  1110,  1115,
    1120,  1125,  1130,  1135,  1140,  1145,  1150,  1155,  1160,  1165,
    1170,  1175,  1180,  1187,  1191,  1200,  1204,  1211,  1216,  1221,
    1226,  1233,  1244,  1253,  1257,  1263,  1268,  1275,  1282,  1291,
    1296,  1301,  1310,  1323,  1335,  1348,  1360,  1375,  1379,  1385,
    1389,  1395,  1402,  1406,  1411,  1416,  1423,  1427,  1431,  1436,
    1442,  1449,  1455,  1460,  1468,  1472,  1476,  1483,  1468,  1491,
    1494,  1496,  1502,  1506,  1510,  1517,  1521,  1527,  1534,  1545,
    1562,  1567,  1571,  1575,  1579,  1584,  1588,  1592,  1592,  1593,
    1598,  1603,  1607,  1612,  1612,  1618,  1625,  1625,  1633,  1640,
    1645,  1650,  1656,  1661,  1670,  1680,  1680,  1694,  1699,  1703,
    1708,  1712,  1708,  1718,  1722,  1718,  1731,  1731,  1738,  1738,
    1747,  1752,  1756,  1760,  1764,  1768,  1771,  1777,  1781,  1777,
    1791,  1795,  1801,  1806,  1815,  1820,  1824,  1828,  1832,  1837,
    1841,  1848,  1852,  1856,  1861,  1865,  1870,  1874,  1886,  1886,
    1903,  1908,  1908,  1931,  1937,  1944,  1952,  1961,  1967,  1976,
    1976,  1994,  1999,  2004,  2013,  2013,  2028,  2028,  2045,  2045,
    2047,  2047,  2050,  2056,  2062,  2070,  2074,  2078,  2082,  2086,
    2090,  2094,  2098,  2102,  2106,  2112,  2117,  2125,  2129,  2135,
    2141,  2148,  2153,  2160,  2168,  2172,  2177,  2183,  2189,  2194,
    2201,  2217,  2221,  2226,  2230,  2236,  2240,  2246,  2251,  2260,
    2265,  2272,  2279,  2287,  2291,  2297,  2304,  2311,  2315,  2321,
    2326,  2334,  2338,  2341,  2347,  2352,  2359,  2363,  2367,  2372,
    2379,  2383,  2390,  2394,  2398,  2402,  2406,  2410,  2416,  2420,
    2424,  2428,  2433,  2437,  2441,  2447,  2452,  2459,  2463,  2467,
    2471,  2478,  2485,  2491,  2494,  2498,  2502,  2506,  2510,  2514,
    2516,  2518,  2520,  2522,  2524,  2529,  2534,  2541,  2541,  2543,
    2551,  2560,  2566,  2574,  2579,  2587,  2592,  2600,  2605,  2615,
    2619,  2623,  2629,  2637,  2646,  2653,  2664,  2675,  2682,  2690,
    2695,  2703,  2711,  2718,  2727,  2731,  2737,  2742,  2749,  2753,
    2757,  2763,  2767,  2771,  2775,  2779,  2783,  2790,  2794,  2799,
    2809,  2821,  2827,  2834,  2844,  2848,  2853,  2857,  2861,  2870,
    2875,  2870,  2895,  2899,  2903,  2908,  2913,  2908,  2926,  2930,
    2937,  2943,  2948,  2953,  2953,  2968,  2973,  2978,  2983,  2988,
    2992,  2996,  3000,  3005,  3010,  3014,  3018,  3024,  3028,  3033,
    3037,  3042,  3046,  3051,  3061,  3067,  3073,  3078,  3083,  3088,
    3093,  3098,  3103,  3108,  3113,  3118,  3128,  3133,  3138,  3143,
    3149,  3156,  3165,  3169,  3176,  3176,  3176,  3179,  3179,  3183,
    3193,  3196,  3198,  3201,  3206,  3209,  3214,  3218,  3224,  3224,
    3232,  3232,  3234,  3241,  3244,  3249,  3252,  3257,  3264,  3270,
    3274,  3280,  3285,  3292,  3296,  3300,  3304,  3308,  3312,  3316,
    3320,  3324,  3328,  3332,  3336,  3340,  3344,  3348,  3352,  3356,
    3360,  3364,  3368,  3372,  3376,  3380,  3386,  3390,  3394,  3398,
    3404,  3417,  3421,  3425,  3431,  3436,  3443,  3448,  3455,  3459,
    3465,  3469,  3475,  3480,  3488,  3493,  3488,  3504,  3509,  3502
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "SYSTEM_IDENTIFIER",
  "STRING", "PATHPULSE_IDENTIFIER", "BASED_NUMBER", "DEC_NUMBER",
  "REALTIME", "VLOG_LE", "VLOG_GE", "VLOG_EG", "VLOG_EQ", "VLOG_NE",
  "VLOG_CEQ", "VLOG_CNE", "VLOG_LS", "VLOG_RS", "VLOG_RSS", "VLOG_SG",
  "VLOG_PO_POS", "VLOG_PO_NEG", "VLOG_PSTAR", "VLOG_STARP",
  "DEFAULT_NETTYPE", "VLOG_LOR", "VLOG_LAND", "VLOG_NAND", "VLOG_NOR",
  "VLOG_NXOR", "VLOG_TRIGGER", "VLOG_POW", "VLOG_always", "VLOG_and",
  "VLOG_assign", "VLOG_begin", "VLOG_buf", "VLOG_bufif0", "VLOG_bufif1",
  "VLOG_case", "VLOG_casex", "VLOG_casez", "VLOG_cmos", "VLOG_deassign",
  "VLOG_default", "VLOG_defparam", "VLOG_disable", "VLOG_edge",
  "VLOG_else", "VLOG_end", "VLOG_endcase", "VLOG_endfunction",
  "VLOG_endmodule", "VLOG_endprimitive", "VLOG_endspecify",
  "VLOG_endtable", "VLOG_endtask", "VLOG_event", "VLOG_for",
  "VLOG_automatic", "VLOG_force", "VLOG_forever", "VLOG_fork",
  "VLOG_function", "VLOG_highz0", "VLOG_highz1", "VLOG_if", "VLOG_initial",
  "VLOG_inout", "VLOG_input", "VLOG_integer", "VLOG_join", "VLOG_large",
  "VLOG_localparam", "VLOG_macromodule", "VLOG_medium", "VLOG_module",
  "VLOG_nand", "VLOG_negedge", "VLOG_nmos", "VLOG_nor", "VLOG_not",
  "VLOG_notif0", "VLOG_notif1", "VLOG_or", "VLOG_output", "VLOG_parameter",
  "VLOG_pmos", "VLOG_posedge", "VLOG_primitive", "VLOG_pull0",
  "VLOG_pull1", "VLOG_pulldown", "VLOG_pullup", "VLOG_rcmos", "VLOG_real",
  "VLOG_realtime", "VLOG_reg", "VLOG_release", "VLOG_repeat", "VLOG_rnmos",
  "VLOG_rpmos", "VLOG_rtran", "VLOG_rtranif0", "VLOG_rtranif1",
  "VLOG_scalared", "VLOG_signed", "VLOG_small", "VLOG_specify",
  "VLOG_specparam", "VLOG_strong0", "VLOG_strong1", "VLOG_supply0",
  "VLOG_supply1", "VLOG_table", "VLOG_task", "VLOG_time", "VLOG_tran",
  "VLOG_tranif0", "VLOG_tranif1", "VLOG_tri", "VLOG_tri0", "VLOG_tri1",
  "VLOG_triand", "VLOG_trior", "VLOG_trireg", "VLOG_vectored", "VLOG_wait",
  "VLOG_wand", "VLOG_weak0", "VLOG_weak1", "VLOG_while", "VLOG_wire",
  "VLOG_wor", "VLOG_xnor", "VLOG_xor", "VLOG_Shold", "VLOG_Speriod",
  "VLOG_Srecovery", "VLOG_Srecrem", "VLOG_Ssetup", "VLOG_Swidth",
  "VLOG_Ssetuphold", "VVLOG_attribute", "VLOG_Sremoval", "VLOG_Sskew",
  "VLOG_Stimeskew", "VLOG_Sfullskew", "VLOG_Snochange", "VLOG_TAND",
  "VLOG_generate", "VLOG_endgenerate", "VLOG_genvar", "'?'", "':'", "'|'",
  "'^'", "'&'", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UNARY_PREC", "less_than_VLOG_else", "','", "'='", "';'", "'('", "')'",
  "'#'", "'@'", "'~'", "'!'", "'['", "']'", "'{'", "'}'", "'.'", "'0'",
  "'1'", "'x'", "'b'", "'f'", "'F'", "'l'", "'h'", "'B'", "'r'", "'R'",
  "'M'", "'n'", "'N'", "'p'", "'P'", "'Q'", "'q'", "'_'", "$accept",
  "main", "source_file", "number", "attribute_list_opt", "attribute_list",
  "attribute", "block_item_decl", "block_item_decls",
  "block_item_decls_opt", "case_item", "case_items", "charge_strength",
  "charge_strength_opt", "defparam_assign", "defparam_assign_list",
  "delay1", "delay3", "delay3_opt", "delay_value", "delay_value_simple",
  "description", "drive_strength", "drive_strength_opt", "dr_strength0",
  "dr_strength1", "event_control", "event_expression_list",
  "event_expression", "expression", "expression_list", "expr_primary",
  "task_func_in_port_decl", "task_out_port_decl", "task_inout_port_decl",
  "func_input_decls", "function_item", "function_item_list",
  "gate_instance", "gate_instance_list", "gatetype", "pullupdown_gate",
  "identifier", "list_of_identifiers", "udp_list_of_identifiers",
  "dimension", "dimension_opt", "dimension_select", "list_of_ports",
  "list_of_port_declarations", "port_declaration", "net_type_opt",
  "signed_opt", "lavalue", "lpvalue", "assign", "assign_list", "module",
  "$@1", "$@2", "$@3", "$@4", "module_start", "range_delay",
  "module_port_list_opt", "module_parameter_port_list_opt",
  "module_parameter_port_list", "module_item", "$@5",
  "module_or_generate_item", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11",
  "$@12", "generated_instantiation", "$@13", "$@14",
  "generate_item_list_opt", "generate_item_list", "generate_decl_item",
  "generate_item_opt", "generate_item", "generate_if_clause",
  "generate_conditional_statement", "$@15", "generate_case_statement",
  "$@16", "genvar_declaration", "genvar_case_item_list",
  "genvar_case_item", "generate_loop_statement", "$@17",
  "genvar_identifier", "generate_block_identifier", "genvar_assign",
  "generate_block", "$@18", "$@19", "module_item_list",
  "module_item_list_opt", "net_decl_assign", "net_decl_assigns",
  "net_type", "var_type", "parameter_assign", "parameter_assign_decl",
  "parameter_assign_list", "localparam_assign", "localparam_assign_decl",
  "localparam_assign_list", "parameter_value_opt",
  "parameter_value_byname", "parameter_value_byname_list", "port",
  "port_opt", "port_reference", "port_reference_list", "port_name",
  "port_name_list", "port_type", "range", "range_opt",
  "function_range_or_type_opt", "task_func_param_type",
  "register_variable", "register_variable_list", "specify_item",
  "specify_delay_value_list", "specify_item_list",
  "specify_edge_path_decl", "specify_edge_path", "polarity_operator",
  "specify_simple_path_decl", "specify_simple_path",
  "specify_path_identifiers", "specparam", "specparam_list",
  "spec_polarity", "spec_reference_event", "spec_notifier_opt",
  "spec_notifier", "statement", "$@20", "$@21", "$@22", "$@23", "$@24",
  "statement_list", "statement_opt", "task_port_decl",
  "task_port_decl_list", "task_item", "task_item_list",
  "task_item_list_opt", "auto_tf_opt", "udp_body", "$@25",
  "udp_entry_list", "udp_comb_entry", "udp_comb_entry_list",
  "udp_sequ_entry_list", "udp_sequ_entry", "udp_initial", "udp_init_opt",
  "udp_input_list", "udp_input_sym", "udp_output_sym", "udp_port_decl",
  "udp_port_decls", "udp_port_list", "reg_opt", "udp_initial_expr_opt",
  "udp_input_declaration_list", "udp_primitive", "$@26", "$@27", "$@28",
  "$@29", 0
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
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,    63,    58,   124,    94,    38,    60,
      62,    43,    45,    42,    47,    37,   409,   410,    44,    61,
      59,    40,    41,    35,    64,   126,    33,    91,    93,   123,
     125,    46,    48,    49,   120,    98,   102,    70,   108,   104,
      66,   114,    82,    77,   110,    78,   112,    80,    81,   113,
      95
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   201,   202,   202,   203,   203,   204,   204,   204,   205,
     205,   205,   206,   206,   207,   207,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   209,   209,   210,   210,   211,   211,   211,   211,
     212,   212,   213,   213,   213,   214,   214,   215,   216,   216,
     216,   217,   217,   218,   218,   218,   218,   219,   219,   220,
     220,   221,   221,   221,   222,   222,   222,   222,   223,   223,
     223,   223,   223,   223,   224,   224,   225,   225,   225,   225,
     226,   226,   226,   226,   227,   227,   227,   228,   228,   228,
     229,   229,   229,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   231,   231,   231,   231,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   233,
     233,   234,   234,   235,   235,   236,   236,   236,   237,   237,
     238,   238,   239,   239,   239,   239,   239,   240,   240,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   242,   242,   243,   243,   244,   244,   245,
     245,   246,   246,   247,   247,   248,   248,   249,   249,   250,
     250,   250,   251,   251,   251,   251,   251,   252,   252,   253,
     253,   254,   254,   254,   254,   254,   255,   255,   255,   255,
     255,   256,   257,   257,   259,   260,   261,   262,   258,   263,
     263,   264,   265,   265,   265,   266,   266,   267,   267,   267,
     268,   268,   268,   268,   268,   268,   268,   268,   268,   268,
     268,   268,   268,   269,   268,   268,   270,   270,   270,   270,
     270,   270,   270,   270,   270,   271,   270,   270,   270,   270,
     272,   273,   270,   274,   275,   270,   276,   270,   277,   270,
     270,   270,   270,   270,   270,   270,   270,   279,   280,   278,
     281,   281,   282,   282,   283,   283,   283,   283,   283,   284,
     284,   285,   285,   285,   285,   285,   286,   287,   288,   287,
     287,   290,   289,   291,   292,   292,   293,   293,   293,   295,
     294,   296,   297,   298,   300,   299,   301,   299,   302,   302,
     303,   303,   304,   305,   305,   306,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   307,   308,   309,   309,   309,
     309,   310,   310,   311,   312,   312,   312,   312,   313,   313,
     314,   314,   314,   314,   314,   315,   315,   316,   316,   317,
     317,   317,   317,   318,   318,   319,   319,   319,   319,   320,
     320,   321,   321,   321,   322,   322,   323,   323,   323,   324,
     325,   325,   326,   326,   326,   326,   326,   326,   327,   327,
     327,   327,   328,   328,   328,   329,   329,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   331,   331,   332,   332,   333,
     333,   334,   334,   334,   334,   334,   334,   334,   334,   335,
     335,   335,   336,   336,   336,   337,   337,   337,   338,   338,
     338,   338,   339,   339,   339,   339,   340,   340,   341,   341,
     341,   342,   342,   342,   342,   342,   342,   343,   343,   344,
     344,   344,   344,   344,   345,   345,   345,   345,   345,   346,
     347,   345,   345,   345,   345,   348,   349,   345,   345,   345,
     345,   345,   345,   350,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     351,   351,   352,   352,   353,   353,   353,   354,   354,   354,
     355,   355,   356,   356,   357,   357,   358,   358,   360,   359,
     361,   361,   362,   363,   363,   364,   364,   365,   366,   367,
     367,   368,   368,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   369,
     369,   369,   369,   369,   369,   369,   370,   370,   370,   370,
     371,   371,   371,   371,   372,   372,   373,   373,   374,   374,
     375,   375,   376,   376,   378,   379,   377,   380,   381,   377
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     2,     1,     1,     2,     3,
       2,     0,     3,     1,     1,     3,     6,     5,     4,     3,
       3,     3,     3,     3,     3,     4,     4,     3,     3,     3,
       3,     3,     1,     2,     1,     0,     3,     3,     2,     3,
       2,     1,     3,     3,     3,     1,     0,     3,     1,     2,
       3,     2,     4,     2,     4,     6,     8,     1,     0,     1,
       5,     1,     1,     1,     1,     1,     2,     8,     5,     5,
       5,     5,     5,     5,     1,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     4,     4,     1,     3,     3,
       2,     2,     1,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     5,     3,     1,     0,     2,     1,     1,     1,     1,
       1,     2,     7,     6,     4,     4,     3,     3,     6,     6,
       4,     6,     4,     6,     4,     1,     3,     4,     2,     1,
       1,     2,     4,     5,     3,     2,     4,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     2,     4,     1,
       3,     1,     2,     1,     0,     3,     4,     1,     3,     1,
       3,     3,     7,     7,     7,     7,     8,     1,     0,     1,
       0,     1,     2,     7,     6,     3,     1,     2,     7,     6,
       3,     3,     3,     1,     0,     0,     0,     0,    12,     1,
       1,     2,     3,     3,     0,     4,     0,     3,     3,     4,
       6,     6,     6,     5,     5,     6,     6,     1,     2,     6,
       6,     5,     2,     0,     4,     3,     1,     3,     4,     5,
       5,     6,     3,     6,     6,     0,     5,     5,     3,     3,
       0,     0,     9,     0,     0,    12,     0,    10,     0,    13,
       1,     2,     5,     3,     3,     9,     5,     0,     0,     5,
       1,     0,     1,     2,     6,     6,     6,     5,     1,     1,
       0,     1,     1,     1,     1,     1,     4,     2,     0,     5,
       2,     0,     7,     3,     1,     2,     3,     3,     2,     0,
      10,     1,     1,     3,     0,     6,     0,     4,     2,     1,
       1,     0,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     2,     3,
       2,     1,     3,     3,     1,     2,     3,     2,     1,     3,
       4,     4,     2,     2,     0,     5,     4,     1,     3,     1,
       5,     3,     7,     1,     0,     1,     6,     4,     4,     1,
       3,     5,     5,     4,     3,     1,     1,     1,     1,     5,
       1,     0,     1,     1,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     3,     2,     1,     3,     3,     2,     2,
       6,     6,    10,     8,    10,    10,    12,    12,    10,     7,
       7,     7,     7,     7,     7,     1,     3,     1,     2,     5,
       3,     7,    11,     7,    11,     7,    11,     7,    11,     1,
       1,     1,     5,     3,     5,     6,     6,     3,     1,     2,
       3,     4,     3,     7,     3,     7,     1,     3,     1,     1,
       0,     2,     2,     4,     4,     3,     1,     0,     1,     1,
       2,     2,     3,     1,     5,     3,     5,     3,     3,     0,
       0,     8,     2,     4,     3,     0,     0,     8,     2,     4,
       3,     3,     2,     0,     4,     5,     6,     6,     6,     6,
       6,     6,     5,     7,     5,     7,    13,    11,    13,     5,
       5,     5,     2,     2,     3,     5,     4,     4,     5,     5,
       5,     9,     5,     9,     5,     5,     2,     5,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       1,     2,     2,     1,     1,     0,     1,     0,     0,     4,
       1,     1,     4,     1,     2,     1,     2,     6,     5,     1,
       0,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     4,     1,     2,     1,     3,     1,     0,
       2,     0,     2,     4,     0,     0,    12,     0,     0,    15
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
      11,     0,     0,     0,     0,     0,    11,     0,     4,    64,
      65,    14,    10,     0,    13,   348,   352,   346,   351,   347,
     350,   354,   349,   345,   353,    66,     0,     0,     1,     5,
     240,   239,     0,     0,     9,     0,     0,     0,   234,   195,
     140,   138,     6,     7,   137,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   134,   136,    15,    93,
     139,    12,   606,   609,     0,     0,   246,   134,     8,   101,
     102,   103,    99,   105,   100,    97,    94,    95,     0,    96,
     104,    98,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     134,     0,     0,   141,   608,     0,     0,     0,     0,     0,
     235,   133,     0,   146,   134,   135,   147,   125,   126,   123,
     127,   124,   128,   120,   121,   122,   129,   130,   115,   116,
     117,   112,     0,   114,   106,   113,   118,   119,   110,   111,
     107,   108,   109,     0,     0,   196,     0,   611,   607,     0,
       0,     0,   244,   145,     0,   132,     0,   144,   449,   450,
     451,   205,     0,     0,     0,     0,     0,     0,     0,   604,
     614,     0,   407,     0,    11,     0,     0,   131,     0,   206,
       0,   610,     0,   199,     0,     0,     0,     0,   605,   570,
      67,   403,   404,   405,   406,     0,   402,     0,     0,   245,
     385,     0,     0,     0,     0,     0,   209,   383,   207,   379,
     236,   148,   143,     0,     0,     0,     0,   600,   601,   602,
       0,     0,   569,   615,     0,     0,   247,     0,   248,     0,
     389,     0,     0,   218,   218,   218,   384,   242,    11,   243,
       0,   142,   612,     0,     0,   200,   603,     0,     0,     0,
       0,   249,     0,     0,     0,   381,     0,   220,   217,   220,
     355,   220,   220,   208,   211,   210,     0,   275,     0,     0,
       0,     0,   398,   396,     0,   397,     0,   194,   193,     0,
       0,     0,   557,     0,    46,     0,     0,     0,   266,     0,
     339,   257,   290,     0,     0,   220,     0,   617,     0,   558,
       0,     0,   356,   388,     0,   387,   390,     0,     0,   219,
     401,   401,   401,   401,   291,   374,     0,     0,    74,    58,
      48,     0,     0,     0,   204,     0,     0,   556,   220,   220,
     220,     0,     0,   408,   409,   410,     0,   411,   368,     0,
     364,     0,     0,   220,     0,     0,   361,     0,   357,     0,
       0,     0,     0,     0,     0,     0,   262,     0,     0,     0,
     412,   415,     0,     0,    45,   401,     0,     0,     0,   169,
     175,   176,   177,   185,     0,     0,   170,   181,   172,   178,
     179,   180,   171,   183,   186,     0,   182,   184,   188,   191,
     192,   187,   189,   190,   174,   173,   297,     0,   258,   220,
       0,   134,   168,     0,   338,   237,   401,   220,   613,     0,
       0,     0,   616,   399,     0,     0,   380,   400,     0,     0,
       0,     0,     0,     0,     0,   293,     0,     0,    78,    82,
      77,    81,    76,    80,    79,    83,     0,     0,     0,    57,
       0,     0,   267,     0,    49,   203,   197,   201,     0,    24,
     294,   407,   401,   401,    31,     0,     0,    23,     0,   365,
     367,   401,    30,     0,    22,     0,   358,   360,    28,    20,
      29,    21,   265,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,     0,
       0,     0,     0,     0,   283,    27,     0,   414,     0,    19,
       0,     0,     0,     0,    58,     0,     0,   323,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   503,     0,     0,     0,     0,     0,     0,     0,   134,
       0,     0,   226,     0,   278,   279,     0,     0,     0,     0,
       0,   134,     0,    74,     0,     0,   401,   134,   165,     0,
       0,     0,     0,   272,   238,     0,   401,     0,   568,   576,
     595,   578,   579,   573,   574,   575,   577,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   594,     0,   563,   560,   561,   565,     0,   571,   386,
       0,   204,   204,   204,   204,   373,   372,   134,   134,     0,
       0,     0,     0,     0,     0,    63,    61,    62,     0,    53,
     134,   221,     0,   233,     0,    50,    47,   202,   204,     0,
       0,     0,   363,   366,   369,     0,   359,   362,     0,     0,
       0,   466,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   458,     0,     0,   470,
     264,   438,   419,     0,   418,     0,   280,     0,   413,   416,
      44,    43,    42,     0,   241,     0,     0,   539,   536,   134,
       0,     0,     0,   492,     0,   541,     0,     0,     0,     0,
       0,     0,     0,     0,   502,   498,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,    84,     0,   543,
     542,   522,   523,   538,   134,     0,   227,     0,     0,    26,
      18,    25,     0,     0,   336,     0,     0,     0,    46,     0,
     308,   298,     0,   315,   302,     0,   312,   311,   313,   314,
       0,     0,     0,   268,     0,     0,     0,     0,   395,     0,
     134,     0,     0,   164,   167,     0,     0,     0,   618,   559,
     564,     0,   566,     0,     0,   572,   382,   213,   212,   214,
       0,   215,     0,     0,   377,     0,   276,   292,     0,     0,
       0,     0,     0,     0,     0,    59,     0,     0,   222,     0,
       0,   277,   198,   288,     0,     0,     0,     0,     0,     0,
       0,   417,     0,     0,   476,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   457,     0,   459,
     470,   470,   468,   469,     0,     0,     0,   440,     0,   453,
     555,     0,   253,   296,     0,     0,   501,     0,   494,   489,
     488,   540,     0,     0,     0,   485,   500,     0,   226,     0,
       0,   495,     0,     0,     0,   487,     0,     0,     0,     0,
       0,   524,     0,     0,     0,     0,     0,    87,    92,   230,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    17,     0,     0,     0,     0,     0,   401,
     220,     0,   303,   320,   317,   269,   270,     0,     0,   344,
       0,   204,     0,     0,     0,   162,     0,   166,     0,     0,
       0,   261,   254,     0,   619,     0,     0,   599,   573,   574,
     575,     0,     0,   216,     0,   370,     0,   371,    73,    72,
      70,    68,    71,    69,     0,    54,     0,   225,     0,     0,
     231,   232,   286,     0,   260,   259,   256,     0,   462,     0,
     464,   467,   472,    93,   471,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   460,     0,     0,   435,     0,     0,     0,
     407,   407,   407,     0,   550,   544,   545,   546,   547,     0,
     553,   554,     0,     0,     0,     0,     0,   493,    35,     0,
       0,     0,     0,     0,     0,   499,    35,   504,     0,     0,
       0,     0,     0,     0,    52,    86,    91,    90,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,   527,
       0,     0,     0,   526,    16,   332,   334,     0,     0,   331,
       0,     0,     0,     0,   401,   299,   318,   271,     0,     0,
     252,   250,   251,     0,   394,   163,   274,   273,   255,   596,
     597,   598,     0,   562,     0,   378,     0,     0,     0,     0,
      11,     0,     0,     0,     0,     0,     0,     0,   475,     0,
     477,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   461,     0,     0,     0,
     439,   454,   452,   220,     0,     0,   220,   220,     0,     0,
     551,   552,   281,     0,     0,   535,   484,    32,    34,     0,
       0,     0,    41,     0,     0,     0,     0,     0,     0,   519,
       0,   486,     0,   514,   512,   505,   534,   521,   520,   525,
      88,    89,   537,     0,     0,     0,   529,   532,     0,   528,
     530,     0,   337,   321,     0,     0,   316,     0,     0,     0,
       0,   342,   343,     0,   393,     0,     0,   376,     0,     0,
      55,     0,     0,     0,   159,   155,     0,   160,     0,     0,
     421,   420,     0,     0,   474,   473,     0,   483,   479,     0,
     478,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     455,   456,   436,   401,   204,   204,   401,   401,   204,   204,
     548,     0,    35,     0,    33,     0,   509,     0,     0,    38,
       0,   506,    40,     0,   510,   507,   511,   508,     0,     0,
       0,     0,   229,     0,     0,     0,     0,   134,   333,     0,
     307,     0,     0,     0,   319,   392,   391,   567,   375,     0,
      60,   224,     0,     0,   158,   161,     0,     0,     0,     0,
     477,   480,     0,   481,   477,     0,   477,   477,   429,     0,
     430,   431,   432,   433,   434,   445,     0,   447,     0,   441,
       0,   443,     0,     0,   154,   150,     0,     0,   152,   549,
     282,     0,   295,     0,    39,    37,    36,     0,     0,   496,
     515,   513,   228,     0,     0,   335,     0,     0,   134,   324,
       0,   306,   304,   305,    56,   223,   204,   156,   287,    35,
     463,   465,     0,   423,   482,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   204,   204,   204,   284,   491,     0,
       0,   497,     0,     0,     0,   328,     0,   322,   325,     0,
     157,     0,     0,     0,   477,     0,     0,   477,     0,     0,
       0,     0,   153,   149,   151,     0,     0,     0,     0,   533,
     531,   327,   326,   309,   329,     0,   422,   424,     0,   425,
     428,     0,     0,     0,     0,     0,   285,     0,     0,     0,
       0,   289,     0,     0,   446,   448,   442,   444,     0,   517,
       0,   330,   427,   426,     0,     0,   518,   516
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,    57,   729,    13,    14,   298,  1118,  1119,
    1122,  1123,   374,   375,   330,   331,   540,   449,   450,   976,
     619,     8,   328,   329,   446,   447,   541,   866,   867,   121,
    1124,    59,   985,   986,   987,  1176,  1177,  1178,   412,   413,
     407,   299,    60,   335,   194,   455,   456,   716,   214,   215,
     216,   267,   320,   622,   543,   623,   624,     9,    66,   162,
     250,   564,    32,   513,   185,   120,   183,   300,   367,   730,
     325,   830,  1221,   667,  1375,  1070,   943,   408,   550,   891,
     731,   732,   733,  1382,   734,   735,   736,  1160,   737,  1247,
     302,  1318,  1319,   738,  1400,  1040,  1036,  1041,   739,  1151,
     885,   303,   304,   899,   900,   268,   272,   356,   357,   358,
     348,   349,   350,   433,   774,   775,   217,   218,   219,   241,
     748,   749,   305,   427,   514,   207,   352,   371,   372,   498,
     977,   499,   500,   501,   172,   502,   503,   659,   641,   642,
     825,   805,  1189,  1190,   710,   998,  1303,  1006,  1351,   697,
     686,   711,   988,   989,   990,   991,   992,   338,   310,   421,
     592,   593,   594,   595,   596,   232,   233,   597,   598,   922,
     179,   180,    64,   115,   175,   225,    10,   199,   258,   419,
     914
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -987
static const yytype_int16 yypact[] =
{
     415,   163,  1406,    87,   -64,   176,   439,   277,  -987,  -987,
    -987,   -29,  -987,    81,  -987,  -987,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  -987,  -987,  -987,   108,   249,  -987,  -987,
    -987,  -987,   322,  2864,  -987,   356,    83,   220,  -987,  -987,
     210,  -987,  -987,   388,  -987,   507,   507,   507,   507,   204,
     507,   507,   507,  2864,   507,   213,  2864,  -987,  5625,  -987,
     276,  -987,  -987,   333,   156,   446,   301,  2864,  -987,  -987,
    -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  3708,  -987,
    -987,  -987,  3062,   154,  2864,  2864,  2864,  2864,  2864,  2864,
    2864,  2864,  2864,  2864,  2864,  2864,  2864,  2864,  2864,  2864,
    2864,  2864,  2864,  2864,  2864,  2864,  2864,  2864,  2864,  2864,
    2864,  2864,   488,   324,  -987,   525,   532,   409,   432,   449,
    -987,  5625,   447,  -987,  2864,  2864,  -987,   392,   392,  1267,
    1267,  1267,  1267,   206,   206,   206,  5781,  5804,  1344,  5850,
    5850,  5625,  5354,  5827,  5850,  1344,   392,   392,    14,    14,
     599,   599,   599,   461,  3088,  -987,  2864,   482,  -987,   307,
     639,   593,   491,  -987,   192,  5625,  2864,  -987,  -987,  -987,
    -987,  -987,  2864,  3113,  2864,   521,   704,   735,   153,  -987,
     307,   548,   268,   478,    79,   597,   609,  5625,  3161,  -987,
    2864,  5625,   716,  -987,   234,   627,   643,   823,  -987,   765,
    -987,  -987,  -987,  -987,  -987,  2864,  -987,   835,   177,  -987,
     664,   845,   854,   505,   503,   519,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  3317,   878,   546,   889,  -987,  -987,  -987,
     726,   897,  -987,  -987,  5510,   761,  -987,   835,  -987,   451,
    -987,   298,   756,  1406,  1406,  5921,   107,  -987,   315,  -987,
    6101,  -987,  -987,   875,   784,  -987,  -987,   800,   861,  2864,
    2864,  -987,   807,  3138,   845,  -987,    36,   898,  -987,   898,
    -987,   898,   898,  -987,  -987,  -987,   818,  -987,   105,    27,
    1001,   316,   921,   921,   499,   921,   540,  -987,  -987,   587,
     671,  2773,   967,   678,   879,   885,  1029,  7094,  -987,    32,
    -987,  -987,  -987,  6228,  1004,   900,  1061,  -987,   299,  -987,
    1011,  3340,  5625,  -987,  2864,  -987,  -987,   845,   895,  -987,
     894,   894,   894,   894,  -987,   896,   603,   987,  -987,   899,
    -987,   520,   267,  1067,   894,   553,  1021,  -987,   898,   898,
     898,   904,   913,  -987,  -987,  -987,   894,  -987,  -987,   914,
     915,  1085,  1085,   898,   920,   894,  -987,   923,   926,   835,
     835,   925,   560,   927,   630,  1041,  -987,  1966,  1099,   935,
     248,  -987,   648,   236,  -987,   894,   833,   939,  1913,  -987,
    -987,  -987,  -987,  -987,  1913,   855,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  -987,  -987,   117,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  -987,  -987,  -987,  -987,    88,  -987,   898,
     290,  2641,  -987,   698,  -987,  -987,   894,   898,  -987,   861,
     940,  7315,  -987,  -987,  3363,   314,  -987,  -987,  1111,  1117,
    1121,  1123,   120,   109,  2864,  -987,   960,   962,  -987,  -987,
    -987,  -987,  -987,  -987,  -987,  -987,   965,   966,   122,  -987,
      46,  1067,  -987,  2864,  -987,   894,  -987,  -987,  1134,  -987,
    -987,   268,   894,   894,  -987,  2864,  1085,  -987,  1085,   915,
     915,   894,  -987,   835,  -987,   835,   926,   926,  -987,  -987,
    -987,  -987,  -987,   968,   611,   975,   976,   977,   978,   980,
     981,   983,   984,   985,   986,   993,   994,   469,  -987,  2482,
     997,   999,  1000,  1002,  1005,  -987,  2864,   894,  1166,  -987,
    1006,  1007,  1008,  1001,   899,  1009,  1014,  -987,  1017,   674,
    1067,    46,  1198,  1003,  1018,  1019,    46,  1067,  1020,    46,
    1913,   231,  1022,    46,  1023,  1024,  1025,   129,    31,  2864,
    1415,  1415,   472,    28,  -987,  -987,  1027,   699,  1028,    86,
    6355,  2608,   109,   109,   899,   710,   422,  2325,  1032,  1016,
    1034,   554,   109,  -987,  -987,   883,   894,  1118,  -987,  -987,
    -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,
    -987,  -987,  1120,  -987,  7315,  7315,  -987,  5716,  -987,  -987,
    1035,   894,   894,   894,   353,  -987,  -987,  2341,  2864,   717,
    4762,   357,   547,   810,   829,  -987,  -987,  -987,  2864,  -987,
    2864,   558,  1031,  -987,   725,  -987,  5625,  -987,   894,  1189,
    1001,  1001,  5625,   915,  -987,  1001,   926,  -987,  2864,  1039,
    1040,  -987,   737,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,  1038,  1037,  1201,  1201,   473,
    -987,  -987,  -987,   142,  -987,   152,  -987,  1044,  5625,  -987,
    -987,  -987,  -987,   747,  -987,  1042,  1206,  -987,  -987,  2864,
     306,  1048,   115,  -987,  1217,  -987,  1711,  2864,  2864,  2864,
    1051,   325,    40,  1053,  -987,  -987,  1220,  1913,  2411,  1055,
    2864,  2864,  2506,  2864,  -987,  1415,  2053,  1047,   341,  -987,
    -987,  -987,  -987,  -987,  2864,  2864,  1049,  1392,  2785,  -987,
    -987,  -987,  1166,   751,  1075,  1060,  1065,  1066,   879,  7198,
    -987,  -987,  6482,  -987,  -987,  6863,  -987,  -987,  -987,  -987,
     758,   768,   109,  -987,  1229,  1238,  1240,   568,  -987,   575,
    2864,   109,   109,  -987,  -987,  1074,   778,  1001,  -987,  -987,
    -987,  7174,  -987,  7221,  7268,  -987,  -987,  -987,  -987,  -987,
    2864,  -987,  1243,   576,  -987,   577,  -987,  -987,  1077,  1079,
    1083,  1090,  1092,  1096,   578,  5533,   359,  2864,  1070,  2864,
      46,  -987,  -987,  1105,   792,   793,   804,  3868,  2864,  2895,
     611,  -987,  2864,  2864,  1126,  1114,  1115,  1122,  1124,  1125,
    1127,  1128,  1132,  1133,  1136,  1137,  1146,  -987,  2864,  1112,
     473,   473,  -987,  -987,  1285,   530,  2864,  -987,  2522,  -987,
     881,   517,  -987,  -987,  1150,   583,  -987,  2864,  -987,  1241,
    -987,  -987,  3891,  3914,  3937,  -987,  -987,  1130,   581,  1151,
    2864,  1222,  1759,  1147,  3960,  -987,  3983,  4143,  1149,  4166,
    1152,  -987,  1153,  2864,  2864,  1155,   235,  -987,  5625,  -987,
     585,  3088,  2864,  1157,   119,  2864,  2864,  4785,  1158,  2864,
    2864,  4808,   805,  -987,  1319,  6990,  2864,  1320,  2864,   894,
     898,  1179,  -987,   818,  1283,  -987,  -987,   812,  1164,  -987,
     813,   383,   821,   831,  1168,  -987,  1154,  -987,   606,   832,
     848,  -987,  -987,   858,  -987,   335,  7315,  -987,  1170,  1171,
    1172,  1188,  1175,  5625,  1177,  -987,  1169,  -987,  -987,  -987,
    -987,  -987,  -987,  -987,  2864,  -987,  2864,  -987,  3088,  2864,
    5625,  -987,  -987,  1178,  -987,  -987,  -987,  1180,  5556,  2864,
    5625,  -987,  5625,  1202,  5625,  1215,  2864,   237,  2864,   237,
     237,   237,  2864,   237,  2864,  2864,  2864,  2864,  2864,  3386,
    2864,   549,   590,  1037,  1201,  1201,  -987,   607,  1184,   612,
     427,   635,   475,    56,  -987,  -987,  -987,  -987,  -987,   868,
    -987,   881,  1415,   624,  1362,  1199,  4966,  -987,   801,  2090,
    2234,  2267,  1913,  2864,  4989,  -987,   801,  -987,  1415,  1415,
    1913,  1415,  1913,  1913,  -987,  -987,  5625,  5625,  1415,  2818,
    2818,  -987,  1200,  2864,  3113,  2864,  2297,  5012,  5035,  -987,
    2864,  5058,  5081,  -987,  -987,  -987,  -987,  6609,  4189,  -987,
    1204,  1205,  4212,  1001,   422,  -987,  -987,  -987,  2864,  1229,
    -987,  -987,  -987,  2313,  -987,  -987,  -987,  -987,  -987,  -987,
    -987,  -987,   335,  -987,  1308,  -987,   637,  5579,  2864,  3113,
     753,  1298,  1211,  1216,  2864,  3685,  2864,  2864,  5625,  1221,
     131,  1223,  1224,  1226,   642,  1230,  1231,  1235,  1236,  1237,
    1239,  3409,   133,   139,   140,   150,  1112,   647,   657,  2864,
    -987,  -987,  -987,   898,  1385,  1409,   898,   898,  1410,   360,
    -987,  -987,  -987,  1244,  1245,  -987,  -987,  -987,   801,  1913,
      26,  1343,  -987,   328,   175,   172,  1993,   186,  2023,  -987,
    5239,  -987,  1913,  1366,  1374,  -987,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  3432,  2864,  4235,  -987,  -987,  4258,  -987,
    -987,  6990,  -987,  -987,  2864,  2864,  -987,   869,  1229,  1238,
    6990,  5625,  -987,  1252,  -987,  4418,  1255,  -987,  4441,  2864,
    -987,  2864,  3588,  2864,  -987,  -987,   872,  -987,  1045,   660,
    -987,  -987,  5602,  2864,  5625,  5625,  2864,  -987,  1067,  1261,
    1266,  2864,  2864,  2864,  2864,  1265,  2864,  1268,  1270,  1271,
    1274,  1275,  1264,   507,  1276,   507,  1280,   507,  1281,   507,
    -987,  -987,  -987,   894,   894,   894,   894,   894,   894,   894,
    -987,  1380,   801,  1277,  -987,  1813,  -987,  1415,  1415,  -987,
    1299,  -987,  -987,  1415,  -987,  -987,  -987,  -987,  2555,  1863,
    1415,  1415,  -987,  3611,  1284,  1284,  6736,  2834,  5625,  5262,
    -987,   877,   892,   893,  -987,  -987,  -987,  -987,  -987,  1288,
    5625,  -987,  3634,   245,  -987,  -987,  1411,  1291,  2864,  4464,
     131,  1047,  1297,  1067,   131,  1300,   131,  3662,  -987,  1304,
    -987,  -987,  -987,  -987,  -987,  -987,   141,  -987,   141,  -987,
     141,  -987,   141,  1470,  -987,  -987,  1478,  1482,  -987,  -987,
    -987,  1415,  -987,  1440,  -987,  -987,  -987,  1321,  5285,  -987,
    -987,  -987,  -987,  2864,  2864,  -987,  5993,   223,  2699,  -987,
    1320,  -987,  -987,  -987,  -987,  -987,   894,  -987,  -987,   801,
    5625,  -987,  1323,  -987,  1047,  1324,  2864,  1325,  1327,  2864,
    2864,  2864,  2864,  2864,   894,   894,   894,  -987,  -987,    77,
     110,  -987,  5308,  5331,  6990,  -987,  1569,  -987,  -987,  1338,
    -987,  1913,  1341,  1342,   131,  1348,  1351,   131,  4487,  4510,
    4533,  4693,  -987,  -987,  -987,  1436,  1354,  1352,  1356,  -987,
    -987,  -987,  -987,  -987,  -987,  1474,  -987,  -987,  1360,  -987,
    -987,  1361,  1364,  1365,  1369,  1370,  -987,  2864,  1913,  2864,
    6990,  -987,  1368,  1375,  -987,  -987,  -987,  -987,  4716,  -987,
    4739,  -987,  -987,  -987,  1913,  1913,  -987,  -987
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -987,  -987,  -987,  1247,     5,  -987,  1509,  -816,  -987,  -979,
    -560,  -193,  -987,   824,  -169,  -987,   143,  -363,  1043,  -568,
    -495,  1540,  -404,  1144,  -380,  -378,  -708,  -987,  -146,   -33,
     -31,    49,  -986,  -987,  -987,   485,   380,  -987,   998,  -324,
    -987,  -987,   261,  -282,  -987,  1191,  -455,   -59,  -987,  -987,
    1311,   666,  -224,  -107,  -686,   774,  -987,  -987,  -987,  -987,
    -987,  -987,  -987,  -412,  -987,  -987,  -987,  1272,  -987,    37,
    -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,  -987,
    -987,  -859,  -987,  -987,  -695,  -987,  -987,  -987,  -987,  -987,
    -987,  -987,   251,  -987,  -987,  -987,  -987,   259,  -987,  -987,
    -987,  -987,  -987,   531,  -724,     0,   282,  -120,  -987,  -159,
    1113,  -987,  -168,  -987,   656,  -987,  -987,  1337,   281,  1269,
     681,  -987,  -987,  -160,  -304,  -448,  1305,  1076,  -361,  1091,
     767,  -987,   646,  -987,  -161,   649,  -987,  -629,   816,  -987,
     144,  -379,  -561,  -987,  -148,  -987,  -987,  -987,  -987,  -987,
    -682,  -284,   492,   769,   622,  -987,  -987,  1326,  1207,  -987,
    -987,  1030,  -987,  -987,  1026,  -987,  -987,   400,  -565,   555,
    1439,  -987,  -987,   644,  -987,  -987,  -987,  -987,  -987,  -987,
    -987
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -610
static const yytype_int16 yytable[] =
{
      58,   113,    25,   553,   565,     7,   849,   362,   364,   876,
     880,     7,   190,   629,   984,   852,   428,   429,   430,   431,
      78,   903,   206,    82,   547,    83,  1037,  1132,   820,   821,
      39,   559,   765,   560,    39,   410,   122,   892,   717,   210,
     894,   847,   704,    39,   552,   321,    98,   322,   323,    39,
     784,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,  1226,   154,   153,
      39,   416,   210,   555,  1175,  1175,    62,   236,   238,   370,
      26,   410,   165,   164,    69,    70,    71,    72,    74,    75,
      76,    77,     1,    79,    81,    34,   326,    27,   -75,   609,
     210,  1377,   410,    39,   461,   462,   463,   261,   548,   333,
    -220,   605,    39,   173,   351,   615,   359,   385,   606,   471,
     616,   617,   615,   187,  1187,   860,  1202,   616,   617,   188,
      33,   191,  1204,  1206,   745,   615,   767,   768,   769,   771,
     616,   617,   744,  1208,   395,   615,   196,   223,   630,   631,
     616,   617,   168,   169,   454,   838,    11,   635,   827,    63,
     829,   549,   234,   792,   457,   984,    28,   107,   108,   109,
     235,  1227,  1117,   469,   470,   556,   466,    12,   723,   213,
    1117,   742,  1175,   566,   705,   473,   765,   718,   765,   921,
     476,   477,   706,   411,   205,    73,   263,    39,    40,    41,
     457,    42,    43,    44,    80,   317,    39,    40,    41,   539,
      42,    43,    44,  1234,   319,   620,   311,   312,   740,   741,
     544,   673,   779,   778,   783,   781,   545,  1236,    98,   197,
      39,    40,    41,  1301,    42,    43,    44,  -384,  1326,    35,
     558,  -384,    37,   213,  1174,   297,   539,   712,   211,   551,
     212,   448,   757,   205,   237,   806,   807,   808,   809,   810,
     811,   812,   813,   814,   815,   816,   327,  1327,   -75,    36,
     608,   424,   625,   756,   -75,   677,   211,   301,   212,   539,
    1026,   607,  1246,   618,  -220,   627,   170,   409,   633,  1188,
     703,   206,  1224,   695,  1203,   417,    42,    43,   297,   510,
    1205,  1207,   511,   826,   636,   981,   802,   336,   274,  -557,
    1019,  1209,   125,   828,   116,    38,   803,  1227,   117,  1230,
    1233,    39,    40,    41,   126,    42,    43,    44,     1,   201,
     301,  1227,   892,   125,   512,  1097,  1098,   627,   794,   795,
    1361,   921,    30,   796,    31,   637,    45,    46,    47,    11,
     125,   882,  1174,  1219,   202,   203,  1066,   105,   106,   107,
     108,   109,   186,  1121,   685,    53,   337,   176,  1356,  1231,
     561,    67,   694,    56,    53,   204,   696,  -557,    65,   722,
    1080,   125,    56,   177,  1084,    68,  1086,  1087,  1088,  1089,
    1090,   610,   226,  1020,   227,   178,  1117,  1021,    53,    90,
      91,    92,  -557,  -557,   681,    -3,    56,   506,   897,   690,
     626,   861,   693,  -557,    98,   205,   699,   909,   910,   980,
     981,   114,   632,  -557,  1251,  1253,   453,  1225,     1,    -2,
       2,   457,   457,   457,   457,   205,   982,   110,   112,   439,
    1239,   118,   262,   111,    39,    40,    41,   112,    42,    43,
      44,   557,     1,   902,     2,  1254,   264,   205,   457,   441,
     655,   443,   656,   668,   119,   913,   836,  1043,   265,    45,
      46,    47,   264,  -134,    48,    49,    50,   112,   445,    51,
      52,   155,   240,  -557,   600,   846,  -134,   917,   201,    53,
     341,   156,   342,    54,    55,     3,   112,    56,   708,   125,
      39,    40,    41,  1117,    42,    43,    44,  1059,  1060,  1061,
     561,   869,   770,   202,   203,  1103,   747,   125,   157,     3,
     205,  1212,  1104,  1105,  1108,   158,  1313,  1314,   841,   937,
     332,   354,   974,   235,   204,   316,   201,   318,   657,   685,
     975,   892,  1048,   105,   106,   107,   108,   109,   658,     4,
     205,  1092,   788,  1232,   339,   340,  1232,   353,  1232,  1093,
     343,   202,   203,  1107,   243,   244,   773,   561,  1079,   159,
    1081,  1082,  1083,     4,  1085,   785,   980,   981,   361,   786,
     334,   245,   204,   327,   332,   344,   345,   819,   240,   205,
     160,  1259,  1094,   982,   205,   797,   346,    48,    49,    50,
    1095,   343,    51,    52,   639,   125,   347,   640,  1270,   163,
     161,  1355,    53,  1274,  1275,  1276,    54,    55,  1279,   125,
      56,    98,  1159,   167,   822,   823,   344,   345,   438,   542,
    1158,   824,   713,   714,   181,   542,   208,   355,   835,   715,
     209,   174,   205,   112,   842,   843,   844,   347,   440,  1381,
     442,  1383,   184,  1376,  1378,   854,  1044,   856,   857,   859,
     785,   246,   363,   868,   334,   247,   205,   444,    53,   369,
     182,   370,   871,   870,   877,   881,    56,   248,   451,   192,
     452,   249,   804,   804,   804,   804,   804,   804,   804,   804,
     804,   804,   804,   804,   841,  1411,   201,   193,  1112,  1332,
    1023,   621,   332,  1335,   253,  1337,  1338,   205,   254,   908,
     200,   458,   125,   459,  1133,  1134,   753,  1136,   458,   890,
     479,   202,   203,   114,  1139,   787,   125,   923,   195,   112,
     905,   457,  -609,   906,   125,   926,   934,   907,   925,   927,
     935,   125,   204,   125,   938,   995,   940,  1022,   715,  1294,
    1295,  1157,   112,  1298,  1299,   948,   950,   220,  1364,   952,
     954,  1367,   434,   435,   125,  1099,     1,  1068,  1055,  1100,
    1099,   680,   621,   542,  1102,   969,   224,   621,   691,   221,
     621,   542,  1109,   785,   621,   785,  1113,   228,   458,   707,
     481,   542,   542,  1388,   996,  1169,  1391,  1126,  1128,  1170,
    1194,   280,   205,   229,  1195,   824,   508,  1004,   509,  1210,
     206,   206,   206,   981,     1,   824,   230,   284,  1263,  1211,
    1016,  1017,  1267,   231,   515,   983,   516,  1229,   235,  1024,
     286,   239,  1027,  1028,   678,   679,  1031,  1032,   210,   289,
     290,   953,   955,  1038,  1129,  1042,   546,   242,   370,   280,
     875,   879,  1135,  1144,  1137,  1138,   562,   508,   563,   720,
     293,  1360,   -11,  1140,  1141,   284,   780,  1252,   562,  1213,
     743,   252,  1216,  1217,   755,   562,   334,   776,   286,  1372,
    1373,  1374,   255,   790,   782,   791,   256,   289,   290,   -11,
     257,   785,   439,  1067,     1,   800,  1069,   801,  1173,  1293,
     269,   271,  1296,  1297,  1096,   458,  1075,   832,   293,   508,
     438,   883,   441,  1078,   443,   785,   562,   266,   895,   785,
     260,   785,   785,   785,   785,   785,   562,  1091,   896,   280,
     440,   445,   442,  1304,  1305,   306,   458,   542,   912,  1306,
     980,   981,   -11,   848,   307,   284,  1310,  1311,   542,   444,
     458,   458,   944,   945,   971,   972,   542,   982,   286,   308,
    1130,   685,   458,   508,   946,  1034,   309,   289,   290,   -11,
     562,  1049,  1047,  1050,   685,   313,   868,   868,   324,   458,
    1143,  1051,  1145,   868,   761,   763,   983,  1148,   293,  1049,
     562,  1052,  1056,   983,   334,   319,   804,   319,   804,   804,
     804,   983,   804,    15,    16,  1161,   562,  1347,  1057,   270,
    1165,    17,    18,    19,    20,    21,   458,   337,  1058,    22,
    1266,  1168,   377,    23,    24,  1172,  1109,   458,  1110,  1250,
    1263,  1182,  1264,  1184,  1185,  1049,   518,  1321,    39,   519,
     373,   621,   436,   437,   457,   457,   376,   415,   457,   457,
     458,  1049,  1322,  1323,   418,   422,   785,   426,     1,   432,
      39,   205,   448,   460,   464,   983,   520,   841,   438,   439,
     521,   522,   465,   468,   467,   523,   524,   525,   342,   526,
     472,   841,   527,   474,   475,   478,   482,   480,   440,   441,
     442,   443,   504,   280,   528,   505,   529,   530,   531,   517,
     568,  1243,   532,   542,   601,   981,   -11,   444,   445,   284,
     602,  1248,  1249,   983,   603,  1340,   604,  1341,   611,  1342,
     612,  1343,   286,   613,   614,   707,   785,   628,  1260,   638,
    1262,   289,   290,   -11,   533,   534,   643,   644,   645,   646,
    1269,   647,   648,   785,   649,   650,   651,   652,   785,   785,
     785,  1277,   293,   785,   653,   654,   457,   662,   663,   370,
     664,   665,   758,   535,   687,   666,   759,   536,   670,   671,
     672,   675,   676,   983,   457,   457,   457,   677,   751,   688,
     689,   692,   793,   698,   700,   701,   702,   719,   721,   682,
     789,    39,   519,   750,   656,  1308,   752,   766,   798,   799,
     817,   834,   833,  1385,   818,   831,  1317,   837,   537,   538,
     839,   845,   850,   851,   539,   855,   872,   983,   112,   520,
     884,   886,   898,   521,   522,  1330,   887,   888,   523,   524,
     525,   901,   526,   904,   911,   527,   924,   939,   683,   928,
    1409,   929,  1286,   542,  1288,   930,  1290,   528,  1292,   529,
     530,   531,   931,   542,   932,   532,  1416,  1417,   933,   542,
     542,   542,   542,   542,   542,   942,   956,    84,    85,   542,
    1352,  1353,   957,   958,    90,    91,    92,  1317,   973,   970,
     959,   997,   960,   961,  1005,   962,   963,   533,   534,    98,
     964,   965,  1002,   785,   966,   967,   785,  1368,  1369,  1370,
    1371,    39,    40,    41,   968,    42,    43,    44,   994,  1008,
    1003,  1012,  1035,  1039,  1014,  1015,   535,  1018,  1025,  1030,
     536,  1045,  1046,  1048,   983,   746,    45,    46,    47,  1053,
    -596,  -597,  -598,  1062,   518,  1063,    39,   519,  1064,  1071,
     772,   497,  1076,   684,    84,    85,  1101,    86,    87,    88,
      89,    90,    91,    92,  1408,  1077,  1410,  1114,   981,  1115,
    1142,   537,   538,  1154,   520,  1155,    98,   539,   521,   522,
     542,  1180,   542,   523,   524,   525,  1181,   526,  1214,  1186,
     527,  1191,  1192,   542,  1193,    39,    40,    41,  1196,    42,
      43,    44,   528,  1197,   529,   530,   531,  1198,  1199,  1200,
     532,  1201,  1215,  1218,  1222,  1240,   518,  1223,    39,   519,
      45,    46,    47,  1241,  1255,  1257,   103,   104,   105,   106,
     107,   108,   109,  1272,  1273,  1278,  1285,  1300,  1280,   542,
    1281,  1282,   533,   534,  1283,  1284,   520,  1302,  1287,  1271,
     521,   522,  1289,  1291,  1227,   523,   524,   525,   874,   526,
    1324,  1329,   527,  1328,    48,    49,    50,  1333,  1336,    51,
      52,   535,  1339,  1344,   528,   536,   529,   530,   531,    53,
    1167,  1345,   532,    54,    55,  1346,   542,    56,   542,   542,
    1348,  1349,   873,  1396,   542,  1362,  1363,  1365,  1228,  1366,
     542,   542,   542,   103,   104,   105,   106,   107,   108,   109,
    1384,  1386,  1387,   709,   533,   534,   537,   538,  1389,    15,
      16,  1390,   539,  1397,  1398,  1399,  1401,    17,    18,    19,
      20,    21,  1402,  1403,  1334,    22,  1404,  1405,  1412,    23,
      24,  1406,  1407,   535,    61,  1413,    29,   536,    48,    49,
      50,   554,   889,    51,    52,   420,  1179,   674,  1265,   275,
     754,   507,   542,    53,   941,   537,   874,    54,    55,  1358,
     276,    56,   277,  -310,  -310,   414,  -310,  -310,  -310,  1359,
    1162,   634,  1065,   273,   669,   709,   425,  1054,   537,   538,
     661,   360,     1,  1072,   539,   979,  1073,  -310,  -310,  -310,
     993,  1220,   -11,   -11,   278,   724,   -11,   -11,   -11,   725,
     848,   848,   -11,  1111,  -310,   279,   951,  1166,   368,   198,
    -310,   762,   542,     0,   760,  1106,   567,   280,   726,     0,
       0,     0,     0,   281,     0,     0,   727,   -11,     0,     0,
     -11,     0,     0,   284,     0,     0,     0,   -11,     0,   -11,
     -11,   -11,   -11,   -11,   -11,     0,   286,   -11,     0,   542,
       0,     0,   287,   288,   -11,   289,   290,   -11,     0,     0,
     -11,   -11,   -11,   -11,   -11,   542,   542,     0,     0,     0,
       0,     0,   -11,   -11,     0,   292,   293,   -11,   -11,   -11,
     -11,   -11,   -11,   -11,   -11,   728,     0,     0,   -11,     0,
       0,     0,   -11,   -11,   -11,   -11,     0,     0,     0,     0,
       0,     0,   518,   295,    39,   519,     0,     0,     0,     0,
       0,     0,   296,     0,  -310,  -310,  -310,  -310,     0,     0,
    -310,  -310,     0,     0,     0,     0,     0,  -310,     0,     0,
    -310,     0,   520,     0,  -310,  -310,   521,   522,  -310,     0,
       0,   523,   524,   525,     0,   526,     0,     0,   527,     0,
     518,   840,    39,   519,     0,     0,     0,     0,     0,     0,
     528,     0,   529,   530,   531,     0,     0,     0,   532,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     520,     0,     0,     0,   521,   522,     0,     0,     0,   523,
     524,   525,     0,   526,     0,     0,   527,     0,     0,     0,
     533,   534,     0,     0,   518,     0,    39,   519,   528,     0,
     529,   530,   531,     0,     0,     0,   532,     0,     0,     0,
       0,  1007,     0,     0,     0,     0,     0,     0,     0,   535,
       0,     0,     0,   536,   520,     0,     0,     0,   521,   522,
       0,     0,     0,   523,   524,   525,     0,   526,   533,   534,
     527,     0,     0,  -490,   518,     0,    39,   519,     0,     0,
       0,     0,   528,     0,   529,   530,   531,     0,     0,     0,
     532,     0,     0,     0,   537,   538,     0,   535,     0,     0,
     539,   536,     0,     0,   520,     0,     0,     0,   521,   522,
       0,     0,     0,   523,   524,   525,     0,   526,     0,     0,
     527,     0,   533,   534,   518,     0,    39,   519,     0,     0,
       0,     0,   528,     0,   529,   530,   531,     0,     0,     0,
     532,     0,   537,   538,     0,  1309,     0,     0,   539,     0,
       0,   535,     0,     0,   520,   536,     0,     0,   521,   522,
       0,     0,     0,   523,   524,   525,     0,   526,     0,     0,
     527,     0,   533,   534,     0,     0,     0,     0,     0,     0,
       0,     0,   528,     0,   529,   530,   531,     0,     0,     0,
     532,     0,     0,     0,     0,     0,   537,   538,     0,     0,
       0,   535,   539,     0,  1230,   536,    39,    40,    41,     0,
      42,    43,    44,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   533,   534,     0,     0,     0,     0,     0,     0,
       0,    45,    46,    47,  1230,     0,    39,    40,    41,     0,
      42,    43,    44,   483,     0,     0,   537,   538,  1121,     0,
       0,   535,   539,     0,  1235,   536,     0,     0,     0,     0,
       0,    45,    46,    47,   862,     0,    39,    40,    41,     0,
      42,    43,    44,     0,     0,     0,     0,     0,  1121,     0,
       0,     0,     0,     0,  1237,     0,   484,     0,     0,     0,
       0,    45,    46,    47,     0,     0,   537,   538,     0,     0,
       0,  1120,   539,    39,    40,    41,     0,    42,    43,    44,
       0,     0,     0,   485,   486,   487,   488,   489,   490,   491,
       0,   492,   493,   494,   495,   496,     0,     0,    45,    46,
      47,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   863,     0,     0,  1121,     0,   497,     0,     0,
       0,     0,   864,     0,     0,     0,     0,     0,  -134,    48,
      49,    50,     0,     0,    51,    52,     0,     0,     0,     0,
       0,  -134,     0,     0,    53,     0,     0,     0,    54,    55,
       0,     0,    56,     0,     0,     0,     0,     0,  -134,    48,
      49,    50,     0,     0,    51,    52,     0,     0,     0,     0,
       0,  -134,     0,     0,    53,     0,     0,     0,    54,    55,
       0,     0,    56,     0,     0,     0,     0,     0,     0,    48,
      49,    50,     0,     0,    51,    52,   865,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,    54,    55,
       0,     0,    56,     0,     0,  1125,     0,    39,    40,    41,
       0,    42,    43,    44,     0,  -134,    48,    49,    50,     0,
       0,    51,    52,     0,     0,     0,     0,     0,  -134,     0,
       0,    53,    45,    46,    47,    54,    55,     0,  1127,    56,
      39,    40,    41,     0,    42,    43,    44,     0,     0,  1121,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    45,    46,    47,   862,     0,
      39,    40,    41,     0,    42,    43,    44,     0,     0,     0,
       0,     0,  1121,     0,  1163,     0,    39,    40,    41,     0,
      42,    43,    44,     0,     0,    45,    46,    47,    39,    40,
      41,     0,    42,    43,    44,     0,     0,     0,     0,     0,
       0,    45,    46,    47,    39,    40,    41,     0,    42,    43,
      44,     0,     0,    45,    46,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    45,
      46,    47,     0,     0,     0,     0,   863,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   864,     0,     0,  -134,
      48,    49,    50,     0,     0,    51,    52,     0,     0,     0,
       0,     0,  -134,     0,     0,    53,     0,     0,     0,    54,
      55,     0,   853,    56,    39,    40,    41,     0,    42,    43,
      44,     0,  -134,    48,    49,    50,     0,     0,    51,    52,
       0,     0,     0,     0,     0,  -134,     0,     0,    53,    45,
      46,    47,    54,    55,     0,     0,    56,     0,     0,     0,
       0,     0,     0,    48,    49,    50,     0,     0,    51,    52,
       0,     0,     0,     0,     0,     0,     0,     0,    53,    48,
      49,    50,    54,    55,    51,    52,    56,     0,     0,     0,
       0,    48,    49,    50,    53,  1164,    51,    52,    54,    55,
       0,     0,    56,     0,     0,     0,    53,    48,    49,    50,
      54,    55,    51,    52,    56,     0,   746,   858,     0,    39,
      40,    41,    53,    42,    43,    44,    54,    55,     0,     0,
      56,     0,   772,   978,     0,    39,    40,    41,     0,    42,
      43,    44,     0,     0,    45,    46,    47,   660,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   483,
      45,    46,    47,     0,     0,     0,  1307,     0,    39,    40,
      41,     0,    42,    43,    44,     0,     0,    48,    49,    50,
       0,     0,    51,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    45,    46,    47,    54,    55,     0,     0,
      56,     0,   484,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    39,    40,    41,     0,    42,    43,    44,     0,   485,
     486,   487,   488,   489,   490,   491,     0,   492,   493,   494,
     495,   496,     0,     0,     0,     0,    45,    46,    47,     0,
       0,     0,     0,     0,    39,    40,    41,     0,    42,    43,
      44,     0,     0,   497,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,    50,     0,     0,    51,    52,    45,
      46,    47,     0,   436,   437,     0,     0,    53,    48,    49,
      50,    54,    55,    51,    52,    56,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,    54,    55,   438,
     439,    56,    39,    40,    41,     0,    42,    43,    44,     0,
       0,    48,    49,    50,     0,     0,    51,    52,     0,   440,
     441,   442,   443,     0,     0,     0,    53,    45,    46,    47,
      54,    55,   438,   439,    56,     0,     0,     0,   444,   445,
       0,     0,     0,     0,  1316,     0,     0,     0,     0,     0,
    1357,     0,   440,   441,   442,   443,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,     0,     0,    51,
      52,   444,   445,     0,   365,     0,     0,     0,     0,    53,
       0,     0,     0,    54,    55,     0,     0,    56,    39,    40,
      41,     0,    42,    43,    44,     0,     0,    48,    49,    50,
       0,     0,    51,    52,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    45,    46,    47,    54,    55,     0,     0,
      56,    39,    40,    41,     0,    42,    43,    44,   366,     0,
       0,     0,     0,     0,     0,     0,     0,    39,    40,    41,
    -263,    42,    43,    44,     0,     0,    45,    46,    47,     0,
       0,     0,     0,     0,     0,    48,    49,    50,     0,     0,
      51,    52,    45,    46,    47,     0,     0,    39,    40,    41,
      53,    42,    43,    44,    54,    55,     0,     0,    56,  1316,
       0,     0,     0,  -263,     0,   878,     0,     0,     0,     0,
       0,     0,    45,    46,    47,     0,     0,   863,    39,    40,
      41,     0,    42,    43,    44,     0,     0,   864,     0,     0,
    -263,  -263,  -263,  -263,  -263,  -263,  -263,     0,  -263,  -263,
    -263,  -263,  -263,    45,    46,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,  -263,     0,    51,    52,     0,     0,
       0,     0,     0,     0,     0,     0,    53,     0,   537,   874,
      54,    55,     0,     0,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,    49,    50,     0,     0,    51,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    53,
      48,    49,    50,    54,    55,    51,    52,    56,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,    54,
      55,     0,     0,    56,     0,     0,     0,     0,     0,     0,
      48,    49,    50,     0,     0,    51,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    53,     0,     0,     0,    54,
      55,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,    48,    49,    50,     0,     0,    51,    52,     0,     0,
       0,     0,     0,     0,     0,     0,   949,     0,     0,     0,
      54,    55,    84,    85,    56,    86,    87,    88,    89,    90,
      91,    92,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,    98,     0,     0,     0,    84,    85,
       0,    86,    87,    88,    89,    90,    91,    92,     0,   168,
     169,     0,     0,     0,    93,    94,    95,    96,    97,     0,
      98,     0,     0,    84,    85,     0,    86,    87,    88,    89,
      90,    91,    92,     0,   168,   169,     0,     0,     0,    93,
      94,    95,    96,    97,     0,    98,     0,     0,    84,    85,
       0,    86,    87,    88,    89,    90,    91,    92,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
      98,    84,    85,     0,    86,    87,    88,    89,    90,    91,
      92,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   124,    99,   170,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   171,    99,   170,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   189,    99,   314,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,   315,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,    84,    85,     0,
      86,    87,    88,    89,    90,    91,    92,     0,     0,   222,
       0,     0,     0,    93,    94,    95,    96,    97,     0,    98,
      84,    85,     0,    86,    87,    88,    89,    90,    91,    92,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,    98,    84,    85,     0,    86,    87,    88,    89,
      90,    91,    92,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,    98,    84,    85,     0,    86,
      87,    88,    89,    90,    91,    92,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,    98,    84,
      85,     0,    86,    87,    88,    89,    90,    91,    92,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,    98,    84,    85,     0,    86,    87,    88,    89,    90,
      91,    92,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,    98,     0,     0,     0,     0,     0,
       0,    99,     0,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,   251,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,   423,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,   599,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,   171,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,   189,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,    84,    85,
       0,    86,    87,    88,    89,    90,    91,    92,     0,     0,
    1242,     0,     0,     0,    93,    94,    95,    96,    97,     0,
      98,    84,    85,     0,    86,    87,    88,    89,    90,    91,
      92,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,    98,    84,    85,     0,    86,    87,    88,
      89,    90,    91,    92,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,  1187,    98,     0,     0,     0,
       0,     0,    84,    85,     0,    86,    87,    88,    89,    90,
      91,    92,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,    98,    84,    85,     0,    86,    87,
      88,    89,    90,    91,    92,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,    98,    84,    85,
       0,    86,    87,    88,    89,    90,    91,    92,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
      98,     0,    99,     0,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,  1261,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,  1312,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1325,     0,     0,     0,    99,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
    1188,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,  1183,     0,     0,     0,   123,     0,     0,
       0,     0,    99,     0,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,     0,     0,     0,     0,    84,    85,
     123,    86,    87,    88,    89,    90,    91,    92,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
      98,    84,    85,     0,    86,    87,    88,    89,    90,    91,
      92,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,    98,    84,    85,     0,    86,    87,    88,
      89,    90,    91,    92,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,    98,    84,    85,     0,
      86,    87,    88,    89,    90,    91,    92,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,    98,
      84,    85,     0,    86,    87,    88,    89,    90,    91,    92,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,    98,    84,    85,     0,    86,    87,    88,    89,
      90,    91,    92,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,    98,     0,     0,     0,     0,
       0,     0,    99,     0,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,     0,     0,     0,     0,     0,     0,
     947,     0,     0,     0,     0,    99,     0,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,     0,
       0,     0,     0,   999,     0,     0,     0,     0,    99,     0,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
       0,     0,     0,     0,     0,     0,  1000,     0,     0,     0,
       0,    99,     0,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,     0,     0,     0,     0,     0,     0,  1001,
       0,     0,     0,     0,    99,     0,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,     0,     0,     0,     0,
       0,     0,  1009,     0,     0,     0,     0,    99,     0,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,     0,     0,    84,    85,  1010,    86,    87,    88,    89,
      90,    91,    92,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,    98,    84,    85,     0,    86,
      87,    88,    89,    90,    91,    92,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,    98,    84,
      85,     0,    86,    87,    88,    89,    90,    91,    92,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,    98,    84,    85,     0,    86,    87,    88,    89,    90,
      91,    92,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,    98,    84,    85,     0,    86,    87,
      88,    89,    90,    91,    92,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,    98,    84,    85,
       0,    86,    87,    88,    89,    90,    91,    92,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
      98,     0,     0,     0,     0,     0,     0,    99,     0,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,     0,     0,     0,     0,  1011,     0,     0,     0,     0,
      99,     0,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,     0,     0,     0,     0,     0,     0,  1013,     0,
       0,     0,     0,    99,     0,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,     0,     0,     0,
       0,  1153,     0,     0,     0,     0,    99,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
       0,     0,     0,     0,  1156,     0,     0,     0,     0,    99,
       0,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,     0,     0,     0,     0,  1244,     0,     0,
       0,     0,    99,     0,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,     0,     0,     0,     0,    84,    85,
    1245,    86,    87,    88,    89,    90,    91,    92,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
      98,    84,    85,     0,    86,    87,    88,    89,    90,    91,
      92,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,    98,    84,    85,     0,    86,    87,    88,
      89,    90,    91,    92,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,    98,    84,    85,     0,
      86,    87,    88,    89,    90,    91,    92,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,    98,
      84,    85,     0,    86,    87,    88,    89,    90,    91,    92,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,    98,    84,    85,     0,    86,    87,    88,    89,
      90,    91,    92,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,    98,     0,     0,     0,     0,
       0,     0,    99,     0,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,     0,     0,     0,     0,     0,     0,
    1256,     0,     0,     0,     0,    99,     0,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,     0,
       0,     0,     0,  1258,     0,     0,     0,     0,    99,     0,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
       0,     0,     0,     0,     0,     0,  1331,     0,     0,     0,
       0,    99,     0,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,     0,     0,     0,     0,     0,     0,  1392,
       0,     0,     0,     0,    99,     0,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,     0,     0,     0,     0,
       0,     0,  1393,     0,     0,     0,     0,    99,     0,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,     0,     0,    84,    85,  1394,    86,    87,    88,    89,
      90,    91,    92,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,    98,    84,    85,     0,    86,
      87,    88,    89,    90,    91,    92,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,    98,    84,
      85,     0,    86,    87,    88,    89,    90,    91,    92,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,    98,    84,    85,     0,    86,    87,    88,    89,    90,
      91,    92,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,    98,    84,    85,     0,    86,    87,
      88,    89,    90,    91,    92,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,    98,    84,    85,
       0,    86,    87,    88,    89,    90,    91,    92,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
      98,     0,     0,     0,     0,     0,     0,    99,     0,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,     0,     0,     0,     0,  1395,     0,     0,     0,     0,
      99,     0,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,     0,     0,     0,     0,     0,     0,  1414,     0,
       0,     0,     0,    99,     0,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,     0,     0,     0,
       0,  1415,     0,     0,     0,     0,    99,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
       0,     0,   777,     0,     0,     0,     0,     0,     0,    99,
       0,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,     0,     0,  1029,     0,     0,     0,     0,
       0,     0,    99,     0,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,     0,     0,    84,    85,  1033,    86,
      87,    88,    89,    90,    91,    92,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,    98,    84,
      85,     0,    86,    87,    88,    89,    90,    91,    92,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,    98,    84,    85,     0,    86,    87,    88,    89,    90,
      91,    92,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,    98,    84,    85,     0,    86,    87,
      88,    89,    90,    91,    92,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,    98,    84,    85,
       0,    86,    87,    88,    89,    90,    91,    92,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
      98,    84,    85,     0,    86,    87,    88,    89,    90,    91,
      92,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,    98,     0,     0,     0,     0,     0,     0,
      99,     0,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,     0,     0,     0,     0,  1116,     0,     0,     0,
       0,     0,     0,    99,     0,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,     0,     0,  1131,
       0,     0,     0,     0,     0,     0,    99,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
       0,     0,  1146,     0,     0,     0,     0,     0,     0,    99,
       0,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,     0,     0,  1147,     0,     0,     0,     0,
       0,     0,    99,     0,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,     0,     0,     0,     0,  1149,     0,
       0,     0,     0,     0,     0,    99,     0,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,    84,
      85,  1150,    86,    87,    88,    89,    90,    91,    92,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,    98,    84,    85,     0,    86,    87,    88,    89,    90,
      91,    92,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,    98,    84,    85,     0,    86,    87,
      88,    89,    90,    91,    92,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,    98,    84,    85,
       0,    86,    87,    88,    89,    90,    91,    92,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
      98,    84,    85,     0,    86,    87,    88,    89,    90,    91,
      92,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,    98,    84,    85,     0,    86,    87,    88,
      89,    90,    91,    92,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,    98,     0,     0,     0,
       0,     0,     0,    99,     0,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,     0,     0,  1238,
       0,     0,     0,     0,     0,     0,    99,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
       0,     0,  1320,     0,     0,     0,     0,     0,     0,    99,
       0,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,     0,     0,     0,     0,  1350,     0,     0,     0,     0,
       0,     0,    99,     0,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,     0,     0,     0,     0,  1379,     0,
       0,     0,     0,     0,     0,    99,     0,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,     0,
       0,  1380,     0,     0,     0,     0,     0,     0,    99,   166,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
      84,    85,     0,    86,    87,    88,    89,    90,    91,    92,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,    98,    84,    85,     0,    86,    87,    88,    89,
      90,    91,    92,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,    98,    84,    85,     0,    86,
      87,    88,    89,    90,    91,    92,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,    98,    84,
      85,     0,    86,    87,    88,    89,    90,    91,    92,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,    98,    84,    85,     0,    86,    87,    88,    89,    90,
      91,    92,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,    98,    84,    85,     0,    86,    87,
      88,    89,    90,    91,    92,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,    98,     0,     0,
       0,     0,     0,     0,    99,   259,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,   936,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,  1074,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,  1171,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,  1268,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,    84,    85,     0,    86,    87,    88,    89,    90,    91,
      92,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,     0,    98,    84,    85,     0,    86,    87,    88,
      89,    90,    91,    92,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,     0,    98,    84,    85,     0,
      86,    87,    88,    89,    90,    91,    92,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,     0,    98,
      84,    85,     0,    86,    87,    88,    89,    90,    91,    92,
     569,   764,     0,     0,     0,     0,     0,   570,    95,   571,
       0,   572,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   101,   102,   103,   104,   105,   106,
     107,   108,   109,     0,   276,     0,   277,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   102,   103,
     104,   105,   106,   107,   108,   109,     1,     0,     0,   270,
       0,     0,     0,     0,     0,     0,   -11,   -11,   278,   724,
     -11,   -11,   -11,   725,    15,    16,   -11,     0,     0,   279,
       0,     0,    17,    18,    19,    20,    21,     0,     0,     0,
      22,   280,   726,     0,    23,    24,     0,   281,     0,     0,
     727,   -11,     0,     0,   -11,     0,     0,   284,     0,     0,
       0,   -11,     0,   -11,   -11,   -11,   -11,   -11,   -11,     0,
     286,   -11,     0,     0,     0,     0,   287,   288,   -11,   289,
     290,   -11,     0,     0,   -11,   -11,   -11,   -11,   -11,     0,
       0,     0,   276,     0,   277,     0,   -11,   -11,     0,   292,
     293,   -11,   -11,   -11,   -11,   -11,   -11,   -11,   -11,   728,
       0,     0,   -11,     0,     1,     0,   -11,   -11,   -11,   -11,
       0,     0,     0,     0,   -11,   -11,   278,   295,   -11,   -11,
     -11,     0,     0,     0,   -11,     0,   296,   279,  1354,     0,
       0,     0,     0,     0,  -341,     0,     0,     0,     0,   280,
       0,     0,     0,     0,     0,   281,     0,     0,     0,   -11,
     282,   283,   -11,     0,     0,   284,     0,     0,     0,   -11,
       0,   -11,   -11,   -11,   -11,   -11,   -11,   285,   286,   -11,
       0,     0,     0,     0,   287,   288,   -11,   289,   290,   -11,
       0,     0,   -11,   -11,   -11,   -11,   -11,     0,     0,     0,
     291,     0,     0,     0,   -11,   -11,     0,   292,   293,   -11,
     -11,   -11,   -11,   -11,   -11,   -11,   -11,   294,     0,   276,
     -11,   277,     0,     0,   -11,   -11,   -11,   -11,     0,     0,
       0,     0,     0,     0,     0,   295,     0,     0,     0,     0,
       0,     1,   -11,     0,   296,     0,     0,     0,     0,     0,
       0,   -11,   -11,   278,     0,   -11,   -11,   -11,     0,     0,
       0,   -11,     0,     0,   279,     0,     0,     0,     0,     0,
       0,  -340,     0,     0,     0,     0,   280,     0,     0,     0,
       0,     0,   281,     0,     0,     0,   -11,   282,   283,   -11,
       0,     0,   284,     0,     0,     0,   -11,     0,   -11,   -11,
     -11,   -11,   -11,   -11,   285,   286,   -11,     0,     0,     0,
       0,   287,   288,   -11,   289,   290,   -11,     0,     0,   -11,
     -11,   -11,   -11,   -11,     0,     0,     0,   291,     0,     0,
       0,   -11,   -11,     0,   292,   293,   -11,   -11,   -11,   -11,
     -11,   -11,   -11,   -11,   294,     0,   276,   -11,   277,     0,
       0,   -11,   -11,   -11,   -11,     0,     0,     0,     0,     0,
       0,     0,   295,     0,     0,     0,     0,     0,     1,   -11,
       0,   296,     0,     0,     0,     0,     0,     0,   -11,   -11,
     278,   724,   -11,   -11,   -11,   725,     0,     0,   -11,     0,
       0,   279,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   280,   726,     0,     0,     0,     0,   281,
       0,     0,   727,   -11,     0,     0,   -11,     0,     0,   284,
       0,     0,     0,   -11,     0,   -11,   -11,   -11,   -11,   -11,
     -11,     0,   286,   -11,     0,     0,     0,     0,   287,   288,
     -11,   289,   290,   -11,     0,     0,   -11,   -11,   -11,   -11,
     -11,     0,     0,     0,     0,     0,     0,     0,   -11,   -11,
       0,   292,   293,   -11,   -11,   -11,   -11,   -11,   -11,   -11,
     -11,   728,     0,   276,   -11,   277,     0,     0,   -11,   -11,
     -11,   -11,     0,     0,     0,     0,     0,     0,     0,   295,
       0,     0,     0,     0,     0,     1,     0,  -301,   296,     0,
       0,     0,     0,     0,     0,   -11,   -11,   278,   724,   -11,
     -11,   -11,   725,     0,     0,   -11,     0,     0,   279,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     280,   726,     0,     0,     0,     0,   281,     0,     0,   727,
     -11,     0,     0,   -11,     0,     0,   284,     0,     0,     0,
     -11,     0,   -11,   -11,   -11,   -11,   -11,   -11,     0,   286,
     -11,     0,     0,     0,     0,   287,   288,   -11,   289,   290,
     -11,     0,     0,   -11,   -11,   -11,   -11,   -11,     0,     0,
       0,     0,     0,     0,     0,   -11,   -11,     0,   292,   293,
     -11,   -11,   -11,   -11,   -11,   -11,   -11,   -11,   728,     0,
     276,   -11,   277,     0,     0,   -11,   -11,   -11,   -11,     0,
       0,     0,     0,     0,     0,     0,   295,     0,     0,     0,
       0,     0,     1,     0,  -300,   296,     0,     0,     0,     0,
       0,     0,   -11,   -11,   278,   724,   -11,   -11,   -11,   725,
       0,     0,   -11,     0,     0,   279,     0,     0,     0,  1152,
       0,     0,     0,     0,     0,     0,     0,   280,   726,     0,
       0,     0,     0,   281,     0,     0,   727,   -11,     0,     0,
     -11,     0,     0,   284,     0,     0,     0,   -11,     0,   -11,
     -11,   -11,   -11,   -11,   -11,     0,   286,   -11,     0,     0,
       0,     0,   287,   288,   -11,   289,   290,   -11,     0,     0,
     -11,   -11,   -11,   -11,   -11,     0,     0,     0,     0,     0,
       0,     0,   -11,   -11,     0,   292,   293,   -11,   -11,   -11,
     -11,   -11,   -11,   -11,   -11,   728,     0,   276,   -11,   277,
       0,     0,   -11,   -11,   -11,   -11,     0,     0,     0,     0,
       0,     0,     0,   295,     0,     0,     0,     0,     0,     1,
       0,     0,   296,     0,     0,     0,     0,     0,     0,   -11,
     -11,   278,   724,   -11,   -11,   -11,   725,     0,     0,   -11,
       0,     0,   279,     0,     0,     0,  1315,     0,     0,     0,
       0,     0,     0,     0,   280,   726,     0,     0,     0,     0,
     281,     0,     0,   727,   -11,     0,     0,   -11,     0,     0,
     284,     0,     0,     0,   -11,     0,   -11,   -11,   -11,   -11,
     -11,   -11,     0,   286,   -11,     0,     0,     0,     0,   287,
     288,   -11,   289,   290,   -11,     0,     0,   -11,   -11,   -11,
     -11,   -11,     0,     0,     0,     0,     0,     0,     0,   -11,
     -11,     0,   292,   293,   -11,   -11,   -11,   -11,   -11,   -11,
     -11,   -11,   728,     0,   893,   -11,   277,     0,     0,   -11,
     -11,   -11,   -11,     0,     0,     0,     0,     0,     0,     0,
     295,     0,     0,     0,     0,     0,     1,     0,     0,   296,
       0,     0,     0,     0,     0,     0,   -11,   -11,   278,   724,
     -11,   -11,   -11,   725,     0,     0,   -11,     0,     0,   279,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   280,   726,     0,     0,     0,     0,   281,     0,     0,
     727,   -11,     0,     0,   -11,     0,     0,   284,     0,     0,
       0,   -11,     0,   -11,   -11,   -11,   -11,   -11,   -11,     0,
     286,   -11,     0,     0,     0,     0,   287,   288,   -11,   289,
     290,   -11,     0,     0,   -11,   -11,   -11,   -11,   -11,     0,
       0,     0,     0,     0,     0,     0,   -11,   -11,     0,   292,
     293,   -11,   -11,   -11,   -11,   -11,   -11,   -11,   -11,   728,
       0,   276,   -11,   277,     0,     0,   -11,   -11,   -11,   -11,
       0,     0,     0,     0,     0,     0,     0,   295,     0,     0,
       0,     0,     0,     1,     0,     0,   296,     0,     0,     0,
       0,     0,     0,   -11,   -11,   278,   724,   -11,   -11,   -11,
     725,     0,     0,   -11,     0,     0,   279,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   280,   726,
       0,     0,     0,     0,   281,     0,     0,   727,   -11,     0,
       0,   -11,     0,     0,   284,     0,     0,     0,   -11,     0,
     -11,   -11,   -11,   -11,   -11,   -11,     0,   286,   -11,     0,
       0,     0,     0,   287,   288,   -11,   289,   290,   -11,     0,
       0,   -11,   -11,   -11,   -11,   -11,     0,     0,     0,     0,
       0,     0,     0,   -11,   -11,     0,   292,   293,   -11,   -11,
     -11,   -11,   -11,   -11,   -11,   -11,   728,     0,     0,   -11,
       0,     0,     0,   -11,   -11,   -11,   -11,   378,   379,     0,
       0,   380,   381,   382,   295,     0,     0,   383,     0,     0,
       0,     0,     0,   296,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   384,     0,     0,   385,     0,     0,     0,     0,
       0,     0,   386,     0,   387,   388,   389,   390,   391,   392,
       0,     0,   393,     0,     0,     0,     0,     0,     0,   394,
       0,     0,   395,     0,     0,   396,   397,   398,   399,   400,
       0,     0,     0,     0,     0,     0,     0,    15,    16,     0,
       0,     0,   401,   402,   403,    17,    18,    19,    20,    21,
       0,     0,     0,    22,     0,     0,     0,    23,    24,   404,
     405,   378,   379,     0,     0,   380,   381,   382,     0,     0,
       0,   383,     0,     0,     0,   406,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   384,     0,     0,   385,
       0,     0,     0,     0,     0,     0,   386,     0,   387,   388,
     389,   390,   391,   392,     0,     0,   393,     0,     0,     0,
       0,     0,     0,   394,     0,     0,   395,     0,     0,   396,
     397,   398,   399,   400,     0,     0,     0,     0,     0,     0,
       0,    15,    16,     0,     0,     0,   401,   402,   403,    17,
      18,    19,    20,    21,     0,     0,     0,    22,   569,   915,
       0,    23,    24,   404,   405,   570,     0,   571,     0,   572,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   573,   574,   575,   576,
     577,   578,   579,   580,   581,   582,   583,   584,   585,   586,
     587,   588,   589,   590,   591,   569,   916,     0,     0,     0,
       0,     0,   570,     0,   571,     0,   572,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   573,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   587,   588,   589,
     590,   591,   569,     0,     0,     0,     0,     0,     0,   570,
     917,   571,     0,   572,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     918,   919,   920,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   569,
       0,     0,     0,     0,     0,     0,   570,     0,   571,     0,
     572,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   573,   574,   575,
     576,   577,   578,   579,   580,   581,   582,   583,   584,   585,
     586,   587,   588,   589,   590,   591
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-987))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      33,    60,     2,   407,   416,     0,   692,   289,   290,   717,
     718,     6,   173,   461,   830,   697,   320,   321,   322,   323,
      53,   745,   182,    56,   385,    56,   885,  1006,   657,   658,
       3,   411,   597,   411,     3,     3,    67,   732,    10,     3,
     735,     1,   537,     3,   407,   269,    32,   271,   272,     3,
     618,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,    51,   111,   110,
       3,   305,     3,   407,  1070,  1071,     3,   207,   208,     3,
       3,     3,   125,   124,    45,    46,    47,    48,    49,    50,
      51,    52,    23,    54,    55,    24,     1,   171,     3,   433,
       3,     1,     3,     3,   338,   339,   340,   237,     1,   279,
       3,     1,     3,   156,   284,     3,   286,    71,     8,   353,
       8,     9,     3,   166,     3,   703,     3,     8,     9,   172,
     169,   174,     3,     3,   556,     3,   601,   602,   603,   604,
       8,     9,   556,     3,    98,     3,     3,   190,   462,   463,
       8,     9,    21,    22,   333,    50,     3,   471,   663,    86,
     665,   395,   205,   628,   334,   991,     0,   163,   164,   165,
       3,   155,   998,   351,   352,   409,   346,    24,   549,   184,
    1006,   554,  1178,   417,   163,   355,   761,   169,   763,   764,
     359,   360,   171,   171,   177,     1,   239,     3,     4,     5,
     370,     7,     8,     9,     1,   179,     3,     4,     5,   179,
       7,     8,     9,    51,   107,   179,   259,   260,   552,   553,
     378,   513,   612,   611,   614,   613,   384,    51,    32,    86,
       3,     4,     5,  1222,     7,     8,     9,   168,     3,   168,
     410,   172,     3,   248,  1070,   250,   179,   541,   179,   171,
     181,   173,   566,   177,    87,   644,   645,   646,   647,   648,
     649,   650,   651,   652,   653,   654,   171,  1263,   173,   171,
     171,   314,   451,   565,   179,   170,   179,   250,   181,   179,
     171,   171,  1151,   171,   177,   455,   155,   297,   466,   168,
     171,   461,  1118,    72,   171,   305,     7,     8,   303,    73,
     171,   171,    76,   171,   473,    70,    79,     1,     3,     3,
      85,   171,   168,   171,   168,     3,    89,   155,   172,     1,
     155,     3,     4,     5,   180,     7,     8,     9,    23,    71,
     303,   155,  1037,   168,   108,   974,   975,   507,   630,   631,
    1329,   916,    75,   635,    77,   475,    28,    29,    30,     3,
     168,   722,  1178,     3,    96,    97,   934,   161,   162,   163,
     164,   165,   180,    45,   522,   171,    60,    70,   155,    51,
     411,   171,   530,   179,   171,   117,   155,    71,   168,   549,
     958,   168,   179,    86,   962,     7,   964,   965,   966,   967,
     968,   434,   168,   168,   170,    98,  1222,   172,   171,    17,
      18,    19,    96,    97,   521,     0,   179,   169,   742,   526,
     453,   705,   529,   107,    32,   177,   533,   751,   752,    69,
      70,    98,   465,   117,  1158,  1159,   169,  1119,    23,     0,
      25,   601,   602,   603,   604,   177,    86,   171,   181,    92,
    1132,     5,     1,   177,     3,     4,     5,   181,     7,     8,
       9,   171,    23,   745,    25,  1160,   168,   177,   628,   112,
       1,   114,     3,   506,   173,   757,   170,   889,   180,    28,
      29,    30,   168,   155,   156,   157,   158,   181,   131,   161,
     162,     3,   211,   177,   180,   170,   168,   162,    71,   171,
       1,   177,     3,   175,   176,    90,   181,   179,   539,   168,
       3,     4,     5,  1329,     7,     8,     9,   182,   183,   184,
     551,   180,   169,    96,    97,    98,   557,   168,     3,    90,
     177,  1099,   980,   981,   982,     3,  1244,  1245,   686,   180,
     279,     1,    12,     3,   117,   264,    71,   266,    79,   697,
      20,  1246,   169,   161,   162,   163,   164,   165,    89,   144,
     177,    12,   621,  1123,   282,   283,  1126,   285,  1128,    20,
      71,    96,    97,    98,    69,    70,   607,   608,   957,   170,
     959,   960,   961,   144,   963,   618,    69,    70,     1,   620,
       3,    86,   117,   171,   333,    96,    97,   656,   317,   177,
     168,  1169,    12,    86,   177,   638,   107,   156,   157,   158,
      20,    71,   161,   162,     3,   168,   117,     6,  1186,   172,
     171,  1316,   171,  1191,  1192,  1193,   175,   176,  1196,   168,
     179,    32,  1044,   172,   161,   162,    96,    97,    91,   378,
    1044,   168,   170,   171,     5,   384,   168,   107,   679,   177,
     172,   169,   177,   181,   687,   688,   689,   117,   111,  1354,
     113,  1356,   171,  1349,  1350,   698,   890,   700,   701,   702,
     703,   168,     1,   706,     3,   172,   177,   130,   171,     1,
      87,     3,   715,   714,   717,   718,   179,   168,   168,   168,
     170,   172,   643,   644,   645,   646,   647,   648,   649,   650,
     651,   652,   653,   654,   852,  1400,    71,     3,   992,  1270,
     871,   450,   451,  1274,   168,  1276,  1277,   177,   172,   750,
     172,   168,   168,   170,  1008,  1009,   172,  1011,   168,   729,
     170,    96,    97,    98,  1018,   177,   168,   770,     3,   181,
     172,   901,   107,   168,   168,   168,   168,   172,   172,   172,
     172,   168,   117,   168,   787,   172,   789,   172,   177,  1214,
    1215,  1043,   181,  1218,  1219,   798,   799,   170,  1336,   802,
     803,  1339,   169,   170,   168,   168,    23,   938,   172,   172,
     168,   520,   521,   522,   172,   818,    70,   526,   527,   180,
     529,   530,   168,   826,   533,   828,   172,   170,   168,   538,
     170,   540,   541,  1364,   837,   168,  1367,  1000,  1001,   172,
     168,    58,   177,   170,   172,   168,   168,   850,   170,   172,
     980,   981,   982,    70,    23,   168,     3,    74,   168,   172,
     863,   864,   172,    68,     1,   830,     3,  1121,     3,   872,
      87,   177,   875,   876,   170,   171,   879,   880,     3,    96,
      97,   802,   803,   886,  1002,   888,     1,     3,     3,    58,
     717,   718,  1010,  1024,  1012,  1013,   168,   168,   170,   170,
     117,  1326,    71,  1019,  1020,    74,    66,  1159,   168,  1103,
     170,     3,  1106,  1107,     1,   168,     3,   170,    87,  1344,
    1345,  1346,     3,   168,    65,   170,   170,    96,    97,    98,
       3,   934,    92,   936,    23,   168,   939,   170,  1069,  1213,
     244,   245,  1216,  1217,   973,   168,   949,   170,   117,   168,
      91,   170,   112,   956,   114,   958,   168,   171,   170,   962,
     169,   964,   965,   966,   967,   968,   168,   970,   170,    58,
     111,   131,   113,  1227,  1228,    70,   168,   686,   170,  1233,
      69,    70,    71,   692,   170,    74,  1240,  1241,   697,   130,
     168,   168,   170,   170,   820,   821,   705,    86,    87,   169,
    1003,  1119,   168,   168,   170,   170,   115,    96,    97,    98,
     168,   168,   170,   170,  1132,   178,  1019,  1020,   170,   168,
    1023,   170,  1025,  1026,   594,   595,   991,  1030,   117,   168,
     168,   170,   170,   998,     3,   107,   957,   107,   959,   960,
     961,  1006,   963,   113,   114,  1048,   168,  1301,   170,    98,
    1053,   121,   122,   123,   124,   125,   168,    60,   170,   129,
    1178,  1064,     3,   133,   134,  1068,   168,   168,   170,   170,
     168,  1074,   170,  1076,  1077,   168,     1,   170,     3,     4,
     171,   790,    65,    66,  1214,  1215,   171,    53,  1218,  1219,
     168,   168,   170,   170,     3,    54,  1099,   172,    23,   173,
       3,   177,   173,    52,   170,  1070,    31,  1225,    91,    92,
      35,    36,   169,   168,   170,    40,    41,    42,     3,    44,
     170,  1239,    47,   170,   168,   170,    55,   170,   111,   112,
     113,   114,     3,    58,    59,   170,    61,    62,    63,   170,
     170,  1144,    67,   852,     3,    70,    71,   130,   131,    74,
       3,  1154,  1155,  1118,     3,  1286,     3,  1288,   168,  1290,
     168,  1292,    87,   168,   168,   874,  1169,     3,  1171,   171,
    1173,    96,    97,    98,    99,   100,   171,   171,   171,   171,
    1183,   171,   171,  1186,   171,   171,   171,   171,  1191,  1192,
    1193,  1194,   117,  1196,   171,   171,  1326,   170,   169,     3,
     170,   169,    54,   128,   171,   170,    56,   132,   172,   172,
     172,   172,   168,  1178,  1344,  1345,  1346,   170,   172,   171,
     171,   171,     3,   171,   171,   171,   171,   170,   170,     1,
     169,     3,     4,   171,     3,  1238,   172,   172,   169,   169,
     172,     5,   170,  1361,   177,   171,  1247,   169,   173,   174,
       3,   170,   169,     3,   179,   170,   177,  1222,   181,    31,
     155,   171,     3,    35,    36,  1268,   171,   171,    40,    41,
      42,     3,    44,     3,   170,    47,     3,   177,    50,   172,
    1398,   172,  1203,   992,  1205,   172,  1207,    59,  1209,    61,
      62,    63,   172,  1002,   172,    67,  1414,  1415,   172,  1008,
    1009,  1010,  1011,  1012,  1013,   170,   150,    10,    11,  1018,
    1313,  1314,   168,   168,    17,    18,    19,  1318,     3,   177,
     168,    50,   168,   168,    72,   168,   168,    99,   100,    32,
     168,   168,   172,  1336,   168,   168,  1339,  1340,  1341,  1342,
    1343,     3,     4,     5,   168,     7,     8,     9,   168,   172,
     169,   172,     3,     3,   172,   172,   128,   172,   171,   171,
     132,   152,    49,   169,  1329,   181,    28,    29,    30,   171,
     170,   170,   170,   155,     1,   170,     3,     4,   171,   171,
     181,   171,   150,   155,    10,    11,   172,    13,    14,    15,
      16,    17,    18,    19,  1397,   150,  1399,     5,    70,   170,
     170,   173,   174,   169,    31,   170,    32,   179,    35,    36,
    1119,   170,  1121,    40,    41,    42,   170,    44,     3,   168,
      47,   168,   168,  1132,   168,     3,     4,     5,   168,     7,
       8,     9,    59,   172,    61,    62,    63,   172,   172,   172,
      67,   172,     3,     3,   170,    49,     1,   172,     3,     4,
      28,    29,    30,    49,   172,   170,   159,   160,   161,   162,
     163,   164,   165,   172,   168,   170,   172,    57,   170,  1178,
     170,   170,    99,   100,   170,   170,    31,   170,   172,  1188,
      35,    36,   172,   172,   155,    40,    41,    42,   174,    44,
     172,   170,    47,    52,   156,   157,   158,   170,   168,   161,
     162,   128,   168,     3,    59,   132,    61,    62,    63,   171,
     172,     3,    67,   175,   176,     3,  1225,   179,  1227,  1228,
      50,   170,   100,    57,  1233,   172,   172,   172,   155,   172,
    1239,  1240,  1241,   159,   160,   161,   162,   163,   164,   165,
     172,   170,   170,   170,    99,   100,   173,   174,   170,   113,
     114,   170,   179,   169,   172,   169,    52,   121,   122,   123,
     124,   125,   172,   172,  1273,   129,   172,   172,   170,   133,
     134,   172,   172,   128,    35,   170,     6,   132,   156,   157,
     158,   407,   728,   161,   162,   308,  1071,   514,  1178,   248,
     562,   370,  1301,   171,   790,   173,   174,   175,   176,  1318,
       1,   179,     3,     4,     5,   303,     7,     8,     9,  1320,
    1049,   468,   926,   246,   508,   170,   317,   906,   173,   174,
     499,   286,    23,   947,   179,   828,   947,    28,    29,    30,
     831,  1109,    33,    34,    35,    36,    37,    38,    39,    40,
    1349,  1350,    43,   991,    45,    46,   800,  1062,   292,   180,
      51,   595,  1361,    -1,   594,   981,   419,    58,    59,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    -1,
      71,    -1,    -1,    74,    -1,    -1,    -1,    78,    -1,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    -1,  1398,
      -1,    -1,    93,    94,    95,    96,    97,    98,    -1,    -1,
     101,   102,   103,   104,   105,  1414,  1415,    -1,    -1,    -1,
      -1,    -1,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,    -1,    -1,   129,    -1,
      -1,    -1,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
      -1,    -1,     1,   144,     3,     4,    -1,    -1,    -1,    -1,
      -1,    -1,   153,    -1,   155,   156,   157,   158,    -1,    -1,
     161,   162,    -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,
     171,    -1,    31,    -1,   175,   176,    35,    36,   179,    -1,
      -1,    40,    41,    42,    -1,    44,    -1,    -1,    47,    -1,
       1,    50,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      59,    -1,    61,    62,    63,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    35,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    44,    -1,    -1,    47,    -1,    -1,    -1,
      99,   100,    -1,    -1,     1,    -1,     3,     4,    59,    -1,
      61,    62,    63,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,    -1,    -1,   132,    31,    -1,    -1,    -1,    35,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    44,    99,   100,
      47,    -1,    -1,    50,     1,    -1,     3,     4,    -1,    -1,
      -1,    -1,    59,    -1,    61,    62,    63,    -1,    -1,    -1,
      67,    -1,    -1,    -1,   173,   174,    -1,   128,    -1,    -1,
     179,   132,    -1,    -1,    31,    -1,    -1,    -1,    35,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    99,   100,     1,    -1,     3,     4,    -1,    -1,
      -1,    -1,    59,    -1,    61,    62,    63,    -1,    -1,    -1,
      67,    -1,   173,   174,    -1,    72,    -1,    -1,   179,    -1,
      -1,   128,    -1,    -1,    31,   132,    -1,    -1,    35,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    44,    -1,    -1,
      47,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    59,    -1,    61,    62,    63,    -1,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,   173,   174,    -1,    -1,
      -1,   128,   179,    -1,     1,   132,     3,     4,     5,    -1,
       7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    30,     1,    -1,     3,     4,     5,    -1,
       7,     8,     9,    67,    -1,    -1,   173,   174,    45,    -1,
      -1,   128,   179,    -1,    51,   132,    -1,    -1,    -1,    -1,
      -1,    28,    29,    30,     1,    -1,     3,     4,     5,    -1,
       7,     8,     9,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    -1,    -1,    -1,    51,    -1,   110,    -1,    -1,    -1,
      -1,    28,    29,    30,    -1,    -1,   173,   174,    -1,    -1,
      -1,     1,   179,     3,     4,     5,    -1,     7,     8,     9,
      -1,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
      -1,   145,   146,   147,   148,   149,    -1,    -1,    28,    29,
      30,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    45,    -1,   171,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,   155,   156,
     157,   158,    -1,    -1,   161,   162,    -1,    -1,    -1,    -1,
      -1,   168,    -1,    -1,   171,    -1,    -1,    -1,   175,   176,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,   155,   156,
     157,   158,    -1,    -1,   161,   162,    -1,    -1,    -1,    -1,
      -1,   168,    -1,    -1,   171,    -1,    -1,    -1,   175,   176,
      -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,   156,
     157,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,   175,   176,
      -1,    -1,   179,    -1,    -1,     1,    -1,     3,     4,     5,
      -1,     7,     8,     9,    -1,   155,   156,   157,   158,    -1,
      -1,   161,   162,    -1,    -1,    -1,    -1,    -1,   168,    -1,
      -1,   171,    28,    29,    30,   175,   176,    -1,     1,   179,
       3,     4,     5,    -1,     7,     8,     9,    -1,    -1,    45,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    30,     1,    -1,
       3,     4,     5,    -1,     7,     8,     9,    -1,    -1,    -1,
      -1,    -1,    45,    -1,     1,    -1,     3,     4,     5,    -1,
       7,     8,     9,    -1,    -1,    28,    29,    30,     3,     4,
       5,    -1,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    30,     3,     4,     5,    -1,     7,     8,
       9,    -1,    -1,    28,    29,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
      29,    30,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,   155,
     156,   157,   158,    -1,    -1,   161,   162,    -1,    -1,    -1,
      -1,    -1,   168,    -1,    -1,   171,    -1,    -1,    -1,   175,
     176,    -1,     1,   179,     3,     4,     5,    -1,     7,     8,
       9,    -1,   155,   156,   157,   158,    -1,    -1,   161,   162,
      -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,   171,    28,
      29,    30,   175,   176,    -1,    -1,   179,    -1,    -1,    -1,
      -1,    -1,    -1,   156,   157,   158,    -1,    -1,   161,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,   156,
     157,   158,   175,   176,   161,   162,   179,    -1,    -1,    -1,
      -1,   156,   157,   158,   171,   172,   161,   162,   175,   176,
      -1,    -1,   179,    -1,    -1,    -1,   171,   156,   157,   158,
     175,   176,   161,   162,   179,    -1,   181,     1,    -1,     3,
       4,     5,   171,     7,     8,     9,   175,   176,    -1,    -1,
     179,    -1,   181,     1,    -1,     3,     4,     5,    -1,     7,
       8,     9,    -1,    -1,    28,    29,    30,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,
      28,    29,    30,    -1,    -1,    -1,     1,    -1,     3,     4,
       5,    -1,     7,     8,     9,    -1,    -1,   156,   157,   158,
      -1,    -1,   161,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,    28,    29,    30,   175,   176,    -1,    -1,
     179,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,     7,     8,     9,    -1,   137,
     138,   139,   140,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,     8,
       9,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   156,   157,   158,    -1,    -1,   161,   162,    28,
      29,    30,    -1,    65,    66,    -1,    -1,   171,   156,   157,
     158,   175,   176,   161,   162,   179,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   171,    -1,    -1,    -1,   175,   176,    91,
      92,   179,     3,     4,     5,    -1,     7,     8,     9,    -1,
      -1,   156,   157,   158,    -1,    -1,   161,   162,    -1,   111,
     112,   113,   114,    -1,    -1,    -1,   171,    28,    29,    30,
     175,   176,    91,    92,   179,    -1,    -1,    -1,   130,   131,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,    -1,
      51,    -1,   111,   112,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   157,   158,    -1,    -1,   161,
     162,   130,   131,    -1,     1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,   175,   176,    -1,    -1,   179,     3,     4,
       5,    -1,     7,     8,     9,    -1,    -1,   156,   157,   158,
      -1,    -1,   161,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   171,    28,    29,    30,   175,   176,    -1,    -1,
     179,     3,     4,     5,    -1,     7,     8,     9,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      67,     7,     8,     9,    -1,    -1,    28,    29,    30,    -1,
      -1,    -1,    -1,    -1,    -1,   156,   157,   158,    -1,    -1,
     161,   162,    28,    29,    30,    -1,    -1,     3,     4,     5,
     171,     7,     8,     9,   175,   176,    -1,    -1,   179,    45,
      -1,    -1,    -1,   110,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    30,    -1,    -1,    79,     3,     4,
       5,    -1,     7,     8,     9,    -1,    -1,    89,    -1,    -1,
     137,   138,   139,   140,   141,   142,   143,    -1,   145,   146,
     147,   148,   149,    28,    29,    30,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,   157,   158,   171,    -1,   161,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,   173,   174,
     175,   176,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   156,   157,   158,    -1,    -1,   161,
     162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,
     156,   157,   158,   175,   176,   161,   162,   179,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,   175,
     176,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
     156,   157,   158,    -1,    -1,   161,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,   175,
     176,    -1,    -1,   179,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   156,   157,   158,    -1,    -1,   161,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
     175,   176,    10,    11,   179,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,
      28,    29,    30,    -1,    32,    -1,    -1,    -1,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    21,
      22,    -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,
      32,    -1,    -1,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    21,    22,    -1,    -1,    -1,    26,
      27,    28,    29,    30,    -1,    32,    -1,    -1,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,
      32,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,
      29,    30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   179,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   178,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,   178,
      -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,    32,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,
      30,    -1,    32,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      27,    28,    29,    30,    -1,    32,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    27,    28,    29,    30,    -1,    32,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,
      -1,    32,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,
      28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   178,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   178,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,   178,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,   178,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,   178,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
     178,    -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,
      32,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,
      29,    30,    -1,    32,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    27,    28,    29,    30,     3,    32,    -1,    -1,    -1,
      -1,    -1,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,
      28,    29,    30,    -1,    32,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    27,    28,    29,    30,    -1,    32,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,
      32,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   154,   178,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   178,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,   168,    -1,    -1,    -1,   172,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    10,    11,
     172,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,
      32,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,
      29,    30,    -1,    32,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    27,    28,    29,    30,    -1,    32,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,    32,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,
      30,    -1,    32,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      27,    28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,    -1,    -1,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    10,    11,   172,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      27,    28,    29,    30,    -1,    32,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    27,    28,    29,    30,    -1,    32,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,
      -1,    32,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,
      28,    29,    30,    -1,    32,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    27,    28,    29,    30,    -1,    32,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    10,    11,
     172,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,
      32,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,
      29,    30,    -1,    32,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    27,    28,    29,    30,    -1,    32,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,    32,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,
      30,    -1,    32,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      27,    28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
     172,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,    -1,    -1,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    10,    11,   172,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      27,    28,    29,    30,    -1,    32,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    27,    28,    29,    30,    -1,    32,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,
      -1,    32,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,
      28,    29,    30,    -1,    32,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    27,    28,    29,    30,    -1,    32,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    10,    11,   170,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    27,    28,    29,    30,    -1,    32,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,
      -1,    32,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,
      28,    29,    30,    -1,    32,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    27,    28,    29,    30,    -1,    32,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,
      32,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,
      29,    30,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    10,
      11,   170,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,
      -1,    32,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,
      28,    29,    30,    -1,    32,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    27,    28,    29,    30,    -1,    32,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,    -1,
      32,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,
      29,    30,    -1,    32,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    27,    28,    29,    30,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,
      30,    -1,    32,    10,    11,    -1,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,
      27,    28,    29,    30,    -1,    32,    10,    11,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    27,    28,    29,    30,    -1,    32,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    28,    29,    30,
      -1,    32,    10,    11,    -1,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    27,
      28,    29,    30,    -1,    32,    10,    11,    -1,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    27,    28,    29,    30,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,    10,    11,    -1,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
      29,    30,    -1,    32,    10,    11,    -1,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    28,    29,    30,    -1,    32,    10,    11,    -1,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,    32,
      10,    11,    -1,    13,    14,    15,    16,    17,    18,    19,
     154,   155,    -1,    -1,    -1,    -1,    -1,   161,    28,   163,
      -1,   165,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,    -1,     1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,    23,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    35,    36,
      37,    38,    39,    40,   113,   114,    43,    -1,    -1,    46,
      -1,    -1,   121,   122,   123,   124,   125,    -1,    -1,    -1,
     129,    58,    59,    -1,   133,   134,    -1,    64,    -1,    -1,
      67,    68,    -1,    -1,    71,    -1,    -1,    74,    -1,    -1,
      -1,    78,    -1,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      97,    98,    -1,    -1,   101,   102,   103,   104,   105,    -1,
      -1,    -1,     1,    -1,     3,    -1,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
      -1,    -1,   129,    -1,    23,    -1,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    33,    34,    35,   144,    37,    38,
      39,    -1,    -1,    -1,    43,    -1,   153,    46,   155,    -1,
      -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,
      69,    70,    71,    -1,    -1,    74,    -1,    -1,    -1,    78,
      -1,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,    -1,   101,   102,   103,   104,   105,    -1,    -1,    -1,
     109,    -1,    -1,    -1,   113,   114,    -1,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,    -1,     1,
     129,     3,    -1,    -1,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    23,   151,    -1,   153,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    35,    -1,    37,    38,    39,    -1,    -1,
      -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    68,    69,    70,    71,
      -1,    -1,    74,    -1,    -1,    -1,    78,    -1,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    97,    98,    -1,    -1,   101,
     102,   103,   104,   105,    -1,    -1,    -1,   109,    -1,    -1,
      -1,   113,   114,    -1,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,    -1,     1,   129,     3,    -1,
      -1,   133,   134,   135,   136,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    23,   151,
      -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    43,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    67,    68,    -1,    -1,    71,    -1,    -1,    74,
      -1,    -1,    -1,    78,    -1,    80,    81,    82,    83,    84,
      85,    -1,    87,    88,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    -1,    -1,   101,   102,   103,   104,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,
      -1,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,    -1,     1,   129,     3,    -1,    -1,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    23,    -1,   152,   153,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    -1,    43,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    59,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,
      68,    -1,    -1,    71,    -1,    -1,    74,    -1,    -1,    -1,
      78,    -1,    80,    81,    82,    83,    84,    85,    -1,    87,
      88,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,    -1,   101,   102,   103,   104,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,    -1,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,    -1,
       1,   129,     3,    -1,    -1,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    23,    -1,   152,   153,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,    -1,
      71,    -1,    -1,    74,    -1,    -1,    -1,    78,    -1,    80,
      81,    82,    83,    84,    85,    -1,    87,    88,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    97,    98,    -1,    -1,
     101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,    -1,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,    -1,     1,   129,     3,
      -1,    -1,   133,   134,   135,   136,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    35,    36,    37,    38,    39,    40,    -1,    -1,    43,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    67,    68,    -1,    -1,    71,    -1,    -1,
      74,    -1,    -1,    -1,    78,    -1,    80,    81,    82,    83,
      84,    85,    -1,    87,    88,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,    -1,   101,   102,   103,
     104,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,    -1,     1,   129,     3,    -1,    -1,   133,
     134,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,    -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,   153,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    -1,    43,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    59,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      67,    68,    -1,    -1,    71,    -1,    -1,    74,    -1,    -1,
      -1,    78,    -1,    80,    81,    82,    83,    84,    85,    -1,
      87,    88,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      97,    98,    -1,    -1,   101,   102,   103,   104,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
      -1,     1,   129,     3,    -1,    -1,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,    -1,
      -1,    -1,    -1,    23,    -1,    -1,   153,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    35,    36,    37,    38,    39,
      40,    -1,    -1,    43,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    68,    -1,
      -1,    71,    -1,    -1,    74,    -1,    -1,    -1,    78,    -1,
      80,    81,    82,    83,    84,    85,    -1,    87,    88,    -1,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
      -1,   101,   102,   103,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   113,   114,    -1,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,    -1,    -1,   129,
      -1,    -1,    -1,   133,   134,   135,   136,    33,    34,    -1,
      -1,    37,    38,    39,   144,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    81,    82,    83,    84,    85,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    -1,    98,    -1,    -1,   101,   102,   103,   104,   105,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,   118,   119,   120,   121,   122,   123,   124,   125,
      -1,    -1,    -1,   129,    -1,    -1,    -1,   133,   134,   135,
     136,    33,    34,    -1,    -1,    37,    38,    39,    -1,    -1,
      -1,    43,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    81,
      82,    83,    84,    85,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    98,    -1,    -1,   101,
     102,   103,   104,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,   118,   119,   120,   121,
     122,   123,   124,   125,    -1,    -1,    -1,   129,   154,   155,
      -1,   133,   134,   135,   136,   161,    -1,   163,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   154,   155,    -1,    -1,    -1,
      -1,    -1,   161,    -1,   163,    -1,   165,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   154,    -1,    -1,    -1,    -1,    -1,    -1,   161,
     162,   163,    -1,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   154,
      -1,    -1,    -1,    -1,    -1,    -1,   161,    -1,   163,    -1,
     165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    23,    25,    90,   144,   202,   203,   205,   222,   258,
     377,     3,    24,   206,   207,   113,   114,   121,   122,   123,
     124,   125,   129,   133,   134,   306,     3,   171,     0,   222,
      75,    77,   263,   169,    24,   168,   171,     3,     3,     3,
       4,     5,     7,     8,     9,    28,    29,    30,   156,   157,
     158,   161,   162,   171,   175,   176,   179,   204,   230,   232,
     243,   207,     3,    86,   373,   168,   259,   171,     7,   232,
     232,   232,   232,     1,   232,   232,   232,   232,   230,   232,
       1,   232,   230,   231,    10,    11,    13,    14,    15,    16,
      17,    18,    19,    26,    27,    28,    29,    30,    32,   154,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     171,   177,   181,   248,    98,   374,   168,   172,     5,   173,
     266,   230,   231,   172,   179,   168,   180,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   231,   230,     3,   177,     3,     3,   170,
     168,   171,   260,   172,   231,   230,   155,   172,    21,    22,
     155,   178,   335,   230,   169,   375,    70,    86,    98,   371,
     372,     5,    87,   267,   171,   265,   180,   230,   230,   178,
     335,   230,   168,     3,   245,     3,     3,    86,   371,   378,
     172,    71,    96,    97,   117,   177,   324,   326,   168,   172,
       3,   179,   181,   205,   249,   250,   251,   317,   318,   319,
     170,   180,   178,   230,    70,   376,   168,   170,   170,   170,
       3,    68,   366,   367,   230,     3,   308,    87,   308,   177,
     319,   320,     3,    69,    70,    86,   168,   172,   168,   172,
     261,   178,     3,   168,   172,     3,   170,     3,   379,   155,
     169,   308,     1,   230,   168,   180,   171,   252,   306,   252,
      98,   252,   307,   318,     3,   251,     1,     3,    35,    46,
      58,    64,    69,    70,    74,    86,    87,    93,    94,    96,
      97,   109,   116,   117,   126,   144,   153,   205,   208,   242,
     268,   270,   291,   302,   303,   323,    70,   170,   169,   115,
     359,   230,   230,   178,   155,   178,   319,   179,   319,   107,
     253,   253,   253,   253,   170,   271,     1,   171,   223,   224,
     215,   216,   243,   324,     3,   244,     1,    60,   358,   307,
     307,     1,     3,    71,    96,    97,   107,   117,   311,   312,
     313,   324,   327,   307,     1,   107,   308,   309,   310,   324,
     327,     1,   244,     1,   244,     1,    55,   269,   358,     1,
       3,   328,   329,   171,   213,   214,   171,     3,    33,    34,
      37,    38,    39,    43,    68,    71,    78,    80,    81,    82,
      83,    84,    85,    88,    95,    98,   101,   102,   103,   104,
     105,   118,   119,   120,   135,   136,   151,   241,   278,   306,
       3,   171,   239,   240,   268,    53,   253,   306,     3,   380,
     204,   360,    54,   178,   230,   320,   172,   324,   325,   325,
     325,   325,   173,   314,   169,   170,    65,    66,    91,    92,
     111,   112,   113,   114,   130,   131,   225,   226,   173,   218,
     219,   168,   170,   169,   215,   246,   247,   324,   168,   170,
      52,   253,   253,   253,   170,   169,   324,   170,   168,   313,
     313,   253,   170,   324,   170,   168,   310,   310,   170,   170,
     170,   170,    55,    67,   110,   137,   138,   139,   140,   141,
     142,   143,   145,   146,   147,   148,   149,   171,   330,   332,
     333,   334,   336,   337,     3,   170,   169,   246,   168,   170,
      73,    76,   108,   264,   325,     1,     3,   170,     1,     4,
      31,    35,    36,    40,    41,    42,    44,    47,    59,    61,
      62,    63,    67,    99,   100,   128,   132,   173,   174,   179,
     217,   227,   243,   255,   345,   345,     1,   329,     1,   253,
     279,   171,   218,   223,   224,   240,   253,   171,   324,   225,
     226,   231,   168,   170,   262,   264,   253,   359,   170,   154,
     161,   163,   165,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   361,   362,   363,   364,   365,   368,   369,   178,
     180,     3,     3,     3,     3,     1,     8,   171,   171,   240,
     230,   168,   168,   168,   168,     3,     8,     9,   171,   221,
     179,   243,   254,   256,   257,   215,   230,   324,     3,   326,
     325,   325,   230,   313,   311,   325,   310,   308,   171,     3,
       6,   339,   340,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,     1,     3,    79,    89,   338,
      55,   330,   170,   169,   170,   169,   170,   274,   230,   328,
     172,   172,   172,   244,   219,   172,   168,   170,   170,   171,
     243,   254,     1,    50,   155,   345,   351,   171,   171,   171,
     254,   243,   171,   254,   345,    72,   155,   350,   171,   254,
     171,   171,   171,   171,   221,   163,   171,   243,   231,   170,
     345,   352,   352,   170,   171,   177,   248,    10,   169,   170,
     170,   170,   324,   329,    36,    40,    59,    67,   126,   205,
     270,   281,   282,   283,   285,   286,   287,   289,   294,   299,
     240,   240,   218,   170,   223,   264,   181,   231,   321,   322,
     171,   172,   172,   172,   239,     1,   244,   325,    54,    56,
     362,   368,   365,   368,   155,   369,   172,   247,   247,   247,
     169,   247,   181,   231,   315,   316,   170,   170,   226,   225,
      66,   226,    65,   225,   220,   230,   231,   177,   248,   169,
     168,   170,   247,     3,   244,   244,   244,   230,   169,   169,
     168,   170,    79,    89,   232,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   172,   177,   248,
     338,   338,   161,   162,   168,   341,   171,   221,   171,   221,
     272,   171,   170,   170,     5,   231,   170,   169,    50,     3,
      50,   345,   230,   230,   230,   170,   170,     1,   243,   255,
     169,     3,   351,     1,   230,   170,   230,   230,     1,   230,
     220,   352,     1,    79,    89,   163,   228,   229,   230,   180,
     231,   230,   177,   100,   174,   217,   227,   230,   100,   217,
     227,   230,   329,   170,   155,   301,   171,   171,   171,   214,
     306,   280,   285,     1,   285,   170,   170,   240,     3,   304,
     305,     3,   244,   305,     3,   172,   168,   172,   231,   240,
     240,   170,   170,   244,   381,   155,   155,   162,   182,   183,
     184,   369,   370,   230,     3,   172,   168,   172,   172,   172,
     172,   172,   172,   172,   168,   172,   155,   180,   230,   177,
     230,   256,   170,   277,   170,   170,   170,   172,   230,   171,
     230,   339,   230,   232,   230,   232,   150,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   230,
     177,   341,   341,     3,    12,    20,   220,   331,     1,   331,
      69,    70,    86,   205,   208,   233,   234,   235,   353,   354,
     355,   356,   357,   354,   168,   172,   230,    50,   346,   172,
     172,   172,   172,   169,   230,    72,   348,    72,   172,   172,
     172,   172,   172,   172,   172,   172,   230,   230,   172,    85,
     168,   172,   172,   335,   230,   171,   171,   230,   230,   170,
     171,   230,   230,   170,   170,     3,   297,   282,   230,     3,
     296,   298,   230,   264,   253,   152,    49,   170,   169,   168,
     170,   170,   170,   171,   321,   172,   170,   170,   170,   182,
     183,   184,   155,   170,   171,   315,   220,   230,   335,   230,
     276,   171,   333,   336,   155,   230,   150,   150,   230,   342,
     220,   342,   342,   342,   220,   342,   220,   220,   220,   220,
     220,   230,    12,    20,    12,    20,   248,   338,   338,   168,
     172,   172,   172,    98,   326,   326,   374,    98,   326,   168,
     170,   355,   352,   172,     5,   170,   170,   208,   209,   210,
       1,    45,   211,   212,   231,     1,   212,     1,   212,   345,
     230,   170,   210,   352,   352,   345,   352,   345,   345,   352,
     229,   229,   170,   230,   335,   230,   170,   170,   230,   170,
     170,   300,    50,   172,   169,   170,   172,   244,   223,   264,
     288,   230,   304,     1,   172,   230,   370,   172,   230,   168,
     172,   155,   230,   335,   208,   233,   236,   237,   238,   236,
     170,   170,   230,   168,   230,   230,   168,     3,   168,   343,
     344,   168,   168,   168,   168,   172,   168,   172,   172,   172,
     172,   172,     3,   171,     3,   171,     3,   171,     3,   171,
     172,   172,   220,   253,     3,     3,   253,   253,     3,     3,
     353,   273,   170,   172,   208,   351,    51,   155,   155,   352,
       1,    51,   211,   155,    51,    51,    51,    51,   170,   351,
      49,    49,   178,   230,   172,   172,   282,   290,   230,   230,
     170,   305,   244,   305,   285,   172,   172,   170,   172,   220,
     230,   178,   230,   168,   170,   237,   345,   172,   155,   230,
     220,   243,   172,   168,   220,   220,   220,   230,   170,   220,
     170,   170,   170,   170,   170,   172,   232,   172,   232,   172,
     232,   172,   232,   325,   247,   247,   325,   325,   247,   247,
      57,   210,   170,   347,   352,   352,   352,     1,   230,    72,
     352,   352,   178,   227,   227,    50,    45,   231,   292,   293,
     170,   170,   170,   170,   172,   178,     3,   233,    52,   170,
     230,   172,   343,   170,   243,   343,   168,   343,   343,   168,
     335,   335,   335,   335,     3,     3,     3,   352,    50,   170,
     170,   349,   230,   230,   155,   285,   155,    51,   293,   298,
     247,   210,   172,   172,   220,   172,   172,   220,   230,   230,
     230,   230,   247,   247,   247,   275,   255,     1,   255,   170,
     170,   285,   284,   285,   172,   345,   170,   170,   343,   170,
     170,   343,   172,   172,   172,   172,    57,   169,   172,   169,
     295,    52,   172,   172,   172,   172,   172,   172,   230,   345,
     230,   285,   170,   170,   172,   172,   345,   345
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
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
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


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
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
		  Type, Value); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
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
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
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
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
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
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

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
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


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
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

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

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

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

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

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
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
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

  yystate = yyn;
  *++yyvsp = yylval;

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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 6:

/* Line 1806 of yacc.c  */
#line 256 "parser.y"
    {
    (yyval. CONST ) = OMCreator::instance()->createBasedNum((yyvsp[(1) - (1)]. text ));
}
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 260 "parser.y"
    {
    (yyval. CONST ) = new Num(atol((yyvsp[(1) - (1)]. text )));
}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 264 "parser.y"
    {
    (yyval. CONST ) = OMCreator::instance()->createBasedNum(string((yyvsp[(1) - (2)]. text )) + string((yyvsp[(2) - (2)]. text )));
}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 270 "parser.y"
    {
    (yyval. attribtable ) = (yyvsp[(2) - (3)]. attribtable );
}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 274 "parser.y"
    {
    (yyval. attribtable ) = NULL;
}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 278 "parser.y"
    {
    (yyval. attribtable ) = NULL;
}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 284 "parser.y"
    {
    (yyval. attribtable )->insert(*((yyvsp[(3) - (3)]. idassp )));
    delete (yyvsp[(3) - (3)]. idassp );
}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 289 "parser.y"
    {
    (yyval. attribtable ) = new std::map < std::string, Expr *, compare_str > ();
    (yyval. attribtable )->insert(*((yyvsp[(1) - (1)]. idassp )));
	OMCreator::instance()->setCurrAttribTable((yyval. attribtable ));
    delete (yyvsp[(1) - (1)]. idassp );
}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 298 "parser.y"
    {
    (yyval. idassp ) = new std::pair < char *, Expr * >();
    (yyval. idassp )->first = (yyvsp[(1) - (1)]. text );
    (yyval. idassp )->second = NULL;
}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 304 "parser.y"
    {
    (yyval. idassp ) = new std::pair < char *, Expr * >();
    (yyval. idassp )->first = (yyvsp[(1) - (3)]. text );
    (yyval. idassp )->second = (yyvsp[(3) - (3)]. expr );
}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 313 "parser.y"
    {
    OMCreator::instance()->createBlockDecls((yyvsp[(1) - (6)]. attribtable ), (yyvsp[(5) - (6)]. reginfos ), (yyvsp[(4) - (6)]. range ), (yyvsp[(3) - (6)]. BOOL ), "reg");
}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 317 "parser.y"
    {
    OMCreator::instance()->createBlockDecls((yyvsp[(1) - (5)]. attribtable ), (yyvsp[(4) - (5)]. reginfos ), NULL, (yyvsp[(3) - (5)]. BOOL ), "reg");
}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 321 "parser.y"
    {
    OMCreator::instance()->createBlockDecls((yyvsp[(1) - (4)]. attribtable ), (yyvsp[(3) - (4)]. reginfos ), NULL, true, "integer");
}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 325 "parser.y"
    {
    OMCreator::instance()->createBlockDecls(NULL, (yyvsp[(2) - (3)]. reginfos ), NULL, false, "time");
}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 329 "parser.y"
    {
    OMCreator::instance()->createBlockDecls(NULL, (yyvsp[(2) - (3)]. iddims ), "real");
}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 333 "parser.y"
    {
    OMCreator::instance()->createBlockDecls(NULL, (yyvsp[(2) - (3)]. iddims ), "realtime");
}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 337 "parser.y"
    {
    OMCreator::instance()->addParams((yyvsp[(2) - (3)]. params ));
}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 341 "parser.y"
    {
    OMCreator::instance()->addLocalParams((yyvsp[(2) - (3)]. lparams ));
}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 344 "parser.y"
    {
    //Presently being added to module. Should be
	//added to current scope
    OMCreator::instance()->createEvents((yyvsp[(2) - (3)]. iddims ));
}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 351 "parser.y"
    {
    yyerrok;
}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 355 "parser.y"
    {
    yyerrok;
}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 359 "parser.y"
    {
    yyerrok;
}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 363 "parser.y"
    {
    yyerrok;
}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 367 "parser.y"
    {
    yyerrok;
}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 371 "parser.y"
    {
    yyerrok;
}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 375 "parser.y"
    {
    yyerrok;
}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 385 "parser.y"
    {
    (yyval. caseitem ) = (yyvsp[(1) - (3)]. exprs ) ? new CaseItem(*(yyvsp[(1) - (3)]. exprs ), (yyvsp[(3) - (3)]. stmt )) : 0;
}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 389 "parser.y"
    {
    std::vector < Expr * >defexpr;
    defexpr.push_back(new VString("default"));
    (yyval. caseitem ) = new CaseItem(defexpr, (yyvsp[(3) - (3)]. stmt ));
}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 395 "parser.y"
    {
    std::vector < Expr * >defexpr;
    defexpr.push_back(new VString("default"));
    (yyval. caseitem ) = new CaseItem(defexpr, (yyvsp[(2) - (2)]. stmt ));
}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 401 "parser.y"
    {
    (yyval. caseitem ) = NULL;
}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 407 "parser.y"
    {
    (yyval. caseitems ) = (yyvsp[(1) - (2)]. caseitems );
    (yyval. caseitems )->push_back((yyvsp[(2) - (2)]. caseitem ));
}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 412 "parser.y"
    {
    (yyval. caseitems ) = new std::vector < CaseItem * >();
    (yyval. caseitems )->push_back((yyvsp[(1) - (1)]. caseitem ));
}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 423 "parser.y"
    {
    Expr *id = OMCreator::instance()->createIdUse((yyvsp[(1) - (3)]. text ));
    if ((yyvsp[(1) - (3)]. text )) {
        (yyval. defparam ) = new Defparam(id, (yyvsp[(3) - (3)]. expr ));
    }
}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 432 "parser.y"
    {
    (yyval. defparams ) = new std::vector < Defparam * >();
    (yyval. defparams )->push_back((yyvsp[(1) - (1)]. defparam ));
}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 437 "parser.y"
    {
    (yyval. defparams ) = new std::vector < Defparam * >();
    (yyvsp[(2) - (2)]. defparam )->setRange((yyvsp[(1) - (2)]. range ));
    (yyval. defparams )->push_back((yyvsp[(2) - (2)]. defparam ));
}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 443 "parser.y"
    {
    (yyval. defparams ) = (yyvsp[(1) - (3)]. defparams );
    (yyval. defparams )->push_back((yyvsp[(3) - (3)]. defparam ));
}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 450 "parser.y"
    {
    (yyval. delay ) = new Delay((yyvsp[(2) - (2)]. dval ));
}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 454 "parser.y"
    {
    (yyval. delay ) = new Delay((yyvsp[(3) - (4)]. dval ));
}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 460 "parser.y"
    {
    (yyval. delay ) = new Delay((yyvsp[(2) - (2)]. dval ));
}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 464 "parser.y"
    {
    (yyval. delay ) = new Delay((yyvsp[(3) - (4)]. dval ));
}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 468 "parser.y"
    {
    (yyval. delay ) = new Delay((yyvsp[(3) - (6)]. dval ), (yyvsp[(5) - (6)]. dval ));
}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 472 "parser.y"
    {
    (yyval. delay ) = new Delay((yyvsp[(3) - (8)]. dval ), (yyvsp[(5) - (8)]. dval ), (yyvsp[(7) - (8)]. dval ));
}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 478 "parser.y"
    {
    (yyval. delay ) = (yyvsp[(1) - (1)]. delay );
}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 482 "parser.y"
    {
    (yyval. delay ) = NULL;
}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 488 "parser.y"
    {
    (yyval. dval ) = new DelayVal((yyvsp[(1) - (1)]. expr ));
}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 492 "parser.y"
    {
    (yyval. dval ) = new DelayVal((yyvsp[(1) - (5)]. expr ), (yyvsp[(3) - (5)]. expr ), (yyvsp[(5) - (5)]. expr ));
}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 498 "parser.y"
    {
    (yyval. dval ) = new DelayVal(new Num(atol(yytext)));
}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 502 "parser.y"
    {
    (yyval. dval ) = new DelayVal(new RealConst(yytext));
}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 506 "parser.y"
    {
    (yyval. dval ) = new DelayVal(OMCreator::instance()->createIdUse((yyvsp[(1) - (1)]. text )));
}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 514 "parser.y"
    {OMCreator::instance()->setDefNetType((yyvsp[(2) - (2)]. nettype ));}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 515 "parser.y"
    {
}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 520 "parser.y"
    {
    (yyval. dst ) = new DriveSt((yyvsp[(2) - (5)]. drstr ), (yyvsp[(4) - (5)]. drstr ));
}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 524 "parser.y"
    {
    (yyval. dst ) = new DriveSt((yyvsp[(2) - (5)]. drstr ), (yyvsp[(4) - (5)]. drstr ));
}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 528 "parser.y"
    {
    (yyval. dst ) = new DriveSt((yyvsp[(2) - (5)]. drstr ), DHIGHZ1);
}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 532 "parser.y"
    {
    (yyval. dst ) = new DriveSt((yyvsp[(2) - (5)]. drstr ), DHIGHZ0);
}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 536 "parser.y"
    {
    (yyval. dst ) = new DriveSt(DHIGHZ1, (yyvsp[(4) - (5)]. drstr ));
}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 540 "parser.y"
    {
    (yyval. dst ) = new DriveSt(DHIGHZ0, (yyvsp[(4) - (5)]. drstr ));
}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 546 "parser.y"
    {
    (yyval. dst ) = (yyvsp[(1) - (1)]. dst );
}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 550 "parser.y"
    {
    (yyval. dst ) = NULL;
}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 556 "parser.y"
    {
    (yyval. drstr ) = DSUPPLY0;
}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 560 "parser.y"
    {
    (yyval. drstr ) = DSTRONG0;
}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 564 "parser.y"
    {
    (yyval. drstr ) = DPULL0;
}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 568 "parser.y"
    {
    (yyval. drstr ) = DWEAK0;
}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 574 "parser.y"
    {
    (yyval. drstr ) = DSUPPLY1;
}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 578 "parser.y"
    {
    (yyval. drstr ) = DSTRONG1;
}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 582 "parser.y"
    {
    (yyval. drstr ) = DPULL1;
}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 586 "parser.y"
    {
    (yyval. drstr ) = DWEAK1;
}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 592 "parser.y"
    {
    Expr *expr = OMCreator::instance()->createIdUse((yyvsp[(2) - (2)]. text ));
    std::vector < std::pair < EdgeType, Expr * > >edgeexprs;
    edgeexprs.push_back(std::make_pair(NOEDGE, expr));
    (yyval. evctrl ) = new EventCntrl(edgeexprs, NULL, false);
}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 599 "parser.y"
    {
    (yyval. evctrl ) = new EventCntrl((*(yyvsp[(3) - (4)]. edexprs )), NULL, false);
    delete (yyvsp[(3) - (4)]. edexprs );
}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 604 "parser.y"
    {
(yyval. evctrl ) = NULL;}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 609 "parser.y"
    {
    (yyval. edexprs ) = new std::vector < std::pair < EdgeType, Expr * > >();
    (yyval. edexprs )->push_back(*((yyvsp[(1) - (1)]. edexpr )));
    delete (yyvsp[(1) - (1)]. edexpr );
}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 615 "parser.y"
    {
    (yyval. edexprs ) = (yyvsp[(1) - (3)]. edexprs );
    (yyval. edexprs )->push_back(*((yyvsp[(3) - (3)]. edexpr )));
    delete (yyvsp[(3) - (3)]. edexpr );
}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 621 "parser.y"
    {
    (yyval. edexprs ) = (yyvsp[(1) - (3)]. edexprs );
    (yyval. edexprs )->push_back(*((yyvsp[(3) - (3)]. edexpr )));
    delete (yyvsp[(3) - (3)]. edexpr );

}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 630 "parser.y"
    {
    (yyval. edexpr ) = new std::pair < EdgeType, Expr * >(POSEDGE, (yyvsp[(2) - (2)]. expr ));
}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 634 "parser.y"
    {
    (yyval. edexpr ) = new std::pair < EdgeType, Expr * >(NEGEDGE, (yyvsp[(2) - (2)]. expr ));
}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 638 "parser.y"
    {
    (yyval. edexpr ) = new std::pair < EdgeType, Expr * >(NOEDGE, (yyvsp[(1) - (1)]. expr ));
}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 644 "parser.y"
    {
    (yyval. expr ) = (yyvsp[(1) - (1)]. expr );
}
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 648 "parser.y"
    {
    (yyval. expr ) = new UnaryExpr((yyvsp[(2) - (2)]. expr ), UNPLUS);
}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 652 "parser.y"
    {
    (yyval. expr ) = new UnaryExpr((yyvsp[(2) - (2)]. expr ), UNMINUS);
}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 656 "parser.y"
    {
    (yyval. expr ) = new UnaryExpr((yyvsp[(2) - (2)]. expr ), BITNEG);
}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 660 "parser.y"
    {
    (yyval. expr ) = new UnaryExpr((yyvsp[(2) - (2)]. expr ), REDAND);
}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 664 "parser.y"
    {
    (yyval. expr ) = new UnaryExpr((yyvsp[(2) - (2)]. expr ), LOGNEG);
}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 668 "parser.y"
    {
    (yyval. expr ) = new UnaryExpr((yyvsp[(2) - (2)]. expr ), REDOR);
}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 672 "parser.y"
    {
    (yyval. expr ) = new UnaryExpr((yyvsp[(2) - (2)]. expr ), REDXOR);
}
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 676 "parser.y"
    {
    (yyval. expr ) = new UnaryExpr((yyvsp[(2) - (2)]. expr ), REDNAND);
}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 680 "parser.y"
    {
    (yyval. expr ) = new UnaryExpr((yyvsp[(2) - (2)]. expr ), REDNOR);
}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 684 "parser.y"
    {
    (yyval. expr ) = new UnaryExpr((yyvsp[(2) - (2)]. expr ), REDXNOR);
}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 688 "parser.y"
    {
    (yyval. expr ) = NULL;

}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 693 "parser.y"
    {
    (yyval. expr ) = NULL;

}
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 698 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), BITXOR);
}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 702 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), MULT);
}
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 706 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), DIV);
}
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 710 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), MOD);
}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 714 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), PLUS);
}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 718 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), MINUS);
}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 722 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), POW);
}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 725 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), BITAND);
}
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 729 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), BITOR);
}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 733 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), BITNAND);
}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 737 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), BITNOR);
}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 741 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), BITNXOR);
}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 745 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), LTHAN);
}
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 749 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), GTHAN);
}
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 753 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), LSHIFT);
}
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 757 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), RSHIFT);
}
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 761 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), RRSHIFT);
}
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 765 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), LOGEQ);
}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 769 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), CASEEQ);
}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 773 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), LTHANEQ);
}
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 777 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), GTHANEQ);
}
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 781 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), LOGENEQ);
}
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 785 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), CASEENEQ);
}
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 789 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), LOGOR);
}
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 793 "parser.y"
    {
    (yyval. expr ) = new BinExpr((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ), LOGAND);
}
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 797 "parser.y"
    {
    (yyval. expr ) = new CondExpr((yyvsp[(1) - (5)]. expr ), (yyvsp[(3) - (5)]. expr ), (yyvsp[(5) - (5)]. expr ));
}
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 803 "parser.y"
    {
    if ((yyvsp[(1) - (3)]. exprs )) {
        (yyval. exprs ) = (yyvsp[(1) - (3)]. exprs );
    } else {
        (yyval. exprs ) = new std::vector < Expr * >();
        // for case like inst( , in);
        (yyval. exprs )->push_back(NULL);
    }
    (yyval. exprs )->push_back((yyvsp[(3) - (3)]. expr ));
}
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 814 "parser.y"
    {
    (yyval. exprs ) = new std::vector < Expr * >();
    (yyval. exprs )->push_back((yyvsp[(1) - (1)]. expr ));
}
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 819 "parser.y"
    {
    (yyval. exprs ) = NULL;
}
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 823 "parser.y"
    {
    if((yyval. exprs ) == NULL) {
        (yyval. exprs ) = new std::vector < Expr * >();
	}
    (yyval. exprs )->push_back(NULL);
}
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 832 "parser.y"
    {
    (yyval. expr ) = (yyvsp[(1) - (1)]. CONST );
}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 836 "parser.y"
    {
    (yyval. expr ) = new RealConst(yytext);
}
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 840 "parser.y"
    {
    (yyval. expr ) = new VString((yyvsp[(1) - (1)]. text ));
}
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 844 "parser.y"
    {
    (yyval. expr ) = OMCreator::instance()->createIdUse((yyvsp[(1) - (1)]. text ), true);
    if(!((yyval. expr ))){
        (yyval. expr ) = new ImplicitNet((yyvsp[(1) - (1)]. text ), OMCreator::instance()->getDefNetType());
    }
}
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 851 "parser.y"
    {
    (yyval. expr ) = new FuncCall((yyvsp[(1) - (1)]. text ), std::vector < Expr * >(), true);
}
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 855 "parser.y"
    {
    (yyval. expr ) = OMCreator::instance()->createBitSelect((yyvsp[(1) - (2)]. text ), (yyvsp[(2) - (2)]. exprs ));
}
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 859 "parser.y"
    {
    (yyval. expr ) = OMCreator::instance()->createPartSelect((yyvsp[(1) - (7)]. text ),
                             new Range((yyvsp[(4) - (7)]. expr ), (yyvsp[(6) - (7)]. expr )), (yyvsp[(2) - (7)]. exprs ), (yyvsp[(5) - (7)]. pop ));
}
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 864 "parser.y"
    {
    (yyval. expr ) = OMCreator::instance()->createPartSelect((yyvsp[(1) - (6)]. text ),
                             new Range((yyvsp[(3) - (6)]. expr ), (yyvsp[(5) - (6)]. expr )), NULL,
                             (yyvsp[(4) - (6)]. pop ));
}
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 870 "parser.y"
    {
    //TBD: resolve function call
    std::vector < Expr * >vexpr;
    if ((yyvsp[(3) - (4)]. exprs )) {
        vexpr = *((yyvsp[(3) - (4)]. exprs ));
    }
    (yyval. expr ) = new FuncCall((yyvsp[(1) - (4)]. text ), vexpr, false);
}
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 879 "parser.y"
    {
    (yyval. expr ) = new FuncCall((yyvsp[(1) - (4)]. text ), (yyvsp[(3) - (4)]. exprs ) ? *((yyvsp[(3) - (4)]. exprs )) : std::vector < Expr * >(), true);
}
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 883 "parser.y"
    {
    (yyval. expr ) = (yyvsp[(2) - (3)]. expr );
}
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 887 "parser.y"
    {
    (yyval. expr ) = new Concat(*(yyvsp[(2) - (3)]. exprs ));
    delete (yyvsp[(2) - (3)]. exprs );
}
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 892 "parser.y"
    {
    (yyval. expr ) = new MultiConcat((yyvsp[(2) - (6)]. expr ), *(yyvsp[(4) - (6)]. exprs ));
    delete (yyvsp[(4) - (6)]. exprs );
}
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 899 "parser.y"
    {
    OMCreator::instance()->addTaskFuncPort((yyvsp[(5) - (6)]. text ), IN, (yyvsp[(4) - (6)]. range ), (yyvsp[(6) - (6)]. ranges ), (yyvsp[(3) - (6)]. BOOL ), 5);
    (yyval. dirsigtyperange ) = new std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >(std::make_pair(IN, (yyvsp[(3) - (6)]. BOOL )), std::make_pair(5, (yyvsp[(4) - (6)]. range )));
}
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 905 "parser.y"
    {
    OMCreator::instance()->addTaskFuncPort((yyvsp[(3) - (4)]. text ), IN, (yyvsp[(2) - (4)]. typerange ) ? (yyvsp[(2) - (4)]. typerange )->second :
                           NULL, (yyvsp[(4) - (4)]. ranges ), false,
                           (yyvsp[(2) - (4)]. typerange ) ? (yyvsp[(2) - (4)]. typerange )->first : -1);
    (yyval. dirsigtyperange ) = new std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >();
    (yyval. dirsigtyperange )->first = std::make_pair(IN, false);
    if ((yyvsp[(2) - (4)]. typerange )) {
        (yyval. dirsigtyperange )->second = *((yyvsp[(2) - (4)]. typerange ));
        delete (yyvsp[(2) - (4)]. typerange );
    }
}
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 920 "parser.y"
    {
    OMCreator::instance()->addTaskFuncPort((yyvsp[(5) - (6)]. text ), OUT, (yyvsp[(4) - (6)]. range ), (yyvsp[(6) - (6)]. ranges ), (yyvsp[(3) - (6)]. BOOL ), 5);
    (yyval. dirsigtyperange ) = new std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >();
    (yyval. dirsigtyperange )->first = std::make_pair(OUT, (yyvsp[(3) - (6)]. BOOL ));
    (yyval. dirsigtyperange )->second = std::make_pair(5, (yyvsp[(4) - (6)]. range ));
}
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 928 "parser.y"
    {
    OMCreator::instance()->addTaskFuncPort((yyvsp[(3) - (4)]. text ), OUT, (yyvsp[(2) - (4)]. typerange ) ? (yyvsp[(2) - (4)]. typerange )->second :
                           NULL, (yyvsp[(4) - (4)]. ranges ), false,
                           (yyvsp[(2) - (4)]. typerange ) ? (yyvsp[(2) - (4)]. typerange )->first : -1);
    (yyval. dirsigtyperange ) = new std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >();
    (yyval. dirsigtyperange )->first = std::make_pair(OUT, false);
    if ((yyvsp[(2) - (4)]. typerange )) {
        (yyval. dirsigtyperange )->second = *((yyvsp[(2) - (4)]. typerange ));
        delete (yyvsp[(2) - (4)]. typerange );
    }
}
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 943 "parser.y"
    {
    OMCreator::instance()->addTaskFuncPort((yyvsp[(5) - (6)]. text ), INOUT, (yyvsp[(4) - (6)]. range ), (yyvsp[(6) - (6)]. ranges ), (yyvsp[(3) - (6)]. BOOL ), 5);
    (yyval. dirsigtyperange ) = new std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >();
    (yyval. dirsigtyperange )->first = std::make_pair(INOUT, (yyvsp[(3) - (6)]. BOOL ));
    (yyval. dirsigtyperange )->second = std::make_pair(5, (yyvsp[(4) - (6)]. range ));
}
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 951 "parser.y"
    {
    OMCreator::instance()->addTaskFuncPort((yyvsp[(3) - (4)]. text ), OUT, (yyvsp[(2) - (4)]. typerange ) ? (yyvsp[(2) - (4)]. typerange )->second :
                           NULL, (yyvsp[(4) - (4)]. ranges ), false,
                           (yyvsp[(2) - (4)]. typerange ) ? (yyvsp[(2) - (4)]. typerange )->first : -1);
    (yyval. dirsigtyperange ) = new std::pair < std::pair < Dir, bool >, std::pair < int,
        Range * > >();
    (yyval. dirsigtyperange )->first = std::make_pair(INOUT, false);
    if ((yyvsp[(2) - (4)]. typerange )) {
        (yyval. dirsigtyperange )->second = *((yyvsp[(2) - (4)]. typerange ));
        delete (yyvsp[(2) - (4)]. typerange );
    }
}
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 966 "parser.y"
    {
    (yyval. dirsigtyperange ) = (yyvsp[(1) - (1)]. dirsigtyperange );
}
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 970 "parser.y"
    {
    (yyval. dirsigtyperange ) = (yyvsp[(3) - (3)]. dirsigtyperange );
}
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 974 "parser.y"
    {
    (yyval. dirsigtyperange ) = (yyvsp[(1) - (4)]. dirsigtyperange );
    OMCreator::instance()->addTaskFuncPort((yyvsp[(3) - (4)]. text ), IN, (yyval. dirsigtyperange )->second.second,
                           (yyvsp[(4) - (4)]. ranges ), (yyval. dirsigtyperange )->first.second,
                           (yyval. dirsigtyperange )->second.first);
}
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 984 "parser.y"
    {
}
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 989 "parser.y"
    {
}
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 992 "parser.y"
    {
}
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 997 "parser.y"
    {
    std::vector < NameConn * >*nameConns = NULL;
    if ((yyvsp[(3) - (4)]. exprs )) {
        nameConns = new std::vector < NameConn * >();
        for (unsigned int i = 0; i < (yyvsp[(3) - (4)]. exprs )->size(); ++i) {
            nameConns->push_back(new NameConn(std::string(),
                              (*(yyvsp[(3) - (4)]. exprs ))[i]));
        }
    }
    (yyval. inst ) = OMCreator::instance()->createInst((yyvsp[(1) - (4)]. text ), nameConns);
	if((yyval. inst )){
        (yyval. inst )->setAttribTable(OMCreator::instance()->getCurrAttribTable());
	}	
}
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 1012 "parser.y"
    {
    std::vector < NameConn * >*nameConns = NULL;
    if ((yyvsp[(4) - (5)]. exprs )) {
        nameConns = new std::vector < NameConn * >();
        for (unsigned int i = 0; i < (yyvsp[(4) - (5)]. exprs )->size(); ++i) {
            nameConns->push_back(new NameConn(std::string(),
                              (*(yyvsp[(4) - (5)]. exprs ))[i]));
        }
    }
    (yyval. inst ) = OMCreator::instance()->createInst((yyvsp[(1) - (5)]. text ), nameConns);
    (yyval. inst )->setAttribTable(OMCreator::instance()->getCurrAttribTable());
}
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 1025 "parser.y"
    {
    std::vector < NameConn * >*nameConns = NULL;
    if ((yyvsp[(2) - (3)]. exprs )) {
        nameConns = new std::vector < NameConn * >();
        for (unsigned int i = 0; i < (yyvsp[(2) - (3)]. exprs )->size(); ++i) {
            nameConns->push_back(new NameConn(std::string(),
                              (*(yyvsp[(2) - (3)]. exprs ))[i]));
        }
    }
    (yyval. inst ) = OMCreator::instance()->
        createInst(GetGateName(OMCreator::instance()->getCurrGateType()),
               nameConns);
    (yyval. inst )->setAttribTable(OMCreator::instance()->getCurrAttribTable());
}
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 1041 "parser.y"
    {
    (yyval. inst ) = OMCreator::instance()->createInst((yyvsp[(1) - (2)]. text ), NULL);
    (yyval. inst )->setAttribTable(OMCreator::instance()->getCurrAttribTable());
}
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 1047 "parser.y"
    {
    (yyval. inst ) = OMCreator::instance()->createInst((yyvsp[(1) - (4)]. text ), (yyvsp[(3) - (4)]. nconns ));
}
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 1053 "parser.y"
    {
    (yyval. insts ) = (yyvsp[(1) - (3)]. insts );
    (yyval. insts )->push_back((yyvsp[(3) - (3)]. inst ));
}
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 1058 "parser.y"
    {
    (yyval. insts ) = new std::vector < Inst * >();
    (yyval. insts )->push_back((yyvsp[(1) - (1)]. inst ));
}
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 1065 "parser.y"
    {
    (yyval. gtype ) = AND;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 1070 "parser.y"
    {
    (yyval. gtype ) = NAND;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 1075 "parser.y"
    {
    (yyval. gtype ) = OR;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 1080 "parser.y"
    {
    (yyval. gtype ) = NOR;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 1085 "parser.y"
    {
    (yyval. gtype ) = XOR;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 1090 "parser.y"
    {
    (yyval. gtype ) = XNOR;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 1095 "parser.y"
    {
    (yyval. gtype ) = BUF;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 1100 "parser.y"
    {
    (yyval. gtype ) = BUFIF0;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 1105 "parser.y"
    {
    (yyval. gtype ) = BUFIF1;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 1110 "parser.y"
    {
    (yyval. gtype ) = NOT;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 1115 "parser.y"
    {
    (yyval. gtype ) = NOTIF0;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 1120 "parser.y"
    {
    (yyval. gtype ) = NOTIF1;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 1125 "parser.y"
    {
    (yyval. gtype ) = NMOS;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 1130 "parser.y"
    {
    (yyval. gtype ) = RNMOS;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 1135 "parser.y"
    {
    (yyval. gtype ) = PMOS;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 1140 "parser.y"
    {
    (yyval. gtype ) = RPMOS;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 1145 "parser.y"
    {
    (yyval. gtype ) = CMOS;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 1150 "parser.y"
    {
    (yyval. gtype ) = RCMOS;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 1155 "parser.y"
    {
    (yyval. gtype ) = TRAN;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 1160 "parser.y"
    {
    (yyval. gtype ) = RTRAN;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 1165 "parser.y"
    {
    (yyval. gtype ) = TRANIF0;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 1170 "parser.y"
    {
    (yyval. gtype ) = TRANIF1;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 1175 "parser.y"
    {
    (yyval. gtype ) = RTRANIF0;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 1180 "parser.y"
    {
    (yyval. gtype ) = RTRANIF1;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 1187 "parser.y"
    {
    (yyval. gtype ) = PULLUP;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 1191 "parser.y"
    {
    (yyval. gtype ) = PULLUP;
    OMCreator::instance()->setCurrGateType((yyval. gtype ));
}
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 1200 "parser.y"
    {
    (yyval. text ) = yylval.text;
}
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 1204 "parser.y"
    {
    (yyval. text ) = (char *)malloc(strlen((yyvsp[(1) - (3)]. text )) + 2 + strlen(yytext));
    sprintf((yyval. text ), "%s.%s", (yyvsp[(1) - (3)]. text ), yytext);
}
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 1211 "parser.y"
    {
    (yyval. iddims ) = new std::vector < std::pair < char *, std::vector < Range * >*> >();
    (yyval. iddims )->push_back(std::make_pair((yyvsp[(1) - (2)]. text ), (yyvsp[(2) - (2)]. ranges )));
}
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 1216 "parser.y"
    {
    (yyvsp[(1) - (4)]. iddims )->push_back(std::make_pair((yyvsp[(3) - (4)]. text ), (yyvsp[(4) - (4)]. ranges )));
    (yyval. iddims ) = (yyvsp[(1) - (4)]. iddims );
}
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 1221 "parser.y"
    {
    (yyval. vtext ) = new std::vector < std::pair < char *, Expr * > >();
    (yyval. vtext )->push_back(std::make_pair(yylval.text, (Expr *) 0));
}
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 1226 "parser.y"
    {
    (yyvsp[(1) - (3)]. vtext )->push_back(std::make_pair((yyvsp[(3) - (3)]. text ), (Expr *) 0));
    (yyval. vtext ) = (yyvsp[(1) - (3)]. vtext );
}
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 1233 "parser.y"
    {
    if((yyvsp[(1) - (1)]. range )) 
	{
        (yyval. ranges ) = new std::vector < Range * >();
        (yyval. ranges )->push_back((yyvsp[(1) - (1)]. range ));
	}
	else {
	    (yyval. ranges ) = 0;
	}	
}
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 1244 "parser.y"
    {
    (yyval. ranges ) = (yyvsp[(1) - (2)]. ranges );
	if((yyval. ranges ) && (yyvsp[(2) - (2)]. range )) {
        (yyval. ranges )->push_back((yyvsp[(2) - (2)]. range ));
	}	
}
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 1253 "parser.y"
    {
    (yyval. ranges ) = (yyvsp[(1) - (1)]. ranges );
}
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 1257 "parser.y"
    {
    (yyval. ranges ) = NULL;
}
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 1263 "parser.y"
    {
    (yyval. exprs ) = new std::vector < Expr * >();
    (yyval. exprs )->push_back((yyvsp[(2) - (3)]. expr ));
}
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 1268 "parser.y"
    {
    (yyval. exprs ) = (yyvsp[(1) - (4)]. exprs );
    (yyval. exprs )->push_back((yyvsp[(3) - (4)]. expr ));
}
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 1275 "parser.y"
    {
    (yyval. parseInfos ) = new std::vector < ParseInfo * >();
	if((yyvsp[(1) - (1)]. parseInfo )) {
        (yyval. parseInfos )->push_back((yyvsp[(1) - (1)]. parseInfo ));
	}	
}
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 1282 "parser.y"
    {
    if((yyvsp[(3) - (3)]. parseInfo )) {
        (yyvsp[(1) - (3)]. parseInfos )->push_back((yyvsp[(3) - (3)]. parseInfo ));
	}	
    (yyval. parseInfos ) = (yyvsp[(1) - (3)]. parseInfos );
}
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 1291 "parser.y"
    {
    (yyval. parseInfos ) = new std::vector < ParseInfo * >();
    (yyval. parseInfos )->push_back((yyvsp[(1) - (1)]. parseInfo ));
}
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 1296 "parser.y"
    {
    (yyvsp[(1) - (3)]. parseInfos )->push_back((yyvsp[(3) - (3)]. parseInfo ));
    (yyval. parseInfos ) = (yyvsp[(1) - (3)]. parseInfos );
}
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 1301 "parser.y"
    {
    ParseInfo *tpi = new ParseInfo(*(*(yyvsp[(1) - (3)]. parseInfos ))[(yyvsp[(1) - (3)]. parseInfos )->size() - 1]);
    tpi->name = (yyvsp[(3) - (3)]. text );
    (yyvsp[(1) - (3)]. parseInfos )->push_back(tpi);
    (yyval. parseInfos ) = (yyvsp[(1) - (3)]. parseInfos );
}
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 1312 "parser.y"
    {
    (yyval. parseInfo ) = new ParseInfo();
    (yyval. parseInfo )->name = std::string((yyvsp[(6) - (7)]. text ));
    (yyval. parseInfo )->isSigned = (yyvsp[(4) - (7)]. BOOL );
    (yyval. parseInfo )->range = (yyvsp[(5) - (7)]. range );
    (yyval. parseInfo )->dir = IN;
    (yyval. parseInfo )->net = (yyvsp[(3) - (7)]. nettype );
    (yyval. parseInfo )->dim = (yyvsp[(7) - (7)]. ranges );
    (yyval. parseInfo )->table = (yyvsp[(1) - (7)]. attribtable );
}
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 1324 "parser.y"
    {
    (yyval. parseInfo ) = new ParseInfo();
    (yyval. parseInfo )->name = std::string((yyvsp[(6) - (7)]. text ));
    (yyval. parseInfo )->isSigned = (yyvsp[(4) - (7)]. BOOL );
    (yyval. parseInfo )->range = (yyvsp[(5) - (7)]. range );
    (yyval. parseInfo )->dir = INOUT;
    (yyval. parseInfo )->net = (yyvsp[(3) - (7)]. nettype );
    (yyval. parseInfo )->dim = (yyvsp[(7) - (7)]. ranges );
    (yyval. parseInfo )->table = (yyvsp[(1) - (7)]. attribtable );
}
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 1336 "parser.y"
    {
    (yyval. parseInfo ) = new ParseInfo();
    (yyval. parseInfo )->name = std::string((yyvsp[(6) - (7)]. text ));
    (yyval. parseInfo )->isSigned = (yyvsp[(4) - (7)]. BOOL );
    (yyval. parseInfo )->range = (yyvsp[(5) - (7)]. range );
    (yyval. parseInfo )->dir = OUT;
    (yyval. parseInfo )->net = (yyvsp[(3) - (7)]. nettype );
    (yyval. parseInfo )->dim = (yyvsp[(7) - (7)]. ranges );
    (yyval. parseInfo )->table = (yyvsp[(1) - (7)]. attribtable );

}
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 1349 "parser.y"
    {
    (yyval. parseInfo ) = new ParseInfo();
    (yyval. parseInfo )->name = std::string((yyvsp[(6) - (7)]. text ));
    (yyval. parseInfo )->isSigned = (yyvsp[(4) - (7)]. BOOL );
    (yyval. parseInfo )->range = (yyvsp[(5) - (7)]. range );
    (yyval. parseInfo )->dir = OUT;
    (yyval. parseInfo )->net = REG;
    (yyval. parseInfo )->dim = (yyvsp[(7) - (7)]. ranges );
    (yyval. parseInfo )->table = (yyvsp[(1) - (7)]. attribtable );
}
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 1361 "parser.y"
    {
    (yyval. parseInfo ) = new ParseInfo();
    (yyval. parseInfo )->name = std::string((yyvsp[(6) - (8)]. text ));
    (yyval. parseInfo )->isSigned = (yyvsp[(4) - (8)]. BOOL );
    (yyval. parseInfo )->range = (yyvsp[(5) - (8)]. range );
    (yyval. parseInfo )->dir = OUT;
    (yyval. parseInfo )->net = REG;
    (yyval. parseInfo )->exprs.push_back((yyvsp[(8) - (8)]. expr ));
    (yyval. parseInfo )->table = (yyvsp[(1) - (8)]. attribtable );
}
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 1375 "parser.y"
    {
    (yyval. nettype ) = (yyvsp[(1) - (1)]. nettype );
}
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 1379 "parser.y"
    {
    (yyval. nettype ) = NONET;
}
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 1385 "parser.y"
    {
    (yyval. BOOL ) = true;
}
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 1389 "parser.y"
    {
    (yyval. BOOL ) = false;
}
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 1395 "parser.y"
    {
    (yyval. expr ) = OMCreator::instance()->createIdUse((yyvsp[(1) - (1)]. text ), true);
	if(!(yyval. expr )) {
	    (yyval. expr ) = new VString((yyvsp[(1) - (1)]. text ));
	}	
}
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 1402 "parser.y"
    {
    (yyval. expr ) = OMCreator::instance()->createBitSelect((yyvsp[(1) - (2)]. text ), (yyvsp[(2) - (2)]. exprs ));
}
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 1406 "parser.y"
    {
    (yyval. expr ) = OMCreator::instance()->createPartSelect((yyvsp[(1) - (7)]. text ), new Range((yyvsp[(4) - (7)]. expr ), (yyvsp[(6) - (7)]. expr )), (yyvsp[(2) - (7)]. exprs ),
                             (yyvsp[(5) - (7)]. pop ));
}
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 1411 "parser.y"
    {
    (yyval. expr ) = OMCreator::instance()->createPartSelect((yyvsp[(1) - (6)]. text ), new Range((yyvsp[(3) - (6)]. expr ), (yyvsp[(5) - (6)]. expr )),
                             NULL, (yyvsp[(4) - (6)]. pop ));
}
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 1416 "parser.y"
    {
    (yyval. expr ) = new Concat(*(yyvsp[(2) - (3)]. exprs ));
    delete (yyvsp[(2) - (3)]. exprs );
}
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 1423 "parser.y"
    {
    (yyval. expr ) = OMCreator::instance()->createIdUse((yyvsp[(1) - (1)]. text ));
}
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 1427 "parser.y"
    {
    (yyval. expr ) = OMCreator::instance()->createBitSelect((yyvsp[(1) - (2)]. text ), (yyvsp[(2) - (2)]. exprs ));
}
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 1431 "parser.y"
    {
    (yyval. expr ) = OMCreator::instance()->createPartSelect((yyvsp[(1) - (7)]. text ),
                             new Range((yyvsp[(4) - (7)]. expr ), (yyvsp[(6) - (7)]. expr )), (yyvsp[(2) - (7)]. exprs ), (yyvsp[(5) - (7)]. pop ));
}
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 1436 "parser.y"
    {
    (yyval. expr ) = ((yyvsp[(3) - (6)]. expr ) && (yyvsp[(5) - (6)]. expr )) ? OMCreator::instance()->createPartSelect((yyvsp[(1) - (6)]. text ),
                             new Range((yyvsp[(3) - (6)]. expr ), (yyvsp[(5) - (6)]. expr )), NULL,
                             (yyvsp[(4) - (6)]. pop )) : NULL;
}
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 1442 "parser.y"
    {
    (yyval. expr ) = new Concat(*(yyvsp[(2) - (3)]. exprs ));
    delete (yyvsp[(2) - (3)]. exprs );
}
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 1449 "parser.y"
    {
    (yyval. assgp ) = new std::pair < Expr *, Expr * >((yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ));
}
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 1455 "parser.y"
    {
    (yyval. assgps ) = (yyvsp[(1) - (3)]. assgps );
    (yyval. assgps )->push_back((yyvsp[(3) - (3)]. assgp ));
}
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 1460 "parser.y"
    {
    (yyval. assgps ) = new std::vector < std::pair < Expr *, Expr * >*>();
    (yyval. assgps )->push_back((yyvsp[(1) - (1)]. assgp ));
}
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 1468 "parser.y"
    {
    OMCreator::instance()->createModule((yyvsp[(1) - (3)]. attribtable ), yylval.text);
}
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 1472 "parser.y"
    {
    OMCreator::instance()->addParams((yyvsp[(5) - (5)]. params ));
}
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 1476 "parser.y"
    {
    if ((yyvsp[(7) - (8)]. parseInfos )) {
        OMCreator::instance()->setParseInfo((yyvsp[(7) - (8)]. parseInfos ));
        OMCreator::instance()->addV2kPorts();
    }
}
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 1483 "parser.y"
    {
    OMCreator::instance()->scopeEndAction();
}
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1485 "parser.y"
    {
	(yyval. mod ) = const_cast<Module*>(OMCreator::instance()->getCurrModule());
}
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1491 "parser.y"
    {
}
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1496 "parser.y"
    {
    (yyval. range ) = (yyvsp[(1) - (2)]. range );
}
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1502 "parser.y"
    {
    (yyval. parseInfos ) = (yyvsp[(2) - (3)]. parseInfos );
}
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1506 "parser.y"
    {
    (yyval. parseInfos ) = (yyvsp[(2) - (3)]. parseInfos );
}
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1510 "parser.y"
    {
    (yyval. parseInfos ) = NULL;
}
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 1517 "parser.y"
    {
    (yyval. params ) = (yyvsp[(3) - (4)]. params );
}
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 1521 "parser.y"
    {
    (yyval. params ) = NULL;
}
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 1528 "parser.y"
    {
    std::vector < ParseInfo * >*tpinfo = new std::vector < ParseInfo * >();
    tpinfo->push_back((yyvsp[(3) - (3)]. parseInfo ));
    (yyval. params ) = (OMCreator::instance()->createParams < Param, VecParam > (tpinfo));
}
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 1534 "parser.y"
    {
    std::vector < ParseInfo * >*tpinfo = new std::vector < ParseInfo * >();
    tpinfo->push_back((yyvsp[(3) - (3)]. parseInfo ));
    std::vector < Param * >*tpv =
        OMCreator::instance()->createParams < Param, VecParam > (tpinfo);
    for (unsigned int i = 0; i < tpv->size(); ++i) {
        (yyval. params )->push_back((*tpv)[i]);
    }
    delete tpv;
}
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 1545 "parser.y"
    {
    std::vector < ParseInfo * >*tpinfo = new std::vector < ParseInfo * >();
    tpinfo->push_back((yyvsp[(4) - (4)]. parseInfo ));
    std::vector < Param * >*tpv =
        OMCreator::instance()->createParams < Param, VecParam > (tpinfo);
    for (unsigned int i = 0; i < tpv->size(); ++i) {
        (yyval. params )->push_back((*tpv)[i]);
    }
    delete tpv;
}
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 1563 "parser.y"
    {
    OMCreator::instance()->addNets((yyvsp[(1) - (6)]. attribtable ), (yyvsp[(2) - (6)]. nettype ), (yyvsp[(3) - (6)]. BOOL ), (yyvsp[(4) - (6)]. range ), (yyvsp[(5) - (6)]. iddims ));
}
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 1567 "parser.y"
    {
    OMCreator::instance()->addNets((yyvsp[(1) - (6)]. attribtable ), (yyvsp[(2) - (6)]. nettype ), (yyvsp[(3) - (6)]. BOOL ), (yyvsp[(4) - (6)]. range ), (yyvsp[(5) - (6)]. vtext ));
}
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 1571 "parser.y"
    {
    OMCreator::instance()->addNets((yyvsp[(1) - (6)]. attribtable ), (yyvsp[(2) - (6)]. nettype ), (yyvsp[(3) - (6)]. BOOL ), NULL, (yyvsp[(5) - (6)]. vtext ));
}
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 1575 "parser.y"
    {
    OMCreator::instance()->addNets(NULL, TRIREG, false, (yyvsp[(3) - (5)]. range ), (yyvsp[(4) - (5)]. iddims ));
}
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 1579 "parser.y"
    {
    OMCreator::instance()->addPorts((yyvsp[(1) - (5)]. dir ), WIRE, (yyvsp[(2) - (5)]. BOOL ), (yyvsp[(3) - (5)]. range ), (yyvsp[(4) - (5)]. iddims ));
}
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 1584 "parser.y"
    {
    OMCreator::instance()->addPorts((yyvsp[(1) - (6)]. dir ), (yyvsp[(2) - (6)]. nettype ), (yyvsp[(3) - (6)]. BOOL ), (yyvsp[(4) - (6)]. range ), (yyvsp[(5) - (6)]. iddims ));
}
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 1588 "parser.y"
    {
    OMCreator::instance()->addPorts(OUT, REG, (yyvsp[(3) - (6)]. BOOL ), (yyvsp[(4) - (6)]. range ), (yyvsp[(5) - (6)]. iddims ));
}
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 1593 "parser.y"
    {
    //yyerror("Input port can not be of reg type.");
	MsgHandler::instance()->print(48, OMCreator::instance()->getLine(), "Input");
}
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 1598 "parser.y"
    {
    //yyerror("Inout port can not be of reg type.");
	MsgHandler::instance()->print(48, OMCreator::instance()->getLine(), "Inout");
}
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 1603 "parser.y"
    {
    yyerrok;
}
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 1607 "parser.y"
    {
    OMCreator::instance()->createSpecifyBlock();
	OMCreator::instance()->scopeEndAction();
}
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 1612 "parser.y"
    {
    OMCreator::instance()->createSpecifyBlock();
}
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 1616 "parser.y"
    {
	OMCreator::instance()->scopeEndAction();
}
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 1618 "parser.y"
    {
    yyerrok;
}
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 1625 "parser.y"
    {
    OMCreator::instance()->addDefparams((yyvsp[(2) - (3)]. defparams ));
}
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 1635 "parser.y"
    {
    OMCreator::instance()->OMCreator::setDelayAndDrStr((yyvsp[(3) - (4)]. insts ), NULL, NULL);
    OMCreator::instance()->setCurrAttribTable(NULL);
}
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 1640 "parser.y"
    {
    OMCreator::instance()->OMCreator::setDelayAndDrStr((yyvsp[(4) - (5)]. insts ), (yyvsp[(3) - (5)]. delay ), NULL);
    OMCreator::instance()->setCurrAttribTable(NULL);
}
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 1645 "parser.y"
    {
    OMCreator::instance()->OMCreator::setDelayAndDrStr((yyvsp[(4) - (5)]. insts ), NULL, (yyvsp[(3) - (5)]. dst ));
    OMCreator::instance()->setCurrAttribTable(NULL);
}
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 1650 "parser.y"
    {
    OMCreator::instance()->OMCreator::setDelayAndDrStr((yyvsp[(5) - (6)]. insts ), (yyvsp[(4) - (6)]. delay ), (yyvsp[(3) - (6)]. dst ));
    OMCreator::instance()->setCurrAttribTable(NULL);
}
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 1657 "parser.y"
    {
    OMCreator::instance()->OMCreator::setDelayAndDrStr((yyvsp[(2) - (3)]. insts ), NULL, NULL);
}
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 1661 "parser.y"
    {
    if((yyvsp[(1) - (6)]. gtype ) == PULLDOWN) {
	    yyerror("");
	}
	else {
        OMCreator::instance()->OMCreator::setDelayAndDrStr((yyvsp[(5) - (6)]. insts ), NULL, NULL);
	}	
}
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 1670 "parser.y"
    {
    if((yyvsp[(1) - (6)]. gtype ) == PULLUP) {
	    yyerror("");
	}
	else {
        OMCreator::instance()->OMCreator::setDelayAndDrStr((yyvsp[(5) - (6)]. insts ), NULL, NULL);
	}	
}
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 1680 "parser.y"
    {
    OMCreator::instance()->setCurrMasterName((yyvsp[(1) - (1)]. text ));
}
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 1684 "parser.y"
    {
    if ((yyvsp[(3) - (5)]. nconns )) {
        const int size = (yyvsp[(4) - (5)]. insts )->size();
        for (int i = 0; i < size; ++i) {
            OMCreator::instance()->setInstParams(static_cast <
                                 ModInst *
                                 >((*(yyvsp[(4) - (5)]. insts ))[i]), *(yyvsp[(3) - (5)]. nconns ));
    }}

}
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 1694 "parser.y"
    {
    OMCreator::instance()->createContAssigns((yyvsp[(2) - (5)]. dst ), (yyvsp[(3) - (5)]. delay ), (yyvsp[(4) - (5)]. assgps ));
}
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 1699 "parser.y"
    {
    OMCreator::instance()->createAlwaysBlock((yyvsp[(1) - (3)]. attribtable ), (yyvsp[(3) - (3)]. stmt ));
}
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 1703 "parser.y"
    {
    OMCreator::instance()->createInitialBlock((yyvsp[(1) - (3)]. attribtable ), (yyvsp[(3) - (3)]. stmt ));
}
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 1708 "parser.y"
    {
    OMCreator::instance()->createTask((yyvsp[(3) - (4)]. text ), (yyvsp[(2) - (4)]. BOOL ));
}
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 1712 "parser.y"
    {
    OMCreator::instance()->setTaskOrFuncStmt((yyvsp[(7) - (7)]. stmt ));
}
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 1714 "parser.y"
    {
    OMCreator::instance()->scopeEndAction();
}
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 1718 "parser.y"
    {
    OMCreator::instance()->createTask((yyvsp[(3) - (3)]. text ), (yyvsp[(2) - (3)]. BOOL ));
}
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 1722 "parser.y"
    {
    OMCreator::instance()->setTaskOrFuncStmt((yyvsp[(10) - (10)]. stmt ));
}
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 1726 "parser.y"
    {
    OMCreator::instance()->scopeEndAction();
}
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 1731 "parser.y"
    {
    OMCreator::instance()->createFunc((yyvsp[(5) - (6)]. text ), (yyvsp[(2) - (6)]. BOOL ), (yyvsp[(3) - (6)]. BOOL ), (yyvsp[(4) - (6)]. typerange ));
}
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 1735 "parser.y"
    {
    OMCreator::instance()->setTaskOrFuncStmt((yyvsp[(9) - (10)]. stmt ));
    OMCreator::instance()->scopeEndAction();
}
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 1738 "parser.y"
    {
    OMCreator::instance()->createFunc((yyvsp[(5) - (5)]. text ), (yyvsp[(2) - (5)]. BOOL ), (yyvsp[(3) - (5)]. BOOL ), (yyvsp[(4) - (5)]. typerange ));
}
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 1742 "parser.y"
    {
    OMCreator::instance()->setTaskOrFuncStmt((yyvsp[(12) - (13)]. stmt ));
    OMCreator::instance()->scopeEndAction();
}
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 1747 "parser.y"
    {
    OMCreator::instance()->createGenvar((yyvsp[(1) - (1)]. text ));
}
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 1752 "parser.y"
    {
    yyerrok;
}
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1756 "parser.y"
    {
    yyerrok;
}
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1760 "parser.y"
    {
    yyerrok;
}
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1764 "parser.y"
    {
    yyerrok;
}
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1768 "parser.y"
    {
}
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1771 "parser.y"
    {
}
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1777 "parser.y"
    {
    OMCreator::instance()->createGenStmt();
}
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1781 "parser.y"
    {
    OMCreator::instance()->setGenerateItems((yyvsp[(3) - (3)]. genitems ));
}
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1785 "parser.y"
    {
    OMCreator::instance()->scopeEndAction();
}
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1791 "parser.y"
    {
    (yyval. genitems ) = (yyvsp[(1) - (1)]. genitems );
}
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1795 "parser.y"
    {
    (yyval. genitems ) = NULL;
}
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1801 "parser.y"
    {
    (yyval. genitems ) = new std::vector < GenerateItem * >();
    (yyval. genitems )->push_back((yyvsp[(1) - (1)]. genitem ));
}
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1806 "parser.y"
    {
    (yyval. genitems ) = (yyvsp[(1) - (2)]. genitems );
    if (std::find((yyval. genitems )->begin(), (yyval. genitems )->end(), (yyvsp[(2) - (2)]. genitem )) == (yyval. genitems )->end()) {
        (yyval. genitems )->push_back((yyvsp[(2) - (2)]. genitem ));
    }
}
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1816 "parser.y"
    {
    OMCreator::instance()->addNets((yyvsp[(1) - (6)]. attribtable ), (yyvsp[(2) - (6)]. nettype ), (yyvsp[(3) - (6)]. BOOL ), (yyvsp[(4) - (6)]. range ), (yyvsp[(5) - (6)]. iddims ));
}
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1820 "parser.y"
    {
    OMCreator::instance()->addNets((yyvsp[(1) - (6)]. attribtable ), (yyvsp[(2) - (6)]. nettype ), (yyvsp[(3) - (6)]. BOOL ), (yyvsp[(4) - (6)]. range ), (yyvsp[(5) - (6)]. vtext ));
}
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1824 "parser.y"
    {
    OMCreator::instance()->addNets((yyvsp[(1) - (6)]. attribtable ), (yyvsp[(2) - (6)]. nettype ), (yyvsp[(3) - (6)]. BOOL ), NULL, (yyvsp[(5) - (6)]. vtext ));
}
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1828 "parser.y"
    {
    OMCreator::instance()->addNets(NULL, TRIREG, false, (yyvsp[(3) - (5)]. range ), (yyvsp[(4) - (5)]. iddims ));
}
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1832 "parser.y"
    {
}
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1837 "parser.y"
    {
    (yyval. genitem ) = (yyvsp[(1) - (1)]. genitem );
}
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1841 "parser.y"
    {
    (yyval. genitem ) = NULL;
}
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1848 "parser.y"
    {
    (yyval. genitem ) = (yyvsp[(1) - (1)]. genitem );
}
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1852 "parser.y"
    {
    (yyval. genitem ) = (yyvsp[(1) - (1)]. genitem );
}
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1856 "parser.y"
    {
    (yyval. genitem ) = (yyvsp[(1) - (1)]. genitem );
    OMCreator::instance()->removeGenItem();
}
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1861 "parser.y"
    {
    (yyval. genitem ) = (yyvsp[(1) - (1)]. genitem );
}
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1865 "parser.y"
    {
    (yyval. genitem ) = OMCreator::instance()->getCurrGenItem();
}
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1870 "parser.y"
    {
	OMCreator::instance()->createGenCond((yyvsp[(3) - (4)]. expr ));
}
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1875 "parser.y"
    {
    OMCreator::instance()->removeGenItem(true);
    GenerateCond *gc =
        dynamic_cast <
        GenerateCond * >(OMCreator::instance()->getCurrGenItem());
    (yyval. genitem ) = gc;
	if(gc) {
        gc->ifpart = (yyvsp[(2) - (2)]. genitem );
	}	
}
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 1886 "parser.y"
    {
    OMCreator::instance()->removeGenItem(true);
}
    break;

  case 319:

/* Line 1806 of yacc.c  */
#line 1890 "parser.y"
    {
    OMCreator::instance()->removeGenItem(true);
    GenerateCond *gc =
        dynamic_cast <
        GenerateCond * >(OMCreator::instance()->getCurrGenItem());
    (yyval. genitem ) = gc;
	if(gc)
	{
        gc->ifpart = (yyvsp[(2) - (5)]. genitem );
        gc->elsepart = (yyvsp[(5) - (5)]. genitem );
	}	
}
    break;

  case 320:

/* Line 1806 of yacc.c  */
#line 1903 "parser.y"
    { (yyval. genitem ) = NULL;}
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 1908 "parser.y"
    {
    OMCreator::instance()->createGenCase((yyvsp[(3) - (4)]. expr ));
}
    break;

  case 322:

/* Line 1806 of yacc.c  */
#line 1912 "parser.y"
    {
    const int size = (yyvsp[(6) - (7)]. exprsgenitemsv )->size();
    OMCreator::instance()->removeGenItem(true);
    GenerateCase *gc =
        dynamic_cast <
        GenerateCase * >(OMCreator::instance()->getCurrGenItem());
    for (int i = 0; i < size; ++i) {
        if ((*((yyvsp[(6) - (7)]. exprsgenitemsv )))[i].first.empty()) {
            gc->defstmt = (*((yyvsp[(6) - (7)]. exprsgenitemsv )))[i].second;
        } else {
            gc->caseitems.push_back((*((yyvsp[(6) - (7)]. exprsgenitemsv )))[i]);
        }
    }
    (yyval. genitem ) = gc;
}
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 1931 "parser.y"
    {
    (yyval. text ) = (yyvsp[(2) - (3)]. text );
}
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 1937 "parser.y"
    {
    (yyval. exprsgenitemsv ) = new std::vector < std::pair < std::vector < Expr * >,
        GenerateItem * > >();
    (yyval. exprsgenitemsv )->push_back(*((yyvsp[(1) - (1)]. exprsgenitems )));
    delete (yyvsp[(1) - (1)]. exprsgenitems );
}
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1944 "parser.y"
    {
    (yyval. exprsgenitemsv )->push_back(*((yyvsp[(2) - (2)]. exprsgenitems )));
    delete (yyvsp[(2) - (2)]. exprsgenitems );
}
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1952 "parser.y"
    {
    (yyval. exprsgenitems ) = new std::pair < std::vector < Expr * >, GenerateItem * >();
    (yyval. exprsgenitems )->first = *((yyvsp[(1) - (3)]. exprs ));
    (yyval. exprsgenitems )->second = (yyvsp[(3) - (3)]. genitem );
    if ((yyvsp[(3) - (3)]. genitem )) {
        OMCreator::instance()->removeGenItem(true);
    }
}
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1961 "parser.y"
    {
    (yyval. exprsgenitems ) = new std::pair < std::vector < Expr * >, GenerateItem * >();
    (yyval. exprsgenitems )->second = (yyvsp[(3) - (3)]. genitem );
    OMCreator::instance()->removeGenItem(true);
}
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1967 "parser.y"
    {
    (yyval. exprsgenitems ) = new std::pair < std::vector < Expr * >, GenerateItem * >();
    (yyval. exprsgenitems )->second = (yyvsp[(2) - (2)]. genitem );
    OMCreator::instance()->removeGenItem(true);
}
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1976 "parser.y"
    {
    OMCreator::instance()->createGenLoop((yyvsp[(3) - (8)]. assgp )->first, (yyvsp[(3) - (8)]. assgp )->second,
                         (yyvsp[(5) - (8)]. expr ), (yyvsp[(7) - (8)]. assgp )->second);
}
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1981 "parser.y"
    {
    OMCreator::instance()->removeGenItem(true);
    GenerateLoop *gl =
        dynamic_cast <
        GenerateLoop * >(OMCreator::instance()->getCurrGenItem());
	if(gl) {
        gl->gitem = (yyvsp[(10) - (10)]. genitem );
	}	
    (yyval. genitem ) = gl;
	OMCreator::instance()->scopeEndAction();
}
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1994 "parser.y"
    {
    (yyval. text ) = (yyvsp[(1) - (1)]. text );
}
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1999 "parser.y"
    {
    (yyval. text ) = (yyvsp[(1) - (1)]. text );
}
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 2004 "parser.y"
    {
    (yyval. assgp ) = new std::pair < Expr *, Expr * >();
    (yyval. assgp )->first = OMCreator::instance()->createIdUse((yyvsp[(1) - (3)]. text ));
    (yyval. assgp )->second = (yyvsp[(3) - (3)]. expr );
}
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 2013 "parser.y"
    {
    OMCreator::instance()->createGenBlock((yyvsp[(3) - (3)]. text ));
}
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 2017 "parser.y"
    {
    OMCreator::instance()->removeGenItem(true);
    GenerateBlock *gb =
        dynamic_cast <
        GenerateBlock * >(OMCreator::instance()->getCurrGenItem());
	if(gb){	
        gb->gitems = *((yyvsp[(5) - (6)]. genitems ));
	}	
     OMCreator::instance()->removeGenItem();
     (yyval. genitem ) = gb;
	 OMCreator::instance()->scopeEndAction();
}
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 2028 "parser.y"
    {
    OMCreator::instance()->createGenBlock("");
}
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 2032 "parser.y"
    {
    OMCreator::instance()->removeGenItem(true);
    GenerateBlock *gb =
        dynamic_cast <
        GenerateBlock * >(OMCreator::instance()->getCurrGenItem());
	if(gb){	
        gb->gitems = *((yyvsp[(3) - (4)]. genitems ));
	}	
     OMCreator::instance()->removeGenItem();
     (yyval. genitem ) = gb;
	 OMCreator::instance()->scopeEndAction();
}
    break;

  case 342:

/* Line 1806 of yacc.c  */
#line 2050 "parser.y"
    {
    (yyval. idassp ) = new std::pair < char *, Expr * >((yyvsp[(1) - (3)]. text ), (yyvsp[(3) - (3)]. expr ));
}
    break;

  case 343:

/* Line 1806 of yacc.c  */
#line 2056 "parser.y"
    {
    (yyvsp[(1) - (3)]. vtext )->push_back(*(yyvsp[(3) - (3)]. idassp ));
    (yyval. vtext ) = (yyvsp[(1) - (3)]. vtext );
    delete (yyvsp[(3) - (3)]. idassp );
}
    break;

  case 344:

/* Line 1806 of yacc.c  */
#line 2062 "parser.y"
    {
    (yyval. vtext ) = new std::vector < std::pair < char *, Expr * > >();
    (yyval. vtext )->push_back(*(yyvsp[(1) - (1)]. idassp ));
    delete (yyvsp[(1) - (1)]. idassp );
}
    break;

  case 345:

/* Line 1806 of yacc.c  */
#line 2070 "parser.y"
    {
    (yyval. nettype ) = WIRE;
}
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 2074 "parser.y"
    {
    (yyval. nettype ) = TRI;
}
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 2078 "parser.y"
    {
    (yyval. nettype ) = TRI1;
}
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 2082 "parser.y"
    {
    (yyval. nettype ) = SUPPLY0;
}
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 2086 "parser.y"
    {
    (yyval. nettype ) = WAND;
}
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 2090 "parser.y"
    {
    (yyval. nettype ) = TRIAND;
}
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 2094 "parser.y"
    {
    (yyval. nettype ) = TRI0;
}
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 2098 "parser.y"
    {
    (yyval. nettype ) = SUPPLY1;
}
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 2102 "parser.y"
    {
    (yyval. nettype ) = WOR;
}
    break;

  case 354:

/* Line 1806 of yacc.c  */
#line 2106 "parser.y"
    {
    (yyval. nettype ) = TRIOR;
}
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 2112 "parser.y"
    {
}
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 2117 "parser.y"
    {
    (yyval. parseInfo ) = new ParseInfo();
    (yyval. parseInfo )->name = string((yyvsp[(1) - (3)]. text ));
    (yyval. parseInfo )->exprs.push_back((yyvsp[(3) - (3)]. expr ));
}
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 2125 "parser.y"
    {
    (yyval. params ) = OMCreator::instance()->createParams < Param, VecParam > ((yyvsp[(1) - (1)]. parseInfos ));
}
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 2129 "parser.y"
    {
    (*(yyvsp[(2) - (2)]. parseInfos ))[0]->range = (yyvsp[(1) - (2)]. range );
    (*(yyvsp[(2) - (2)]. parseInfos ))[0]->isSigned = false;
    (yyval. params ) = (yyvsp[(1) - (2)]. range ) ? OMCreator::instance()->createVecParams((yyvsp[(2) - (2)]. parseInfos )) : 0;
}
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 2135 "parser.y"
    {
    (*(yyvsp[(3) - (3)]. parseInfos ))[0]->range = (yyvsp[(2) - (3)]. range );
    (*(yyvsp[(3) - (3)]. parseInfos ))[0]->isSigned = true;
    (yyval. params ) = (yyvsp[(2) - (3)]. range ) ? OMCreator::instance()->createVecParams((yyvsp[(3) - (3)]. parseInfos )) : 0;
}
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 2141 "parser.y"
    {
    (*(yyvsp[(2) - (2)]. parseInfos ))[0]->isSigned = false;
    (yyval. params ) = OMCreator::instance()->createParams < Param, VecParam > ((yyvsp[(2) - (2)]. parseInfos ), (yyvsp[(1) - (2)]. INT ));
}
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 2148 "parser.y"
    {
    (yyval. parseInfos ) = new std::vector < ParseInfo * >();
    (yyval. parseInfos )->push_back((yyvsp[(1) - (1)]. parseInfo ));
}
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 2153 "parser.y"
    {
    (yyvsp[(1) - (3)]. parseInfos )->push_back((yyvsp[(3) - (3)]. parseInfo ));
}
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 2160 "parser.y"
    {
    (yyval. parseInfo ) = new ParseInfo();
    (yyval. parseInfo )->name = string((yyvsp[(1) - (3)]. text ));
    (yyval. parseInfo )->exprs.push_back((yyvsp[(3) - (3)]. expr ));
}
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 2168 "parser.y"
    {
    (yyval. lparams ) = OMCreator::instance()->createParams < LocalParam, VecLocalParam > ((yyvsp[(1) - (1)]. parseInfos ));
}
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 2172 "parser.y"
    {
    (*(yyvsp[(2) - (2)]. parseInfos ))[0]->range = (yyvsp[(1) - (2)]. range );
    (yyval. lparams ) = OMCreator::instance()->createVecLocalParams((yyvsp[(2) - (2)]. parseInfos ));
}
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 2177 "parser.y"
    {
    (*(yyvsp[(3) - (3)]. parseInfos ))[0]->range = (yyvsp[(2) - (3)]. range );
    (*(yyvsp[(3) - (3)]. parseInfos ))[0]->isSigned = true;
    (yyval. lparams ) = OMCreator::instance()->createVecLocalParams((yyvsp[(3) - (3)]. parseInfos ));
}
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 2183 "parser.y"
    {
    (yyval. lparams ) = OMCreator::instance()->createParams < LocalParam, VecLocalParam > ((yyvsp[(2) - (2)]. parseInfos ), (yyvsp[(1) - (2)]. INT ));
}
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 2189 "parser.y"
    {
    (yyval. parseInfos ) = new std::vector < ParseInfo * >();
    (yyval. parseInfos )->push_back((yyvsp[(1) - (1)]. parseInfo ));
}
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 2194 "parser.y"
    {
    (yyvsp[(1) - (3)]. parseInfos )->push_back((yyvsp[(3) - (3)]. parseInfo ));
}
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 2201 "parser.y"
    {
    (yyval. nconns ) = new std::vector < NameConn * >();
    const int size = (yyvsp[(3) - (4)]. exprs ) ? (yyvsp[(3) - (4)]. exprs )->size() : 0;
    for (int i = 0; i < size; ++i) 
	{
	    Expr *expr = (*(yyvsp[(3) - (4)]. exprs ))[i] ? OMCreator::instance()->createIdUse((*(yyvsp[(3) - (4)]. exprs ))[i]->toString(), true) : 0;
		if(expr && !expr->isConstExpr()) 
		{
		    MsgHandler::instance()->print(47, OMCreator::instance()->getLine());
			continue;
		}	
        (yyval. nconns )->push_back(new NameConn(std::string(),
		(*(yyvsp[(3) - (4)]. exprs ))[i]));
    }
}
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 2217 "parser.y"
    {
    (yyval. nconns ) = (yyvsp[(3) - (4)]. nconns );
}
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 2221 "parser.y"
    {
    (yyval. nconns ) = new std::vector < NameConn * >();
    (yyval. nconns )->push_back(new NameConn(std::string(), new Num(atol(yytext))));
}
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 2226 "parser.y"
    {
    (yyval. nconns ) = NULL;
}
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 2230 "parser.y"
    {
    (yyval. nconns ) = NULL;
}
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 2236 "parser.y"
    {
    (yyval. nconn ) = new NameConn((yyvsp[(2) - (5)]. text ), (yyvsp[(4) - (5)]. expr ));
}
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 2240 "parser.y"
    {
    (yyval. nconn ) = new NameConn((yyvsp[(2) - (4)]. text ), NULL);
}
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 2246 "parser.y"
    {
    (yyval. nconns ) = new std::vector < NameConn * >();
    (yyval. nconns )->push_back((yyvsp[(1) - (1)]. nconn ));
}
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 2251 "parser.y"
    {
    (yyval. nconns ) = (yyvsp[(1) - (3)]. nconns );
    (yyval. nconns )->push_back((yyvsp[(3) - (3)]. nconn ));

}
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 2260 "parser.y"
    {
    (yyval. parseInfo ) = (yyvsp[(1) - (1)]. parseInfo );
}
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 2265 "parser.y"
    {
    MsgHandler::instance()->print(52, OMCreator::instance()->getLine(), "complex port declaration");
	return 1;
    //$$ = $4;
}
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 2272 "parser.y"
    {
    MsgHandler::instance()->print(52, OMCreator::instance()->getLine(), "complex port declaration");
	return 1;
    //$$ = NULL;
}
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 2279 "parser.y"
    {
    MsgHandler::instance()->print(52, OMCreator::instance()->getLine(), "complex port declaration");
	return 1;
    //$$ = NULL;
}
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 2287 "parser.y"
    {
    (yyval. parseInfo ) = (yyvsp[(1) - (1)]. parseInfo );
}
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 2291 "parser.y"
    {
    (yyval. parseInfo ) = NULL;
}
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 2297 "parser.y"
    {
    (yyval. parseInfo ) = new ParseInfo();
    (yyval. parseInfo )->name = std::string((yyvsp[(1) - (1)]. text ));
    (yyval. parseInfo )->range = NULL;

}
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 2304 "parser.y"
    {
    Range *range = new Range((yyvsp[(3) - (6)]. expr ), (yyvsp[(5) - (6)]. expr ));
    (yyval. parseInfo ) = new ParseInfo();
    (yyval. parseInfo )->name = std::string((yyvsp[(1) - (6)]. text ));
    (yyval. parseInfo )->range = range;
}
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 2311 "parser.y"
    {
    (yyval. parseInfo ) = NULL;
}
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 2315 "parser.y"
    {
    (yyval. parseInfo ) = NULL;
}
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 2321 "parser.y"
    {
    (yyval. parseInfos ) = new std::vector < ParseInfo * >();
    (yyval. parseInfos )->push_back((yyvsp[(1) - (1)]. parseInfo ));
}
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 2326 "parser.y"
    {
    (yyvsp[(1) - (3)]. parseInfos )->push_back((yyvsp[(3) - (3)]. parseInfo ));
    (yyval. parseInfos ) = (yyvsp[(1) - (3)]. parseInfos );
}
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 2334 "parser.y"
    {
    (yyval. nconn ) = new NameConn((yyvsp[(2) - (5)]. text ), (yyvsp[(4) - (5)]. expr ));
}
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 2338 "parser.y"
    {
(yyval. nconn ) = NULL;}
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 2341 "parser.y"
    {
    (yyval. nconn ) = new NameConn((yyvsp[(2) - (4)]. text ), NULL);
}
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 2347 "parser.y"
    {
    (yyval. nconns ) = (yyvsp[(1) - (3)]. nconns );
    (yyval. nconns )->push_back((yyvsp[(3) - (3)]. nconn ));
}
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 2352 "parser.y"
    {
    (yyval. nconns ) = new std::vector < NameConn * >();
    (yyval. nconns )->push_back((yyvsp[(1) - (1)]. nconn ));
}
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 2359 "parser.y"
    {
    (yyval. dir ) = IN;
}
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 2363 "parser.y"
    {
    (yyval. dir ) = OUT;
}
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 2367 "parser.y"
    {
(yyval. dir ) = INOUT;}
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 2372 "parser.y"
    {
    //$$ = new Range($2, $4);
	(yyval. range ) = ((yyvsp[(2) - (5)]. expr ) && (yyvsp[(4) - (5)]. expr )) ? OMCreator::instance()->createRange((yyvsp[(2) - (5)]. expr ), (yyvsp[(4) - (5)]. expr )) : 0;
}
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 2379 "parser.y"
    {
    (yyval. range ) = (yyvsp[(1) - (1)]. range );
}
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 2383 "parser.y"
    {
    (yyval. range ) = NULL;
}
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 2390 "parser.y"
    {
    (yyval. typerange ) = new std::pair < int, Range * >(0, (yyvsp[(1) - (1)]. range ));
}
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 2394 "parser.y"
    {
    (yyval. typerange ) = new std::pair < int, Range * >(1, NULL);
}
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 2398 "parser.y"
    {
    (yyval. typerange ) = new std::pair < int, Range * >(2, NULL);
}
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 2402 "parser.y"
    {
    (yyval. typerange ) = new std::pair < int, Range * >(3, NULL);
}
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 2406 "parser.y"
    {
    (yyval. typerange ) = new std::pair < int, Range * >(4, NULL);
}
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 2410 "parser.y"
    {
    (yyval. typerange ) = NULL;
}
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 2416 "parser.y"
    {
    (yyval. INT ) = 1;
}
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 2420 "parser.y"
    {
    (yyval. INT ) = 2;
}
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 2424 "parser.y"
    {
    (yyval. INT ) = 3;
}
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 2428 "parser.y"
    {
    (yyval. INT ) = 4;
}
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 2433 "parser.y"
    {
    (yyval. reginfo ) = new RegInfo((yyvsp[(1) - (1)]. text ));
}
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 2437 "parser.y"
    {
    (yyval. reginfo ) = new RegInfo((yyvsp[(1) - (3)]. text ), (yyvsp[(3) - (3)]. expr ));
}
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 2441 "parser.y"
    {
    (yyval. reginfo ) = new RegInfo((yyvsp[(1) - (2)]. text ), NULL, (yyvsp[(2) - (2)]. ranges ));
}
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 2447 "parser.y"
    {
    (yyval. reginfos ) = new std::vector < RegInfo * >();
    (yyval. reginfos )->push_back((yyvsp[(1) - (1)]. reginfo ));
}
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 2452 "parser.y"
    {
    (yyval. reginfos ) = (yyvsp[(1) - (3)]. reginfos );
    (yyval. reginfos )->push_back((yyvsp[(3) - (3)]. reginfo ));
}
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 2459 "parser.y"
    {
    OMCreator::instance()->addSpecparamDecl((yyvsp[(2) - (3)]. sparams ));
}
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 2463 "parser.y"
    {
    OMCreator::instance()->addSpecPath((yyvsp[(1) - (2)]. specpath ));
}
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 2467 "parser.y"
    {
    OMCreator::instance()->addSpecPath((yyvsp[(1) - (2)]. specpath ));
}
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 2471 "parser.y"
    {
    OMCreator::instance()->addSpecPath((yyvsp[(5) - (6)]. specpath ));
	if((yyvsp[(5) - (6)]. specpath )){
        (yyvsp[(5) - (6)]. specpath )->setIfExpr((yyvsp[(3) - (6)]. expr ));
	}	
}
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 2478 "parser.y"
    {
    OMCreator::instance()->addSpecPath((yyvsp[(5) - (6)]. specpath ));
	if((yyvsp[(5) - (6)]. specpath )) {
        (yyvsp[(5) - (6)]. specpath )->setIfExpr((yyvsp[(3) - (6)]. expr ));
	}	
}
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 2486 "parser.y"
    {
    //OMCreator::instance()->createSpecTask("$hold", $3, $5, $7, NULL, 
	//    $8 ? *($8) : std::vector<Expr*>());
}
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 2491 "parser.y"
    {
}
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 2495 "parser.y"
    {
}
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 2499 "parser.y"
    {
}
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 2503 "parser.y"
    {
}
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 2507 "parser.y"
    {
}
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 2511 "parser.y"
    {
}
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 2514 "parser.y"
    {
}
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 2516 "parser.y"
    {
}
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 2518 "parser.y"
    {
}
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 2520 "parser.y"
    {
}
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 2522 "parser.y"
    {
}
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 2524 "parser.y"
    {
}
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 2529 "parser.y"
    {
    (yyval. dvals ) = new std::vector < DelayVal * >();
    (yyval. dvals )->push_back((yyvsp[(1) - (1)]. dval ));
}
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 2534 "parser.y"
    {
    (yyval. dvals ) = (yyvsp[(1) - (3)]. dvals );
    (yyval. dvals )->push_back((yyvsp[(3) - (3)]. dval ));
}
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 2543 "parser.y"
    {
    (yyval. specpath ) = (yyvsp[(1) - (5)]. specpath );
    if ((yyvsp[(4) - (5)]. dvals )) {
        (yyval. specpath )->setDelays(*((yyvsp[(4) - (5)]. dvals )));
        delete (yyvsp[(4) - (5)]. dvals );
    }
}
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 2551 "parser.y"
    {
    (yyval. specpath ) = (yyvsp[(1) - (3)]. specpath );
    std::vector < DelayVal * >d;
    d.push_back((yyvsp[(3) - (3)]. dval ));
    (yyval. specpath )->setDelays(d);
}
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 2561 "parser.y"
    {
    (yyval. specpath ) = OMCreator::instance()->createSpecifyEdgePath(POSEDGE, (yyvsp[(3) - (7)]. exprs ), (yyvsp[(4) - (7)]. CHAR ),
                              PCONN, (yyvsp[(6) - (7)]. text ));
}
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 2567 "parser.y"
    {
    (yyval. specpath ) = OMCreator::instance()->createSpecifyEdgePath(POSEDGE, (yyvsp[(3) - (11)]. exprs ), (yyvsp[(4) - (11)]. CHAR ),
                              PCONN,
                              std::string(""), (yyvsp[(7) - (11)]. expr ),
                              (yyvsp[(8) - (11)]. pop ), (yyvsp[(9) - (11)]. expr ));
}
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 2574 "parser.y"
    {
    (yyval. specpath ) = OMCreator::instance()->createSpecifyEdgePath(POSEDGE, (yyvsp[(3) - (7)]. exprs ), (yyvsp[(4) - (7)]. CHAR ),
                              FCONN, (yyvsp[(6) - (7)]. text ));
}
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 2580 "parser.y"
    {
    (yyval. specpath ) = OMCreator::instance()->createSpecifyEdgePath(POSEDGE, (yyvsp[(3) - (11)]. exprs ), (yyvsp[(4) - (11)]. CHAR ),
                              FCONN,
                              std::string(""), (yyvsp[(7) - (11)]. expr ),
                              (yyvsp[(8) - (11)]. pop ), (yyvsp[(9) - (11)]. expr ));
}
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 2587 "parser.y"
    {
    (yyval. specpath ) = OMCreator::instance()->createSpecifyEdgePath(NEGEDGE, (yyvsp[(3) - (7)]. exprs ), (yyvsp[(4) - (7)]. CHAR ),
                              PCONN, (yyvsp[(6) - (7)]. text ));
}
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 2593 "parser.y"
    {
    (yyval. specpath ) = OMCreator::instance()->createSpecifyEdgePath(NEGEDGE, (yyvsp[(3) - (11)]. exprs ), (yyvsp[(4) - (11)]. CHAR ),
                              PCONN,
                              std::string(""), (yyvsp[(7) - (11)]. expr ),
                              (yyvsp[(8) - (11)]. pop ), (yyvsp[(9) - (11)]. expr ));
}
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 2600 "parser.y"
    {
    (yyval. specpath ) = OMCreator::instance()->createSpecifyEdgePath(NEGEDGE, (yyvsp[(3) - (7)]. exprs ), (yyvsp[(4) - (7)]. CHAR ),
                              FCONN, (yyvsp[(6) - (7)]. text ));
}
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 2606 "parser.y"
    {
    (yyval. specpath ) = OMCreator::instance()->createSpecifyEdgePath(NEGEDGE, (yyvsp[(3) - (11)]. exprs ), (yyvsp[(4) - (11)]. CHAR ),
                              FCONN,
                              std::string(""), (yyvsp[(7) - (11)]. expr ),
                              (yyvsp[(8) - (11)]. pop ), (yyvsp[(9) - (11)]. expr ));
}
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 2615 "parser.y"
    {
    (yyval. pop ) = POPOS;
}
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 2619 "parser.y"
    {
    (yyval. pop ) = PONEG;
}
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 2623 "parser.y"
    {
    (yyval. pop ) = POCOL;
}
    break;

  case 452:

/* Line 1806 of yacc.c  */
#line 2630 "parser.y"
    {
    (yyval. specpath ) = (yyvsp[(1) - (5)]. specpath );
    if ((yyval. specpath ) && (yyvsp[(4) - (5)]. dvals )) {
        (yyval. specpath )->setDelays(*((yyvsp[(4) - (5)]. dvals )));
    }
}
    break;

  case 453:

/* Line 1806 of yacc.c  */
#line 2637 "parser.y"
    {
    (yyval. specpath ) = (yyvsp[(1) - (3)]. specpath );
    if ((yyval. specpath ) && (yyvsp[(3) - (3)]. dval )) {
        std::vector < DelayVal * >dv;
        dv.push_back((yyvsp[(3) - (3)]. dval ));
        (yyval. specpath )->setDelays(dv);
    }
}
    break;

  case 454:

/* Line 1806 of yacc.c  */
#line 2646 "parser.y"
    {
    (yyval. specpath ) = NULL;
    yyerrok;
}
    break;

  case 455:

/* Line 1806 of yacc.c  */
#line 2654 "parser.y"
    {
    if ((yyvsp[(2) - (6)]. exprs ) && (yyvsp[(5) - (6)]. exprs )) {
        (yyval. specpath ) = new SpecifyPath((yyvsp[(3) - (6)]. CHAR ), PCONN, *((yyvsp[(2) - (6)]. exprs )), *((yyvsp[(5) - (6)]. exprs )));
        delete (yyvsp[(2) - (6)]. exprs );
        delete (yyvsp[(5) - (6)]. exprs );
    } else {
        (yyval. specpath ) = NULL;
    }
}
    break;

  case 456:

/* Line 1806 of yacc.c  */
#line 2664 "parser.y"
    {
    if ((yyvsp[(2) - (6)]. exprs ) && (yyvsp[(5) - (6)]. exprs )) {
        (yyval. specpath ) = new SpecifyPath((yyvsp[(3) - (6)]. CHAR ), FCONN, *((yyvsp[(2) - (6)]. exprs )), *((yyvsp[(5) - (6)]. exprs )));
        delete (yyvsp[(2) - (6)]. exprs );
        delete (yyvsp[(5) - (6)]. exprs );
    } else {
        (yyval. specpath ) = NULL;
    }

}
    break;

  case 457:

/* Line 1806 of yacc.c  */
#line 2675 "parser.y"
    {
    (yyval. specpath ) = NULL;
    yyerrok;
}
    break;

  case 458:

/* Line 1806 of yacc.c  */
#line 2682 "parser.y"
    {
    (yyval. exprs ) = new std::vector < Expr * >();
    Expr *expr = OMCreator::instance()->createIdUse((yyvsp[(1) - (1)]. text ));
    if (expr) {
        (yyval. exprs )->push_back(expr);
    }
}
    break;

  case 459:

/* Line 1806 of yacc.c  */
#line 2690 "parser.y"
    {
    (yyval. exprs ) = new std::vector < Expr * >();
    (yyval. exprs )->push_back(OMCreator::instance()->createBitSelect((yyvsp[(1) - (2)]. text ), (yyvsp[(2) - (2)]. exprs )));
}
    break;

  case 460:

/* Line 1806 of yacc.c  */
#line 2695 "parser.y"
    {
    (yyval. exprs ) = (yyvsp[(1) - (3)]. exprs );
    Expr *expr = OMCreator::instance()->createIdUse((yyvsp[(3) - (3)]. text ));
    if (expr) {
        (yyval. exprs )->push_back(expr);
    }
}
    break;

  case 461:

/* Line 1806 of yacc.c  */
#line 2703 "parser.y"
    {
    Expr *bitsel = OMCreator::instance()->createBitSelect((yyvsp[(3) - (4)]. text ), (yyvsp[(4) - (4)]. exprs ));
    (yyval. exprs ) = (yyvsp[(1) - (4)]. exprs );
    (yyval. exprs )->push_back(bitsel);
}
    break;

  case 462:

/* Line 1806 of yacc.c  */
#line 2711 "parser.y"
    {
    std::vector < Expr * >exprv;
    exprv.push_back((yyvsp[(3) - (3)]. expr ));
    std::string name((yyvsp[(1) - (3)]. text ));
    (yyval. sparam ) = new Specparam(name, exprv);
}
    break;

  case 463:

/* Line 1806 of yacc.c  */
#line 2718 "parser.y"
    {
    std::vector < Expr * >exprv;
    exprv.push_back((yyvsp[(3) - (7)]. expr ));
    exprv.push_back((yyvsp[(5) - (7)]. expr ));
    exprv.push_back((yyvsp[(7) - (7)]. expr ));
    std::string name((yyvsp[(1) - (7)]. text ));
    (yyval. sparam ) = new Specparam(name, exprv);
}
    break;

  case 464:

/* Line 1806 of yacc.c  */
#line 2727 "parser.y"
    {
    (yyval. sparam ) = NULL;
}
    break;

  case 465:

/* Line 1806 of yacc.c  */
#line 2731 "parser.y"
    {
    (yyval. sparam ) = NULL;
}
    break;

  case 466:

/* Line 1806 of yacc.c  */
#line 2737 "parser.y"
    {
    (yyval. sparams ) = new std::vector < Specparam * >();
    (yyval. sparams )->push_back((yyvsp[(1) - (1)]. sparam ));
}
    break;

  case 467:

/* Line 1806 of yacc.c  */
#line 2742 "parser.y"
    {
    (yyval. sparams ) = (yyvsp[(1) - (3)]. sparams );
    (yyval. sparams )->push_back((yyvsp[(3) - (3)]. sparam ));
}
    break;

  case 468:

/* Line 1806 of yacc.c  */
#line 2749 "parser.y"
    {
    (yyval. CHAR ) = '+';
}
    break;

  case 469:

/* Line 1806 of yacc.c  */
#line 2753 "parser.y"
    {
    (yyval. CHAR ) = '-';
}
    break;

  case 470:

/* Line 1806 of yacc.c  */
#line 2757 "parser.y"
    {
    (yyval. CHAR ) = ' ';
}
    break;

  case 471:

/* Line 1806 of yacc.c  */
#line 2763 "parser.y"
    {
    (yyval. specre ) = new SpecRefEvent(POSEDGE, NULL, (yyvsp[(2) - (2)]. expr ));
}
    break;

  case 472:

/* Line 1806 of yacc.c  */
#line 2767 "parser.y"
    {
    (yyval. specre ) = new SpecRefEvent(NEGEDGE, NULL, (yyvsp[(2) - (2)]. expr ));
}
    break;

  case 473:

/* Line 1806 of yacc.c  */
#line 2771 "parser.y"
    {
    (yyval. specre ) = new SpecRefEvent(POSEDGE, (yyvsp[(2) - (4)]. expr ), (yyvsp[(4) - (4)]. expr ));
}
    break;

  case 474:

/* Line 1806 of yacc.c  */
#line 2775 "parser.y"
    {
    (yyval. specre ) = new SpecRefEvent(NEGEDGE, (yyvsp[(2) - (4)]. expr ), (yyvsp[(4) - (4)]. expr ));
}
    break;

  case 475:

/* Line 1806 of yacc.c  */
#line 2779 "parser.y"
    {
    (yyval. specre ) = new SpecRefEvent(NOEDGE, (yyvsp[(1) - (3)]. expr ), (yyvsp[(3) - (3)]. expr ));
}
    break;

  case 476:

/* Line 1806 of yacc.c  */
#line 2783 "parser.y"
    {
    (yyval. specre ) = new SpecRefEvent(NOEDGE, (yyvsp[(1) - (1)]. expr ), NULL);
}
    break;

  case 477:

/* Line 1806 of yacc.c  */
#line 2790 "parser.y"
    {
    (yyval. exprs ) = NULL;
}
    break;

  case 478:

/* Line 1806 of yacc.c  */
#line 2794 "parser.y"
    {
    (yyval. exprs ) = (yyvsp[(1) - (1)]. exprs );
}
    break;

  case 479:

/* Line 1806 of yacc.c  */
#line 2799 "parser.y"
    {
    //if (dynamic_cast < std::vector < Expr * >*>($$ - 1)) {
    //    $$ = $$ - 1;
    //} else {
    //    $$ = new std::vector < Expr * >();
    //}
    //$$->push_back(NULL);
	(yyval. exprs ) = NULL;
}
    break;

  case 480:

/* Line 1806 of yacc.c  */
#line 2809 "parser.y"
    {
    //if (dynamic_cast < std::vector < Expr * >*>($$ - 1)) {
    //    $$ = $$ - 1;
    ///} else {
    //    $$ = new std::vector < Expr * >();
    //}
    //$$->push_back(NULL);
    //Expr *expr = OMCreator::instance()->createIdUse($2);
    //$$->push_back(expr);
	(yyval. exprs ) = NULL;
}
    break;

  case 481:

/* Line 1806 of yacc.c  */
#line 2821 "parser.y"
    {
    //$$ = $1;
    //$$->push_back(NULL);
	(yyval. exprs ) = NULL;
}
    break;

  case 482:

/* Line 1806 of yacc.c  */
#line 2827 "parser.y"
    {
    //$$ = $1;
    //Expr *expr = OMCreator::instance()->createIdUse($3);
    //$$->push_back(expr);
	(yyval. exprs ) = NULL;
}
    break;

  case 483:

/* Line 1806 of yacc.c  */
#line 2834 "parser.y"
    {
    (yyval. exprs ) = new std::vector < Expr * >();
    Expr *expr = OMCreator::instance()->createIdUse((yyvsp[(1) - (1)]. text ));
    (yyval. exprs )->push_back(expr);
	(yyval. exprs ) = NULL;
}
    break;

  case 484:

/* Line 1806 of yacc.c  */
#line 2844 "parser.y"
    {
    (yyval. stmt ) = new ProcContAssign((yyvsp[(2) - (5)]. expr ), (yyvsp[(4) - (5)]. expr ));
}
    break;

  case 485:

/* Line 1806 of yacc.c  */
#line 2848 "parser.y"
    {
    (yyval. stmt ) = new DAssign((yyvsp[(2) - (3)]. expr ));
}
    break;

  case 486:

/* Line 1806 of yacc.c  */
#line 2853 "parser.y"
    {
    (yyval. stmt ) = new Force((yyvsp[(2) - (5)]. expr ), (yyvsp[(4) - (5)]. expr ));
}
    break;

  case 487:

/* Line 1806 of yacc.c  */
#line 2857 "parser.y"
    {
    (yyval. stmt ) = new Release((yyvsp[(2) - (3)]. expr ));
}
    break;

  case 488:

/* Line 1806 of yacc.c  */
#line 2861 "parser.y"
    {
    Block *blk = new Block();
    const int size = (yyvsp[(2) - (3)]. stmts )->size();
    for (int i = 0; i < size; ++i) {
        blk->addStmt((*(yyvsp[(2) - (3)]. stmts ))[i]);
    }
    (yyval. stmt ) = blk;
}
    break;

  case 489:

/* Line 1806 of yacc.c  */
#line 2870 "parser.y"
    {
    NamedBlockBase *nb = new NamedBlock((yyvsp[(3) - (3)]. text ));
    OMCreator::instance()->addNamedBlock(nb);
}
    break;

  case 490:

/* Line 1806 of yacc.c  */
#line 2875 "parser.y"
    {
    NamedBlock *nb =
        dynamic_cast < NamedBlock * >(const_cast <
                      Scope *
                      >(OMCreator::
                        instance()->getCurrScope()));
    const int size = (yyvsp[(6) - (6)]. stmts )->size();
    for (int i = 0; i < size; ++i) {
        static_cast < NamedBlock * >(nb)->addStmt((*(yyvsp[(6) - (6)]. stmts ))[i]);
    }
}
    break;

  case 491:

/* Line 1806 of yacc.c  */
#line 2887 "parser.y"
    {
    (yyval. stmt ) = dynamic_cast < NamedBlock * >(const_cast <
                       Scope *
                       >(OMCreator::
                         instance()->getCurrScope()));
    OMCreator::instance()->scopeEndAction();
}
    break;

  case 492:

/* Line 1806 of yacc.c  */
#line 2895 "parser.y"
    {
    (yyval. stmt ) = new Block();
}
    break;

  case 493:

/* Line 1806 of yacc.c  */
#line 2899 "parser.y"
    {
    (yyval. stmt ) = new NamedBlock((yyvsp[(3) - (4)]. text ));
}
    break;

  case 494:

/* Line 1806 of yacc.c  */
#line 2903 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 495:

/* Line 1806 of yacc.c  */
#line 2908 "parser.y"
    {
    Fork *fork = new Fork((yyvsp[(3) - (3)]. text ));
    OMCreator::instance()->addNamedBlock(fork);
}
    break;

  case 496:

/* Line 1806 of yacc.c  */
#line 2913 "parser.y"
    {
    const int size = (yyvsp[(6) - (7)]. stmts )->size();
	Fork *fork = const_cast<Fork*>(dynamic_cast<const Fork*>(
	OMCreator::instance()->getCurrScope()));
    for (int i = 0; i < size; ++i) {
        (fork)->addStmt((*(yyvsp[(6) - (7)]. stmts ))[i]);
    }
}
    break;

  case 497:

/* Line 1806 of yacc.c  */
#line 2920 "parser.y"
    {
	(yyval. stmt ) = const_cast<Fork*>(
		dynamic_cast<const Fork*>(OMCreator::instance()->getCurrScope()));
    OMCreator::instance()->scopeEndAction();
}
    break;

  case 498:

/* Line 1806 of yacc.c  */
#line 2926 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 499:

/* Line 1806 of yacc.c  */
#line 2930 "parser.y"
    {
    (yyval. stmt ) = new Fork((yyvsp[(3) - (4)]. text ));
    OMCreator::instance()->addNamedBlock(static_cast <
                         NamedBlockBase * >((yyval. stmt )));
    OMCreator::instance()->scopeEndAction();
}
    break;

  case 500:

/* Line 1806 of yacc.c  */
#line 2937 "parser.y"
    {
    //$$ = new Disable(OMCreator::instance()->createIdUse($2));
	(yyval. stmt ) = new Disable((yyvsp[(2) - (3)]. text ));
	(yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 501:

/* Line 1806 of yacc.c  */
#line 2943 "parser.y"
    {
    (yyval. stmt ) = new Trigger(OMCreator::instance()->createIdUse((yyvsp[(2) - (3)]. text )));
	(yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 502:

/* Line 1806 of yacc.c  */
#line 2948 "parser.y"
    {
    (yyval. stmt ) = new Forever((yyvsp[(2) - (2)]. stmt ));
	(yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 503:

/* Line 1806 of yacc.c  */
#line 2953 "parser.y"
    {
    Fork *fork = new Fork("");
    OMCreator::instance()->addNamedBlock(fork);
	}
    break;

  case 504:

/* Line 1806 of yacc.c  */
#line 2957 "parser.y"
    {
    const int size = (yyvsp[(3) - (4)]. stmts )->size();
	Fork *fork = const_cast<Fork*>(dynamic_cast<const Fork*>(
	OMCreator::instance()->getCurrScope()));
    for (int i = 0; i < size; ++i) {
        (fork)->addStmt((*(yyvsp[(3) - (4)]. stmts ))[i]);
    }
    OMCreator::instance()->scopeEndAction();
    (yyval. stmt ) = fork;
}
    break;

  case 505:

/* Line 1806 of yacc.c  */
#line 2968 "parser.y"
    {
    (yyval. stmt ) = new Repeat((yyvsp[(3) - (5)]. expr ), (yyvsp[(5) - (5)]. stmt ));
	(yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 506:

/* Line 1806 of yacc.c  */
#line 2973 "parser.y"
    {
    (yyval. stmt ) = new Case(*(yyvsp[(5) - (6)]. caseitems ), (yyvsp[(3) - (6)]. expr ), CASE);
	(yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 507:

/* Line 1806 of yacc.c  */
#line 2978 "parser.y"
    {
    (yyval. stmt ) = new Case(*(yyvsp[(5) - (6)]. caseitems ), (yyvsp[(3) - (6)]. expr ), CASEX);
	(yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 508:

/* Line 1806 of yacc.c  */
#line 2983 "parser.y"
    {
    (yyval. stmt ) = new Case(*(yyvsp[(5) - (6)]. caseitems ), (yyvsp[(3) - (6)]. expr ), CASEZ);
	(yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 509:

/* Line 1806 of yacc.c  */
#line 2988 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 510:

/* Line 1806 of yacc.c  */
#line 2992 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 511:

/* Line 1806 of yacc.c  */
#line 2996 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 512:

/* Line 1806 of yacc.c  */
#line 3000 "parser.y"
    {
    (yyval. stmt ) = new If((yyvsp[(3) - (5)]. expr ), (yyvsp[(5) - (5)]. stmt ), NULL);
	(yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 513:

/* Line 1806 of yacc.c  */
#line 3005 "parser.y"
    {
    (yyval. stmt ) = new If((yyvsp[(3) - (7)]. expr ), (yyvsp[(5) - (7)]. stmt ), (yyvsp[(7) - (7)]. stmt ));
	(yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 514:

/* Line 1806 of yacc.c  */
#line 3010 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 515:

/* Line 1806 of yacc.c  */
#line 3014 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 516:

/* Line 1806 of yacc.c  */
#line 3019 "parser.y"
    {
    (yyval. stmt ) = new For((yyvsp[(3) - (13)]. expr ), (yyvsp[(5) - (13)]. expr ), (yyvsp[(7) - (13)]. expr ), (yyvsp[(11) - (13)]. expr ), (yyvsp[(13) - (13)]. stmt ));
	(yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 517:

/* Line 1806 of yacc.c  */
#line 3024 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 518:

/* Line 1806 of yacc.c  */
#line 3029 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 519:

/* Line 1806 of yacc.c  */
#line 3033 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 520:

/* Line 1806 of yacc.c  */
#line 3037 "parser.y"
    {
    (yyval. stmt ) = new While((yyvsp[(3) - (5)]. expr ), (yyvsp[(5) - (5)]. stmt ));
	(yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 521:

/* Line 1806 of yacc.c  */
#line 3042 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 522:

/* Line 1806 of yacc.c  */
#line 3046 "parser.y"
    {
    (yyval. stmt ) = new DelayStmt((yyvsp[(1) - (2)]. delay ), (yyvsp[(2) - (2)]. stmt ));
	(yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 523:

/* Line 1806 of yacc.c  */
#line 3051 "parser.y"
    {
    if ((yyvsp[(1) - (2)]. evctrl )) {
        (yyvsp[(1) - (2)]. evctrl )->setStmt((yyvsp[(2) - (2)]. stmt ));
        (yyval. stmt ) = (yyvsp[(1) - (2)]. evctrl );
	    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
    } else {
        (yyval. stmt ) = NULL;
    }
}
    break;

  case 524:

/* Line 1806 of yacc.c  */
#line 3061 "parser.y"
    {
    (yyval. stmt ) = new EventCntrl(std::vector < std::pair < EdgeType, Expr * > >(),
                (yyvsp[(3) - (3)]. stmt ), true);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 525:

/* Line 1806 of yacc.c  */
#line 3067 "parser.y"
    {
    (yyval. stmt ) = new EventCntrl(std::vector < std::pair < EdgeType, Expr * > >(),
                (yyvsp[(5) - (5)]. stmt ), true);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 526:

/* Line 1806 of yacc.c  */
#line 3073 "parser.y"
    {
    (yyval. stmt ) = new ProcAssign(NULL, NULL, (yyvsp[(1) - (4)]. expr ), (yyvsp[(3) - (4)]. expr ), BLOCK);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 527:

/* Line 1806 of yacc.c  */
#line 3078 "parser.y"
    {
    (yyval. stmt ) = new ProcAssign(NULL, NULL, (yyvsp[(1) - (4)]. expr ), (yyvsp[(3) - (4)]. expr ), NONBLOCK);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 528:

/* Line 1806 of yacc.c  */
#line 3083 "parser.y"
    {
    (yyval. stmt ) = new ProcAssign(NULL, (yyvsp[(3) - (5)]. delay ), (yyvsp[(1) - (5)]. expr ), (yyvsp[(4) - (5)]. expr ), BLOCK);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 529:

/* Line 1806 of yacc.c  */
#line 3088 "parser.y"
    {
    (yyval. stmt ) = new ProcAssign(NULL, (yyvsp[(3) - (5)]. delay ), (yyvsp[(1) - (5)]. expr ), (yyvsp[(4) - (5)]. expr ), NONBLOCK);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 530:

/* Line 1806 of yacc.c  */
#line 3093 "parser.y"
    {
    (yyval. stmt ) = new CompAssign((yyvsp[(1) - (5)]. expr ), NULL, (yyvsp[(3) - (5)]. evctrl ), (yyvsp[(4) - (5)]. expr ), BLOCK);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 531:

/* Line 1806 of yacc.c  */
#line 3098 "parser.y"
    {
    (yyval. stmt ) = new CompAssign((yyvsp[(1) - (9)]. expr ), (yyvsp[(5) - (9)]. expr ), (yyvsp[(7) - (9)]. evctrl ), (yyvsp[(8) - (9)]. expr ), BLOCK);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 532:

/* Line 1806 of yacc.c  */
#line 3103 "parser.y"
    {
    (yyval. stmt ) = new CompAssign((yyvsp[(1) - (5)]. expr ), NULL, (yyvsp[(3) - (5)]. evctrl ), (yyvsp[(4) - (5)]. expr ), NONBLOCK);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 533:

/* Line 1806 of yacc.c  */
#line 3108 "parser.y"
    {
    (yyval. stmt ) = new CompAssign((yyvsp[(1) - (9)]. expr ), (yyvsp[(5) - (9)]. expr ), (yyvsp[(7) - (9)]. evctrl ), (yyvsp[(8) - (9)]. expr ), NONBLOCK);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 534:

/* Line 1806 of yacc.c  */
#line 3113 "parser.y"
    {
    (yyval. stmt ) = new Wait((yyvsp[(3) - (5)]. expr ), (yyvsp[(5) - (5)]. stmt ));
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 535:

/* Line 1806 of yacc.c  */
#line 3118 "parser.y"
    {
    if((yyvsp[(3) - (5)]. exprs )) {
        (yyval. stmt ) = new TaskFuncCall((yyvsp[(1) - (5)]. text ), *((yyvsp[(3) - (5)]. exprs )), true);
        (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
	}
	else {
	    (yyval. stmt ) = 0;
	}	
}
    break;

  case 536:

/* Line 1806 of yacc.c  */
#line 3128 "parser.y"
    {
    (yyval. stmt ) = new TaskFuncCall((yyvsp[(1) - (2)]. text ), std::vector < Expr * >(), true);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 537:

/* Line 1806 of yacc.c  */
#line 3133 "parser.y"
    {
    (yyval. stmt ) = new TaskFuncCall((yyvsp[(1) - (5)]. text ), (yyvsp[(3) - (5)]. exprs ) ? *((yyvsp[(3) - (5)]. exprs )) : std::vector< Expr* >(), false);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 538:

/* Line 1806 of yacc.c  */
#line 3138 "parser.y"
    {
    (yyval. stmt ) = new TaskFuncCall((yyvsp[(1) - (2)]. text ), std::vector < Expr * >(), false);
    (yyval. stmt )->setScope(OMCreator::instance()->getCurrScope());
}
    break;

  case 539:

/* Line 1806 of yacc.c  */
#line 3143 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 540:

/* Line 1806 of yacc.c  */
#line 3149 "parser.y"
    {
    (yyval. stmts ) = (yyvsp[(1) - (2)]. stmts );
	if((yyvsp[(2) - (2)]. stmt )) {
        (yyval. stmts )->push_back((yyvsp[(2) - (2)]. stmt ));
	}	
}
    break;

  case 541:

/* Line 1806 of yacc.c  */
#line 3156 "parser.y"
    {
    (yyval. stmts ) = new std::vector < Stmt * >();
	if((yyvsp[(1) - (1)]. stmt )) {
        (yyval. stmts )->push_back((yyvsp[(1) - (1)]. stmt ));
	}	
}
    break;

  case 542:

/* Line 1806 of yacc.c  */
#line 3165 "parser.y"
    {
    (yyval. stmt ) = (yyvsp[(1) - (1)]. stmt );
}
    break;

  case 543:

/* Line 1806 of yacc.c  */
#line 3169 "parser.y"
    {
    (yyval. stmt ) = NULL;
}
    break;

  case 548:

/* Line 1806 of yacc.c  */
#line 3179 "parser.y"
    {
    (yyval. dirsigtyperange ) = (yyvsp[(3) - (3)]. dirsigtyperange );
}
    break;

  case 549:

/* Line 1806 of yacc.c  */
#line 3183 "parser.y"
    {
    (yyval. dirsigtyperange ) = (yyvsp[(1) - (4)]. dirsigtyperange );
    OMCreator::instance()->addTaskFuncPort((yyvsp[(3) - (4)]. text ), (yyval. dirsigtyperange )->first.first,
                           (yyval. dirsigtyperange )->second.second, (yyvsp[(4) - (4)]. ranges ),
                           (yyval. dirsigtyperange )->first.second,
                           (yyval. dirsigtyperange )->second.first);
}
    break;

  case 550:

/* Line 1806 of yacc.c  */
#line 3193 "parser.y"
    {
}
    break;

  case 552:

/* Line 1806 of yacc.c  */
#line 3198 "parser.y"
    {
}
    break;

  case 553:

/* Line 1806 of yacc.c  */
#line 3201 "parser.y"
    {
}
    break;

  case 554:

/* Line 1806 of yacc.c  */
#line 3206 "parser.y"
    {
}
    break;

  case 555:

/* Line 1806 of yacc.c  */
#line 3209 "parser.y"
    {
}
    break;

  case 556:

/* Line 1806 of yacc.c  */
#line 3214 "parser.y"
    {
    (yyval. BOOL ) = true;
}
    break;

  case 557:

/* Line 1806 of yacc.c  */
#line 3218 "parser.y"
    {
    (yyval. BOOL ) = false;
}
    break;

  case 558:

/* Line 1806 of yacc.c  */
#line 3224 "parser.y"
    {
}
    break;

  case 559:

/* Line 1806 of yacc.c  */
#line 3227 "parser.y"
    {
}
    break;

  case 562:

/* Line 1806 of yacc.c  */
#line 3234 "parser.y"
    {
    OMCreator::instance()->addTableEntry(new TableEntry(*((yyvsp[(1) - (4)]. VCHAR )), (yyvsp[(3) - (4)]. CHAR )));
    delete (yyvsp[(1) - (4)]. VCHAR );
}
    break;

  case 563:

/* Line 1806 of yacc.c  */
#line 3241 "parser.y"
    {
}
    break;

  case 564:

/* Line 1806 of yacc.c  */
#line 3244 "parser.y"
    {
}
    break;

  case 565:

/* Line 1806 of yacc.c  */
#line 3249 "parser.y"
    {
}
    break;

  case 566:

/* Line 1806 of yacc.c  */
#line 3252 "parser.y"
    {
}
    break;

  case 567:

/* Line 1806 of yacc.c  */
#line 3257 "parser.y"
    {
    OMCreator::instance()->addTableEntry(new TableEntry(*((yyvsp[(1) - (6)]. VCHAR )), (yyvsp[(5) - (6)]. CHAR ), (yyvsp[(3) - (6)]. CHAR )));
    delete (yyvsp[(1) - (6)]. VCHAR );
}
    break;

  case 568:

/* Line 1806 of yacc.c  */
#line 3264 "parser.y"
    {
    (yyval. expr ) = (yyvsp[(4) - (5)]. CONST );
}
    break;

  case 569:

/* Line 1806 of yacc.c  */
#line 3270 "parser.y"
    {
    (yyval. expr ) = (yyvsp[(1) - (1)]. expr );
}
    break;

  case 570:

/* Line 1806 of yacc.c  */
#line 3274 "parser.y"
    {
    (yyval. expr ) = NULL;
}
    break;

  case 571:

/* Line 1806 of yacc.c  */
#line 3280 "parser.y"
    {
    (yyval. VCHAR ) = new std::vector < char >();
    (yyval. VCHAR )->push_back((yyvsp[(1) - (1)]. CHAR ));
}
    break;

  case 572:

/* Line 1806 of yacc.c  */
#line 3285 "parser.y"
    {
    (yyval. VCHAR ) = (yyvsp[(1) - (2)]. VCHAR );
    (yyval. VCHAR )->push_back((yyvsp[(2) - (2)]. CHAR ));
}
    break;

  case 573:

/* Line 1806 of yacc.c  */
#line 3292 "parser.y"
    {
    (yyval. CHAR ) = '0';
}
    break;

  case 574:

/* Line 1806 of yacc.c  */
#line 3296 "parser.y"
    {
    (yyval. CHAR ) = '1';
}
    break;

  case 575:

/* Line 1806 of yacc.c  */
#line 3300 "parser.y"
    {
    (yyval. CHAR ) = 'x';
}
    break;

  case 576:

/* Line 1806 of yacc.c  */
#line 3304 "parser.y"
    {
    (yyval. CHAR ) = '?';
}
    break;

  case 577:

/* Line 1806 of yacc.c  */
#line 3308 "parser.y"
    {
    (yyval. CHAR ) = 'b';
}
    break;

  case 578:

/* Line 1806 of yacc.c  */
#line 3312 "parser.y"
    {
    (yyval. CHAR ) = '*';
}
    break;

  case 579:

/* Line 1806 of yacc.c  */
#line 3316 "parser.y"
    {
    (yyval. CHAR ) = '%';
}
    break;

  case 580:

/* Line 1806 of yacc.c  */
#line 3320 "parser.y"
    {
    (yyval. CHAR ) = 'f';
}
    break;

  case 581:

/* Line 1806 of yacc.c  */
#line 3324 "parser.y"
    {
    (yyval. CHAR ) = 'F';
}
    break;

  case 582:

/* Line 1806 of yacc.c  */
#line 3328 "parser.y"
    {
    (yyval. CHAR ) = 'l';
}
    break;

  case 583:

/* Line 1806 of yacc.c  */
#line 3332 "parser.y"
    {
    (yyval. CHAR ) = 'h';
}
    break;

  case 584:

/* Line 1806 of yacc.c  */
#line 3336 "parser.y"
    {
    (yyval. CHAR ) = 'B';
}
    break;

  case 585:

/* Line 1806 of yacc.c  */
#line 3340 "parser.y"
    {
    (yyval. CHAR ) = 'r';
}
    break;

  case 586:

/* Line 1806 of yacc.c  */
#line 3344 "parser.y"
    {
    (yyval. CHAR ) = 'R';
}
    break;

  case 587:

/* Line 1806 of yacc.c  */
#line 3348 "parser.y"
    {
    (yyval. CHAR ) = 'M';
}
    break;

  case 588:

/* Line 1806 of yacc.c  */
#line 3352 "parser.y"
    {
    (yyval. CHAR ) = 'n';
}
    break;

  case 589:

/* Line 1806 of yacc.c  */
#line 3356 "parser.y"
    {
    (yyval. CHAR ) = 'N';
}
    break;

  case 590:

/* Line 1806 of yacc.c  */
#line 3360 "parser.y"
    {
    (yyval. CHAR ) = 'p';
}
    break;

  case 591:

/* Line 1806 of yacc.c  */
#line 3364 "parser.y"
    {
    (yyval. CHAR ) = 'P';
}
    break;

  case 592:

/* Line 1806 of yacc.c  */
#line 3368 "parser.y"
    {
    (yyval. CHAR ) = 'Q';
}
    break;

  case 593:

/* Line 1806 of yacc.c  */
#line 3372 "parser.y"
    {
    (yyval. CHAR ) = 'q';
}
    break;

  case 594:

/* Line 1806 of yacc.c  */
#line 3376 "parser.y"
    {
    (yyval. CHAR ) = '-';
}
    break;

  case 595:

/* Line 1806 of yacc.c  */
#line 3380 "parser.y"
    {
    (yyval. CHAR ) = '+';
}
    break;

  case 596:

/* Line 1806 of yacc.c  */
#line 3386 "parser.y"
    {
    (yyval. CHAR ) = '0';
}
    break;

  case 597:

/* Line 1806 of yacc.c  */
#line 3390 "parser.y"
    {
    (yyval. CHAR ) = '1';
}
    break;

  case 598:

/* Line 1806 of yacc.c  */
#line 3394 "parser.y"
    {
    (yyval. CHAR ) = 'x';
}
    break;

  case 599:

/* Line 1806 of yacc.c  */
#line 3398 "parser.y"
    {
    (yyval. CHAR ) = '-';
}
    break;

  case 600:

/* Line 1806 of yacc.c  */
#line 3404 "parser.y"
    {
    if ((yyvsp[(2) - (3)]. vtext )) {
        (yyval. parseInfo ) = new ParseInfo("", IN, WIRE);
        const int size = (yyvsp[(2) - (3)]. vtext )->size();
        for (int i = 0; i < size; ++i) {
            (yyval. parseInfo )->ids.push_back((*((yyvsp[(2) - (3)]. vtext )))[i].first);
        }
    } else {
        (yyval. parseInfo ) = NULL;
    }

}
    break;

  case 601:

/* Line 1806 of yacc.c  */
#line 3417 "parser.y"
    {
    (yyval. parseInfo ) = new ParseInfo((yyvsp[(2) - (3)]. text ), OUT, WIRE);
}
    break;

  case 602:

/* Line 1806 of yacc.c  */
#line 3421 "parser.y"
    {
    (yyval. parseInfo ) = new ParseInfo((yyvsp[(2) - (3)]. text ), OUT, REG);
}
    break;

  case 603:

/* Line 1806 of yacc.c  */
#line 3425 "parser.y"
    {
    (yyval. parseInfo ) = new ParseInfo((yyvsp[(3) - (4)]. text ), OUT, REG);
}
    break;

  case 604:

/* Line 1806 of yacc.c  */
#line 3431 "parser.y"
    {
    (yyval. parseInfos ) = new std::vector < ParseInfo * >();
    (yyval. parseInfos )->push_back((yyvsp[(1) - (1)]. parseInfo ));
}
    break;

  case 605:

/* Line 1806 of yacc.c  */
#line 3436 "parser.y"
    {
    (yyval. parseInfos ) = (yyvsp[(1) - (2)]. parseInfos );
    (yyval. parseInfos )->push_back((yyvsp[(2) - (2)]. parseInfo ));
}
    break;

  case 606:

/* Line 1806 of yacc.c  */
#line 3443 "parser.y"
    {
    (yyval. ids ) = new std::vector < char *>();
    (yyval. ids )->push_back((yyvsp[(1) - (1)]. text ));
}
    break;

  case 607:

/* Line 1806 of yacc.c  */
#line 3448 "parser.y"
    {
    (yyval. ids ) = (yyvsp[(1) - (3)]. ids );
    (yyval. ids )->push_back((yyvsp[(3) - (3)]. text ));
}
    break;

  case 608:

/* Line 1806 of yacc.c  */
#line 3455 "parser.y"
    {
    (yyval. text ) = strdup("reg");
}
    break;

  case 609:

/* Line 1806 of yacc.c  */
#line 3459 "parser.y"
    {
    (yyval. text ) = NULL;
}
    break;

  case 610:

/* Line 1806 of yacc.c  */
#line 3465 "parser.y"
    {
    (yyval. expr ) = (yyvsp[(2) - (2)]. expr );
}
    break;

  case 611:

/* Line 1806 of yacc.c  */
#line 3469 "parser.y"
    {
    (yyval. expr ) = NULL;
}
    break;

  case 612:

/* Line 1806 of yacc.c  */
#line 3475 "parser.y"
    {
    (yyval. ids ) = new std::vector < char *>();
    (yyval. ids )->push_back((yyvsp[(2) - (2)]. text ));
}
    break;

  case 613:

/* Line 1806 of yacc.c  */
#line 3480 "parser.y"
    {
    (yyval. ids ) = (yyvsp[(1) - (4)]. ids );
    (yyval. ids )->push_back((yyvsp[(4) - (4)]. text ));
}
    break;

  case 614:

/* Line 1806 of yacc.c  */
#line 3488 "parser.y"
    {
    OMCreator::instance()->createUDP((yyvsp[(2) - (7)]. text ));
    OMCreator::instance()->createUDPPorts((yyvsp[(7) - (7)]. parseInfos ), (yyvsp[(4) - (7)]. ids ));
}
    break;

  case 615:

/* Line 1806 of yacc.c  */
#line 3493 "parser.y"
    {
    OMCreator::instance()->setUDPInitVal((yyvsp[(9) - (9)]. expr ));
}
    break;

  case 616:

/* Line 1806 of yacc.c  */
#line 3497 "parser.y"
    {
    OMCreator::instance()->scopeEndAction();
}
    break;

  case 617:

/* Line 1806 of yacc.c  */
#line 3504 "parser.y"
    {
    OMCreator::instance()->createUDP((yyvsp[(2) - (11)]. text ));
    OMCreator::instance()->createUDPPorts((yyvsp[(6) - (11)]. text ), (yyvsp[(7) - (11)]. expr ), (yyvsp[(9) - (11)]. ids ), (yyvsp[(5) - (11)]. text ) != 0);
}
    break;

  case 618:

/* Line 1806 of yacc.c  */
#line 3509 "parser.y"
    {
    OMCreator::instance()->scopeEndAction();
}
    break;

  case 619:

/* Line 1806 of yacc.c  */
#line 3511 "parser.y"
    {
	(yyval. udp ) = const_cast<UDP*>(OMCreator::instance()->getCurrUdp());
}
    break;



/* Line 1806 of yacc.c  */
#line 9981 "parser.tab.cc"
      default: break;
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
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
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
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



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
		      yytoken, &yylval);
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


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
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
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



