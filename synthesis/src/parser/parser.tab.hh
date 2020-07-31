#include "parsehelp.h"
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

/* Line 2068 of yacc.c  */
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



/* Line 2068 of yacc.c  */
#line 273 "parser.tab.hh"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


