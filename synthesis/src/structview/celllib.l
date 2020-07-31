%{

#include <iostream>
#include <string>
#include <list>

#include "sport.h"
#include "scell.h"
#include "libraryreader.h"
using namespace stview;

#include "celllib.tab.hpp"
int Lineno = 0;
extern void yyerror(char const*);
struct TokenInfo {
	char	*text;
	int		line;
};	
TokenInfo tokenInfo = {0, 0};

%}

%x StringWithSpaces

WhiteSpace 		[ \t\b\f\r]+
Number			[0-9]+
AlphaUnderS		[a-zA-Z_]
AlphaNumUnderS	[a-zA-Z0-9_]

%%

{WhiteSpace} { }

\n	{++Lineno;}
"//".* { }

[lL][iI][bB][rR][aA][rR][yY] 			{ BEGIN StringWithSpaces; return LIBRARY;	}
[aA][uU][tT][hH][oO][rR]				{ BEGIN StringWithSpaces; return AUTHOR;	}
[vV][eE][rR][sS][iI][oO][nN]			{ BEGIN StringWithSpaces; return VERSION;	}
[cC][eE][lL][lL]						{ return CELL; 		}
[pP][iI][nN]							{ return PIN; 		}
[iI][nN][pP][uU][tT]					{ return IN; 		}
[oO][uU][tT][pP][uU][tT]				{ return OUT; 		}
[iI][nN][oO][uU][tT]					{ return BIDI; 		}
[bB][uU][fF]							{ return BUF; 		}
[tT][rR][iI][bB][uU][fF]				{ return TRIBUF;	}
[oO][rR]								{ return OR;		}
[aA][nN][dD]							{ return AND;		}
[nN][oO][rR]							{ return NOR;		}
[nN][aA][nN][dD]						{ return NAND;		}
[xX][oO][rR]							{ return XOR;		}
[xX][nN][oO][rR]						{ return XNOR;		}
[nN][oO][tT]							{ return NOT;		}
[aA][dD][dD]							{ return ADD;		}
[sS][uU][bB]							{ return SUB;		}
[mM][uU][lL]							{ return MUL;		}
[dD][iI][vV]							{ return DIV;		}
[fF][fF]								{ return FF;		}
[rR][fF][fF]							{ return RFF;		}
[sS][fF][fF]							{ return SFF;		}
[rR][sS][fF][fF]						{ return RSFF;		}
[sS][rR][fF][fF]						{ return SRFF;		}
[lL][dD]								{ return LD;		}
[rR][lL][dD]							{ return RLD;		}
[mM][uU][xX]							{ return MUX;		}
[eE][nN][cC]							{ return ENC;		}
[dD][eE][cC]							{ return DEC;		}
[vV][cC][cC]							{ return VCC;		}
[gG][nN][dD]							{ return GND;		}
[aA][rR][eE][aA]						{ return AREA;		}
[dD][eE][lL][aA][yY]					{ return DELAY;		}
[aA][tT][tT][rR][iI][bB][uU][tT][eE]	{ return ATTRIBUTE; }
[lL][sS][hH]							{ return LSH;		}
[rR][sS][hH]							{ return RSH;		}
[cC][mM][pP]							{ return CMP;		}
[fF][uU][nN][cC][tT][iI][oO][nN]		{ return FUNCTION;	}
[Ii][Nn][Cc]							{ return INC;		}

{AlphaUnderS}{AlphaNumUnderS}* {
		CellLiblval.TEXT = strdup(yytext);
		return IDENTIFIER;
	}	
{Number}	{ CellLiblval.INT = atoi(yytext); return NUMBER; }
{Number}\.[0-9]+ { CellLiblval.REAL = atof(yytext); return REAL_NUMBER; }

<StringWithSpaces>: { return ':'; }
<StringWithSpaces>[^:\n]+ {
		CellLiblval.TEXT = strdup(yytext); 
		BEGIN 0;
		return STRING;
	}
.	{ return yytext[0]; }	

%%

int yywrap() {
	return 1;
}	
