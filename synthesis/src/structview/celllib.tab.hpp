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
     LIBRARY = 258,
     AUTHOR = 259,
     VERSION = 260,
     CELL = 261,
     PIN = 262,
     IN = 263,
     OUT = 264,
     BIDI = 265,
     FF = 266,
     RFF = 267,
     SFF = 268,
     RSFF = 269,
     SRFF = 270,
     CMP = 271,
     BUF = 272,
     TRIBUF = 273,
     OR = 274,
     AND = 275,
     NOR = 276,
     NAND = 277,
     XOR = 278,
     XNOR = 279,
     NOT = 280,
     ADD = 281,
     SUB = 282,
     MUL = 283,
     LSH = 284,
     FUNCTION = 285,
     DIV = 286,
     MUX = 287,
     ENC = 288,
     DEC = 289,
     VCC = 290,
     GND = 291,
     AREA = 292,
     DELAY = 293,
     ATTRIBUTE = 294,
     LD = 295,
     RLD = 296,
     RSH = 297,
     INC = 298,
     STRING = 299,
     IDENTIFIER = 300,
     NUMBER = 301,
     REAL_NUMBER = 302
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 32 "celllib.y"

	char				*TEXT;
	int					INT;
	float				REAL;
	std::pair<int, int>	*INTPAIR;
	std::list< SPort* >	*PORTS;
	std::list< SCell* >	*SCELLS;
	SCell				*SCELL;
	SPort::Dir			PORTTYPE;
	SCell::CellType		CELLTYPE;
	Attribute*			ATTRIB;



/* Line 2068 of yacc.c  */
#line 112 "celllib.tab.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE CellLiblval;


